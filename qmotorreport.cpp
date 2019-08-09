#include "qmotorreport.h"

QMotorReport::QMotorReport(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QVector<QVector<QString> >>("QVector<QVector<QString> >");
}

void QMotorReport::setExpInfomation(QVector<QString> &data)
{
    //数据库改变的话，size = 20 也需要改变
    if (data.size() != 20) {
        emit logMsg(tr("数据库错误，是否改变了表的结构？"));
        return;
    }

    this->exp_id_           = data[0];
    this->exp_name_         = data[1];
    this->usr_name_         = data[2];
    this->exp_no_           = data[3];
    this->flywheel_no_      = data[4];
    this->flywheel_vol_.push_back(data[5].toDouble());
    this->flywheel_cur_.push_back(data[6].toDouble());
    this->flywheel_setSpd_.push_back(data[7].toDouble());
    this->flywheel_spd_.push_back(data[8].toDouble());
    this->flywheel_setTor_.push_back(data[9].toDouble());
    this->flywheel_tor_.push_back(data[10].toDouble());
    this->flywheel_wate_.push_back( data[11].toDouble());
    this->flywheel_JDL_.push_back( data[12].toDouble());
    this->flywheel_JDL_dynamic_.push_back(data[13].toDouble());
    this->flywheel_JDL_const_.push_back(data[14].toDouble());
    this->flywheel_mode_ =data[15];
    this->exp_vacuum_       = data[16];
    this->exp_address_      = data[17];
    this->flywheel_act_cur_.push_back(data[18].toDouble());
    this->exp_time_         = data[19];
}



//创建word报告
void QMotorReport::createWordReport()
{
    QString save_path = "E:/result.doc";
    QWord word;
    if (!word.createNewWord(save_path)){
        qDebug() << "qword:create word error!";
        return;
    }
    word.setPageOrientation(0);			//页面方向
    word.setWordPageView(3);			//页面视图
    //sword.setFontName(QString::fromLocal8Bit("宋体"));
//    word.setParagraphAlignment(0);		//下面文字位置
//    word.setFontSize(20);				//字体大小
//    word.setFontBold(true);				//字体加粗
//    word.insertText(tr("Electrical Equipment Infrared Diagnosis Report "));
    word.setFontBold(false);

//    word.insertMoveDown();  //控制插入位置

//    word.setFontSize(10);
//    word.setParagraphAlignment(1);
//    QString current_Time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//    word.insertText(tr("Report Generation Date:"));
//    word.insertText(current_Time);

    word.intsertTable(18,10);             //创建表格
    word.setColumnHeight(1,1,10);

    //合并单元格
    word.MergeCells(1,1,1,1,10);          //第一行

    word.MergeCells(1,2,2,2,4);     //第二行
    word.MergeCells(1,2,4,2,5);
    word.MergeCells(1,2,6,2,7);

    word.MergeCells(1,3,2,3,4);     //第三行
    word.MergeCells(1,3,4,3,5);
    word.MergeCells(1,3,6,3,7);

    word.MergeCells(1,4,4,4,7); //第四行

    word.MergeCells(1,5,1,18,1); //第5行
    word.MergeCells(1,5,2,18,2);

    //第一行数据设置
    word.setCellFontBold(1,1,1,true);   //设置是否粗体
//    word.setTableAutoFitBehavior(0);    //自动拉伸列
    word.setCellFontSize(1,1,1,14);
    word.setColumnWidth(1,1,120);
    word.setRowAlignment(1,1,0);
    word.setCellString(1,1,1,tr("4Nms-1A 飞轮电性能测试记录表"));

    //第二行数据设置
    word.setCellFontBold(1,2,1,false);   //设置是否粗体
//    word.setTableAutoFitBehavior(0);    //自动拉伸列
//    word.setColumnWidth(1,2,30);
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,1,10);
    word.setCellString(1,2,1,tr("代号"));

    word.setCellFontBold(1,2,2,false);   //设置是否粗体
    word.setRowAlignment(1,2,1);
    word.setCellFontSize(1,2,2,10);
    word.setCellString(1,2,2,tr("FW-GR.160.4-1A"));

    word.setCellFontBold(1,2,3,false);   //设置是否粗体
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,3,10);
    word.setCellString(1,2,3,tr("飞轮编号"));

    word.setCellFontBold(1,2,4,false);   //设置是否粗体
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,4,10);
    word.setCellString(1,2,4,tr("00100"));

    word.setCellFontBold(1,2,5,false);   //设置是否粗体
//    word.setTableAutoFitBehavior(0);    //自动拉伸列
//    word.setColumnWidth(1,3,60);
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,5,10);
    word.setCellString(1,2,5,tr("测试时真空度"));

    word.setCellFontBold(1,2,6,false);   //设置是否粗体
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,6,10);
    word.setCellString(1,2,6,tr("500"));

    //第三行数据设置
    word.setCellFontBold(1,3,1,false);   //设置是否粗体
//    word.setTableAutoFitBehavior(0);    //自动拉伸列
//    word.setColumnWidth(1,2,30);
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,1,10);
    word.setCellString(1,3,1,tr("测试时间"));

    word.setCellFontBold(1,3,2,false);   //设置是否粗体
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,2,10);
    word.setCellString(1,3,2,tr("-"));

    word.setCellFontBold(1,3,3,false);   //设置是否粗体
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,3,10);
    word.setCellString(1,3,3,tr("测试地点"));

    word.setCellFontBold(1,3,4,false);   //设置是否粗体
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,4,10);
    word.setCellString(1,3,4,tr("-"));

    word.setCellFontBold(1,3,5,false);   //设置是否粗体
//    word.setTableAutoFitBehavior(0);    //自动拉伸列
//    word.setColumnWidth(1,3,60);
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,5,10);
    word.setCellString(1,3,5,tr("测试人员"));

    word.setCellFontBold(1,3,6,false);   //设置是否粗体
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,6,10);
    word.setCellString(1,3,6,tr("500"));

    //第四行数据设置
    word.setCellFontBold(1,4,1,false);   //设置是否粗体
    word.setRowAlignment(1,4,0);
    word.setCellFontSize(1,4,1,8);
    word.setCellString(1,4,1,tr("序号"));

    word.setCellFontBold(1,4,2,false);   //设置是否粗体
    word.setRowAlignment(1,4,0);
    word.setCellFontSize(1,4,2,8);
    word.setCellString(1,4,2,tr("测试项目"));

    word.setCellFontBold(1,4,3,false);   //设置是否粗体
    word.setRowAlignment(1,4,0);
    word.setCellFontSize(1,4,3,8);
    word.setCellString(1,4,3,tr("指标值"));

    word.setCellFontBold(1,4,4,false);   //设置是否粗体
    word.setRowAlignment(1,4,0);
    word.setCellFontSize(1,4,4,8);
    word.setCellString(1,4,4,tr("实测(计算)值"));

    word.setCellFontBold(1,4,5,false);   //设置是否粗体
    word.setRowAlignment(1,4,0);
    word.setCellFontSize(1,4,5,8);
    word.setCellString(1,4,5,tr("指令转速"));

    word.setCellFontBold(1,4,6,false);   //设置是否粗体
    word.setRowAlignment(1,4,0);
    word.setCellFontSize(1,4,6,8);
    word.setCellString(1,4,6,tr("结论"));

    word.setCellFontBold(1,4,7,false);   //设置是否粗体
    word.setRowAlignment(1,4,0);
    word.setCellFontSize(1,4,7,8);
    word.setCellString(1,4,7,tr("备注"));

    //速度测试向表里填数据
    for (int spd_idx =0;spd_idx < m_test_unit_setspd_.size();++spd_idx){
        //写指令转速
        word.setCellFontBold(1,6+spd_idx,8,false);   //设置是否粗体
        word.setRowAlignment(1,6+spd_idx,0);
        word.setCellFontSize(1,6+spd_idx,8,8);
        word.setCellString(1,6+spd_idx,8,QString::number(m_test_unit_setspd_.at(spd_idx)));
        //写串口
        word.setCellFontBold(1,6+spd_idx,4,false);   //设置是否粗体
        word.setRowAlignment(1,6+spd_idx,0);
        word.setCellFontSize(1,6+spd_idx,4,8);
        word.setCellString(1,6+spd_idx,4,QString::number(m_result_spd_.at(spd_idx).at(0)));
        //写常值偏差
        word.setCellFontBold(1,6+spd_idx,6,false);   //设置是否粗体
        word.setRowAlignment(1,6+spd_idx,0);
        word.setCellFontSize(1,6+spd_idx,6,8);
        word.setCellString(1,6+spd_idx,6,QString::number(m_result_spd_.at(spd_idx).at(1)));
        //写动态偏差
        word.setCellFontBold(1,6+spd_idx,7,false);   //设置是否粗体
        word.setRowAlignment(1,6+spd_idx,0);
        word.setCellFontSize(1,6+spd_idx,7,8);
        word.setCellString(1,6+spd_idx,7,QString::number(m_result_spd_.at(spd_idx).at(2)));
    }

//  save and close file
    word.setVisible(false);
    word.saveAs();
    word.close();

    emit logMsg(tr("测试报表已生成！存储地址：%1").arg(save_path));
}


void QMotorReport::getDataFromSql(QVector<QVector<QString> > res)
{
    if (res.empty()) return;
    setExpInfomation(*res.begin());
    for (int i =1;i < res.size();++i){
        const QVector<QString>& data = res.at(i);
        this->flywheel_vol_.push_back(data[5].toDouble());
        this->flywheel_cur_.push_back(data[6].toDouble());
        this->flywheel_setSpd_.push_back(data[7].toDouble());
        this->flywheel_spd_.push_back(data[8].toDouble());
        this->flywheel_setTor_.push_back(data[9].toDouble());
        this->flywheel_tor_.push_back(data[10].toDouble());
        this->flywheel_wate_.push_back( data[11].toDouble());
        this->flywheel_JDL_.push_back( data[12].toDouble());
        this->flywheel_JDL_dynamic_.push_back(data[13].toDouble());
        this->flywheel_JDL_const_.push_back(data[14].toDouble());
        this->flywheel_act_cur_.push_back(data[18].toDouble());
    }
    qDebug()<<"report: data recv complete!";


    initExpData();
    calExpDataSetSpd();

    createWordReport();
}

void QMotorReport::initExpData()
{
    //提取setspd
    QSet<double> t_setspd;
    for (double &setspd:flywheel_setSpd_){
        t_setspd.insert(setspd);
    }

    m_test_unit_setspd_.clear();
    for (auto &set_spd:t_setspd){
        m_test_unit_setspd_.push_back(set_spd);
    }
}

void QMotorReport::calExpDataSetSpd()
{
    for (int idx = 0;idx<m_test_unit_setspd_.size();++idx){
        double current_set_spd = m_test_unit_setspd_.at(idx);

        //每一次提取当前需要测试的数据
        QVector<double> need_test_spd;
        QVector<double> jdl_const;
        QVector<double> jdl_dynamic;
        for (int i =0;i < flywheel_setSpd_.size();++i){
            if (flywheel_setSpd_.at(i) == current_set_spd){
                need_test_spd.push_back(flywheel_spd_.at(i));
                jdl_const.push_back(flywheel_JDL_const_.at(i));
                jdl_dynamic.push_back(flywheel_JDL_dynamic_.at(i));
            }
        }

        //处理需要测试的数据
        //第一个处理速度 角动量常值偏差 动态偏差
        bool start_test = false;
        double avg_spd = 0;
        double avg_jdl_const = 0;
        double avg_jdl_dynamic = 0;
        uint data_num = 0;
        for (int i = 0;i < need_test_spd.size();++i){
            double spd = need_test_spd.at(i);
            if (start_test == false && spd < current_set_spd){
                continue;
            }
            else{
                start_test = true;
            }

            avg_spd += spd;
            avg_jdl_const += jdl_const.at(i);
            avg_jdl_dynamic += jdl_dynamic.at(i);
            data_num++;
        }
        QVector<double> tmp_res;
        tmp_res.push_back(avg_spd / data_num);
        tmp_res.push_back(avg_jdl_const / data_num);
        tmp_res.push_back(avg_jdl_dynamic / data_num);
        m_result_spd_.push_back(tmp_res);
    }
}
