#ifndef TIMER_TRIGGER_HPP
#define TIMER_TRIGGER_HPP

#include "utils/file_processor.hpp"
#include "queue/queue.hpp"

class TimerTrigger {
public:
    TimerTrigger(FileProcessor& processor, Queue<std::string>& parserQueue);
    void trigger(const std::string& filePath);

private:
    FileProcessor& processor_;
    Queue<std::string>& parserQueue_;
};

#endif // TIMER_TRIGGER_HPP
