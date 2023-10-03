#include "NaiveSearch.h"

#include <iostream>
#include <fstream>
#include <string>

NaiveSearch::NaiveSearch()
    :  SearchInterface()
{

}

bool NaiveSearch::loadWordList(const std::string &wordListPath)
{
    std::ifstream iFile(wordListPath);
    if( !iFile ) {
        return false;
    }

    std::string word;
    while( std::getline(iFile, word, '\n') ) {
        _wordList.push_back(word);
    }
    return true;
}


std::vector<std::string> NaiveSearch::searchWords(const std::string &subStr)
{
    return searchWord(subStr, std::begin(_wordList), std::end(_wordList));
}

std::vector<std::string> NaiveSearch::searchWord(const std::string &subStr, StringVecConstIterator startIt, StringVecConstIterator endIt)
{
    std::vector<std::string> foundWords;

    for( StringVecConstIterator it = startIt; it != endIt; ++it ) {
        const std::string word = (*it);
        if( startstWith(word, subStr) ) {
            foundWords.push_back(word);
        }
    }
    return foundWords;
}

bool NaiveSearch::startstWith(const std::string &word, const std::string &subStr)
{
    return (word.substr(0, subStr.size()) == subStr);
}
