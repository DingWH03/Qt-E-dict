#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include "Fenci.cpp"
#include "InvertedIndex.h"
#include <thread>
#include <QMessageBox>
#include "form0.h"
#include <QProgressDialog>
#include "BPlusTree.h"
#include <QResource>
#include <QDir>
#include <QFile>

QString filename;
std::string outputfile;

void CopyResourcesToDirectory(const QString& targetDirectory);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    filename="";
    CopyResourcesToDirectory(QCoreApplication::applicationDirPath()+"/Jiebadict");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(nullptr, "选择文件", "", "CSV文件(*.csv)");
    ui->lineEdit->setText(filename);
    ui->lineEdit->setReadOnly(true);
    outputfile = filename.toStdString().erase(filename.toStdString().length() - 4) + ".dat";
}


void MainWindow::on_pushButton_2_clicked()
{
    if(filename.isEmpty())
    {
        QMessageBox::information(nullptr, "错误", "打开文件为空");
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(nullptr, "警告", "确定要打开吗，这可能需要一点时间",
                                 QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QFile file(QString::fromStdString(outputfile));
            if (file.exists())
            {
                LoadafterSortedandFenci();
            }
            else
            {
                Load();
            }

            close();
        }
        else
        {
            filename  = "";
            ui->lineEdit->setText(filename);
            ui->lineEdit->setReadOnly(false);
        }

    }
}

void sortWordsInFile(const std::string &Filename)
{
    std::ifstream inputFile(Filename);

    if (inputFile.is_open())
    {
        std::string word;
        std::vector<std::string> words;

        while (std::getline(inputFile, word))
        {
            words.push_back(word);
        }
        inputFile.close();
        std::sort(words.begin(), words.end());
        std::ofstream outputFile(Filename);
        for (const std::string &w : words)
        {
            outputFile << w << "\n";
        }

        outputFile.close();
    }
}

void CopyResourcesToDirectory(const QString& targetDirectory)
{
    // 加载 Qt 资源
    QResource::registerResource(":/resources.qrc");

    // 创建目标文件夹（如果不存在）
    QDir dir(targetDirectory);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QStringList resourceFiles = {":/dict/hmm_model.utf8", ":/dict/idf.utf8", ":/dict/jieba.dict.utf8", ":/dict/stop_words.utf8", ":/dict/user.dict.utf8"};
    QStringList target = {targetDirectory+"/hmm_model.utf8", targetDirectory+"/idf.utf8", targetDirectory+"/jieba.dict.utf8", targetDirectory+"/stop_words.utf8", targetDirectory+"/user.dict.utf8"};
    int i = 0;
    for (const QString& resourceFile : resourceFiles) {

        QFile sourceFile(resourceFile);
        QFile destinationFile(target[i++]);

        if (sourceFile.exists() && sourceFile.open(QIODevice::ReadOnly)) {
            if (destinationFile.open(QIODevice::WriteOnly)) {
                destinationFile.write(sourceFile.readAll());
                destinationFile.close();

            }
            sourceFile.close();
        }
    }

    QResource::unregisterResource("resources.qrc");
}

void fenci()
{
    std::vector<WordEntry> dict;
    WordSegmentation(dict);
}

void BPlustree(Form0* form0)
{
    BPlusTree* dictionary=new BPlusTree(filename.toStdString());
    dictionary->Build();
    form0->getBPlusTree(dictionary);
}

void Trietree(Form0* form0)
{
    TrieTree* trietree=new TrieTree(filename.toStdString());
    trietree->build();
    form0->getTrieTree(trietree);
}

void sort()
{
    sortWordsInFile(filename.toStdString());
}

void invertedindex(Form0* form0){
    InvertedIndex* invertedindex=new InvertedIndex(filename.toStdString());
    invertedindex->build();
    form0->getInvertedIndex(invertedindex);
}

void MainWindow::LoadafterSortedandFenci(){
    Form0* form0 = new Form0();
    QProgressDialog progressDialog("正在加载...",nullptr,0,100);
    progressDialog.setWindowTitle("Loading");
    progressDialog.setLabelText("请耐心等待");
    progressDialog.setRange(0, 5); // 设置进度条
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setModal(true); // 将加载界面设置为模态对话框，阻止用户操作其他窗口
    progressDialog.show();
    std::thread thread1(sort);
    QCoreApplication::processEvents();
    thread1.join();
    progressDialog.setValue(1);
    std::thread thread3(fenci);
    QCoreApplication::processEvents();
    std::thread thread4(std::bind(BPlustree,form0));
    progressDialog.setValue(2);
    QCoreApplication::processEvents();
    std::thread thread5(std::bind(Trietree,form0));
    QCoreApplication::processEvents();
    thread3.join();
    progressDialog.setValue(3);
    std::thread thread2(std::bind(invertedindex,form0));
    QCoreApplication::processEvents();
    thread4.join();
    progressDialog.setValue(4);
    thread5.join();
    thread2.join();
    progressDialog.setValue(5);
    progressDialog.close();
    form0->show();
}

void MainWindow::Load(){
    Form0* form0 = new Form0();
    QProgressDialog progressDialog("正在加载...",nullptr,0,100);
progressDialog.setWindowTitle("Loading");
    progressDialog.setLabelText("请耐心等待");
    progressDialog.setRange(0, 5); // 设置进度条
    progressDialog.setWindowModality(Qt::WindowModal);
    QCoreApplication::processEvents();
    progressDialog.setModal(true); // 将加载界面设置为模态对话框，阻止用户操作其他窗口
    progressDialog.show();
    progressDialog.setValue(1);
    QCoreApplication::processEvents();
    std::thread thread4(std::bind(BPlustree,form0));
    QCoreApplication::processEvents();
    progressDialog.setValue(2);
    std::thread thread5(std::bind(Trietree,form0));
    QCoreApplication::processEvents();
    progressDialog.setValue(3);
    std::thread thread2(std::bind(invertedindex,form0));
    QCoreApplication::processEvents();
    thread4.join();
    progressDialog.setValue(4);
    thread5.join();
    thread2.join();
    progressDialog.setValue(5);
    progressDialog.close();
    form0->show();
}




