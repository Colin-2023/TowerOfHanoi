#ifndef STARTGAME_H
#define STARTGAME_H

#include <QMainWindow>
#include "towerpresenter.h"
#include "diskstate.h"

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <map>
#include <deque>
#include <QTimer>
#include <QKeyEvent>

namespace Ui {
class StartGame;
}

class StartGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartGame(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);   //绘图事件,设置背景
    ~StartGame();

    void get_size(int size_get); //获得设置的圆盘数量

private slots:
    void on_BAButton_clicked();

    void on_ABButton_clicked();

    void on_ACButton_clicked();

    void on_BCButton_clicked();

    void on_CAButton_clicked();

    void on_CBButton_clicked();

    void changeTime();

    void on_newGameButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::StartGame *ui;
    //tower_presenter *presenter;
    int amount_of_disks_ = 10 ; //圆盘数量
    int stepnum=0;//步数
    QString picture;
    QGraphicsScene* scene_;  //图形视图框架
    QTimer* timer_;     //定时器

    //不同柱子上的盘子
    std::map<const std::string, std::vector<QGraphicsRectItem*>> disks_;

    //游戏面板的坐标
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 500;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 750;

    //柱子坐标
    const int BOTTOM_OF_TAP = BORDER_DOWN - 40;
    std::map<const std::string, const int> TAPS_x_coords =
    {
        {"first" , BORDER_RIGHT/6},
        {"second" , BORDER_RIGHT/2},
        {"third" , BORDER_RIGHT*5/6}
    };

    //圆盘
    const int BIGGEST_DISK_WIDTH = 220;  //起始盘子宽度
    const int SMALLEST_DISK_WIDTH = 20;  //最小盘子宽度
    const int NORMAL_DISK_HEIGHT_ = 20;  //正常盘子高度

    int disk_height_ = NORMAL_DISK_HEIGHT_; //盘子高度

    //时间
    std::pair<int, int> current_time_ = {0, 0};
    std::pair<int, int> best_time_ = {0, 0};

//    std::array<std::vector<DISC_>, 3> current_state_;  //当前状态数组，数组元素类型为vector<DISC_>，元素个数为3（3个柱子）

    void new_game();  //初始化新游戏
    bool move_disk(std::string from, std::string to);  //移动盘子
    void check_win();  //检查是否胜利


};

#endif // STARTGAME_H
