#include "parser.h"

std::string get_body_from_request(const std::string& request)
{
    nlohmann::json json_request = nlohmann::json::parse(request);
    return json_request["data"].dump();
}

RequestType get_type_from_request(const std::string& request)
{
    nlohmann::json json_request = nlohmann::json::parse(request);
    size_t type = json_request["type"];
    return RequestType(type);
}

std::pair<RequestType, std::string> parse_from_json(const std::string& request)
{   
    return {get_type_from_request(request), get_body_from_request(request)};
}

std::pair<std::string, std::string> parse_auth_from_body(std::string body)
{
    nlohmann::json json_body = nlohmann::json::parse(body);
    return {(std::string)json_body["login"], (std::string)json_body["password"]};
}


void make_body_to_request(nlohmann::json body, nlohmann::json& request)
{
    request["data"] = body;
}

void make_type_to_request(RequestType type, nlohmann::json& request)
{
    request["type"] = (size_t)type;
}


std::string make_auth_answer(const std::string& login, const size_t& status)
{
    nlohmann::json json_body;
    json_body["login"] = login;
    json_body["status"] = status;

    nlohmann::json json_out;
    make_body_to_request(json_body, json_out);
    make_type_to_request(RequestType(AUTH_ANSWER), json_out);

    return json_out.dump();
}

std::string make_data_request()
{
    nlohmann::json json_out;
    make_type_to_request(RequestType(SENSORS_DATA), json_out);

    return json_out.dump();
}

std::string make_status_reley_request(const std::string& body)
{
    nlohmann::json json_out;
    make_type_to_request(RequestType(STATUS_RELEY), json_out);
    make_body_to_request(nlohmann::json::parse(body), json_out);

    return json_out.dump();
}

DataFromESP32 parse_data_esp32_from_body(const std::string& body)
{
    DataFromESP32 out_data;

    nlohmann::json json_body = nlohmann::json::parse(body);

    auto solar = json_body["solar"];
    out_data.solar.voltage = (double)solar[0];
    out_data.solar.current = (double)solar[1];
    out_data.solar.status = (size_t)solar[2];

    auto wind = json_body["wind"];
    out_data.wind.voltage = (double)wind[0];
    out_data.wind.current = (double)wind[1];
    out_data.wind.status = (size_t)wind[2];

    auto gen = json_body["gen"];
    out_data.gen.voltage = (double)gen[0];
    out_data.gen.current = (double)gen[1];
    out_data.gen.status = (size_t)gen[2];

    out_data.buttery = (double)json_body["bat"][0];

    for(size_t relay_number = 1; relay_number < 4; relay_number++)
    {
        auto relay = json_body[std::to_string(relay_number)];
        out_data.relay[relay_number - 1].voltage = (double)relay[0];
        out_data.relay[relay_number - 1].current = (double)relay[1];
        out_data.relay[relay_number - 1].status = (size_t)relay[2];
    }

    return out_data;
}

std::string make_data_answer(const DataFromESP32& data)
{
    nlohmann::json json_body;
    json_body["solar"] = nlohmann::json::array({data.solar.voltage, data.solar.current, data.solar.voltage * data.solar.current, 37, data.solar.status});
    json_body["wind"] = nlohmann::json::array({data.wind.voltage, data.wind.current, data.wind.voltage * data.wind.current, 52, data.wind.status});
    json_body["gen"] = nlohmann::json::array({data.gen.voltage, data.gen.current, data.gen.voltage * data.gen.current, 32, data.gen.status});

    json_body["bat"] = nlohmann::json::array({data.buttery, 1});

    for(size_t relay_number = 1; relay_number < 4; relay_number++)
    {
        json_body[std::to_string(relay_number)] = nlohmann::json::array({data.relay[relay_number - 1].voltage, data.relay[relay_number - 1].current, data.relay[relay_number - 1].voltage * data.relay[relay_number - 1].current, 60, data.relay[relay_number - 1].status});
    }

    nlohmann::json json_out;

    make_type_to_request(RequestType(SENSORS_DATA), json_out);
    make_body_to_request(json_body, json_out);

    return json_out.dump();  
}