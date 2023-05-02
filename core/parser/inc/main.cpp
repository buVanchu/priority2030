#include "json.hpp"
#include<iostream>

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

    for(size_t i = 1; i < 4; i++)
    {
        auto relay = json_body[std::to_string(i)];
        out_data.relay[i - 1].voltage = (double)relay[0];
        out_data.relay[i - 1].current = (double)relay[1];
        out_data.relay[i - 1].status = (size_t)relay[2];
    }

    return out_data;
}

// {"type": 3, "data": {"solar": [12, 0.124, 1], "wind": [42, 0.0012, 1], "gen": [0.54, 12, 0], "bat": [123], "1": [3, 4, 1], "2": [5, 6, 0], "3": [7, 8, 1]}}

int main()
{
    std::string str = R"({"type": 3, "data": {"solar": [12, 0.124, 1], "wind": [42, 0.0012, 1], "gen": [0.54, 12, 0], "bat": [123], "1": [3, 4, 1], "2": [5, 6, 0], "3": [7, 8, 1]}})";

    auto data = parse_data_esp32_from_body(str);

    std::cout << data.solar.voltage<< " " << data.solar.current<< " " << data.solar.status<< " " << std::endl;

    std::cout << data.wind.voltage<< " " << data.wind.current<< " " << data.wind.status<< " " << std::endl;

    std::cout << data.gen.voltage<< " " << data.gen.current<< " " << data.gen.status<< " " << std::endl;

    std::cout << data.buttery << std::endl;

    for(size_t i = 1; i < 4; i++)
    {
        std::cout << data.relay[i-1].voltage << " " << data.relay[i-1].current << " " << data.relay[i-1].status << std::endl;
    }
    
    return EXIT_SUCCESS;
}