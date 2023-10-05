#include "TreeNode.h"

TreeNodeMap::TreeNodeMap()
    : TreeNode()
{}

TreeNodeMap::TreeNodeMap(char letter) : TreeNode(letter) {}

bool TreeNodeMap::getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase> &treeNode)
{
    treeNode = std::shared_ptr<TreeNodeBase>(_nextLetters[letter]);
    return true;
}

void TreeNodeMap::addChildNode(char letter, std::shared_ptr<TreeNodeBase> &childNode)
{
    auto res = _nextLetters.insert({letter, std::make_shared<TreeNodeMap>(letter)});
    childNode = res.first->second;
}

void TreeNodeMap::visitPartialTree(MapType::iterator start, MapType::iterator end, std::function<void (std::shared_ptr<TreeNodeBase> &)> &func)
{
    for( auto it = start; it != end; ++it )
    {
        std::shared_ptr<TreeNodeBase> node = it->second;
        func(node);
        it->second->visitTree(func);
    }
}

//////////////////////////////////////////////////////////////////

TreeNodeVector::TreeNodeVector()
    : TreeNode()
{}

TreeNodeVector::TreeNodeVector(char letter) : TreeNode(letter) {}

bool TreeNodeVector::getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase> &treeNode)
{
    treeNode = _nextLetters[letter];
    return true;
}

void TreeNodeVector::addChildNode(char letter, std::shared_ptr<TreeNodeBase> &childNode)
{
    _nextLetters.resize(UCHAR_MAX + 1);
    if( _nextLetters[letter] ) {
        childNode = _nextLetters[letter];
        return;
    }
    _nextLetters[letter] = std::make_shared<TreeNodeVector>(letter);
    childNode = _nextLetters[letter];
    return;
}

void TreeNodeVector::visitPartialTree(std::vector<std::shared_ptr<TreeNodeVector>>::iterator start, std::vector<std::shared_ptr<TreeNodeVector>>::iterator end, std::function<void (std::shared_ptr<TreeNodeBase> &)> &func)
{
    for( auto it = start; it != end; ++it )
    {
        std::shared_ptr<TreeNodeBase> node = (*it);
        if (node)
        {
            func(node);
            node->visitTree(func);
        }
    }
}

//////////////////////////////////////////////////////////////////

TreeNodeVectorCompact::TreeNodeVectorCompact()
    : TreeNode()
{}

TreeNodeVectorCompact::TreeNodeVectorCompact(char letter) : TreeNode(letter) {}

bool TreeNodeVectorCompact::getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase> &treeNode)
{
    auto compareFunc = [&](const std::shared_ptr<TreeNodeVectorCompact> &treeNode) { return (treeNode->getLetter() == letter); };

    auto it = std::find_if(_nextLetters.begin(), _nextLetters.end(), compareFunc);
    if (it == _nextLetters.end()) {
        return false;
    }
    treeNode = (*it);
    return true;
}

void TreeNodeVectorCompact::addChildNode(char letter, std::shared_ptr<TreeNodeBase> &childNode)
{
    if( !getNodeByLetter(letter, childNode) ) {
        _nextLetters.push_back(std::make_shared<TreeNodeVectorCompact>(letter));
        childNode = _nextLetters.back();
    }
}

void TreeNodeVectorCompact::visitPartialTree(std::vector<std::shared_ptr<TreeNodeVectorCompact>>::iterator start, std::vector<std::shared_ptr<TreeNodeVectorCompact>>::iterator end, std::function<void (std::shared_ptr<TreeNodeBase> &)>& func)
{
    for( auto it = start; it != end; ++it )
    {
        std::shared_ptr<TreeNodeBase> node = std::dynamic_pointer_cast<TreeNodeBase>(*it);
        func(node);
        (*it)->visitTree(func);
    }
}
