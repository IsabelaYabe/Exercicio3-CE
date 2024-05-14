#ifndef FILE_PROCESSOR_HPP
#define FILE_PROCESSOR_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <unordered_map>
#include <atomic>
#include <chrono>

#include "queue/queue.hpp"

class FileProcessor {
public:
    FileProcessor(Queue<std::string>& inputQueue, Queue<std::string>& outputQueue);
    ~FileProcessor();

private:
    std::vector<std::thread> workerThreads;
    std::mutex processMutex_;
    Queue<std::string>& inputQueue_;
    Queue<std::string>& outputQueue_;
    std::unordered_map<std::string, std::streampos> fileState_;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> fileLastAccess_;

    void processFiles();
    void cleanupFileStates();

    FileProcessor(const FileProcessor&) = delete;
    FileProcessor& operator=(const FileProcessor&) = delete;
};

#endif  // FILE_PROCESSOR_HPP