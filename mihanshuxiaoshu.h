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
extern double *x_dengchangdu;
extern double *y_dengchangdu;
extern double *x_dengwucha;
extern double *y_dengwucha;
extern double *x_toolcompensate;
extern double *y_toolcompensate;
extern double dengchang_max_error;
extern std::vector<double> changdu_x_nodes;
extern std::vector<double> changdu_y_nodes;
extern std::vector<double> dengwucha_x_nodes;
extern std::vector<double> dengwucha_y_nodes;

#endif
