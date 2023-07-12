QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BPlusTree.cpp \
    Fenci.cpp \
    InvertedIndex.cpp \
    TrieTree.cpp \
    form0.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BPlusTree.h \
    InvertedIndex.h \
    TrieTree.h \
    form0.h \
    include/cppjieba/DictTrie.hpp \
    include/cppjieba/FullSegment.hpp \
    include/cppjieba/HMMModel.hpp \
    include/cppjieba/HMMSegment.hpp \
    include/cppjieba/Jieba.hpp \
    include/cppjieba/KeywordExtractor.hpp \
    include/cppjieba/MPSegment.hpp \
    include/cppjieba/MixSegment.hpp \
    include/cppjieba/PosTagger.hpp \
    include/cppjieba/PreFilter.hpp \
    include/cppjieba/QuerySegment.hpp \
    include/cppjieba/SegmentBase.hpp \
    include/cppjieba/SegmentTagged.hpp \
    include/cppjieba/TextRankExtractor.hpp \
    include/cppjieba/Trie.hpp \
    include/cppjieba/Unicode.hpp \
    include/cppjieba/limonp/ArgvContext.hpp \
    include/cppjieba/limonp/BlockingQueue.hpp \
    include/cppjieba/limonp/BoundedBlockingQueue.hpp \
    include/cppjieba/limonp/BoundedQueue.hpp \
    include/cppjieba/limonp/Closure.hpp \
    include/cppjieba/limonp/Colors.hpp \
    include/cppjieba/limonp/Condition.hpp \
    include/cppjieba/limonp/Config.hpp \
    include/cppjieba/limonp/FileLock.hpp \
    include/cppjieba/limonp/ForcePublic.hpp \
    include/cppjieba/limonp/LocalVector.hpp \
    include/cppjieba/limonp/Logging.hpp \
    include/cppjieba/limonp/Md5.hpp \
    include/cppjieba/limonp/MutexLock.hpp \
    include/cppjieba/limonp/NonCopyable.hpp \
    include/cppjieba/limonp/StdExtension.hpp \
    include/cppjieba/limonp/StringUtil.hpp \
    include/cppjieba/limonp/Thread.hpp \
    include/cppjieba/limonp/ThreadPool.hpp \
    mainwindow.h

FORMS += \
    form0.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources.qrc
