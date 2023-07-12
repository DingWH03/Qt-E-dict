#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include "BPlusTree.h"
#include "TrieTree.cpp"
const int ORDER=14;


// 插入关键字到B+树
void BPlusTree::insert(const std::string &key, int value)
{
    if (root == nullptr)
    {
        root = new BPlusTreeNode(true);
        root->keys.push_back(key);
        root->values.push_back(value);
    }
    else
    {
        if (int(root->keys.size()) == 2 * ORDER - 1)
        {
            BPlusTreeNode *newRoot = new BPlusTreeNode(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(newRoot, 0);
        }
        insertNonFull(root, key, value);
    }
}

// 在非满节点插入关键字
void BPlusTree::insertNonFull(BPlusTreeNode *node, const std::string &key, int value)
{
    int i = node->keys.size() - 1;
    if (node->isLeaf)
    {
        node->keys.push_back("");
        node->values.push_back(0);
        while (i >= 0 && key < node->keys[i])
        {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            --i;
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = value;
    }
    else
    {
        while (i >= 0 && key < node->keys[i])
        {
            --i;
        }
        ++i;
        if (int(node->children[i]->keys.size()) == 2 * ORDER - 1)
        {
            splitChild(node, i);
            if (key > node->keys[i])
            {
                ++i;
            }
        }
        insertNonFull(node->children[i], key, value);
    }
}

// 分裂子节点
void BPlusTree::splitChild(BPlusTreeNode *parent, int index)
{
    BPlusTreeNode *child = parent->children[index];
    BPlusTreeNode *newNode = new BPlusTreeNode(child->isLeaf);

    parent->keys.insert(parent->keys.begin() + index, child->keys[ORDER - 1]);
    parent->values.insert(parent->values.begin() + index, child->values[ORDER - 1]);
    parent->children.insert(parent->children.begin() + index + 1, newNode);

    newNode->keys.assign(child->keys.begin() + ORDER, child->keys.end());
    newNode->values.assign(child->values.begin() + ORDER, child->values.end());
    child->keys.resize(ORDER - 1);
    child->values.resize(ORDER - 1);

    if (!child->isLeaf)
    {
        newNode->children.assign(child->children.begin() + ORDER, child->children.end());
        child->children.resize(ORDER);
    }
}

int BPlusTree::Build()
{
    // 从词库文件中读取单词数据，并插入到B+树中
    std::ifstream wordFile(filename);
    if (wordFile.is_open())
    {
        std::string line;
        std::string word;
        std::istringstream iss;
        int lineNumber = 1;
        while (std::getline(wordFile, line))
        {
            iss.str(line);
            std::getline(iss, word, ',');
            word=convertToLowerCase(word);
            insert(word, lineNumber);
            ++lineNumber;
        }
        wordFile.close();
        return 1;
    }
    else
    {
        return 0;
    }
}

// 搜索关键字在B+树中的位置
int BPlusTree::search(const std::string &key)
{
    std::string ss = convertToLowerCase(key);
//    std::replace(ss.begin(), ss.end(), ',', '{');

    return searchNode(root, ss);
}

std::string *BPlusTree::getsearch(const std::string &key)
{
    std::string ss = convertToLowerCase(key);
//    std::replace(ss.begin(), ss.end(), ',', '{');
    std::cout<<ss<<std::endl;
    int lineNumber = searchNode(root, ss);
    std::string *information = new std::string[13];
    if (lineNumber != -1)
    {
        std::ifstream wordFile(filename);

        if (wordFile.is_open())
        {
            std::istringstream iss;
            std::string line;
            std::string word, phonetic, definition, translation, pos, collins, oxford, tag, bnc, frq, exchange, detail, audio;
            int currentLine = 1;
            while (std::getline(wordFile, line))
            {
                if (currentLine == lineNumber)
                {
                    iss.str(line);

                    std::getline(iss, information[0], ',');
                    std::getline(iss, information[1], ',');
                    std::getline(iss, information[2], ',');
                    std::getline(iss, information[3], ',');
                    std::getline(iss, information[4], ',');
                    std::getline(iss, information[5], ',');
                    std::getline(iss, information[6], ',');
                    std::getline(iss, information[7], ',');
                    std::getline(iss, information[8], ',');
                    std::getline(iss, information[9], ',');
                    std::getline(iss, information[10], ',');
                    std::getline(iss, information[11], ',');
                    std::getline(iss, information[12], '\n');
                    break;
                }
                ++currentLine;
            }
            wordFile.close();
        }

    }

    char target = '{';
    char replacement = ',';
    for (int i = 0; i < 13; i++) {
        std::replace(information[i].begin(), information[i].end(), target, replacement);
        std::replace(information[i].begin(), information[i].end(), '\\', '\n');
    }
    std::cout<<information[3]<<std::endl;
    return information;
}

// 在节点中搜索关键字
int BPlusTree::searchNode(const BPlusTreeNode *node, const std::string &key)
{
    int i = 0;
    while (i < int(node->keys.size()) && key > node->keys[i])
    {
        ++i;
    }
    if (i < int(node->keys.size()) && key == node->keys[i])
    {
        return node->values[i];
    }
    else if (node->isLeaf)
    {
        return -1; // 未找到关键字
    }
    else
    {
        return searchNode(node->children[i], key);
    }
}

int BPlusTree::InsertToFile(wordEntry newword)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        return 0;
    }
    std::vector<wordEntry> entries;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word, phonetic, definition, translation, pos, collins, oxford, tag, bnc, frq, exchange, detail, audio;
        std::getline(iss, word, ',');
        std::getline(iss, phonetic, ',');
        std::getline(iss, definition, ',');
        std::getline(iss, translation, ',');
        std::getline(iss, pos, ',');
        std::getline(iss, collins, ',');
        std::getline(iss, oxford, ',');
        std::getline(iss, tag, ',');
        std::getline(iss, bnc, ',');
        std::getline(iss, frq, ',');
        std::getline(iss, exchange, ',');
        std::getline(iss, detail, ',');
        std::getline(iss, audio, '\n');
        entries.push_back({ word, phonetic, definition, translation, pos, collins, oxford, tag, bnc, frq, exchange, detail, audio });
    }
    file.close();
    //备份文件
    std::rename(filename.c_str(), (filename+".bak").c_str());
    destroyTree(root);
    root = nullptr;
    int lineNumber = 1;
    wordEntry newWord = newword;
    insertWord(entries, newWord);
    std::ofstream output(filename);
    if (!output.is_open()) {
        return 0;
    }

    for (const auto& entry : entries) {
        output << entry.word << ',' << entry.phonetic << ',' << entry.definition << ',' << entry.translation << ',' << entry.pos << ',' << entry.collins << ',' << entry.oxford << ',' << entry.tag << ',' << entry.bnc << ',' << entry.frq << ',' << entry.exchange << ',' << entry.detail << ',' << entry.audio << '\n';
        insert(convertToLowerCase(entry.word), lineNumber++);
    }

    output.close();
    return 1;
}

void BPlusTree::Rebuild()
{
    destroyTree(root);
    root = nullptr;
    Build();
}

// 删除关键字
int BPlusTree::remove(const std::string &key)
{
    if (root == nullptr)
    {
        return 0;
    }

    int lineNumber = search(key);
    if (lineNumber != -1)
    {
        std::ifstream inputFile(filename);
        if (inputFile.is_open())
        {
            std::vector<std::string> lines;
            std::string line;
            int currentLine = 1;

            // 读取文件的每一行，并将其存储在向量中
            while (std::getline(inputFile, line))
            {
                if (currentLine == lineNumber)
                {
                    // 忽略需要删除的行
                    currentLine++;
                    continue;
                }
                lines.push_back(line);
                currentLine++;
            }
            inputFile.close();
            //备份文件
            std::rename(filename.c_str(), (filename+".bak").c_str());
            // 将修改后的内容写回文件
            std::ofstream outputFile(filename, std::ofstream::trunc);
            if (outputFile.is_open())
            {
                for (const auto &line : lines)
                {
                    outputFile << line << std::endl;
                }
                outputFile.close();
                Rebuild();
                return lineNumber;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


void BPlusTree::destroyTree(BPlusTreeNode *root)
{
    if (root == nullptr)
    {
        return;
    }

    std::stack<BPlusTreeNode *> nodeStack;
    nodeStack.push(root);

    while (!nodeStack.empty())
    {
        BPlusTreeNode *current = nodeStack.top();
        nodeStack.pop();

        if (!current->isLeaf)
        {
            for (BPlusTreeNode *child : current->children)
            {
                if (child != nullptr)
                {
                    nodeStack.push(child);
                }
            }
        }

        delete current;
    }
}



//转换成小写
std::string BPlusTree::convertToLowerCase(const std::string& word)
{
    std::string result = word;
    const int length = word.length();

    for (int i = 0; i < length; ++i)
    {
        if (result[i] >= 'A' && result[i] <= 'Z')
        {
            result[i] |= 32;
        }
    }

    return result;
}


bool compareWordEntry(const wordEntry& entry1, const wordEntry& entry2) {
    return entry1.word < entry2.word;
}

void insertWord(std::vector<wordEntry>& entries, const wordEntry& newEntry) {
    auto it = std::lower_bound(entries.begin(), entries.end(), newEntry, compareWordEntry);
    entries.insert(it, newEntry);
}


