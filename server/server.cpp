#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <map>

struct AccountInfo {
    std::string password;
    bool status;
};

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

void saveAccounts(const std::string& filename, const std::map<std::string, AccountInfo>& accounts) {
    std::ofstream file(filename);

    for (const auto& account : accounts) {
        file << account.first << " " << account.second.password << " " << account.second.status << "\n";
    }
}

void handleLogin(const std::string& username, const std::string& password, std::map<std::string, AccountInfo>& accounts, boost::asio::ip::tcp::socket& socket) {
    std::string message;
    if (accounts.count(username) > 0 && accounts[username].password == password) {
        std::cout << "Debug: " << username << " " 
            << password << " " <<accounts[username].status<< " "
            << (accounts[username].status == 0) << std::endl;

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

int main() {
    try {
        boost::asio::io_service io_service;
        std::map<std::string, AccountInfo> accounts = loadAccounts("../accounts.txt");
        for (const auto& account : accounts) {
                std::cout << "Username: " << account.first 
                        << ", Password: " << account.second.password 
                        << ", Status: " << account.second.status << std::endl;
            }

        boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234));

        for (;;) {
            boost::asio::ip::tcp::socket socket(io_service);
            acceptor.accept(socket);
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break;
            else if (error)
                throw boost::system::system_error(error);            
            std::string received(buf.data(), len);
            std::istringstream iss(received);

            std::string header;
            std::string content;
            iss >> header;
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
            }else if (header == "LOGOUT") {
                std::string username;
                // Remove leading spaces from content
                content.erase(0, content.find_first_not_of(" \n\r\t"));

                // The rest of the content is the username
                username = content;

                handleLogout(username, accounts, socket);
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


