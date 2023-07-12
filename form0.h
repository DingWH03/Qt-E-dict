#ifndef FORM0_H
#define FORM0_H
#include "BPlusTree.h"
#include "TrieTree.h"
#include "InvertedIndex.h"
#include <QWidget>
#include <QCompleter>
#include "ui_form0.h"
#include <QTimer>
#include <QStringListModel>

namespace Ui {
class Form0;
}

class Form0 : public QWidget
{
    Q_OBJECT

public:
    explicit Form0(QWidget *parent = nullptr);
    ~Form0();
    void getBPlusTree(BPlusTree* bplustree);
    void getTrieTree(TrieTree* trietree);
    void setupCompleter();
    void getInvertedIndex(InvertedIndex* invertedindex);



private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Form0 *ui;
    BPlusTree* bplustree;
    TrieTree* trietree;
    InvertedIndex* invertedindex;
    bool flag = 1;
    bool flagmodify = 0;
    bool flagnew = 0;
    QLineEdit *lineEdit ;

    QTimer *timer = new QTimer(this);

    QCompleter *completer;
    QStringListModel *model ;
    QStringList matchingWords={};
    void setReadOnly();
};

#endif // FORM0_H
