#ifndef VIDEOABILTIY_VIEW_H
#define VIDEOABILTIY_VIEW_H

#include <QDialog>

#include <Apis/CHD_WMP.h>
#include <Apis/CHD_WMP_Audio.h>
#include <Apis/CHD_WMP_Code.h>
#include <Apis/CHD_WMP_Gpio.h>
#include <Apis/CHD_WMP_Return.h>
#include <Apis/CHD_WMP_Serial.h>
#include <Apis/CHD_WMP_Type.h>
#include <Apis/CHD_WMP_Video.h>


namespace Ui {
class VideoAbiltiy_view;
}

class VideoAbiltiy_view : public QDialog
{
    Q_OBJECT

public:
    explicit VideoAbiltiy_view(QWidget *parent = 0);
    ~VideoAbiltiy_view();

    void Active(CHD_WMP_T handler);
    void Inactive();
    void videoabilty_fill();

private:
    Ui::VideoAbiltiy_view *ui;

    CHD_WMP_T handler;



protected:
    void closeEvent(QCloseEvent *e);
};

#endif // VIDEOABILTIY_VIEW_H
