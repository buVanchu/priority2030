#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <thread>
#include "boost/asio.hpp"
#include "socket_manager.h"
#include "parser.h"

class Client : private SocketManager
{
private:

public:
    Client(boost::asio::io_service &io_service);
    boost::asio::ip::tcp::socket& get_address();

    std::pair<bool, std::string> authorization();
    std::string read();
    void write(const std::string& message);

};

#endif /*CLIENT_H*/