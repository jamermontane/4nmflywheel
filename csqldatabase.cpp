#include "csqldatabase.h"

SqlDataBase::SqlDataBase(QObject *parent) : QObject(parent)
{
//    sqlInit();
    qRegisterMetaType<QSqlQuery>("QSqlQuery");
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    qRegisterMetaType<QVector<QVector<QString> >>("QVector<QVector<QString> >");


    connect(this,&SqlDataBase::sendQueryRes,this,&SqlDataBase::analysisSqlForDocRes);
}

bool SqlDataBase::sqlInit()
{
    //qDebug() << QSqlDatabase::drivers();//查看当前支持的SQL驱动
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
        emit sendErrorText(" sql init error!");
        return false;
    }
    p_sql_query_ = new QSqlQuery(m_data_base_);
    return true;
}

void SqlDataBase::initMySQL()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) //判断是否存在这个连接
    {
        m_data_base_ = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        m_data_base_ = QSqlDatabase::addDatabase("QMYSQL");
        m_data_base_.setUserName("root");
        m_data_base_.setDatabaseName("flywheel");
        m_data_base_.setPassword("123456");
    }
    if (!m_data_base_.open()){
        qDebug()<<" mysql init error!";
        emit sendErrorText("mysql init error!");
    }
    p_sql_query_ = new QSqlQuery(m_data_base_);
}

void SqlDataBase::doSqlQuery(QString query_str, int dst)
{
    if (!p_sql_query_->exec(query_str))
    {
        qDebug() << p_sql_query_->lastError().text();
        emit sendErrorText(p_sql_query_->lastError().text());
    }
    else{
        if (dst == 1){
            emit sendQueryRes(*p_sql_query_,1);
        }
        else if (dst == 2){
            emit sendQueryRes(*p_sql_query_,2);
        }
        else{
            return;
        }
    }
}

QString SqlDataBase::makeSaveString(QString exp_name, QString usr_name, QString exp_no,
                                    QVector<QString> motor)
{
//    qDebug()<<"SQL:"<<QThread::currentThreadId();
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
    if(!m_data_base_.tables().contains(motor.at(0).toLower()))
    {
        QString tempsql = "CREATE TABLE ";
        tempsql.append(motor.at(0));
        //rowid int UNSIGNED  primary key AUTO_INCREMENT,
        tempsql.append("( EXPID  VARCHAR (50), EXPNAME  VARCHAR (50), USRNAME  VARCHAR (50), EXPNO  VARCHAR (50),"
                       " MOTORID  VARCHAR (50), VOL  float(20,4),  CURRENT  float(20,4),  SETSPEED  float(20,4),  SPEED  float(20,4),"
                       " SETTORQUE  float(20,4), TORQUE  float(20,4), WATE  float(20,4), ANGULARMOMENTUM  float(20,4),"
                       " ANGULARMOMENTUMDT  float(20,4), ANGULARMOMENTUMJT  float(20,4), FLYWHEELMODE  VARCHAR (50),"
                       " VACUUM  VARCHAR (50), EXPADDRESS  VARCHAR (50), ACTCUR  float(20,4) ,"
                       " TIME  TimeStamp NOT NULL DEFAULT CURRENT_TIMESTAMP) character set = utf8;");

        QSqlQuery sql_query(m_data_base_);

        if (!sql_query.exec(tempsql))
        {
            qDebug() << sql_query.lastError().text();
            emit sendErrorText(sql_query.lastError().text());
        }
    }
    else{
        query_string.append("INSERT INTO ");
        query_string.append(motor.at(0));
        query_string.append("( EXPID , EXPNAME , USRNAME , EXPNO , MOTORID , VOL , CURRENT , SETSPEED , SPEED "
                            ", SETTORQUE , TORQUE , WATE , ANGULARMOMENTUM , ANGULARMOMENTUMDT , ANGULARMOMENTUMJT "
                            ", FLYWHEELMODE , VACUUM , EXPADDRESS , ACTCUR ) VALUES(");
        query_string.append("'"+motor.at(1)+"',");
        query_string.append("'"+exp_name+"',");
        query_string.append("'"+usr_name+"',");
        query_string.append("'"+exp_no+"',");
        query_string.append("'"+motor.at(2)+"',");
        query_string.append("'"+motor.at(3)+"',");
        query_string.append("'"+motor.at(4)+"',");
        query_string.append("'"+motor.at(5)+"',");
        query_string.append("'"+motor.at(6)+"',");
        query_string.append("'"+motor.at(7)+"',");
        query_string.append("'"+motor.at(8)+"',");
        query_string.append("'"+motor.at(9)+"',");
        query_string.append("'"+motor.at(10)+"',");
        query_string.append("'"+motor.at(11)+"',");
        query_string.append("'"+motor.at(12)+"',");
        query_string.append("'"+motor.at(13)+"',");
        query_string.append("'"+motor.at(14)+"',");
        query_string.append("'"+motor.at(15)+"',");
        query_string.append("'"+motor.at(16)+"'");
        query_string.append(")");
    }
    return query_string;
}

//查询最后一次实验的实验ID
//为了测试完成后自动生成报表
QString SqlDataBase::getLastExpId(QString motor_id)
{
    QString tempsql = QString("select * from %1 where EXPID is not \"\" ORDER BY EXPID DESC LIMIT 1 ").arg(motor_id);
    QSqlQuery sql_query(m_data_base_);
    if (!sql_query.exec(tempsql))
    {
        qDebug() << sql_query.lastError().text();
        emit sendErrorText(sql_query.lastError().text());
    }
    else{
        if (sql_query.next()){
            return sql_query.value("EXPID").toString();
        }
    }
    return QString("null");
}

//调用该接口，从数据库中查询数据并返回查询结果
void SqlDataBase::getExpDataFromSqlDB(QString motor_id, QString exp_id, QString motor_mode,QString start_time,QString end_time)
{
    QString query_str = "SELECT * FROM ";
    query_str.append(motor_id);
    if (exp_id.size() != 0){
        query_str.append(" WHERE EXPID = ");
        query_str.append(exp_id);
    }
    if (exp_id.size()!=0 && motor_mode.size() != 0){
        if (exp_id.size() != 0) query_str.append(" AND ");
        else query_str.append(" WHERE ");
        query_str.append(" FLYWHEELMODE = ");
        query_str.append(motor_mode);
    }
    QString ss = "WHERE";
    if (!query_str.contains(ss)){
        query_str.append(" WHERE ");
        query_str.append(" TIME >= \"");
        query_str.append(start_time);
        query_str.append("\" AND ");
        query_str.append(" TIME <= \"");
        query_str.append(end_time);
        query_str.append("\"");
    }
    else{
        query_str.append(" AND ");
        query_str.append(" TIME >= \"");
        query_str.append(start_time);
        query_str.append("\" AND ");
        query_str.append(" TIME <= \"");
        query_str.append(end_time);
        query_str.append("\"");
    }
    query_str.append(" LIMIT 1000");
    doSqlQuery(query_str,1);
}

void SqlDataBase::insertIntoDB(QString exp_name, QString usr_name, QString exp_no,const QVector<QString> &motor)
{
//    qDebug()<<"SQL:"<<QThread::currentThreadId();
    QString query_str = makeSaveString(exp_name, usr_name, exp_no,motor);
    doSqlQuery(query_str,0);

}


void SqlDataBase::analysisSqlForDocRes(QSqlQuery query_res, int dst)
{
    QVector<QVector<QString> > res;
    while(query_res.next()){
        QVector<QString> t;
        for (int i =0;i<20;++i){
            t.append(query_res.value(i).toString());
        }
        res.push_back(std::move(t));   
    }
    if (dst == 1)
        emit emitExpData(res);
    else if (dst == 2){
        emit emitLastExpData(res);
    }
}

//得到最后一次实验数据
void SqlDataBase::getLastExpData(QString motor_id,QString motor_mode)
{
    QString query_str = "SELECT * FROM ";
    query_str.append(motor_id);
    query_str.append(" WHERE EXPID = ");
    query_str.append(getLastExpId(motor_id));
    if (motor_mode.size() != 0){
        query_str.append(" AND ");
        query_str.append(" WHERE ");
        query_str.append(" FLYWHEELMODE = ");
        query_str.append(motor_mode);
    }
    doSqlQuery(query_str,2);
}


