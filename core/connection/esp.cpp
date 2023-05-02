#include "esp.h"

Esp::Esp(boost::asio::io_service &io_service) : SocketManager(io_service) {}

std::pair<bool, std::string> Esp::authorization()
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

boost::asio::ip::tcp::socket& Esp::get_address()
{
    return SocketManager::get_socket();
}

void Esp::write(const std::string& request)
{
    SocketManager::write(request);
}

std::string Esp::read()
{
    return SocketManager::read();
}