#include "client.h"

Client::Client(boost::asio::io_service &io_service) : SocketManager(io_service) {}

std::pair<bool, std::string> Client::authorization()
{
    std::string authorization_request = SocketManager::read();
    auto [request_type, request_body] = parse_from_json(authorization_request);

    if(request_type == RequestType(AUTH))
    {   
        auto [login, password] = parse_auth_from_body(request_body);
        if(login=="priority2030")
        {
            auto authorization_answer = make_auth_answer(login, 0);
            SocketManager::write(authorization_answer);
            return {true, login};
        }
        else
            return {false, login};
    }

    return {false, "FUCK"};
}


boost::asio::ip::tcp::socket& Client::get_address()
{
    return SocketManager::get_socket();
}

std::string Client::read()
{
    return SocketManager::read();
}

void Client::write(const std::string& message)
{
    SocketManager::write(message);
}