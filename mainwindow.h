#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPalette>
#include <QTimer>
#include <QLineEdit>
#include <QMessageBox>
#include <QFile>
#include "mihanshuxiaoshu.h"
#include "yuxuanpamxing.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawAxis(QPainter *painter);
    void paintEvent(QPaintEvent *event);
    void generateCompenate(QPainter *painter);
    void createNCAbsolute();
    void createNCRelative();
    void generateCode();
    void paint();

    MIHANSHUXIAOSHU power;
    QImage image;
    YUXUANPAMXING cam;
    QPoint paintposition;
    QTimer *timer1;

    int paintX;
    int paintY;
    int paintWidth;
    int paintHeight;
    float paintScale = 1;
    bool paintFlag = 0;
    bool paintKnife = 0;
    float n;
    float xStart;
    float xFinish;
    bool shengchengyuanlujing = 0;
    bool shengchengdaojulujing = 0;

private slots:
    void on_shengchenglujing_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_dengjianju_toggled(bool checked);
    void on_dengwucha_toggled(bool checked);
    void on_jinxingfangzhen_clicked();
    void on_shengchengdaojulujing_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
