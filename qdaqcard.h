#ifndef QDAQCARD_H
#define QDAQCARD_H

#include <QObject>
#include <QString>
#include <QVector>

#include "D2kDask.h"

//该数据采集卡型号为adlink DAQ2213

using namespace DAQCARD;
class QDaqcard : public QObject
{
    Q_OBJECT
public:
    explicit QDaqcard(QObject *parent = 0);
    ~QDaqcard();
    void init();

signals:
    void logMsg(QString);
    void sendAllData(QVector<double>);
    void sendSurgeCurrnt(QVector<double>);
public slots:
    void readAllChannel();
    void getSurgeCurrent(U16 channel, QVector<double> &v, int point_num);
    void setDOPort(uint port,bool status);
private:
    bool is_init_ = false;
    DAQCARD::I16 card_;
    uchar m_DO_port_status_ = 0xff;
};

#endif // QDAQCARD_H
