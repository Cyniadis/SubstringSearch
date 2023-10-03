#include "TreeSearch.h"

#include <algorithm>
#include <fstream>
#include <limits.h>

TreeSearch::TreeSearch(bool compactTree)
    : _compactTree(compactTree)
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
        addWordToTree(word, _searchTree, 0);
    }
    return true;
}

std::vector<std::string> TreeSearch::searchWords(const std::string &subStr)
{
    std::vector<std::string> foundWords;

    if( subStr.empty() ) {
        return std::vector<std::string>();
    }

    // TODO: add checks
    if (_compactTree)
    {
        searchWordCompact(subStr, _searchTree, foundWords);
    }
    else {
        searchWordPreallocated(subStr, _searchTree, foundWords);
    }


    return foundWords;
}

void TreeSearch::searchWordRec(const std::string &subStr, const std::unique_ptr<TreeNode>& treeNode, std::vector<std::string>& foundWords)
{
    if( treeNode->_nextLetters.empty() ) // Is leaf
    {
        foundWords.push_back(treeNode->_word);
    }
    else {
        for( int i = 0; i < treeNode->_nextLetters.size(); ++i ) {
            if( treeNode->_nextLetters[i] ) {
                searchWordRec(subStr, treeNode->_nextLetters[i], foundWords);
            }
        }
    }
}

std::vector<std::unique_ptr<TreeSearch::TreeNode>>::iterator TreeSearch::findLetter(const std::unique_ptr<TreeNode>& treeNode, const char &letter)
{
    auto compFunc = [&](const std::unique_ptr<TreeNode>& node) { return (node->_letter == letter); };
    auto it = std::find_if(std::begin(treeNode->_nextLetters), std::end(treeNode->_nextLetters), compFunc);
    return it;
}


void TreeSearch::addWordToTree(const std::string &word, const std::unique_ptr<TreeNode>& treeNode, int letterIdx)
{
    if( _compactTree ) {
        addWordToTreeCompact(word, treeNode, letterIdx);
    }
    else {
        addWordToTreePreallocated(word, treeNode, letterIdx);
    }
}
void TreeSearch::addWordToTreePreallocated(const std::string &word, const std::unique_ptr<TreeNode>& treeNode, int letterIdx)
{
    const char l = word[letterIdx];

    if( letterIdx == word.size() ) { // Leaf
        treeNode->_word = word;
        return;
    }

    if( treeNode->_nextLetters.empty() ) {
        treeNode->_nextLetters.resize(UCHAR_MAX+1);

    }
    if( !treeNode->_nextLetters[l] ) {
        treeNode->_nextLetters[l] = std::make_unique<TreeNode>(l);
    }
    addWordToTree(word, treeNode->_nextLetters[l], letterIdx+1);

}

void TreeSearch::searchWordPreallocated(const std::string &subStr, const std::unique_ptr<TreeNode> &treeNode, std::vector<std::string> &foundWords)
{
    if( !treeNode || subStr.empty() ) {
        return;
    }

    std::vector<std::unique_ptr<TreeNode>>::iterator treeIt = _searchTree->_nextLetters.begin() + subStr[0];

    for( int i = 1; i < subStr.size() && (*treeIt); ++i )
    {
        char letter = subStr[i];
        treeIt = (*treeIt)->_nextLetters.begin() + letter;
    }

    if( (*treeIt) ) {
        searchWordRec(subStr, (*treeIt), foundWords);
    }
}

void TreeSearch::addWordToTreeCompact(const std::string &word, const std::unique_ptr<TreeNode> &treeNode, int letterIdx)
{
    const char l = word[letterIdx];

    if( letterIdx == word.size() ) { // Leaf
        treeNode->_word = word;
        return;
    }

    auto it = findLetter(treeNode, l);
    if( it == treeNode->_nextLetters.end() ) {
        treeNode->_nextLetters.push_back(std::make_unique<TreeNode>(l));
        addWordToTree(word, treeNode->_nextLetters.back(), letterIdx+1);
    }
    else {
        addWordToTree(word, (*it), letterIdx+1);
    }
}

void TreeSearch::searchWordCompact(const std::string &subStr, const std::unique_ptr<TreeNode> &treeNode, std::vector<std::string> &foundWords)
{
    if( !treeNode || subStr.empty() ) {
        return;
    }

    std::vector<std::unique_ptr<TreeNode>>::iterator treeIt = findLetter(_searchTree, subStr[0]);

    if( treeIt == _searchTree->_nextLetters.end()) {
        return;
    }

    for( int i = 1; i < subStr.size(); ++i )
    {
        char letter = subStr[i];
        treeIt = findLetter((*treeIt), letter);
    }

    if( (*treeIt) ) {
        searchWordRec(subStr, (*treeIt), foundWords);
    }
}

