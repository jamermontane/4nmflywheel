#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qstring.h>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include "motor.h"
#include "motordriver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool initDriver1();
    void initCombox();

public slots:
    void logMsg(QString);


private slots:
    void on_pushButton_system_power_1_clicked();

    void on_pushButton_single_test_mode_1_clicked();

private:
    Ui::MainWindow *ui;

    Motor m_motor1_;
    Motor m_motor2_;
    Motor m_motor3_;
    Motor m_motor4_;
    Motor m_motor5_;
    Motor m_motor6_;

    MotorDriver* p_driver1_;
    MotorDriver* p_driver2_;
    MotorDriver* p_driver3_;
    MotorDriver* p_driver4_;
    MotorDriver* p_driver5_;
    MotorDriver* p_driver6_;

    QThread* p_driver_thread1_;
    //...etc


    bool m_sys_status_1_;

    QTimer m_timer_get_data_;

};

#endif // MAINWINDOW_H
