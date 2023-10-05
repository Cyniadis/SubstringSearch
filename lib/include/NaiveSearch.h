#ifndef NAIVESEARCH_H
#define NAIVESEARCH_H

#include "SearchInterface.h"
#include <vector>

/**
 * @brief NaiveSearch : Simplest single threaded search algorithm
 */
class NaiveSearch : public SearchInterface
{
    // Shortcut for const iterator
    using StringVecConstIterator = std::vector<std::string>::const_iterator;

public:
    /**
     * @brief NaiveSearch : Default constructor
     */
    NaiveSearch();
    virtual ~NaiveSearch() = default;

    /**
     * @brief loadWordList : Open given text and loads the word in a vector
     * @param[in] wordListPath : Path a text file containing the list of words to search
     * @return True if the file was successfully loadded, False otherwise
     */
    virtual bool loadWordList(const std::string &wordListPath) override;
    /**
     * @brief searchWords : Search all the words starting with given string
     * @param[in] subStr : String to search
     * @return Vector of string with all the found words
     */
    virtual std::vector<std::string> searchWords(const std::string &subStr) override;
    /**
     * @brief searchWords : Look for all the words starting with given string in the list defined by the given iterators
     * @param[in] subStr : String to search
     * @param[in] startIt : First iterator
     * @param[in] endIt : Last iteretor
     * @return Vector of string with all the found words
     */
    virtual std::vector<std::string> searchWords(const std::string &subStr, StringVecConstIterator startIt, StringVecConstIterator endIt);
    /**
     * @brief incrementalSearchWords : Perform incremental search
     * @param[in] subStr : String to search
     * @param[in] idx : Index of the new character
     * @param[in] add
     * @return Vector of string with all the found words
     */
    virtual std::vector<std::string> incrementalSearchWords(const std::string &subStr, int idx, bool add) override;

protected:
    std::vector<std::string> _wordList; /*! List of words to search */
    std::vector<std::string> _currentFoundWords; /*! List of current words to search for incremental search */

    /**
     * @brief startstWith : Check if the given word starts with the given sub string
     * @param[in] word : String to check
     * @param[in] subStr : Sub string to test
     * @return True if word starts with subStr
     */
    bool startstWith(const std::string& word, const std::string& subStr);
};

#endif // NAIVESEARCH_H
