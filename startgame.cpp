#include "startgame.h"
#include "ui_startgame.h"

StartGame::StartGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartGame)
{
    ui->setupUi(this);

    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    //将柱子标志插入到map
    disks_.insert({"first", {}});
    disks_.insert({"second", {}});
    disks_.insert({"third", {}});

    //创建定时器
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(changeTime()));

}

StartGame::~StartGame()
{
    delete ui;
    //delete presenter;
}

void StartGame::get_size(int size_get)  //在主界面点击Start Game时被调用
{
    amount_of_disks_ = size_get;
}


void StartGame::on_newGameButton_clicked()
{
    if(ui->newGameButton->text()=="Restart")
    {
        for ( auto& pair : disks_ )
        {
            for ( QGraphicsRectItem* disk : pair.second )
            {
                delete disk;
            }
            pair.second.clear();
        }
        current_time_ = {0, 0};

        ui->currentMinLcdNumber->display(current_time_.first);
        ui->currentSecLcdNumber->display(current_time_.second);
        ui->winLabel->clear();
        ui->gameLogTextBrowser->clear();
        ui->illegalLabel->clear();
        stepnum = 0;
        ui->steptextBrowser->clear();
    }

    //设置按钮可用
    ui->newGameButton->setText("Restart");
    ui->ABButton->setEnabled(true);
    ui->ACButton->setEnabled(true);
    ui->BAButton->setEnabled(true);
    ui->BCButton->setEnabled(true);
    ui->CAButton->setEnabled(true);
    ui->CBButton->setEnabled(true);

    // 在每个柱子位置创建一个柱子
    for (int i = 0; i < amount_of_disks_; i++)
    {
        // 创建一个柱子（矩形）
        int pillarWidth = 5; // 柱子的宽度
        int pillarHeight = 500; // 柱子的高度与盘子高度相关
        QGraphicsRectItem* pillar = scene_->addRect(0, 0, pillarWidth, pillarHeight);

        // 计算柱子的位置，将其放在盘子上方
        int pillarX = TAPS_x_coords.at("first") - pillarWidth / 2;
        int pillarY = BORDER_DOWN-BOTTOM_OF_TAP ;
        pillar->setPos(pillarX, pillarY);

        // 设置柱子的颜色，例如将其设置为灰色
        QBrush pillarBrush(Qt::black); // 这里使用灰色，您可以选择其他颜色
        pillar->setBrush(pillarBrush);
    }

    for (int i = 0; i < amount_of_disks_; i++)
    {
        // 创建一个柱子（矩形）
        int pillarWidth = 5; // 柱子的宽度
        int pillarHeight = 500; // 柱子的高度与盘子高度相关
        QGraphicsRectItem* pillar = scene_->addRect(0, 0, pillarWidth, pillarHeight);

        // 计算柱子的位置，将其放在盘子上方
        int pillarX = TAPS_x_coords.at("second") - pillarWidth / 2;
        int pillarY = BORDER_DOWN-BOTTOM_OF_TAP ;
        pillar->setPos(pillarX, pillarY);

        // 设置柱子的颜色，例如将其设置为灰色
        QBrush pillarBrush(Qt::black); // 这里使用灰色，您可以选择其他颜色
        pillar->setBrush(pillarBrush);
    }

    for (int i = 0; i < amount_of_disks_; i++)
    {
        // 创建一个柱子（矩形）
        int pillarWidth = 5; // 柱子的宽度
        int pillarHeight = 500; // 柱子的高度与盘子高度相关
        QGraphicsRectItem* pillar = scene_->addRect(0, 0, pillarWidth, pillarHeight);
        QGraphicsRectItem* pillar2 = scene_->addRect(0, 0, 1020, 25);

        // 计算柱子的位置，将其放在盘子上方
        int pillarX = TAPS_x_coords.at("third") - pillarWidth / 2;
        int pillarY = BORDER_DOWN-BOTTOM_OF_TAP ;
        pillar->setPos(pillarX, pillarY);
        pillar2->setPos(0, 480);

        // 设置柱子的颜色，例如将其设置为灰色
        QBrush pillarBrush(Qt::black); // 这里使用灰色，您可以选择其他颜色
        pillar->setBrush(pillarBrush);
        pillar2->setBrush(pillarBrush);
    }


    timer_->start(1000);
    ui->bestMinLcdNumber->display(best_time_.first);
    ui->bestSecLcdNumber->display(best_time_.second);

    int width_difference = (BIGGEST_DISK_WIDTH-SMALLEST_DISK_WIDTH)
                            / amount_of_disks_;  //盘子宽度差

    disk_height_ = NORMAL_DISK_HEIGHT_;
    //如果圆盘大小不适合场景，则调整圆盘高度直到它们适合场景
    while ( amount_of_disks_ * disk_height_ > BOTTOM_OF_TAP )
    {
       disk_height_ -= 1;
    }

    for (int i = 0; i < amount_of_disks_; i++)
    {
        QGraphicsRectItem* disk;
        int offset;
        int width = BIGGEST_DISK_WIDTH - i * width_difference;
        offset = width / 2;
        disk = scene_->addRect(0, 0, width, disk_height_);

        disk->moveBy(TAPS_x_coords.at("first") - offset,
                     BOTTOM_OF_TAP - i * disk_height_);

        // 设置矩形的填充颜色，例如将其设置为红色
        QBrush brush(QColorConstants::Svg::lightblue); // 这里使用红色，您可以选择其他颜色
        disk->setBrush(brush);

        disks_.at("first").push_back(disk);
    }

}



bool StartGame::move_disk(std::string from, std::string to)
{
    ui->illegalLabel->clear();  //清空非法标志
    if ( ! disks_.at(from).empty() )
    {
        QGraphicsRectItem* disk = disks_.at(from).back();
        if ( not disks_.at(to).empty() )
        {
            //圆盘所在的柱子坐标 - 圆盘的x坐标 = 圆盘宽度的一半
            //以此检查移动是否非法
            if ( ( TAPS_x_coords.at(from) - disk->x() ) >
                 ( TAPS_x_coords.at(to) - disks_.at(to).back()->x()) )
            {
                ui->illegalLabel->setText("Illegal move!\nBigger disk on top of smaller.");
                return false;
            }
        }
        //计算圆盘需要移动的距离
        int disks_at_from = disks_.at(from).size();
        int disks_at_to = disks_.at(to).size();
        int delta_x = TAPS_x_coords.at(to) - TAPS_x_coords.at(from);
        int delta_y = (disks_at_from - disks_at_to - 1) * disk_height_;
        disk->moveBy(delta_x, delta_y);

        //移动圆盘
        disks_.at(from).pop_back();
        disks_.at(to).push_back(disk);

        //检查游戏是否胜利
        check_win();

        return true;
    }
    else
    {
        ui->illegalLabel->setText("No disk to move.");
        return false;
    }

}

void StartGame::check_win()
{
    if ( disks_.at("first").empty() && disks_.at("third").size() == amount_of_disks_)
    {
        ui->winLabel->setText("You have won!");

        //设置按钮不可用
        ui->ABButton->setEnabled(false);
        ui->ACButton->setEnabled(false);
        ui->BAButton->setEnabled(false);
        ui->BCButton->setEnabled(false);
        ui->CAButton->setEnabled(false);
        ui->CBButton->setEnabled(false);

        timer_->stop();

        if ( best_time_.first == 0 and best_time_.second == 0 )
        {
            best_time_ = current_time_;
        }
        else if ( best_time_.first > current_time_.first)
        {
            best_time_ = current_time_;
        }
        else if ( best_time_.first == current_time_.first and
                  current_time_.second < best_time_.second )
        {
            best_time_ = current_time_;
        }
        ui->bestMinLcdNumber->display(best_time_.first);
        ui->bestSecLcdNumber->display(best_time_.second);
    }
}



void StartGame::on_ABButton_clicked()
{
    if ( move_disk("first", "second") )
    {
        stepnum++;
        ui->gameLogTextBrowser->append("Step "+QString::number(stepnum,10)+" : A -> B");
        ui->steptextBrowser->setText(QString::number(stepnum,10));
    }

}


void StartGame::on_ACButton_clicked()
{
    if ( move_disk("first", "third") )
    {
        stepnum++;
        ui->gameLogTextBrowser->append("Step "+QString::number(stepnum,10)+" : A -> C");
        ui->steptextBrowser->setText(QString::number(stepnum,10));
    }
}

void StartGame::on_BAButton_clicked()
{
    if ( move_disk("second", "first") )
    {
        stepnum++;
        ui->gameLogTextBrowser->append("Step "+QString::number(stepnum,10)+" : B -> A");
        ui->steptextBrowser->setText(QString::number(stepnum,10));
    }
}

void StartGame::on_BCButton_clicked()
{
    if ( move_disk("second", "third") )
    {
        stepnum++;
        ui->gameLogTextBrowser->append("Step "+QString::number(stepnum,10)+" : B -> C");
        ui->steptextBrowser->setText(QString::number(stepnum,10));
    }
}


void StartGame::on_CAButton_clicked()
{
    if ( move_disk("third", "first") )
    {
        stepnum++;
        ui->gameLogTextBrowser->append("Step "+QString::number(stepnum,10)+" : C -> A");
        ui->steptextBrowser->setText(QString::number(stepnum,10));
    }
}


void StartGame::on_CBButton_clicked()
{
    if ( move_disk("third", "second") )
    {
        stepnum++;
        ui->gameLogTextBrowser->append("Step "+QString::number(stepnum,10)+" : C -> B");
        ui->steptextBrowser->setText(QString::number(stepnum,10));
    }
}

void StartGame::changeTime()
{
    ++current_time_.second;
    if ( current_time_.second == 60 )
    {
        ++current_time_.first;
        current_time_.second = 0;
    }
    ui->currentMinLcdNumber->display(current_time_.first);
    ui->currentSecLcdNumber->display(current_time_.second);

}
void StartGame::paintEvent(QPaintEvent *) //绘图事件,设置背景
{
    picture = ":/resource/background2";
    QPainter painter(this);//为画家指定画图设备为当前窗口
    //指定从窗口(0,0)点开始,画窗口大小的图
    QPixmap pixmap(picture);//创建pixmap对象,并加载图片
    painter.drawPixmap(0,0,this->width(),this->height(),pixmap);
}



void StartGame::on_pushButton_clicked()
{
    QMessageBox::about(nullptr,"Game Introduction","   There are three poles, A, B, and C. Pole A has N disks stacked on it, with the size of the disks decreasing from bottom to top. \n"
                                      "   The objective is to move all the disks to pole C, following these rules: only one disk can be moved at a time, and a larger disk cannot be placed on top of a smaller disk.\n"
                                      "   First, click on the \"Start\" button, and then click on the button below the pole to begin the game!\n"
                                                   "   ( If you want to change the number of discs, click \"New Model\" in the main window, select the number, and re-enter \"Start Game\". )");

}

