#include "pseudo.h"
#include "ui_pseudo.h"
#include <QTextCursor>
#include <QTextCharFormat>
#include "dialog.h"
#include <QDebug>
#include "sta.h"

pseudo::pseudo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pseudo)
{
    ui->setupUi(this);

    QString str = "HANOI(n,X,Y,Z)\n  if n=1\n    then MOVE(X,1,Z)\n    else HANOI(n-1,X,Z,Y)\n      MOVE(X,n,Z)\n      HANOI(n-1,Y,X,Z)";
    ui->textBrowser_2->setPlainText(str);
    //highlightLine(3); // 高亮显示第3行
}

pseudo::~pseudo()
{
    delete ui;
}

void pseudo::clearHighlight()
{
    // 获取文本浏览器的文档
    QTextDocument *doc = ui->textBrowser_2->document();

    // 清除所有行的高亮
    QTextCursor cursor(doc);
    cursor.select(QTextCursor::Document);
    cursor.setCharFormat(QTextCharFormat()); // 清除格式
}

void pseudo::highlightLine(int lineNumber)
{
    // 获取文本浏览器的文档
    QTextDocument *doc = ui->textBrowser_2->document();

    //取消
    clearHighlight();

    // 设置高亮格式
    QTextCharFormat format;
    format.setBackground(QColor(255, 255, 0)); // 设置背景颜色为黄色，你可以根据需要修改颜色

    // 遍历文档找到要高亮的行
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    int currentLineNumber = 1;

    while (!cursor.atEnd())
    {
        if (currentLineNumber == lineNumber)
        {
            cursor.select(QTextCursor::LineUnderCursor);
            cursor.setCharFormat(format);
            cursor.clearSelection();
            break;
        }
        cursor.movePosition(QTextCursor::NextBlock);
        currentLineNumber++;
    }
}


//void pseudo::updateTextBrowser(QString text)
//{
//    //Dialog *dialog = new Dialog(); // 你可能需要在Dialog类中定义get_result方法来返回结果
//    //int var = mainWindowPtr->result();
//    int i=0;
//    int var = sta().count;

//    if (var == 1)
//        highlightLine(3); // 当var等于1时，第三行高亮
//    else
//    {
//        highlightLine(4);
//        if(i==1)
//            highlightLine(6);

//        qDebug()<<var;
//        QString string1 = "HANOI(" + QString::number(var - 1) + ",Y,X,Z)";
//        QString string2 = "HANOI(" + QString::number(var - 2) + ",Y,Z,X)";
//        if (string1 == text)
//        {
//            highlightLine(5); // 在string1字符串与text字符串相等时，高亮第5行
//        }
//        if (string2 == text)
//        {
//            highlightLine(6); // 在string2和text相等时，高亮第6行
//            i=1;
//        }
//    }
//    ui->textBrowser->append(text);
//}


//void pseudo::updateTextBrowser(QString text)
//{
//    static int highlightLineNum = 4; // 初始时高亮第4行
//    int var = sta().count;
//    qDebug()<<sta().count;

//    if (var == 1)
//    {
//        highlightLineNum = 3; // 当var等于1时，高亮第3行
//    }
//    else
//    {
//        QString string1 = "HANOI(" + QString::number(var - 1) + ",Y,X,Z)";
//        QString string2 = "HANOI(" + QString::number(var - 2) + ",Y,Z,X)";
//        if (string1 == text)
//        {
//            highlightLineNum = 5; // 当string1字符串与text字符串相等时，高亮第5行
//        }
//        if (string2 == text)
//        {
//            highlightLineNum = 6; // 当string2和text相等时，高亮第6行
//        }
//    }
//    // 高亮指定行
//    QString string3 = "Exit";
//    if(string3==text)
//    {
//        highlightLineNum=4;
//        clearHighlight();
//    }
//    else
//        ui->textBrowser->append(text);

//    highlightLine(highlightLineNum);
//}

void pseudo::updateTextBrowser(QString text)
{
    static int highlightLineNum = 4; // 初始时高亮第4行
    int var = sta().count;

    if (var == 1)
    {
        highlightLineNum = 3; // 当var等于1时，高亮第3行
    }
    else
    {
        QString string1 = "HANOI(" + QString::number(var - 1) + ",Y,X,Z)";
        QString string2 = "HANOI(" + QString::number(var - 2) + ",Y,Z,X)";
        if (string1 == text)
        {
            highlightLineNum = 5; // 当string1字符串与text字符串相等时，高亮第5行
        }
        if (string2 == text)
        {
            highlightLineNum = 6; // 当string2和text相等时，高亮第6行
        }
    }

     highlightLine(highlightLineNum);
     // 高亮指定行
     QString string3 = "Exit";
     if (string3 == text)
     {
         highlightLineNum = 4;
         clearHighlight();
     }
     else
     {
         ui->textBrowser->append(text);
     }
}


