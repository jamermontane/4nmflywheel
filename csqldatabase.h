#ifndef CSQLDATABASE_H
#define CSQLDATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <motor.h>
#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include <QThread>

#include "motor.h"

class SqlDataBase : public QObject
{
    Q_OBJECT
public:
    explicit SqlDataBase(QObject *parent = 0);
    bool sqlInit();
    ~SqlDataBase(){
        m_data_base_.close();
    }


    QString makeSaveString(QString exp_name, QString usr_name, QString exp_no, QVector<QString>);
signals:
    void sendQueryRes(QSharedPointer<QSqlQuery>);
public slots:
    void insertIntoDB(QString exp_name, QString usr_name, QString exp_no, QVector<QString> motor);
    void queryFromDB(QString);
private:
    QSqlDatabase m_data_base_;
};

#endif // CSQLDATABASE_H
