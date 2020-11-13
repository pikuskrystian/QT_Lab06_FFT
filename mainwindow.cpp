#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    centralWidget()->layout()->setContentsMargins(0,0,0,0);
    centralWidget()->layout()->setSpacing(0);

    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::sendCommand);
    connect(&serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(&serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(error(QSerialPort::SerialPortError)));

    ui->plot->dataSeries.resize(DSIZE2);
    ui->plot->dataSeries.fill(0);
    ui->plot->plotColor=Qt::red;
    ui->plot->setRange(0, DSIZE2-1, -1, 1);
    ui->plot->setAxes(10, 0, 100, 10, -1, 1);

    ui->plot_2->dataSeries.resize(FFT_SIZE2);
    ui->plot_2->dataSeries.fill(0);
    ui->plot_2->plotColor=Qt::green;
    ui->plot_2->setRange(0, FFT_SIZE2-1, 0, 1);
    ui->plot_2->setAxes(16, 0, FS/2, 10, 0, 1);

    fftData.resize(FFT_SIZE);
    fftWin.resize(FFT_SIZE);

    // Rectangular Window
    fftWin.fill(1);

    magnitudeData.resize(FFT_SIZE/2);
    phaseData.resize(FFT_SIZE/2);

    ui->statusbar->showMessage("No device");
    QString portname;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        if (info.description()=="VCOM-MEAS") {
            portname=info.portName();
            serial.setPortName(portname);
            if (serial.open(QIODevice::ReadWrite)) {
                ui->statusbar->showMessage(tr("Device: %1").arg(info.description()));
                serial.clear();
                ui->statusbar->setEnabled(true);
            } else {
                ui->statusbar->showMessage(tr("Can't open %1, error code %2") .arg(serial.portName()).arg(serial.error()));
                return;
            }
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    serial.close();
    delete ui;
}

void MainWindow::sendCommand()
{
    senddata.clear();
    senddata.resize(1);
    senddata[0]=static_cast<uint8_t>(ui->actionRun->isChecked() << 7); // Run/Stop
    serial.write(senddata);
}

void MainWindow::readData()
{
    if (serial.size() >= DSIZE) {

        readdata=serial.read(DSIZE);
        qDebug()<<readdata.size();

        uint16_t *sample=reinterpret_cast<uint16_t*>(readdata.data());
        for (int n=0; n<ui->plot->dataSeries.size(); n++) {
            ui->plot->dataSeries[n]=(sample[n]-32768)/32768.0;
        }

        calculateFFT();
        ui->plot_2->dataSeries=magnitudeData;
        ui->plot->update();
        ui->plot_2->update();
    }
}

void MainWindow::error(QSerialPort::SerialPortError error)
{
    qDebug()<<error;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if(ui->actionRun->isChecked())
        ui->actionRun->trigger();
}

void MainWindow::calculateFFT()
{
    for(int i=0;i<FFT_SIZE; i++) {

        fftData[static_cast<uint>(i)].real(ui->plot->dataSeries[i]*fftWin[i]);
        fftData[static_cast<uint>(i)].imag(0);
    }

    fftData=arma::fft(fftData);

    for(int i=0;i<FFT_SIZE2; i++) {

        magnitudeData[i]=abs(fftData[static_cast<uint>(i)])/(FFT_SIZE2);
        phaseData[i]=arg(fftData[static_cast<uint>(i)]);
    }

}
/*
void MainWindow::on_verticalSlider_valueChanged(int value)
{
    double f=value*100;
    for(int n=0; n<ui->plot->dataSeries.size(); n++)
        ui->plot->dataSeries[n]=(1.0*sin(2*M_PI*f*n/static_cast<double>(FS)));

    calculateFFT();

    ui->plot_2->dataSeries=magnitudeData;
    ui->plot_2->dataSeries=phaseData;
    ui->plot->update();
    ui->plot_2->update();
}

*/
