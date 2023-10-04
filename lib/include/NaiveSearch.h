#ifndef NAIVESEARCH_H
#define NAIVESEARCH_H

#include "SearchInterface.h"


#include <vector>

class NaiveSearch : public SearchInterface
{
    using StringVecConstIterator = std::vector<std::string>::const_iterator;

public:
    NaiveSearch();
    virtual ~NaiveSearch() = default;

    virtual bool loadWordList(const std::string &wordListPath) override;
    virtual std::vector<std::string> searchWords(const std::string &subStr) override;
    virtual std::vector<std::string> searchWords(const std::string &subStr, StringVecConstIterator startIt, StringVecConstIterator endIt);

protected:
    std::vector<std::string> _wordList;

    bool startstWith(const std::string& word, const std::string& subStr);
};

#endif // NAIVESEARCH_H
