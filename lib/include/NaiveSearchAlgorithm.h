#ifndef NAIVESEARCHALGORITHM_H
#define NAIVESEARCHALGORITHM_H

#include "SearchAlgorithmInterface.h"


#include <vector>

class NaiveSearchAlgorithm : public SearchAlgorithmInterface
{
public:
    NaiveSearchAlgorithm();

    virtual bool loadWordList(const std::string &wordListPath) override;
    virtual std::vector<std::string> searchWord(const std::string &subStr) override;

protected:
    std::vector<std::string> _wordList;

    bool startstWith(const std::string& word, const std::string& subStr);
};

#endif // NAIVESEARCHALGORITHM_H
