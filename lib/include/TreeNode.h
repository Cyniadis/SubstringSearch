#ifndef TREENODE_H
#define TREENODE_H

#include <algorithm>
#include <climits>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief TreeNodeBase : Base interface for prefix tree search
 */
class TreeNodeBase
{
public:
    /**
     * @brief TreeNodeBase : Default contructor. Letter is '\0'
     */
    TreeNodeBase() : _letter('\0') {}
    /**
     * @brief TreeNodeBase : Contruct with given letter
     * @param[in] letter
     */
    TreeNodeBase(char letter) : _letter(letter) {}
    virtual ~TreeNodeBase() = default;

    /**
     * @brief getNodeByLetter : Return the node defined by the given letter
     * @param[in] letter : Searched letter
     * @param[out] treeNode : Output tree node, nullptr if not found
     * @return true if node if found, false otherwise
     */
    virtual bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) = 0;
    /**
     * @brief addChildNode : Add a new node as a child
     * @param[in] letter : Letter of the new child node
     * @param[out] childNode : Pointer to the new child node
     */
    virtual void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) = 0;
    /**
     * @brief getLetter : Return node's letter
     */
    inline char getLetter() const { return _letter; }
    /**
     * @brief isLetterValid : Check if node's letter is not NUL character
     */
    inline bool isLetterValid() { return (_letter != '\0'); }
    /**
     * @brief getWord : Return node's word
     */
    inline std::string getWord() const { return _word; }
    /**
     * @brief setWord : Set word value
     */
    inline void setWord(const std::string &newWord) { _word = newWord; }

    /**
     * @brief visitTree : Perform a DFS and execute given function on all nodes
     * @param[in] func : Function to execute on nodes
     */
    virtual void visitTree(std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) = 0;
    /**
     * @brief visitPartialTree : Perform a DFS and execute given function parts of the children defined by idxTh over nbSplits
     * @param[in] nbSplits : Number of parts
     * @param[in] idx : Nth parts to perform DFS
     * @param[in] func : Function to execute
     */
    virtual void visitPartialTree(unsigned nbSplits,
                                  unsigned idx,
                                  std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) = 0;



protected:
    std::string _word;
    char _letter;

};

//////////////////////////////////////////////////////////////////
/**
 * @brief TreeNode : Templated tree node class defining the children container
 */
template <typename ContainerType>
class TreeNode : public TreeNodeBase
{
public:
    /**
     * @brief TreeNode : Default contructor. Letter is '\0'
     */
    TreeNode() : TreeNodeBase() {}
    /**
     * @brief TreeNodeBase : Contruct with given letter
     * @param[in] letter
     */
    TreeNode(char letter) : TreeNodeBase(letter) {}
    virtual ~TreeNode() = default;

    /**
     * @brief getPartialNextLetters : Return a start and end iterator corresponding to a range of the cildren container
     *                                Range is defined by [idx * size() / nbSplits, (idx+1) * size() / nbSplits  ]
     *                                Use for multithreading
     * @param[in] nbSplits : Number of part
     * @param[in] idx: : Index of part
     * @param[out] start : Start iterator of output range
     * @param[out] end : End iterator of output range
     */
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
    /**
     * @brief visitTree : Perform a DFS and execute given function on all nodes
     * @param[in] func : Function to execute on nodes
     */
    virtual void visitTree(std::function<void(std::shared_ptr<TreeNodeBase> &)>& func)
    {
        visitPartialTree(_nextLetters.begin(), _nextLetters.end(), func);
    }
    /**
     * @brief visitPartialTree : Perform a DFS and execute given function parts of the children defined by idxTh over nbSplits
     * @param[in] nbSplits : Number of parts
     * @param[in] idx : Nth parts to perform DFS
     * @param[in] func : Function to execute
     */
    virtual void visitPartialTree(unsigned nbSplits,
                                  unsigned idx,
                                  std::function<void(std::shared_ptr<TreeNodeBase> &)>& func)
    {
        typename ContainerType::iterator start, end;
        this->getPartialNextLetters(nbSplits, idx, start, end);
        visitPartialTree(start, end, func);
    }
    /**
     * @brief visitPartialTree : Perform a DFS and execute given function parts of the children defined by given iterators
     * @param[out] start : Start iterator of output range
     * @param[out] end : End iterator of output range
     * @param[in] func : Function to execute
     */
    virtual void visitPartialTree(typename ContainerType::iterator start,
                                  typename ContainerType::iterator end,
                                  std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) = 0;

protected:
    ContainerType _nextLetters;
};

//////////////////////////////////////////////////////////////////
/**
 * @brief TreeNodeMap : tree node that use a unordered map to manage child nodes. Key is letter, Value is child node.
 */
class TreeNodeMap : public TreeNode<std::unordered_map<char, std::shared_ptr<TreeNodeMap>>>
{
public:
    using MapType = std::unordered_map<char, std::shared_ptr<TreeNodeMap>>;

    /**
     * @brief TreeNodeMap : Default constructor. Letter is '\0'
     */
    TreeNodeMap();
    /**
     * @brief TreeNodeMap : Contruct with given letter
     * @param[in] letter
     */
    TreeNodeMap(char letter);
    ~TreeNodeMap() = default;
    /**
     * @brief getNodeByLetter : Return the node defined by the given letter
     * @param[in] letter : Searched letter
     * @param[out] treeNode : Output tree node, nullptr if not found
     * @return true if node if found, false otherwise
     */
    bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) override;
    /**
     * @brief addChildNode : Add a new node as a child
     * @param[in] letter : Letter of the new child node
     * @param[out] childNode : Pointer to the new child node
     */
    void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) override;
    /**
     * @brief visitPartialTree : Perform a DFS and execute given function parts of the children defined by given iterators
     * @param[out] start : Start iterator of output range
     * @param[out] end : End iterator of output range
     * @param[in] func : Function to execute
     */
    void visitPartialTree(typename MapType::iterator start,
                          typename MapType::iterator end,
                          std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) override;
};

/**
 * @brief TreeNodeVector : tree node that use a preallocated vector to manage child nodes and use random access to get the child nodes.
 */
//////////////////////////////////////////////////////////////////

class TreeNodeVector : public TreeNode<std::vector<std::shared_ptr<TreeNodeVector>>>
{
public:
    /**
     * @brief TreeNodeVector : Default constructor. Letter is '\0'
     */
    TreeNodeVector();
    /**
     * @brief TreeNodeVector : Contruct with given letter
     * @param[in] letter
     */
    TreeNodeVector(char letter);
    ~TreeNodeVector() = default;

    /**
     * @brief getNodeByLetter : Return the node defined by the given letter
     * @param[in] letter : Searched letter
     * @param[out] treeNode : Output tree node, nullptr if not found
     * @return true if node if found, false otherwise
     */
    bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) override;
    /**
     * @brief addChildNode : Add a new node as a child
     * @param[in] letter : Letter of the new child node
     * @param[out] childNode : Pointer to the new child node
     */
    void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) override;
    /**
     * @brief visitPartialTree : Perform a DFS and execute given function parts of the children defined by given iterators
     * @param[out] start : Start iterator of output range
     * @param[out] end : End iterator of output range
     * @param[in] func : Function to execute
     */
    void visitPartialTree(typename std::vector<std::shared_ptr<TreeNodeVector>>::iterator start,
                          typename std::vector<std::shared_ptr<TreeNodeVector>>::iterator end,
                          std::function<void(std::shared_ptr<TreeNodeBase> &)>& func) override;
};

//////////////////////////////////////////////////////////////////

/**
 * @brief TreeNodeVectorCompact : tree node that use a vector to manage child nodes. Chidren are inserted when created.
 *                                Use find() to access child nodes
 */
class TreeNodeVectorCompact : public TreeNode<std::vector<std::shared_ptr<TreeNodeVectorCompact>>>
{
public:
    /**
     * @brief TreeNodeVectorCompact : Default constructor. Letter is '\0'
     */
    TreeNodeVectorCompact();
    /**
     * @brief TreeNodeVectorCompact : Contruct with given letter
     * @param[in] letter
     */
    TreeNodeVectorCompact(char letter);
    ~TreeNodeVectorCompact() = default;

    /**
     * @brief getNodeByLetter : Return the node defined by the given letter
     * @param[in] letter : Searched letter
     * @param[out] treeNode : Output tree node, nullptr if not found
     * @return true if node if found, false otherwise
     */
    bool getNodeByLetter(char letter, std::shared_ptr<TreeNodeBase>& treeNode) override;

    /**
     * @brief addChildNode : Add a new node as a child
     * @param[in] letter : Letter of the new child node
     * @param[out] childNode : Pointer to the new child node
     */
    void addChildNode(char letter, std::shared_ptr<TreeNodeBase>& childNode) override;

    /**
     * @brief visitPartialTree : Perform a DFS and execute given function parts of the children defined by given iterators
     * @param[out] start : Start iterator of output range
     * @param[out] end : End iterator of output range
     * @param[in] func : Function to execute
     */
    void visitPartialTree(typename std::vector<std::shared_ptr<TreeNodeVectorCompact>>::iterator start,
                          typename std::vector<std::shared_ptr<TreeNodeVectorCompact>>::iterator end,
                          std::function<void (std::shared_ptr<TreeNodeBase> &)> &func) override;
};

#endif // !TREENODE_H
