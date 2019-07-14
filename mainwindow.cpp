#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_sys_status_1_(false)
{
    ui->setupUi(this);
    initCombox();
    initDriver1();


    m_timer_get_data_.setInterval(100); //get data timer: 0.1s
}

MainWindow::~MainWindow()
{
    delete ui;
    p_driver_thread1_->quit();
    p_driver_thread1_->wait();


}

bool MainWindow::initDriver1()
{
    //driver thread 1
    p_driver1_ = new MotorDriver;
    p_driver_thread1_ = new QThread;
    p_driver1_->moveToThread(p_driver_thread1_);

    connect(p_driver_thread1_,&QThread::finished,p_driver1_,&MotorDriver::deleteLater);
    connect(p_driver_thread1_,&QThread::finished,p_driver_thread1_,&MotorDriver::deleteLater);
    connect(p_driver1_,&MotorDriver::sendErrText,this,&MainWindow::logMsg);
    connect(p_driver_thread1_,&QThread::started,p_driver1_,&MotorDriver::init);
    //seems that should connect driver obj later
    p_driver1_->setPortName("COM1");
    p_driver1_->setBaudRate("115200");
    p_driver_thread1_->start();

    //get data timer init
    connect(&m_timer_get_data_,&QTimer::timeout,p_driver1_,&MotorDriver::getMotorData);
    //control init
    connect(&m_motor1_,&Motor::sendMoTorSpd,p_driver1_,&MotorDriver::ctlMotorSpd);
    connect(&m_motor1_,&Motor::sendMoTorTor,p_driver1_,&MotorDriver::ctlMotorTor);
    connect(p_driver1_,&MotorDriver::sendMotorSpd,&m_motor1_,&Motor::setSpeed);
    connect(p_driver1_,&MotorDriver::sendMotorCur,&m_motor1_,&Motor::setCurrent);
    connect(p_driver1_,&MotorDriver::sendMotorTmp,&m_motor1_,&Motor::setTemperature);


    return true;
}

void MainWindow::initCombox()
{
    ui->comboBox_motor_test_mode_1->addItem("速度模式");
    ui->comboBox_motor_test_mode_1->addItem("力矩模式");
}

void MainWindow::logMsg(QString text)
{
    qDebug() << text;
    ui->statusBar->showMessage(text,2000);
}

void MainWindow::on_pushButton_system_power_1_clicked()
{
    if (!m_sys_status_1_){
        //power not open

        m_sys_status_1_ = true;
        ui->pushButton_system_power_1->setText("关闭");


    }
    else{
        //power had open
        m_sys_status_1_ = false;
        ui->pushButton_system_power_1->setText("启动");

        if (m_motor1_.getIsRunning()){
            QMessageBox::warning(this,"警告","依然有任务运行中，但电源已关闭。");
        }
    }
}

void MainWindow::on_pushButton_single_test_mode_1_clicked()
{
    static bool this_mode_running = false;
    if (!m_sys_status_1_){
        QMessageBox::warning(this,"电源未打开","电源未打开,请开启总电源！");
    }
    else{
        if(!m_motor1_.getIsRunning() && !this_mode_running){
            switch(ui->comboBox_motor_test_mode_1->currentIndex()){
            case 0: m_motor1_.setSetSpeed(ui->doubleSpinBox_motor_test_spd_1->text().toDouble()); break;
            case 1: m_motor1_.setSetTorque(ui->doubleSpinBox_motor_test_spd_1->text().toDouble());break;
            default:
                break;
            }
            m_motor1_.setIsRunning(true);
            m_timer_get_data_.start();
            this_mode_running = true;
            ui->pushButton_single_test_mode_1->setText("停止");
        }
        else{
            if (this_mode_running){
                m_motor1_.setIsRunning(false);

                m_timer_get_data_.stop();
                this_mode_running = false;
                ui->pushButton_single_test_mode_1->setText("启动");
            }
            else{
                QMessageBox::warning(this,"警告","其它模式运行中。");
            }
        }
    }
}
