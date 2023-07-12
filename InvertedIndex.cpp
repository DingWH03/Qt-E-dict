#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <QResource>
#include <QString>
#include <QDir>
#include <QCoreApplication>
#include "InvertedIndex.h"
#include "./include/cppjieba/Jieba.hpp"

std::vector<std::string> InvertedIndex::WordSegmentation(std::string sentence)
{
    QString targetDirectory = QCoreApplication::applicationDirPath()+"/Jiebadict";
    // 将相对路径传递给jieba库的构造函数
    cppjieba::Jieba jieba((targetDirectory+"/jieba.dict.utf8").toStdString(), (targetDirectory+"/hmm_model.utf8").toStdString(),
                          (targetDirectory+"/user.dict.utf8").toStdString(), (targetDirectory+"/idf.utf8").toStdString(),
                          (targetDirectory+"/stop_words.utf8").toStdString());

    // Tokenize the sentence
    std::vector<std::string> words;
    jieba.Cut(sentence, words, true);

    return words;
}

// Function to build an inverted index from a file
std::unordered_map<std::string, std::vector<int>> InvertedIndex::buildInvertedIndex(const std::string &filenameafter)
{
    std::unordered_map<std::string, std::vector<int>> invertedIndex;
    std::ifstream inputFile(filenameafter);
    if(!inputFile.is_open())
    {
    }
    std::string line;
    int lineNumber = 1;

    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string word;

        while (iss >> word)
        {
            invertedIndex[word].push_back(lineNumber);
        }

        lineNumber++;
    }

    inputFile.close();
    return invertedIndex;
}

// Function to search for a word in the inverted index
std::vector<int> InvertedIndex::searchInvertedIndex(const std::unordered_map<std::string, std::vector<int>> &invertedIndex, const std::string &word)
{
    std::vector<int> results;

    if (invertedIndex.count(word) > 0)
    {
        results = invertedIndex.at(word);
    }

    return results;
}

std::vector<int> InvertedIndex::FindIntersection(const std::vector<std::vector<int>> &arrays)
{
    if (arrays.empty())
        return {};

    std::unordered_set<int> intersectionSet(arrays[0].begin(), arrays[0].end());

    for (size_t i = 1; i < arrays.size(); ++i)
    {
        std::unordered_set<int> currentSet(arrays[i].begin(), arrays[i].end());
        std::unordered_set<int> intersection;

        for (const auto &num : currentSet)
        {
            if (intersectionSet.count(num) > 0)
                intersection.insert(num);
        }

        intersectionSet = std::move(intersection);
    }

    std::vector<int> intersectionVec(intersectionSet.begin(), intersectionSet.end());
    return intersectionVec;
}

std::vector<std::string> InvertedIndex::CtoE(std::string key)
{

    std::string searchWord = key;
    std::vector<std::string> resultline;

    std::vector<std::string> searchWordsplit = WordSegmentation(searchWord);
    std::vector<std::vector<int>> searchresultsRaw;
    for (const auto &searchWordsingle : searchWordsplit)
    {
        std::vector<int> searchResults = searchInvertedIndex(invertedIndex, searchWordsingle);
        searchresultsRaw.push_back(searchResults);
    }

    std::vector<int> searchResultsCommon = FindIntersection(searchresultsRaw);

    std::ifstream wordFile(filename);
    if (wordFile.is_open())
    {
        std::string line;
        int count=1;

        for (const auto &result : searchResultsCommon)
        {
            // 定位到目标行
            wordFile.seekg(std::ios::beg);
            for (int i = 1; i < result; ++i)
            {
                std::getline(wordFile, line);
            }

            // 读取目标行并添加到结果
            std::getline(wordFile, line);
            resultline.push_back(line);
            count++;
            if (count>=100)
            {
                break;
            }
        }
    }

    wordFile.close();


    return resultline;
}

void InvertedIndex::build()
{
    invertedIndex = buildInvertedIndex(filenameafter);
}

void InvertedIndex::removeUniqueResult(int lineNumber)
{
    if (lineNumber != 0)
    {
        std::ifstream inputFile(filenameafter);
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

            // 将修改后的内容写回文件
            std::ofstream outputFile(filenameafter, std::ofstream::trunc);
            if (outputFile.is_open())
            {
                for (const auto &line : lines)
                {
                    outputFile << line << std::endl;
                }
                outputFile.close();
            }

        }

    }
}

void InvertedIndex::rebuild()
{
    // 清空现有的倒排索引
    invertedIndex.clear();

    // 重新构建倒排索引
    invertedIndex = buildInvertedIndex(filenameafter);
}

void InvertedIndex::insertLine(int lineNumber, std::string newLine) {
    std::ifstream inFile(filenameafter);
    std::vector<std::string> lines;
    std::string line;
    std::vector<std::string> temp = WordSegmentation(newLine);
    newLine="";
    for(const auto& line : temp)
    {
        newLine+=line;
        newLine+=" ";
    }

    // 读取文件内容
    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    // 在指定行之前插入新行
    if (lineNumber > 0 && lineNumber <= lines.size() + 1) {
        lines.insert(lines.begin() + lineNumber - 1, newLine);
    } else {
        return;
    }

    // 将修改后的内容写回文件
    std::ofstream outFile(filenameafter);
    for (const auto& l : lines) {
        outFile << l << std::endl;
    }
    outFile.close();
}

