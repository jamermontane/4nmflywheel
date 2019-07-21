#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>
#include <QDebug>

class MotorDriver : public QObject
{
    Q_OBJECT
public:
    explicit MotorDriver(QObject *parent = 0);

    ~MotorDriver(){
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


    QByteArray calSpdData(QString spd);
    QByteArray calSpdData2(QString spd,QString acc);
    QByteArray calTorData(QString tor);

    void setPortName(QString str){
        port_name = str;
    }

    void setBaudRate(QString baud){
        baud_rate = baud;
    }

signals:
    void sendErrText(QString);

    void sendMotorSpd(double);
    void sendMotorCur(double);
    void sendMotorTmp(double);
    void sendMotorStatus(uchar);


public slots:
    void ctlMotorSpd2(double spd = 0,double acc = 0);
    void ctlMotorSpd(double spd = 0);
    void ctlMotorTor(double tor = 0);
    void getMotorData();
    void resolveDataFromSerialport();
    bool init();
private:

    QSerialPort*    serial_port_;
    bool            isInit;
    QString         port_name;
    QString         baud_rate;
    QByteArray      recv_data_buf;
    /*
     * //----------flywheel 4Nm-----------
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
    //----------flywheel 4Nm end-----------
    */
    //----------flywheel 50Nm start--------
    union{
        uchar   array[4];
        int32_t  spd;
    }spd_array_;
    union{
        uchar   array[4];
        int32_t  acc;
    }acc_array_;

    union recv_spd{
        qint32 spd;
        uchar   array[4];
    }recv_spd_;

    union recv_cur{
        quint32  cur;
        uchar   array[4];
    }recv_cur_;
    //----------flywheel 50Nm end----------
};

#endif // MOTORDRIVER_H
