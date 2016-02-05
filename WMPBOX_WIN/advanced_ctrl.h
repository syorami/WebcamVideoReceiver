#ifndef ADVANCED_CTRL_H
#define ADVANCED_CTRL_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>

#include <Apis/CHD_WMP.h>
#include <Apis/CHD_WMP_Audio.h>
#include <Apis/CHD_WMP_Code.h>
#include <Apis/CHD_WMP_Gpio.h>
#include <Apis/CHD_WMP_Return.h>
#include <Apis/CHD_WMP_Serial.h>
#include <Apis/CHD_WMP_Type.h>
#include <Apis/CHD_WMP_Video.h>

namespace Ui {
class Advanced_ctrl;
}

class Advanced_ctrl : public QDialog
{
    Q_OBJECT

public:
    explicit Advanced_ctrl(QWidget *parent = 0);
    ~Advanced_ctrl();

    void Active(CHD_WMP_T handler);
    void Inactive();

private slots:
    void on_pushButton_synctime_clicked();

    void on_pushButton_setbuff_clicked();

    void on_comboBox_transmode_activated(int index);

    void on_pushButton_setlocalvideobuff_clicked();

private:
    Ui::Advanced_ctrl *ui;

    CHD_WMP_T handler;

    CHD_MAC_S Mac;
    char Encrypt[100];
    CHD_TIME_S stime;
    void advanced_fill();

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // ADVANCED_CTRL_H
