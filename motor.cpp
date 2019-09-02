#include "motor.h"


QList<double> Motor::getNoAirTestUnit() const
{
    return air_test_containor_raw_data_;
}
//真空测试模式
void Motor::initTestModeNOAir(QVector<QString> mode_lst)
{
    exp_id_ = QDateTime::currentDateTime().toString("yyMMddhhmmss");
    for (QString &lst:mode_lst){
        all_test_mode_.push_back(lst.toInt());
    }
    //发射开始循环信号
    emit autoTestStart();
}

//将会被autoTestStart触发
void Motor::selectTestMode()
{
    if (all_test_mode_.isEmpty()){
        emit autoTestEnd();

    }
    else{
        int cur_mode = all_test_mode_.front();
        all_test_mode_.pop_front();
        switch (cur_mode) {
        case FLYWHEEL_MODE_SPD:
            initAutoTestSpdMode();
            break;
        case FLYWHEEL_MODE_HX:
            initHXMode();
            break;
        default:
            break;
        }
    }
}

void Motor::initAutoTestSpdMode()
{
    if (getIsRunning()){
        setFlywheelMode(FLYWHEEL_MODE_SPD);
        auto_test_spd_mode_containor_.clear();
        auto_test_spd_mode_containor_ = auto_test_spd_mode__containor_raw_data_;

        if(p_timer_auto_test_ == nullptr){
            p_timer_auto_test_ = new QTimer;
        }

        connect(p_timer_auto_test_,SIGNAL(timeout()),this,SLOT(nxtNoAirModeTestSpd()));
        p_timer_auto_test_->setInterval(5000);
        is_timer_started = false;
        auto_test_spd_mode_ = true;
        setSetSpeed(0);
    }
    else{
        auto_test_spd_mode_ = false;
        p_timer_auto_test_->stop();
        disconnect(p_timer_auto_test_,SIGNAL(timeout()),this,SLOT(nxtNoAirModeTestSpd()));
    }
}

void Motor::nxtNoAirModeTestSpd()
{
    if(!getIsRunning()){
        auto_test_spd_mode_ = false;
        p_timer_auto_test_->stop();
        disconnect(p_timer_auto_test_,SIGNAL(timeout()),this,SLOT(nxtNoAirModeTestSpd()));
        return;
    }
    is_timer_started = false;
    p_timer_auto_test_->stop();
    auto_test_spd_mode_containor_.pop_front();
    if (auto_test_spd_mode_containor_.empty()){
        auto_test_spd_mode_ = false;
        disconnect(p_timer_auto_test_,SIGNAL(timeout()),this,SLOT(nxtNoAirModeTestSpd()));

        //发射开始循环信号，以开启下一个模式
        emit autoTestStart();
    }
}
//控制一键测试的目标速度
void Motor::setSpdOfAutoTestSpdMode()
{
    if(!getIsRunning()){
        auto_test_spd_mode_ = false;
        p_timer_auto_test_->stop();
        disconnect(p_timer_auto_test_,SIGNAL(timeout()),this,SLOT(nxtNoAirModeTestSpd()));
        return;
    }
    if (!auto_test_spd_mode_containor_.empty()){
        this->setSetSpeed(auto_test_spd_mode_containor_.front());
        if (abs(this->getSpeed() - auto_test_spd_mode_containor_.front()) < 10){
            if (!is_timer_started){
                p_timer_auto_test_->start();
                is_timer_started = true;
            }
        }
    }
}

void Motor::initHXMode()
{
    setFlywheelMode(FLYWHEEL_MODE_HX);
    is_hx_mode_init = true;
    reach_target_ = false;
    hx_mode_target_spd_ = 4000;
}

void Motor::setSpdOfAutoTestHXMode()
{
    if(!getIsRunning()){
        is_hx_mode_init = false;
    }
    else{
        //此处应该有控制滑行模式的指令，但是50的飞轮没有
        if (!reach_target_){
            setSetSpeed(hx_mode_target_spd_);
            }
        else{
            //此处应该是开启滑行指令
            setSetSpeed(hx_mode_target_spd_);
            }
        if (abs(this->getSpeed() - hx_mode_target_spd_) < 1){
            if (!reach_target_){
                reach_target_ = true;
                hx_mode_target_spd_ = 0;
            }
            else{
                reach_target_ = false;
                is_hx_mode_init = false;
                //发射开始循环信号，以开启下一个模式
                emit autoTestStart();
            }
        }
    }
}

