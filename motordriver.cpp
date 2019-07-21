#include "motordriver.h"

MotorDriver::MotorDriver(QObject *parent) : QObject(parent)
  ,isInit(false)
{
    spd_array_.spd = 0;
    /*fw 4nm
    tor_array_.torque = 0;

    */
    acc_array_.acc = 0;
    recv_spd_.spd = 0;
    serial_port_ = nullptr;


}

QByteArray MotorDriver::calSpdData(QString spd)
{
    // fw 4nm
    QByteArray spd_arr;
    spd_array_.spd = spd.toInt() * 2;
    spd_arr.resize(7);
    spd_arr[0] = 0xD3;
    spd_arr[1] = 0x00;
    spd_arr[2] = 0x00;
    spd_arr[3] = spd_array_.array[1];
    spd_arr[4] = spd_array_.array[0];
    spd_arr[5] = 0x5B;
    // cal total
    uchar t = 0x00;
    for (uint i = 1; i < 6;++i){
        t += spd_arr[i];
    }
    spd_arr[6] = t;
    return spd_arr;
}

bool MotorDriver::init()
{
    serial_port_ = new QSerialPort;
    connect(this->serial_port_,SIGNAL(readyRead()),this,SLOT(resolveDataFromSerialport()));

    if (!serial_port_->isOpen()){

        serial_port_->setPortName(port_name);
        serial_port_->setBaudRate(baud_rate.toInt());
        serial_port_->setParity(QSerialPort::OddParity);


        if(!serial_port_->open(QIODevice::ReadWrite)){

            emit sendErrText(serial_port_->errorString());
            return false;

        }

    }
    qDebug()<<port_name<<" has init";
    isInit = true;
    return true;
}

QByteArray MotorDriver::calSpdData2(QString spd, QString acc)
{
    QByteArray spd_arr;

    //flywheel 50nm
    spd_array_.spd = int(spd.toFloat()*1000);
    acc_array_.acc = int(acc.toFloat()*1000);
    spd_arr.resize(11);
    spd_arr[0] = 0x58;

    spd_arr[1] = spd_array_.array[3];
    spd_arr[2] = spd_array_.array[2];
    spd_arr[3] = spd_array_.array[1];
    spd_arr[4] = spd_array_.array[0];

    spd_arr[5] = acc_array_.array[3];
    spd_arr[6] = acc_array_.array[2];
    spd_arr[7] = acc_array_.array[1];
    spd_arr[8] = acc_array_.array[0];

    spd_arr[9] = 0x11;
    // cal total
    uchar t = 0x00;
    for (uint i = 0; i < 10;++i){
        t += spd_arr[i];
    }
    spd_arr[10] = t;

    return spd_arr;
}

QByteArray MotorDriver::calTorData(QString tor)
{
    QByteArray tor_arr;
/*
    tor_array_.torque = tor.toInt() / 0.058;
    tor_arr.resize(7);
    tor_arr[0] = 0x1C;
    tor_arr[1] = 0x00;
    tor_arr[2] = 0x00;
    tor_arr[3] = tor_array_.array[1];
    tor_arr[4] = tor_array_.array[0];
    tor_arr[5] = 0x5B;
    // cal total
    uchar t = 0x00;
    for (uint i = 1; i < 6;++i){
        t += tor_arr[i];
    }
    tor_arr[6] = t;
*/
    return tor_arr;
}

void MotorDriver::ctlMotorSpd2(double spd, double acc)
{
    if (!isInit){
        emit sendErrText(QString("driver not init"));
    }
    else{
        QByteArray spd_str = calSpdData2(QString::number(spd),QString::number(acc));
        if(serial_port_->write(spd_str) != spd_str.size()){
            emit sendErrText(QString("driver control error"));
        }
    }
}

void MotorDriver::ctlMotorSpd(double spd)
{
    if (!isInit){
        emit sendErrText(QString("driver not init"));
    }
    else{
        QByteArray spd_str = calSpdData(QString::number(spd));
        if(serial_port_->write(spd_str) != spd_str.size()){
            emit sendErrText(QString("driver control error"));
        }
    }
}

void MotorDriver::ctlMotorTor(double tor)
{
    if (!isInit){
        emit sendErrText(QString("driver not init"));
    }
    else{
        QByteArray tor_str = calTorData(QString::number(tor));
        if(serial_port_->write(tor_str) != tor_str.size()){
            emit sendErrText(QString("driver control error"));
        }
    }
}

void MotorDriver::getMotorData()
{
    /* flywheel 4nm
    QByteArray get_str;
    get_str.resize(1);
    get_str[0] = 0xa5;
    serial_port_->write(get_str);
    */

}

void MotorDriver::resolveDataFromSerialport()
{
    QByteArray tmp_data = serial_port_->readAll();
    for (int i = 0;i < tmp_data.size();++i){
        recv_data_buf.push_back(tmp_data.at(i));
        /*flywheel 4nm
        //a full frame detected
        if (recv_data_buf.size() >= 7){
            //check frame
            uchar t = 0x00;
            for (uint i = 0; i < 6;++i){
                t += recv_data_buf[i];
            }
            if (t != recv_data_buf[6]){
                recv_data_buf = recv_data_buf.mid(1);
                emit sendErrText("recv message error! chack communication!");
                return;
            }



            recv_spd_.array[1] = recv_data_buf[0];
            recv_spd_.array[0] = recv_data_buf[1];

            emit sendMotorSpd(recv_spd_.spd * 0.05);

            recv_cur_.array[1] = recv_data_buf[2];
            recv_cur_.array[0] = recv_data_buf[3];
            emit sendMotorCur(recv_cur_.cur * 0.0017);

            uchar tmp = recv_data_buf[4];
            emit sendMotorTmp(tmp);

            uchar status = recv_data_buf[5];
            emit sendMotorStatus(status);

            if (recv_data_buf.size() == 7){
                recv_data_buf.clear();
            }
            else{
                recv_data_buf = recv_data_buf.mid(7);
            }
        }
        */
        if (recv_data_buf.size() >= 11){
            //check frame
            uint8_t t = 0x00;
            for (uint i = 0; i < 10;++i){
                t += recv_data_buf[i];
            }

            if (t != uint8_t(recv_data_buf[10])){
                recv_data_buf = recv_data_buf.mid(1);
                emit sendErrText("recv message error! chack communication!");
                return;
            }
            recv_spd_.array[3] = recv_data_buf[1];
            recv_spd_.array[2] = recv_data_buf[2];
            recv_spd_.array[1] = recv_data_buf[3];
            recv_spd_.array[0] = recv_data_buf[4];

            emit sendMotorSpd(recv_spd_.spd * 0.001);

            recv_cur_.array[3] = recv_data_buf[5];
            recv_cur_.array[2] = recv_data_buf[6];
            recv_cur_.array[1] = recv_data_buf[7];
            recv_cur_.array[0] = recv_data_buf[8];
            emit sendMotorCur(recv_cur_.cur * 0.0017);

            if (recv_data_buf.size() == 11){
                recv_data_buf.clear();
            }
            else{
                recv_data_buf = recv_data_buf.mid(11);
            }
        }
    }
}

