#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <iostream>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    ThreadPool(unsigned int nbThreads);
    ~ThreadPool();

    int runThread(std::function<void(void)> func);

    void jointThread(int idx);
    bool isThreadValid(int idx);

    void jointAllThreads();

    static inline unsigned getMaxThreads() { return std::thread::hardware_concurrency(); }

    inline size_t getThreadPoolSize() { return _threadPool.size(); }

private:
    std::vector<std::thread> _threadPool;
    unsigned _idx;
};

#endif // THREADPOOL_H
