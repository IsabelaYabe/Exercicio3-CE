#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include <string>
#include <vector>
#include <thread>
#include <filesystem>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <chrono>

#include "queue/queue.hpp"

class Watchdog {
public:
    Watchdog(const std::vector<std::string>& directories, Queue<std::string>& queue);
    ~Watchdog();

private:
    std::vector<std::thread> monitoringThreads;
    Queue<std::string>& queue_;
    std::unordered_map<std::string, std::filesystem::file_time_type> fileLastModified_;
    std::shared_timed_mutex modificationMutex_;

    void monitorDirectory(const std::string& directory);
    std::string extractPrefix(const std::string& filename);
    bool isRelevantExtension(const std::string& filename);
    void removeOldFileFromMonitoring(const std::string& prefix, const std::string& newPath);

    Watchdog(const Watchdog&) = delete;
    Watchdog& operator=(const Watchdog&) = delete;
};

#endif  // WATCHDOG_HPP