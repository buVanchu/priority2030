#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <iostream>
#include "boost/asio.hpp"

class SocketManager
{
private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer_;
	std::atomic<bool> socket_status_ {true};

public:
    SocketManager(boost::asio::io_service &io_service);
	boost::asio::ip::tcp::socket& get_socket();
	bool isConnected() const;
	void close_connection();

    std::string read();
    void write(const std::string& message);
};

#endif /*SOCKET_MANAGER_H*/