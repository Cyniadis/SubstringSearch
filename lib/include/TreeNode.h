#ifndef TREENODE_H
#define TREENODE_H

#include <algorithm>
#include <climits>
#include <functional>
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

    virtual void visitTree(std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) = 0;
    virtual void visitPartialTree(unsigned nbSplits,
                                  unsigned idx,
                                  std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) = 0;


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
        if( nbSplits <= 1  || _nextLetters.size() <= nbSplits) {
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
            std::advance(end, step);
        }
    }

    virtual void visitTree(std::function<void(std::shared_ptr<TreeNodeBase> &)>& func)
    {
        visitPartialTree(_nextLetters.begin(), _nextLetters.end(), func);
    }

    virtual void visitPartialTree(unsigned nbSplits,
                                  unsigned idx,
                                  std::function<void(std::shared_ptr<TreeNodeBase> &)>& func)
    {
        typename ContainerType::iterator start, end;
        this->getPartialNextLetters(nbSplits, idx, start, end);
        visitPartialTree(start, end, func);
    }

    virtual void visitPartialTree(typename ContainerType::iterator start,
                                  typename ContainerType::iterator end,
                                  std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) = 0;

protected:
    ContainerType _nextLetters;
};

//////////////////////////////////////////////////////////////////

class TreeNodeHashTable : public TreeNode<std::unordered_map<char, std::shared_ptr<TreeNodeHashTable>>>
{
public:
    using MapType = std::unordered_map<char, std::shared_ptr<TreeNodeHashTable>>;

    TreeNodeHashTable();
    TreeNodeHashTable(char letter);
    ~TreeNodeHashTable() = default;

    bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) override;

    void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) override;

    void visitPartialTree(typename MapType::iterator start,
                          typename MapType::iterator end,
                          std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) override;
};

//////////////////////////////////////////////////////////////////

class TreeNodeVector : public TreeNode<std::vector<std::shared_ptr<TreeNodeVector>>>
{
public:
    TreeNodeVector();
    TreeNodeVector(char letter);
    ~TreeNodeVector() = default;

    bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) override;

    void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) override;

    void visitPartialTree(typename std::vector<std::shared_ptr<TreeNodeVector>>::iterator start,
                          typename std::vector<std::shared_ptr<TreeNodeVector>>::iterator end,
                          std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) override;
};

//////////////////////////////////////////////////////////////////

class TreeNodeVectorCompact : public TreeNode<std::vector<std::shared_ptr<TreeNodeVectorCompact>>>
{
public:
    TreeNodeVectorCompact();
    TreeNodeVectorCompact(char letter);
    ~TreeNodeVectorCompact() = default;

    bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) override;

    void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) override;


    void visitPartialTree(typename std::vector<std::shared_ptr<TreeNodeVectorCompact>>::iterator start,
                          typename std::vector<std::shared_ptr<TreeNodeVectorCompact>>::iterator end,
                          std::function<void (std::shared_ptr<TreeNodeBase> &)> &func) override;
};

#endif // !TREENODE_H
