#include "utils/file_processor.hpp"
#include <fstream>
#include <sstream>
#include <vector>

FileProcessor::FileProcessor(Queue<std::string>& inputQueue, Queue<std::string>& outputQueue) : inputQueue_(inputQueue), outputQueue_(outputQueue) {
    for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
        workerThreads.emplace_back(&FileProcessor::processFiles, this);
    }
}

FileProcessor::~FileProcessor() {
    for (auto& worker : workerThreads) {
        worker.join();
    }
}

void FileProcessor::processFiles() {
    while (true) {
        auto optFilepath = inputQueue_.dequeue();
        if (!optFilepath) {
            std::cerr << "No item to process. Thread is waiting...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep to reduce CPU usage
            continue;
        }

        std::string filepath = *optFilepath;
        std::cout << "Processing file: " << filepath << std::endl;

        static std::mutex fileMutex;
        std::lock_guard<std::mutex> guard(fileMutex);

        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            continue;
        }

        // Always read the first line
        std::string firstLine;
        if (std::getline(file, firstLine)) {
            firstLine += "\n"; // Restore the newline removed by getline
        } else {
            std::cerr << "Failed to read the first line of the file: " << filepath << std::endl;
            continue;
        }

        // Check and set the file pointer if we continue from the previous state
        std::streampos startPos;
        if (fileState_.find(filepath) != fileState_.end() && fileState_[filepath] != std::streampos(0)) {
            startPos = fileState_[filepath];  // Resume from where we left off last time
            file.seekg(startPos);
        } else {
            startPos = file.tellg(); // Or just continue from the current position if starting new
        }

        std::string line;
        std::string content = firstLine;  // Start content with the first line
        std::streampos lastPos = startPos;

        while (std::getline(file, line)) {
            content += line + "\n";
            lastPos = file.tellg();  // Update last processed position
        }

        // Store the last read position for this file
        fileState_[filepath] = lastPos;

        if (!content.empty()) {
            std::cout << "Enqueuing content: \n" << content.substr(0, 100) << "...\n"; // Show part of the content for verification
            outputQueue_.enqueue(content); // Enqueue the content for further processing
        } else {
            std::cout << "No new content to process for file: " << filepath << std::endl;
        }

        // Optional cleanup logic can be included here  
        // cleanupFileStates();  
    }
}
void FileProcessor::cleanupFileStates() {
    auto now = std::chrono::steady_clock::now();
    for (auto it = fileLastAccess_.begin(); it != fileLastAccess_.end(); ) {
        if (now - it->second > std::chrono::minutes(5)) {
            fileState_.erase(it->first);
            it = fileLastAccess_.erase(it);
        } else {
            ++it;
        }
    }
}