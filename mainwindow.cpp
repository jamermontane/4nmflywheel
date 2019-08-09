#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_sys_status_1_(false)
{
    ui->setupUi(this);
    qRegisterMetaType<QVector<QVector<QString> >>("QVector<QVector<QString> >");
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    qDebug()<<"MAIN THREAD:"<<QThread::currentThreadId();
    initCombox();
    initDriver1();
    initQCustomPlot1();
    initSql();
    initReport();

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
    p_motor_thread1_ = new QThread;
    p_motor1_ = new Motor;
    p_motor1_->setChannel("MOTOR1");
    p_motor1_->setAccelerate(50);
    p_motor1_->moveToThread(p_motor_thread1_);
    connect(ui->pushButton_auto_test_with_air_power_1,SIGNAL(clicked(bool)),p_motor1_,SLOT(initTestModeWithAir()));
    p_motor_thread1_->start();

    //driver thread 1
    p_driver1_ = new MotorDriver;
    p_driver_thread1_ = new QThread;
    p_driver1_->moveToThread(p_driver_thread1_);

    connect(p_driver_thread1_,&QThread::finished,p_driver1_,&MotorDriver::deleteLater);
    connect(p_driver_thread1_,&QThread::finished,p_driver_thread1_,&QThread::deleteLater);
    connect(p_driver1_,&MotorDriver::sendErrText,this,&MainWindow::logMsg);
    connect(p_driver_thread1_,&QThread::started,p_driver1_,&MotorDriver::init);
    //seems that should connect driver obj later
    p_driver1_->setPortName("COM1");
    p_driver1_->setBaudRate("115200");
    p_driver_thread1_->start();

    //get data timer init

    connect(&m_timer_get_data_,&QTimer::timeout,p_driver1_,&MotorDriver::getMotorData);

    //control init
//    connect(&p_motor1_,&Motor::sendMoTorSpd,p_driver1_,&MotorDriver::ctlMotorSpd());
    connect(p_motor1_,SIGNAL(sendMoTorSpd(double,double)),p_driver1_,SLOT(ctlMotorSpd2(double,double)));
    connect(p_motor1_,&Motor::sendMoTorTor,p_driver1_,&MotorDriver::ctlMotorTor);
    connect(p_driver1_,&MotorDriver::sendMotorSpd,p_motor1_,&Motor::setSpeed);
    connect(p_driver1_,&MotorDriver::sendMotorCur,p_motor1_,&Motor::setCurrent);
    connect(p_driver1_,&MotorDriver::sendMotorTmp,p_motor1_,&Motor::setTemperature);
    //采样间隔改了的话，记得改这个
    p_motor1_->setCurrentInterval(m_timer_get_data_.interval() * 1000);
    return true;
}

//初始化数据库
void MainWindow::initSql()
{
    p_sql_ = new SqlDataBase;
    p_sql_thread_ = new QThread;
    p_sql_->moveToThread(p_sql_thread_);
    p_sql_->sqlInit();

    connect(p_sql_thread_,&QThread::finished,p_sql_,&SqlDataBase::deleteLater);
    connect(p_sql_thread_,&QThread::finished,p_sql_thread_,&QThread::deleteLater);

    connect(this,&MainWindow::sendToSqlDB,p_sql_,&SqlDataBase::insertIntoDB);

    connect(p_sql_,&SqlDataBase::sendErrorText,this,&MainWindow::logMsg);
    connect(p_sql_,&SqlDataBase::emitExpData,this,&MainWindow::updataSqlTableView);
    p_sql_thread_->start();
}

void MainWindow::initReport()
{
    p_repoter_ = new QMotorReport;
    p_repoter_thread_ = new QThread;
    p_repoter_->moveToThread(p_repoter_thread_);

    connect(p_sql_thread_,&QThread::finished,p_repoter_,&QMotorReport::deleteLater);
    connect(p_sql_thread_,&QThread::finished,p_repoter_thread_,&QThread::deleteLater);

    //将接受数据与处理函数连接
//    connect(p_sql_,&SqlDataBase::emitExpData,p_repoter_,&QMotorReport::getDataFromSql);
    connect(this,&MainWindow::getLastExpData,p_sql_,&SqlDataBase::getLastExpData);
    connect(p_sql_,&SqlDataBase::emitLastExpData,p_repoter_,&QMotorReport::getDataFromSql);
    connect(p_repoter_,&QMotorReport::logMsg,this,&MainWindow::logMsg);
    p_repoter_thread_->start();
}

void MainWindow::initCombox()
{
    ui->comboBox_motor_test_mode_1->addItem("速度模式");
    ui->comboBox_motor_test_mode_1->addItem("力矩模式");
    ui->comboBox_motor_test_mode_1->addItem("斜坡模式");
}

//更新电机1---波形显示
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
        //检查各个飞轮的状态
        if (p_motor1_->getIsRunning()){
            on_pushButton_single_test_mode_1_clicked();
        }
        //检查飞轮二。。。


        m_sys_status_1_ = false;
        ui->pushButton_system_power_1->setText("启动");

        if (p_motor1_->getIsRunning()){
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
        if(!p_motor1_->getIsRunning() && !this_mode_running){
            switch(ui->comboBox_motor_test_mode_1->currentIndex()){
            case 0:
                p_motor1_->setSetSpeed(ui->doubleSpinBox_motor_test_spd_1->text().toDouble());
                p_motor1_->setAccelerate(ui->doubleSpinBox_motor_test_acc->text().toDouble());
                break;
            case 1: p_motor1_->setSetTorque(ui->doubleSpinBox_motor_test_spd_1->text().toDouble());break;
            case 2:
                p_motor1_->setSetSpeed(0);
                p_motor1_->initXpMode(ui->doubleSpinBox_motor_test_spd_1->text().toDouble()
                                     ,ui->doubleSpinBox_motor_test_acc->text().toDouble());
                connect(&m_timer_get_data_,SIGNAL(timeout()),p_motor1_,SLOT(calXpMode()));
                xp_mode_running = true;
                p_motor1_->setXpStatus(true);
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

            p_motor1_->setIsRunning(true);
            m_timer_get_data_.start();
            this_mode_running = true;
            ui->pushButton_single_test_mode_1->setText("停止");
            ui->comboBox_motor_test_mode_1->setEnabled(false);
        }
        else{
            if (this_mode_running){
                if (xp_mode_running){
                    disconnect(&m_timer_get_data_,SIGNAL(timeout()),p_motor1_,SLOT(calXpMode()));
                    xp_mode_running = false;
                    p_motor1_->setXpStatus(false);
                }
                p_motor1_->setIsRunning(false);
                p_motor1_->setSetSpeed(0);
                m_timer_get_data_.stop();
                this_mode_running = false;
                ui->pushButton_single_test_mode_1->setText("启动");
                ui->comboBox_motor_test_mode_1->setEnabled(true);
            }
            else{
                QMessageBox::warning(this,"警告","其它模式运行中。");
            }
        }
    }
}

//为sql查询生成一个vector，为了方便信号槽数据的传递。
QVector<QString> MainWindow::makeSqlVector(Motor &motor)
{
    static QVector<QString> res;
    res.clear();
    res.append(motor.getChannel());
    res.append(motor.getExpId());
    res.append(QString::number(motor.getID()));
    res.append(QString::number(motor.getVoltage()));
    res.append(QString::number(motor.getCurrent()));
    res.append(QString::number(motor.getSetSpeed()));
    res.append(QString::number(motor.getSpeed()));
    res.append(QString::number(motor.getSetTorque()));
    res.append(QString::number(motor.getTorque()));
    res.append(QString::number(motor.getWate()));
    res.append(QString::number(motor.getAngularMomentum()));
    res.append(QString::number(motor.getAngularMomentumConst()));
    res.append(QString::number(motor.getAngularMomentumDynamic()));
    res.append(QString::number(motor.getFlywheelMode()));
    res.append(ui->lineEdit_vacuum->text());
    res.append(ui->lineEdit_exp_address->text());
    res.append(QString::number(motor.getActCur()));
    return res;
}
//更新电机1 总控制函数
void MainWindow::updateMotor()
{
    if (p_motor1_->getIsRunning()){
        //斜坡模式不用发

        if (!p_motor1_->getXpStatus() && !p_motor1_->getNoAirMode()){
            p_motor1_->setSetSpeed(ui->doubleSpinBox_motor_test_spd_1->text().toDouble());
        }


        //更新显示界面
        updateMotor1Display();
        refreshCustomPlotData1();
        //更新数据库

        emit sendToSqlDB(ui->lineEdit_exp_name_1->text(),ui->lineEdit_exp_usr_name_1->text(),
                         ui->lineEdit_exp_fw_id_1->text(),makeSqlVector(*p_motor1_));

    }

    //...etc motor
}
//更新电机1---数据显示
void MainWindow::updateMotor1Display()
{
    //update lineedit
    ui->lineEdit_motor_set_spd_1->setText(QString::number(p_motor1_->getSetSpeed()));
    ui->lineEdit_motor_act_spd_1->setText(QString::number(p_motor1_->getSpeed()));
    ui->lineEdit_motor_cur_1->setText(QString::number(p_motor1_->getCurrent()));
    ui->lineEdit_motor_act_tor_1->setText(QString::number(p_motor1_->getTorque()));
    ui->lineEdit_motor_jdl_1->setText(QString::number(p_motor1_->getAngularMomentum()));
    ui->lineEdit_motor_jdl_czpc_1->setText(QString::number(p_motor1_->getAngularMomentumConst()));
    ui->lineEdit_motor_jdl_dtpc_1->setText(QString::number(p_motor1_->getAngularMomentumDynamic()));
}
//转速改变响应函数
void MainWindow::on_doubleSpinBox_motor_test_spd_1_editingFinished()
{
    if(p_motor1_->getIsRunning()){
        switch(ui->comboBox_motor_test_mode_1->currentIndex()){
        case 0: p_motor1_->setSetSpeed(ui->doubleSpinBox_motor_test_spd_1->text().toDouble()); break;
        case 1: p_motor1_->setSetTorque(ui->doubleSpinBox_motor_test_spd_1->text().toDouble());break;
        default:
            break;
        }
    }
}
//测试模式下拉框改变响应函数
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
        tmpContainer.push_back(p_motor1_->getTemperature());
        curContainer.push_back(p_motor1_->getCurrent());
        spdContainer.push_back(p_motor1_->getSpeed());
        setSpdContainer.push_back(p_motor1_->getSetSpeed());

        ui->qcp_motor_tmp_1->graph(0)->setData(keyContainer,tmpContainer,true);
        ui->qcp_motor_cur_1->graph(0)->setData(keyContainer,curContainer,true);
        ui->qcp_motor_spd_1->graph(0)->setData(keyContainer,spdContainer,true);
        ui->qcp_motor_spd_1->graph(1)->setData(keyContainer,setSpdContainer,true);
        lastPointKey = key;
        if (keyContainer.size() >= 300){
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

        ui->qcp_motor_cur_1->yAxis->setRange(*std::min_element(curContainer.begin(),curContainer.end()),
                                             *std::max_element(curContainer.begin(),curContainer.end()));
        ui->qcp_motor_spd_1->yAxis->setRange(*std::min_element(spdContainer.begin(),spdContainer.end()),
                                             *std::max_element(spdContainer.begin(),spdContainer.end()));
        ui->qcp_motor_tmp_1->yAxis->setRange(*std::min_element(tmpContainer.begin(),tmpContainer.end()),
                                             *std::max_element(tmpContainer.begin(),tmpContainer.end()));

        ui->qcp_motor_cur_1->xAxis->setRange(key, 20, Qt::AlignRight);
        ui->qcp_motor_spd_1->xAxis->setRange(key, 20, Qt::AlignRight);
        ui->qcp_motor_tmp_1->xAxis->setRange(key, 20, Qt::AlignRight);

        ui->qcp_motor_cur_1->replot();
        ui->qcp_motor_tmp_1->replot();
        ui->qcp_motor_spd_1->replot();


    }
    // make key axis range scroll with the data (at a constant range size of 8):




}
//设置界面数据更新间隔
void MainWindow::on_doubleSpinBox_moto_test_time_valueChanged(double arg1)
{
    m_timer_update_.setInterval(arg1*1000);
}

//非真空性能测试响应函数
void MainWindow::on_pushButton_auto_test_with_air_power_1_clicked()
{
    static bool this_mode_running = false;
    if (!m_sys_status_1_){
        QMessageBox::warning(this,"电源未打开","电源未打开,请开启总电源！");
    }
    else{
        if(!p_motor1_->getIsRunning() && !this_mode_running && ui->checkBox_noair_1->isChecked()){
            //如果选择了该模式，启动测试流程
            p_motor1_->setIsRunning(true);
            this_mode_running = true;
            m_timer_get_data_.start();
//            p_motor1_->initTestModeWithAir();
            ui->pushButton_auto_test_with_air_power_1->setText("停止");
            ui->statusBar->showMessage("非真空性能测试运行中！");
            //disconnect(ui->pushButton_auto_test_with_air_power_1,SIGNAL(clicked(bool)),p_motor1_,SLOT(initTestModeWithAir()));
            connect(p_motor1_,SIGNAL(airTestEnd()),this,SLOT(on_pushButton_auto_test_with_air_power_1_clicked()));
        }
        else if (this_mode_running){
            p_motor1_->setSetSpeed(0);
            p_motor1_->setIsRunning(false);
            this_mode_running = false;
            m_timer_get_data_.stop();
            ui->pushButton_auto_test_with_air_power_1->setText("启动");
            ui->statusBar->showMessage("非真空性能测试结束！",5000);
            QMessageBox::warning(this,"完成","非真空性能测试完成！");
            //connect(ui->pushButton_auto_test_with_air_power_1,SIGNAL(clicked(bool)),p_motor1_,SLOT(initTestModeWithAir()));
            disconnect(p_motor1_,SIGNAL(airTestEnd()),this,SLOT(on_pushButton_auto_test_with_air_power_1_clicked()));

        }
        else{
            QMessageBox::warning(this,"警告","运行失败，请检查当前状态。");
        }
    }
}

void MainWindow::on_pushButton_sql_query_clicked()
{
    tab_num_ = 0;
    p_sql_->getExpDataFromSqlDB(ui->lineEdit_sql_motor_id->text(),ui->lineEdit_sql_exp_id->text()
                                ,ui->lineEdit_sql_motor_mode->text());
}

void MainWindow::updataSqlTableView(QVector<QVector<QString> > res)
{
    static QStandardItemModel  *model = nullptr;
    if (model == nullptr){
        model = new QStandardItemModel();
    }
    if (tab_num_ == 0){
        model->clear();
        model->setColumnCount(20);
        model->setHeaderData(0,Qt::Horizontal,"实验ID");
        model->setHeaderData(1,Qt::Horizontal,"实验名称");
        model->setHeaderData(2,Qt::Horizontal,"人员名称");
        model->setHeaderData(3,Qt::Horizontal,"实验编号");
        model->setHeaderData(4,Qt::Horizontal,"飞轮编号");
        model->setHeaderData(5,Qt::Horizontal,"飞轮电压");
        model->setHeaderData(6,Qt::Horizontal,"飞轮电流");
        model->setHeaderData(7,Qt::Horizontal,"飞轮设置速度");
        model->setHeaderData(8,Qt::Horizontal,"飞轮实际速度");
        model->setHeaderData(9,Qt::Horizontal,"飞轮设置力矩");
        model->setHeaderData(10,Qt::Horizontal,"飞轮实际力矩");
        model->setHeaderData(11,Qt::Horizontal,"飞轮功率");
        model->setHeaderData(12,Qt::Horizontal,"飞轮角动量");
        model->setHeaderData(13,Qt::Horizontal,"飞轮角动量常值偏差");
        model->setHeaderData(14,Qt::Horizontal,"飞轮角动量动态偏差");
        model->setHeaderData(15,Qt::Horizontal,"飞轮运行模式");
        model->setHeaderData(16,Qt::Horizontal,"真空度");
        model->setHeaderData(17,Qt::Horizontal,"实验地点");
        model->setHeaderData(18,Qt::Horizontal,"采集电流");
        model->setHeaderData(19,Qt::Horizontal,"运行时间");
        ui->tableView_sql->setModel(model);
    }

    for (QVector<QString> &query_dispResult_:res){
        for (int j=0;j<20;++j){
        model->setItem(tab_num_,j,new QStandardItem(query_dispResult_.at(j)));
        //设置字符位置
        model->item(tab_num_,j)->setTextAlignment(Qt::AlignCenter);
        }
        tab_num_++;
        if (tab_num_ > 100){
            break;
        }
    }
}

void MainWindow::on_pushButton_make_report_clicked()
{
    emit getLastExpData(ui->lineEdit_sql_motor_id->text(),ui->lineEdit_sql_motor_mode->text());
}
