#ifndef TREESEARCHPARALLEL_H
#define TREESEARCHPARALLEL_H

#include "SpinLock.h"
#include "ThreadPool.h"
#include "TreeSearch.h"
#include <mutex>

/**
 * @brief TreeSearchParallel : Multithread prefix search
 */
class TreeSearchParallel : public TreeSearch
{

public:
    /**
     * @brief TreeSearchParallel
     * @param{ nbThreads
     * @param{ searchTree
     * @param[in] useDirectInsert
     */
    TreeSearchParallel(unsigned nbThreads, std::shared_ptr<TreeNodeBase> searchTree, bool useDirectInsert);
    ~TreeSearchParallel() = default;

private:
    ThreadPool _threadPool;
    unsigned _nbThreads;

//    std::mutex _mutex;
    SpinLock _mutex;
    bool _useDirectInsert;

private:
    /**
     * @brief runSearchBatchedInsert : Function executed by thread. Perform direct insert search in the list defined by given parameters.     *
     *                                 Range is defined by [idx * size() / nbSplits, (idx+1) * size() / nbSplits  ]
     * @param treeNode : Tree to start the search
     * @param[in] nbSplits : Number of part
     * @param[in] idx: : Index of part
     * @param[out] foundWords : Vector of found words
     */
    void runSearchBatchedInsert(const std::shared_ptr<TreeNodeBase>& treeNode, unsigned int nbSplits, unsigned int idx, std::vector<std::string>& foundWords);
    /**
     * @brief runSearchDirectInsert : Function executed by thread. Perform direct batched search in the list defined by given params
     *                                Range is defined by [idx * size() / nbSplits, (idx+1) * size() / nbSplits  ]
     * @param[in] treeNode : Tree to start the search
     * @param[in] nbSplits : Number of part
     * @param[in] idx: : Index of part
     * @param[out] foundWords : Vector of found words
     */
    void runSearchDirectInsert(const std::shared_ptr<TreeNodeBase>& treeNode, unsigned int nbSplits, unsigned int idx, std::vector<std::string>& foundWords);
    /**
     * @brief saveWord : If exists, safely add the node's word in the output vector.
     * @param[in] treeNode : Tree node storing the word to save
     * @param[in/out] foundWords : Vector where to save the node's word
     */
    void saveWordSafe(std::shared_ptr<TreeNodeBase>& treeNode, std::vector<std::string>& foundWords);
    /**
     * @brief saveAllSubtreeWords : Recusively save all words to the given vector starting from the given tree node
     * @param[in] treeNode : Tree node storing the word to save
     * @param[in/out] foundWords : Vector where to save the node's word
     */
    void saveAllSubtreeWords(const std::shared_ptr<TreeNodeBase>& treeNode, std::vector<std::string>& foundWords);
};

#endif // TREESEARCHPARALLEL_H
