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
    ui->quxianduanshuxianshi->setText("未计算");
    ui->zuidawuchaxianshi->setText("未计算");

    ui->x_qidian->setValue(0);
    ui->x_zhongdian->setValue(20);
    ui->mishu->setValue(0.25);

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

    paint();
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
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawRect(0,0,paintWidth,paintHeight);
    painter.setTransform(QTransform().rotate(180, Qt::XAxis));
    painter.translate((ui->paint_widget->width()/2/paintScale),(ui->paint_widget->height()/2/paintScale));
    painter.setPen(QPen(Qt::blue,1));
    //坐标初始化

    if(shengchengyuanlujing==1){
        if(ui->xuanzequxian->currentIndex()==0)//幂函数
        {
            if(ui->dengjianju->isChecked())//等间距
            {
                MIHANSHUXIAOSHU().PaintMihanshuxiaoshu_dengjianju(&painter);
                qDebug()<<"等间距"<<painters;
                shengchengyuanlujing=0;
            }

            if(ui->dengwucha->isChecked())//等误差
            {
                MIHANSHUXIAOSHU().PaintMihanshuxiaoshu_dengwucha(&painter);
            }
        }

        if(ui->xuanzequxian->currentIndex()==1)//凸轮
        {
            if(ui->dengjianju->isChecked())//等间距
            {
                YUXUANPAMXING().PaintTulun_dengjianju(&painter);
            }

            if(ui->dengwucha->isChecked())//等误差
            {
                YUXUANPAMXING().PaintTulun_dengwucha(&painter);
            }
        }
    }

    update();
}

void MainWindow::on_shengchenglujing_clicked()
{
    shengchengyuanlujing=1;

    if(ui->xuanzequxian->currentIndex()==0)//幂函数
    {
        if(ui->dengjianju->isChecked())//等间距
        {
            ui->zuidawuchaxianshi->setText(QString::number(MIHANSHUXIAOSHU().dengchang_zuobiao(ui->x_qidian->value(),ui->x_zhongdian->value(),ui->mishu->value(),ui->zuidawucha->value())));
            ui->quxianduanshuxianshi->setText(QString::number(changdu_dengchang));
        }

        if(ui->dengwucha->isChecked())//等误差
        {

        }
    }

    if(ui->xuanzequxian->currentIndex()==1)//凸轮
    {
        if(ui->dengjianju->isChecked())//等间距
        {
            ui->quxianduanshuxianshi->setText(QString::number(YUXUANPAMXING().dengchang_zuobiao(ui->jiyuanzhijing->value(),ui->shengcheng->value(),ui->shengchengjiao->value(),ui->huichengjiao->value(),ui->yuanxiujiao->value(),ui->jinxiujiao->value(),ui->zuidawucha->value(),1)));
            ui->zuidawuchaxianshi->setText(QString::number(maxError));
        }

        if(ui->dengwucha->isChecked())//等误差
        {

        }
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    paintScale = float(value)/100;
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}
