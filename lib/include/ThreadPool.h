#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <iostream>
#include <thread>
#include <vector>

/**
 * @brief ThreadPool
 */
class ThreadPool
{
public:
    /**
     * @brief Contructor
     * @param nbThreads : Maximum numbers of threads
     * @exceptions Runtime exception if given number of threads is greater than hardware maximum number of threads
     */
    ThreadPool(unsigned int nbThreads);
    ~ThreadPool();
    /**
     * @brief runThread : Run a thread executing the given function
     * @param[in] func : Function to execute
     * @return index of the thread in the pool, -1 if an error occurs
     */
    int runThread(std::function<void(void)> func);
    /**
     * @brief joinThread : Join thread at the given index
     * @param[in] idx : Index of the thread to join
     */
    void joinThread(int idx);
    /**
     * @brief isThreadValid : Check if the thread at the given index has been runned
     * @param[in] idx : Index of the thread to check
     * @return true if thread is valid, false otherwise
     */
    bool isThreadValid(int idx);
    /**
     * @brief joinAllThreads : Join all threads in the thread pool
     */
    void joinAllThreads();
    /**
     * @brief getMaxThreads : Return the maximum number of threads
     */
    static inline unsigned getMaxThreads() { return std::thread::hardware_concurrency(); }
    /**
     * @brief getThreadPoolSize : Return size of the thread pool
     */
    inline size_t getThreadPoolSize() { return _threadPool.size(); }

private:
    std::vector<std::thread> _threadPool; /*!< Vector of threads */
    unsigned _idx; /*!< Index of last thread */
};

#endif // THREADPOOL_H
