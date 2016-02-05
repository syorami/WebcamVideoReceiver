#ifndef I2C_CTRL_H
#define I2C_CTRL_H

#include <QDialog>
#include <QStringList>
#include <QMessageBox>

#include <Apis/CHD_WMP.h>
#include <Apis/CHD_WMP_Audio.h>
#include <Apis/CHD_WMP_Code.h>
#include <Apis/CHD_WMP_Gpio.h>
#include <Apis/CHD_WMP_Return.h>
#include <Apis/CHD_WMP_Serial.h>
#include <Apis/CHD_WMP_Type.h>
#include <Apis/CHD_WMP_Video.h>
#include <Apis/CHD_WMP_I2C.h>

namespace Ui {
class I2C_Ctrl;
}

unsigned int atohex(char *hexValue); // 在mainwindow.cpp中实现

class I2C_Ctrl : public QDialog
{
    Q_OBJECT

public:
    explicit I2C_Ctrl(QWidget *parent = 0);
    ~I2C_Ctrl();

    void Active(CHD_WMP_T handler);
    void Inactive();

private:
    Ui::I2C_Ctrl *ui;

     CHD_WMP_T handler;

protected:
    void closeEvent(QCloseEvent *e);
private slots:
    void on_pushButton_write_clicked();
    void on_pushButton_read_clicked();
};

#endif // I2C_CTRL_H
