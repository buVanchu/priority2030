#include "server.h"

Server::Server(boost::asio::io_service &io_service, short esp_acceptor_port, short client_acceptor_port)
	: io_service_(io_service),
	esp_acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), esp_acceptor_port)),
	client_acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), client_acceptor_port)) {}

void Server::start_accept_esp_()
{
    std::cout << "START ACCEPT ESP" << std::endl;

    while((!this->stopped_accept_esp_.load(std::memory_order_relaxed)))
    {
        std::shared_ptr<Esp> new_esp(new Esp(io_service_));
        esp_acceptor_.accept(new_esp->get_address());

        std::pair<bool, std::string> answer_authorization = new_esp->authorization();

        if(answer_authorization.first)
        {   
            auto result_session = std::find_if(std::begin(sessions_), std::end(sessions_), [answer_authorization](std::shared_ptr<Session> session)
																		{
																			return session->get_session_name() == answer_authorization.second; 
																		});

            if(result_session == std::end(sessions_))
            {
                std::cout << "ESP " << answer_authorization.second << " AUTHORIZED" << std::endl;

                std::shared_ptr<Session> new_session(new Session(answer_authorization.second));
                new_session->esp_connection = new_esp;
                sessions_.push_back(new_session);

                std::thread t(&Session::run_session, std::ref(*new_session));
                t.detach();
            }
            else
                std::cout << "ESP " << answer_authorization.second << " YET AUTHORIZED" << std::endl;
        }
        else
        {
            std::cout << "ESP " << answer_authorization.second << " BAD AUTHORIZED" << std::endl;
        }
		
    }
}

void Server::stop_accept_esp_()
{
    stopped_accept_esp_.store(true, std::memory_order_relaxed);
}

void Server::start_accept_client_()
{
    std::cout << "START ACCEPT CLIENT" << std::endl;

    while((!this->stopped_accept_client_.load(std::memory_order_relaxed)))
    {
        std::shared_ptr<Client> new_client(new Client(io_service_));
        client_acceptor_.accept(new_client->get_address());

        std::pair<bool, std::string> answer_authorization = new_client->authorization();

        if(answer_authorization.first)
        {   
            auto result_session = std::find_if(std::begin(sessions_), std::end(sessions_), [answer_authorization](std::shared_ptr<Session> session)
																		{
																			return (session->get_session_name() == answer_authorization.second) && (session->client_connection == nullptr); 
																		});

            if(result_session != std::end(sessions_))
            {
                std::cout << "CLIENT " << answer_authorization.second << " AUTHORIZED" << std::endl;
                (*result_session)->client_connection = new_client;
            }
            else
                std::cout << "CLIENT " << answer_authorization.second << " YET AUTHORIZED" << std::endl;
        }
        else
        {
            std::cout << "CLIENT " << answer_authorization.second << " BAD AUTHORIZED" << std::endl;
        }
		
    }
}

void Server::stop_accept_client_()
{
    stopped_accept_client_.store(true, std::memory_order_relaxed);
}

void Server::run()
{
    std::thread t1(&Server::start_accept_esp_, this);
    std::thread t2(&Server::start_accept_client_, this);
	
    int a = 1;

    while(a)
    {
        std::cin >> a;
    }

    this->stop_accept_esp_();
    this->stop_accept_client_();

	t1.join();
    t2.join();
}