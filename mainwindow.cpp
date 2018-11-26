#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_serial = new QSerialPort(this);
    lin_handle = new Lin(m_serial);
    //connect(m_serial,&QSerialPort::readyRead,this,&MainWindow::readReadSlot);
//    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
//    {
//        qDebug()<<"Name :" << info.portName();
//        qDebug()<<"Description :" << info.description();
//        qDebug()<<"Manafacturer :" << info.manufacturer();
//        qDebug()<<"Serial Number :" << info.serialNumber();
//        qDebug()<<"System Location :" << info.systemLocation();
//        QSerialPort serial;
//        serial.setPort(info);
//        if(serial.open(QIODevice::ReadWrite))
//        {
//            serial.close();
//        }else
//        {
//        }
//    }
}

MainWindow::~MainWindow()
{
    if(ui->BtnOpen->text() == "close")
    {
        m_serial->close();
    }
    delete m_serial;
    delete ui;
}

void MainWindow::readReadSlot()
{
    QByteArray arr = m_serial->readAll();
    if(!arr.isEmpty())
    {
        qDebug()<<arr.toHex();
    }
}

void MainWindow::on_BtnOpen_clicked()
{
    if(ui->BtnOpen->text() == "open")
    {
        m_serial->setPortName("ttyUSB0");
         if(m_serial->open(QIODevice::ReadWrite))
         {
            m_serial->setBaudRate(QSerialPort::Baud115200);
            m_serial->setDataBits(QSerialPort::Data8);
            m_serial->setParity(QSerialPort::NoParity);
            m_serial->setFlowControl(QSerialPort::NoFlowControl);
            m_serial->setStopBits(QSerialPort::OneStop);
            //m_serial->setReadBufferSize(128);
            ui->BtnOpen->setText("close");
         }else
         {
             qDebug()<<"open failed";
         }
    }else
    {
        m_serial->close();
        ui->BtnOpen->setText("open");
    }
}

void MainWindow::on_test_clicked()
{
    qDebug()<<"send";
    if(lin_handle->testlink())
    {
       qDebug()<<"link success";
    }else
    {
       qDebug()<<"link failed";
    }

}
