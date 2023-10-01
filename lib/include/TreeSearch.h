#ifndef TREESEARCH_H
#define TREESEARCH_H

#include <SearchInterface.h>
#include <memory>

class TreeSearch : public SearchInterface
{
public:

    struct TreeNode
    {
        TreeNode()
            : _letter('\0')
        {
        }

        TreeNode(char letter)
            : _letter(letter)
        {
        }

        bool isLetterValid() { return (_letter != '\0'); }

        char _letter;
        std::vector<std::unique_ptr<TreeNode>> _nextLetters;
        std::string _word;
    };

    TreeSearch(bool compactTree);
    virtual ~TreeSearch() = default;

    bool loadWordList(const std::string &wordListPath);
    std::vector<std::string> searchWord(const std::string &subStr);

protected:
    std::unique_ptr<TreeNode> _searchTree;
    bool _compactTree;

protected:
    virtual void addWordToTree(const std::string &word, const std::unique_ptr<TreeNode> &treeNode, int letterIdx);
    virtual void searchWordRec(const std::string &subStr, const std::unique_ptr<TreeNode> &treeNode, std::vector<std::string> &foundWords);

    virtual void addWordToTreeRandomAccess(const std::string &word, const std::unique_ptr<TreeNode> &treeNode, int letterIdx);
    virtual void searchWordRandomAccess(const std::string &subStr, const std::unique_ptr<TreeNode> &treeNode, std::vector<std::string> &foundWords);

    virtual std::vector<std::unique_ptr<TreeSearch::TreeNode>>::iterator findLetter(const std::unique_ptr<TreeNode> &treeNode, const char& letter);
    virtual void addWordToTreeCompact(const std::string &word, const std::unique_ptr<TreeNode> &treeNode, int letterIdx);
    virtual void searchWordCompact(const std::string &subStr, const std::unique_ptr<TreeNode> &treeNode, std::vector<std::string> &foundWords);

};

#endif // TREESEARCH_H
