#include "qmotorreport.h"

QMotorReport::QMotorReport(QObject *parent) : QObject(parent)
{

}

void QMotorReport::setExpInfomation(QVector<QString> &data)
{
    if (data.size() != 20) return;

    this->exp_id_           = data[0];
    this->exp_name_         = data[1];
    this->usr_name_         = data[2];
    this->exp_no_           = data[3];
    this->flywheel_no_      = data[4];
    this->flywheel_vol_     = data[5];
    this->flywheel_cur_     = data[6];
    this->flywheel_setSpd_  = data[7];
    this->flywheel_spd_     = data[8];
    this->flywheel_setTor_  = data[9];
    this->flywheel_tor_     = data[10];
    this->flywheel_wate_    = data[11];
    this->flywheel_JDL_     = data[12];
    this->flywheel_JDL_dynamic_= data[13];
    this->flywheel_JDL_const_ = data[14];
    this->flywheel_mode_    = data[15];
    this->exp_vacuum_       = data[16];
    this->exp_address_      = data[17];
    this->flywheel_act_cur_ = data[18];
    this->exp_time_         = data[19];
}

void QMotorReport::getDataFromSql(QVector<QVector<QString> > res)
{
    if (res.empty()) return;
    QVector<QString> data = res[0];
    setExpInfomation(data);


}
