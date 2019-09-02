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
    QVector<int>        flywheel_mode_;

    /*-----------------师弟看这里----------------*/
    //需要计算的值    11 12等为该测试项目的编号
    QList<double> m_test_unit_setspd_;      //11.速度测试、17.转速模式电机电流
    QList<double> m_test_unit_max_tor_;     //12.最大反作用力矩
    QList<double> m_test_unit_avg_wate_;    //13.稳速功耗
    QList<double> m_test_unit_max_wate_;    //14.最大功耗
    QList<double> m_test_unit_max_jdl_;     //15.最大角动量
    QList<double> m_test_unit_tor_;         //16.力矩模式
    QList<double> m_test_unit_start_tor_;   //18.启动模式摩擦力矩
    QList<double> m_test_unit_loss_tor_;    //19.最大损失力矩
    QList<double> m_test_unit_tor_time_;    //110.力矩响应时间
    QList<double> m_test_unit_hx_time_;     //111.整机滑行时间
    QList<double> m_test_unit_start_cur_;   //112.浪涌电流测试


    //测试结果存放地点
    //如有多条记录，以1.2.3.的形式列出
    QVector<QVector<double> > m_result_spd_;    //速度测试 1.平均速度 2.角动量常值偏差 3.角动量动态偏差
    double  m_result_max_tor_;                  //最大力矩
    QVector<double> m_result_wate_;             //稳速功耗
    double m_result_max_wate_;                  //最大功耗
    double m_result_max_jdl_;                   //最大角动量
    QVector<QVector<double> > m_result_tor_;    //力矩控制特性 1.实测电机电流 2.实测输出力矩
    QVector<QVector<double> > m_result_cur_;    //转速模式电机电流 1.测试口总电流 2.实测电机电流
    double m_result_start_tor_;                 //启动摩擦力矩
    double m_result_loss_tor_;                  //最大损失力矩
    double m_result_tor_time_;                  //力矩响应时间
    double m_result_hx_time_;                   //整机滑行时间
    double m_result_start_cur_;                 //浪涌电流测试

};

#endif // QMOTORREPORT_H
