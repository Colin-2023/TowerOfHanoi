#include "towermodel.h"

tower_model::tower_model(int tower_size)
{
    tower_size_ = tower_size;  //设置塔的大小
    stack_ = std::vector<DISC_STATE_>(); //创建DISC_STATE_数组
    current_state_ = std::array<std::vector<DISC_>, 3>(); //创建当前状态

    for (int i = 0 ; i < tower_size; i++)
    {
        int mass = tower_size - i; //圆盘下标
        DISC_ disc = DISC_(mass); //创建圆盘
        current_state_[0].push_back(disc); //将圆盘添加进第一根柱子
    }

    DISC_STATE_ state = DISC_STATE_(tower_size, 1, 3, 2);
    //创建一个圆盘状态，圆盘总数为tower_size，源位置为1，目标位置为3，临时位置为2，默认步数为0

    stack_.push_back(state);  //将该state添加进圆盘状态栈
}

void tower_model::setPsedo(pseudo *pseudo_t)
{
    pseudo_= pseudo_t;
}


//输出当前状态
void tower_model::print_current_state()
{
    for(int i = 0; i < 3; i++) //遍历柱子
    {
        std::cout << "column #" << i + 1 << "\n";
        if(current_state_[i].size() != 0)  //如果当前状态该柱子上圆盘数不为0
        {
            for(int j = current_state_[i].size() - 1; j >= 0 ; --j)
            {
                std::cout << current_state_[i][j].mass << "\n"; //输出该柱子圆盘的编号
            }
        }else
        {
            std::cout << "-------\n";
        }
    }
}


//获取当前状态
std::array<std::vector<DISC_>, 3> tower_model::get_state()
{
    return current_state_;
}


//下一步操作
/*
 1.初始化变量 model_is_updated 为空字符串，表示模型未被更新。
 2.初始化布尔变量 is_not_end 为 false，表示圆盘移动已经结束。
 3.遍历所有圆盘状态数组，如果存在任何一个圆盘的步数不等于2，将 is_not_end 设置为 true，表示圆盘移动尚未结束。
 4.进入循环，当模型未被更新、圆盘状态栈不为空且 is_not_end 为 true 时继续循环。
 5.获取圆盘状态栈的栈顶元素，保存在引用变量 state 中。
 6.如果 state 的步数为0，表示需要执行第一个步骤：
  ①将 state 的步数设置为1，表示已经执行了第一个步骤。
  ②如果圆盘数 state.n 不等于1，创建一个新的圆盘状态 newState，圆盘数减一，源柱为 state.src，中间柱为 state.tmp，目标柱为 state.dest。
  ③将 newState 添加到圆盘状态栈中。
 7.如果 state 的步数为1，表示需要执行第二个步骤：
  ①更新模型状态，将源柱 state.src 上的圆盘移动到目标柱 state.dest。
  ②记录更新的信息，将源柱和目标柱的编号拼接成字符串，保存在 model_is_updated 中。
  ③将 state 的步数设置为2，表示已经执行了第二个步骤。
  ④如果圆盘数 state.n 不等于1，创建一个新的圆盘状态 newState，圆盘数减一，源柱为 state.tmp，目标柱为 state.dest，中间柱为 state.src。
  ⑤ 将 newState 添加到圆盘状态栈中。
 8.如果 state 的步数为2，表示当前圆盘移动已经完成，将其从圆盘状态栈中移除。
 9.返回更新的信息 model_is_updated。
 */
QString tower_model::next_step()
{
    QString model_is_updated = "";
    for(int i = 0;  i < int(stack_.size()); i++) //遍历所有圆盘状态
    {
        if(is_not_end ==6){//已经结束过一次，标志定为6，防止演示计时重复调用 重复结束
            break;
        }
        if(stack_[i].step != 2)  //如果某个圆盘的步数不为2
        {
            is_not_end = 1;  //设置is_not_end标志为ture
            break;
        }
        else{
            is_not_end = 0;
        }
    }
    //当模型未更新、栈不为空且不满足终止条件时，进行循环
    /*循环条件：
    1.model_is_updated不为空字符串
    2.所有圆盘状态数组不为空
    3.且is_not_end为ture
    */
    while (model_is_updated.isEmpty() && stack_.size() != 0 && is_not_end)
    {
        DISC_STATE_ &state = stack_.back(); //获取圆盘状态栈的栈顶元素

        //如果圆盘的步数为0
        if(state.step == 0)
        {
            state.step = 1; //进行第一个步骤，并设置步数为1

            if(state.n != 1) //如果圆盘数不等于1
            {
                DISC_STATE_ newState = DISC_STATE_(state.n - 1, state.src, state.tmp, state.dest);  //新增圆盘状态，需要移动的圆盘数=原圆盘数-1，源->目标->中间，默认步数为0
                HANOI(state.n - 1, state.src, state.dest, state.tmp);
                stack_.push_back(newState); //将新增圆盘状态添加进圆盘状态栈
            }
        }
        //如果圆盘的步数为1
        else if (state.step == 1)
        {
            update_model(state.src, state.dest); //更新模型状态，源—>目标
            MOVE(state.src, state.dest);
            model_is_updated = "[" + QString::number(state.src) + "]->[" + QString::number(state.dest)+"]"; //记录更新的信息
            state.step = 2; //进行第二个步骤，并设置步数为2

            if(state.n != 1) //如果圆盘数不等于1
            {
                DISC_STATE_ newState = DISC_STATE_(state.n - 1, state.tmp, state.dest, state.src);  //新增圆盘状态，需要移动的圆盘数=原圆盘数-1，中间->源->目标，默认步数为0
                HANOI(state.n - 1, state.tmp, state.src, state.dest);
                stack_.push_back(newState); //将新增圆盘状态添加进圆盘状态栈
            }
        }
        //如果圆盘的步数为2
        else if (state.step == 2)
        {
            stack_.pop_back(); //移动完毕，圆盘状态栈的栈顶元素出栈
        }
    }

    for(int i = 0;  i < int(stack_.size()); i++) //遍历所有圆盘状态
    {
        if(is_not_end ==6){
            break;
        }
        if(stack_[i].step != 2)  //如果某个圆盘的步数不为2
        {
            is_not_end = 1;  //设置is_not_end标志为ture
            break;
        }
        else{
            is_not_end = 0;
        }
    }
   if(is_not_end == 0)  //所有圆盘的步数都为2
    {
      QMessageBox::about(nullptr,"Prompt","The demonstration has ended.");
      is_not_end =6;
    }

    return  model_is_updated;  //返回更新的信息
}


//上一步操作
/*
1.初始化变量 model_is_updated 为空字符串，表示模型未被更新。
2.初始化布尔变量 is_not_start 为 false，表示游戏已经开始。
3.检查第一根柱子上的圆盘数量是否等于圆盘总数，如果是，则将 is_not_start 设置为 true，表示游戏还未开始。
4.进入循环，当模型未被更新、圆盘状态栈不为空且游戏已经开始时继续循环。
5.获取圆盘状态栈的栈顶元素，保存在引用变量 state 中。
6.如果 state 的步数为2，表示需要执行第二个步骤的逆向操作：
    ①将 state 的步数设置为1，表示已经执行了第二个步骤的逆向操作。
    ②记录更新的信息，将目标柱和源柱的编号拼接成字符串，保存在 model_is_updated 中。
    ③更新模型状态，将目标柱 state.dest 上的圆盘移动回源柱 state.src。
7.如果 state 的步数为1，表示需要执行第一个步骤的逆向操作：
    ①备份需要移动的盘数为 copy_n，初始化布尔变量 is_happened 为 false，表示逆向操作是否发生，复制当前圆盘状态为 state_copy，初始化布尔变量 first_generate 为 true，表示第一次生成圆盘状态。
    ②进入循环，当 n 大于1时，循环执行逆向操作：
        如果是第一次生成圆盘状态，创建一个新的圆盘状态 newState，圆盘数减一，源柱为 state_copy.src，中间柱为 state_copy.tmp，目标柱为 state_copy.dest，步数为2。
        否则，创建一个新的圆盘状态 newState，圆盘数减一，源柱为 state_copy.tmp，中间柱为 state_copy.dest，目标柱为 state_copy.src，步数为2。
        将 newState 添加到圆盘状态栈中，更新 state_copy 为圆盘状态栈的栈顶元素，将 first_generate 设置为 false，表示不是第一次生成圆盘状态，将 is_happened 设置为 true，表示逆向操作已发生。
    ③如果 is_happened 为 false，表示逆向操作未发生，将 state 的步数减一。
8.如果 state 的步数为0，表示当前圆盘状态已处理完毕，将其从圆盘状态栈中移除。
    异常处理：尝试获取圆盘状态栈的新栈顶元素 two_state。
        如果 two_state 的步数不等于2，将其步数减一。
9.返回更新的信息 model_is_updated。
*/
QString tower_model::back_step()
{
    QString model_is_updated = "";

    bool is_not_start = 0;  //未开始标志

    if (current_state_[0].size() == tower_size_)
    {
        is_not_start = true;  //若第一根柱子上圆盘的数量为圆盘总数，则未开始
    }

    while (model_is_updated.isEmpty() && stack_.size() != 0 && !is_not_start)
    {
        DISC_STATE_ &state = stack_.back();  //获取圆盘状态栈的栈顶元素

        //如果圆盘的步数为2
        if (state.step == 2)
        {
            state.step = 1; //设置步数为1
            model_is_updated = "[" + QString::number(state.dest) + "]->[" + QString::number(state.src)+"]"; //记录更新的信息
            update_model(state.dest, state.src);  //更新模型状态，目标—>源
        }
        //如果圆盘的步数为1
        else if (state.step == 1)
        {
            int copy_n = state.n; //备份需要移动的盘数

            bool is_happened = false;
            auto state_copy = state;  //备份当前圆盘状态
            bool first_generate = true;  //第一次生成
            for (int n = copy_n; n > 1; n--)
            {
                if(first_generate)
                {
                    DISC_STATE_ newState = DISC_STATE_(state_copy.n - 1, state_copy.src, state_copy.tmp, state_copy.dest, 2); //新增圆盘状态，需要移动的圆盘数=原圆盘数-1，源->中间->目标，步数为2
                    stack_.push_back(newState); //将新增圆盘状态添加进圆盘状态栈
                }
                else
                {
                    DISC_STATE_ newState = DISC_STATE_(state_copy.n - 1, state_copy.tmp, state_copy.dest, state_copy.src, 2); //新增圆盘状态，需要移动的圆盘数=原圆盘数-1，中间->源->目标，步数为2
                    stack_.push_back(newState); //将新增圆盘状态添加进圆盘状态栈
                }

                state_copy = stack_.back();
                first_generate = false;
                is_happened = true;
            }

            if(!is_happened)
                state.step--;

        }
        //如果圆盘的步数为0
        else if (state.step == 0)
        {

            stack_.pop_back(); //圆盘状态栈的栈顶元素出栈

            try {
                auto &two_state = stack_.back();  //获取圆盘状态栈的新栈顶元素
                if (two_state.step != 2)
                    two_state.step = two_state.step - 1;

            } catch (...)
            {

            }

        }
    }
    return  model_is_updated;  //返回更新的信息
}


//更新模型状态（真正的移动操作）
void tower_model::update_model(size_t src, size_t dest)
{
    DISC_ &moved_disc = current_state_[src-1].back(); //移动的圆盘为源位置的栈顶圆盘
    current_state_[src-1].pop_back(); //源位置的栈顶圆盘出栈

    current_state_[dest-1].push_back(moved_disc);  //将圆盘添加进目标位置栈

    emit model_updated(current_state_); //发送模型被更新的信号
}

void tower_model::HANOI(int x, int y, int z, int w)
{
    //pseudo* pseudo_ = new pseudo();
    QString str;
    str += "HANOI(";

    str += QString::number(x); // 将整数 x 转换为字符串并附加到 str
    str+=",";

    if (y == 1) str += "X,";
    else if (y == 2) str += "Y,";
    else str += "Z,";

    if (z == 1) str += "X,";
    else if (z == 2) str += "Y,";
    else str += "Z,";

    if (w == 1) str += "X)";
    else if (w == 2) str += "Y)";
    else str += "Z)";

    pseudo_->updateTextBrowser(str);

    QApplication::processEvents(); // 强制更新界面
}


void tower_model::MOVE(int y, int z)
{

    QString str;
    str += "MOVE(";

    if (y == 1) str += "X,";
    else if (y == 2) str += "Y,";
    else str += "Z,";

    if (z == 1) str += "X)";
    else if (z == 2) str += "Y)";
    else str += "Z)";
    pseudo_->updateTextBrowser(str);
}
