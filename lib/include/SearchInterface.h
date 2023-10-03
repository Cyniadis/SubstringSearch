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
    virtual std::vector<std::string> searchWords(const std::string& subStr) = 0;

    virtual std::vector<std::string> searchWordTimed(const std::string& subStr, unsigned long& elapsed)
    {
        using clock = std::chrono::high_resolution_clock;

        std::chrono::time_point start = clock::now();
        std::vector<std::string> words = searchWords(subStr);
        std::chrono::time_point end = clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return words;
    }

};

#endif // SEARCHINTERFACE_H
