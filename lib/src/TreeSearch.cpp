#include "TreeSearch.h"

#include <fstream>

TreeSearch::TreeSearch(unsigned int nbChar)
    : _nbChar(nbChar)
{
    _searchTree = std::make_unique<TreeNode>();
}

bool TreeSearch::loadWordList(const std::string &wordListPath)
{
    std::ifstream iFile(wordListPath);
    if( !iFile ) {
        return false;
    }

    std::string word;
    while( std::getline(iFile, word, '\n') )
    {
        addWordToTree(word, _nbChar, _searchTree, 0);
    }
    return true;
}

std::vector<std::string> TreeSearch::searchWord(const std::string &subStr)
{
    std::vector<std::string> foundWords;

    if( subStr.empty() ) {
        return std::vector<std::string>();
    }

    // TODO: add checks
    std::vector<std::unique_ptr<TreeNode>>::iterator treeIt = _searchTree->_nextLetters.begin() + (subStr[0] - 'A');
    // Optimize for 1, 2 letters
    for( int i = 1; i < subStr.size(); ++i )
    {
        char letter = subStr[i];
        treeIt = (*treeIt)->_nextLetters.begin() + (letter - 'A');
    }

    if( (*treeIt) ) {
        searchWordRec(subStr, (*treeIt), foundWords);
    }
    return foundWords;
}

void TreeSearch::searchWordRec(const std::string &subStr, std::unique_ptr<TreeNode>& treeNode, std::vector<std::string>& foundWords)
{
    if( treeNode->_nextLetters.empty() ) // Is leaf
    {
        foundWords.push_back(treeNode->_word);
    }
    else {
        for( int i = 0; i < treeNode->_nextLetters.size(); ++i ) {
            searchWordRec(subStr, treeNode->_nextLetters[i], foundWords);
        }
    }
}

void TreeSearch::addWordToTree(const std::string &word, unsigned nbChar, std::unique_ptr<TreeNode>& treeNode, int  letterIdx)
{
    const char letter = word[letterIdx];
    int l = letter - 'A';

    if( letterIdx == word.size() ) { // Leaf
        treeNode->_word = word;
        return;
    }

    if( treeNode->_nextLetters.empty() ) {
        treeNode->_nextLetters.resize(nbChar);

    }
    if( !treeNode->_nextLetters[l] ) {
        treeNode->_nextLetters[l] = std::make_unique<TreeNode>(letter);
    }
    addWordToTree(word, nbChar, treeNode->_nextLetters[l], letterIdx+1);

}
