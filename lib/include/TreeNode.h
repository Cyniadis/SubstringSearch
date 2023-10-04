#ifndef TREENODE_H
#define TREENODE_H

#include <algorithm>
#include <climits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


class TreeNodeBase
{
public:
    TreeNodeBase() : _letter('\0') {}
    TreeNodeBase(char letter) : _letter(letter) {}
    virtual ~TreeNodeBase() = default;

    virtual bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) = 0;
    virtual void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) = 0;

    inline char getLetter() const { return _letter; }
    inline bool isLetterValid() { return (_letter != '\0'); }

    inline std::string getWord() const { return _word; }
    inline void setWord(const std::string& newWord) { _word = newWord; }

    virtual void visitTree(std::function<void(std::shared_ptr<TreeNodeBase> &)> func) = 0;
    virtual void visitPartialTree(unsigned nbSplits,
                                  unsigned idx,
                                  std::function<void(std::shared_ptr<TreeNodeBase> &)> func) = 0;


protected:
    std::string _word;
    char _letter;

};

//////////////////////////////////////////////////////////////////

template <typename ContainerType>
class TreeNode : public TreeNodeBase
{
public:
    TreeNode() : TreeNodeBase() {}
    TreeNode(char letter) : TreeNodeBase(letter) {}
    virtual ~TreeNode() = default;

    ContainerType getNextLetters() const { return _nextLetters; }

    virtual void getPartialNextLetters(unsigned nbSplits,
                                       unsigned idx,
                                       typename ContainerType::iterator &start,
                                       typename ContainerType::iterator &end)
    {
        if( nbSplits <= 1 ) {
            start = std::begin(_nextLetters);
            end = std::end(_nextLetters);
            return;
        }

        int step = (_nextLetters.size() / nbSplits); //add check if step = 0;

        start = std::begin(_nextLetters);
        std::advance(start, (idx * step));

        if (idx == nbSplits - 1) {
            end = std::end(_nextLetters);
        } else {
            end = start;
            std::advance(start, step);
        }
    }

    virtual void visitTree(std::function<void(std::shared_ptr<TreeNodeBase> &)> func)
    {
        visitPartialTree(_nextLetters.begin(), _nextLetters.end(), func);
    }

    virtual void visitPartialTree(unsigned nbSplits,
                          unsigned idx,
                          std::function<void(std::shared_ptr<TreeNodeBase> &)> func)
    {
        typename ContainerType::iterator start, end;
        this->getPartialNextLetters(nbSplits, idx, start, end);
        visitPartialTree(start, end, func);
    }

    virtual void visitPartialTree(typename ContainerType::iterator start,
                                  typename ContainerType::iterator end,
                                  std::function<void(std::shared_ptr<TreeNodeBase> &)> func) = 0;

protected:
    ContainerType _nextLetters;
};

//////////////////////////////////////////////////////////////////

class TreeNodeHashTable : public TreeNode<std::unordered_map<char, std::shared_ptr<TreeNodeHashTable>>>
{
public:
    TreeNodeHashTable() : TreeNode() {}
    TreeNodeHashTable(char letter) : TreeNode(letter) {}
    ~TreeNodeHashTable() = default;

    bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) override
    {
        treeNode = std::shared_ptr<TreeNodeBase>(_nextLetters[letter]);
        return true;
    }

    void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) override
    {
        auto res = _nextLetters.insert({letter, std::make_shared<TreeNodeHashTable>(letter)});
        childNode = res.first->second;
    }

    void visitPartialTree(typename std::unordered_map<char, std::shared_ptr<TreeNodeHashTable>>::iterator start,
                          typename std::unordered_map<char, std::shared_ptr<TreeNodeHashTable>>::iterator end,
                          std::function<void(std::shared_ptr<TreeNodeBase> &)> func) override
    {
        for( auto it = start; it != end; ++it )
        {
            std::shared_ptr<TreeNodeBase> node = it->second;
            func(node);
            it->second->visitTree(func);
        }
    }
};

//////////////////////////////////////////////////////////////////

class TreeNodeVector : public TreeNode<std::vector<std::shared_ptr<TreeNodeVector>>>
{
public:
    TreeNodeVector() : TreeNode() {}
    TreeNodeVector(char letter) : TreeNode(letter) {}
    ~TreeNodeVector() = default;

    bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) override
    {
        treeNode = _nextLetters[letter];
        return true;
    }

    void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) override
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

    void visitPartialTree(typename std::vector<std::shared_ptr<TreeNodeVector>>::iterator start,
                          typename std::vector<std::shared_ptr<TreeNodeVector>>::iterator end,
                          std::function<void(std::shared_ptr<TreeNodeBase> &)> func) override
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
};

//////////////////////////////////////////////////////////////////

class TreeNodeVectorCompact : public TreeNode<std::vector<std::shared_ptr<TreeNodeVectorCompact>>>
{
public:
    TreeNodeVectorCompact() : TreeNode() {}
    TreeNodeVectorCompact(char letter) : TreeNode(letter) {}
    ~TreeNodeVectorCompact() = default;

    bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) override
    {
        auto compareFunc = [&](const std::shared_ptr<TreeNodeVectorCompact> &treeNode) { return (treeNode->getLetter() == letter); };

        auto it = std::find_if(_nextLetters.begin(), _nextLetters.end(), compareFunc);
        if (it == _nextLetters.end()) {
            return false;
        }
        treeNode = (*it);
        return true;
    }

    void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) override
    {
        if( !getNodeByLetter(letter, childNode) ) {
            _nextLetters.push_back(std::make_shared<TreeNodeVectorCompact>(letter));
            childNode = _nextLetters.back();
        }
    }


    void visitPartialTree(typename std::vector<std::shared_ptr<TreeNodeVectorCompact>>::iterator start,
                          typename std::vector<std::shared_ptr<TreeNodeVectorCompact>>::iterator end,
                          std::function<void(std::shared_ptr<TreeNodeBase> &)> func) override
    {
        for( auto it = start; it != end; ++it )
        {
            std::shared_ptr<TreeNodeBase> node = (*it);
            func(node);
            (*it)->visitTree(func);
        }
    }
};

#endif // !TREENODE_H
