#ifndef TREESEARCH_H
#define TREESEARCH_H

#include "SearchInterface.h"
#include "TreeNode.h"

#include <fstream>
#include <memory>
#include <type_traits>

class TreeSearch : public SearchInterface
{
public:
    TreeSearch(std::shared_ptr<TreeNodeBase> searchTree);
    virtual ~TreeSearch() = default;

    virtual bool loadWordList(const std::string &wordListPath) override;
    virtual std::vector<std::string> searchWords(const std::string &subStr) override;

protected:
    std::shared_ptr<TreeNodeBase> _searchTree;

protected:
    virtual std::shared_ptr<TreeNodeBase> &findPrefixSubTree(std::shared_ptr<TreeNodeBase> &treeNode,
                                                             const std::string &subStr,
                                                             int letterIdx);
    virtual void saveWord(std::shared_ptr<TreeNodeBase> &treeNode,
                          std::vector<std::string> &foundWords);

    virtual void addWordToTree(const std::string &word,
                               const std::shared_ptr<TreeNodeBase> &treeNode,
                               int letterIdx);
    virtual void saveAllSubtreeWords(const std::shared_ptr<TreeNodeBase>& treeNode,
                                     std::vector<std::string> &foundWords);
};

#endif // TREESEARCH_H
