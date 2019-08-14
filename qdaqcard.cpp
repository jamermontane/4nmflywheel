#include "qdaqcard.h"

QDaqcard::QDaqcard(QObject *parent) : QObject(parent)
{

}

QDaqcard::~QDaqcard()
{
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
        err = D2K_AI_CH_Config (card_, i, AD_B_10_V);
    }
    if (err!=0) {
        logMsg(tr("D2K_AI_CH_Config error=%1").arg(QString::number(err)));
        return;
    }
    is_init_ = true;
}

void QDaqcard::readAllChannel()
{
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

void QDaqcard::getStartCurrent(int channel)
{
    /*
    I16 card, err, card_num,i,Id, tmpId = 0, card_type=0;
        BOOLEAN halfReady, fStop, fok=0;
        U32 count=0, count1, startPos;

        printf("This program inputs %d scans from CH-0 of DAQ-2005/2006 in %d Hz.\n", SCANCOUNT, (int)(BASETIME/SCAN_INTERVAL));
        printf("Card Type: (0) DAQ_2005 or (1) DAQ2006 or (2) DAQ2016 ? ");
        scanf(" %hd", &card_type);
        printf("Please input a card number: ");
        scanf(" %hd", &card_num);
        if(card_type==1) card_type = DAQ_2006;
        else if(card_type==2) card_type = DAQ_2016;
        else card_type = DAQ_2005;
        if ((card=D2K_Register_Card (card_type, card_num)) <0 ) {
            printf("Register_Card error=%d", card);
            exit(1);
        }
        printf("Save data to a file (DmaData.dat): Yes(1) or No(0)? ");
        scanf(" %hd", &fok);
        err = D2K_AI_CH_Config (card, CHANNELNUMBER, AIRANGE);
        if (err!=NoError) {
           printf("D2K_AI_CH_Config error=%d", err);
           exit(1);
        }
        err = D2K_AI_Config (card, ADCONVERTSRC, ADTRIGSRC|ADTRIGMODE|ADTRIGPOL|DELAYMODE, DELAYCOUNT, 0, 0, BUFAUTORESET);
        //Or you can use the following function
        //err = D2K_AI_DelayTrig_Config (card, ADCONVERTSRC, ADTRIGSRC|ADTRIGPOL|DELAYMODE, DELAYCOUNT, 0, 0, BUFAUTORESET);
        if (err!=0) {
           printf("D2K_AI_Config error=%d", err);
           exit(1);
        }
        err=D2K_AI_AsyncDblBufferMode (card, 1);
        if (err!=NoError) {
           printf("D2K_AI_AsyncDblBufferMode error=%d", err);
           exit(1);
        }
        if(fok) {
            svfile = fopen("DmaData.dat", "w");
            fprintf( svfile, "CH%d :\n", CHANNELNUMBER);
        }
        err=D2K_AI_ContBufferSetup (card, ai_buf, SCANCOUNT, &Id);
        err=D2K_AI_ContBufferSetup (card, ai_buf2, SCANCOUNT, &Id);
        err = D2K_AI_ContReadChannel (card, CHANNELNUMBER, Id, SCANCOUNT, SCAN_INTERVAL, SCAN_INTERVAL, ASYNCH_OP);
        if (err!=0) {
           printf("D2K_AI_ContReadChannel error=%d", err);
           exit(1);
        }
        printf("\n\n\nStart Data Conversion by External Trigger Signal\nAnd Press any key to stop Opeartion.\n");
        printf("\n\nData count : \n");
        do {
            do {
                 D2K_AI_AsyncDblBufferHalfReady(card, &halfReady, &fStop);
            } while (!halfReady && !fStop);

            //Here to handle the data stored in ready buffer
            count += (SCANCOUNT);
            printf("%d\r", count);
            if(fok)
               write_to_file( tmpId?ai_buf2:ai_buf, SCANCOUNT );
            tmpId = (tmpId + 1) % 2;
        } while(!kbhit());
        D2K_AI_AsyncClear(card, &startPos, &count1);
        if(fok) {
         write_to_file( tmpId?ai_buf2:ai_buf, count1 );
         fclose(svfile);
        }
        */
}
