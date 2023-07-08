// caro.cpp
#include "caro.hpp"
#include <iostream>
#include <boost/array.hpp>
#include <sstream>

void create_game(boost::asio::ip::tcp::socket& socket) {
    try {
        // Send game creation request to server
        std::ostringstream oss;
        oss << "CREATE_GAME\n";
        std::string gameRequest = oss.str();

        boost::asio::write(socket, boost::asio::buffer(gameRequest));

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
