#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mihanshuxiaoshu.h"
#include "yuxuanpamxing.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("数控技术课程设计2024");
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(jiShiQi()));

    ui->xuanzequxian->setCurrentIndex(0);
    ui->x_qidian->setRange(0,200);
    ui->x_zhongdian->setRange(0,200);
    ui->mishu->setRange(0,1);
    ui->mishu->setDecimals(3);
    ui->mishu->setSingleStep(0.125);
    ui->shengchengjiao->setRange(0,360);
    ui->shengchengjiao->setDecimals(1);
    ui->shengchengjiao->setSingleStep(10);
    ui->huichengjiao->setRange(0,360);
    ui->huichengjiao->setDecimals(1);
    ui->huichengjiao->setSingleStep(10);
    ui->jinxiujiao->setRange(0,360);
    ui->jinxiujiao->setDecimals(1);
    ui->jinxiujiao->setSingleStep(10);
    ui->yuanxiujiao->setRange(0,360);
    ui->yuanxiujiao->setDecimals(1);
    ui->yuanxiujiao->setSingleStep(10);
    ui->zhuzhouzhuansu->setRange(0,30000);
    ui->zhuzhouzhuansu->setSingleStep(100);
    ui->jinjisudu->setRange(0,1000);
    ui->jinjisudu->setSingleStep(10);
    ui->daojubanjing->setSingleStep(0.25);
    ui->daojubanjing->setDecimals(2);
    ui->houdu->setDecimals(1);
    ui->houdu->setSingleStep(0.5);
    ui->zuidawucha->setDecimals(3);
    ui->zuidawucha->setSingleStep(0.001);
    ui->gudingwucha->setDecimals(3);
    ui->gudingwucha->setSingleStep(0.001);
    ui->wuchashezhi->setVisible(false);
    ui->dengjianju->setChecked(false);
    ui->dengwucha->setChecked(false);
    ui->bijinfangfa->setVisible(false);
    ui->progressBar->reset();
    ui->quxianduanshuxianshi->setText("未计算，请选择逼近方法");
    ui->zuidawuchaxianshi->setText("未计算，请选择逼近方法");
    ui->x_qidian->setValue(0);
    ui->x_zhongdian->setValue(20);
    ui->mishu->setValue(0.8);
    ui->jiyuanzhijing->setValue(10);
    ui->shengcheng->setValue(10);
    ui->shengchengjiao->setValue(60);
    ui->huichengjiao->setValue(90);
    ui->jinxiujiao->setValue(110);
    ui->yuanxiujiao->setValue(100);
    ui->zhuzhouzhuansu->setValue(24000);
    ui->jinjisudu->setValue(120);
    ui->daojubanjing->setValue(2);
    ui->tuidaogaodu->setValue(10);
    ui->houdu->setValue(3);
    ui->gudingwucha->setValue(0.002);
    ui->zuidawucha->setValue(0.002);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)//创建绘图窗口及坐标轴
{
    QPainter painter(this);
    paintX = ui->widget_2->x();
    paintY = ui->widget_2->y();
    painter.drawImage(paintX,paintY,image);
    paintWidth = ui->paint_widget->width();
    paintHeight = ui->paint_widget->height();
    image = QImage(paintWidth,paintHeight,QImage::Format_RGB32);
    image.fill(qRgb(255,255,255));
    painter.setPen(QPen(Qt::black,2));
    painter.drawLine(paintX,paintHeight/2,paintX+paintWidth,paintHeight/2);
    painter.drawLine(paintX+paintWidth/2,paintY,paintX+paintWidth/2,paintY+paintHeight);
    for(int i=0;i<(paintWidth/2);i+=10*paintScale)
    {
        painter.drawLine(paintX+(paintWidth/2)+i,(paintHeight/2)-2,paintX+(paintWidth/2)+i,(paintHeight/2)+2);
        painter.drawLine(paintX+(paintWidth/2)-i,(paintHeight/2)-2,paintX+(paintWidth/2)-i,(paintHeight/2)+2);
    }
    for(int i=0;i<(paintHeight/2)-10;i+=10*paintScale)
    {
        painter.drawLine(paintX+(paintWidth/2)-2,(paintHeight/2)+i,paintX+(paintWidth/2)+2,(paintHeight/2)+i);
        painter.drawLine(paintX+(paintWidth/2)-2,(paintHeight/2)-i,paintX+(paintWidth/2)+2,(paintHeight/2)-i);
    }
    paint();
}

void MainWindow::paint()//主绘图函数
{
    if(shengchengyuanlujing==1)//生成原图像
    {
        QPainter mainpainter(&image);
        mainpainter.fillRect(rect(),Qt::transparent);
        mainpainter.setRenderHint(QPainter::Antialiasing, true);
        mainpainter.drawRect(0,0,paintWidth,paintHeight);
        mainpainter.translate((ui->paint_widget->width()/2),(-9+ui->paint_widget->height()/2));
        mainpainter.scale(paintScale,-paintScale);
        mainpainter.setPen(QPen(Qt::blue,1,Qt::SolidLine,Qt::RoundCap));
        if(ui->xuanzequxian->currentIndex()==0)//幂函数
        {
            if(ui->dengjianju->isChecked())//等间距
            {
                for(int j=1;j<changdu_dengchang;j++)
                {
                    mainpainter.drawLine(10*x_dengchangdu[j-1],10*y_dengchangdu[j-1],10*x_dengchangdu[j],10*y_dengchangdu[j]);
                }
            }

            if(ui->dengwucha->isChecked())//等误差
            {
                for(int j=1;j<changdu_dengwucha;j++)
                {
                    mainpainter.drawLine(10*x_dengwucha[j-1],10*y_dengwucha[j-1],10*x_dengwucha[j],10*y_dengwucha[j]);
                }
            }
        }

        if(ui->xuanzequxian->currentIndex()==1)//凸轮
        {
            if(ui->dengjianju->isChecked())//等间距
            {
                for(int i=0;i<NCX.length()-1;i++)
                {
                    mainpainter.drawLine(10*NCX[i], -10*NCY[i], 10*NCX[i+1],-10*NCY[i+1]);
                }
            }

            if(ui->dengwucha->isChecked())//等误差
            {
                for(int i=0;i<NCX2.length()-1;i++)
                {
                    mainpainter.drawLine(10*NCX2[i], -10*NCY2[i], 10*NCX2[i+1],-10*NCY2[i+1]);
                }
            }
        }
    }

    if(shengchengdaojulujing==1)//生成刀具路径
    {
        QPainter toolpainter(&image);
        toolpainter.fillRect(rect(),Qt::transparent);
        toolpainter.setRenderHint(QPainter::Antialiasing, true);
        toolpainter.drawRect(0,0,paintWidth,paintHeight);
        toolpainter.translate((ui->paint_widget->width()/2),(-9+ui->paint_widget->height()/2));
        toolpainter.scale(paintScale,-paintScale);
        toolpainter.setPen(QPen(Qt::green,1,Qt::SolidLine,Qt::RoundCap));
        if(ui->xuanzequxian->currentIndex()==0)//幂函数
        {            
            for(int j=1;j<changdu_compensate;j++)
            {
                toolpainter.drawLine(10*x_toolcompensate[j-1],10*y_toolcompensate[j-1],10*x_toolcompensate[j],10*y_toolcompensate[j]);
            }
        }

        if(ui->xuanzequxian->currentIndex()==1)//凸轮
        {
            for(int i=0;i<COMX.length()-1;i++)
            {
                toolpainter.drawLine(10*COMX[i], -10*COMY[i], 10*COMX[i+1],-10*COMY[i+1]);
            }
        }
    }

    if(kaishifangzhen==1)//仿真加工
    {
        QPainter compensatepainter(&image);
        compensatepainter.fillRect(rect(),Qt::transparent);
        compensatepainter.setRenderHint(QPainter::Antialiasing, true);
        compensatepainter.drawRect(0,0,paintWidth,paintHeight);
        compensatepainter.translate((ui->paint_widget->width()/2),(-9+ui->paint_widget->height()/2));
        compensatepainter.scale(paintScale,-paintScale);
        compensatepainter.setPen(QPen(QColor(255,200,200),1));

        float daojubanjing=ui->daojubanjing->value();
        if(ui->xuanzequxian->currentIndex()==0)//幂函数
        {
            for(int i=1;i<time;i+=60)
            {
                QPointF circleCenter(10*x_toolcompensate[i],10*y_toolcompensate[i]);
                compensatepainter.drawEllipse(circleCenter,10*daojubanjing,10*daojubanjing);
            }
        }
        else//凸轮
        {
            for(int i=1;i<time;i+=60)
            {
                QPointF circleCenter(10*COMX[i],-10*COMY[i]);
                compensatepainter.drawEllipse(circleCenter,10*daojubanjing,10*daojubanjing);
            }
        }
        update();
    }
    update();
}

void MainWindow::on_shengchenglujing_clicked()//点击生成原曲线
{
    shengchengdaojulujing=0;
    kaishifangzhen=0;
    if(ui->dengjianju->isChecked()==0 && ui->dengwucha->isChecked()==0)//因为逼近方法没有改变事件的话下面的段数和最大误差不会更新，所以创建了一个空RadioButton用来保证下面的统计有数值
    {
        QMessageBox::warning(this,"未选择逼近方法","请在“插补参数”中选择逼近方法");
    }
    else
    {
        if(ui->xuanzequxian->currentIndex()==1 && ui->shengchengjiao->value()+ui->huichengjiao->value()+ui->jinxiujiao->value()+ui->yuanxiujiao->value()!=360)
        {
            QMessageBox::warning(this,"参数错误","请保证角度之和为360°");
        }
        else if(ui->xuanzequxian->currentIndex()==0 && ui->mishu->value()<=0 || ui->mishu->value()>=1)
        {
            QMessageBox::warning(this,"参数错误","请保证幂数在0到1之间");
        }
        else
        {
            if(ui->dengjianju->isChecked())
            {
                if(ui->xuanzequxian->currentIndex()==0)
                {
                    ui->zuidawuchaxianshi->setText(QString::number(MIHANSHUXIAOSHU().dengchang_zuobiao(ui->x_qidian->value(),ui->x_zhongdian->value(),ui->mishu->value(),ui->zuidawucha->value())));
                    ui->quxianduanshuxianshi->setText(QString::number(changdu_dengchang));
                }
                else
                {
                    ui->quxianduanshuxianshi->setText(QString::number(YUXUANPAMXING().dengchang_zuobiao(ui->jiyuanzhijing->value(),ui->shengcheng->value(),ui->shengchengjiao->value(),ui->huichengjiao->value(),ui->yuanxiujiao->value(),ui->jinxiujiao->value(),ui->zuidawucha->value(),1)));
                    ui->zuidawuchaxianshi->setText(QString::number(maxError));

                }
            }

            if(ui->dengwucha->isChecked())
            {
                if(ui->xuanzequxian->currentIndex()==0)
                {
                    ui->zuidawuchaxianshi->setText(QString::number(ui->gudingwucha->value()));
                    ui->quxianduanshuxianshi->setText(QString::number(MIHANSHUXIAOSHU().dengwucha_zuobiao(ui->x_qidian->value(),ui->x_zhongdian->value(),ui->mishu->value(),ui->gudingwucha->value())));
                }
                else
                {
                    ui->zuidawuchaxianshi->setText(QString::number(ui->gudingwucha->value()));
                    ui->quxianduanshuxianshi->setText(QString::number(YUXUANPAMXING().dengwucha_zuobiao(ui->jiyuanzhijing->value(),ui->shengcheng->value(),ui->shengchengjiao->value(),ui->huichengjiao->value(),ui->yuanxiujiao->value(),ui->jinxiujiao->value(),ui->gudingwucha->value(),1)));
                }
            }

            shengchengyuanlujing=1;
        }
    }
}

void MainWindow::on_shengchengdaojulujing_clicked()//点击生成刀具路径
{
    kaishifangzhen=0;
    if(ui->xuanzequxian->currentIndex()==1)//凸轮
    {
        if(ui->dengjianju->isChecked())
        {
            if(ui->zuodaobu->isChecked())
            {
                if(YUXUANPAMXING().dengchang_compute_toolcompensateleft(ui->daojubanjing->value())==1)
                {
                    shengchengdaojulujing=1;
                }
                if(YUXUANPAMXING().dengchang_compute_toolcompensateleft(ui->daojubanjing->value())==-1)
                {
                    QMessageBox::warning(this,"过切警告","请更改刀具半径或检查刀补方向");
                }
            }
            if(ui->youdaobu->isChecked())
            {
                if(YUXUANPAMXING().dengchang_compute_toolcompensateright(ui->daojubanjing->value())==1)
                {
                    shengchengdaojulujing=1;
                }
                if(YUXUANPAMXING().dengchang_compute_toolcompensateright(ui->daojubanjing->value())==-1)
                {
                    QMessageBox::warning(this,"过切警告","请更改刀具半径或检查刀补方向");
                }
            }
        }
        if(ui->dengwucha->isChecked())
        {
            if(ui->zuodaobu->isChecked())
            {
                if(YUXUANPAMXING().dengwucha_compute_toolcompensateleft(ui->daojubanjing->value())==1)
                {
                    shengchengdaojulujing=1;
                }
                if(YUXUANPAMXING().dengwucha_compute_toolcompensateleft(ui->daojubanjing->value())==-1)
                {
                    QMessageBox::warning(this,"过切警告","请更改刀具半径或检查刀补方向");
                }
            }
            if(ui->youdaobu->isChecked())
            {
                if(YUXUANPAMXING().dengwucha_compute_toolcompensateright(ui->daojubanjing->value())==1)
                {
                    shengchengdaojulujing=1;
                }
                if(YUXUANPAMXING().dengwucha_compute_toolcompensateright(ui->daojubanjing->value())==-1)
                {
                    QMessageBox::warning(this,"过切警告","请更改刀具半径或检查刀补方向");
                }
            }
        }
    }

    if(ui->xuanzequxian->currentIndex()==0)//幂函数
    {
        if(ui->zuodaobu->isChecked())
        {
            MIHANSHUXIAOSHU().compute_toolcompensateleft(ui->dengwucha->isChecked(),ui->daojubanjing->value(),ui->mishu->value());
        }
        if(ui->youdaobu->isChecked())
        {
            MIHANSHUXIAOSHU().compute_toolcompensateright(ui->dengwucha->isChecked(),ui->daojubanjing->value(),ui->mishu->value());
        }

        shengchengdaojulujing=1;
    }
}

void MainWindow::on_jinxingfangzhen_clicked()//点击开始仿真
{
    if(ui->bijinfangfa->isChecked())
    {
        QMessageBox::warning(this,"未选择逼近方法","请在“插补参数”中选择逼近方法");
    }
    else
    {
        if(ui->jinjisudu->value()==0)
        {
            QMessageBox::warning(this,"参数错误","请在“机床参数”中填入进给速度");
        }
        else
        {
            if(ui->daojubanjing->value()==0)
            {
                QMessageBox::warning(this,"参数错误","请在“插补参数”中填入刀具半径");
            }
            else
            {
                kaishifangzhen=1;
                timer->start(10000/ui->jinjisudu->value());
                update();
            }
        }
    }
}

void MainWindow::on_shengchengNC_clicked()//点击生成NC代码
{
    kaishifangzhen=0;
    if(ui->tuidaogaodu->value()<ui->houdu->value())
    {
        QMessageBox::warning(this,"参数错误","请保证退刀高度大于工件厚度");
    }
    else if(ui->bijinfangfa->isChecked())
    {
        QMessageBox::warning(this,"未选择逼近方法","请在“插补参数”中选择逼近方法");
    }
    else if(ui->houdu->value()==0)
    {
        QMessageBox::warning(this,"参数错误","请在“机床参数”中填入工件的加工厚度");
    }
    else if(ui->zhuzhouzhuansu->value()==0)
    {
        QMessageBox::warning(this,"参数错误","请在“机床参数”中填入主轴转速");
    }
    else if(ui->jinjisudu->value()==0)
    {
        QMessageBox::warning(this,"参数错误","请在“机床参数”中填入进给速度");
    }
    else
    {
        if(ui->jueduizuobiao->isChecked())//绝对坐标
        {
            float x = ui->daojuX->value();//刀具原点x坐标
            float y = ui->daojuY->value();//刀具原点y坐标
            float z = ui->daojuZ->value();//刀具原点z坐标
            float s = ui->zhuzhouzhuansu->value();//转速
            float f = ui->jinjisudu->value();//进给速度
            int choose;//曲线类型
            int way;//逼近方法

            if (ui->xuanzequxian->currentIndex() == 0)//判断曲线类型
            {choose = 0;}//幂函数
            else
            {choose = 1;}//凸轮
            if (ui->dengjianju->isChecked() == 1)//判断逼近方法
            {way = 1;}//等间距
            else
            {way = 2;}//等误差

            int rowNum = 0;//进行步数
            QString str = "%0001";//程序标题号
            ui->gCode->clear();
            ui->gCode->appendPlainText(str);

            rowNum++;//第一行 设定参数
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G90 G92");
            str.append(" X");
            str.append(QString::number(x));
            str.append(" Y");
            str.append(QString::number(y));
            str.append(" Z");
            str.append(QString::number(z));
            str.append(" S");
            str.append(QString::number(s));
            str.append(" F");
            str.append(QString::number(f));

            if (ui->zhuzhoushun->isChecked())//判断正反转
            {str.append(" M03");}//主轴正转
            else if (ui->zhuzhouni->isChecked())
            {str.append(" M04");}//主轴反转

            str.append(" M08");//冷却液
            ui->gCode->appendPlainText(str);

            rowNum++;//第二行 退刀
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G00 Z");
            str.append(QString::number(ui->tuidaogaodu->value()));//刀具先升到退刀高度
            ui->gCode->appendPlainText(str);

            rowNum++;//第三行 移动到下刀点上方
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G17 G00");//选定xy面
            if (ui->zuodaobu->isChecked())//判断左右刀补
            {str.append(" G41");}
            else if (ui->youdaobu->isChecked())
            {str.append(" G42");}

            if (choose == 1)//判断曲线起点
            {
                if (way == 1)//判断凸轮的刀补方式
                {
                    str.append(" X");
                    str.append(QString::number(NCX[0]));
                    str.append(" Y");
                    str.append(QString::number(NCY[0]));
                }
                else
                {
                    str.append(" X");
                    str.append(QString::number(NCX2[0]));
                    str.append(" Y");
                    str.append(QString::number(NCY2[0]));
                }
            }
            else
            {
                if (way==1)
                {
                    str.append(" X");
                    str.append(QString::number(x_dengchangdu[0]));
                    str.append(" Y");
                    str.append(QString::number(y_dengchangdu[0]));
                }
                else
                {
                    str.append(" X");
                    str.append(QString::number(x_dengwucha[0]));
                    str.append(" Y");
                    str.append(QString::number(y_dengwucha[0]));
                }
            }

            str.append(" D01");//刀号
            ui->gCode->appendPlainText(str);

            rowNum++;//第四行 下刀
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G01");
            str.append(" Z0");
            ui->gCode->appendPlainText(str);

            //中间代码
            if (choose == 1)
            {
                if (way == 1)
                {
                    for (int i = 1; i<NCX.length() - 0; i++)
                    {
                        rowNum++;//程序步数增加
                        str = "N" + QString::number(rowNum / 1000) +QString::number((rowNum % 1000) / 100) + QString::number((rowNum % 100) / 10) +QString::number((rowNum % 10) % 10);
                        str.append(" X");
                        str.append(QString::number(NCX[i]));
                        str.append(" Y");
                        str.append(QString::number(NCY[i]));
                        ui->gCode->appendPlainText(str);
                    }
                }
                else
                {
                    for (int i = 1; i<NCX2.length() - 0; i++)
                    {
                        rowNum++;
                        str = "N" + QString::number(rowNum / 1000) +QString::number((rowNum % 1000) / 100) + QString::number((rowNum % 100) / 10) +QString::number((rowNum % 10) % 10);
                        str.append(" X");
                        str.append(QString::number(NCX2[i]));
                        str.append(" Y");
                        str.append(QString::number(NCY2[i]));
                        ui->gCode->appendPlainText(str);
                    }
                }
            }
            else
            {
                if(way==1)
                {
                    for (int i = 1; i<changdu_dengchang; i++)
                    {
                        rowNum++;
                        str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum% 1000) / 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum% 10) % 10);
                        str.append(" X");
                        str.append(QString::number(x_dengchangdu[i]));
                        str.append(" Y");
                        str.append(QString::number(y_dengchangdu[i]));
                        ui->gCode->appendPlainText(str);
                    }
                }
                else
                {
                    for (int i = 1; i<changdu_dengwucha; i++)
                    {
                        rowNum++;
                        str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum% 1000) / 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum% 10) % 10);
                        str.append(" X");
                        str.append(QString::number(x_dengwucha[i]));
                        str.append(" Y");
                        str.append(QString::number(y_dengwucha[i]));
                        ui->gCode->appendPlainText(str);
                    }
                }
            }

            rowNum++;//倒数第三行 退刀
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G00");
            str.append(" Z");
            str.append(QString::number(ui->tuidaogaodu->value()));
            ui->gCode->appendPlainText(str);

            rowNum++;//倒数第二行 回原点 关附属设备
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G40 G00");//取消刀补 刀具回原位
            str.append(" X");
            str.append(QString::number(x));
            str.append(" Y");
            str.append(QString::number(y));
            str.append(" M05 M09");//关主轴 冷却液
            ui->gCode->appendPlainText(str);

            rowNum++;//最后一行 程序结束
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" M02");
            ui->gCode->appendPlainText(str);
        }

        if(ui->xiangduizuobiao->isChecked())//相对坐标
        {
            float x = ui->daojuX->value();//刀具原点x坐标
            float y = ui->daojuY->value();//刀具原点y坐标
            float z = ui->daojuZ->value();//刀具原点z坐标
            float s = ui->zhuzhouzhuansu->value();//转速
            float f = ui->jinjisudu->value();//进给速度
            float realx = 0;
            float realy = 0;
            int choose;//曲线类型
            int way;//逼近方法

            if (ui->xuanzequxian->currentIndex() == 0)//判断曲线类型
            {choose = 0;}//幂函数
            else
            {choose = 1;}//凸轮

            if (ui->dengjianju->isChecked() == 1)//判断逼近方法
            {way = 1;}//等间距
            else
            {way = 2;}//等误差

            int rowNum = 0;//进行步数
            QString str = "%0001";//程序标题号
            ui->gCode->clear();
            ui->gCode->appendPlainText(str);

            rowNum++;//第一行 设定参数
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G91");
            str.append(" S");
            str.append(QString::number(s));
            str.append(" F");
            str.append(QString::number(f));

            if (ui->zhuzhoushun->isChecked())//判断正反转
            {str.append(" M03");}//主轴正转
            else if (ui->zhuzhouni->isChecked())
            {str.append(" M04");}//主轴反转

            str.append(" M08");//冷却液
            ui->gCode->appendPlainText(str);

            rowNum++;//第二行 退刀
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G00 Z");
            str.append(QString::number(ui->tuidaogaodu->value() - z));//刀具先升到退刀高度
            ui->gCode->appendPlainText(str);

            rowNum++;//第三行 移动到下刀点上方
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G17 G00");//选定xy面
            if (ui->zuodaobu->isChecked())//判断左右刀补
            {str.append(" G41");}
            else if (ui->youdaobu->isChecked())
            {str.append(" G42");}

            if (choose == 1)//判断曲线起点
            {
                if (way == 1)//判断凸轮的刀补方式
                {
                    realx=NCX[0]-x;
                    realy=NCY[0]-y;
                    str.append(" X");
                    str.append(QString::number(realx));
                    str.append(" Y");
                    str.append(QString::number(realy));
                }
                else
                {
                    realx=NCX2[0]-x;
                    realy=NCY2[0]-y;
                    str.append(" X");
                    str.append(QString::number(realx));
                    str.append(" Y");
                    str.append(QString::number(realy));
                }
            }
            else
            {
                if (way==1)
                {
                    realx=x_dengchangdu[0]-x;
                    realy=y_dengchangdu[0]-y;
                    str.append(" X");
                    str.append(QString::number(realx));
                    str.append(" Y");
                    str.append(QString::number(realy));
                }
                else
                {
                    realx=x_dengwucha[0]-x;
                    realy=y_dengwucha[0]-y;
                    str.append(" X");
                    str.append(QString::number(realx));
                    str.append(" Y");
                    str.append(QString::number(realy));
                }
            }

            str.append(" D01");//刀号
            ui->gCode->appendPlainText(str);

            rowNum++;//第四行 下刀
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G01");
            str.append(" Z-");
            str.append(QString::number(ui->tuidaogaodu->value()));
            ui->gCode->appendPlainText(str);

            //中间代码
            if (choose == 1)
            {
                if (way == 1)
                {
                    for (int i = 1; i<NCX.length() - 0; i++)
                    {
                        rowNum++;//程序步数增加
                        str = "N" + QString::number(rowNum / 1000) +QString::number((rowNum % 1000) / 100) + QString::number((rowNum % 100) / 10) +QString::number((rowNum % 10) % 10);
                        realx = fabs(NCX[i] - NCX[i - 1]);//相对坐标x
                        realy = fabs(NCY[i] - NCY[i - 1]);//相对坐标y

                        if (realx<0.000001)//四舍五入
                            {realx = 0.000001;}
                        else
                            {realx = round(realx * pow(10, 6)) * pow(10, -6);}
                        if (realy<0.000001)
                            {realy = 0.000001;}
                        else
                            {realy = round(realy * pow(10, 6)) * pow(10, -6);}

                        str.append(" X");
                        if (NCX[i]<NCX[i - 1])//判断正负
                            {str.append("-");}
                        str.append(QString("%1").arg(realx, 0, 'q', 6));
                        str.append(" Y");
                        if (NCY[i]<NCY[i - 1])//判断正负
                            {str.append("-");}
                        str.append(QString("%1").arg(realy, 0, 'q', 6));

                        ui->gCode->appendPlainText(str);
                    }
                }
                else
                {
                    for (int i = 1; i<NCX2.length() - 0; i++)
                    {
                        rowNum++;
                        str = "N" + QString::number(rowNum / 1000) +QString::number((rowNum % 1000) / 100) + QString::number((rowNum % 100) / 10) +QString::number((rowNum % 10) % 10);
                        realx = fabs(NCX2[i] - NCX2[i - 1]);//相对坐标x
                        realy = fabs(NCY2[i] - NCY2[i - 1]);//相对坐标y

                        if (realx<0.000001)//四舍五入
                        {realx = 0.000001;}
                        else
                        {realx = round(realx * pow(10, 6)) * pow(10, -6);}
                        if (realy<0.000001)
                        {realy = 0.000001;}
                        else
                        {realy = round(realy * pow(10, 6)) * pow(10, -6);}

                        str.append(" X");
                        if (NCX2[i]<NCX2[i - 1])//判断正负
                        {str.append("-");}
                        str.append(QString("%1").arg(realx, 0, 'q', 6));
                        str.append(" Y");
                        if (NCY2[i]<NCY2[i - 1])//判断正负
                        {str.append("-");}
                        str.append(QString("%1").arg(realy, 0, 'q', 6));

                        ui->gCode->appendPlainText(str);
                    }
                }
            }
            else
            {
                if(way==1)
                {
                    for (int i = 1; i<changdu_dengchang; i++)
                    {
                        rowNum++;
                        str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum% 1000) / 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum% 10) % 10);
                        realx = fabs(x_dengchangdu[i] - x_dengchangdu[i - 1]);//相对坐标x
                        realy = fabs(y_dengchangdu[i] - y_dengchangdu[i - 1]);//相对坐标y

                        if (realx<0.000001)//四舍五入
                        {realx = 0.000001;}
                        else
                        {realx = round(realx * pow(10, 6)) * pow(10, -6);}
                        if (realy<0.000001)
                        {realy = 0.000001;}
                        else
                        {realy = round(realy * pow(10, 6)) * pow(10, -6);}

                        str.append(" X");
                        if (x_dengchangdu[i]<x_dengchangdu[i - 1])//判断正负
                        {str.append("-");}
                        str.append(QString("%1").arg(realx, 0, 'q', 6));
                        str.append(" Y");
                        if (y_dengchangdu[i]<y_dengchangdu[i - 1])//判断正负
                        {str.append("-");}
                        str.append(QString("%1").arg(realy, 0, 'q', 6));

                        ui->gCode->appendPlainText(str);
                    }
                }
                else
                {
                    for (int i = 1; i<changdu_dengwucha; i++)
                    {
                        rowNum++;
                        str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum% 1000) / 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum% 10) % 10);
                        str.append(" X");
                        realx = fabs(x_dengwucha[i] - x_dengwucha[i - 1]);//相对坐标x
                        realy = fabs(y_dengwucha[i] - y_dengwucha[i - 1]);//相对坐标y

                        if (realx<0.000001)//四舍五入
                        {realx = 0.000001;}
                        else
                        {realx = round(realx * pow(10, 6)) * pow(10, -6);}
                        if (realy<0.000001)
                        {realy = 0.000001;}
                        else
                        {realy = round(realy * pow(10, 6)) * pow(10, -6);}

                        str.append(" X");
                        if (x_dengwucha[i]<x_dengwucha[i - 1])//判断正负
                        {str.append("-");}
                        str.append(QString("%1").arg(realx, 0, 'q', 6));
                        str.append(" Y");
                        if (y_dengwucha[i]<y_dengwucha[i - 1])//判断正负
                        {str.append("-");}
                        str.append(QString("%1").arg(realy, 0, 'q', 6));

                        ui->gCode->appendPlainText(str);
                    }
                }
            }

            rowNum++;//倒数第三行 退刀
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G00");
            str.append(" Z");
            str.append(QString::number(ui->tuidaogaodu->value()));
            ui->gCode->appendPlainText(str);

            rowNum++;//倒数第二行 回原点 关附属设备
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" G40 G00");//取消刀补 刀具回原位
            str.append(" X");
            str.append(QString::number(x-realx));
            str.append(" Y");
            str.append(QString::number(y-realy));
            str.append(" M05 M09");//关主轴 冷却液
            ui->gCode->appendPlainText(str);

            rowNum++;//最后一行 程序结束
            str = "N" + QString::number(rowNum / 1000) + QString::number((rowNum % 1000)/ 100) + QString::number((rowNum % 100) / 10) + QString::number((rowNum % 10) %10);
            str.append(" M02");
            ui->gCode->appendPlainText(str);
        }
    }

}

void MainWindow::on_shengchengNC_2_clicked()//点击保存NC代码
{
    kaishifangzhen=0;
    QClipboard *clipboard = QApplication::clipboard();
    QString selectedFileNameFilter;
    QString fileNameFilter="NC文件(*.nc);;LinuxCNC文件(*.ngc);;文本文件(*.txt)";
    QString fileName=QFileDialog::getSaveFileName(nullptr,tr("保存文件"),"gCode.nc",fileNameFilter,&selectedFileNameFilter);
    if (!fileName.isEmpty())//检查是否输入文件名
    {
        if (!fileName.contains('.'))//检查是否输入后缀
        {
            QStringList filterParts = fileNameFilter.split(";");//查找用户选择的文件格式
            for (const QString &part : filterParts)
            {
                if (part.contains(selectedFileNameFilter))
                {
                    QString extension = part.split("(").at(1).split(")").at(0);//自动为文件添加文件名
                    fileName += "." + extension;
                    break;
                }
            }
        }
    }
    QFile file(fileName);
    file.setPermissions(QFile::WriteUser | QFile::ReadUser);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"保存失败","保存失败，请重新保存或尝试以管理员权限启动软件");
        return;
    }
    file.write(ui->gCode->toPlainText().toUtf8());
    file.close();
    clipboard->setText(fileName);
    QMessageBox::information(this,"保存成功","文件成功保存在"+fileName+"\n\n文件路径已复制到剪贴板");
}

void MainWindow::jiShiQi()//仿真计时函数
{
    timer->stop();
    float currentProgress;
    if(ui->xuanzequxian->currentIndex()==0 && time<changdu_compensate)//幂函数走刀
    {
        if(kaishifangzhen)
        {
            time+=60;
            currentProgress=(time*100)/changdu_compensate;
            ui->progressBar->setValue(currentProgress);
        }
        else
        {
            time=0;
        }
    }

    if(ui->xuanzequxian->currentIndex()==0 && time>=changdu_compensate && kaishifangzhen==1)//幂函数完成
    {
        time=0;
        QMessageBox::information(this,"加工完成","加工完成");
        ui->progressBar->setValue(0);
        return;
    }

    if(ui->xuanzequxian->currentIndex()==1 && time<COMX.length())//凸轮走刀
    {
        if(kaishifangzhen)
        {
            time+=60;
            currentProgress=(time*100)/COMX.length();
            ui->progressBar->setValue(currentProgress);
        }
        else
        {
            time=0;
        }
    }

    if(ui->xuanzequxian->currentIndex()==1 && time>=COMX.length() && kaishifangzhen==1)//凸轮完成
    {
        time=0;
        QMessageBox::information(this,"加工完成","加工完成");
        ui->progressBar->setValue(0);
        return;
    }

    update();
    timer->start(10000/ui->jinjisudu->value());
}

void MainWindow::on_dengwucha_toggled(bool checked)//更新最大误差和段数
{
    if(ui->xuanzequxian->currentIndex()==0)
    {
        ui->zuidawuchaxianshi->setText(QString::number(ui->gudingwucha->value()));
        ui->quxianduanshuxianshi->setText(QString::number(MIHANSHUXIAOSHU().dengwucha_zuobiao(ui->x_qidian->value(),ui->x_zhongdian->value(),ui->mishu->value(),ui->gudingwucha->value())));
    }

    if(ui->xuanzequxian->currentIndex()==1)
    {
        ui->zuidawuchaxianshi->setText(QString::number(ui->gudingwucha->value()));
        ui->quxianduanshuxianshi->setText(QString::number(YUXUANPAMXING().dengwucha_zuobiao(ui->jiyuanzhijing->value(),ui->shengcheng->value(),ui->shengchengjiao->value(),ui->huichengjiao->value(),ui->yuanxiujiao->value(),ui->jinxiujiao->value(),ui->gudingwucha->value(),1)));
    }
}

void MainWindow::on_dengjianju_toggled(bool checked)//更新最大误差和段数
{
    if(ui->xuanzequxian->currentIndex()==0)
    {
        ui->zuidawuchaxianshi->setText(QString::number(MIHANSHUXIAOSHU().dengchang_zuobiao(ui->x_qidian->value(),ui->x_zhongdian->value(),ui->mishu->value(),ui->zuidawucha->value())));
        ui->quxianduanshuxianshi->setText(QString::number(changdu_dengchang));
    }

    if(ui->xuanzequxian->currentIndex()==1)
    {
        ui->quxianduanshuxianshi->setText(QString::number(YUXUANPAMXING().dengchang_zuobiao(ui->jiyuanzhijing->value(),ui->shengcheng->value(),ui->shengchengjiao->value(),ui->huichengjiao->value(),ui->yuanxiujiao->value(),ui->jinxiujiao->value(),ui->zuidawucha->value(),1)));
        ui->zuidawuchaxianshi->setText(QString::number(maxError));
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)//改变窗口缩放
{
    paintScale = float(value)/100;
    update();
}

void MainWindow::on_xuanzequxian_currentChanged(int index)//更换曲线类型时清空窗口
{
    shengchengdaojulujing=0;
    shengchengyuanlujing=0;
    kaishifangzhen=0;
    ui->progressBar->setValue(0);
    ui->horizontalSlider->setValue(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}


#include "GeneticAlgorithm.h"
#include <algorithm>
#include <random>
#include <iostream>

// 构造函数
GeneticAlgorithm::GeneticAlgorithm(int rows, int cols, int populationSize, int generations, double mutationRate)
    : rows(rows), cols(cols), populationSize(populationSize), generations(generations), mutationRate(mutationRate) {}

// 生成个体
std::vector<std::vector<int>> GeneticAlgorithm::generateIndividual() const {
    std::vector<std::vector<int>> individual(rows, std::vector<int>(cols, 0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, cols - 1);
    for (int i = 0; i < rows; ++i) {
        int index = dis(gen);
        individual[i][index] = 1;
    }
    return individual;
}

// 生成种群
std::vector<std::vector<std::vector<int>>> GeneticAlgorithm::generatePopulation() const {
    std::vector<std::vector<std::vector<int>>> population;
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(generateIndividual());
    }
    return population;
}

// 适应度函数
double GeneticAlgorithm::fitness(const std::vector<std::vector<int>>& individual) const {
    double fit = 0.0;
    // 假设适应度函数为所有元素之和（即每行中1的位置）
    for (int i = 0; i < rows; ++i) {
        fit += std::distance(individual[i].begin(), std::find(individual[i].begin(), individual[i].end(), 1));
    }
    return fit;
}

// 选择操作
std::vector<std::vector<int>> GeneticAlgorithm::selection() const {
    std::vector<double> fitnessValues;
    for (const auto& ind : population) {
        fitnessValues.push_back(fitness(ind));
    }

    std::discrete_distribution<int> dist(fitnessValues.begin(), fitnessValues.end());
    std::mt19937 gen{std::random_device{}()};
    return population[dist(gen)];
}

// 交叉操作
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> GeneticAlgorithm::crossover(const std::vector<std::vector<int>>& parent1, const std::vector<std::vector<int>>& parent2) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, rows - 1);
    int crossover_point = dis(gen);

    std::vector<std::vector<int>> child1 = parent1;
    std::vector<std::vector<int>> child2 = parent2;

    for (int i = crossover_point; i < rows; ++i) {
        std::swap(child1[i], child2[i]);
    }

    return {child1, child2};
}

// 变异操作
void GeneticAlgorithm::mutate(std::vector<std::vector<int>>& individual) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> row_dis(0, rows - 1);
    std::uniform_int_distribution<> col_dis(0, cols - 1);

    if (dis(gen) < mutationRate) {
        int row = row_dis(gen);
        int new_col = col_dis(gen);

        // 将当前行的1移动到新的列位置
        std::fill(individual[row].begin(), individual[row].end(), 0);
        individual[row][new_col] = 1;
    }
}

// 获取最优个体
std::vector<std::vector<int>> GeneticAlgorithm::getBestIndividual() const {
    return *std::max_element(population.begin(), population.end(), [this](const auto& a, const auto& b) {
        return fitness(a) < fitness(b);
    });
}

// 运行遗传算法
void GeneticAlgorithm::run() {
    population = generatePopulation();
    for (int generation = 0; generation < generations; ++generation) {
        std::vector<std::vector<std::vector<int>>> newPopulation;
        for (int i = 0; i < populationSize / 2; ++i) {
            auto parent1 = selection();
            auto parent2 = selection();
            auto [child1, child2] = crossover(parent1, parent2);
            mutate(child1);
            mutate(child2);
            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }
        population = newPopulation;
    }
}

// 打印最优解
void GeneticAlgorithm::printBestSolution() const {
    auto best = getBestIndividual();
    std::cout << "Best solution with fitness " << fitness(best) << ":\n";
    for (const auto& row : best) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}
