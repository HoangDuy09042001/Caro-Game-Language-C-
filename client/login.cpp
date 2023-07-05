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
        // std::string loginInfo = username + " " + password + "\n";
        std::ostringstream oss;
        oss << "LOGIN " << username << " " << password << "\n";
        std::string loginInfo = oss.str();

        boost::asio::write(socket, boost::asio::buffer(loginInfo));

        // Read server response
        for (;;) {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break;
            else if (error)
                throw boost::system::system_error(error);

            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

