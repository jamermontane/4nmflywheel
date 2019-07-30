#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>
#include <QQueue>
#include <qmath.h>
#include <algorithm>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QDateTime>

//计算常数定义
static const double J_ = 0.0064;
static const double PI_ = 3.1415926;

//模式编号定义
#define FLYWHEEL_MODE_STOP  0
#define FLYWHEEL_MODE_XP    1

/*
 * 电机类
 * 如果后期发现时间效率达不到要求，部分计算算法可以优化
 *
 * @author LYC
 * @time 2019/07/15
 */
class MotorBasic : public QObject{

    Q_OBJECT

public:
    MotorBasic(): id_(0),vol_(12),cur_(0),
        spd_(0),acc_(0.1),setSpd_(0),torque_(0),
        setTorque_(0),isRunning_(false),channel_("")
    {

    }


    ~MotorBasic(){

    }
signals:
    void sendMoTorSpd(double);
    void sendMoTorSpd(double,double);
    void sendMoTorTor(double);
    void spdChanged(double);
    void sendErrorText(QString);
public slots:
    void setSetSpeed(const double spd){
        this->setSpd_ = spd;
        emit sendMoTorSpd(setSpd_,acc_);
    }
    void setSpeed(const double spd){
        this->spd_ = spd;
        emit spdChanged(spd);
    }
    void setCurrent(const double cur){
        this->cur_ = cur;
    }
    void setVoltage(const double vol){
        this->vol_ = vol;
    }
    void setAccelerate(const double acc){
        this->acc_ = acc;
        emit sendMoTorSpd(setSpd_,acc_);
    }
    void setId(const uint id){
        this->id_ = id;
    }
    void setIsRunning(const bool isRunning){
        this->isRunning_ = isRunning;
    }

    void setTorque(const double tor){
        this->torque_ = tor;

    }
    void setSetTorque(const double setTor){
        this->setTorque_ = setTor;
        emit sendMoTorTor(setTor);
    }

public:
    //设置速度
    double getSetSpeed() const{
        return this->setSpd_;
    }

    //实时速度
    double getSpeed() const{
        return this->spd_;
    }

    //电流
    double getCurrent() const{
        return this->cur_;
    }

    //电压
    double getVoltage() const{
        return this->vol_;
    }

    //ID

    uint getID() const{
        return this->id_;
    }
    //ACC
    double getAccelerate() const{
        return this->acc_;
    }

    //isrunning
    bool getIsRunning(){
        return isRunning_;
    }



    //torque
    double getSetTorque() const{
        return this->setTorque_;
    }

    double getTorque() const{
        return this->torque_;
    }
    //设置通道号,保存数据库时用
    QString getChannel()const{
        return this->channel_;
    }

    void setChannel(QString str){
        this->channel_ = str;
    }



private:
    uint id_;
    double vol_;
    double cur_;
    double spd_;
    double acc_;
    double setSpd_;

    double torque_;
    double setTorque_;

    bool isRunning_;
    QString channel_;


};

class Motor : public MotorBasic{

    Q_OBJECT

public:
    Motor():last_ten_cur_(0),
            last_ten_vol_(12),
            last_ten_spd_(0)
    {
        initXpMode(0,0);
        connect(this,SIGNAL(spdChanged(double)),this,SLOT(setLastTen(double)));

    }
    ~Motor(){

    }


    //得到温度
    double getTemperature() const{
        return this->temperature_;
    }
    //得到功率
    double getWate() const{
        return this->wate_;
    }

    //得到角动量
    double getAngularMomentum() const{
        return this->angular_momentum_;
    }
    //得到角动量常值偏差
    double getAngularMomentumConst() const{
        return this->angular_momentum_const_d;
    }
    //得到角动量动态偏差
    double getAngularMomentumDynamic() const{
        return this->angular_momentum_dynamic_d;
    }
    //得到反作用力矩
    double getReactionMoment_() const{
        return this->reaction_moment_;
    }
    //斜坡模式
    void initXpMode(double espd,double interval){
        this->xp_end_spd_ = espd;
        this->xp_spd_interval_ = interval;
        this->setAccelerate(interval);
        this->xp_status_ = false;
    }
    void setXpStatus(bool status){
        this->xp_status_ = status;
    }
    bool getXpStatus() const{
        return this->xp_status_;
    }
    bool getNoAirMode() const{
        return this->is_noair_init_;
    }
    QString getExpId() const{
        return this->exp_id_;
    }

public slots:
    //get last ten
    void setLastTen(double spd){
//        qDebug()<<"motor thread:"<<QThread::currentThreadId();
        if (last_ten_vol_queue_.size() <10){
            last_ten_vol_queue_.push_back(this->getVoltage());
        }
        else{
            last_ten_vol_queue_.pop_front();
            last_ten_vol_queue_.push_back(this->getVoltage());
        }

        if (last_ten_cur_queue_.size() <10){
            last_ten_cur_queue_.push_back(this->getCurrent());
        }
        else{
            last_ten_cur_queue_.pop_front();
            last_ten_cur_queue_.push_back(this->getCurrent());
        }

        if (last_ten_spd_queue_.size() <10){
            last_ten_spd_queue_.push_back(spd);
        }
        else{
            last_ten_spd_queue_.pop_front();
            last_ten_spd_queue_.push_back(spd);
        }
        last_ten_cur_ = 0;
        for(double &t:last_ten_cur_queue_){
            last_ten_cur_ += t;
        }
        last_ten_cur_ /= last_ten_cur_queue_.size();

        last_ten_spd_ = 0;
        for(double &t:last_ten_spd_queue_){
            last_ten_spd_ += t;
        }
        last_ten_spd_ /= last_ten_spd_queue_.size();

        last_ten_vol_ = 0;
        for(double &t:last_ten_vol_queue_){
            last_ten_vol_ += t;
        }
        last_ten_vol_ /= last_ten_vol_queue_.size();
        //cal
        setAngularMomentum();
        setAngularMomentumConst();
        setAngularMomentumDynamic();
        setReactionMoment();
    }

    //设置温度
    void setTemperature(const double tmp){
        temperature_ =  4050/(qLn(((7500*tmp)/(256-tmp))) + 4.98) - 273;
    }
    //设置功率
    void setWate(){
        wate_ = last_ten_cur_ * last_ten_vol_;
    }

    //设置角动量
    void setAngularMomentum(){
        this->angular_momentum_ = 9.55*J_*this->last_ten_spd_;
    }
    //设置角动量常值偏差
    void setAngularMomentumConst(){

        this->angular_momentum_const_d = 0.00428 * qAbs(this->getSpeed() - this->last_ten_spd_);
    }
    //设置角动量动态偏差
    void setAngularMomentumDynamic(){
        double tmp_max = *(std::max_element(last_ten_spd_queue_.begin(),last_ten_spd_queue_.end()));

        this->angular_momentum_dynamic_d = 0.00428 * qAbs(tmp_max - last_ten_spd_);
    }
    //设置反作用力矩
    void setReactionMoment(){
        if (last_ten_spd_queue_.size() <= 2) return;
        double last_spd = *last_ten_spd_queue_.rbegin();
        double last_last__spd = *(++last_ten_spd_queue_.rbegin());
        double current_reaction = (last_spd - last_last__spd)* J_ * 2 * PI_ / (60 * current_interval) ;
        //求平均10个
        if (last_ten_reaction_queue_.size() <10){
            last_ten_reaction_queue_.push_back(current_reaction);
        }
        else{
            last_ten_reaction_queue_.pop_front();
            last_ten_reaction_queue_.push_back(current_reaction);
        }

        for(double &t:last_ten_reaction_queue_){
            last_ten_reaction_ += t;
        }
        this->reaction_moment_ = last_ten_reaction_ / last_ten_reaction_queue_.size();
    }
    //设置当前间隔
    void setCurrentInterval(const double interval){
        this->current_interval = interval;
    }

    //斜坡模式
    void calXpMode(){
        if (qAbs(qAbs(getSpeed()) - qAbs(xp_end_spd_)) > qAbs(xp_spd_interval_)){
            double ctl_spd = getSpeed()+xp_spd_interval_;
            if (ctl_spd < -6050){

                setSetSpeed(-6050);
            }
            else if (ctl_spd > 6050){
                setSetSpeed(6050);
            }
            else
                setSetSpeed(ctl_spd);
        }
        else{
            setSetSpeed(xp_end_spd_);
        }
    }

    void initTestModeWithAir(){
        exp_id_ = QDateTime::currentDateTime().toString("yyMMddhhmmss");
        if (is_noair_init_){
            return;
        }
        noair_test_containor_.clear();
        noair_test_containor_.append(0);
        noair_test_containor_.append(100);
        noair_test_containor_.append(500);
        noair_test_containor_.append(1000);
        noair_test_containor_.append(1500);
        noair_test_containor_.append(2000);
        noair_test_containor_.append(2500);
        noair_test_containor_.append(3000);
        noair_test_containor_.append(-3000);
        noair_test_containor_.append(-2500);
        noair_test_containor_.append(-2000);
        noair_test_containor_.append(-1500);
        noair_test_containor_.append(-1000);
        noair_test_containor_.append(-500);
        noair_test_containor_.append(0);
        is_noair_init_ = true;
        if(p_timer_auto_test_ == nullptr){
            p_timer_auto_test_ = new QTimer;
        }
        connect(this,SIGNAL(spdChanged(double)),this,SLOT(runWithAirMode(double)));
        connect(p_timer_auto_test_,SIGNAL(timeout()),this,SLOT(nxtWithAirModeTestSpd()));
        p_timer_auto_test_->setInterval(5000);
        is_timer_started = false;
        this->setSetSpeed(0);
    }

    void runWithAirMode(double spd){
        if(!getIsRunning()){
            exp_id_.clear();
            is_noair_init_ = false;
            p_timer_auto_test_->stop();
            disconnect(this,SIGNAL(spdChanged(double)),this,SLOT(runWithAirMode(double)));
            disconnect(p_timer_auto_test_,SIGNAL(timeout()),this,SLOT(nxtWithAirModeTestSpd()));
            return;
        }
        if (!noair_test_containor_.empty()){
            this->setSetSpeed(noair_test_containor_.front());
            if (abs(spd - noair_test_containor_.front()) < 10){
                if (!is_timer_started){
                    p_timer_auto_test_->start();
                    is_timer_started = true;
                }
            }
        }
    }

    void nxtWithAirModeTestSpd(){
        if(!getIsRunning()){
            exp_id_.clear();
            is_noair_init_ = false;
            p_timer_auto_test_->stop();
            disconnect(this,SIGNAL(spdChanged(double)),this,SLOT(runWithAirMode(double)));
            disconnect(p_timer_auto_test_,SIGNAL(timeout()),this,SLOT(nxtWithAirModeTestSpd()));
            return;
        }
        is_timer_started = false;
        p_timer_auto_test_->stop();
        noair_test_containor_.pop_front();
        if (noair_test_containor_.empty()){
            is_noair_init_ = false;
            disconnect(this,SIGNAL(spdChanged(double)),this,SLOT(runWithAirMode(double)));
            disconnect(p_timer_auto_test_,SIGNAL(timeout()),this,SLOT(nxtWithAirModeTestSpd()));
            emit airTestEnd();
        }
    }


signals:
    void airTestEnd();
private:
    double temperature_;
    double wate_;

    //存放最近10个电流
    QQueue<double> last_ten_cur_queue_;
    double last_ten_cur_;
    //存放最近10个电压
    QQueue<double> last_ten_vol_queue_;
    double last_ten_vol_;
    //存放最近10个转速
    QQueue<double> last_ten_spd_queue_;
    double last_ten_spd_;
    //反作用力矩
    QQueue<double> last_ten_reaction_queue_;
    double last_ten_reaction_;

    //角动量
    double angular_momentum_;
    //角动量常值偏差
    double angular_momentum_const_d;
    //角动量动态偏差
    double angular_momentum_dynamic_d;
    //反作用力矩
    double reaction_moment_;
    //当前运行间隔
    double current_interval = 0.5;

    //斜坡模式
    bool xp_status_;
    double xp_end_spd_;
    double xp_spd_interval_;

    //模式代码

    //非真空测试
    QList<double> noair_test_containor_;
    bool is_noair_init_ = false;
    bool is_timer_started = false;
    QTimer* p_timer_auto_test_ = nullptr;


    QString exp_id_; //用来记录一键测试的编号（用以区分不同实验）

};

#endif // MOTOR_H
