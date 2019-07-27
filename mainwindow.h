#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qstring.h>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QTime>

#include "motor.h"
#include "motordriver.h"
#include "csqldatabase.h"

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
    void updateMotor1Display();
    void initQCustomPlot1();
    void initSql();
    QVector<QString> makeSqlVector(Motor &);
signals:
    void sendToSqlDB(QString,QString,QString,QVector<QString>);
public slots:
    void logMsg(QString);


private slots:
    void on_pushButton_system_power_1_clicked();

    void on_pushButton_single_test_mode_1_clicked();

    void updateMotor();

    void on_doubleSpinBox_motor_test_spd_1_editingFinished();

    void on_comboBox_motor_test_mode_1_currentIndexChanged(int index);

    void refreshCustomPlotData1();

    void on_doubleSpinBox_moto_test_time_valueChanged(double arg1);

	void on_pushButton_auto_test_with_air_power_1_clicked();

    void on_pushButton_auto_test_with_air_power_1_clicked();

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
    //获取数据定时器
    QTimer m_timer_get_data_;
    //发送控制指令定时器
    QTimer m_timer_update_;
    //界面刷新buffer
    QVector<double> keyContainer;
    QVector<double> tmpContainer;
    QVector<double> curContainer;
    QVector<double> spdContainer;
    QVector<double> setSpdContainer;

    //数据库部分
    SqlDataBase* p_sql_;
    QThread* p_sql_thread_;

};

#endif // MAINWINDOW_H
