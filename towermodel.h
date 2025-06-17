#pragma once
#include "diskstate.h"
#include "disc.h"

#include <vector>
#include <array>
#include <iostream>
#include <QObject>
#include <QString>
#include <QMessageBox>
#include <QApplication>
#include "pseudo.h"

//using size_t = unsigned int;

struct DISC_STATE_;
struct DISC_;

class tower_model: public QObject
{
   Q_OBJECT

private:
    std::vector<DISC_STATE_> stack_;    //所有圆盘状态栈（移动依据）

    std::array<std::vector<DISC_>, 3> current_state_;  //当前状态数组，数组元素类型为vector<DISC_>，元素个数为3（3个柱子）

    size_t tower_size_;  //塔的大小(圆盘总数）

    pseudo* pseudo_ = new pseudo();

public:
    int is_not_end=0;//圆盘移动是否结束标志（6为已经结束一次，1为尚未结束，0为初始结束判断状态

    void print_current_state();  //输出当前状态

    std::array<std::vector<DISC_>, 3> get_state();  //获取当前状态

    QString next_step();  //下一步操作

    QString back_step();  //上一步操作

    tower_model(int tower_size);

    void setPsedo(pseudo* pseudo_t);

    void HANOI(int x,int y,int z,int w);

    void MOVE(int y,int z);



signals:
    void model_updated(std::array<std::vector<DISC_>, 3> model_state); //模型被更新信号

private:
    void update_model(size_t src, size_t dest);
};

