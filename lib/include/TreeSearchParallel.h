#ifndef TREESEARCHPARALLEL_H
#define TREESEARCHPARALLEL_H

#include "ThreadPool.h"
#include "TreeSearch.h"
#include <mutex>

class TreeSearchParallel : public TreeSearch
{
    using TreeNodeVecConstIterator = std::vector<std::unique_ptr<TreeNode>>::const_iterator;

public:
    TreeSearchParallel(unsigned nbThreads, bool compactTree);

private:
    ThreadPool _threadPool;
    unsigned _nbThreads;
    std::mutex _mutex;
    inline static std::vector<std::string> _foundWords;



    void searchWordRec(const std::string &subStr, const std::unique_ptr<TreeNode> &treeNode, std::vector<std::string> &foundWords) override;
    void runSearchWord(const std::string &subStr,
                       const std::unique_ptr<TreeNode>& treeNode,
                       TreeNodeVecConstIterator startIt,
                       TreeNodeVecConstIterator endIt);


};

#endif // TREESEARCHPARALLEL_H
