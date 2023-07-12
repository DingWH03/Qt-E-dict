#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <stack>

extern const int ORDER;

// 单词数据结构
struct wordEntry {
    std::string word;
    std::string phonetic;
    std::string definition;
    std::string translation;
    std::string pos;
    std::string collins;
    std::string oxford;
    std::string tag;
    std::string bnc;
    std::string frq;
    std::string exchange;
    std::string detail;
    std::string audio;
};

// B+树节点的数据结构
struct BPlusTreeNode
{
    std::vector<std::string> keys;         // 节点包含的关键字列表
    std::vector<int> values;               // 关键字对应的行号或其他信息
    std::vector<BPlusTreeNode *> children; // 子节点指针列表
    bool isLeaf;                           // 是否为叶子节点

    BPlusTreeNode(bool leaf = false) : isLeaf(leaf) {}
};

// B+树类
class BPlusTree
{
public:
    BPlusTree() : root(nullptr) {}
    BPlusTree(const std::string filename) : root(nullptr)
    {
        this->filename = filename;
        this->indexFilename = filename + ".index";
    }
    ~BPlusTree()
    {
        destroyTree(root);
    }
    int Build();
    void insert(const std::string &key, int value);
    int search(const std::string &key);
    std::string *getsearch(const std::string &key);
    int remove(const std::string &key);
    void Rebuild();
    int InsertToFile(wordEntry newword);
    void sortWordsInFile(const std::string &inputFilename);

private:
    std::string filename;
    std::string indexFilename;
    BPlusTreeNode *root;
    void splitChild(BPlusTreeNode *parent, int index);
    void insertNonFull(BPlusTreeNode *node, const std::string &key, int value);
    int searchNode(const BPlusTreeNode *node, const std::string &key);
    void destroyTree(BPlusTreeNode *node);

    std::string convertToLowerCase(const std::string& word);
};

bool compareWordEntry(const wordEntry& entry1, const wordEntry& entry2);
void insertWord(std::vector<wordEntry>& entries, const wordEntry& newEntry);



#endif // BPLUSTREE_H
