#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

#include <armadillo>

#define FS          96000

#define DSIZE       8192
#define DSIZE2      (DSIZE/2)

#define FFT_SIZE    DSIZE2
#define FFT_SIZE2   (FFT_SIZE/2)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendCommand();
    void readData();
    void error(QSerialPort::SerialPortError error);
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
     void calculateFFT();

    QSerialPort serial;
    QByteArray senddata;
    QByteArray readdata;

    QVector<double> magnitudeData;
    QVector<double> phaseData;
    QVector<double> fftWin;

    arma::cx_vec fftData;

};
#endif // MAINWINDOW_H
