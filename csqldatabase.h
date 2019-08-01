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
        delete p_sql_query_;
        m_data_base_.close();
    }

    void doSqlQuery(QString query_str,bool need_return);
    QString makeSaveString(QString exp_name, QString usr_name, QString exp_no, QVector<QString>);
    QString getLastExpId(QString motor_id);
    void getExpDataFromSqlDB(QString motor_id,QString exp_id,QString motor_mode);
signals:
    void emitExpData(QVector<QVector<QString> >);
    void sendQueryRes(QSqlQuery);
    void sendErrorText(QString);
public slots:
    void insertIntoDB(QString exp_name, QString usr_name, QString exp_no, QVector<QString> motor);
    void analysisSqlForDocRes(QSqlQuery);
private:
    QSqlDatabase m_data_base_;
    QSqlQuery* p_sql_query_;
};

#endif // CSQLDATABASE_H
