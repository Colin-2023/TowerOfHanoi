#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    presenter_ = new tower_presenter(ui_->stateLabel);

    //设置相应的控件
    presenter_->set_back_step_button(ui_->backStepButton);
    presenter_->set_next_step_button(ui_->nextStepButton);
    presenter_->set_new_model_button(ui_->newModelButton);
    presenter_->set_timer_button(ui_->timerButton);
    presenter_->set_logs_browser(ui_->logsBrowser);
    presenter_->set_timer_time(ui_->timerSlider->value());

    //关联信号与槽
    //关联上一步按钮的点击信号与back_step_click()函数
    QObject::connect(ui_->backStepButton, SIGNAL(clicked()), this, SLOT(back_step_click()));

    //关联下一步按钮的点击信号与next_step_click()函数
    QObject::connect(ui_->nextStepButton, SIGNAL(clicked()), this, SLOT(next_step_click()));

    //关联新建模型按钮的点击信号与create_new_model_click()函数
    QObject::connect(ui_->newModelButton, SIGNAL(clicked()), this, SLOT(create_new_model_click()));

    //关联计时按钮的点击信号与switch_timer_click()函数
    QObject::connect(ui_->timerButton, SIGNAL(clicked()), this, SLOT(switch_timer_click()));

    //关联保存日志按钮的点击信号与save_logs_click()函数
    QObject::connect(ui_->saveButton, SIGNAL(clicked()), this, SLOT(save_logs_click()));

    //关联清空日志按钮的点击信号与clear_logs_click()函数
    QObject::connect(ui_->clearButton, SIGNAL(clicked()), this, SLOT(clear_logs_click()));

    //关联时间滑杆的改变信号与time_changed(int)函数
    QObject::connect(ui_->timerSlider, SIGNAL(valueChanged(int)), this, SLOT(time_changed(int)));

    //关联方向选择框的勾选信号与switch_direction()函数
    QObject::connect(ui_->toEndCheck, SIGNAL(clicked()), this, SLOT(switch_direction()));
}

MainWindow::~MainWindow()
{
    delete ui_;
    delete presenter_;

}


void MainWindow::next_step_click()
{
    presenter_->next_step();
}

void MainWindow::back_step_click()
{
    presenter_->back_step();
}

void MainWindow::switch_timer_click()
{
    presenter_->switch_timer();
}

void MainWindow::create_new_model_click()
{
    //用对话窗口设置汉诺塔大小
    Dialog* pInputDialog = new Dialog();
    if (pInputDialog->exec() == QDialog::Accepted)
    {
        int new_tower_size = pInputDialog->get_result();
        presenter_->create_new_model(new_tower_size);
        size = new_tower_size;
    }
    delete pInputDialog;

}

void MainWindow::switch_direction()
{
    presenter_->set_direction(ui_->toEndCheck->isChecked());
}

void MainWindow::clear_logs_click()
{
    presenter_->clear_logs();
}

void MainWindow::save_logs_click()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save to", "", "*.txt");

    if(str != "")
        presenter_->save_logs(str);
}

void MainWindow::on_startGameButton_clicked()
{
    StartGame* game = new StartGame();
    game->get_size(size);
    game->show();
}


void MainWindow::time_changed(int value)
{
    presenter_->set_timer_time((unsigned int)value);
}

void MainWindow::paintEvent(QPaintEvent *) //绘图事件,设置背景
{
    picture = ":/resource/background2";
    QPainter painter(this);//为画家指定画图设备为当前窗口
    //指定从窗口(0,0)点开始,画窗口大小的图
    QPixmap pixmap(picture);//创建pixmap对象,并加载图片
    painter.drawPixmap(0,0,this->width(),this->height(),pixmap);
}



void MainWindow::on_Pseudocode_clicked()
{
    // 创建一个 pseudo 对话框
    pseudo *pseudoDialog = new pseudo();
    presenter_->model_->setPsedo(pseudoDialog);
    //pseudoDialog->myMain = this;
    // 使用show()函数显示对话框
    pseudoDialog->show();

}

