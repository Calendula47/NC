#include "yuxuanpamxing.h"
#include <QDebug>
#include <QtMath>
#include <QPainter>
#include <math.h>
#include<QString>
YUXUANPAMXING::YUXUANPAMXING(QWidget *parent) : QWidget{parent}
{

}
YUXUANPAMXING::~YUXUANPAMXING()
{

}

float maxError;

void YUXUANPAMXING::PaintTulun_dengjianju(QPainter *painter)
{
    for(int i=0;i<NCX.length()-1;i++){
        painter->drawLine(NCX[i], -NCY[i], NCX[i+1],-NCY[i+1]);
    }
}

void YUXUANPAMXING::PaintTulun_dengwucha(QPainter *painter)
{
    for(int i=0;i<NCX2.length()-1;i++){
        painter->drawLine(NCX2[i], -NCY2[i], NCX2[i+1],-NCY2[i+1]);
    }
}

void YUXUANPAMXING::PaintTulun_toolcompensate(QPainter *painter)
{
    for(int i=0;i<COMX.length()-1;i++)
    {
        painter->drawLine(COMX[i], -COMY[i], COMX[i+1],-COMY[i+1]);
    }
}

//余弦加减速凸轮等间距插补，计算各节点的x和y坐标，r是基圆，h是升程，o是升角，oo是回程角，step是步进角度，permit——eeror是允许误差，ooo是远休止角度，oooo是近休止角度
int YUXUANPAMXING::dengchang_zuobiao(float r,float h,float o,float oo,float ooo,float oooo,float permit_eeror,float step)
{
    float pi=3.14159;//Π
    float s=0;//推杆升程或回程的位移
    float x;//推杆升程或回程的位移x坐标
    float y;//推杆升程或回程的位移y坐标
    float error=0;
    float ex;
    float ey;
    float lastX=0;
    float lastY=0;
    float k;
    float b;
    float temp=1;
    NCX.clear();
    NCY.clear();
    maxError=0;
    for(float i =0;i<o;)//升程
    {
        while(1)
        {
            error=0;
            s=h*(1-cos(pi*i/o))/2;
            x=(cos(i*pi/180)*(r+s));
            y=(sin(i*pi/180)*(r+s));
            if(i!=0)
            {//
                for(int j=1;j<4;j++)//误差计算，循环三次计算进行评估误差
                {
                    ex=(cos((i-0.25*j*temp*step)*pi/180)*(r+s));
                    ey=(sin((i-0.25*j*temp*step)*pi/180)*(r+s));
                    k=(y-lastY)/(x-lastX);//斜率
                    b=y-k*x;//截距
                    if(fabs(k*ex-ey+b)/sqrt(k*k+1)>error)
                        error=fabs(k*ex-ey+b)/ sqrt(k*k+1);//更新误差
                }
            }
            if(error > permit_eeror)
            {
                temp*=0.5;//步长减半
                i-=temp*step;//更新角度
            }else
            {
                break;
            }
        }
        NCX.append(x);
        NCY.append(y);
        lastX=x;
        lastY=y;
        temp=1;//重置步长
        i+=step;
        if(error>maxError)
            maxError=error;//判断每一次的误差大小，获取最大误差

    }
    for(float i=0;i<ooo;)//远休止
    {
        while(1){
            error=0;
            x=(cos((i+o)*pi/180)*(r+h));
            y=(sin((i+o)*pi/180)*(r+h));
            if(i!=0)
            {
                for(int j=1;j<4;j++)
                {
                    ex=(cos(((i+o)-0.25*j*temp*step)*pi/180)*(r+h));
                    ey=(sin(((i+o)-0.25*j*temp*step)*pi/180)*(r+h));
                    k=(y-lastY)/(x-lastX);
                    b=y-k*x;
                    if(fabs(k*ex-ey+b)/sqrt(k*k+1)>error) error=fabs(k*ex-ey+b)/sqrt(k*k+1);
                }
            }
            if(error > permit_eeror)
            {
                temp*=0.5;
                i-=temp*step;
            }
            else
            {
                break;
            }
        }
        NCX.append(x);
        NCY.append(y);
        lastX=x;
        lastY=y;
        temp=1;//重置步长
        i+=step;
        if(error>maxError)
            maxError=error;//判断每一次的误差大小，获取最大误差
    }
    for(float i =0;i<oo;)//回程
    {//
        while(1){
            error=0;
            s=h*(1+cos(pi*i/oo))/2;
            x=(cos((i+o+ooo)*pi/180)*(r+s));
            y=(sin((i+o+ooo)*pi/180)*(r+s));
            if(i!=0)
            {
                for(int j=1;j<4;j++)
                {
                    ex=(cos(((i+o+ooo)-0.25*j*temp*step)*pi/180)*(r+s));
                    ey=(sin(((i+o+ooo)-0.25*j*temp*step)*pi/180)*(r+s));
                    k=(y-lastY)/(x-lastX);
                    b=y-k*x;
                    if(fabs(k*ex-ey+b)/sqrt(k*k+1)>error) error=fabs(k*ex-ey+b)/sqrt(k*k+1);
                }
            }
            if(error > permit_eeror)
            {
                temp*=0.5;
                i-=temp*step;
            }
            else
            {
                break;
            }
        }
        NCX.append(x);
        NCY.append(y);
        lastX=x;
        lastY=y;
        temp=1;//重置步长
        i+=step;
        if(error>maxError)
            maxError=error;//判断每一次的误差大小，获取最大误差
    }
    for(float i=0;i<oooo;)//近休止
    {
        while(1){
            error=0;
            x=(cos((i+o+oo+ooo)*pi/180)*(r));
            y=(sin((i+o+oo+ooo)*pi/180)*(r));
            if(i!=0)
            {
                for(int j=1;j<4;j++)
                {
                    ex=(cos(((i+o+oo+ooo)-0.25*j*temp*step)*pi/180)*(r));
                    ey=(sin(((i+o+oo+ooo)-0.25*j*temp*step)*pi/180)*(r));
                    k=(y-lastY)/(x-lastX);
                    b=y-k*x;
                    if(fabs(k*ex-ey+b)/sqrt(k*k+1)>error) error=fabs(k*ex-ey+b)/sqrt(k*k+1);
                }
            }
            if(error > permit_eeror)
            {
                temp*=0.5;
                i-=temp*step;
            }
            else
            {
                break;
            }
        }
        NCX.append(x);
        NCY.append(y);
        lastX=x;
        lastY=y;
        temp=1;//重置步长
        i+=step;
        if(error>maxError)
            maxError=error;//判断每一次的误差大小，获取最大误差
    }
    NCX.append(r);
    NCY.append(0);
    return NCX.length()-1;
}

//余弦加减速凸轮等误差插补，计算各节点的x和y坐标，r是基圆，h是升程，o是升角，oo是回程角，step是步进角度，permit——eeror是允许误差
int YUXUANPAMXING::dengwucha_zuobiao(float r,float h,float o,float oo,float ooo,float oooo,float permit_eeror,float step)
{
    float pi=3.14159;//Π
    float s=0;//推杆升程或回程的位移
    float x;//推杆升程或回程的位移x坐标
    float y;//推杆升程或回程的位移y坐标
    float error=0;
    float oa;
    float ob;
    float oi;
    float xa;
    float ya;
    float xb;
    float yb;
    float xi;
    float yi;
    float lastX=0;
    float lastY=0;
    float temp=1;
    double k;
    double b;
    NCX2.clear();
    NCY2.clear();
    NCX2.append(r);
    NCY2.append(0);
    for(float oa =0,oi = oa +0.005;(oa<o)&&(oi<o);oi+=0.005)//升程
    {
        s=h*(1-cos(pi*oa/o));
        xa=(cos(oa*pi/180)*(r+s));
        ya=(sin(oa*pi/180)*(r+s));
        s=h*(1-cos(pi*oi/o));
        xi=(cos(oi*pi/180)*(r+s));
        yi=(sin(oi*pi/180)*(r+s));
        k=-1/atan(oi);
        b=fabs(k*xa-ya-k*xi+yi)/sqrt(k*k+1);
        if(permit_eeror-b < 10e-4)
        {//满足误差需求
            ob=oi+(oi-oa);
            if(ob>o)
                ob=o;//防止越界
            s=h*(1-cos(pi*ob/o));
            xb=(cos(ob*pi/180)*(r+s));
            yb=(sin(ob*pi/180)*(r+s));
            NCX2.append(xb);
            NCY2.append(yb);
            oa=ob;
            oi=oa;
        }
    }
    for(float oa =0,oi = oa +0.005;(oa<ooo)&&(oi<ooo);oi+=0.005)//远休止
    {
        xa=(cos((oa+o)*pi/180)*(r+h));
        ya=(sin((oa+o)*pi/180)*(r+h));
        xi=(cos((oi+o)*pi/180)*(r+h));
        yi=(sin((oi+o)*pi/180)*(r+h));
        k=-1/atan(oi);
        b=fabs(k*xa-ya-k*xi+yi)/sqrt(k*k+1);
        if(permit_eeror-b < 10e-4)
        {//满足误差需求
            ob=oi+(oi-oa);
            if(ob>ooo)
                ob=ooo;//防止越界
            xb=(cos((ob+o)*pi/180)*(r+h));
            yb=(sin((ob+o)*pi/180)*(r+h));
            NCX2.append(xb);
            NCY2.append(yb);
            oa=ob;
            oi=oa;
        }
    }
    for(float oa =0,oi = oa +0.005;(oa<oo)&&(oi<oo);oi+=0.005)//回程
    {
        s=h*(1+cos(pi*oa/oo))/2;
        xa=(cos((oa+o+ooo)*pi/180)*(r+s));
        ya=(sin((oa+o+ooo)*pi/180)*(r+s));
        s=h*(1-cos(pi*oi/oo));
        xi=(cos((oi+o+ooo)*pi/180)*(r+s));
        yi=(sin((oi+o+ooo)*pi/180)*(r+s));
        k=-1/atan(oi);
        b=fabs(k*xa-ya-k*xi+yi)/sqrt(k*k+1);
        if(permit_eeror-b < 10e-4)
        {//满足误差需求
            ob=oi+(oi-oa);
            if(ob>o)
                ob=o;//防止越界
            s=h*(1+cos(pi*ob/oo))/2;
            xb=(cos((ob+o+ooo)*pi/180)*(r+s));
            yb=(sin((ob+o+ooo)*pi/180)*(r+s));
            NCX2.append(xb);
            NCY2.append(yb);
            oa=ob;
            oi=oa;
        }
    }
    for(float oa =0,oi = oa +0.005;(oa<oooo)&&(oi<oooo);oi+=0.005)//近休止
    {
        xa=(cos((oa+o+oo+ooo)*pi/180)*(r));
        ya=(sin((oa+o+oo+ooo)*pi/180)*(r));
        xi=(cos((oi+o+oo+ooo)*pi/180)*(r));
        yi=(sin((oi+o+oo+ooo)*pi/180)*(r));
        k=-1/atan(oi);
        b=fabs(k*xa-ya-k*xi+yi)/sqrt(k*k+1);
        if(permit_eeror-b < 10e-4)
        {//满足误差需求
            ob=oi+(oi-oa);
            if(ob>oooo)
                ob=oooo;//防止越界
            xb=(cos((ob+o+ooo+oo)*pi/180)*(r));
            yb=(sin((ob+o+oo+ooo)*pi/180)*(r));
            NCX2.append(xb);
            NCY2.append(yb);
            oa=ob;
            oi=oa;
        }
    }
    NCX2.append(r);
    NCY2.append(0);
    return NCX2.length()-1;
}

/*凸轮等间距左刀补 将点集存储到COMXCOMY中输入参数:tool刀具半径;返回值:1没有过切-1产生过切*/
int YUXUANPAMXING:: dengchang_compute_toolcompensateleft(float tool)
{
    float k1;
    float k2;
    float xr1;
    float yr1;
    float xr2;
    float yr2;
    float toolx;
    float tooly;
    float k;
    float b;
    float u1;
    float v1;
    float u2;
    float v2;
    int cmd;
    COMX.clear();
    COMY.clear();
    for(int i=1;i<NCX.length()-1;i++)
    {
        u1=NCX[i]-NCX[i-1];
        v1=NCY[i]-NCY[i-1];
        u2=NCX[i+1]-NCX[i];
        v2=NCY[i+1]-NCY[i];
        if((u1*v2-v1*u2)<=0)
        {//<180
            cmd=1;
        }
        else if((u1*v2-v1*u2)>0&&(u1*u2+v1*v2)>=0)
        {//180-270
            cmd=2;
        }
        else
        {//270-360
            cmd = 3;
        }
        k1=((NCY[i]-NCY[i-1])/(NCX[i]-NCX[i-1]));//0A段
        k2=((NCY[i+1]-NCY[i])/(NCX[i+1]-NCX[i]));//AB段
        xr1=cos(atan(-1/k1))*tool;
        yr1=sin(atan(-1/k1))*tool;
        xr2=-cos(atan(-1/k2))*tool;
        yr2=-sin(atan(-1/k2))*tool;
        if(cmd==1)
        {
            toolx=NCX[i]+xr2;
            tooly=NCY[i]+yr2;
            if((NCX[i]*NCX[i]+NCY[i]*NCY[i])<(toolx*toolx+tooly*tooly))//防止点越界
            {
                toolx=NCX[i]-xr2;
                tooly=NCY[i]-yr2;
            }
            COMX.append(toolx);
            COMY.append(tooly);
        }
    }
    for(int j=5;j<COMX.length();j++)
    {
        if((COMX.at(j)>0)&&(COMY.at(j-5)-COMY.at(j))>0.02)
        {
            return -1;
        }
    }
     return 1;
}

/*凸轮等间距右刀补 将点集存储到COMXCOMY中输入参数:tool刀具半径返回值:1没有过切-1产生过切*/
int YUXUANPAMXING::dengchang_compute_toolcompensateright(float tool)
{
    float k1;
    float k2;
    float xr1;
    float yr1;
    float xr2;
    float yr2;
    float toolx;
    float tooly;
    float k;
    float b;
    float u1;
    float v1;
    float u2;
    float v2;
    int cmd;
    COMX.clear();
    COMY.clear();
    for(int i=1;i<NCX.length()-1;i++)
    {
        u1=NCX[i]-NCX[i-1];
        v1=NCY[i]-NCY[i-1];
        u2=NCX[i+1]-NCX[i];
        v2=NCY[i+1]-NCY[i];
        if((u1*v2-v1*u2)<=0){
            //<180
            cmd=1;
        }
        else if((u1*v2-v1*u2)>0&&(u1*u2+v1*v2)>=0)
        {//180-270
            cmd=2;
        }
        else
        {//270-360
            cmd=3;
        }
        k1=((NCY[i]-NCY[i-1])/(NCX[i]-NCX[i-1]));//0A段
        k2=((NCY[i+1]-NCY[i])/(NCX[i+1]-NCX[i]));//AB段
        xr1=cos(atan(-1/k1))*tool;
        yr1=sin(atan(-1/k1))*tool;
        xr2=cos(atan(-1/k2))*tool;
        yr2=sin(atan(-1/k2))*tool;
        if(cmd==2)
        {
            toolx=NCX2[i]+xr2;
            tooly=NCY2[i]+yr2;
            if((NCX2[i]*NCX2[i]+NCY2[i]*NCY2[i])>(toolx*toolx+tooly*tooly))//防止越界
            {
                toolx=NCX2[i]-xr2;
                tooly=NCY2[i]-yr2;
            }
                COMX.append(toolx);
                COMY.append(tooly);
        }
    }
    if(COMY.at(0)>=0&&COMY.at(this->COMX.length()-1)<=0)//判读是否过切
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

/*凸轮等误差左刀补 将点集存储到COMXCOMY中输入参数:tool刀具半径返回值:1没有过切 -1产生过切*/
int YUXUANPAMXING::dengwucha_compute_toolcompensateleft(float tool)
{
    float k1;
    float k2;
    float xr1;
    float yr1;
    float xr2;
    float yr2;
    float toolx;
    float tooly;
    float k;
    float b;
    float u1;
    float v1;
    float u2;
    float v2;
    int cmd;//矢量夹角
    COMX.clear();
    COMY.clear();
    for(int i=1;i<NCX2.length()-1;i++)
    {
        //计算矢量夹角的大小
        u1=NCX2[i]-NCX2[i-1];
        v1=NCY2[i]-NCY2[i-1];
        u1=NCX2[i+1]-NCX2[i];
        v2=NCY2[i+1]-NCY2[i];
        if((u1*v2-v1*u2)<=0)
        {//矢量夹角<180
            cmd=1;
        }
        else if((u1*v2-v1*u2)>0&&(u1*u2+v1*v2)>=0)
        {//矢量夹角：180-270
            cmd=2;
        }
        else
        {//矢量夹角：270-360
            cmd=3;
        }
        k1=((NCY2[i]-NCY2[i-1])/(NCX2[i]-NCX2[i-1]));//0A段
        k2=((NCY2[i+1]-NCY2[i])/(NCX2[i+1]-NCX2[i]));//AB段
        xr1=cos(atan(-1/k1))*tool;
        yr1=sin(atan(-1/k1))*tool;
        xr2=-cos(atan(-1/k2))*tool;
        yr2=-sin(atan(-1/k2))*tool;
        if(cmd==1)
        {
            toolx=NCX2[i]+xr2;
            tooly=NCY2[i]+yr2;
            if((NCX2[i]*NCX2[i]+NCY2[i]*NCY2[i])<(toolx*toolx+tooly*tooly))//防止点越界
            {
                toolx=NCX2[i]-xr2;
                tooly=NCY2[i]-yr2;
            }
            COMX.append(toolx);
            COMY.append(tooly);
        }
    }
    for(int j=5;j<COMX.length();j++)
     {
         if((COMX.at(j)>0)&&(COMY.at(j-5)-COMY.at(j))>0.02)//判断是否过切
         {
             return -1;
         }
     }
    return 1;
}

/*凸轮等误差右刀补 将点集存储到COMXCOMY中输入参数:tool刀具半径返回值:1没有过切 -1产生过切*/
int YUXUANPAMXING::dengwucha_compute_toolcompensateright(float tool)
{
    float k1;
    float k2;
    float xr1;
    float yr1;
    float xr2;
    float yr2;
    float toolx;
    float tooly;
    float k;
    float b;
    float u1;
    float v1;
    float u2;
    float v2;
    int cmd;//矢量夹角
    COMX.clear();
    COMY.clear();
    for(int i=1;i<NCX2.length()-1;i++)
    {
        //计算矢量夹角的大小
        u1=NCX2[i]-NCX2[i-1];
        v1=NCY2[i]-NCY2[i-1];
        u1=NCX2[i+1]-NCX2[i];
        v2=NCY2[i+1]-NCY2[i];
        if((u1*v2-v1*u2)<=0)
        {//矢量夹角<180
            cmd=1;
        }
        else if((u1*v2-v1*u2)>0&&(u1*u2+v1*v2)>=0)
        {//矢量夹角：180-270
            cmd=2;
        }
        else
        {//矢量夹角：270-360
            cmd=3;
        }
        k1=((NCY2[i]-NCY2[i-1])/(NCX2[i]-NCX2[i-1]));//0A段
        k2=((NCY2[i+1]-NCY2[i])/(NCX2[i+1]-NCX2[i]));//AB段
        xr1=cos(atan(-1/k1))*tool;
        yr1=sin(atan(-1/k1))*tool;
        xr2=-cos(atan(-1/k2))*tool;
        yr2=-sin(atan(-1/k2))*tool;
        if(cmd==1)
        {
            toolx=NCX2[i]+xr2;
            tooly=NCY2[i]+yr2;
            if((NCX2[i]*NCX2[i]+NCY2[i]*NCY2[i])<(toolx*toolx+tooly*tooly))//防止点越界
            {
                toolx=NCX2[i]-xr2;
                tooly=NCY2[i]-yr2;
            }
            COMX.append(toolx);
            COMY.append(tooly);
        }
    }
    if(COMY.at(0)>=0&&COMY.at(this->COMX.length()-1)<=0)//判断是否过切
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
