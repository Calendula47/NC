#ifndef YUXUANPAMXING_H
#define YUXUANPAMXING_H
#include <QWidget>
#include <QPainter>
class YUXUANPAMXING : public QWidget
{
    Q_OBJECT
public:
    explicit YUXUANPAMXING(QWidget *parent = 0);

    int dengchang_zuobiao(float r,float h,float o,float oo,float ooo,float oooo,float permit_eeror,float step);
    int dengwucha_zuobiao(float r,float h,float o,float oo,float ooo,float oooo,float permit_eeror,float step);
    int dengchang_compute_toolcompensateright(float tool);
    int dengchang_compute_toolcompensateleft(float tool);
    int dengwucha_compute_toolcompensateright(float tool);
    int dengwucha_compute_toolcompensateleft(float tool);

    ~YUXUANPAMXING();
signals:

};

extern float maxError;

extern QList<float> NCX; //等间距升程x坐标
extern QList<float> NCY; //等间距升程y坐标

extern QList<float> NCX2; //等误差升程x坐标
extern QList<float> NCY2; //等误差升程y坐标

extern QList<float> COMX; //刀补X
extern QList<float> COMY; //刀补Y

#endif
