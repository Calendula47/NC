#ifndef MIHANSHUXIAOSHU_H
#define MIHANSHUXIAOSHU_H
#include <QWidget>
#include <QPainter>

class MIHANSHUXIAOSHU : public QWidget
{
    Q_OBJECT
public:
    explicit MIHANSHUXIAOSHU(QWidget *parent = 0);

    double mihanshu(double x,double y);
    void PaintMihanshuxiaoshu_dengjianju(QPainter *painter);
    void PaintMihanshuxiaoshu_dengwucha(QPainter *painter);
    void Paintmihanshu_toolcompensate(QPainter *painter);
    double bijinwucha(double x0,double x1, double zhishu);
    double dengchang_zuobiao(double a, double b, double exponent, double max_error);
    double dengwucha_zuobiao(double a, double b, double zhishu,double deng_error);
    void compute_toolcompensateright(int chabu_type, double toolright,double zhishu);
    void compute_toolcompensateleft(int chabu_type, double toolleft,double zhishu);
    void summit_error();

    ~MIHANSHUXIAOSHU();
signals:
};

extern int changdu_dengchang;
extern int changdu_dengwucha;
extern int changdu_compensate;
extern int error_flag;


#endif
