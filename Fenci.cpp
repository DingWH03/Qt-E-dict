#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <QFile>
#include <QString>
#include <QResource>
#include <QCoreApplication>
#include <QDir>
#include "./include/cppjieba/Jieba.hpp"

extern QString filename;
extern std::string outputfile;

struct WordEntry {
    std::string word;
    std::string translation;
};

void WordSegmentation(std::vector<WordEntry>& dict)
{
    std::string inputfile = filename.toStdString();
    std::ifstream file(inputfile);
    if (!file.is_open()) {
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word, translation, other;
        std::getline(iss, word, ',');
        std::getline(iss, other, ',');
        std::getline(iss, other, ',');
        std::getline(iss, translation, ',');
        dict.push_back({ word, translation});
        iss.clear();
    }
    file.close();

    Q_INIT_RESOURCE(resources);
    QResource::registerResource(":/resources.qrc");

    QString targetDirectory = QCoreApplication::applicationDirPath()+"/Jiebadict";
    // 将相对路径传递给jieba库的构造函数
    cppjieba::Jieba jieba((targetDirectory+"/jieba.dict.utf8").toStdString(), (targetDirectory+"/hmm_model.utf8").toStdString(),
                          (targetDirectory+"/user.dict.utf8").toStdString(), (targetDirectory+"/idf.utf8").toStdString(),
                          (targetDirectory+"/stop_words.utf8").toStdString());
    std::string sentence;
    // 遍历句子数组，逐个进行分词
    for (WordEntry& wordentry : dict) {
        // 分词结果的字符串向量
        std::vector<std::string> words;
        sentence=wordentry.translation;
        // 使用 Jieba 分词器进行分词
        jieba.Cut(sentence, words, true);
        // 将分词结果重新拼接为句子
        sentence = "";
        for (const std::string& word : words) {
            sentence += word + " ";
        }
        if(sentence=="")
        {
            sentence="Blank ";
        }
        // 去除末尾的空格
        sentence.pop_back();
        wordentry.translation=sentence;
    }

    std::ofstream output(outputfile);
    if (!output.is_open()) {
    }

    for (const auto& entry : dict) {
        output << entry.word << ',' << entry.translation << '\n';
    }

    output.close();
}


