#ifndef QMOTORREPORT_H
#define QMOTORREPORT_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QList>
#include <QVector>
#include <QSet>
#include <QDateTime>
#include <QString>
#include <QtMath>

#include <qword.h>

class QMotorReport : public QObject
{
    Q_OBJECT
public:
    explicit QMotorReport(QObject *parent = 0);
    void setExpInfomation(QVector<QString>&);

    //创建word报告
    void createWordReport();

    //计算设置速度-实际速度
    void calExpDataSetSpd();
    //计算最大反作用力矩

    //计算稳速功耗

    //计算最大功耗

    //计算最大角动量

    //计算力矩控制特性

    //转速模式电机电流

    //启动摩擦力矩

    //最大损失力矩

    //力矩响应时间

    //整机滑行时间

    //浪涌电流测试


signals:
    void logMsg(QString);
    void reportCreated();
public slots:
    void getDataFromSql(QVector<QVector<QString> > res);

    //从原始数据中提取和初始化被测数据
    void initExpData();
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

    //实验原始数据
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
    QList<double> m_test_unit_setspd_;  //设置速度测试

    //测试结果存放地点
    //速度测试 1.平均速度 2.角动量常值偏差 3.角动量动态偏差
    QVector<QVector<double> > m_result_spd_;

};

#endif // QMOTORREPORT_H
