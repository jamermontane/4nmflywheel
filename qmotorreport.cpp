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
    this->flywheel_mode_.push_back(data[15].toInt());
    this->exp_vacuum_       = data[16];
    this->exp_address_      = data[17];
    this->flywheel_act_cur_.push_back(data[18].toDouble());
    this->exp_time_         = data[19];

}



//创建word报告
void QMotorReport::createWordReport()
{
    QString save_path = "E:/result.doc";//E:/result.doc
    QWord word;
    if (!word.createNewWord(save_path)){
        qDebug() << "qword:create word error!";
        return;
    }
    word.setPageOrientation(0);			//页面方向
    word.setWordPageView(3);			//页面视图
    //sword.setFontName(QString::fromLocal8Bit("宋体"));
    word.setParagraphAlignment(0);		//下面文字位置
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

    word.intsertTable(70,10); //24            //创建表格
    word.setColumnHeight(1,1,10);

    //合并单元格
    word.MergeCells(1,1,1,1,10);          //第一行4Nms-1A 飞轮电性能测试记录表

    word.MergeCells(1,2,2,2,4);     //第二行
    word.MergeCells(1,2,4,2,5);
    word.MergeCells(1,2,6,2,7);

    word.MergeCells(1,3,2,3,4);     //第三行
    word.MergeCells(1,3,4,3,5);
    word.MergeCells(1,3,6,3,7);

    word.MergeCells(1,4,4,4,7); //第四行
    word.MergeCells(1,4,5,5,8);//表示的是将c4l5的单元格与c5l8的单元格合并
    //第五行
    word.MergeCells(1,5,1,18,1);//（序号）1
    word.MergeCells(1,5,2,18,2);//转速控制特性n(-6000-6000rpm)
    word.MergeCells(1,5,3,18,3);//角动量控制偏差△H:≤0.002Nms常量偏差：≤0.005Nms
    word.MergeCells(1,5,9,18,9);//结论下的数据
    word.MergeCells(1,5,10,18,10);//转速可取多个数的平均值
    //第六行
    word.MergeCells(1,19,4,19,5);
    word.MergeCells(1,19,5,19,7);
    //word.MergeCells(1,6,6,6,);
    //第七行
    word.MergeCells(1,20,1,24,1);
    word.MergeCells(1,20,2,24,2);
    word.MergeCells(1,20,3,22,3);
    word.MergeCells(1,23,3,24,3);

    word.MergeCells(1,20,4,20,5);
    word.MergeCells(1,20,5,20,7);
    word.MergeCells(1,21,4,21,5);
    word.MergeCells(1,21,5,21,7);
    word.MergeCells(1,22,4,22,5);
    word.MergeCells(1,22,5,22,7);
    word.MergeCells(1,23,4,23,5);
    word.MergeCells(1,23,5,23,7);
    word.MergeCells(1,24,4,24,5);
    word.MergeCells(1,24,5,24,7);

    word.MergeCells(1,20,6,24,6);
    word.MergeCells(1,20,7,24,7);
    //第八行
    word.MergeCells(1,25,4,25,5);
    word.MergeCells(1,25,5,25,7);

    word.MergeCells(1,20,6,25,6);
    word.MergeCells(1,20,7,25,7);
    //第九行
    word.MergeCells(1,26,4,26,5);
    word.MergeCells(1,26,5,26,7);
    //第十行
    word.MergeCells(1,27,1,35,1);
    word.MergeCells(1,27,2,35,2);
    word.MergeCells(1,27,3,35,3);

    word.MergeCells(1,27,4,27,8);
    word.MergeCells(1,27,5,27,6);

    word.MergeCells(1,28,4,28,5);
    word.MergeCells(1,28,5,28,6);

    word.MergeCells(1,29,4,29,5);
    word.MergeCells(1,29,5,29,6);

    word.MergeCells(1,30,4,30,5);
    word.MergeCells(1,30,5,30,6);

    word.MergeCells(1,31,4,31,5);
    word.MergeCells(1,31,5,31,6);

    word.MergeCells(1,32,4,32,5);
    word.MergeCells(1,32,5,32,6);

    word.MergeCells(1,33,4,33,5);
    word.MergeCells(1,33,5,33,6);

    word.MergeCells(1,34,4,34,5);
    word.MergeCells(1,34,5,34,6);

    word.MergeCells(1,35,4,35,5);
    word.MergeCells(1,35,5,35,6);

    word.MergeCells(1,28,7,35,7);
    word.MergeCells(1,28,8,35,8);
    //第十一行
    word.MergeCells(1,36,1,46,1);
    word.MergeCells(1,36,2,46,2);
    word.MergeCells(1,36,3,46,3);

    word.MergeCells(1,36,4,36,5);
    word.MergeCells(1,36,5,36,6);

    word.MergeCells(1,37,4,37,5);
    word.MergeCells(1,37,5,37,6);

    word.MergeCells(1,38,4,38,5);
    word.MergeCells(1,38,5,38,6);

    word.MergeCells(1,39,4,39,5);
    word.MergeCells(1,39,5,39,6);

    word.MergeCells(1,40,4,40,5);
    word.MergeCells(1,40,5,40,6);

    word.MergeCells(1,41,4,41,5);
    word.MergeCells(1,41,5,41,6);

    word.MergeCells(1,42,4,42,5);
    word.MergeCells(1,42,5,42,6);

    word.MergeCells(1,43,4,43,5);
    word.MergeCells(1,43,5,43,6);

    word.MergeCells(1,44,4,44,5);
    word.MergeCells(1,44,5,44,6);

    word.MergeCells(1,45,4,45,5);
    word.MergeCells(1,45,5,45,6);

    word.MergeCells(1,46,4,46,5);
    word.MergeCells(1,46,5,46,6);

    word.MergeCells(1,36,7,46,7);
    word.MergeCells(1,36,8,46,8);
    //第十二行
    word.MergeCells(1,47,4,47,5);
    word.MergeCells(1,47,5,47,7);
    //第十三行
    word.MergeCells(1,48,4,48,5);
    word.MergeCells(1,48,5,48,7);
    //第十四行
    word.MergeCells(1,49,4,49,5);
    word.MergeCells(1,49,5,49,7);
    //第十五行
    word.MergeCells(1,50,4,50,5);
    word.MergeCells(1,50,5,50,7);
    //第十六行
    word.MergeCells(1,51,4,51,5);
    word.MergeCells(1,51,5,51,7);
    //第十七行
    word.MergeCells(1,52,1,52,10);
    //第一行数据设置
    word.setCellFontBold(1,1,1,true);   //设置是否粗体
//    word.setTableAutoFitBehavior(0);    //自动拉伸列
    word.setCellFontSize(1,1,1,14);
    word.setColumnWidth(1,1,120);//设置列的宽度
    word.setRowAlignment(1,1,0);//设置行的对其方式
    word.setCellString(1,1,1,tr("4Nms-1A 飞轮电性能测试记录表"));

    //第二行数据设置
    word.setCellFontBold(1,2,1,false);   //设置是否粗体
//    word.setTableAutoFitBehavior(0);    //自动拉伸列
//    word.setColumnWidth(1,2,30);
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,1,10);
    word.setCellString(1,2,1,tr("代号"));

    word.setCellFontBold(1,2,2,false);   //设置是否粗体
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,2,10);
    word.setCellString(1,2,2,exp_no_);

    word.setCellFontBold(1,2,3,false);   //设置是否粗体
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,3,10);
    word.setCellString(1,2,3,tr("飞轮编号"));

    word.setCellFontBold(1,2,4,false);   //设置是否粗体
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,4,10);
    word.setCellString(1,2,4,flywheel_no_);

    word.setCellFontBold(1,2,5,false);   //设置是否粗体
//    word.setTableAutoFitBehavior(0);    //自动拉伸列
//    word.setColumnWidth(1,3,60);
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,5,10);
    word.setCellString(1,2,5,tr("测试时真空度"));

    word.setCellFontBold(1,2,6,false);   //设置是否粗体
    word.setRowAlignment(1,2,0);
    word.setCellFontSize(1,2,6,10);
    word.setCellString(1,2,6,exp_vacuum_);

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
    word.setCellString(1,3,2,exp_time_);

    word.setCellFontBold(1,3,3,false);   //设置是否粗体
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,3,10);
    word.setCellString(1,3,3,tr("测试地点"));

    word.setCellFontBold(1,3,4,false);   //设置是否粗体
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,4,10);
    word.setCellString(1,3,4,exp_address_);

    word.setCellFontBold(1,3,5,false);   //设置是否粗体
//    word.setTableAutoFitBehavior(0);    //自动拉伸列
//    word.setColumnWidth(1,3,60);
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,5,10);
    word.setCellString(1,3,5,tr("测试人员"));

    word.setCellFontBold(1,3,6,false);   //设置是否粗体
    word.setRowAlignment(1,3,0);
    word.setCellFontSize(1,3,6,10);
    word.setCellString(1,3,6,this->usr_name_);

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
    //第五行数据
    word.setCellFontBold(1,5,1,false);   //设置是否粗体
    word.setRowAlignment(1,5,0);
    word.setCellFontSize(1,5,1,8);
    word.setCellString(1,5,1,tr("1"));

    word.setCellFontBold(1,5,2,false);   //设置是否粗体
    word.setRowAlignment(1,5,0);
    word.setCellFontSize(1,5,2,8);
    word.setCellString(1,5,2,tr("转速控制特性n(-6000~6000rpm)"));

    word.setCellFontBold(1,5,3,false);   //设置是否粗体
    word.setRowAlignment(1,5,0);
    word.setCellFontSize(1,5,3,8);
    word.setCellString(1,5,3,tr("角动量控制偏差△H:≤0.002Nms常量偏差：≤0.005Nms"));

    word.setCellFontBold(1,5,10,false);   //设置是否粗体
    word.setRowAlignment(1,5,0);
    word.setCellFontSize(1,5,10,8);
    word.setCellString(1,5,10,tr("转速可取多个数的平均值"));

    word.setCellFontBold(1,5,4,false);   //设置是否粗体
    word.setRowAlignment(1,5,0);
    word.setCellFontSize(1,5,4,8);
    word.setCellString(1,5,4,tr("串口"));

    word.setCellFontBold(1,5,5,false);   //设置是否粗体
    word.setRowAlignment(1,5,0);
    word.setCellFontSize(1,5,5,8);
    word.setCellString(1,5,5,tr("地测"));

    word.setCellFontBold(1,5,6,false);   //设置是否粗体
    word.setRowAlignment(1,5,0);
    word.setCellFontSize(1,5,6,8);
    word.setCellString(1,5,6,tr("常值偏差"));

    word.setCellFontBold(1,5,7,false);   //设置是否粗体
    word.setRowAlignment(1,5,0);
    word.setCellFontSize(1,5,7,8);
    word.setCellString(1,5,7,tr("△H偏差"));
    int cur_idx = 6;
    //第六行
    //速度测试：向表里填数据
    for (int spd_idx =0;spd_idx < m_test_unit_setspd_.size()-1;++spd_idx,++cur_idx){
        //写指令转速
        word.setCellFontBold(1,cur_idx,8,false);   //设置是否粗体
        word.setRowAlignment(1,cur_idx,0);
        word.setCellFontSize(1,cur_idx,8,8);
        word.setCellString(1,cur_idx,8,QString::number(m_test_unit_setspd_.at(spd_idx)));
        //写串口
        word.setCellFontBold(1,cur_idx,4,false);   //设置是否粗体
        word.setRowAlignment(1,cur_idx,0);
        word.setCellFontSize(1,cur_idx,4,8);
        word.setCellString(1,cur_idx,4,QString::number(m_result_spd_.at(spd_idx).at(0)));
        //写常值偏差
        word.setCellFontBold(1,cur_idx,6,false);   //设置是否粗体
        word.setRowAlignment(1,cur_idx,0);
        word.setCellFontSize(1,cur_idx,6,8);
        word.setCellString(1,cur_idx,6,QString::number(m_result_spd_.at(spd_idx).at(1)));
        //写动态偏差
        word.setCellFontBold(1,cur_idx,7,false);   //设置是否粗体
        word.setRowAlignment(1,cur_idx,0);
        word.setCellFontSize(1,cur_idx,7,8);
        word.setCellString(1,cur_idx,7,QString::number(m_result_spd_.at(spd_idx).at(2)));
    }

    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("2"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("最大反作用力矩T"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("绝对值≥0.1Nm"));

    word.setCellFontBold(1,cur_idx,5,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,5,8);
    word.setCellString(1,cur_idx,5,tr("斜坡指令±60rpm 采样时间取0.4s"));

    word.setCellFontBold(1,cur_idx,7,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,7,8);
    word.setCellString(1,cur_idx++,7,tr("可取多次平均"));
    //第七行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("3"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("稳压功耗P（133Pa下）"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("≤7.5W"));

    word.setCellFontBold(1,20,7,false);   //设置是否粗体
    word.setRowAlignment(1,20,0);
    word.setCellFontSize(1,20,7,8);
    word.setCellString(1,20,7,tr("P=U*I,     U、I可取多个数的平均值"));

    cur_idx+=3;

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("≤15W"));

    cur_idx+=2;
    //第八行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("4"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("最大功耗Pmax"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("≤100W"));

    word.setCellFontBold(1,cur_idx,5,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,5,8);
    word.setCellString(1,cur_idx,5,tr("按±（0.1±0.01）Nm加减速"));
    cur_idx++;
    //第九行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("5"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("最大角动量Hmax"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("≥4Nms"));

    word.setCellFontBold(1,cur_idx,5,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,5,8);
    word.setCellString(1,cur_idx,5,tr("转速绝对值≥6000rpm"));
    cur_idx++;
    //第十行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("6"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("力矩控制特性n(-5500~5500rpm)"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("电流控制允许偏差绝对值≤0.24A,实测输出力矩绝对值≥|Tc|-10mNm"));

    word.setCellFontBold(1,cur_idx,4,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,4,8);
    word.setCellString(1,cur_idx,4,tr("实测（计算）值"));

    cur_idx++;

    word.setCellFontBold(1,cur_idx,4,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,4,8);
    word.setCellString(1,cur_idx,4,tr("实测电机电流"));

    word.setCellFontBold(1,cur_idx,5,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,5,8);
    word.setCellString(1,cur_idx,5,tr("实测输出力矩"));

    word.setCellFontBold(1,cur_idx,6,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,6,8);
    word.setCellString(1,cur_idx,6,tr("力矩控制指令Tc"));

    word.setCellFontBold(1,cur_idx,8,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,8,8);
    word.setCellString(1,cur_idx,8,tr("可取多次平均"));

    cur_idx+=8;
    //第十一行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("7"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("转速模式电机电流"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("≤3.2A"));

    word.setCellFontBold(1,cur_idx,4,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,4,8);
    word.setCellString(1,cur_idx,4,tr("测试口总电流"));

    word.setCellFontBold(1,cur_idx,5,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,5,8);
    word.setCellString(1,cur_idx,5,tr("实测电机电流"));

    word.setCellFontBold(1,cur_idx,6,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,6,8);
    word.setCellString(1,cur_idx,6,tr("转速指令"));

    word.setCellFontBold(1,cur_idx,8,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,8,8);
    word.setCellString(1,cur_idx,8,tr("I可取多个数的平均值"));
    cur_idx++;
    for (int spd_idx =0;spd_idx < m_test_unit_setspd_.size()-1;++spd_idx,++cur_idx){
        //写指令转速
        word.setCellFontBold(1,cur_idx,6,false);   //设置是否粗体
        word.setRowAlignment(1,cur_idx,0);
        word.setCellFontSize(1,cur_idx,6,8);
        word.setCellString(1,cur_idx,6,QString::number(m_test_unit_setspd_.at(spd_idx)));
        //写串口
        word.setCellFontBold(1,cur_idx,4,false);   //设置是否粗体
        word.setRowAlignment(1,cur_idx,0);
        word.setCellFontSize(1,cur_idx,4,8);
        word.setCellString(1,cur_idx,4,QString::number(m_result_cur_.at(spd_idx).at(0)));
        //写常值偏差
        word.setCellFontBold(1,cur_idx,5,false);   //设置是否粗体
        word.setRowAlignment(1,cur_idx,0);
        word.setCellFontSize(1,cur_idx,5,8);
        word.setCellString(1,cur_idx,5,QString::number(m_result_cur_.at(spd_idx).at(1)));
    }
//    cur_idx+=10;

    word.setCellFontBold(1,cur_idx,6,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,6,8);
    word.setCellString(1,cur_idx,6,tr("按±（0.1±0.01）Nm加减速"));

    cur_idx++;
    //第十二行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("8"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("启动摩擦力矩"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("≤10mNm"));

    word.setCellFontBold(1,cur_idx,5,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,5,8);
    word.setCellString(1,cur_idx,5,tr("0rpm条件下发送力矩指令"));

    word.setCellFontBold(1,cur_idx,7,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,7,8);
    word.setCellString(1,cur_idx,7,tr("可取多次平均"));

    cur_idx++;

    //第十三行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("9"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("最大损失力矩"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("≤15mNm"));

    word.setCellFontBold(1,cur_idx,5,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,5,8);
    word.setCellString(1,cur_idx,5,tr("分段滑行时测试"));

    word.setCellFontBold(1,cur_idx,7,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,7,8);
    word.setCellString(1,cur_idx,7,tr("分段多次平均"));

    cur_idx++;

    //第十四行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("10"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("力矩响应时间"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("≤20ms"));

    word.setCellFontBold(1,cur_idx,5,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,5,8);
    word.setCellString(1,cur_idx,5,tr("由力矩控制模式条件下测试"));

    cur_idx++;

    //第十五行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("11"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("整机滑行时间"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("不少于20min"));

    cur_idx++;
   //第十五行
    word.setCellFontBold(1,cur_idx,1,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,1,8);
    word.setCellString(1,cur_idx,1,tr("12"));

    word.setCellFontBold(1,cur_idx,2,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,2,8);
    word.setCellString(1,cur_idx,2,tr("浪涌电流测试"));

    word.setCellFontBold(1,cur_idx,3,false);   //设置是否粗体
    word.setRowAlignment(1,cur_idx,0);
    word.setCellFontSize(1,cur_idx,3,8);
    word.setCellString(1,cur_idx,3,tr("≤3A，15ms"));

    cur_idx++;

    //第十六行
    word.setCellFontSize(1,cur_idx,1,8);
   // word.setColumnWidth(1,1,120);//设置列的宽度
    word.setRowAlignment(cur_idx,1,0);//设置行的对其方式
    word.setCellString(1,cur_idx,1,tr("注1：改性能测试采用软件自动测试。注2：测试时真空度要保持在133Pa以下。"));



//  save and close file
    word.setVisible(false);
    word.saveAs();
    word.close();

    emit logMsg(tr("生成报告：测试报表已生成！(100%)，存储地址：%1").arg(save_path));
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
        this->flywheel_mode_.push_back(data[15].toInt());
    }
    emit logMsg(tr("生成报告：数据查询完毕(20%)"));


    initExpData();
    calExpDataSetSpd();

    createWordReport();

    emit reportCreated();
}

//初始化实验数据
void QMotorReport::initExpData()
{
    //提取setspd
    QSet<double> t_setspd;
    int idx = 0;
    for (double &setspd:flywheel_setSpd_){
        //判断是否是速度模式
        if (flywheel_mode_.at(idx++) == 0)
            t_setspd.insert(setspd);
    }

    m_test_unit_setspd_.clear();
    for (auto &set_spd:t_setspd){
        m_test_unit_setspd_.push_back(set_spd);
    }

    //输出结果排序
    std::sort(m_test_unit_setspd_.begin(),m_test_unit_setspd_.end(),[](const double &a,const double &b){
        return qAbs(a) < qAbs(b) || (qAbs(a) == qAbs(b) && a >= 0) ;
    });


}

//计算速度实验数据
void QMotorReport::calExpDataSetSpd()
{
    for (int idx = 0;idx<m_test_unit_setspd_.size();++idx){
        double current_set_spd = m_test_unit_setspd_.at(idx);

        //每一次提取当前需要测试的数据
        QVector<double> need_test_spd;
        QVector<double> jdl_const;
        QVector<double> jdl_dynamic;
        QVector<double> serial_cur;
        QVector<double> daq_cur;
        for (int i =0;i < flywheel_setSpd_.size();++i){
            //spd 模式编号为0
            if (flywheel_setSpd_.at(i) == current_set_spd && flywheel_mode_.at(i) == 0){
                need_test_spd.push_back(flywheel_spd_.at(i));
                jdl_const.push_back(flywheel_JDL_const_.at(i));
                jdl_dynamic.push_back(flywheel_JDL_dynamic_.at(i));
                serial_cur.push_back(flywheel_cur_.at(i));
                daq_cur.push_back(flywheel_act_cur_.at(i));
            }
        }

        //处理需要测试的数据
        //第一个处理速度 角动量常值偏差 动态偏差
        bool start_test = false;
        double avg_spd = 0;
        double avg_jdl_const = 0;
        double avg_jdl_dynamic = 0;
        double avg_cur = 0;
        double avg_act_cur = 0;
        uint data_num = 0;
        for (int i = 0;i < need_test_spd.size();++i){
            double spd = need_test_spd.at(i);
            if (start_test == false && qAbs((spd) - (current_set_spd)) > 1){
                continue;
            }
            else{
                start_test = true;
            }

            avg_spd += spd;
            avg_jdl_const += jdl_const.at(i);
            avg_jdl_dynamic += jdl_dynamic.at(i);
            avg_cur += serial_cur.at(i);
            avg_act_cur += daq_cur.at(i);
            data_num++;
        }
        QVector<double> tmp_res;
        tmp_res.push_back(avg_spd / data_num);
        tmp_res.push_back(avg_jdl_const / data_num);
        tmp_res.push_back(avg_jdl_dynamic / data_num);
        m_result_spd_.push_back(tmp_res);

        tmp_res.clear();
        tmp_res.push_back(avg_cur / data_num);
        tmp_res.push_back(avg_act_cur / data_num);

        m_result_cur_.push_back(tmp_res);

    }
    emit logMsg(tr("生成报告：数据计算完毕(50%)"));
}
