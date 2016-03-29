#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class CThreadSafeQueue
{
private:
    std::mutex head_mutex;
    std::mutex tail_mutex;
    std::condition_variable data_cond;
    std::queue<T> _queue;
public:
    CThreadSafeQueue(){};

    CThreadSafeQueue(const CThreadSafeQueue& other) = delete;
    CThreadSafeQueue& operator=(const CThreadSafeQueue& other) = delete;

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lc(head_mutex);
        
        if (_queue.empty()) return std::shared_ptr<T>();
        auto pos = std::shared_ptr<T>(std::move(_queue.front()));
        _queue.pop();
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lc(head_mutex);

        if (_queue.empty()) return false;
        value = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lc(head_mutex);
        data_cond.wait(lc, [&]{return !empty(); });

        if (_queue.empty()) return std::shared_ptr<T>(NULL);

        std::shared_ptr<T> sptr(new T(std::move(_queue.front())));
        _queue.pop();
        return sptr;
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lc(head_mutex);
        data_cond.wait(head_mutex, [&]{return !empty(); });
        if (_queue.empty()) return std::shared_ptr<T>();
        value = std::move(_queue.front());
        _queue.pop();

    }

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lc(tail_mutex);

        _queue.push(new_value);

        data_cond.notify_one();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lc(tail_mutex);
        return _queue.empty();
    }

    void stop()
    {
        data_cond.notify_all();
    }

};
