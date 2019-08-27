#include "qdaqcard.h"

QDaqcard::QDaqcard(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QVector<double>>("QVector<double>");
    m_DO_port_status_ = 0xff;
}

QDaqcard::~QDaqcard()
{
    D2K_DO_WritePort(card_, Channel_P1A, 255);
    D2K_Release_Card(card_);
}

void QDaqcard::init()
{
    I16 err, card_num = 0, card_type=DAQ_2213;
    if ((card_=D2K_Register_Card (card_type, card_num)) <0 ) {
        logMsg(tr("Register_Card error=%1").arg(QString::number(card_)));
        return;
    }
    //需要使用的采集卡channel
    for (int i = 0;i < 7;++i){
        err = D2K_AI_CH_Config (card_, i, AD_B_10_V | AI_DIFF);
        if (err!=0) {
            logMsg(tr("D2K_AI_CH_Config error=%1").arg(QString::number(err)));
            return;
        }
    }
    is_init_ = true;

    //tmp do
    //port configured
    D2K_DIO_PortConfig(card_ ,Channel_P1A,
    OUTPUT_PORT);
    D2K_DIO_PortConfig(card_, Channel_P1B,
    OUTPUT_PORT);
    D2K_DIO_PortConfig(card_, Channel_P1CL,
    OUTPUT_PORT);
    D2K_DIO_PortConfig(card_, Channel_P1CH,
    OUTPUT_PORT);
    D2K_DO_WritePort(card_, Channel_P1A, 255);
}

void QDaqcard::readAllChannel()
{
    if (!is_init_) return;
    QVector<double> res;
    F64 chan_voltage;
    //使用循环去读取
    for (int i = 0; i <7;++i){
        I16 err = D2K_AI_VReadChannel (card_, i, &chan_voltage);
        if (err!=0) {
            logMsg(tr("D2K_AI_ReadChannel error=%1").arg(QString::number(err)));
            return;
        }
        res.push_back(chan_voltage);
    }
    emit sendAllData(res);
}

void QDaqcard::getSurgeCurrent(U16 channel,QVector<double> &v,int point_num)
{
    F64 chan_voltage;
    v.resize(point_num);

    for (int i = 0;i<point_num;++i){
        if (i == int(point_num*0.2)){
            setDOPort(channel+1,true);
        }
        if (i == int(point_num*0.8)){
            setDOPort(channel+1,false);
        }
        I16 err = D2K_AI_VReadChannel (card_, channel, &chan_voltage);
        if (err!=0) {
            logMsg(tr("D2K_AI_ReadChannel error=%1").arg(QString::number(err)));
            return;
        }
        v[i] = 2.5 - chan_voltage;
    }

    return;
}

//prot: 1 to 8 ,
//status: open = true,close=false
void QDaqcard::setDOPort(uint port, bool status)
{
    switch (port) {
    case 1:
        if (status)
            m_DO_port_status_ = m_DO_port_status_ & 254;
        else
            m_DO_port_status_ = m_DO_port_status_ | 1;
        break;
    case 2:
        if (status)
            m_DO_port_status_ = m_DO_port_status_ & 253;
        else
            m_DO_port_status_ = m_DO_port_status_ | 2;
        break;
    case 3:
        if (status)
            m_DO_port_status_ = m_DO_port_status_ & 251;
        else
            m_DO_port_status_ = m_DO_port_status_ | 4;
        break;
    case 4:
        if (status)
            m_DO_port_status_ = m_DO_port_status_ & 247;
        else
            m_DO_port_status_ = m_DO_port_status_ | 8;
        break;
    case 5:
        if (status)
            m_DO_port_status_ = m_DO_port_status_ & 239;
        else
            m_DO_port_status_ = m_DO_port_status_ | 16;
        break;
    case 6:
        if (status)
            m_DO_port_status_ = m_DO_port_status_ & 223;
        else
            m_DO_port_status_ = m_DO_port_status_ | 32;
        break;
    case 7:
        if (status)
            m_DO_port_status_ = m_DO_port_status_ & 191;
        else
            m_DO_port_status_ = m_DO_port_status_ | 64;
        break;
    case 8:
        if (status)
            m_DO_port_status_ = m_DO_port_status_ & 127;
        else
            m_DO_port_status_ = m_DO_port_status_ | 128;
        break;
    default:
        return;
        break;
    }
    //DO operation
    D2K_DO_WritePort(card_, Channel_P1A, m_DO_port_status_);
}
