#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

#define MX 40
#define MY 20


class Plot : public QWidget
{
    Q_OBJECT
public:
    explicit Plot(QWidget *parent = nullptr);

    QVector<double> dataSeries;

    QColor backgroundColor=Qt::darkBlue;
    QColor gridColor=Qt::gray;
    QColor textColor=Qt::white;
    QColor plotColor=Qt::red;

    void setRange(double minX, double maxX, double minY, double maxY);
    void setAxes(int divX, double minX, double maxX, int divY, double minY, double maxY);

private:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void drawLinearGrid(QPainter &painter);
    void drawLinearData(QPainter &painter);

    double dx, dy, dvx, dvy;
    int gx, gy, gw, gh, gmy;


    int gridNumX=10, gridNumY=10;
    double minValueX= 0, maxValueX=10;
    double minValueY=-1, maxValueY=1;
    double minAxisX= 0, maxAxisX=1;
    double minAxisY=-1, maxAxisY=1;

signals:


};

#endif // PLOT_H
