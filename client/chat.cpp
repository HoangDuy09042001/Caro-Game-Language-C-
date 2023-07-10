// chat.cpp
#include "chat.hpp"
#include <iostream>
#include <boost/array.hpp>
#include <sstream>
#include <thread>
#include <atomic>
#include <string>
#include <vector>

std::atomic<bool> chatActive(true);
char board[12][12] = {
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'}
};
std::vector<int> splitStringToInt(const std::string& str, char delimiter) {
    std::vector<int> result;
    std::stringstream ss(str);
    std::string token;
  
    while (std::getline(ss, token, delimiter)) {
        result.push_back(std::stoi(token));
    }

    return result;
}
void printBoard(const char board[12][12]) {
    for (int row = 0; row < 12; ++row) {
        for (int col = 0; col < 12; ++col) {
            std::cout << board[row][col] << ' ';
        }
        std::cout << '\n';
    }
}
void sendChatMessage(boost::asio::ip::tcp::socket& socket) {
        char delimiter=',';
        std::string step;
        std::cout << "Enter step: ";
        std::cin >> step;
        std::vector<int> numbers = splitStringToInt(step, delimiter);
        board[numbers[0]][numbers[1]] = 'X';
        printBoard(board);
        std::ostringstream oss;
        oss << "STEP " << step;
        std::string stepInfo = oss.str();
        boost::asio::write(socket, boost::asio::buffer(stepInfo));
}

// void receiveServerResponse(boost::asio::ip::tcp::socket& socket) {
//     boost::asio::streambuf response;
//     boost::system::error_code error;

//     while (true) {
//         // Clear the buffer before reading
//         response.consume(response.size());

//         // Read server response
//         boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error);

//         if (!error) {
//             // std::cout <<"response: "<<&response;
//             boost::asio::streambuf::const_buffers_type bufs = response.data();
//             std::string responseData(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));
//             std::cout <<"\nresponseData: " <<responseData;
//             if (responseData.find("@")!= std::string::npos) {
//                 std::cout<<"A loi\n";
//                 break;
//             }

//         } else {
//             std::cerr << "Error receiving server response: " << error.message() << std::endl;
//             break; // Exit the loop on error
//         }
//     }
// }

// void chat(boost::asio::ip::tcp::socket& socket) {
//     try {
//         std::thread receiveThread(receiveServerResponse, std::ref(socket));

//         while (true) {
//             sendChatMessage(socket);
//         }

//         receiveThread.join();
//     }
//     catch (std::exception& e) {
//         std::cerr << e.what() << std::endl;
//     }
// }

void receiveServerResponse(boost::asio::ip::tcp::socket& socket) {
    boost::asio::streambuf response;
    boost::system::error_code error;

    while (chatActive) {
        // Clear the buffer before reading
        response.consume(response.size());

        // Read server response
        boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error);

        if (!error) {
            char delimiter = ',';
            boost::asio::streambuf::const_buffers_type bufs = response.data();
            std::string responseData(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));
            // std::cout << "\nresponseData: " << responseData;
            std::vector<int> numbers = splitStringToInt(responseData, delimiter);

            for (const auto& number : numbers) {
                std::cout << number << std::endl;
            }
            board[numbers[0]][numbers[1]]='O';
            printBoard(board);
            if (responseData.find("@") != std::string::npos) {
                std::cout << "Chat ended." << std::endl;
                chatActive = false;
            }
        } else {
            std::cerr << "Error receiving server response: " << error.message() << std::endl;
            break; // Exit the loop on error
        }
    }
}

void chat(boost::asio::ip::tcp::socket& socket) {
    try {
        board[5][5] = 'X';
        board[6][6] = 'O';
        printBoard(board);
        std::thread receiveThread(receiveServerResponse, std::ref(socket));
        while (chatActive) {
            sendChatMessage(socket);
        }

        receiveThread.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}


