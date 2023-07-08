// // logout.cpp
// #include "logout.hpp"
// #include <iostream>
// #include <boost/array.hpp>
// #include <sstream>

// void logout(boost::asio::ip::tcp::socket& socket) {
//     try {
//         // Get logout info from user
//         std::string username;
//         std::cout << "Enter username to logout: ";
//         std::cin >> username;

//         // Send logout info to server
//         std::ostringstream oss;
//         oss << "LOGOUT " << username << "\n";
//         std::string logoutInfo = oss.str();

//         boost::asio::write(socket, boost::asio::buffer(logoutInfo));

//         // Read server response
//         for (;;) {
//             boost::array<char, 128> buf;
//             boost::system::error_code error;

//             size_t len = socket.read_some(boost::asio::buffer(buf), error);

//             if (error == boost::asio::error::eof)
//                 break;
//             else if (error)
//                 throw boost::system::system_error(error);

//             std::cout.write(buf.data(), len);
//         }
//     }
//     catch (std::exception& e) {
//         std::cerr << e.what() << std::endl;
//     }
// }


// logout.cpp
#include "logout.hpp"
#include <iostream>
#include <boost/array.hpp>
#include <sstream>
#include <boost/asio.hpp>

void logout(boost::asio::ip::tcp::socket& socket) {
    try {
        // Get logout info from user
        std::string username;
        std::cout << "Enter username to logout: ";
        std::cin >> username;

        // Send logout info to server
        std::ostringstream oss;
        oss << "LOGOUT " << username << "\n";
        std::string logoutInfo = oss.str();

        boost::asio::write(socket, boost::asio::buffer(logoutInfo));

        // Read server response
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\n");
        std::cout << &response;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
