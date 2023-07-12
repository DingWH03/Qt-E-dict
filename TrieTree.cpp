#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <sstream>
#include "TrieTree.h"

const int ALPHABET_SIZE = 41;  // 增加的特殊字符个数

// 插入一个单词到Trie树
void TrieTree::insert(const std::string& word)
{
    TrieNode* current = root;

    for (char c : word)
    {
        int index;
        if (std::islower(c))
        {
            index = c - 'a';
        }
        else if (std::isdigit(c))
        {
            index = c - '0' + 26;
        }
        else
        {
            // Handle other special characters as needed
            if (c == ' ')
            {
                index = 36;  // 空格的索引为36
            }
            else if (c == '.')
            {
                index = 37;  // . 的索引为37
            }
            else if (c == ',')
            {
                index = 38;  // " 的索引为38
            }
            else if (c == '\'')
            {
                index = 39;  // ' 的索引为39
            }
            else if (c == '-')
            {
                index = 40;  // - 的索引为40
            }
            else
            {
                // Skip unsupported characters
                continue;
            }
        }
        if (current->children[index] == nullptr)
        {
            current->children[index] = new TrieNode();
        }
        current = current->children[index];
    }

    current->isEndOfWord = true;
}

int TrieTree::build()
{
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
            word = convertToLowerCase(word);
            insert(word);
            ++lineNumber;
        }
        wordFile.close();
        return lineNumber;
    }
    else
    {
        return 0;
    }
}

// 搜索模糊匹配的单词
std::vector<std::string> TrieTree::search(std::string word)
{
    std::vector<std::string> result;
    TrieNode* current = root;

    for (char& c : word)
    {
        if (std::isalpha(c))
        {
            c = std::tolower(c);
        }
    }

    for (char c : word)
    {
        int index;
        if (std::islower(c))
        {
            index = c - 'a';
        }
        else if (std::isdigit(c))
        {
            index = c - '0' + 26;
        }
        else
        {
            // Handle other special characters as needed
            if (c == ' ')
            {
                index = 36;  // 空格的索引为36
            }
            else if (c == '.')
            {
                index = 37;  // . 的索引为37
            }
            else if (c == ',')
            {
                index = 38;  // " 的索引为38
            }
            else if (c == '\'')
            {
                index = 39;  // ' 的索引为39
            }
            else if (c == '-')
            {
                index = 40;  // - 的索引为40
            }
            else
            {
                // Skip unsupported characters
                continue;
            }
        }
        if (current->children[index] == nullptr)
        {
            std::vector<std::string> null;
            return null;  // 没有匹配的前缀
        }
        current = current->children[index];
    }

    // 从匹配的前缀开始进行深度优先搜索
    int count = 0;
    searchHelper(current, word, result, count);

    return result;
}

// 辅助函数：从给定节点开始进行深度优先搜索
void TrieTree::searchHelper(TrieNode* node, std::string& prefix, std::vector<std::string>& result, int& count)
{
    if (count >= 30)  // 达到30个结果，停止递归
    {
        return;
    }
    if (node->isEndOfWord)
    {
        result.push_back(prefix);
        count++;  // 结果数量加一
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != nullptr)
        {
            if (i < 26)
            {
                prefix.push_back('a' + i);
            }
            else if (i < 36)
            {
                prefix.push_back('0' + i - 26);
            }
            else if (i == 36)
            {
                prefix.push_back(' ');  // 空格字符
            }
            else if (i == 37)
            {
                prefix.push_back('.');  // . 字符
            }
            else if (i == 38)
            {
                prefix.push_back(',');  // " 字符
            }
            else if (i == 39)
            {
                prefix.push_back('\'');  // ' 字符
            }
            else if (i == 40)
            {
                prefix.push_back('-');  // - 字符
            }
            searchHelper(node->children[i], prefix, result, count);
            prefix.pop_back();
        }
    }
}

// 从 Trie 树中删除键
void TrieTree::remove(const std::string key)
{
    if (key.empty())
    {
        return;
    }

    TrieNode* current = root;
    std::stack<TrieNode*> nodeStack;
    std::stack<int> indexStack;
    int len = key.length();

    // 找到要删除的键所在的最后一个节点
    for (int i = 0; i < len; i++)
    {
        char c = key[i];
        int index;
        if (std::islower(c))
        {
            index = c - 'a';
        }
        else if (std::isdigit(c))
        {
            index = c - '0' + 26;
        }
        else
        {
            // Handle other special characters as needed
            if (c == ' ')
            {
                index = 36;  // 空格的索引为36
            }
            else if (c == '.')
            {
                index = 37;  // . 的索引为37
            }
            else if (c == ',')
            {
                index = 38;  // " 的索引为38
            }
            else if (c == '\'')
            {
                index = 39;  // ' 的索引为39
            }
            else if (c == '-')
            {
                index = 40;  // - 的索引为40
            }
            else
            {
                // Skip unsupported characters
                continue;
            }
        }

        if (current->children[index] == nullptr)
        {
            return;  // 如果键不存在，则直接返回
        }

        nodeStack.push(current);
        indexStack.push(index);
        current = current->children[index];
    }

    // 删除键标记
    current->isEndOfWord = false;

    // 删除节点（如果该节点没有子节点且不是其他键的前缀）
    if (isNodeDeletable(current))
    {
        while (!nodeStack.empty())
        {
            TrieNode* node = nodeStack.top();
            int index = indexStack.top();

            nodeStack.pop();
            indexStack.pop();

            TrieNode* child = node->children[index];
            node->children[index] = nullptr;
            delete child;
            // 如果节点不可删除，则终止删除过程
            if (!isNodeDeletable(node))
            {
                break;
            }
        }
    }
}

// 判断节点是否可以删除（没有子节点且不是其他键的前缀）
bool TrieTree::isNodeDeletable(TrieNode* node)
{
    for (TrieNode* child : node->children)
    {
        if (child != nullptr)
        {
            return false;  // 存在子节点，不可删除
        }
    }
    return !node->isEndOfWord;  // 不是其他键的前缀，可删除
}

// 迭代释放Trie树节点的内存
void TrieTree::destroyTrie(TrieNode* root)
{
    if (root == nullptr)
    {
        return;
    }

    std::stack<TrieNode*> nodeStack;
    nodeStack.push(root);

    while (!nodeStack.empty())
    {
        TrieNode* current = nodeStack.top();
        nodeStack.pop();

        for (TrieNode* child : current->children)
        {
            if (child != nullptr)
            {
                nodeStack.push(child);
            }
        }

        delete current;
    }
}

// 转换成小写
std::string TrieTree::convertToLowerCase(const std::string& word)
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
