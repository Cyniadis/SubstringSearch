#include "TreeSearchParallel.h"

TreeSearchParallel::TreeSearchParallel(unsigned nbThreads, bool compactTree)
    : TreeSearch(compactTree),
    _threadPool(nbThreads),
    _nbThreads(nbThreads)
{

}

void TreeSearchParallel::runSearchWord(const std::string &subStr, const std::unique_ptr<TreeNode> &treeNode, TreeNodeVecConstIterator startIt, TreeNodeVecConstIterator endIt)
{
    if( treeNode->_nextLetters.empty() ) // Is leaf
    {
        _mutex.lock();
        this->_foundWords.push_back(treeNode->_word);
        _mutex.unlock();
    }

    for( auto it = startIt; it != endIt; ++it )
    {
        if( (*it) ) {
            runSearchWord(subStr, (*it), (*it)->_nextLetters.begin(), (*it)->_nextLetters.end());
        }
    }
}

void TreeSearchParallel::searchWordRec(const std::string &subStr, const std::unique_ptr<TreeNode> &treeNode, std::vector<std::string> &foundWords)
{
    if( !treeNode || treeNode->_nextLetters.empty() ) {
        return;
    }

    for( int i = 0; i < _nbThreads; ++i )
    {
        int step = (treeNode->_nextLetters.size() / _nbThreads); //add check if step = 0;
        TreeNodeVecConstIterator startIt = std::begin(treeNode->_nextLetters) + i * step;
        TreeNodeVecConstIterator endIt = (i == _nbThreads -1 ) ? treeNode->_nextLetters.end() : (startIt + step);

        std::function<void(void)> func = std::bind(&TreeSearchParallel::runSearchWord, this, subStr, std::ref(treeNode), startIt, endIt);
        _threadPool.runThread(func);
    }
    _threadPool.jointAllThreads();
    foundWords = std::move(this->_foundWords);
}




