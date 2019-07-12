#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>


class MotorDriver : public QObject
{
    Q_OBJECT
public:
    explicit MotorDriver(QObject *parent = 0);

    ~MotorDriver(){
        if (serial_port_.isOpen())
            serial_port_.close();

    }

    bool init(QString port_name,QString baud_rate);
    QByteArray calSpdData(QString spd);
    QByteArray calTorData(QString tor);


signals:
    void sendErrText(QString);

    void sendMotorSpd(double);
    void sendMotorCur(double);
    void sendMotorTmp(double);
    void sendMotorStatus(uchar);


public slots:
    void ctlMotorSpd(double spd = 0);
    void ctlMotorTor(double tor = 0);
    void getMotorData();

private:

    QSerialPort     serial_port_;
    bool            isInit;

    union spd_array{
        uchar          array[2];
        qint16         spd;
    }spd_array_;

    union tor_array{
        uchar          array[2];
        qint16         torque;
    }tor_array_;

};

#endif // MOTORDRIVER_H
