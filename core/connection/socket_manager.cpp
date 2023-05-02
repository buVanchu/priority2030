#include "socket_manager.h"

std::string END_POINT = "\n";

SocketManager::SocketManager(boost::asio::io_service &io_service) : socket_(io_service) {}

boost::asio::ip::tcp::socket& SocketManager::get_socket()
{
		return socket_;
}

bool SocketManager::isConnected() const
{
	return socket_status_.load(std::memory_order_relaxed);
}

void SocketManager::close_connection()
{	
	socket_status_.store(false, std::memory_order_relaxed);
	socket_.close();
}

std::string SocketManager::read()
{	
	try
	{
		boost::asio::read_until(
            socket_,
            buffer_,
            END_POINT
		);

		std::ostringstream ss;
		ss<<&buffer_;
		std::string ss_out = ss.str();
		auto poz = ss_out.find(END_POINT);

		return ss_out.substr(0, poz);
	}
	catch(const std::exception& e)
	{
		socket_status_.store(false, std::memory_order_relaxed);
		return END_POINT;
	}
}

void SocketManager::write(const std::string& message)
{
	try
	{
		boost::asio::write(
				socket_,
				boost::asio::buffer(message)
	);
	}
	catch(const std::exception& e)
	{
		socket_status_.store(false, std::memory_order_relaxed);
	}
}