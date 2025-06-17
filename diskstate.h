#pragma once


struct DISC_STATE_  //圆盘状态
{
    unsigned int n;     //需要移动的圆盘数
    unsigned int src;  //源位置
    unsigned int dest; //目标位置
    unsigned int tmp;  //临时位置（中间位置）
    unsigned int step; //步数

    DISC_STATE_(unsigned int n, unsigned int src, unsigned int dest, unsigned int tmp, unsigned int step = 0)
    {
        this->n = n;
        this->src = src;
        this->dest = dest;
        this->tmp = tmp;
        this->step = step;
    }

    DISC_STATE_()
    {
        n = 0;
        src = 0;
        dest = 0;
        tmp = 0;
        step = 0;
    }

};

