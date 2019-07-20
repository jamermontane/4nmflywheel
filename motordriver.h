#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>
#include <QDebug>

//驱动器基类
class MotorDriver : public QObject
{
    Q_OBJECT
public:
    explicit MotorDriver(QObject *parent = 0):QObject(parent)
    {
        isInit = false;
    }

    virtual ~MotorDriver(){}

    void setInit(bool init){
        isInit = init;
    }

    bool getInit() const{
        return isInit;
    }

signals:
    void sendErrText(QString);
    void sendMotorSpd(double);
    void sendMotorCur(double);
    void sendMotorTmp(double);
    void sendMotorStatus(uchar);


public slots:
    virtual void ctlMotorSpd(double spd = 0) = 0;
    virtual void ctlMotorTor(double tor = 0) = 0;
    virtual void getMotorData() = 0;
    virtual bool init() = 0;
private:
    bool isInit;
};

//采用串口通讯的驱动器应该继承该类
class SerialDriver : public MotorDriver{

    Q_OBJECT

public:
    explicit SerialDriver(QObject *parent = 0) :MotorDriver(parent)
    {

    }

    ~SerialDriver(){
        disconnect(this->serial_port_,SIGNAL(readyRead()),this,SLOT(resolveDataFromSerialport()));
        if (serial_port_ != nullptr && serial_port_->isOpen()){
            serial_port_->close();
            delete serial_port_;
            qDebug()<<port_name<<" has close";
        }
        else if(serial_port_ != nullptr && !serial_port_->isOpen()){
            delete serial_port_;
        }
        else{
            serial_port_ = nullptr;
        }

    }



    void setPortName(QString str){
        port_name = str;
    }

    void setBaudRate(QString baud){
        baud_rate = baud;
    }

public slots:
    virtual void ctlMotorSpd(double spd = 0) = 0;
    virtual void ctlMotorTor(double tor = 0) = 0;
    virtual void getMotorData() = 0;
    virtual void resolveDataFromSerialport() = 0;
    virtual bool init();
private:
    QSerialPort*    serial_port_;
    QString         port_name;
    QString         baud_rate;
    QByteArray      recv_data_buf;


};

//型号：flywheel20
class Flywheel4NMDriver : public SerialDriver{
    Q_OBJECT
public:
    explicit Flywheel4NMDriver(QObject *parent = 0);
    QByteArray calSpdData(QString spd);
    QByteArray calTorData(QString tor);

public slots:
    virtual void ctlMotorSpd(double spd = 0) override;
    virtual void ctlMotorTor(double tor = 0) override;
    virtual void getMotorData()              override;
    virtual void resolveDataFromSerialport() override;

private:

    union spd_array{
        uchar   array[2];
        qint16  spd;
    }spd_array_;

    union tor_array{
        uchar   array[2];
        qint16  torque;
    }tor_array_;

    union recv_spd{
        qint16  spd;
        uchar   array[2];
    }recv_spd_;

    union recv_cur{
        quint16  cur;
        uchar   array[2];
    }recv_cur_;

};

//型号：flywheel50
class Flywheel50Driver : public SerialDriver{

    Q_OBJECT

public:
    explicit Flywheel50Driver(QObject *parent = 0):SerialDriver(parent){

    }

    QByteArray calSpdData(QString spd);
    QByteArray calTorData(QString tor);

public slots:
    virtual void ctlMotorSpd(double spd = 0) override;
    virtual void ctlMotorTor(double tor = 0) override;
    virtual void getMotorData()              override;
    virtual void resolveDataFromSerialport() override;

private:

};

#endif // MOTORDRIVER_H
