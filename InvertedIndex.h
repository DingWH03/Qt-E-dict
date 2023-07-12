#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H
#include <vector>
#include <unordered_map>
#include <string>
class InvertedIndex{
private:
    std::string filename;
    std::string filenameafter;
    std::unordered_map<std::string, std::vector<int>> invertedIndex;
public:
    InvertedIndex(std::string Filename){
        filename = Filename;
        if(filename.length()>4)
        {filenameafter = Filename.erase(Filename.length() - 4) + ".dat";}
    }
    ~InvertedIndex()=default;
    void insertLine(int lineNumber, std::string newLine);
    void rebuild();
    void removeUniqueResult(int lineNumber);
    std::vector<std::string> WordSegmentation(std::string sentence);
    std::unordered_map<std::string, std::vector<int>> buildInvertedIndex(const std::string& filename);
    std::vector<int> searchInvertedIndex(const std::unordered_map<std::string, std::vector<int>>& invertedIndex, const std::string& word);
    std::vector<int> FindIntersection(const std::vector<std::vector<int>>& arrays);
    std::vector<std::string> CtoE(std::string key);
    void build();
};
#endif
