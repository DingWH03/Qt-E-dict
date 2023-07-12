#ifndef TRIETREE_H
#define TRIETREE_H

#include <vector>
#include <string>

extern const int ALPHABET_SIZE;

// Trie节点定义
struct TrieNode
{
    bool isEndOfWord;
    std::vector<TrieNode *> children;

    TrieNode()
    {
        isEndOfWord = false;
        children = std::vector<TrieNode *>(ALPHABET_SIZE, nullptr);
    }
};

// Trie树定义
class TrieTree
{
    TrieNode *root;

public:
    TrieTree()
    {
        root = new TrieNode();
    }
    TrieTree(const std::string filename)
    {
        this->filename = filename;
        root = new TrieNode();
    }
    // 析构函数
    ~TrieTree()
    {
        destroyTrie(root);
    }

    // 插入一个单词到Trie树
    void insert(const std::string &word);
    // 从文件中建立TrieTree
    int build();
    // 移除一个单词
    void remove(const std::string key);

    // 搜索模糊匹配的单词
    std::vector<std::string> search(std::string word);

private:
    std::string filename;
    bool isNodeDeletable(TrieNode *node);
    // 辅助函数：从给定节点开始进行深度优先搜索
    void searchHelper(TrieNode *node, std::string &prefix, std::vector<std::string> &result, int& count);
    // 递归释放Trie树节点的内存
    void destroyTrie(TrieNode *node);
    std::string convertToLowerCase(const std::string &word);
};

#endif
