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
    initQCustomPlot1();

    m_timer_get_data_.setInterval(100); //get data timer 0.1s


    m_timer_update_.setInterval(500);   //update view every 0.5s
    connect(&m_timer_update_,SIGNAL(timeout()),this,SLOT(updateMotor()));
    m_timer_update_.start();

    //斜坡模式输入diable
//    ui->doubleSpinBox_motor_test_acc->setEnabled(false);
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
//    connect(&m_motor1_,&Motor::sendMoTorSpd,p_driver1_,&MotorDriver::ctlMotorSpd());
    connect(&m_motor1_,SIGNAL(sendMoTorSpd(double,double)),p_driver1_,SLOT(ctlMotorSpd2(double,double)));
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
    ui->comboBox_motor_test_mode_1->addItem("斜坡模式");
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
    static bool xp_mode_running = false;
    if (!m_sys_status_1_){
        QMessageBox::warning(this,"电源未打开","电源未打开,请开启总电源！");
    }
    else{
        if(!m_motor1_.getIsRunning() && !this_mode_running){
            switch(ui->comboBox_motor_test_mode_1->currentIndex()){
            case 0:
                m_motor1_.setSetSpeed(ui->doubleSpinBox_motor_test_spd_1->text().toDouble());
                m_motor1_.setAccelerate(ui->doubleSpinBox_motor_test_acc->text().toDouble());
                break;
            case 1: m_motor1_.setSetTorque(ui->doubleSpinBox_motor_test_spd_1->text().toDouble());break;
            case 2:
                m_motor1_.setSetSpeed(0);
                m_motor1_.initXpMode(ui->doubleSpinBox_motor_test_spd_1->text().toDouble()
                                     ,ui->doubleSpinBox_motor_test_acc->text().toDouble());
                connect(&m_timer_get_data_,SIGNAL(timeout()),&m_motor1_,SLOT(calXpMode()));
                xp_mode_running = true;
                m_motor1_.setXpStatus(true);
                break;
            default:
                break;
            }
//            m_timer_get_data_.setInterval();

            keyContainer.resize(0);
            tmpContainer.resize(0);
            curContainer.resize(0);
            spdContainer.resize(0);
            setSpdContainer.resize(0);

            m_motor1_.setIsRunning(true);
            m_timer_get_data_.start();
            this_mode_running = true;
            ui->pushButton_single_test_mode_1->setText("停止");
        }
        else{
            if (this_mode_running){
                if (xp_mode_running){
                    disconnect(&m_timer_get_data_,SIGNAL(timeout()),&m_motor1_,SLOT(calXpMode()));
                    xp_mode_running = false;
                    m_motor1_.setXpStatus(false);
                }
                m_motor1_.setIsRunning(false);
                m_motor1_.setSetSpeed(0);
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

void MainWindow::updateMotor()
{

    if (m_motor1_.getIsRunning()){
        //
        if (!m_motor1_.getXpStatus()){
            m_motor1_.setSetSpeed(ui->doubleSpinBox_motor_test_spd_1->text().toDouble());
        }
        //更新显示界面
        updateMotor1();
        refreshCustomPlotData1();
        //更新数据库
    }

    //...etc motor
}

void MainWindow::updateMotor1()
{
    //update lineedit
    ui->lineEdit_motor_set_spd_1->setText(QString::number(m_motor1_.getSetSpeed()));
    ui->lineEdit_motor_act_spd_1->setText(QString::number(m_motor1_.getSpeed()));
    ui->lineEdit_sys_cur_1->setText(QString::number(m_motor1_.getCurrent()));
}

void MainWindow::initQCustomPlot1()
{

        ui->qcp_motor_cur_1->addGraph();
        ui->qcp_motor_cur_1->graph(0)->setPen(QPen(Qt::blue));
        ui->qcp_motor_cur_1->graph(0)->setName("motorCurrent");

        QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
        timeTicker->setTimeFormat("%m:%s"); // %h代表小时
        ui->qcp_motor_cur_1->xAxis->setTicker(timeTicker);
        ui->qcp_motor_cur_1->axisRect()->setupFullAxesBox();
        ui->qcp_motor_cur_1->yAxis->setLabel("电流(A)");

        // make left and bottom axes transfer their ranges to right and top axes:
        connect( ui->qcp_motor_cur_1->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->qcp_motor_cur_1->xAxis2, SLOT(setRange(QCPRange)));
        connect( ui->qcp_motor_cur_1->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->qcp_motor_cur_1->yAxis2, SLOT(setRange(QCPRange)));

        // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:

        //dataTimer.start(0); // Interval 0 means to refresh as fast as possibl

        ui->qcp_motor_spd_1->addGraph();
        ui->qcp_motor_spd_1->graph(0)->setPen(QPen(Qt::red));
        ui->qcp_motor_spd_1->addGraph();
        ui->qcp_motor_spd_1->graph(1)->setPen(QPen(Qt::green));

        ui->qcp_motor_spd_1->yAxis->setLabel("转速(rpm)");

        ui->qcp_motor_spd_1->xAxis->setTicker(timeTicker);
        ui->qcp_motor_spd_1->axisRect()->setupFullAxesBox();
        ui->qcp_motor_spd_1->yAxis->setRange(0,1);

        connect( ui->qcp_motor_spd_1->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->qcp_motor_spd_1->xAxis2, SLOT(setRange(QCPRange)));
        connect( ui->qcp_motor_spd_1->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->qcp_motor_spd_1->yAxis2, SLOT(setRange(QCPRange)));

        ui->qcp_motor_tmp_1->addGraph();
        ui->qcp_motor_tmp_1->graph(0)->setPen(QPen(Qt::red));

        ui->qcp_motor_tmp_1->yAxis->setLabel("温度(℃)");

        ui->qcp_motor_tmp_1->xAxis->setTicker(timeTicker);
        ui->qcp_motor_tmp_1->axisRect()->setupFullAxesBox();
        //ui->widget->yAxis->setRange(-30,30);


        ui->qcp_motor_tmp_1->xAxis->setTicker(timeTicker);

        connect( ui->qcp_motor_tmp_1->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->qcp_motor_tmp_1->xAxis2, SLOT(setRange(QCPRange)));
        connect( ui->qcp_motor_tmp_1->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->qcp_motor_tmp_1->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::on_doubleSpinBox_motor_test_spd_1_editingFinished()
{
    if(m_motor1_.getIsRunning()){
        switch(ui->comboBox_motor_test_mode_1->currentIndex()){
        case 0: m_motor1_.setSetSpeed(ui->doubleSpinBox_motor_test_spd_1->text().toDouble()); break;
        case 1: m_motor1_.setSetTorque(ui->doubleSpinBox_motor_test_spd_1->text().toDouble());break;
        default:
            break;
        }
    }
}

void MainWindow::on_comboBox_motor_test_mode_1_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->label_setval_1->setText("设置速度");
        ui->label_setval_right_1->setText("设置速度");
//        ui->doubleSpinBox_motor_test_acc->setEnabled(false);
        break;
    case 1:
        ui->label_setval_1->setText("设置力矩");
        ui->label_setval_right_1->setText("设置力矩");
//        ui->doubleSpinBox_motor_test_acc->setEnabled(false);
        break;
    case 2:
        ui->label_setval_1->setText("设置速度");
        ui->label_setval_right_1->setText("设置速度");
//        ui->doubleSpinBox_motor_test_acc->setEnabled(true);
        break;
    default:
        break;
    }
}
//刷新界面速度、电流等图部分
void MainWindow::refreshCustomPlotData1()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds  elaspsed上次开始后得持续时间
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.02) // at most add point every 0.5 s
    {
      // add data to lines:
        keyContainer.push_back(key);
        tmpContainer.push_back(m_motor1_.getTemperature());
        curContainer.push_back(m_motor1_.getCurrent());
        spdContainer.push_back(m_motor1_.getSpeed());
        setSpdContainer.push_back(m_motor1_.getSetSpeed());

        ui->qcp_motor_tmp_1->graph(0)->setData(keyContainer,tmpContainer,true);
        ui->qcp_motor_cur_1->graph(0)->setData(keyContainer,curContainer,true);
        ui->qcp_motor_spd_1->graph(0)->setData(keyContainer,spdContainer,true);
        ui->qcp_motor_spd_1->graph(1)->setData(keyContainer,setSpdContainer,true);
        lastPointKey = key;
        if (keyContainer.size() > 100){
            keyContainer.pop_front();
            tmpContainer.pop_front();
            curContainer.pop_front();
            spdContainer.pop_front();
            setSpdContainer.pop_front();
        }
        ui->qcp_motor_tmp_1->graph(0)->rescaleValueAxis(true);
        ui->qcp_motor_cur_1->graph(0)->rescaleValueAxis(true);
        ui->qcp_motor_spd_1->graph(0)->rescaleValueAxis(true);
        ui->qcp_motor_spd_1->graph(1)->rescaleValueAxis(true);

        ui->qcp_motor_cur_1->xAxis->setRange(key, 8, Qt::AlignRight);
        ui->qcp_motor_spd_1->xAxis->setRange(key, 8, Qt::AlignRight);
        ui->qcp_motor_tmp_1->xAxis->setRange(key, 8, Qt::AlignRight);

        ui->qcp_motor_cur_1->replot();
        ui->qcp_motor_tmp_1->replot();
        ui->qcp_motor_spd_1->replot();


    }
    // make key axis range scroll with the data (at a constant range size of 8):




}

void MainWindow::on_doubleSpinBox_moto_test_time_valueChanged(double arg1)
{
    m_timer_update_.setInterval(arg1*1000);
}
