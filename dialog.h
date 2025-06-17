#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    int get_result(); //获得设置的圆盘数量

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
