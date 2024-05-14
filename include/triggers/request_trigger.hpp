#ifndef REQUEST_TRIGGER_HPP
#define REQUEST_TRIGGER_HPP

#include <functional>
#include "queue/queue.hpp"

class RequestTrigger {
public:
    RequestTrigger(Queue<std::string>& parserQueue);
    void trigger(const std::string& requestData);

private:
    Queue<std::string>& parserQueue_;
};

#endif // REQUEST_TRIGGER_HPP
