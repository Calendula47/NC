#include "mihanshuxiaoshu.h"
#include <QDebug>
#include <QtMath>
#include <QPainter>
#include <math.h>
MIHANSHUXIAOSHU::MIHANSHUXIAOSHU(QWidget *parent) : QWidget{parent}
{

}
MIHANSHUXIAOSHU::~MIHANSHUXIAOSHU()
{

}
#include <vector>
#include <cmath>

int changdu_dengchang;
int changdu_dengwucha;
int changdu_compensate;
int error_flag;
double *x_dengchangdu = new double[32*1024*1024]; // 等间距的x数组存放
double *y_dengchangdu = new double[32*1024*1024]; // 等间距的y数组存放
double *x_dengwucha = new double[32*1024*1024]; // 等误差的x数组存放
double *y_dengwucha = new double[32*1024*1024];//等误差的y数组存放
double *x_toolcompensate = new double[32*1024*1024];
double *y_toolcompensate = new double[32*1024*1024];
double dengchang_max_error;
std::vector<double> changdu_x_nodes;
std::vector<double> changdu_y_nodes;
std::vector<double> dengwucha_x_nodes;
std::vector<double> dengwucha_y_nodes;

//调用等间距逼近算法来进行绘图
void MIHANSHUXIAOSHU::PaintMihanshuxiaoshu_dengjianju(QPainter *painter)
{

  for(int j=1;j<changdu_dengchang;j++)
    {
        painter->drawLine(x_dengchangdu[j-1],y_dengchangdu[j-1],x_dengchangdu[j],y_dengchangdu[j]);
      if(j%1000==0){qDebug()<<"paint"<<j<<x_dengchangdu[j]<<y_dengchangdu[j];}
    }

}

//调用等误差逼近算法来进行绘图
void MIHANSHUXIAOSHU::PaintMihanshuxiaoshu_dengwucha(QPainter *painter)
{
    for(int j=1;j<changdu_dengwucha;j++)
    {
        painter->drawLine(x_dengwucha[j-1],y_dengwucha[j-1],x_dengwucha[j],y_dengwucha[j]);
    }
}

//刀补绘制
void MIHANSHUXIAOSHU::Paintmihanshu_toolcompensate(QPainter *painter)
{
    for(int j=1;j<changdu_compensate;j++)
    {
        painter->drawLine(x_toolcompensate[j-1],y_toolcompensate[j-1],x_toolcompensate[j],y_toolcompensate[j]);
    }
}

//定义幂函数
double MIHANSHUXIAOSHU::mihanshu(double x, double zhishu) {
    return pow(x, zhishu);
}

//计算两节点，并确定直线与幂函数相切线的逼近误差
double MIHANSHUXIAOSHU::bijinwucha(double x0,double x1, double zhishu    )
{
    double k = (mihanshu(x1, zhishu) - mihanshu(x0,zhishu)) / (x1 - x0);//斜率
    double qie_x = fabs(pow(k / zhishu, (1 / (zhishu - 1))));//切点坐标x
    double qie_y = mihanshu(qie_x, zhishu);//切点坐标y
    double error = fabs(qie_y-k*qie_x+k*x0-mihanshu(x0,zhishu)) / sqrt(pow(k,2)+1) ;//逼近误差
    return error;
}

//等间距计算各个节点的坐标
double MIHANSHUXIAOSHU::dengchang_zuobiao(double a, double b, double zhishu, double max_error)
{
    int num_intervals = ceil((b - a) / max_error); //计算节点间隔的个数
    double step_size = (b - a) / num_intervals; //计算节点间隔的长度
    double x = a; // 当前节点的x坐标
    while (x < b) {
        changdu_x_nodes.push_back(x);
        changdu_y_nodes.push_back(mihanshu(x, zhishu));
        x += step_size; //转变为下一个节点的坐标x
    }

    for(int i=0;i<num_intervals;i+=1000){qDebug()<<"nodes"<<i<<changdu_x_nodes[i]<<changdu_y_nodes[i];}

    for(int i=0;i<num_intervals;i++)
    {
        x_dengchangdu[i]=changdu_x_nodes[i];
        y_dengchangdu[i]=changdu_y_nodes[i];
    }

    for(int i=0;i<num_intervals;i+=1000){qDebug()<<"xy"<<i<<x_dengchangdu[i]<<y_dengchangdu[i];}

    changdu_dengchang=num_intervals-1;
    double dengchang_max_error=0; //最大逼近误差
    for (int i = 1; i < changdu_dengchang - 1; i++)
    {
        double error = bijinwucha(changdu_x_nodes[i-1],changdu_x_nodes[i],zhishu);
        if (error > dengchang_max_error)
        {
            dengchang_max_error = error; //更新最大误差
        }
    }
    return dengchang_max_error;
}

//等误差每个节点坐标
double MIHANSHUXIAOSHU::dengwucha_zuobiao(double a, double b, double zhishu,double deng_error)
{
    dengwucha_x_nodes.push_back(a);
    double new_node;//第一个节点
    double init_step = 0.1;//设置初始步长
    double permit_error = 0;//允许误差
    double next_step;//下一个步长
    bool j = false;
    for (int i = 1; dengwucha_x_nodes[i]<b; i++)//取等误差x坐标数组里每个x值来循环
    {
        next_step = init_step;//初始步长
        new_node = dengwucha_x_nodes[i - 1];//new_node设置为上一个节点
        while (fabs(deng_error - permit_error)>0.000001)//判断等误差的误差
        {
            do
            {
                new_node = new_node + next_step;//更新节点位置
                qDebug() << "x1=" << new_node;
                if (new_node>b)
                {
                    j = true;
                    break;
                }
                permit_error = bijinwucha(dengwucha_x_nodes[i - 1], new_node,zhishu);//允许误差的更新
                qDebug() << "error=" << permit_error;//输出运行误差是多少在qt的调试台上
            }
            while (permit_error < deng_error);//直到满足允许误差小于等误差的误差这个条件才会停止do循环
            new_node = new_node - next_step;//更新节点位置
            next_step = next_step / 2;//更新下一步步长
            if (j == true)
            {
                break;
            }
        }
        permit_error = 0;
        dengwucha_x_nodes.push_back(new_node);//重置 permit_error 为 0，并将新节点 new_node 添加到 dengwucha_x_nodes 向量中
        if (j == true)
        {
            break;
        }
    }
    changdu_dengwucha = dengwucha_x_nodes.size();
    for (int i = 0; i<changdu_dengwucha; i++)
    {
        dengwucha_y_nodes.push_back(mihanshu(dengwucha_x_nodes[i], zhishu));
    }
    for (int i = 0; i<changdu_dengwucha; i++)
    {
        x_dengwucha[i] = dengwucha_x_nodes[i];
        y_dengwucha[i] = dengwucha_y_nodes[i];
    }
    // 输出节点的x和y值
    qDebug() << "节点坐标：";
    for (std::vector<double>::size_type i = 0; i < dengwucha_x_nodes.size(); ++i)
    {
        qDebug() << "x[" << i << "] = " << x_dengwucha[i] << ", " << "y[" << i << "] = " << y_dengwucha[i];
    }
    return changdu_dengwucha;
}

//计算右刀补坐标
void MIHANSHUXIAOSHU::compute_toolcompensateright(int chabu_type, double toolright,double zhishu)
{
    //插补方式为0是等间距逼近刀补，为1是等误差逼近刀补
    if(chabu_type==0)
    {
        x_toolcompensate[0]=x_dengchangdu[0];
        y_toolcompensate[0]=y_dengchangdu[0]-toolright;
        for (int i = 1; i<changdu_dengchang; i++)
        {
            x_toolcompensate[i] = x_dengchangdu[i] + toolright*cos(atan(1 /(zhishu*pow(x_dengchangdu[i], zhishu - 1))));//每个等间距的x坐标对应的刀补x坐标
            y_toolcompensate[i] = y_dengchangdu[i] - toolright*sin(atan(1 /(zhishu*pow(y_dengchangdu[i], zhishu - 1))));//每个等间距的y坐标对应的刀补y坐标
        }
        changdu_compensate = changdu_dengchang;
    }
    if(chabu_type==1)
    {
        x_toolcompensate[0]=x_dengwucha[0];
        y_toolcompensate[0]=y_dengwucha[0]-toolright;
        for (int i = 1; i<changdu_dengwucha; i++)
        {
            x_toolcompensate[i] = x_dengwucha[i] + toolright*cos(atan(1 / (zhishu*pow(x_dengwucha[i], zhishu - 1))));//每个等误差的x坐标对应的刀补x坐标
            y_toolcompensate[i] = y_dengwucha[i] - toolright*sin(atan(1 / (zhishu*pow(y_dengwucha[i], zhishu - 1))));//每个等误差的y坐标对应的刀补y坐标
        }
        changdu_compensate = changdu_dengwucha;
    }
}

//计算左刀补的x和y坐标
void MIHANSHUXIAOSHU::compute_toolcompensateleft(int chabu_type, double toolleft,double zhishu)
{
    //插补方式为0是等间距逼近刀补，为1是等误差逼近刀补
    if(chabu_type==0)
    {
        for (int i = 0; i<changdu_dengchang; i++)
        {
            x_toolcompensate[i] = x_dengchangdu[i] - toolleft*cos(atan(1 /(zhishu*pow(x_dengchangdu[i], zhishu - 1))));//每个等间距的x坐标对应的刀补x坐标
            y_toolcompensate[i] = y_dengchangdu[i] + toolleft*sin(atan(1 / (zhishu*pow(y_dengchangdu[i], zhishu - 1))));//每个等间距的y坐标对应的刀补y坐标
        }
        changdu_compensate = changdu_dengchang;
    }
    if(chabu_type==1)
    {
        for (int i = 0; i<changdu_dengwucha; i++)
        {
            x_toolcompensate[i] = x_dengwucha[i] - toolleft*cos(atan(1 /(zhishu*pow(x_dengwucha[i], zhishu - 1))));//每个等误差的x坐标对应的刀补x坐标
            y_toolcompensate[i] = y_dengwucha[i] + toolleft*sin(atan(1 /(zhishu*pow(y_dengwucha[i], zhishu - 1))));//每个等误差的y坐标对应的刀补y坐标
        }
        changdu_compensate = changdu_dengwucha;
    }
}

void MIHANSHUXIAOSHU::summit_error()
{
    for(int i=1;i<changdu_compensate;i++)
    {
        if(x_toolcompensate[i-1]>x_toolcompensate[i])
        {
            error_flag = 1 ;
            break;
        }
    }
}
