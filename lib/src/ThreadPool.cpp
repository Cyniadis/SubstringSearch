#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned nbThreads)
    :   _idx(0)
{
    unsigned maxThreads = std::thread::hardware_concurrency();
    if (nbThreads > maxThreads) {
        throw std::runtime_error("Maximum number of threads (" + std::to_string(maxThreads) + ") exceeded.");
    }
    else {
        _threadPool.resize(nbThreads);
    }
}

ThreadPool::~ThreadPool()
{
    jointAllThreads();
}

int ThreadPool::runThread(std::function<void ()> func)
{
    if( _idx >= _threadPool.size() ) {
        std::cerr << "ERROR: Thread pool is full\n";
        return -1;
    }

    jointThread(_idx);
    _threadPool[_idx] = std::thread(func);
    _idx++;

    return _idx;
}

void ThreadPool::jointThread(int idx)
{
    if( isThreadValid(idx) && _threadPool[idx].joinable() ) {
        _threadPool[idx].join();
    }
}

bool ThreadPool::isThreadValid(int idx)
{
    return (_threadPool[idx].get_id() != std::thread::id());
}

void ThreadPool::jointAllThreads()
{
    for( unsigned i = 0; i < _threadPool.size(); ++i) {
        jointThread(i);
    }
    _idx = 0;
}

