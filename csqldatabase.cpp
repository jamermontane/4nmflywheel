#include "csqldatabase.h"

SqlDataBase::SqlDataBase(QObject *parent) : QObject(parent)
{
    sqlInit();
    qRegisterMetaType<QSharedPointer<QSqlQuery>>("QSharedPointer<QSqlQuery>");
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
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
                                    QVector<QString> motor)
{
    QString query_string;
    if (exp_name.isEmpty()){
        exp_name = "null";
    }
    if (usr_name.isEmpty()){
        usr_name = "null";
    }
    if (exp_no.isEmpty()){
        exp_no = QDateTime::currentDateTime().toString("YYMMDDHHMMSS");
    }

    if(!m_data_base_.tables().contains(motor.at(0)))
    {
        QString tempsql = "CREATE TABLE ";
        tempsql.append(motor.at(0));
        tempsql.append("([EXPNAME] VARCHAR (50),[USRNAME] VARCHAR (50),[EXPNO] VARCHAR (50),"
                       "[MOTORID] VARCHAR (50),[VOL] DOUBLE, [CURRENT] DOUBLE, [SETSPEED] DOUBLE, [SPEED] DOUBLE,"
                       "[SETTORQUE] DOUBLE,[TORQUE] DOUBLE,[WATE] DOUBLE,[ANGULARMOMENTUM] DOUBLE,"
                       "[ANGULARMOMENTUMDT] DOUBLE,[ANGULARMOMENTUMJT] DOUBLE,"
                       "[TIME] TimeStamp NOT NULL DEFAULT (datetime('now','localtime')))");
        QSqlQuery sql_query(m_data_base_);
        if (!sql_query.exec(tempsql))
        {
            qDebug() << sql_query.lastError().text();
        }
    }
    else{
        query_string.append("INSERT INTO ");
        query_string.append(motor.at(0));
        query_string.append("([EXPNAME],[USRNAME],[EXPNO],[MOTORID],[VOL],[CURRENT],[SETSPEED],[SPEED]"
                            ",[SETTORQUE],[TORQUE],[WATE],[ANGULARMOMENTUM],[ANGULARMOMENTUMDT],[ANGULARMOMENTUMJT]"
                            ") VALUES(");
        query_string.append("'"+exp_name+"',");
        query_string.append("'"+usr_name+"',");
        query_string.append("'"+exp_no+"',");
        query_string.append("'"+motor.at(1)+"',");
        query_string.append("'"+motor.at(2)+"',");
        query_string.append("'"+motor.at(3)+"',");
        query_string.append("'"+motor.at(4)+"',");
        query_string.append("'"+motor.at(5)+"',");
        query_string.append("'"+motor.at(6)+"',");
        query_string.append("'"+motor.at(7)+"',");
        query_string.append("'"+motor.at(8)+"',");
        query_string.append("'"+motor.at(9)+"',");
        query_string.append("'"+motor.at(10)+"',");
        query_string.append("'"+motor.at(11)+"'");
        query_string.append(")");
    }
    return query_string;
}

void SqlDataBase::insertIntoDB(QString exp_name, QString usr_name, QString exp_no,QVector<QString> motor)
{
    QString query_str = makeSaveString(exp_name, usr_name, exp_no,motor);
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


