#include "requests_queue.h"

void SmartRequestsQueue::push_message(RequestType type, const std::string& request)
{
	std::lock_guard<std::mutex> guard(mtx);
	queue_.push(std::make_pair(type, request));
}

std::pair<RequestType, std::string> SmartRequestsQueue::pop_message()
{
	std::lock_guard<std::mutex> guard(mtx);
	auto request = queue_.front();
	queue_.pop();
    return request;
}

bool SmartRequestsQueue::isEmpty()
{
	std::lock_guard<std::mutex> guard(mtx);
	return queue_.empty();
}