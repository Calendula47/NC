#ifndef YUXUANPAMXING_H
#define YUXUANPAMXING_H
#include <QWidget>
#include <QPainter>
class YUXUANPAMXING : public QWidget
{
    Q_OBJECT
public:
    explicit YUXUANPAMXING(QWidget *parent = 0);

    void PaintTulun_dengjianju(QPainter *painter);
    void PaintTulun_dengwucha(QPainter *painter);
    void PaintTulun_toolcompensate(QPainter *painter);
    int dengchang_zuobiao(float r,float h,float o,float oo,float ooo,float oooo,float permit_eeror,float step);
    int dengwucha_zuobiao(float r,float h,float o,float oo,float ooo,float oooo,float permit_eeror,float step);
    int dengchang_compute_toolcompensateright(float tool);
    int dengchang_compute_toolcompensateleft(float tool);
    int dengwucha_compute_toolcompensateright(float tool);
    int dengwucha_compute_toolcompensateleft(float tool);
    QList<float> NCX; //等间距升程x坐标
    QList<float> NCY; //等间距升程y坐标

    QList<float> NCX2; //等误差升程x坐标
    QList<float> NCY2; //等误差升程y坐标

    QList<float> COMX; //刀补X
    QList<float> COMY; //刀补Y

    ~YUXUANPAMXING();
signals:

};

extern float maxError;

#endif
