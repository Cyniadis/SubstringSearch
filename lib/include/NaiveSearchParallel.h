#ifndef NAIVESEARCHPARALLEL_H
#define NAIVESEARCHPARALLEL_H

#include "NaiveSearch.h"
#include "SpinLock.h"
#include "ThreadPool.h"
#include <mutex>

class NaiveSearchParallel : public NaiveSearch
{
    using StringVecConstIterator = std::vector<std::string>::const_iterator;

public:
    NaiveSearchParallel(unsigned nbThreads, bool useDirectInsert);
    ~NaiveSearchParallel() = default;

    virtual std::vector<std::string> searchWords(const std::string &subStr) override;


private:
    ThreadPool _threadPool;
    unsigned _nbThreads;
//    std::mutex _mutex;
    SpinLock _mutex;
    bool _useDirectInsert;
    inline static std::vector<std::string> _foundWords;

private:
    void runSearchWordSingleVector(const std::string& word, StringVecConstIterator startIt, StringVecConstIterator endIt);
    void runSearchWordBatchedInsert(const std::string &subStr, StringVecConstIterator startIt, StringVecConstIterator endIt, unsigned int threadIdx);

    std::vector<std::string> searchWordsDirectInsert(const std::string& subStr);
    std::vector<std::string> searchWordsBatchedInsert(const std::string &subStr);
};

#endif // NAIVESEARCHPARALLEL_H
