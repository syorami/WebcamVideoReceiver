#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QDialog>
#include <QPoint>
#include <QThread>
#include <QMouseEvent>
#include <Apis/CHD_WMP.h>
#include <Apis/CHD_WMP_Audio.h>
#include <Apis/CHD_WMP_Code.h>
#include <Apis/CHD_WMP_Gpio.h>
#include <Apis/CHD_WMP_Return.h>
#include <Apis/CHD_WMP_Serial.h>
#include <Apis/CHD_WMP_Type.h>
#include <Apis/CHD_WMP_Video.h>


namespace Ui {
class VideoDisplay;
}

class VideoDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit VideoDisplay(QWidget *parent = 0);
    ~VideoDisplay();



    void setImageSize(int w , int h);

private:
    Ui::VideoDisplay *ui;

    void mousePressEvent( QMouseEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent * event);

    bool doubledisplay;
    int fullscreen_index;

    QPoint mousePosition;
    bool isMousePressed;
    QPoint oldMousepot;
    QPoint oldMainwpot;

protected:
    void mouseDoubleClickEvent( QMouseEvent * e );

public slots:
    void Display_draw(QImage * image, int index, int format, int w, int h, int fps);

private slots:
    void on_pushButton_clicked();
};

#endif // VIDEODISPLAY_H
