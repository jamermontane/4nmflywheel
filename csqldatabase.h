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

class SqlDataBase : public QObject
{
    Q_OBJECT
public:
    explicit SqlDataBase(QObject *parent = 0);
    bool sqlInit();
    ~SqlDataBase(){
        m_data_base_.close();
    }


    QString makeSaveString(QString exp_name, QString usr_name, QString exp_no,
                           QString motor_id, QString vol, QString cur,
                           QString set_spd, QString spd);
signals:
    void sendQueryRes(QSharedPointer<QSqlQuery>);
public slots:
    void insertIntoDB(const QString &exp_name,const QString &usr_name,const QString &exp_no,
                                  const uint id,const double cur,const double vol,const double set_spd,
                                  const double spd);
    void queryFromDB(QString);
private:
    QSqlDatabase m_data_base_;
};

#endif // CSQLDATABASE_H
