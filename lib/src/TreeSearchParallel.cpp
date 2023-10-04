#include "TreeSearchParallel.h"

TreeSearchParallel::TreeSearchParallel(unsigned int nbThreads, std::shared_ptr<TreeNodeBase> searchTree, bool useDirectInsert)
    : TreeSearch(searchTree),
    _threadPool(nbThreads),
    _nbThreads(nbThreads),
    _useDirectInsert(useDirectInsert)
{

}

void TreeSearchParallel::runSearchBatchedInsert(const std::shared_ptr<TreeNodeBase> &treeNode,
                                                unsigned nbSplits,
                                                unsigned idx,
                                                std::vector<std::string>& foundWords)
{
    std::vector<std::string> partFoundWords;
    std::function<void(std::shared_ptr<TreeNodeBase> &)> func = std::bind(&TreeSearchParallel::saveWord, this, std::placeholders::_1, std::ref(partFoundWords));
    treeNode->visitPartialTree(nbSplits, idx, func);
    _mutex.lock();
    foundWords.insert(foundWords.end(), std::make_move_iterator(partFoundWords.begin()), std::make_move_iterator(partFoundWords.end()));
    _mutex.unlock();
}

void TreeSearchParallel::saveWordSafe(std::shared_ptr<TreeNodeBase> &treeNode,
                                      std::vector<std::string> &foundWords)
{
    const std::string& word = treeNode->getWord();
    if (!word.empty()) // Is leaf
    {
        _mutex.lock();
        foundWords.push_back(word);
        _mutex.unlock();
    }
}



void TreeSearchParallel::runSearchDirectInsert(const std::shared_ptr<TreeNodeBase> &treeNode,
                                                unsigned nbSplits,
                                                unsigned idx,
                                                std::vector<std::string>& foundWords)
{
    std::function<void(std::shared_ptr<TreeNodeBase> &)> func = std::bind(&TreeSearchParallel::saveWordSafe, this, std::placeholders::_1, foundWords);
    treeNode->visitPartialTree(nbSplits, idx, func);
}


void TreeSearchParallel::saveAllSubtreeWords(const std::shared_ptr<TreeNodeBase> &treeNode,
                                             std::vector<std::string> &foundWords)
{
    if( !treeNode ) {
        return;
    }

    for( unsigned i = 0; i < _nbThreads; ++i )
    {
        std::function<void(void)> func;

        if( _useDirectInsert ) {
            func = std::bind(&TreeSearchParallel::runSearchDirectInsert, this, treeNode, _nbThreads, i, foundWords);
        }
        else {
            func = std::bind(&TreeSearchParallel::runSearchBatchedInsert, this, treeNode, _nbThreads, i, foundWords);
        }
        _threadPool.runThread(func);
    }
    _threadPool.jointAllThreads();
    foundWords = std::move(this->_foundWords);
}





