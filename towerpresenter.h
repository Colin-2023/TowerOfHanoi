#pragma once

#include "towermodel.h"
#include "printparams.h"

#include "QPushButton"
#include "QSlider"
#include "QLabel"
#include "QTextBrowser"
#include "QImage"
#include "QPainter"
#include "QPixmap"
#include "QTimer"
#include "QFile"
#include "QTextStream"
#include "QString"

#include <QObject>

#define DEFAULTSIZE 10 //default size
#define DEFAULTHEIGHT 400 //default height
#define DEFAULTWIDTH 800 //default width
#define DEFAULTTIME 1000 //default time
#define COLUMNS_COUNT 3 //columns count

class tower_presenter : public QObject
{
    Q_OBJECT

private:
    QTimer *tmr_;  //定时器


    unsigned int tower_height_; //塔高（层数）

    unsigned int timer_time_;  //定时器时间间隔

    bool to_end_ = false; //反向标志，为0正向，为1反向（自动模式下）

private:
    QPushButton *new_model_button_;
    QPushButton *timer_button_;
    QPushButton *next_step_button_;
    QPushButton *back_step_button_;

    QSlider *timer_time_slider_;

    QLabel *state_output_label_;

    QTextBrowser *logs_browser_;

public:
    tower_model *model_; //基础模型
    PRINT_PARAMS_ *params_;  //输出参数

    tower_presenter();  //初始化
    tower_presenter(QLabel *label);  //带label的初始化
    tower_presenter(QLabel *label, int n);  //带label和圆盘数量的初始化
    ~tower_presenter();  //析构

    void next_step();  //下一步操作
    void back_step();  //上一步操作
    void create_new_model(unsigned int tower_height);  //创建新模型

    void switch_timer(); //切换定时器状态（启用/关闭）
    void set_timer_time(unsigned int time); //设置定时器间隔
    void set_direction(bool to_end); //设置正向反向（自动模式下）
    void save_logs(QString patch);  //保存日志
    void clear_logs();  //清除日志


    //set操作，设置控件
    void set_new_model_button(QPushButton *button);
    void set_timer_button(QPushButton *button);
    void set_next_step_button(QPushButton *button);
    void set_back_step_button(QPushButton *button);
    void set_state_output_label(QLabel *label);
    void set_logs_browser(QTextBrowser *browser);


public slots:
    void on_model_updated(std::array<std::vector<DISC_>, 3> model_state);  //模型被更新信号的槽函数（实现当前状态的绘制）


private slots:
    void update_time();  //更新时间

private:
    void init_print_params(unsigned int height, unsigned int width);   //初始化输出参数
    void init_timer();  //初始化计时器

    void drow_base(QPainter &p); //绘制基座
    void drow_state(QPainter &p, const std::array<std::vector<DISC_>, 3> &model_state);  //绘制当前状态

};
