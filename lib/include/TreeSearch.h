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

    TreeSearch(unsigned nbChar);
    virtual ~TreeSearch() = default;

    bool loadWordList(const std::string &wordListPath);
    std::vector<std::string> searchWord(const std::string &subStr);

private:
    std::unique_ptr<TreeNode> _searchTree;
    unsigned _nbChar;

private:
    void addWordToTree(const std::string &word, unsigned int nbChar, std::unique_ptr<TreeNode> &treeNode, int letterIdx);
    void searchWordRec(const std::string &subStr, std::unique_ptr<TreeNode> &treeNode, std::vector<std::string> &foundWords);
};

#endif // TREESEARCH_H
