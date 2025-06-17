#ifndef PSEUDO_H
#define PSEUDO_H

#include <QMainWindow>

namespace Ui {
class pseudo;
}

class pseudo : public QMainWindow
{
    Q_OBJECT

public:
    explicit pseudo(QWidget *parent = 0);
    ~pseudo();

private:
    Ui::pseudo *ui;


public:
    void updateTextBrowser(QString text);

    void highlightLine(int lineNumber);

    void clearHighlight();

    //MainWindow* myMain;
    //int myFunction();
};

#endif // PSEUDO_H
