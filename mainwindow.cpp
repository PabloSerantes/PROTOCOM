#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSerialPort1 = new QSerialPort(this);
    QSerialPort1 -> setPortName("COM3");
    QSerialPort1 -> setBaudRate(115200);
    QSerialPort1 -> setDataBits(QSerialPort :: Data8);
    QSerialPort1 -> setParity(QSerialPort :: EvenParity); // Para st no parity
    QSerialPort1 -> setFlowControl(QSerialPort :: NoFlowControl);

    connect(QSerialPort1, &QSerialPort::readyRead, this, &MainWindow::onQSerialPort1Rx);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onQSerialPort1Rx(){
    int count;
    uint8_t *buf;

    count = QSerialPort1 -> bytesAvailable();
    if(count <= 0){
        return;
    }
    buf = new uint8_t [count];
    QSerialPort1 -> read((char *)buf, count);

    for(int i=0; i < count; i++){
        strRx = strRx + QString("%1").arg(buf[i], 2, 16, QChar('0'));
    }
    ui -> lineEdit -> setText(strRx);
    delete[] buf;
}

void MainWindow::on_pushButton_clicked()
{
    if(QSerialPort1 -> isOpen()){
        QSerialPort1 -> close();
        ui ->pushButton->setText("OPEN");
    }else{
        if(QSerialPort1 -> open(QSerialPort::ReadWrite)){
            ui ->pushButton-> setText("CLOSE");
        }else{
            QMessageBox::information(this, "PORT", "No se pudo abrir el puerto");
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    uint8_t tx[12];
    if(QSerialPort1 -> isOpen()){
        tx[0] = 'U';
        tx[1] = 'N';
        tx[2] = 'E';
        tx[3] = 'R';
        QSerialPort1 -> write((char *)tx, 4);
        strRx = "0x";
    }else{
        QMessageBox::information(this, "PORT", "Abrir el puerto");
    }
}
