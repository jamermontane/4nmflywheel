#ifndef QMOTORREPORT_H
#define QMOTORREPORT_H

#include <QObject>
#include <qword.h>
#include <qstring.h>

class QMotorReport : public QObject
{
    Q_OBJECT
public:
    explicit QMotorReport(QObject *parent = 0);
    void setExpInfomation(QVector<QString>&);
signals:

public slots:
    void getDataFromSql(QVector<QVector<QString> > res);

private:
    //实验相关参数
    QString             exp_id_;
    QString             exp_name_;
    QString             usr_name_;
    QString             exp_no_;
    QString             exp_time_;
    QString             exp_address_;
    QString             exp_vacuum_;
    QString             flywheel_no_;
    QString             flywheel_mode_;
    QVector<double>     flywheel_act_cur_;
    QVector<double>     flywheel_vol_;
    QVector<double>     flywheel_cur_;
    QVector<double>     flywheel_setSpd_;
    QVector<double>     flywheel_spd_;
    QVector<double>     flywheel_setTor_;
    QVector<double>     flywheel_tor_;
    QVector<double>     flywheel_wate_;
    QVector<double>     flywheel_JDL_;
    QVector<double>     flywheel_JDL_const_;
    QVector<double>     flywheel_JDL_dynamic_;

    //需要计算的值
};

#endif // QMOTORREPORT_H
