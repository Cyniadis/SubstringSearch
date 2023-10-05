#ifndef TREESEARCH_H
#define TREESEARCH_H

#include "SearchInterface.h"
#include "TreeNode.h"

#include <fstream>
#include <memory>
#include <type_traits>

/**
 * @brief TreeSearch : Perform search using a prefix tree
 */
class TreeSearch : public SearchInterface
{
public:
    /**
     * @brief TreeSearch : Construct a tree from given root node
     * @param[in] searchTree : Root node
     */
    TreeSearch(std::shared_ptr<TreeNodeBase> searchTree);
    virtual ~TreeSearch() = default;

    /**
     * @brief searchWords : Search all the words starting with given string
     * @param[in] subStr : String to search
     * @return Vector of string with all the found words
     */
    virtual bool loadWordList(const std::string &wordListPath) override;
    /**
     * @brief searchWords : Search all the words starting with given string
     * @param[in] subStr : String to search
     * @return Vector of string with all the found words
     */
    virtual std::vector<std::string> searchWords(const std::string &subStr) override;
    /**
     * @brief searchWords
     * @param[in] subStr
     * @return
     */
    virtual std::vector<std::string> searchWords(const std::string &subStr, std::shared_ptr<TreeNodeBase> &treeNode);
    /**
     * @brief incrementalSearchWords : Perform incremental search
     * @param[in] subStr : String to search
     * @param[in] idx : Index of the new character
     * @param[in] add
     * @return Vector of string with all the found words
     */
    virtual std::vector<std::string> incrementalSearchWords(const std::string& subStr, int idx, bool add) override;

protected:
    std::shared_ptr<TreeNodeBase> _searchTree;

    std::shared_ptr<TreeNodeBase> _lastPrefixTree;
    std::vector<std::string> _currentWordList;

protected:
    /**
     * @brief findPrefixSubTree : Recusively find the node corresponding to the given string
     * @param[in] treeNode : Node to perform the search
     * @param[in] subStr :  String to search
     * @param[in] letterIdx : index of the searched letter in subStr
     * @return
     */
    virtual std::shared_ptr<TreeNodeBase> findPrefixSubTree(std::shared_ptr<TreeNodeBase> &treeNode,
                                                            const std::string &subStr,
                                                            int letterIdx = 0);
    /**
     * @brief saveWord : If exists, add the node's word in the output vector
     * @param[in] treeNode : Tree node storing the word to save
     * @param[in/out] foundWords : Vector where to save the node's word
     */
    virtual void saveWord(std::shared_ptr<TreeNodeBase> &treeNode,
                          std::vector<std::string> &foundWords);

    /**
     * @brief addWordToTree : Recusively add a word in the tree
     * @param[in] word : Word to add
     * @param[in] treeNode : Tree node to start the recursive traversal
     * @param[in] letterIdx : index of the letter in subStr
     */
    virtual void addWordToTree(const std::string &word,
                               const std::shared_ptr<TreeNodeBase> &treeNode,
                               int letterIdx);
    /**
     * @brief saveAllSubtreeWords : Recusively save all words to the given vector starting from the given tree node
     * @param[in] treeNode : Tree node storing the word to save
     * @param[in/out] foundWords : Vector where to save the node's word
     */
    virtual void saveAllSubtreeWords(const std::shared_ptr<TreeNodeBase>& treeNode,
                                     std::vector<std::string> &foundWords);
};

#endif // TREESEARCH_H
