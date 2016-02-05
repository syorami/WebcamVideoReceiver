#ifndef VIDEO_CTRL_H
#define VIDEO_CTRL_H

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
class video_ctrl;
}

class video_ctrl : public QDialog
{
    Q_OBJECT

public:
    explicit video_ctrl(QWidget *parent = 0);
    ~video_ctrl();

    void Active(CHD_WMP_T handler);
    void Inactive();
    void vctrl_fill();
private slots:

    void on_pushButton_vctrl_exit_clicked();

    void on_pushButton_vctrl_rest_clicked();

    void vctrl_set();


private:
    Ui::video_ctrl *ui;

    CHD_WMP_T handler;

    /*********************************************/
    CHD_VIDEO_CTRL_S temp_vctrl;
    CHD_VIDEO_CTRL_S vctrl[CHD_VCTRL_MAX];
    QList<QString> QString_List;
    QList<QLabel *> QLabel_list;
    QList<QSlider *> QHS_list;
    QList<QSpinBox *>  QSpinBox_list;
    QList<QCheckBox *> QCheckBox_list;
    void vctrl_init();

    /*********************************************/

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // VIDEO_CTRL_H
