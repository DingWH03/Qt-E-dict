#include "form0.h"
#include "ui_form0.h"
#include "BPlusTree.h"
#include <vector>
#include <QMessageBox>

#include <string>
extern QString filename;

bool containsChinese(const QString& text)
{
    for (const QChar& ch : text)
    {
        ushort unicode = ch.unicode();
        if (unicode >= 0x4E00 && unicode <= 0x9FA5)
        {
            return true;
        }
    }

    return false;
}

Form0::Form0(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form0)
{
    ui->setupUi(this);
    setupCompleter();

}

QStringList vectorToQStringList(const std::vector<std::string>& strings, int countall);

Form0::~Form0()
{
    delete ui;
    delete bplustree;
    delete trietree;
    delete invertedindex;
    delete timer;
}

void Form0::setupCompleter() {
    lineEdit = ui->lineEdit;  // 根据对象名称获取LineEdit对象
    model = new QStringListModel();
    completer = new QCompleter(model, lineEdit);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    lineEdit->setCompleter(completer);
}



void Form0::getBPlusTree(BPlusTree* bplustree){
    this->bplustree = bplustree;
}
void Form0::getTrieTree(TrieTree* trietree){
    this->trietree = trietree;
}
void Form0::getInvertedIndex(InvertedIndex* invertedindex){
    this->invertedindex = invertedindex;
}

void Form0::setReadOnly()
{
    ui->definitionplainTextEdit->setReadOnly(true);
    ui->translationplainTextEdit->setReadOnly(true);
    ui->posplainTextEdit->setReadOnly(true);
    ui->collinsplainTextEdit->setReadOnly(true);
    ui->oxfordplainTextEdit->setReadOnly(true);
    ui->tagplainTextEdit->setReadOnly(true);
    ui->bncplainTextEdit->setReadOnly(true);
    ui->frqplainTextEdit->setReadOnly(true);
    ui->exchangeplainTextEdit->setReadOnly(true);
    ui->detailplainTextEdit->setReadOnly(true);
    ui->audioplainTextEdit->setReadOnly(true);
}

void Form0::on_pushButton_clicked()
{
    QString searchword;
        searchword = ui->lineEdit->text();
    if (searchword=="")
        {
        QMessageBox::information(nullptr, "错误", "输入内容为空");
        return;
        }
    if(!containsChinese(searchword))
        {
        std::string* searchresults=bplustree->getsearch(searchword.toStdString());
        if(searchresults[0]!="")
        {
            ui->wordlabel->setText(QString::fromStdString(searchresults[0]));
            ui->phoneticlabel->setText(QString::fromStdString(searchresults[1]));
            ui->label_12->setWordWrap(true);
            ui->label_10->setWordWrap(true);
            ui->definitionplainTextEdit->setPlainText(QString::fromStdString(searchresults[2]));
            ui->translationplainTextEdit->setPlainText(QString::fromStdString(searchresults[3]));
            ui->posplainTextEdit->setPlainText(QString::fromStdString(searchresults[4]));
            ui->collinsplainTextEdit->setPlainText(QString::fromStdString(searchresults[5]));
            ui->oxfordplainTextEdit->setPlainText(QString::fromStdString(searchresults[6]));
            ui->tagplainTextEdit->setPlainText(QString::fromStdString(searchresults[7]));
            ui->bncplainTextEdit->setPlainText(QString::fromStdString(searchresults[8]));
            ui->frqplainTextEdit->setPlainText(QString::fromStdString(searchresults[9]));
            ui->exchangeplainTextEdit->setPlainText(QString::fromStdString(searchresults[10]));
            ui->detailplainTextEdit->setPlainText(QString::fromStdString(searchresults[11]));
            ui->audioplainTextEdit->setPlainText(QString::fromStdString(searchresults[12]));
            QCompleter* dcompleter = completer;
            delete dcompleter;
            completer = new QCompleter(model, lineEdit);
            ui->stackedWidget->setCurrentWidget(ui->page_2);
            setReadOnly();
        }
        else{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(nullptr, "警告", "词库中不存在该单词，是否需要添加？",
                                         QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                ui->wordtextEdit_2->setText(lineEdit->text());
                ui->stackedWidget->setCurrentWidget(ui->page_3);
                flagnew = 1;
            }
        }
        }
    else{
        std::vector<std::string> omatchingwords = invertedindex->CtoE(((ui->lineEdit->text()).toStdString()));
        if(omatchingwords.empty())
        {
            QMessageBox::information(nullptr, "警告", "找不到匹配项");
        }
        else
        {
            for(auto& line:omatchingwords) {
                std::istringstream iss(line);
                std::string word, translation, other;
                std::getline(iss, word, ',');
                std::getline(iss, other, ',');
                std::getline(iss, other, ',');
                std::getline(iss, translation, ',');
                line = "";
                line = word+","+translation;
                std::replace(line.begin(), line.end(), '{', ',');
                std::replace(line.begin(), line.end(), '\\', '#');
                iss.clear();
            }

            matchingWords={};
            matchingWords = vectorToQStringList(omatchingwords,100);
            ui->listWidget->clear();
            for (const QString& word : matchingWords) {
                QListWidgetItem* item = new QListWidgetItem(word);
                ui->listWidget->addItem(item);
            }
            ui->stackedWidget->setCurrentWidget(ui->page_9);
        }

    }
}

QStringList vectorToQStringList(const std::vector<std::string>& strings, int countall) {
        QStringList qStringList;
        int count = 0;
        for (const auto& str : strings) {
            QString qstr = QString::fromStdString(str);
            qStringList.append(qstr);
            count++;
            if (count == countall) {
                break;
            }
        }
        return qStringList;
}

// 返回按钮，选择返回到单词翻译位置或者linedit位置
void Form0::on_pushButton_2_clicked()
{
        if (flag == 1)
        {
            ui->stackedWidget->setCurrentWidget(ui->page);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            lineEdit->setCompleter(completer);
        }
        else{
            flag =1;
            ui->stackedWidget->setCurrentWidget(ui->page_9);
        }

}

void Form0::on_lineEdit_textChanged(const QString &arg1)
{
        if (arg1.length()>1)
        {
            if(!containsChinese(arg1))
            {
                matchingWords={};
                matchingWords = vectorToQStringList(trietree->search(((ui->lineEdit->text()).toStdString())),30);
                model->setStringList(matchingWords);
            }
        }
}


void Form0::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);

}


void Form0::on_listWidget_itemClicked(QListWidgetItem *item)
{
    std::string str = (item->text()).toStdString();
    std::istringstream iss(str);
    std::getline(iss, str, ',');
    std::string* searchresults=bplustree->getsearch(str);
            ui->wordlabel->setText(QString::fromStdString(searchresults[0]));
            ui->phoneticlabel->setText(QString::fromStdString(searchresults[1]));
            ui->label_12->setWordWrap(true);
            ui->label_10->setWordWrap(true);
            ui->definitionplainTextEdit->setPlainText(QString::fromStdString(searchresults[2]));
            ui->translationplainTextEdit->setPlainText(QString::fromStdString(searchresults[3]));
            ui->posplainTextEdit->setPlainText(QString::fromStdString(searchresults[4]));
            ui->collinsplainTextEdit->setPlainText(QString::fromStdString(searchresults[5]));
            ui->oxfordplainTextEdit->setPlainText(QString::fromStdString(searchresults[6]));
            ui->tagplainTextEdit->setPlainText(QString::fromStdString(searchresults[7]));
            ui->bncplainTextEdit->setPlainText(QString::fromStdString(searchresults[8]));
            ui->frqplainTextEdit->setPlainText(QString::fromStdString(searchresults[9]));
            ui->exchangeplainTextEdit->setPlainText(QString::fromStdString(searchresults[10]));
            ui->detailplainTextEdit->setPlainText(QString::fromStdString(searchresults[11]));
            ui->audioplainTextEdit->setPlainText(QString::fromStdString(searchresults[12]));
            ui->stackedWidget->setCurrentWidget(ui->page_2);
            flag = 0;
}


void Form0::on_pushButton_6_clicked()
{
            if (flagmodify||flagnew)
            {
            QMessageBox::StandardButton reply3;
            reply3 = QMessageBox::warning(nullptr, "警告", "确定要返回吗，您的修改尚未保存",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply3 == QMessageBox::Yes)
            {
                if(flagmodify)
                {ui->stackedWidget->setCurrentWidget(ui->page_2);flagmodify=0;}
                else{ui->stackedWidget->setCurrentWidget(ui->page);flagnew=0;}
            }
            }
            else{
                if(flagmodify)
                {ui->stackedWidget->setCurrentWidget(ui->page_2);flagmodify=0;}
                else{ui->stackedWidget->setCurrentWidget(ui->page);flagnew=0;}
            }
}

// 保存按钮
void Form0::on_pushButton_7_clicked()
{
            QString word,phonetic,definition,translation,pos,collins,oxford,tag,bnc,frq,exchange,detail,audio;
            word = ui->wordtextEdit_2->toPlainText();
            phonetic = ui->phonetictextEdit_2->toPlainText();
            definition = ui->definitiontextEdit_2->toPlainText();
            translation = ui->translationtextEdit_2->toPlainText();
            pos = ui ->postextEdit_2->toPlainText();
            collins = ui->collinstextEdit_2->toPlainText();
            oxford = ui->oxfordtextEdit_2->toPlainText();
            tag = ui->tagtextEdit_2->toPlainText();
            bnc = ui->bnctextEdit_2->toPlainText();
            frq =ui->frqtextEdit_2->toPlainText();
            exchange = ui->exchangetextEdit_2->toPlainText();
            detail = ui->detailtextEdit_2->toPlainText();
            audio = ui->audiotextEdit_2->toPlainText();
            wordEntry newword = {
                word.toStdString(),phonetic.toStdString(),definition.toStdString(),translation.toStdString(),pos.toStdString(),collins.toStdString(),
                oxford.toStdString(),tag.toStdString(),bnc.toStdString(),frq.toStdString(),exchange.toStdString(),detail.toStdString(),audio.toStdString()
            };

            if (flagnew)
            {

                bplustree->InsertToFile(newword);
                trietree->insert(word.toStdString());
                invertedindex->insertLine(bplustree->search(word.toStdString()),(word+","+translation).toStdString());
                invertedindex->rebuild();
                flagnew=0;
                QMessageBox::information(nullptr, "注意", "添加成功！");
                ui->stackedWidget->setCurrentWidget(ui->page);
                completer->setCaseSensitivity(Qt::CaseInsensitive);
                lineEdit->setCompleter(completer);
            }
            else if(flagmodify)
            {
                QString wordold = ui->wordlabel->text();
                invertedindex->removeUniqueResult(bplustree->search(wordold.toStdString()));
                bplustree->remove(wordold.toStdString());
                trietree->remove(wordold.toStdString());
                bplustree->InsertToFile(newword);
                trietree->insert(word.toStdString());
                invertedindex->insertLine(bplustree->search(word.toStdString()),(word+","+translation).toStdString());
                invertedindex->rebuild();
                flagmodify=0;
                QMessageBox::information(nullptr, "注意", "修改成功！");
                completer->setCaseSensitivity(Qt::CaseInsensitive);
                lineEdit->setCompleter(completer);
            }
}

// 修改按钮
void Form0::on_pushButton_3_clicked()
{
            ui->wordtextEdit_2->setText(ui->wordlabel->text());
            ui->phonetictextEdit_2->setText(ui->phoneticlabel->text());
            ui->definitiontextEdit_2->setText(ui->definitionplainTextEdit->toPlainText());
            ui->translationtextEdit_2->setText(ui->translationplainTextEdit->toPlainText());
            ui ->postextEdit_2->setText(ui->posplainTextEdit->toPlainText());
            ui->collinstextEdit_2->setText(ui->collinsplainTextEdit->toPlainText());
            ui->oxfordtextEdit_2->setText(ui->oxfordplainTextEdit->toPlainText());
            ui->tagtextEdit_2->setText(ui->tagplainTextEdit->toPlainText());
            ui->bnctextEdit_2->setText(ui->bncplainTextEdit->toPlainText());
            ui->frqtextEdit_2->setText(ui->frqplainTextEdit->toPlainText());
            ui->exchangetextEdit_2->setText(ui->exchangeplainTextEdit->toPlainText());
            ui->detailtextEdit_2->setText(ui->detailplainTextEdit->toPlainText());
            ui->audiotextEdit_2->setText(ui->audioplainTextEdit->toPlainText());
            ui->stackedWidget->setCurrentWidget(ui->page_3);
            flagmodify = 1;
}

// 删除按钮
void Form0::on_pushButton_4_clicked()
{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(nullptr, "警告", "确定要删除吗，该选择无法撤销",
                                         QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                QString wordold = ui->wordlabel->text();
                invertedindex->removeUniqueResult(bplustree->search(wordold.toStdString()));
                invertedindex->rebuild();
                bplustree->remove(wordold.toStdString());
                trietree->remove(wordold.toStdString());
                QMessageBox::information(nullptr, "注意", "删除成功！");
                ui->stackedWidget->setCurrentWidget(ui->page);
                completer->setCaseSensitivity(Qt::CaseInsensitive);
                lineEdit->setCompleter(completer);
            }
}

