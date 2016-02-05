#include "serial_ctrl.h"
#include "ui_serial_ctrl.h"



Serial_Ctrl::Serial_Ctrl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Serial_Ctrl)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::Tool);
    setFixedSize(575, 305);
    ui->radioButton_recv_ascii->setChecked(true);
    ui->radioButton_send_ascii->setChecked(true);

    ui->plainTextEdit_recv->setReadOnly(true);
    serial_init();

    connect(&sendtimer, SIGNAL(timeout()), this, SLOT(send_timerout()));

    lower();
}

Serial_Ctrl::~Serial_Ctrl()
{
    delete ui;
}

void Serial_Ctrl::Active(CHD_WMP_T handler)
{
    this->handler = handler;
    on_pushButton_clear_count_clicked();
    CHD_WMP_Serial_Begin(handler);
    show();
    serial_fill();
}

void Serial_Ctrl::Inactive()
{
    hide();
    on_checkBox_clicked(false);
    CHD_WMP_Serial_End(handler);
}

void Serial_Ctrl::serial_init()
{

}

void Serial_Ctrl::serial_fill()
{
    if(this->isVisible() == false){
        return ;
    }

    CHD_RET_E ret = CHD_WMP_Serial_GetParam(handler, &serial_param);
    if(ret != CHD_RET_SUCCESS){
        return ;
    }

    for(int i = 0; i < 10; i++){
        if(serial_param.speed ==  ui->comboBox_speed->itemText(i).toInt()){
            ui->comboBox_speed->setCurrentIndex(i);
            break;
        }
    }

    for(int i = 0; i < 2; i++){
        if(serial_param.databit ==  ui->comboBox_databit->itemText(i).toInt()){
            ui->comboBox_databit->setCurrentIndex(i);
            break;
        }
    }


    for(int i = 0; i < 2; i++){
        if(serial_param.stopbit ==  ui->comboBox_stopbit->itemText(i).toInt()){
            ui->comboBox_stopbit->setCurrentIndex(i);
            break;
        }
    }

    for(int i = 0; i < 4; i++){
        if(serial_param.parity ==  ui->comboBox_parity->itemText(i).toInt()){
            ui->comboBox_parity->setCurrentIndex(i);
            break;
        }
    }

}

void Serial_Ctrl::on_comboBox_speed_activated(const QString &arg1)
{
    CHD_RET_E ret = CHD_WMP_Serial_SetSpeed(handler, (CHD_SERIAL_SPEED_E)arg1.toInt());
    if(ret != CHD_RET_SUCCESS){

    }
}

void Serial_Ctrl::on_comboBox_databit_activated(const QString &arg1)
{
    CHD_RET_E ret = CHD_WMP_Serial_SetDataBit(handler, (CHD_SERIAL_DATABIT_E)arg1.toInt());
    if(ret != CHD_RET_SUCCESS){

    }
}

void Serial_Ctrl::on_comboBox_stopbit_activated(const QString &arg1)
{
    CHD_RET_E ret = CHD_WMP_Serial_SetStopBit(handler, (CHD_SERIAL_STOPBIT_E)arg1.toInt());
    if(ret != CHD_RET_SUCCESS){

    }
}


void Serial_Ctrl::on_comboBox_parity_activated(const QString &arg1)
{
    CHD_RET_E ret = CHD_WMP_Serial_SetParity(handler, (CHD_SERIAL_PARITY_E)arg1.toLocal8Bit().data()[0]);
    if(ret != CHD_RET_SUCCESS){

    }
}

void Serial_Ctrl::on_comboBox_timeout_activated(const QString &arg1)
{
        QStringList list2;

        list2.append(arg1.split("ms"));

        int ms = list2.at(0).toInt(0);
        ms /= 100;      // 换算成百毫秒为单位
        CHD_RET_E ret = CHD_WMP_Serial_SetTimeout(handler, ms);

        if(ret != CHD_RET_SUCCESS){

        }
}

void Serial_Ctrl::on_pushButton_serial_send_clicked()
{
    CHD_RET_E ret;

    if(ui->radioButton_send_hex->isChecked() == true){
        unsigned char * buffer = new unsigned char[ui->plainTextEdit_send->toPlainText().length()];
        QStringList list;
        list.append(ui->plainTextEdit_send->toPlainText().split(" "));

        for(int i = 0 ;i < list.length(); i++){
            buffer[i] = atohex(list.at(i).toLocal8Bit().data());
   //         qDebug("v = %u", buffer[i]);
        }
        ret = CHD_WMP_Serial_SendData(handler, (const char *)buffer, list.length());
        if(ret == CHD_RET_SUCCESS){
            send_total += list.length();
        }

        delete buffer;
    }
    else{
        ret = CHD_WMP_Serial_SendData(handler, ui->plainTextEdit_send->toPlainText().toLocal8Bit().data(),
                                  ui->plainTextEdit_send->toPlainText().length());
        if(ret == CHD_RET_SUCCESS){
        //  qDebug("Send %d", ui->plainTextEdit_send->toPlainText().length());
            send_total += ui->plainTextEdit_send->toPlainText().length();
            ui->label_sendtotal->setText(QString::number(send_total));
        }
        else{
            qDebug("serial send error ret = %#x", ret);
        }
    }
}

void Serial_Ctrl::serial_recvdata(char * pdata, int len)
{
    static int dis_total = 0;

    if(this->isVisible() == false){
        return ;
    }

    recv_total += len;
    ui->label_recvtotal->setText(QString::number(recv_total));

    if(ui->radioButton_recv_ascii->isChecked() == true){
        pdata[len] = '\0';
        ui->plainTextEdit_recv->insertPlainText((char*)pdata);
    }
    else{
        unsigned char * b = new unsigned char[len*5];
        unsigned char * p = b;
        for(int i = 0 ;i < len; i++){
            sprintf((char*)p, "%02x ", (unsigned char)pdata[i]);
            p += 3;
        }
        ui->plainTextEdit_recv->insertPlainText((char*)b);
        delete b;
    }

    dis_total +=len;
    if(dis_total > 10240){
        dis_total = 0;
        ui->plainTextEdit_recv->clear();
    }


    ui->plainTextEdit_recv->moveCursor(QTextCursor::End);

}

void Serial_Ctrl::on_pushButton_recv_clear_clicked()
{
    ui->plainTextEdit_recv->clear();
    recv_total = 0;
    ui->label_recvtotal->setText(QString::number(recv_total));
}

void Serial_Ctrl::on_pushButton_send_clear_clicked()
{
    ui->plainTextEdit_send->clear();
    send_total = 0;
    ui->label_sendtotal->setText(QString::number(send_total));
}

void Serial_Ctrl::on_pushButton_clear_count_clicked()
{
    on_pushButton_recv_clear_clicked();
    on_pushButton_send_clear_clicked();
}

void Serial_Ctrl::closeEvent(QCloseEvent *e)
{
    Inactive();
}

void Serial_Ctrl::send_timerout()
{
    on_pushButton_serial_send_clicked();
}

void Serial_Ctrl::on_checkBox_clicked(bool checked)
{
    if(checked == true){
        sendtimer.start(ui->spinBox_timerout->value());
        ui->spinBox_timerout->setEnabled(false);
    }
    else{
        sendtimer.stop();
        ui->spinBox_timerout->setEnabled(true);
    }
}
