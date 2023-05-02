#ifndef ESP_H
#define ESP_H

#include <iostream>
#include <thread>
#include "boost/asio.hpp"
#include "socket_manager.h"
#include "parser.h"

class Esp : private SocketManager
{
private:

public:
    Esp(boost::asio::io_service &io_service);
    boost::asio::ip::tcp::socket& get_address();

    std::pair<bool, std::string> authorization();

    void write(const std::string& request);
    std::string read();
};

#endif /*ESP_H*/