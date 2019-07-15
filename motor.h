#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>

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
public slots:
    void setSetSpeed(const double spd){
        this->setSpd_ = spd;
        emit sendMoTorSpd(setSpd_);
    }
    void setSpeed(const double spd){
        this->spd_ = spd;
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
    void setTemperature(const double tmp){
        this->tempurature_ = tmp;
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

    //tmp
    double getTemperature() const{
        return this->tempurature_;
    }

    //torque
    double getSetTorque() const{
        return this->setTorque_;
    }

    double getTorque() const{
        return this->torque_;
    }
    double getWate() const{
        return wate_;
    }
private:
    uint id_;
    double vol_;
    double cur_;
    double spd_;
    double acc_;
    double setSpd_;
    double tempurature_;
    double torque_;
    double setTorque_;
    double wate_;
    bool isRunning_;



};

class Motor : public MotorBasic{
public:
    Motor(){

    }
    ~Motor(){

    }

private:

};

#endif // MOTOR_H
