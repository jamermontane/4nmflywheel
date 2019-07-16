#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>
#include <qmath.h>
#include <QQueue>


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
    MotorBasic(): id_(0),vol_(0),cur_(0),spd_(0),acc_(0.1),setSpd_(0),torque_(0),setTorque_(0),isRunning_(false)
    {

    }


    ~MotorBasic(){

    }
signals:
    void sendMoTorSpd(double);
    void sendMoTorTor(double);
    void spdChanged(double);
public slots:
    void setSetSpeed(const double spd){
        this->setSpd_ = spd;
        emit sendMoTorSpd(setSpd_);
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
        emit sendMoTorSpd(setSpd_);
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



};

class Motor : public MotorBasic{

    Q_OBJECT

public:
    Motor():last_ten_cur_(0),
            last_ten_vol_(0),
            last_ten_spd_(0)
    {

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

    //得到角动量常值偏差

    //得到角动量动态偏差

    //得到反作用力矩
public slots:
    //get last ten
    void setLastTen(double spd){
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
        for(double t:last_ten_cur_queue_){
            last_ten_cur_ += t;
        }
        last_ten_cur_ /= last_ten_cur_queue_.size();

        last_ten_spd_ = 0;
        for(double t:last_ten_spd_queue_){
            last_ten_spd_ += t;
        }
        last_ten_spd_ /= last_ten_spd_queue_.size();

        last_ten_vol_ = 0;
        for(double t:last_ten_vol_queue_){
            last_ten_vol_ += t;
        }
        last_ten_vol_ /= last_ten_vol_queue_.size();

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

    //设置角动量常值偏差

    //设置角动量动态偏差

    //设置反作用力矩
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

    //角动量
    double angular_momentum_;
    //角动量常值偏差
    double angular_momentum_const_d;
    //角动量动态偏差
    double angular_momentum_dynamic_d;
    //反作用力矩
    double reaction_moment_;

};

#endif // MOTOR_H
