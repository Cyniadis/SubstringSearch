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
    std::function<void(std::shared_ptr<TreeNodeBase> &)> func = std::bind(&TreeSearchParallel::saveWordSafe, this, std::placeholders::_1, std::ref(foundWords));
    treeNode->visitPartialTree(nbSplits, idx, func);
}


void TreeSearchParallel::saveAllSubtreeWords(const std::shared_ptr<TreeNodeBase> &treeNode,
                                             std::vector<std::string> &foundWords)
{
    if( !treeNode ) {
        return;
    }

    if( !treeNode->getWord().empty() ) {
        saveWord( const_cast<std::shared_ptr<TreeNodeBase>&>(treeNode), foundWords);
        return;
    }

    if( _nbThreads <= 1 ) {

        if( _useDirectInsert ) {
            runSearchDirectInsert(treeNode, 1, 1, foundWords);
        }
        else {
            runSearchBatchedInsert(treeNode, 1, 1, foundWords);
        }
    }

    for( unsigned i = 0; i < _nbThreads; ++i )
    {
        std::function<void(void)> func;

        if( _useDirectInsert ) {
            func = std::bind(&TreeSearchParallel::runSearchDirectInsert, this, treeNode, _nbThreads, i, std::ref(foundWords));
        }
        else {
            func = std::bind(&TreeSearchParallel::runSearchBatchedInsert, this, treeNode, _nbThreads, i, std::ref(foundWords));
        }
        _threadPool.runThread(func);
    }
    _threadPool.joinAllThreads();
}





