# Qt-E-dict
# 这是一个数据结构课程设计，采用了Qt编写图形界面，引用了Jieba库和libmon库
（一）需求和规格说明
问题描述：这是一款中英文字典查询软件。软件打开一个词库，查询输入的内容，可实现根据输入的英文单词查找他的几种中文释义，也可通过搜索中文查找与之相关的英文单词。软件还需要拥有修改词库和添加、删除单词的功能。

编程任务：
    (1) 建立单词的数据结构，以便于单词大规模存储在文件中，并可以方便地对单词库进行增加、删除与查找。
    (2) 从文件中读取单词，以单词的内容与行号建立B+树索引便于精确查找单词，确定行号。
    (3) 通过检索到的行号读取指定行内容，解析出单词的音标、中文释义等相关信息。
    (4) 建立单词前缀树（TrieTree）以便于输入单词时进行前缀模糊匹配。
    (5) 对单词的中文释义进行分词并建立倒排索引，从而实现根据单词的部分中文释义匹配出相关单词。
    (6) 编写图形界面方便查询、修改与删除相关操作时的交互。

注：为了确保查询效率，采用了三种数据结构实现三种功能。实验词库词汇量为77w。
（二）设计
1．设计思想
本题主要考察单词数据结构的建立与查找算法的选择，相较于电子词典通常使用的哈希表，我使用了B+树查找结构，B+树的优点在于查找速度快，特别适用于数据量及其大的情况，能够保证良好的查找性能且较好的空间开销，我发现已有的电子词典app都可以根据输入的内容弹出下拉栏进行前缀匹配，而B+树不太适合前缀匹配和模糊搜索，故选择前缀匹配树，前缀匹配树是一种典型的空间换时间的数据结构，也能保证在数据量较大时有较高的查找性能。然而这两种数据结构均无法实现中文检索，并且考虑到我的词库中单词的中文释义都存储在一起，难以进行分开，且大多使用句子进行解释，无法进行完整匹配，于是采用Jieba中文分词工具处理后进行倒排索引进行中文匹配。程序在第一次运行时会检查词库文件同级目录下有无分词后的数据文件，若无数据文件将会先对词库进行排序与分词处理，然后生成数据文件。最后建立三种数据结构，存储在内存中。词库的删除操作我使用B+树得到单词所在行，然后通过文件操作删除该行，并从三种数据结构中移除删除的内容，单词的修改操作为了避免出现数据问题，我选择将原单词移除后重新添加，这样操作的优点在于可以修改单词的一切内容包括单词本身的拼写，缺点是需要重新建立数据结构效率较低，添加单词我采用按顺序添加到文件中，并且重建BPlus树和倒排索引，无需重建单词前缀树。因此该程序更适用于查询单词，不太适合创建及修改词库，有待继续完善。程序使用了常用的Qt库创建图形界面，界面开发难度较低且适用于多平台开发，效率相对较高，使用图形界面有利于程序的交互。

3. 核心算法
该程序的主要算法是如何实现精确查询、前缀匹配与中文查询等操作。
    1. 精确查询：BplusTree
(1)建立B+树：
B+树是一种平衡树，每个节点最多有M个子节点（M称为B+树的阶数），且除了根节点外，其他节点的关键字数目要在[M/2]到M之间。
B+树的叶子节点是按照关键字的顺序进行排序的，非叶子节点是指向子节点的指针和关键字的组合。
B+树的建立过程从空树开始，逐渐插入关键字和值，并进行必要的调整，使得树保持平衡状态。
当插入一个新的关键字时，首先找到应该插入的叶子节点，如果该节点已满（即关键字数目达到M），则进行节点分裂操作，将中间关键字上移，并分成两个节点。
分裂操作会导致上层非叶子节点的关键字数量增加，如果上层节点也满了，则递归地进行节点分裂操作，直到根节点。
B+树的建立过程中，还需要处理节点的合并操作，即当删除一个关键字后，如果节点的关键字数目小于[M/2]，则可以从相邻的节点中借关键字，或者进行节点合并。
void insert(const std::string &key, int value)函数用于插入节点，在插入过程中检查结点数量，由void splitChild(BPlusTreeNode *parent, int index);
void insertNonFull(BPlusTreeNode *node, const std::string &key, int value);两个函数负责判断并插入与完成分割节点操作。
（2）查找操作：
B+树的查找操作从根节点开始，根据关键字的大小，选择合适的子节点进行查找。
如果当前节点是非叶子节点，则根据关键字的大小找到合适的子节点，并继续在子节点中进行查找。
如果当前节点是叶子节点，则在当前节点的关键字列表中进行顺序查找，找到匹配的关键字后返回相应的值。
如果找不到匹配的关键字，则表示该关键字不存在于B+树中。
std::string *getsearch(const std::string &key);用于获取int searchNode(const BPlusTreeNode *node, const std::string &key);返回的行号中指定在文件中的内容。
    (3) 排序操作：BPlusTree建立时要求词库按照字典顺序排列，我采用将所有内容读取至vector数组中用algorithm进行排序，或使用ascii码进行比较，排序后写入文件。
    2. 前缀匹配：TrieTree
    (1) 建立：
Trietree把每个单词分解，每个字母作为一个结点，树的每一条从根节点走向叶节点都是一个单词，建立主要使用了递归算法。
1.Trie树的根节点为空，没有关联任何字符。
2.插入一个字符串时，从根节点开始，逐个字符进行处理。
3,。对于每个字符，检查当前节点的子节点中是否存在该字符，如果存在则继续向下处理，否则创建一个新的子节点。
4.在处理完所有字符后，将最后一个节点标记为结束节点，表示该节点对应的字符串在Trie树中存在。
5.重复以上过程，插入所有需要存储的字符串。
(2)查找：
1.从根节点开始，逐个字符进行处理。
2.对于每个字符，检查当前节点的子节点中是否存在该字符，如果存在则继续向下处理，否则表示要查找的字符串不存在。
3.如果处理完所有字符后，最后一个节点被标记为结束节点，则表示要查找的字符串存在。

    3. 中文查询：中文分词+倒排索引
    (1) 对词库中的中文翻译使用结巴库进行分词操作，写入另一文件。
    (2) 遍历该文件，提取出文档中每一行的关键词。
    (3) 对于每个关键词，将其作为索引的关键字，在倒排索引中查找该关键词是否已存在。
    (4) 如果关键词不存在于倒排索引中，创建一个新的索引项，关键词作为索引的键，记录该关键词出现的位置信息（行号）作为索引的值。
    (5) 如果关键词已存在于倒排索引中，将当前文档的位置信息加到已存在的索引项中。
    (6) 重复以上过程，处理所有文档或记录。
    (7) 查找时用关键词进行对应，获得行号
注：为了提高查找的精度，我对用户提交的搜索关键词也进行分词，每个关键词分别进行查找，找出分别含有着写关键词的集合并取交集（算法略）

    4. 内存优化与存储
词库中的单词包含许多项内容，但仅将单词与对应的行号与加载进入内存，而倒排索引由于重复的关键词较多，实际内存占用尚可。

