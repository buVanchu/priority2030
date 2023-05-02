#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <thread>
#include "esp.h"
#include "client.h"
#include "requests_queue.h"
#include "parser.h"

class Session
{
private:
    std::string name_session_;

    void start_creating_data_request_();
    void stop_creating_data_request_();
    std::atomic<bool> stopped_creating_data_request_{true};

    void start_getting_request_();
    void stop_getting_request_();
    std::atomic<bool> stopped_getting_request_{true};

    void start_processing_request_();
    void stop_processing_request_();
    std::atomic<bool> stopped_processing_request_{true};

    SmartRequestsQueue requests_queue_;

    bool get_data_request_();
    bool get_status_reley_request_(const std::string& body);

public:
    std::shared_ptr<Esp> esp_connection;
	std::shared_ptr<Client> client_connection;

    Session(const std::string& name_session);

    void run_session();
    void stop_session();

    std::string get_session_name() const;
    std::atomic<bool> stopped_session_{true};

};

#endif /*SESSION_H*/