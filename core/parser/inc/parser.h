#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "json.hpp"


enum RequestType
{
    ERROR = 0,
    AUTH,
    AUTH_ANSWER,
    ESP_STATUS,
    STATUS_RELEY,
    SENSORS_DATA = 6,
    SHUTDOWN = 7
};


std::string get_body_from_request(const std::string& request);
RequestType get_type_from_request(const std::string& request);
std::pair<RequestType, std::string> parse_from_json(const std::string& request);

std::pair<std::string, std::string> parse_auth_from_body(std::string body);


void make_body_to_request(nlohmann::json body, nlohmann::json& request);
void make_type_to_request(RequestType type, nlohmann::json& request);

std::string make_auth_answer(const std::string& login, const size_t& status);


std::string make_data_request();
std::string make_status_reley_request(const std::string& body);

struct Data
{
    double voltage;
    double current;
    size_t status;
};

struct DataFromESP32
{
    Data solar;
    Data wind;
    Data gen;

    double buttery;

    Data relay[3];
};

DataFromESP32 parse_data_esp32_from_body(const std::string& body);
std::string make_data_answer(const DataFromESP32& data);

#endif /*PARSER_H*/