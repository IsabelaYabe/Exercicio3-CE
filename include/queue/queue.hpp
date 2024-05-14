#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template <typename T>
class Queue {
public:
    Queue() = default;
    ~Queue() = default;

    // Delete copy constructor and assignment operator
    Queue(const Queue& other) = delete;
    Queue& operator=(const Queue& other) = delete;

    void enqueue(T value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        bool wasEmpty = m_queue.empty();
        m_queue.push(std::move(value));
        std::cout << "Item enqueued. Queue was " << (wasEmpty ? "empty" : "not empty") << ".\n";
        m_condition.notify_one();
    }

    std::optional<T> dequeue() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this] { return !m_queue.empty(); });
        if (m_queue.empty()) { // Double-check to handle spurious wake-ups
            return std::nullopt;
        }
        auto value = std::move(m_queue.front());
        m_queue.pop();
        std::cout << "Item dequeued. Queue size is now " << m_queue.size() << ".\n";
        return value;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    int size(){
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    };

private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_condition;
};

#endif // QUEUE_HPP
