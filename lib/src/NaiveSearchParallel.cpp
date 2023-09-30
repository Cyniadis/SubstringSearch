#include "NaiveSearchParallel.h"


NaiveSearchParallel::NaiveSearchParallel(unsigned int nbThreads, bool useSingleVector)
    : NaiveSearch(),
    _threadPool(nbThreads),
    _nbThreads(nbThreads),
    _useSingleVector(useSingleVector)
{

}

void NaiveSearchParallel::runSearchWordSingleVector(const std::string& subStr, StringVecConstIterator startIt, StringVecConstIterator endIt)
{
    for( StringVecConstIterator it = startIt; it != endIt; ++it )
    {
        const std::string word = (*it);
        if( startstWith(word, subStr) )
        {
            _mutex.lock();
            foundWords.push_back(word);
            _mutex.unlock();
        }
    }
}


std::vector<std::string> NaiveSearchParallel::searchWordSingleVector(const std::string& subStr)
{
    // All threads push word in the same vector

    for( int i = 0; i < _nbThreads; ++i )
    {
        StringVecConstIterator startIt = std::begin(_wordList) + i * (_wordList.size() / _nbThreads);
        StringVecConstIterator endIt = std::begin(_wordList) + ((i+1) * (_wordList.size() / _nbThreads));

        std::function<void(void)> func = std::bind(&NaiveSearchParallel::runSearchWordSingleVector, this, subStr, startIt, endIt);
        _threadPool.runThread(func);
    }

    _threadPool.jointAllThreads();
    return foundWords;
}


void NaiveSearchParallel::runSearchWordMultipleVectors(const std::string& subStr, StringVecConstIterator startIt, StringVecConstIterator endIt, unsigned threadIdx)
{
    foundWordsVectors[threadIdx] = NaiveSearch::searchWord(subStr, startIt, endIt);
}

std::vector<std::string> NaiveSearchParallel::searchWordMultipleVectors(const std::string& subStr)
{
    std::vector<std::string> allFoundWords;
    foundWordsVectors.resize(_nbThreads);

    for( int i = 0; i < _nbThreads; ++i )
    {
        StringVecConstIterator startIt = std::begin(_wordList) + i * (_wordList.size() / _nbThreads);
        StringVecConstIterator endIt = std::begin(_wordList) + ((i+1) * (_wordList.size() / _nbThreads));

        std::function<void(void)> func = std::bind(&NaiveSearchParallel::runSearchWordMultipleVectors, this, subStr, startIt, endIt, i);
        _threadPool.runThread(func);
    }
    _threadPool.jointAllThreads();

    for( int j = 0; j < _nbThreads; ++j ) {
//        allFoundWords.insert(allFoundWords.end(), std::make_move_iterator(foundWordsVectors[j].begin()), std::make_move_iterator(foundWordsVectors[j].end()));
        allFoundWords.insert(allFoundWords.end(), foundWordsVectors[j].begin(), foundWordsVectors[j].end());
    }

    return allFoundWords;
}


std::vector<std::string> NaiveSearchParallel::searchWord(const std::string &subStr)
{
    if( _useSingleVector ) {
        return searchWordSingleVector(subStr);
    }
    else {
        return searchWordMultipleVectors(subStr);
    }
}
