#include "csqldatabase.h"

SqlDataBase::SqlDataBase(QObject *parent) : QObject(parent)
{
    sqlInit();
    qRegisterMetaType<QSharedPointer<QSqlQuery>>("QSharedPointer<QSqlQuery>");
}

bool SqlDataBase::sqlInit()
{
    //qDebug() << QSqlDatabase::drivers();查看当前支持的SQL驱动

    if (QSqlDatabase::contains("qt_sql_default_connection")) //判断是否存在这个连接
    {
        m_data_base_ = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        m_data_base_ = QSqlDatabase::addDatabase("QSQLITE"); //是SQLite对应的驱动名，不能改 第二个参数被省略了，
        m_data_base_.setUserName("HDU");        //第二个参数的默认参数就是上面提到的Qt默认连接名称qt_sql_default_connection。
        m_data_base_.setDatabaseName("motionDataBase.db");//如果需要使用自定义的连接名称
                                                   //（如果程序需要处理多个数据库文件的话就会这样），则应该加入第二个参数

        m_data_base_.setPassword("123456");
    }
    if (!m_data_base_.open()){
        qDebug()<<" sql init error!";
        return false;
    }
    return true;
}

QString SqlDataBase::makeSaveString(QString exp_name, QString usr_name, QString exp_no,
                                    QString motor_id, QString vol, QString cur,
                                    QString set_spd, QString spd)
{
    QString query_string;
    if (exp_name.isEmpty()){
        exp_name = "null";
    }
    if (usr_name.isEmpty()){
        exp_name = "null";
    }
    if (exp_no.isEmpty()){
        exp_name = "null";
    }
    if(!m_data_base_.tables().contains("MOTOR1"))
    {
        QString tempsql = "CREATE TABLE MOTOR1";
        tempsql.append("([EXPNAME] VARCHAR (50),[USRNAME] VARCHAR (50),[EXPNO] VARCHAR (50),"
                       "[MOTORID] VARCHAR (50),[VOL] DOUBLE, [CURRENT] DOUBLE, [SETSPEED] DOUBLE, [SPEED] DOUBLE,"
                       "[SETTORQUE] DOUBLE,[TORQUE] DOUBLE,[WATE] DOUBLE,[ANGULARMOMENTUM] DOUBLE,"
                       "[ANGULARMOMENTUM1] DOUBLE,[ANGULARMOMENTUM2] DOUBLE,"
                       "[TIME] TimeStamp NOT NULL DEFAULT (datetime('now','localtime')))");
        QSqlQuery sql_query(m_data_base_);
        if (!sql_query.exec(tempsql))
        {
            qDebug() << sql_query.lastError().text();
        }
    }
    else{

        query_string.append("INSERT INTO MOTOR1");
        query_string.append("([EXPNAME],[USRNAME],[EXPNO],[MOTORID],[VOL],[CURRENT],[SETSPEED],[SPEED]) VALUES(");
        query_string.append("'"+exp_name+"',");
        query_string.append("'"+usr_name+"',");
        query_string.append("'"+exp_no+"',");
        query_string.append("'"+motor_id+"',");
        query_string.append(vol+",");
        query_string.append(cur+",");
        query_string.append(set_spd+",");
        query_string.append(spd);
        query_string.append(")");
    }
    return query_string;
}

void SqlDataBase::insertIntoDB(const QString &exp_name,const QString &usr_name,const QString &exp_no,
                               const uint id,const double cur,const double vol,const double set_spd,
                               const double spd)
{
    QString query_str = makeSaveString(exp_name,usr_name,exp_no,QString::number(id),QString::number(cur),
                                      QString::number(vol),QString::number(set_spd),
                                      QString::number(spd));
    static QSqlQuery sql_query(m_data_base_);
    if(!sql_query.exec(query_str))
    {
        qDebug() << sql_query.lastError().text();
    }
}

void SqlDataBase::queryFromDB(QString query_string)
{

    QSharedPointer<QSqlQuery> pspl_query = QSharedPointer<QSqlQuery>(new QSqlQuery(m_data_base_));


    if (!pspl_query->exec(query_string))
    {
        qDebug() << pspl_query->lastError().text();
    }
    else{
        emit sendQueryRes(pspl_query);
    }
}


