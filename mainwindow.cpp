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
    ui->xuanzequxian->setCurrentIndex(0);
    ui->x_qidian->setSingleStep(0.01);
    ui->x_zhongdian->setSingleStep(0.01);
    ui->x_zhongdian->setRange(0,200);
    ui->mishu->setRange(0,1);
    ui->mishu->setDecimals(4);
    ui->mishu->setSingleStep(0.0001);
    ui->jiyuanzhijing->setSingleStep(0.01);
    ui->shengcheng->setSingleStep(0.01);
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
    ui->zhuzhouzhuansu->setRange(0,10000);
    ui->zhuzhouzhuansu->setSingleStep(100);
    ui->jinjisudu->setRange(0,1000);
    ui->jinjisudu->setSingleStep(10);
    ui->daojubanjing->setSingleStep(0.001);
    ui->daojubanjing->setDecimals(3);
    ui->zuidawucha->setDecimals(4);
    ui->zuidawucha->setSingleStep(0.0001);
    ui->gudingwucha->setDecimals(4);
    ui->gudingwucha->setSingleStep(0.0001);
    ui->wuchashezhi->setVisible(false);
    ui->dengjianju->setChecked(false);
    ui->dengwucha->setChecked(false);
    ui->bijinfangfa->setVisible(false);
    ui->quxianduanshuxianshi->setText("未计算，请选择逼近方法");
    ui->zuidawuchaxianshi->setText("未计算，请选择逼近方法");

    ui->x_qidian->setValue(0);
    ui->x_zhongdian->setValue(20);
    ui->mishu->setValue(0.8);

    ui->jiyuanzhijing->setValue(10);
    ui->shengcheng->setValue(10);
    ui->shengchengjiao->setValue(60);
    ui->huichengjiao->setValue(60);
    ui->jinxiujiao->setValue(120);
    ui->yuanxiujiao->setValue(120);

    ui->zhuzhouzhuansu->setValue(5000);
    ui->jinjisudu->setValue(120);
    ui->daojubanjing->setValue(1.25);

    ui->gudingwucha->setValue(0.002);

    ui->zuidawucha->setValue(0.002);

    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
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

void MainWindow::paint()
{
    if(shengchengyuanlujing==1)//生成原图像
    {
        QPainter mainpainter(&image);
        mainpainter.fillRect(rect(),Qt::transparent);
        mainpainter.setRenderHint(QPainter::Antialiasing, true);
        mainpainter.drawRect(0,0,paintWidth,paintHeight);
        mainpainter.translate((ui->paint_widget->width()/2),(-9+ui->paint_widget->height()/2));
        mainpainter.scale(paintScale,-paintScale);
        mainpainter.setPen(QPen(Qt::blue,0.3,Qt::SolidLine,Qt::RoundCap));
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
        toolpainter.setPen(QPen(Qt::green,0.3,Qt::SolidLine,Qt::RoundCap));
        if(ui->xuanzequxian->currentIndex()==0)//幂函数
        {
            if(ui->zuodaobu->isChecked())//左刀补
            {
                MIHANSHUXIAOSHU().compute_toolcompensateleft(ui->dengwucha->isChecked(),ui->daojubanjing->value(),ui->mishu->value());
                for(int j=1;j<changdu_compensate;j++)
                {
                    toolpainter.drawLine(10*x_toolcompensate[j-1],10*y_toolcompensate[j-1],10*x_toolcompensate[j],10*y_toolcompensate[j]);
                }
            }

            if(ui->youdaobu->isChecked())//右刀补
            {
                MIHANSHUXIAOSHU().compute_toolcompensateright(ui->dengwucha->isChecked(),ui->daojubanjing->value(),ui->mishu->value());
                for(int j=1;j<changdu_compensate;j++)
                {
                    toolpainter.drawLine(10*x_toolcompensate[j-1],10*y_toolcompensate[j-1],10*x_toolcompensate[j],10*y_toolcompensate[j]);
                }
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

    update();
}

void MainWindow::on_shengchenglujing_clicked()//点击生成原曲线
{
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
            MIHANSHUXIAOSHU().dengchang_zuobiao(ui->x_qidian->value(),ui->x_zhongdian->value(),ui->mishu->value(),ui->zuidawucha->value());
            MIHANSHUXIAOSHU().dengwucha_zuobiao(ui->x_qidian->value(),ui->x_zhongdian->value(),ui->mishu->value(),ui->gudingwucha->value());
            YUXUANPAMXING().dengchang_zuobiao(ui->jiyuanzhijing->value(),ui->shengcheng->value(),ui->shengchengjiao->value(),ui->huichengjiao->value(),ui->yuanxiujiao->value(),ui->jinxiujiao->value(),ui->zuidawucha->value(),1);
            YUXUANPAMXING().dengwucha_zuobiao(ui->jiyuanzhijing->value(),ui->shengcheng->value(),ui->shengchengjiao->value(),ui->huichengjiao->value(),ui->yuanxiujiao->value(),ui->jinxiujiao->value(),ui->gudingwucha->value(),1);

            shengchengyuanlujing=1;
        }
    }
}

void MainWindow::on_shengchengdaojulujing_clicked()//点击生成刀具路径
{
    if(ui->xuanzequxian->currentIndex()==1)
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
    if(ui->xuanzequxian->currentIndex()==0)
    {
        MIHANSHUXIAOSHU().summit_error();
        if(error_flag==1)
        {
            QMessageBox::warning(this,"过切警告","请更改刀具半径或检查刀补方向");
        }
        if(error_flag==0)
        {
            shengchengdaojulujing=1;
        }
    }
}

void MainWindow::on_jinxingfangzhen_clicked()//点击开始仿真
{

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

MainWindow::~MainWindow()
{
    delete ui;
}

