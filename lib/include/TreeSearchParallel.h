#ifndef TREESEARCHPARALLEL_H
#define TREESEARCHPARALLEL_H

#include "SpinLock.h"
#include "ThreadPool.h"
#include "TreeSearch.h"
#include <mutex>


class TreeSearchParallel : public TreeSearch
{

public:
    TreeSearchParallel(unsigned nbThreads, std::shared_ptr<TreeNodeBase> searchTree, bool useDirectInsert);
    ~TreeSearchParallel() = default;

private:
    ThreadPool _threadPool;
    unsigned _nbThreads;

//    std::mutex _mutex;
    SpinLock _mutex;
    bool _useDirectInsert;

private:

    void runSearchBatchedInsert(const std::shared_ptr<TreeNodeBase>& treeNode, unsigned int nbSplits, unsigned int idx, std::vector<std::string>& foundWords);
    void saveWordSafe(std::shared_ptr<TreeNodeBase>& treeNode, std::vector<std::string>& foundWords);
    void runSearchDirectInsert(const std::shared_ptr<TreeNodeBase>& treeNode, unsigned int nbSplits, unsigned int idx, std::vector<std::string>& foundWords);
    void saveAllSubtreeWords(const std::shared_ptr<TreeNodeBase>& treeNode, std::vector<std::string>& foundWords);
};

#endif // TREESEARCHPARALLEL_H
