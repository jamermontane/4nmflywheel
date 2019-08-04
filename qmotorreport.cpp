#include "qmotorreport.h"

QMotorReport::QMotorReport(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QVector<QVector<QString> >>("QVector<QVector<QString> >");
}

void QMotorReport::setExpInfomation(QVector<QString> &data)
{
    //数据库改变的话，size = 20 也需要改变
    if (data.size() != 20) return;

    this->exp_id_           = data[0];
    this->exp_name_         = data[1];
    this->usr_name_         = data[2];
    this->exp_no_           = data[3];
    this->flywheel_no_      = data[4];
    this->flywheel_vol_.push_back(data[5].toDouble());
    this->flywheel_cur_.push_back(data[6].toDouble());
    this->flywheel_setSpd_.push_back(data[7].toDouble());
    this->flywheel_spd_.push_back(data[8].toDouble());
    this->flywheel_setTor_.push_back(data[9].toDouble());
    this->flywheel_tor_.push_back(data[10].toDouble());
    this->flywheel_wate_.push_back( data[11].toDouble());
    this->flywheel_JDL_.push_back( data[12].toDouble());
    this->flywheel_JDL_dynamic_.push_back(data[13].toDouble());
    this->flywheel_JDL_const_.push_back(data[14].toDouble());
    this->flywheel_mode_ =data[15];
    this->exp_vacuum_       = data[16];
    this->exp_address_      = data[17];
    this->flywheel_act_cur_.push_back(data[18].toDouble());
    this->exp_time_         = data[19];
}

void QMotorReport::getDataFromSql(QVector<QVector<QString> > res)
{
    if (res.empty()) return;
    QVector<QString> data = res[0];
    setExpInfomation(data);


}
