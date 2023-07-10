#ifndef CHAT_HPP
#define CHAT_HPP

#include <iostream>
#include <boost/asio.hpp>

void chat(boost::asio::ip::tcp::socket& socket);

#endif  // CHAT_HPP
