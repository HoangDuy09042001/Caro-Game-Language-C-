// login.cpp
#include "login.hpp"
#include <iostream>
#include <boost/array.hpp>
#include <sstream>

void login(boost::asio::ip::tcp::socket& socket) {
    try {
        // Get login info from user
        std::string username;
        std::string password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        // Send login info to server
        std::ostringstream oss;
        oss << "LOGIN " << username << " " << password;
        std::string loginInfo = oss.str();

        boost::asio::write(socket, boost::asio::buffer(loginInfo));

        // Read server response
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\n");

        // Print response
        std::cout << &response;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
