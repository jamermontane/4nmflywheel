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
#include <QFile>
#include <QDir>

#include "motor.h"
//if appear this warning:"MySQL server has gone away QMYSQL: Unable to execute query,
//you can follow https://stackoverflow.com/questions/10474922/error-2006-hy000-mysql-server-has-gone-away
//set global max_allowed_packet=64*1024*1024;
class SqlDataBase : public QObject
{
    Q_OBJECT
public:
    explicit SqlDataBase(QObject *parent = 0);
    bool sqlInit();
    void initMySQL(); 
    ~SqlDataBase(){
        delete p_sql_query_;
        m_data_base_.close();
    }

    void doSqlQuery(QString query_str, int dst = 0);
    QString makeSaveString(QString exp_name, QString usr_name, QString exp_no, QVector<QString>);
    QString getLastExpId(QString motor_id);
    void getExpDataFromSqlDB(QString motor_id,QString exp_id,QString motor_mode,QString,QString);
signals:
    void emitExpData(const QVector<QVector<QString> > &);
    void emitLastExpData(const QVector<QVector<QString> > &);
    void sendQueryRes(QSqlQuery,int);
    void sendErrorText(QString);
public slots:
    void insertIntoDB(QString exp_name, QString usr_name, QString exp_no,const QVector<QString> &motor);
    void analysisSqlForDocRes(QSqlQuery, int dst);
    void getLastExpData(QString motor_id,QString motor_mode);
private:
    QSqlDatabase m_data_base_;
    QSqlQuery* p_sql_query_;
};

#endif // CSQLDATABASE_H
