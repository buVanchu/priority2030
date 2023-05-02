#include<iostream>
#include "server.h"

int main()
{
    short port_esp, port_client;
	std::cin >> port_esp >> port_client;

	boost::asio::io_service io;
	Server server(io, port_esp, port_client);

    server.run();

    return EXIT_SUCCESS;
}