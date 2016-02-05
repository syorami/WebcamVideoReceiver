#include "advanced_ctrl.h"
#include "ui_advanced_ctrl.h"
#include <QDateTime>


Advanced_ctrl::Advanced_ctrl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Advanced_ctrl)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::Tool);
    setFixedSize(335, 200);

    lower();
}

Advanced_ctrl::~Advanced_ctrl()
{
    delete ui;
}

void Advanced_ctrl::Active(CHD_WMP_T handler)
{
    this->handler = handler;
    advanced_fill();
    show();
}

void Advanced_ctrl::Inactive()
{
    this->hide();
}


void Advanced_ctrl::on_pushButton_synctime_clicked()
{
     CHD_RET_E ret;
     char timebuf[100];

    // 设置系统时间

     QDateTime qtime;
     qtime = QDateTime::currentDateTime();

     CHD_TIME_S stime;
     stime.year = qtime.date().year();
     stime.month = qtime.date().month();
     stime.mday = qtime.date().day();
     stime.wday = qtime.date().dayOfWeek();
     stime.hour = qtime.time().hour();
     stime.min  = qtime.time().minute();
     stime.sec  = qtime.time().second();
 //    qDebug("settime : %d-%d-%d %d:%d:%d", stime.year, stime.month, stime.mday, stime.hour, stime.min,stime.sec);
     ret = CHD_WMP_SetSystemTime(handler, &stime);
     if(ret == CHD_RET_SUCCESS){
         sprintf(timebuf, "%04d-%02d-%02d %02d:%02d:%02d",
                  (int)stime.year, (int)stime.month,(int)stime.mday,(int)stime.hour,(int)stime.min, (int)stime.sec);
         ui->label_time->setText(timebuf);
     }

}

void Advanced_ctrl::advanced_fill()
{
     CHD_RET_E ret;
     char timebuf[100];

     /* MAC */
     ret = CHD_WMP_GetMac(handler, &Mac);
     if(ret == CHD_RET_SUCCESS){
         ui->label_Mac->setText(QString::number(Mac.Mac[0]&0xff,16) + ":"
                                 + QString::number(Mac.Mac[1]&0xff,16) + ":"
                                 + QString::number(Mac.Mac[2]&0xff,16) + ":"
                                 + QString::number(Mac.Mac[3]&0xff,16) + ":"
                                 + QString::number(Mac.Mac[4]&0xff,16) + ":"
                                  + QString::number(Mac.Mac[5]&0xff,16));
    }

    /* time */
    ret = CHD_WMP_GetSystemTime(handler, &stime);
    if(ret == CHD_RET_SUCCESS){
        sprintf(timebuf, "%04d-%02d-%02d %02d:%02d:%02d",
                 (int)stime.year, (int)stime.month,(int)stime.mday,(int)stime.hour,(int)stime.min, (int)stime.sec);
        ui->label_time->setText(timebuf);
    }

    /* encrypt */
    ret = CHD_WMP_GetEncrypt(handler, Encrypt);
    if(ret == CHD_RET_SUCCESS){
        ui->label_encrypt->setText(Encrypt);
    }

    unsigned int Num;
    ret = CHD_WMP_Video_GetPeerMaxFrameNum(handler, &Num);
    if(ret == CHD_RET_SUCCESS){
        ui->lineEdit_buff->setText(QString::number(Num));
    }

    ret = CHD_WMP_Video_GetLocalMaxFrameNum(handler, &Num);
    if(ret== CHD_RET_SUCCESS){
        ui->lineEdit_localvideobuff->setText(QString::number(Num));
    }
}

void Advanced_ctrl::on_pushButton_setbuff_clicked()
{
    CHD_RET_E ret;

    ret = CHD_WMP_Video_SetPeerMaxFrameNum(handler, ui->lineEdit_buff->text().toInt());
    if(ret == CHD_RET_SUCCESS){

    }
}


void Advanced_ctrl::on_pushButton_setlocalvideobuff_clicked()
{
    CHD_RET_E ret;

    ret = CHD_WMP_Video_SetLocalMaxFrameNum(handler, ui->lineEdit_localvideobuff->text().toInt());
    if(ret == CHD_RET_SUCCESS){
    }

}

void Advanced_ctrl::closeEvent(QCloseEvent *e)
{
    Inactive();
}

void Advanced_ctrl::on_comboBox_transmode_activated(int index)
{
    CHD_RET_E ret;

    ret = CHD_WMP_SetTransMode(handler, (CHD_TRANSMODE_E)index);
    if(ret == CHD_RET_SUCCESS){
    }

}

