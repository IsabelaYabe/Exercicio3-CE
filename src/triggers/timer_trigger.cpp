#include "triggers/timer_trigger.hpp"

TimerTrigger::TimerTrigger(FileProcessor& processor, Queue<std::string>& parserQueue)
    : processor_(processor), parserQueue_(parserQueue) {}

void TimerTrigger::trigger(const std::string& filePath) {
    std::string processedData = processor_.processFile(filePath);
    if (!processedData.empty()) {
        parserQueue_.enqueue(processedData);
    }
}
