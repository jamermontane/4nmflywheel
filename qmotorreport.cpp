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
    setExpInfomation(*res.begin());
    for (int i =1;i < res.size();++i){
        const QVector<QString>& data = res.at(i);
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
        this->flywheel_act_cur_.push_back(data[18].toDouble());
    }
    qDebug()<<"report: data recv complete!";
}

void QMotorReport::initExpData()
{
    //提取setspd
    QSet<double> t_setspd;
    for (double &setspd:flywheel_setSpd_){
        t_setspd.insert(setspd);
    }

    m_test_unit_setspd_.clear();
    for (auto &set_spd:t_setspd){
        m_test_unit_setspd_.push_back(set_spd);
    }


}

void QMotorReport::calExpDataSetSpd()
{
    while (!m_test_unit_setspd_.empty()){
        double current_set_spd = m_test_unit_setspd_.front();
        m_test_unit_setspd_.pop_front();

        //每一次提取当前需要测试的数据
        QVector<double> need_test_spd;
        QVector<double> jdl_const;
        QVector<double> jdl_dynamic;
        for (int i =0;i < flywheel_setSpd_.size();++i){
            if (flywheel_setSpd_.at(i) == current_set_spd){
                need_test_spd.push_back(flywheel_spd_.at(i));
                jdl_const.
            }
        }

        //处理需要测试的数据
        bool start_test = false;
        double avg_spd = 0;
        uint data_num = 0;
        for (double &spd:need_test_spd){
            if (start_test == false && spd < current_set_spd){
                continue;
            }
            else{
                start_test = true;
            }
            avg_spd += spd;
            data_num++;
        }
        //第一个平均转速
        m_result_spd_.push_back(avg_spd / data_num);

    }
}
