#ifndef NAIVESEARCHPARALLEL_H
#define NAIVESEARCHPARALLEL_H

#include "NaiveSearch.h"
#include "SpinLock.h"
#include "ThreadPool.h"
#include <mutex>

/**
 * @brief NaiveSearchParallel : Perform multi threaded naive search
 */
class NaiveSearchParallel : public NaiveSearch
{
    // Shortcut for const iterator
    using StringVecConstIterator = std::vector<std::string>::const_iterator;

public:
    /**
     * @brief NaiveSearchParallel : Constructor
     * @param[in] nbThreads : Number of threads to use
     * @param[in] useDirectInsert : if true, search will use insert found words in the final output vector (direct insert),
     *                              otherwise each thread will store the words in a vector before insertion in the output vector (batched insert)
     */
    NaiveSearchParallel(unsigned nbThreads, bool useDirectInsert);
    ~NaiveSearchParallel() = default;

    /**
     * @brief searchWords : Search all the words starting with given string
     * @param[in] subStr : String to search
     * @return Vector of string containing all the found words
     */
    virtual std::vector<std::string> searchWords(const std::string &subStr) override;

private:
    ThreadPool _threadPool;
    unsigned _nbThreads;
//    std::mutex _mutex;
    SpinLock _mutex;
    bool _useDirectInsert;
    std::vector<std::string> _foundWords;

private:
    /**
     * @brief runSearchWordDirectInsert : Function executed by thread. Perform direct insert search in the list defined by given iterators
     * @param[in] subStr : String to search
     * @param[in] startIt : First iterator
     * @param[in] endIt : Last iterator
     */
    void runSearchWordDirectInsert(const std::string& subStr, StringVecConstIterator startIt, StringVecConstIterator endIt);
    /**
     * @brief runSearchWordBatchedInsert : Function executed by thread. Perform direct batched search in the list defined by given iterators
     * @param[in] subStr : String to search
     * @param[in] startIt : First iterator
     * @param[in] endIt : Last iterator
     */
    void runSearchWordBatchedInsert(const std::string &subStr, StringVecConstIterator startIt, StringVecConstIterator endIt);
};

#endif // NAIVESEARCHPARALLEL_H
