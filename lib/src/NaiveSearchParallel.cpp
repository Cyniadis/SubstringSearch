#include "NaiveSearchParallel.h"


NaiveSearchParallel::NaiveSearchParallel(unsigned int nbThreads, bool useDirectInsert)
    : NaiveSearch(),
    _threadPool(nbThreads),
    _nbThreads(nbThreads),
    _useDirectInsert(useDirectInsert)
{

}

void NaiveSearchParallel::runSearchWordDirectInsert(const std::string& subStr, StringVecConstIterator startIt, StringVecConstIterator endIt)
{
    for( StringVecConstIterator it = startIt; it != endIt; ++it )
    {
        const std::string word = (*it);
        if( startstWith(word, subStr) )
        {
            _mutex.lock();
            _foundWords.push_back(word);
            _mutex.unlock();
        }
    }
}


std::vector<std::string> NaiveSearchParallel::searchWordsDirectInsert(const std::string& subStr)
{
    // All threads push word in the same vector

    for( int i = 0; i < _nbThreads; ++i )
    {
        int step = (_wordList.size() / _nbThreads); //add check if step = 0;
        StringVecConstIterator startIt = std::begin(_wordList) + i * step;
        StringVecConstIterator endIt = (i == _nbThreads -1 ) ? _wordList.end() : (startIt + step);
        
        std::function<void(void)> func = std::bind(&NaiveSearchParallel::runSearchWordDirectInsert, this, subStr, startIt, endIt);
        _threadPool.runThread(func);
    }

    _threadPool.jointAllThreads();
    return _foundWords;
}


void NaiveSearchParallel::runSearchWordBatchedInsert(const std::string& subStr, StringVecConstIterator startIt, StringVecConstIterator endIt)
{
    std::vector<std::string> vec = NaiveSearch::searchWords(subStr, startIt, endIt);
    _mutex.lock();
    _foundWords.insert(_foundWords.end(), vec.begin(), vec.end());
    _mutex.unlock();
}

std::vector<std::string> NaiveSearchParallel::searchWordsBatchedInsert(const std::string& subStr)
{
    for( int i = 0; i < _nbThreads; ++i )
    {
        int step = (_wordList.size() / _nbThreads); //add check if step = 0;
        StringVecConstIterator startIt = std::begin(_wordList) + (i * step);
        StringVecConstIterator endIt = (i == _nbThreads -1 ) ? _wordList.end() : (startIt + step);

        std::function<void(void)> func = std::bind(&NaiveSearchParallel::runSearchWordBatchedInsert, this, subStr, startIt, endIt);
        _threadPool.runThread(func);
    }
    _threadPool.jointAllThreads();

    return _foundWords;
}


std::vector<std::string> NaiveSearchParallel::searchWords(const std::string &subStr)
{
    if( _useDirectInsert ) {
        return searchWordsDirectInsert(subStr);
    }
    else {
        return searchWordsBatchedInsert(subStr);
    }
}
