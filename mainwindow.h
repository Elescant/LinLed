#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include "Lin.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readReadSlot();
    void on_BtnOpen_clicked();

    void on_test_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *m_serial;
    Lin *lin_handle;
};

#endif // MAINWINDOW_H
