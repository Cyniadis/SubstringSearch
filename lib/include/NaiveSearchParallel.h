#ifndef NAIVESEARCHPARALLEL_H
#define NAIVESEARCHPARALLEL_H

#include "NaiveSearch.h"
#include "ThreadPool.h"
#include <mutex>

class NaiveSearchParallel : public NaiveSearch
{
    using StringVecConstIterator = std::vector<std::string>::const_iterator;

public:
    NaiveSearchParallel(unsigned nbThreads, bool useSingleVector);
    ~NaiveSearchParallel() = default;

    virtual std::vector<std::string> searchWord(const std::string &subStr) override;


private:
    ThreadPool _threadPool;
    unsigned _nbThreads;
    std::vector<std::string> _foundWords;
    std::mutex _mutex;

    bool _useSingleVector;
    inline static std::vector<std::string> foundWords;
    inline static std::vector<std::vector<std::string>> foundWordsVectors;

private:
    void runSearchWordSingleVector(const std::string& word, StringVecConstIterator startIt, StringVecConstIterator endIt);
    void runSearchWordMultipleVectors(const std::string &subStr, StringVecConstIterator startIt, StringVecConstIterator endIt, unsigned int threadIdx);

    std::vector<std::string> searchWordSingleVector(const std::string& subStr);
    std::vector<std::string> searchWordMultipleVectors(const std::string &subStr);
};

#endif // NAIVESEARCHPARALLEL_H
