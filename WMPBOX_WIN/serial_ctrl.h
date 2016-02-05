#ifndef SERIAL_CTRL_H
#define SERIAL_CTRL_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QTimer>
#include <QMutex>

#include <Apis/CHD_WMP.h>
#include <Apis/CHD_WMP_Audio.h>
#include <Apis/CHD_WMP_Code.h>
#include <Apis/CHD_WMP_Gpio.h>
#include <Apis/CHD_WMP_Return.h>
#include <Apis/CHD_WMP_Serial.h>
#include <Apis/CHD_WMP_Type.h>
#include <Apis/CHD_WMP_Video.h>

namespace Ui {
class Serial_Ctrl;
}

unsigned int atohex(char *hexValue); // 在mainwindow.cpp中实现

class Serial_Ctrl : public QDialog
{
    Q_OBJECT

public:
    explicit Serial_Ctrl(QWidget *parent = 0);
    ~Serial_Ctrl();

    void Active(CHD_WMP_T handler);
    void Inactive();

    void serial_fill();
    void serial_recvdata(char * pdata, int len);

private slots:

    void on_comboBox_speed_activated(const QString &arg1);

    void on_comboBox_databit_activated(const QString &arg1);

    void on_comboBox_stopbit_activated(const QString &arg1);

    void on_comboBox_parity_activated(const QString &arg1);

    void on_comboBox_timeout_activated(const QString &arg1);

    void on_pushButton_serial_send_clicked();



    void on_pushButton_recv_clear_clicked();

    void on_pushButton_send_clear_clicked();

    void on_pushButton_clear_count_clicked();

    void on_checkBox_clicked(bool checked);

    void send_timerout();

private:
    Ui::Serial_Ctrl *ui;

    QMutex mutex;

    CHD_WMP_T handler;

    int send_total;
    int send_interval;
    QTimer sendtimer;

    int recv_total;

    CHD_SERIAL_PARAM_S serial_param;

    void serial_init();


protected:
    void closeEvent(QCloseEvent *e);
};

#endif // SERIAL_CTRL_H
