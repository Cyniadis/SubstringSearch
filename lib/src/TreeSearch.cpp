#include "TreeSearch.h"
#include <algorithm>
#include <fstream>
#include <limits.h>

TreeSearch::TreeSearch(std::shared_ptr<TreeNodeBase> searchTree)
    : _searchTree(searchTree),
    _lastPrefixTree(searchTree)
{}

bool TreeSearch::loadWordList(const std::string &wordListPath)
{
    std::ifstream iFile(wordListPath);
    if (!iFile) {
        return false;
    }

    std::string word;
    while (std::getline(iFile, word, '\n')) {
        addWordToTree(word, _searchTree, 0);
    }
    return true;
}

std::vector<std::string> TreeSearch::searchWords(const std::string &subStr, std::shared_ptr<TreeNodeBase>& treeNode)
{
    std::vector<std::string> foundWords;
    saveAllSubtreeWords(treeNode, foundWords);
    return foundWords;
}

std::vector<std::string> TreeSearch::incrementalSearchWords(const std::string &subStr, int idx, bool add)
{
    if( subStr.size() == 0) {
        return std::vector<std::string>();
    }

    /*
    if( add )
    {
        if( idx == subStr.size() - 1 ) // add at the end
        {
            // All the new words are included in the current list
            _lastPrefixTree = _treeSearch->findPrefixSubTree(_lastPrefixTree, subStr, idx);
            saveAllSubtreeWords(_lastPrefixTree, _currentWordList );
        }
        else
        { // 1st letter or add at the begginin or middle
            _lastPrefixTree = _treeSearch->findPrefixSubTree(_lastPrefixTree->getTreeRoot(), subStr, idx);
        }
    }
    _wordList = _treeSearch->searchWords(subStr, _treeNode);
    */
    return searchWords(subStr);
}


std::vector<std::string> TreeSearch::searchWords(const std::string &subStr)
{
    std::vector<std::string> foundWords;
    std::shared_ptr<TreeNodeBase> startNode = findPrefixSubTree(_searchTree, subStr, 0);
    if( startNode == _searchTree ) // No prefix found
    {
        return foundWords;
    }

    return searchWords(subStr, startNode);
}

std::shared_ptr<TreeNodeBase> TreeSearch::findPrefixSubTree(std::shared_ptr<TreeNodeBase> &treeNode,
                                                             const std::string &subStr,
                                                             int letterIdx)
{


    if ((letterIdx == subStr.size() ) && (treeNode != _searchTree)) {
        return treeNode;
    }


    const char l = subStr[letterIdx];
    std::shared_ptr<TreeNodeBase> node;
    if (!treeNode->getNodeByLetter(l, node)) {
        return treeNode;
    }

    return findPrefixSubTree(node, subStr, letterIdx + 1);
}


void TreeSearch::saveWord(std::shared_ptr<TreeNodeBase> &treeNode,
                          std::vector<std::string> &foundWords)
{
    if( !treeNode ) {
        return;
    }
    const std::string& word = treeNode->getWord();
    if (!word.empty()) // Is leaf
    {
        foundWords.push_back(word);
    }
}

void TreeSearch::addWordToTree(const std::string &word,
                               const std::shared_ptr<TreeNodeBase> &treeNode,
                               int letterIdx)
{
    const char l = word[letterIdx];

    if (letterIdx == word.size()) { // Leaf
        treeNode->setWord(word);
        return;
    }

    std::shared_ptr<TreeNodeBase> childTreeNode;
    treeNode->addChildNode(l, childTreeNode);
    addWordToTree(word, childTreeNode, letterIdx + 1);
}

void TreeSearch::saveAllSubtreeWords(const std::shared_ptr<TreeNodeBase> &treeNode,
                                    std::vector<std::string> &foundWords)
{
    std::function<void(std::shared_ptr<TreeNodeBase> &)> saveFunc = std::bind(&TreeSearch::saveWord, this, std::placeholders::_1, std::ref(foundWords));

    if( !treeNode->getWord().empty() ) {
        saveWord( const_cast<std::shared_ptr<TreeNodeBase>&>(treeNode), std::ref(foundWords));
        return;
    }

    treeNode->visitTree(saveFunc);
}
