#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <map>
#include <thread>
#include <typeinfo>
struct AccountInfo {
    std::string password;
    bool status;
};
std::map<int, boost::asio::ip::tcp::socket*> clients;
std::vector<std::string> chatMessages;
std::map<std::string, AccountInfo> loadAccounts(const std::string& filename) {
    std::map<std::string, AccountInfo> accounts;
    std::ifstream file(filename);
    std::string username;
    std::string password;
    bool status;

    while (file >> username >> password >> status) {
        accounts[username] = {password, status};
    }

    return accounts;
}

int generateClientId() {
    static int nextId = 0;
    nextId++;
    return nextId;
}
void saveAccounts(const std::string& filename, const std::map<std::string, AccountInfo>& accounts) {
    std::ofstream file(filename);

    for (const auto& account : accounts) {
        file << account.first << " " << account.second.password << " " << account.second.status << "\n";
    }
}

void handleLogin(const std::string& username, const std::string& password, std::map<std::string, AccountInfo>& accounts, boost::asio::ip::tcp::socket& socket) {
    std::string message;
    if (accounts.count(username) > 0 && accounts[username].password == password) {
        if (accounts[username].status == 0) { // Only change status if account is not locked
            accounts[username].status = 1;
            message = "Login successful\n";
        } else {
            message = "Account is locked\n";
        }
    } else {
        message = "Login failed\n";
    }

    boost::asio::write(socket, boost::asio::buffer(message));
    saveAccounts("../accounts.txt", accounts);
}
void handleChat(std::string& message,std::map<std::string, AccountInfo>& accounts, boost::asio::ip::tcp::socket& socket) {
    for (const auto& client : clients) {
        if (client.second != &socket) {
           boost::asio::write(*(client.second), boost::asio::buffer(message+"\n"));

        }
    }
}


void handleLogout(const std::string& username, std::map<std::string, AccountInfo>& accounts, boost::asio::ip::tcp::socket& socket) {
    std::string message;
    if (accounts.count(username) > 0) {
        if (accounts[username].status == 1) { // Only change status if account is logged in
            accounts[username].status = 0;
            message = "Logout successful\n";
        } else {
            message = "Account is already logged out\n";
        }
    } else {
        message = "Account not found\n";
    }

    boost::asio::write(socket, boost::asio::buffer(message));
    saveAccounts("../accounts.txt", accounts);
}

void handleNewGame(std::map<std::string, AccountInfo>& accounts, boost::asio::ip::tcp::socket& socket) {
    static int gameID = 0;  // Keep track of game IDs

    // Send game ID to player
    std::ostringstream oss;
    oss << "GAME " << gameID << "\n";
    std::string gameInfo = oss.str();

    boost::asio::write(socket, boost::asio::buffer(gameInfo));

    gameID++;  // Increase game ID for next game
}
void handleClient(boost::asio::ip::tcp::socket socket, std::map<std::string, AccountInfo>& accounts) {
    // Lấy một ID duy nhất cho client
    int clientId = generateClientId();

    // Thêm thông tin về client vào danh sách clients
    clients[clientId] = &socket;

    boost::system::error_code error;
    boost::array<char, 128> buf;

    while (true) {
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        std::string received(buf.data(), len);
        std::istringstream iss(received);

        std::string header;
        std::string content;
        iss >> header;
        std::cout<<header<<"\n";
        getline(iss, content); // Read the rest of the line
        if (header == "LOGIN") {
            std::string username;
            std::string password;
            // Remove leading spaces from content
            content.erase(0, content.find_first_not_of(" \n\r\t"));

            // Now split the content into username and password
            std::istringstream contentStream(content);
            contentStream >> username >> password;
            handleLogin(username, password, accounts, socket);
        } else if (header == "LOGOUT") {
            std::string username;
            // Remove leading spaces from content
            content.erase(0, content.find_first_not_of(" \n\r\t"));

            // The rest of the content is the username
            username = content;

            handleLogout(username, accounts, socket);
        } else if (header == "CREATE_GAME") {
            handleNewGame(accounts, socket);
        } else if (header == "STEP") {
            std::string message;
            // Remove leading spaces from content
            content.erase(0, content.find_first_not_of(" \n\r\t"));
        
            // The rest of the content is the chat message
            message = content;
            handleChat(message, accounts, socket);
        } else if (header == "EXIT") {
            break; // Close the connection if the client sends an "EXIT" message
        }
    }
}


int main() {
    try {
        boost::asio::io_service io_service;
        std::map<std::string, AccountInfo> accounts = loadAccounts("../accounts.txt");
        boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234));

        for (;;) {
            boost::asio::ip::tcp::socket socket(io_service);
            acceptor.accept(socket);
            
            std::thread clientThread(handleClient, std::move(socket), std::ref(accounts));
            clientThread.detach();
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
