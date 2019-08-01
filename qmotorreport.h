#ifndef QMOTORREPORT_H
#define QMOTORREPORT_H

#include <QObject>

class QMotorReport : public QObject
{
    Q_OBJECT
public:
    explicit QMotorReport(QObject *parent = 0);

signals:

public slots:
};

#endif // QMOTORREPORT_H