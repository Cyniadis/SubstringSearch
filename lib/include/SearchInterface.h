#ifndef SEARCHINTERFACE_H
#define SEARCHINTERFACE_H

#include <chrono>
#include <string>
#include <vector>

class SearchInterface
{
public:
    SearchInterface() = default;
    virtual ~SearchInterface() = default;

    virtual bool loadWordList(const std::string& wordListPath) = 0;
    virtual std::vector<std::string> searchWord(const std::string& subStr) = 0;

    virtual std::vector<std::string> searchWordTimed(const std::string& subStr, unsigned long& elapsed)
    {
        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        std::vector<std::string> words = searchWord(subStr);
        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return words;
    }

};

#endif // SEARCHINTERFACE_H
