#include "plot.h"

Plot::Plot(QWidget *parent) : QWidget(parent)
{
    this->setMinimumHeight(4*MY);
    this->setMinimumWidth(4*MX);

}

void Plot::setRange(double minX, double maxX, double minY, double maxY)
{
    minValueX = minX;
    maxValueX = maxX;
    minValueY = minY;
    maxValueY = maxY;

    minAxisX = minX;
    maxAxisX = maxX;
    minAxisY = minY;
    maxAxisY = maxY;
}

void Plot::setAxes(int divX, double minX, double maxX, int divY, double minY, double maxY)
{
    gridNumX = divX;
    gridNumY = divY;
    minAxisX = minX;
    maxAxisX = maxX;
    minAxisY = minY;
    maxAxisY = maxY;
}

void Plot::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    gx=MX;
    gy=MY;
    gw=this->width()-(2*MX);
    gh=this->height()-(2*MY);
}
void Plot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    drawLinearGrid(painter);
    drawLinearData(painter);
}

void Plot::drawLinearGrid(QPainter &painter)
{
    QPen pen;

    dx=gw/static_cast<double>(gridNumX);
    dy=gh/static_cast<double>(gridNumY);

    pen.setWidth(1);
    pen.setColor(backgroundColor);
    painter.setPen(pen);
    painter.setBrush(backgroundColor);
    painter.drawRect(0, 0, this->width(), this->height());

    // -----  frame ------------
    pen.setStyle(Qt::SolidLine);
    pen.setColor(gridColor);
    pen.setWidth(2);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);
    painter.drawRect(gx, gy, gw, gh);
    // ----- grid ------------
    pen.setStyle(Qt::DotLine);
    pen.setWidth(1);
    painter.setPen(pen);
    for(int px=1; px<gridNumX; px++)
        painter.drawLine(QLineF(gx+(px*dx), gy, gx+(px*dx), gy+gh));
    for(int py=1; py<gridNumY; py++)
        painter.drawLine(QLineF(gx, gy+(py*dy), gx+gw, gy+(py*dy)));
    // ------ desc ----------
    QFont font;
    font.setPointSize(8);
    painter.setFont(font);
    pen.setColor(textColor);
    painter.setPen(pen);
    dvx=maxAxisX-minAxisX;
    dvy=maxAxisY-minAxisY;
    for(int px=0; px<=gridNumX; px++)
        painter.drawText(QPoint(gx-(font.pointSize()/4)+static_cast<int>(px*dx), gy+gh+(font.pointSize()*2)), QString("%1").arg(minAxisX+(dvx*px)/static_cast<double>(gridNumX)));
    for(int py=0; py<=gridNumY; py++)
        painter.drawText(QPoint(gx-(font.pointSize()*4), gy+(font.pointSize()/2)+static_cast<int>(py*dy)), QString("%1").arg(maxAxisY-(dvy*py)/static_cast<double>(gridNumY)));
}

void Plot::drawLinearData(QPainter &painter)
{
    QPen pen;

    dx=gw/(maxValueX - minValueX);
    dy=gh/(maxValueY - minValueY);
    gmy=gy+gh+dy*minValueY;

    pen.setStyle(Qt::SolidLine);
    pen.setColor(plotColor);
    pen.setWidth(1);
    painter.setPen(pen);

    for(int i=1+minValueX; i<1+maxValueX; i++) {

        painter.drawLine(QLineF(gx+(i-1-minValueX)*dx, gmy-(dataSeries[i-1]*dy), gx+(i-minValueX)*dx, gmy-(dataSeries[i]*dy)));
    }
}



