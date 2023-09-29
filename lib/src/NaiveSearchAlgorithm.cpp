#include "NaiveSearchAlgorithm.h"

#include <iostream>
#include <fstream>
#include <string>

NaiveSearchAlgorithm::NaiveSearchAlgorithm()
{

}

bool NaiveSearchAlgorithm::loadWordList(const std::string &wordListPath)
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


std::vector<std::string> NaiveSearchAlgorithm::searchWord(const std::string &subStr)
{
    std::vector<std::string> words;

    for( const std::string& word : _wordList ) {
        if( startstWith(word, subStr) ) {
            words.push_back(word);
        }
    }
    return words;
}

bool NaiveSearchAlgorithm::startstWith(const std::string &word, const std::string &subStr)
{
    return (word.substr(0, subStr.size()) == subStr);
}
