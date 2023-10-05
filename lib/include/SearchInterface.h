#ifndef SEARCHINTERFACE_H
#define SEARCHINTERFACE_H

#include <chrono>
#include <string>
#include <vector>

/**
 * @brief SearchInterface : Common interface for all search algorithms
 */
class SearchInterface
{
public:
    SearchInterface() = default;
    virtual ~SearchInterface() = default;

    /**
     * @brief loadWordList : Open given text and loads the words
     * @param[in] wordListPath : Path a text file containing the list of words to search
     * @return True if the file was successfully loadded, False otherwise
     */
    virtual bool loadWordList(const std::string& wordListPath) = 0;
    /**
     * @brief searchWords : Search all the words starting with given string
     * @param[in] subStr : String to search
     * @return Vector of string with all the found words
     */
    virtual std::vector<std::string> searchWords(const std::string& subStr) = 0;
    /**
     * @brief searchWords : Search all the words starting with given string
     * @param[in] subStr : String to search
     * @param[out] elapsed : Execution time of searchWords function in microseconds
     * @return Vector of string with all the found words
     */
    virtual std::vector<std::string> searchWordTimed(const std::string& subStr, unsigned long& elapsed)
    {
        using clock = std::chrono::high_resolution_clock;

        std::chrono::time_point start = clock::now();
        std::vector<std::string> words = searchWords(subStr);
        std::chrono::time_point end = clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return words;
    }
    /**
     * @brief incrementalSearchWords : Perform incremental search
     * @param[in] subStr : String to search
     * @param[in] idx : Index of the new character
     * @param[in] add
     * @return Vector of string with all the found words
     */
    virtual std::vector<std::string> incrementalSearchWords(const std::string& subStr, int idx, bool add) = 0;

};

#endif // SEARCHINTERFACE_H
