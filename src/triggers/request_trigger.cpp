#include "triggers/request_trigger.hpp"

RequestTrigger::RequestTrigger(Queue<std::string>& parserQueue)
    : parserQueue_(parserQueue) {}

void RequestTrigger::trigger(const std::string& requestData) {
    // Here you might log, analyze, or further process the requestData
    std::cout << "Received data for processing: " << requestData << std::endl;

    // Assuming you enqueue it for further processing
    parserQueue_.enqueue(requestData);
}
