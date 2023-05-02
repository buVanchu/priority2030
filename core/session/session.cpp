#include "session.h"

Session::Session(const std::string& name_session)
    : name_session_(name_session), client_connection(nullptr), esp_connection(nullptr) {}


void Session::start_creating_data_request_()
{   
    while((!this->stopped_creating_data_request_.load(std::memory_order_relaxed)))
    {   
        this->requests_queue_.push_message(SENSORS_DATA, "");
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void Session::stop_creating_data_request_()
{
    this->stopped_creating_data_request_.store(true, std::memory_order_relaxed);
}

void Session::start_getting_request_()
{
    while((!this->stopped_getting_request_.load(std::memory_order_relaxed)))
    {
        auto [request_type, request_text] = parse_from_json(client_connection->read());
        requests_queue_.push_message(request_type, request_text);
    }
}

void Session::stop_getting_request_()
{
    this->stopped_getting_request_.store(true, std::memory_order_relaxed);
}

bool Session::get_data_request_() // надо как-то поумному сделать
{
    esp_connection->write(make_data_request());
    auto answer = esp_connection->read();

    auto [request_type, request_text] = parse_from_json(answer);

    if(request_type == SENSORS_DATA)
    {
        auto data = parse_data_esp32_from_body(request_text);
        client_connection->write(make_data_answer(data));
    }

    return false;
}

bool Session::get_status_reley_request_(const std::string& body) // надо как-то поумному сделать
{
    esp_connection->write(make_status_reley_request(body));
    auto data_answer = esp_connection->read();
    client_connection->write(data_answer);

    return true;
}

void Session::start_processing_request_()
{
    while((!this->stopped_processing_request_.load(std::memory_order_relaxed)))
    {
        if(!requests_queue_.isEmpty())
        {
            auto [request_type, request_text] = requests_queue_.pop_message();

            switch (request_type)
            {
            case RequestType(SENSORS_DATA):
                this->get_data_request_();
                break;
            case RequestType(STATUS_RELEY):
                this->get_status_reley_request_(request_text);
                break;
            default:
                break;
            }
            // switch (request_type)
            // {
            // case DATA:
            // std::cout << 
            //     // std::cout << make_request(DATA, request_text) << std::endl;
            //     // esp_connection->write(make_request(DATA, request_text));
            //     // client_connection->write(esp_connection->read());
            //     // break;
            
            // default:
            //     break;
            // }
        }
    }
}

void Session::stop_processing_request_()
{
    while(!requests_queue_.isEmpty()) {}
    this->stopped_processing_request_.store(true, std::memory_order_relaxed);
}

void Session::run_session()
{
    std::cout << "RUN " << this->name_session_ << " SESSION" << std::endl;

    this->stopped_session_.store(false, std::memory_order_relaxed);

    while(!(this->stopped_session_.load(std::memory_order_relaxed)))
    {           
        if(this->stopped_creating_data_request_.load(std::memory_order_relaxed))
        {
            this->stopped_creating_data_request_.store(false, std::memory_order_relaxed);

            std::thread scdr_th(&Session::start_creating_data_request_, this);
            scdr_th.detach();
        }

        if((this->stopped_getting_request_.load(std::memory_order_relaxed)) && (client_connection != nullptr))
        {
            this->stopped_getting_request_.store(false, std::memory_order_relaxed);

            std::thread sgr_th(&Session::start_getting_request_, this);
            sgr_th.detach();
        }

        if((this->stopped_processing_request_.load(std::memory_order_relaxed)) && (client_connection != nullptr))
        {
            this->stopped_processing_request_.store(false, std::memory_order_relaxed);

            std::thread spr_th(&Session::start_processing_request_, this);
            spr_th.detach();
        }
    }

    this->stop_creating_data_request_();
    this->stop_getting_request_();
    this->stop_processing_request_();
}

void Session::stop_session()
{
    this->stopped_session_.store(true, std::memory_order_relaxed);
}

std::string Session::get_session_name() const
{
    return this->name_session_;
}
