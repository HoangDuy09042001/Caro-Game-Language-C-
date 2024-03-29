#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "ascii_art.hpp"
#include "login.hpp"
#include "logout.hpp"
#include "caro.hpp"
#include "chat.hpp"

int main() {
    printAsciiArt();
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query("localhost", "1234");
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    while (true) {
        std::cout << "==========================\n";
        std::cout << " 1. Login\n";
        std::cout << " 2. Logout\n";
        std::cout << " 3. Exit\n";
        std::cout << " 4. Caro\n";
        std::cout << " 5. Chat\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                login(socket);
                break;
            case 2:
                logout(socket);
                break;
            case 4:
                create_game(socket);
                break;
            case 5:
                chat(socket);
                break;
            case 3:
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

