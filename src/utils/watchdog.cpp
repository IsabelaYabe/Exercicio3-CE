#include "utils/watchdog.hpp"

Watchdog::Watchdog(const std::vector<std::string>& directories, Queue<std::string>& queue) : queue_(queue) {
    for (const auto& dir : directories) {
        monitoringThreads.emplace_back(&Watchdog::monitorDirectory, this, dir);
    }
}

Watchdog::~Watchdog() {
    for (auto& th : monitoringThreads) {
        if (th.joinable())
            th.join();
    }
}

void Watchdog::monitorDirectory(const std::string& directory) {
    try {
        std::unordered_map<std::string, std::string> activeFiles;
        while (true) {
            for (const auto& fileEntry : std::filesystem::directory_iterator(directory)) {
                std::string path = fileEntry.path().string();
                if (!isRelevantExtension(path))
                    continue;

                std::string prefix = extractPrefix(fileEntry.path().filename().string());

                std::unique_lock<std::shared_timed_mutex> lock(modificationMutex_);
                auto last_write_time = std::filesystem::last_write_time(fileEntry);
                //if (activeFiles.count(prefix) && activeFiles[prefix] != path) {
                //    removeOldFileFromMonitoring(prefix, path);
                //}

                if (fileLastModified_.find(path) == fileLastModified_.end() || 
                    fileLastModified_[path] < last_write_time) {
                    fileLastModified_[path] = last_write_time;
                    // activeFiles[prefix] = path;
                    std::cout << "Enqueuing file: " << path << std::endl;
                    queue_.enqueue(path);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in monitoring directory: " << e.what() << std::endl;
    }
}

std::string Watchdog::extractPrefix(const std::string& filename) {
    auto pos = filename.find_last_of('-');
    return (pos != std::string::npos) ? filename.substr(0, pos) : filename;
}

bool Watchdog::isRelevantExtension(const std::string& filename) {
    const size_t dotPos = filename.rfind('.');
    if (dotPos == std::string::npos) return false;  // No extension found
    
    std::string extension = filename.substr(dotPos);
    return extension == ".txt" || extension == ".csv";
}

//void Watchdog::removeOldFileFromMonitoring(const std::string& prefix, const std::string& newPath) {
//    std::string oldPath = fileLastModified_[prefix];
//    fileLastModified_.erase(oldPath);
 //   queue_.dequeue(oldPath);  // Assume FileQueue has a method to remove items
//}