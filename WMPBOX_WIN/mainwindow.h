
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QTimer>
#include <windows.h>
#include <QImage>
#include <QMutex>
#include <QWidget>
#include <QList>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QThread>
#include <QSemaphore>
#include <QDesktopServices>
#include <QUrl>
#include <QTableWidgetItem>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>
#include <QBuffer>

#include <Apis/CHD_WMP.h>
#include <Apis/CHD_WMP_Audio.h>
#include <Apis/CHD_WMP_Code.h>
#include <Apis/CHD_WMP_Gpio.h>
#include <Apis/CHD_WMP_Return.h>
#include <Apis/CHD_WMP_Serial.h>
#include <Apis/CHD_WMP_Type.h>
#include <Apis/CHD_WMP_Video.h>

#include "process_code.h"
#include "custom.h"
#include "videodisplay.h"
#include "imagedecoder.h"
#include "video_ctrl.h"
#include "serial_ctrl.h"
#include "gpio_ctrl.h"
#include "advanced_ctrl.h"
#include "videoabiltiy_view.h"
#include "i2c_ctrl.h"

#include "audioplayback/audio_playback.h"

class RecvThread;
class CMD_Sender;

#define SNAP_PATH       "./snap/"
#define BATCHSNAP_PATH  "./batchsnap/"
#define RECORD_PATH     "./record/"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString GetErrorStr(CHD_RET_E ret);

    void SendCMD_CallBackThread(int cmd);
    void RecvThread_CallBack(int cmd);

    void savepicture(char * path, int index, int type, char * pdata,int len, char * info);

private slots:
    void enable_all();
    void disable_all();

    void Recv_Process(int type, int ret);
    void sendt_response_slot(int cmd, int ret);
    void on_pushButton_Connect_clicked();
    void on_pushButton_Sample_clicked();
    void on_comboBox_format_activated(const QString &arg1);
    void on_comboBox_resolu_activated(const QString &arg1);
    void on_hs_fps_sliderReleased();


    void on_pushButton_snap_clicked();

    void on_pushButton_snapresolu_clicked();

    void on_pushButton_batchsnap_clicked();

    void Devscan_timer_timeout();

    void on_tableWidget_DevScan_clicked(const QModelIndex &index);

    void on_tableWidget_DevScan_doubleClicked(const QModelIndex &index);

    void on_pushButton_dialog_vctrl_clicked();

    void on_pushButton_diglog_gpio_clicked();

    void on_pushButton_diglog_serial_clicked();

    void on_pushButton_record_clicked();

    void on_pushButton_diglog_advanced_clicked();

    void on_pushButton_videoabilityvidew_clicked();

    void on_pushButton_diglog_i2c_clicked();

    void on_pushButton_Web_clicked();

    void on_pushButton_Web_2_clicked();

    void on_pushButton_diglog_audio_clicked();

private:
    Ui::MainWindow *ui;

#ifdef H264_SUPPORT
    QFile h264file;
#endif

    QFile sourceFile;   // class member.
    QBuffer qbuf;
    QAudioOutput* audio; // class member.

    video_ctrl         * vcd;
    Serial_Ctrl        * scd;
    Gpio_Ctrl          * gcd;
    Advanced_ctrl      * acd;
    VideoAbiltiy_view  * vacd;
    I2C_Ctrl           * i2cd;

    QPixmap disconnectled;
    QPixmap connectled;

    bool audioisSample;
    bufferPlayback * audioplay;

    CHD_WMP_T handler;
    bool isconnected;
    bool isSample;
    VideoDisplay * VideoDisplay_Dialog;

    ImageDecoder * imagedecoder[2];
    QThread * imagedecoderthread[2];

    CHD_VIDEO_ABILITY_S ability;
    CHD_VIDEO_PARAM_S param;

    void fill_param();
    void clear_param();

    int temp_format;
    int temp_resolu;
    int temp_fps;

    int totalframe;

    /* *************** 设备扫描 Begin *************** */
    QTableWidgetItem QTable_Devip[20];
    QTableWidgetItem QTable_DevAlias[20];
    QTableWidgetItem QTable_DevId[20];
    QTimer Devscan_timer;
    /* *************** 设备扫描 End *************** */

    void fill_realparam(CHD_VIDEO_FRAME_S * vsp);
    void clear_realparam();

	// send and recv thread
    QSemaphore sem;
    bool recvthread_runflag;
    RecvThread * recvt;

    QThread * sendThread;
    CMD_Sender * sendcmd;

    int maxresolu;

    int batchsnap;
    int record;

    /* move */
    void mousePressEvent( QMouseEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent * event);

    QPoint mousePosition;
    bool isMousePressed;
    QPoint oldMousepot;
    QPoint oldMainwpot;

signals:
    void sendt_response_signal(int cmd, int ret);	// send thread response after process
    void sendt_cmd_signal(int cmd);	// activate the send thread
    void Recv_Process_Signal(int type, int ret);		// recvt thread signal

    void imageDecode_signal1(unsigned int handler, CHD_VIDEO_FRAME_S * videopacket);
    void imageDecode_signal2(unsigned int handler, CHD_VIDEO_FRAME_S * videopacket);
};


/********************** Send and Recv Thread ********************/

class RecvThread : public QThread
{
    Q_OBJECT
public:
    RecvThread(QObject *parent = 0):QThread(parent){}
public slots:
    void run() {((MainWindow*)(parent()))->RecvThread_CallBack(1);}
};

class CMD_Sender : public QObject
{
    Q_OBJECT

public:
    CMD_Sender(QObject *parent = 0):QObject(){parent1 = parent;}
public slots:
    void sendt_cmd_slot(int cmd){
        mutex.lock();
        ((MainWindow*)parent1)->SendCMD_CallBackThread(cmd);
        mutex.unlock();
    }
private:
    QMutex mutex;
    QObject *parent1;
};

/********************** Send and Recv Thread ********************/

#endif // MAINWINDOW_H
