#ifndef GPIO_CTRL_H
#define GPIO_CTRL_H

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
class Gpio_Ctrl;
}

class Gpio_Ctrl : public QDialog
{
    Q_OBJECT

public:
    explicit Gpio_Ctrl(QWidget *parent = 0);
    ~Gpio_Ctrl();

    void Active(CHD_WMP_T handler);
    void Inactive();
    void gpio_fill();

private slots:
    void gpio_set();

private:
    Ui::Gpio_Ctrl *ui;

     CHD_WMP_T handler;

     /*********************************************/
     QList<QCheckBox *> QCheckBox_GPIO_list;
     int state[GPIO_MAX];
     void gpio_init();

     /*********************************************/

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // GPIO_CTRL_H
