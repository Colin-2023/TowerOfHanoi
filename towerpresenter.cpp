


#include "towerpresenter.h"

//初始化
tower_presenter::tower_presenter()
{
    timer_time_ = DEFAULTTIME;
    tower_height_ = DEFAULTSIZE;
    model_ = nullptr;
    params_ = nullptr;
    state_output_label_ = nullptr;
    logs_browser_ = nullptr;
    create_new_model(tower_height_);  //创建新模型

    init_timer();  //初始化计时器
}

//带label的初始化
tower_presenter::tower_presenter(QLabel *label)
{
    timer_time_ = DEFAULTTIME;
    tower_height_ = DEFAULTSIZE;
    model_ = nullptr;
    params_ = nullptr;
    logs_browser_ = nullptr;
    state_output_label_ = label;
    create_new_model(tower_height_);

    init_timer();
}

//带label和圆盘数量的初始化
tower_presenter::tower_presenter(QLabel *label,int n)
{
    timer_time_ = DEFAULTTIME;
    tower_height_ = n;
    model_ = nullptr;
    params_ = nullptr;
    logs_browser_ = nullptr;
    state_output_label_ = label;

    init_print_params(state_output_label_->size().height(), state_output_label_->size().width()); //初始化输出参数

    init_timer();
}

tower_presenter::~tower_presenter()
{
    delete tmr_;
    delete model_;
    delete params_;
}

//下一步操作
void tower_presenter::next_step()
{
    QString result = model_->next_step();  //执行模型的下一步操作
    if(!result.isEmpty())
    {
        logs_browser_->append("Next step : " + result + "\n");
    }
}

//上一步操作
void tower_presenter::back_step()
{
    QString result = model_->back_step();  //执行模型的上一步操作
    if(!result.isEmpty())
    {
        logs_browser_->append("Back step : " + result + "\n");
    }
}

//创建新模型
void tower_presenter::create_new_model(unsigned int tower_height)
{
    tower_height_ = tower_height;

    if (state_output_label_ != nullptr)
    {
        init_print_params(state_output_label_->size().height(), state_output_label_->size().width()); //初始化输出参数
    }
    else
    {
        init_print_params(DEFAULTHEIGHT, DEFAULTWIDTH);
    }

    if(model_ != nullptr) //如果已创建过模型
    {
        QObject::disconnect(model_, SIGNAL(model_updated(std::array<std::vector<DISC_>, 3>)), this, SLOT(on_model_updated(std::array<std::vector<DISC_>, 3>))); //取消关联model_的model_updated信号与on_model_updated()函数
        delete model_;  //删除该模型
    }

    model_ = new tower_model(tower_height);  //新建模型
    QObject::connect(model_, SIGNAL(model_updated(std::array<std::vector<DISC_>, 3>)), this, SLOT(on_model_updated(std::array<std::vector<DISC_>, 3>))); //关联model_的model_updated信号与on_model_updated()函数

    on_model_updated(model_->get_state());  //用当前状态更新模型

    if(logs_browser_ != nullptr)
        logs_browser_->append("@Created new model (" + QString::number(tower_height_) + ")\n");
}


//切换定时器状态（启用/关闭）
void tower_presenter::switch_timer()
{
    if(tmr_->isActive())  //如果定时器正在运行为true
    {
        //启用上一步下一步按钮
        next_step_button_->setEnabled(true);
        back_step_button_->setEnabled(true);

        tmr_->stop();  //关闭定时器
        logs_browser_->append("Stop timer\n");
    }
    else  //如果定时器处于关闭状态
    {
        //关闭上一步下一步按钮
        next_step_button_->setEnabled(false);
        back_step_button_->setEnabled(false);

        if (!to_end_) //如果反向标志为0（正向）
            logs_browser_->append("Start timer (next)\n");
        else
            logs_browser_->append("Start timer (back)\n");

        tmr_->start();  //启用定时器
    }
}

//设置定时器间隔
void tower_presenter::set_timer_time(unsigned int time)
{
    timer_time_ = (time == 0)? 1: time;

    tmr_->setInterval(timer_time_);
}

//设置正向反向（自动模式下）
void tower_presenter::set_direction(bool to_end)
{
    logs_browser_->append("*switch deriection to : ");
    logs_browser_->append((to_end) ? "back\n" : "next");
    to_end_ = to_end;
}

//保存日志
void tower_presenter::save_logs(QString patch)
{
    QFile fileOut(patch); // 对象与文件连接 fileout.txt
    if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream writeStream(&fileOut);
        writeStream << logs_browser_->toPlainText() << "\n";
        fileOut.close(); // 关闭文件

        logs_browser_->append("Save logs in file: " + patch+ "\n");
    }
    else{
        logs_browser_->append("#Error in save logs\n");
    }
}

//清除日志
void tower_presenter::clear_logs()
{
    logs_browser_->clear();
}


//--------------------------------set methods-------------------------
void tower_presenter::set_new_model_button(QPushButton *button)
{
    new_model_button_ = button;
}

void tower_presenter::set_timer_button(QPushButton *button)
{
    timer_button_ = button;
}

void tower_presenter::set_next_step_button(QPushButton *button)
{
    next_step_button_ = button;
}

void tower_presenter::set_back_step_button(QPushButton *button)
{
    back_step_button_ = button;
}

void tower_presenter::set_state_output_label(QLabel *label)
{
    state_output_label_ = label;
    init_print_params(state_output_label_->size().height(), state_output_label_->size().width());
}

void tower_presenter::set_logs_browser(QTextBrowser *browser)
{
    logs_browser_ = browser;
}


//---------------------------slots--------------------------------
//模型被更新信号的槽函数
void tower_presenter::on_model_updated(std::array<std::vector<DISC_>, 3> model_state)
{
    auto image = new QImage(params_->image_width, params_->image_height, QImage::Format_RGB32);
    auto painter = new QPainter(image);





    for(int y = 0; y < image->height(); ++y) {
        for(int x = 0; x < image->width(); ++x) {
            QRgb pixel = image->pixel(x, y);
            if (qRed(pixel) == 0 && qGreen(pixel) == 0 && qBlue(pixel) == 0) {
                image->setPixel(x, y, qRgb(255, 255, 255));
            }
        }
    }

    drow_base(*painter);
    drow_state(*painter, model_state);

    state_output_label_ -> setPixmap(QPixmap::fromImage(*image));

    painter->end();
    delete image;
    delete painter;
}


//------------------------private slots----------------------------
//更新时间
void tower_presenter::update_time()
{
    if(!to_end_)  //如果反向标志为0
        next_step();  //执行下一步操作
    else
        back_step();  //如果反向标志为1，执行上一步操作
}





//------------------------------private methods----------------------
//初始化输出参数
void tower_presenter::init_print_params(unsigned int height, unsigned int width)  //参数为标签的宽和高
{
    if(params_ != nullptr)
    {
        delete params_;  //清除已有参数
    }
    params_ = new PRINT_PARAMS_();  //新建参数

    if(tower_height_ + 2 >= height/3)
    {
        params_->image_height = (tower_height_ + 2) * 2; //图片高度=（塔的层数+上下边界）*2   （一层高度为2）
        auto ad_part_width = (tower_height_ * 2  * 3)/5; //边界部分的宽度=2/5塔的高度
        params_->image_width  = ad_part_width * 7; //图片宽度为边界部分宽度的7倍
    }
    else
    {
        params_->image_width  = width;  //图片宽度为标签的宽度
        params_->image_height = height; //图片高度为标签的高度
    }

    //params_->disc_height = params_->image_height / (tower_height_ + 2); // 盘子高度=图片高度/（塔的层数+上下边界）
    params_->disc_height = 20; // 盘子高度=图片高度/（塔的层数+上下边界）
    auto part_width  = params_->image_width / 7;
    params_->column_width = part_width * 5 / 3; //柱子宽度（3个）
    params_->space_horizontal = part_width * 2 / 4; //水平间隔（4个）

    params_->column_height = params_->disc_height * tower_height_; //柱子高度=盘子高度*塔的层数
    params_->disc_width_delta = params_->column_width / tower_height_;  //盘子宽度增量=柱子宽度/塔的层数
    //params_->kernel_width = params_->disc_width_delta;  //内核宽度为盘子宽度增量
    params_->kernel_width = 5;  //内核宽度为盘子宽度增量
    params_->kernel_height = params_->image_height;  //内核高度为图片高度
    params_->base_height = params_->disc_height;  //基座高度为盘子高度
}


//初始化定时器
void tower_presenter::init_timer()
{
    tmr_ = new QTimer();  //创建新的定时器
    tmr_->setInterval(timer_time_);  //设置定时器时间间隔
    connect(tmr_, SIGNAL(timeout()), this, SLOT(update_time())); //关联定时器超时信号与update_time()函数
}


//绘制基座
void tower_presenter::drow_base(QPainter &p)
{
    p.setPen(QPen(Qt::black,1,Qt::SolidLine));
    p.setBrush(Qt::black);

    for (auto i = 0; i < 3; i++)
    {
        auto column_left_margin = params_->space_horizontal + i * (params_->column_width + params_->space_horizontal);
        auto kernel_left_margin = column_left_margin + (params_->column_width - params_->kernel_width)/2;

        p.drawRect(kernel_left_margin, 0, params_->kernel_width, params_->kernel_height);
    }

    p.drawRect(0, params_->image_height - 1 - params_->base_height, params_->image_width, params_->base_height);
}


//绘制当前状态
void tower_presenter::drow_state(QPainter &p, const std::array<std::vector<DISC_>, 3> &model_state)
{
    p.setBrush(QColorConstants::Svg::lightblue);
    //遍历每个塔
    for(auto i = 0; i < 3; i++)
    {
        //设置间距
        auto botom_margin = params_->base_height;
        auto column_left_margin = params_->space_horizontal + i * (params_->column_width + params_->space_horizontal);

        //遍历该塔上的盘子，绘制每个的盘子
        for(auto j = 0; j < model_state[i].size(); j++)
        {
            auto curent_mass = model_state[i][j].mass; //获取当前盘子的质量
            //设置盘子的位置
            botom_margin += params_->disc_height;

            auto disc_width = curent_mass * params_->disc_width_delta;
            auto disc_left_margin = column_left_margin + (params_->column_width - disc_width)/2;

            //绘制盘子
            p.drawRect(disc_left_margin, params_->image_height - botom_margin, disc_width , params_->disc_height);
        }
    }
}

