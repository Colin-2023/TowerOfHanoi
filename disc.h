#pragma once

struct DISC_  //圆盘
{
    unsigned int mass;  //圆盘质量
    //size_t position;

    DISC_(unsigned int mass)  //设置质量
    {
        this->mass = mass;
    }
};
