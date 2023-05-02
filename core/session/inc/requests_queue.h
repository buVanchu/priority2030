#ifndef REQUESTS_QUEUE_H
#define REQUESTS_QUEUE_H

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include "parser.h"

class SmartRequestsQueue
{
private:
	std::queue<std::pair<RequestType, std::string>> queue_;
	std::mutex mtx;

public:
	void push_message(RequestType type, const std::string& request);
	std::pair<RequestType, std::string> pop_message();
	bool isEmpty();
};

#endif /*REQUESTS_QUEUE_H*/