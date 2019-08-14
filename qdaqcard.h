#ifndef QDAQCARD_H
#define QDAQCARD_H

#include <QObject>
#include <QString>
#include <QVector>

#include "D2kDask.h"

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
public slots:
    void readAllChannel();
    void getStartCurrent(int channel);
private:
    bool is_init_ = false;
    DAQCARD::I16 card_;
};

#endif // QDAQCARD_H
