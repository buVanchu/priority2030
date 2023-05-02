#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <iostream>
#include <thread>
#include "boost/asio.hpp"
#include "session.h"

class Server
{
private:
    boost::asio::io_service &io_service_;
    boost::asio::ip::tcp::acceptor esp_acceptor_;
	boost::asio::ip::tcp::acceptor client_acceptor_;
	std::vector<std::shared_ptr<Session>> sessions_;

    void start_accept_esp_();
    void stop_accept_esp_();

    void start_accept_client_();
    void stop_accept_client_();

    void make_session_();
    void delete_session_();

    void start_session_monitoring_();
    void stop_session_monitoring_();

    std::atomic<bool> stopped_accept_esp_{false};
    std::atomic<bool> stopped_accept_client_{false};
    std::atomic<bool> stopped_session_monitoring_{false};

public:
    Server(boost::asio::io_service &io_service, short esp_acceptor_port, short client_acceptor_port);
    
    void run();
    void stop();
};

#endif /*SERVER_H*/