#ifndef SEARCHALGORITHMINTERFACE_H
#define SEARCHALGORITHMINTERFACE_H

#include <chrono>
#include <string>
#include <vector>

class SearchAlgorithmInterface
{
public:
    SearchAlgorithmInterface() = default;
    ~SearchAlgorithmInterface() = default;

    virtual bool loadWordList(const std::string& wordListPath) = 0;
    virtual std::vector<std::string> searchWord(const std::string& word) = 0;

    virtual std::vector<std::string> searchWordTimed(const std::string& word, unsigned long& elapsed)
    {
        std::chrono::time_point start = std::chrono::system_clock::now();
        std::vector<std::string> words = searchWord(word);
        std::chrono::time_point end = std::chrono::system_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return words;
    }

};

#endif // SEARCHALGORITHMINTERFACE_H
