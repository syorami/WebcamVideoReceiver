#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <windows.h>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <Apis/chd_base.h>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(680, 270);
    isconnected = false;

    connectled.load(":/connect.png ");
    connectled = connectled.scaled(ui->label_connstate_led->size());
    disconnectled.load(":/disconnect.png ");
    disconnectled = disconnectled.scaled(ui->label_connstate_led->size());

    VideoDisplay_Dialog = NULL;

    handler = NULL;

    audioisSample = false;
    audioplay = new bufferPlayback;

    this->setWindowTitle(QString(WINDDOWS_TITLE));
    ui->label_title->setText(this->windowTitle());

#ifndef OFFICAIAL_WEB
    ui->pushButton_Web_2->hide();
#endif

#ifndef WINDOWS_ICO
    ui->label_window_pic->hide();
#else
    QPixmap winmap(WINDOWS_ICO);
    ui->label_window_pic->setPixmap(winmap.scaled(ui->label_window_pic->width(), ui->label_window_pic->height()));
#endif

    VideoDisplay_Dialog = new VideoDisplay(this);
    VideoDisplay_Dialog->hide();
    VideoDisplay_Dialog->setGeometry(this->x(), 50, 640, 480);

    vcd = new video_ctrl(this);         vcd->hide();
    scd = new Serial_Ctrl(this);        scd->hide();
    gcd = new Gpio_Ctrl(this);          gcd->hide();
    acd = new Advanced_ctrl(this);      acd->hide();
    vacd = new VideoAbiltiy_view(this); vacd->hide();
    i2cd = new I2C_Ctrl(this);          i2cd->hide();

    recvthread_runflag = true;
    recvt = new RecvThread(this);
    recvt->start();
    connect(this, SIGNAL(Recv_Process_Signal(int, int)), this, SLOT(Recv_Process(int, int)));

    // 发送线程
    sendcmd = new CMD_Sender(this);
    sendThread = new QThread(this);
    sendcmd->moveToThread(sendThread);
    sendThread->start();
    // 线程发送命令
    connect(this, SIGNAL(sendt_cmd_signal(int)), sendcmd, SLOT(sendt_cmd_slot(int)));
    // 发送响应处理
    connect(this, SIGNAL(sendt_response_signal(int, int)), this, SLOT(sendt_response_slot(int, int)));

    // 创建2个解码线程
    for(int i = 0; i < 2; i++){
        imagedecoder[i] = new ImageDecoder();
        imagedecoderthread[i] = new QThread(this);
        imagedecoder[i]->moveToThread(imagedecoderthread[i]);
        imagedecoderthread[i]->start();

        connect(imagedecoder[i], SIGNAL(imagetoDisplay(QImage*,int,int,int,int,int)), VideoDisplay_Dialog, SLOT(Display_draw(QImage*,int,int,int,int,int)));
    }
    connect(this,SIGNAL(imageDecode_signal1(uint,CHD_VIDEO_FRAME_S*)), imagedecoder[0], SLOT(imageDecode(unsigned int, CHD_VIDEO_FRAME_S*)));
    connect(this,SIGNAL(imageDecode_signal2(uint,CHD_VIDEO_FRAME_S*)), imagedecoder[1], SLOT(imageDecode(unsigned int, CHD_VIDEO_FRAME_S*)));

    clear_param();
    clear_realparam();
    disable_all();

    batchsnap = 0;
    record = 0;

    /* *************** 设备扫描 Begin *************** */
    CHD_WMP_ScanDevice_Init(2);
    ui->tableWidget_DevScan->setRowCount(20);
    ui->tableWidget_DevScan->setColumnWidth(0,100);
    ui->tableWidget_DevScan->setColumnWidth(1,32);
    ui->tableWidget_DevScan->setColumnWidth(2,72);
    for(int idx = 0; idx < 20; idx++){
        ui->tableWidget_DevScan->setRowHeight(idx, 25);
        ui->tableWidget_DevScan->setItem(idx, 0, &QTable_Devip[idx]);
        ui->tableWidget_DevScan->setItem(idx, 1, &QTable_DevId[idx]);
        ui->tableWidget_DevScan->setItem(idx, 2, &QTable_DevAlias[idx]);
        ui->tableWidget_DevScan->setRowHidden(idx, true);
    }
    ui->tableWidget_DevScan->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_DevScan->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(&Devscan_timer, SIGNAL(timeout()), this, SLOT(Devscan_timer_timeout()));
    Devscan_timer.start(500);
    /* *************** 设备扫描 End *************** */

    connect(ui->hs_fps, SIGNAL(sliderMoved(int)), ui->spinBox_fps, SLOT(setValue(int)));
}


MainWindow::~MainWindow()
{
    if(isconnected == true){
         CHD_WMP_Disconnect(handler);
    }

    for(int i= 0 ;i < 2; i++){
        imagedecoderthread[i]->quit();
        imagedecoderthread[i]->wait();
        delete imagedecoderthread[i];
        delete imagedecoder[i];
    }

    sendThread->quit();
    sendThread->wait();
    delete sendThread;
    delete sendcmd;

    audioisSample = false;
    delete audioplay;
    audioplay = NULL;

    isSample = 0;
    recvthread_runflag = 0;
    recvt->quit();
    recvt->wait();
    delete recvt;

    disable_all();
    delete vcd;
    delete scd;
    delete gcd;
    delete acd;
    delete vacd;
    delete i2cd;

    delete ui;
}

QString MainWindow::GetErrorStr(CHD_RET_E ret)
{
    switch((int)ret){
    case CHD_RET_FAILED:
        return QString("操作失败，可能没有该设备存在  ");
    case CHD_RET_TIMEOUT:
        return QString("操作超时，请检查网络  ");
    case CHD_RET_DISCONNECT:
        return QString("远端断开连接  ");
    case CHD_RET_CONNECT_ERROR:
        return QString("连接错误，请检查设备和网络  ");
    }

    qDebug("ret = %d", ret);
    return QString("无效的错误码  ");

}

void MainWindow::SendCMD_CallBackThread(int cmd)
{
    CHD_RET_E ret = CHD_RET_SUCCESS;
    switch(cmd){
    case VIDEO_CONNECT:
        ret = CHD_WMP_ConnectDevice(&handler, ui->lineEdit_IP->text().toLocal8Bit().data()); break;
    case VIDEO_DISCONNECT:
        ret = CHD_WMP_Disconnect(handler); break;
    case VIDEO_STARTDATA:
        ret = CHD_WMP_Video_Begin(handler);  break;
    case VIDEO_STOPDATA:
        ret = CHD_WMP_Video_End(handler);   break;
    case VIDEO_SETFORMAT:
        ret = CHD_WMP_Video_SetFormat(handler, (CHD_VIDEO_FMT_E)ability.format[ui->comboBox_format->currentIndex()].format);
        break;
    case VIDEO_SETRESOLU:
    {
        QStringList list2;
        list2.append(ui->comboBox_resolu->currentText().split("x"));

        unsigned int w = list2.at(0).toInt();
        unsigned int h = list2.at(1).toInt();
        ret = CHD_WMP_Video_SetResolu(handler, CHD_Video_GroupResolu(w, h));
        break;
    }
    case VIDEO_SETFPS:
        ret = CHD_WMP_Video_SetFPS(handler, ui->spinBox_fps->value()); break;
    case VIDEO_SNAP:
        ret = CHD_WMP_Video_SnapShot(handler); break;
    case VIDEO_SNAPMAX:
        ret = CHD_WMP_Video_SnapShotResolu(handler, maxresolu); break;
    case AUDIO_BEGIN:
        ret = CHD_WMP_Audio_Begin(handler);  break;
    case AUDIO_END:
        ret = CHD_WMP_Audio_End(handler); break;
    default: break;
    }

    emit sendt_response_signal(cmd, ret);
}

void MainWindow::sendt_response_slot(int cmd, int ret)
{
    switch(cmd){
    case VIDEO_CONNECT:
        if(ret == CHD_RET_SUCCESS){
            isSample = false;
            totalframe = 0;
            ui->frame_ctrl->setEnabled(true);
            ui->groupBox_realparam->setEnabled(true);
            fill_param();

            ui->pushButton_Sample->setEnabled(true);

            ui->pushButton_Connect->setText(QString("断开设备 "));
            ui->label_connstate_led->setPixmap(connectled);
            isconnected = true;

            ui->pushButton_Sample->setText(QString("视频启动"));
            if(ui->checkBox_autostart->isChecked() == true){
                on_pushButton_Sample_clicked();
            }

            audioisSample = false;
            ui->pushButton_diglog_audio->setText(QString("音频启动"));

            enable_all();
        }
        else{
            QMessageBox::information(this,  QString("连接错误 "),
                                               GetErrorStr((CHD_RET_E)ret),
                                               QMessageBox::Yes);
        }
        ui->pushButton_Connect->setEnabled(true);
        break;
    case VIDEO_DISCONNECT:
        if(record){
            on_pushButton_record_clicked();
        }

        ui->pushButton_Connect->setText(QString("连接设备 "));
        ui->label_connstate_led->setPixmap(disconnectled);

        clear_param();
        clear_realparam();
        disable_all();
        break;

    case VIDEO_STARTDATA:
        if(ret == CHD_RET_SUCCESS){
            isSample = true;
            ui->pushButton_Sample->setText(QString("视频停止 "));
        }
        ui->pushButton_Sample->setEnabled(true);
        break;
    case VIDEO_STOPDATA:
        if(ret == CHD_RET_SUCCESS){
            isSample = false;
            ui->pushButton_Sample->setText(QString("视频启动 "));
        }

        ui->pushButton_Sample->setEnabled(true);
        break;

    case VIDEO_SETFORMAT:
        if(ret == CHD_RET_SUCCESS){
            CHD_WMP_Video_GetParam(handler, &param);
            fill_param();
        }
        ui->comboBox_format->setEnabled(true);
        break;

    case VIDEO_SETRESOLU:
        if(ret == CHD_RET_SUCCESS){
             CHD_WMP_Video_GetParam(handler, &param);
        }
        fill_param();
        ui->comboBox_resolu->setEnabled(true);
        break;

    case VIDEO_SETFPS:
        if(ret == CHD_RET_SUCCESS){
            CHD_WMP_Video_GetParam(handler, &param);
        }
        fill_param();
        ui->hs_fps->setEnabled(true);
        break;
    case VIDEO_SNAP:
        if(ret == CHD_RET_SUCCESS){

        }
        ui->pushButton_snap->setEnabled(true);
        break;
    case VIDEO_SNAPMAX:
        if(ret == CHD_RET_SUCCESS){
        }
        ui->pushButton_snapresolu->setEnabled(true);
        break;
    case AUDIO_BEGIN:
    {
        if(ret == CHD_RET_SUCCESS){
            audioisSample = true;
            ui->pushButton_diglog_audio->setText(QString("音频停止 "));
        }
        else{
            QMessageBox::information(this,  QString("音频错误 "),
                                               QString("音频打开错误，可能没音频设备 "),
                                               QMessageBox::Yes);
        }
        ui->pushButton_diglog_audio->setEnabled(true);
        break;
    }
    case AUDIO_END:
    {
        if(ret == CHD_RET_SUCCESS){
            audioisSample = false;
            ui->pushButton_diglog_audio->setText(QString("音频开启 "));
        }
        else{
        }
        ui->pushButton_diglog_audio->setEnabled(true);
        break;
    }
    default: break;
    }
}

void  MainWindow::savepicture(char * path, int index, int type, char * pdata,int len, char * info)
{
    QDir dirtemp;
    QFile file;

    /* check directory is exist */
    if(dirtemp.exists(path) == false){
        dirtemp.mkdir(path);
    }

    if(type == CHD_FMT_YUYV){
        file.setFileName(QString("%1%2_%3ms.yuyv").arg(path).arg(info).arg(index));
    }
    else if(type == CHD_FMT_MJPEG){
       file.setFileName(QString("%1%2_%3ms.jpg").arg(path).arg(info).arg(index));
    }

    file.open(QIODevice::WriteOnly);
    file.write(pdata, (unsigned int)len);
    file.close();
}

void MainWindow::RecvThread_CallBack(int cmd)
{
    CHD_RET_E ret;
    CHD_STREAM_TYPE_E type;

    qDebug(" ============== RecvThread_CallBack enter %x =============== ", QThread::currentThreadId());

    while(recvthread_runflag){
        if(handler != 0){
            ret = CHD_WMP_Poll(handler, &type, 0, 250);

            if(ret != CHD_RET_SUCCESS){

                /* 断开连接*/
                if(ret == CHD_RET_DISCONNECT){
                    on_pushButton_Connect_clicked();
                    continue;
                }

                Sleep(100);
                continue;
            }

           emit Recv_Process_Signal(type, ret);
        }
        else{
            Sleep(100);
            continue;
        }

    }
    qDebug(" ============== RecvThread_CallBack exit ===============  ");
}


void MainWindow::Recv_Process(int type, int ret)
{
    char buf[12];
    char * serialpacket;
    unsigned int len;
    CHD_AUDIO_FRAME_S * audiopacket;
    CHD_VIDEO_FRAME_S * videopacket;

    if(CHD_STREAM_VIDEO == type){

        ret = CHD_WMP_Video_RequestVideoData(handler, &videopacket);

        if(ret == CHD_RET_SUCCESS){

            if(VideoDisplay_Dialog->isVisible() == false && isSample == true){
               VideoDisplay_Dialog->show();
            }

            // 显示实时数据
            fill_realparam(videopacket);

            // 批量抓拍
            if(batchsnap == 1){
                sprintf(buf, "%dx%d",  videopacket->resolu>>16&0xFFFF, videopacket->resolu&0xFFFF);
                savepicture((char*)BATCHSNAP_PATH,videopacket->timestamp,videopacket->format,(char*)videopacket->pData, videopacket->datalen, buf);
            }
            // 录像
            if(record){
                if(videopacket->format == CHD_FMT_MJPEG)     CHD_WMP_Mjpeg2Avi_PutData(handler, videopacket->pData, (unsigned int)videopacket->datalen);
#ifdef H264_SUPPORT
                else if(videopacket->format == CHD_FMT_H264) h264file.write(videopacket->pData, (unsigned int)videopacket->datalen);
#endif
            }

            // 双线程解码
            if(videopacket->bexist == 1 || videopacket->bexist == 0){
                emit imageDecode_signal1(handler, videopacket);
            }
            else if(videopacket->bexist == 2){
                emit imageDecode_signal2(handler, videopacket);
            }

            // 放到解码完毕之后去释放
       //    CHD_WMP_Video_ReleaseVideoData(handler, videopacket);
        }
    }
    // 图片处理
    else if(CHD_STREAM_PICTURE == type){

        CHD_RET_E ret = CHD_WMP_Video_RequestPicData(handler, &videopacket);
        if(ret == CHD_RET_SUCCESS){
             // 保存文件
            sprintf(buf, "%dx%d",  videopacket->resolu>>16&0xFFFF, videopacket->resolu&0xFFFF);
            savepicture((char *)SNAP_PATH, videopacket->timestamp,videopacket->format,(char*)videopacket->pData, videopacket->datalen,buf);

            CHD_WMP_Video_ReleaseVideoData(handler, videopacket);
        }
    }
    // 音频处理
    else if(CHD_STREAM_AUDIO == type){
        CHD_RET_E  ret = CHD_WMP_Audio_RequestData(handler, &audiopacket);
        if(ret == CHD_RET_SUCCESS){
           audioplay->play(audiopacket->pData, audiopacket->datalen);
           CHD_WMP_Audio_ReleaseData(handler, audiopacket);
        }
    }
    // 串口处理
    else if(CHD_STREAM_SERIAL == type){
        CHD_RET_E ret = CHD_WMP_Serial_RequestData(handler, (void**)&serialpacket, &len);
        if(ret == CHD_RET_SUCCESS){
            scd->serial_recvdata(serialpacket, len);
            CHD_WMP_Serial_ReleaseData(handler, serialpacket);
        }
    }
    else if(CHD_PARAM_CHANGE_VABILITY == type){
        vacd->videoabilty_fill();
        this->fill_param();
    }
    else if(CHD_PARAM_CHANGE_VPARAM == type){
        this->fill_param();
    }
    else if(CHD_PARAM_CHANGE_VCTRL == type){
        vcd->vctrl_fill();
    }
    else if(CHD_PARAM_CHANGE_SERIAL == type){
        scd->serial_fill();
    }
    else if(CHD_PARAM_CHANGE_GPIO == type){
        gcd->gpio_fill();
    }

}

char * CHD_Format_GetStr(int format)
{
    switch(format){
    case CHD_FMT_YUYV:  return (char *)"YUYV";
    case CHD_FMT_MJPEG: return (char *)"MJPEG";
    case CHD_FMT_H264:  return (char *)"H264";
    }
    return (char *)"unknown";
}


void MainWindow::fill_param()
{
    int fps;


    CHD_WMP_Video_GetAbility(handler, &ability);
    CHD_WMP_Video_GetParam(handler, &param);
    ui->comboBox_format->clear();
    maxresolu = 0;
    int maxresolu_1 = 0;

    int formatidx = 0;
    for(int i = 0; i < FORMAT_MAX_NUM; i++){
        if(ability.format[i].format == 0)   break;\
        ui->comboBox_format->addItem(CHD_Format_GetStr(ability.format[i].format));
        if(ability.format[i].format == param.format){
            ui->comboBox_format->setCurrentIndex(i);
            formatidx = i;
        }
    }

    ui->comboBox_resolu->clear();
    int resoluidex = 0;
    for(int i = 0; i < RESOLU_MAX_NUM; i++){
        if(ability.format[formatidx].resolu[i].resolu == 0) break;

        unsigned int w, h;
        CHD_Video_AnalysisResolu(ability.format[formatidx].resolu[i].resolu, &w, &h);

        if(w * h > maxresolu_1){
            maxresolu = ability.format[formatidx].resolu[i].resolu;
            maxresolu_1 = w * h;
        }


        ui->comboBox_resolu->addItem(QString::number(w) + "x" + QString::number(h));
        if(ability.format[formatidx].resolu[i].resolu == param.resolu){
            ui->comboBox_resolu->setCurrentIndex(i);
            resoluidex = i;
        }
    }

    param.maxfps  = ability.format[formatidx].resolu[resoluidex].maxfps;
    fps = (param.maxfps < param.fps) ? param.maxfps : param.fps;
    ui->hs_fps->setMinimum(1);
    ui->hs_fps->setMaximum(param.maxfps);
    ui->hs_fps->setValue(fps);
    ui->spinBox_fps->setValue(fps);
    ui->label_maxfps->setText(QString::number(param.maxfps));
}

void MainWindow::clear_param()
{
     ui->frame_ctrl->setEnabled(false);
    ui->comboBox_format->clear();
    ui->hs_fps->setValue(0);
    ui->comboBox_resolu->clear();
    ui->spinBox_fps->setValue(0);
    ui->label_minfps->setText(QString::number(0));
    ui->label_maxfps->setText(QString::number(0));
}

void MainWindow::fill_realparam(CHD_VIDEO_FRAME_S * videopacket)
{
    totalframe++;

    static int old_refresh_timesampe = 0;

    if(videopacket->timestamp - old_refresh_timesampe > 100){

        if(videopacket->format == CHD_FMT_YUYV){
            ui->label_format->setText("YUYV ");
        }
        else if(videopacket->format == CHD_FMT_MJPEG){
            ui->label_format->setText("MJPEG ");
        }
        else if(videopacket->format == CHD_FMT_H264){
            ui->label_format->setText("H264 ");
        }

        unsigned int w, h;
        CHD_Video_AnalysisResolu(videopacket->resolu, &w, &h);

        ui->label_resolu->setText(QString::number(w) + "x" + QString::number(h));

        ui->label_fps->setText(QString::number(videopacket->fps) + " FPS ");
        ui->label_totalframe->setText(QString::number(totalframe));

        float size = (float)videopacket->datalen / 1024.0;
        ui->label_len->setText(QString::number(size, 'f', 1) + " KB ");

        ui->label_Bps->setText(QString::number(videopacket->BPS/1024) + " KBps ");

        old_refresh_timesampe = videopacket->timestamp;
    }
}

void MainWindow::clear_realparam()
{
    ui->label_format->setText("_ ");
    ui->label_resolu->setText("_ ");
    ui->label_fps->setText("_ ");
    ui->label_totalframe->setText("_ ");
    ui->label_len->setText("_ ");
    ui->label_Bps->setText("_ ");
}

void MainWindow::on_pushButton_Connect_clicked()
{
    if(isconnected == false){
        emit sendt_cmd_signal(VIDEO_CONNECT);
    }
    else{
        emit sendt_cmd_signal(VIDEO_DISCONNECT);
    }
    ui->pushButton_Connect->setEnabled(false);
}

void MainWindow::on_pushButton_Sample_clicked()
{
    if(isSample == false){
        emit sendt_cmd_signal(VIDEO_STARTDATA);
    }
    else{
        emit sendt_cmd_signal(VIDEO_STOPDATA);
    }
    ui->pushButton_Sample->setEnabled(false);

}

void MainWindow::on_comboBox_format_activated(const QString &arg1)
{
    ui->comboBox_format->setEnabled(false);
    emit sendt_cmd_signal(VIDEO_SETFORMAT);

}

void MainWindow::on_comboBox_resolu_activated(const QString &arg1)
{
    ui->comboBox_resolu->setEnabled(false);
    emit sendt_cmd_signal(VIDEO_SETRESOLU);

}

void MainWindow::on_hs_fps_sliderReleased()
{
    ui->hs_fps->setEnabled(false);

    emit sendt_cmd_signal(VIDEO_SETFPS);
}

void MainWindow::on_pushButton_snap_clicked()
{
    ui->pushButton_snap->setEnabled(false);

    emit sendt_cmd_signal(VIDEO_SNAP);
}


void MainWindow::on_pushButton_snapresolu_clicked()
{
    ui->pushButton_snapresolu->setEnabled(false);
    emit sendt_cmd_signal(VIDEO_SNAPMAX);
}

void MainWindow::on_pushButton_diglog_audio_clicked()
{
    if(audioisSample == true){
        emit sendt_cmd_signal(AUDIO_END);
    }
    else{
        emit sendt_cmd_signal(AUDIO_BEGIN);
    }
    ui->pushButton_diglog_audio->setEnabled(false);

}

void MainWindow::on_pushButton_batchsnap_clicked()
{
    if(batchsnap == 0){
        QDir dirtemp;

        /* check directory is exist */
        if(dirtemp.exists(BATCHSNAP_PATH) == false){
            dirtemp.mkdir(BATCHSNAP_PATH);
        }

        batchsnap = 1;
        ui->pushButton_batchsnap->setText(QString("停止批量抓拍 "));
    }
    else {
        batchsnap = 0;
         ui->pushButton_batchsnap->setText(QString("批量抓拍 "));
    }
}


void MainWindow::enable_all()
{
    ui->frame_videorecord->setEnabled(true);
    ui->groupBox_realparam->setEnabled(true);
    ui->groupBox_Advanced->setEnabled(true);
    ui->frame_ctrl->setEnabled(true);
}

void MainWindow::disable_all()
{
    ui->frame_videorecord->setEnabled(false);
    ui->groupBox_realparam->setEnabled(false);
    ui->groupBox_Advanced->setEnabled(false);
    ui->frame_ctrl->setEnabled(false);

    isconnected = false;
    VideoDisplay_Dialog->hide();
    ui->pushButton_Connect->setEnabled(true);

    if(vcd->isHidden() == false) vcd->Inactive();
    if(scd->isHidden() == false) scd->Inactive();
    if(gcd->isHidden() == false) gcd->Inactive();
    if(acd->isHidden() == false) acd->Inactive();
    if(vacd->isHidden() == false) vacd->Inactive();
    if(i2cd->isHidden() == false) i2cd->Inactive();

}

void MainWindow::Devscan_timer_timeout()
{
    CHD_DEVINFO_S IPAddress[20];
    CHD_U32 Number;
    CHD_WMP_Scan_GetDeviceInfo(IPAddress, &Number);

 //   ui->tableWidget_DevScan->clear();
    for(int i = 0; i < 20; i++){
        if(i >= Number) {
            ui->tableWidget_DevScan->setRowHidden(i, true);
            continue;
        }
        QTable_Devip[i].setText(IPAddress[i].IpAddr);
        QTable_DevId[i].setText(QString::number(IPAddress[i].Id));
        QTable_DevAlias[i].setText(IPAddress[i].Alias);
   //     qDebug("ip = %s %d %s", IPAddress[i].IpAddr, IPAddress[i].Id, IPAddress[i].Alias);
        ui->tableWidget_DevScan->setRowHidden(i, false);
    }

    if(Devscan_timer.interval() != 1000){
        Devscan_timer.start(1000);
    }
}

void MainWindow::on_tableWidget_DevScan_clicked(const QModelIndex &index)
{
     ui->lineEdit_IP->setText(ui->tableWidget_DevScan->item(index.row(), 0)->text());
}

void MainWindow::on_tableWidget_DevScan_doubleClicked(const QModelIndex &index)
{
    ui->lineEdit_IP->setText(ui->tableWidget_DevScan->item(index.row(), 0)->text());
    on_pushButton_Connect_clicked();
}


void MainWindow::on_pushButton_dialog_vctrl_clicked()
{
    if(vcd->isHidden() == true){
        vcd->Active(handler);
    }
    else{
        vcd->Inactive();
    }
}

void MainWindow::on_pushButton_diglog_gpio_clicked()
{
    if(gcd->isHidden() == true){
        gcd->Active(handler);
    }
    else{
        gcd->Inactive();
    }
}

void MainWindow::on_pushButton_diglog_serial_clicked()
{
    if(scd->isHidden() == true){
        scd->Active(handler);
    }
    else{
        scd->Inactive();
    }
}

void MainWindow::on_pushButton_diglog_advanced_clicked()
{
    if(acd->isHidden() == true){
        acd->Active(handler);
    }
    else{
        acd->Inactive();
    }
}

void MainWindow::on_pushButton_record_clicked()
{
    CHD_WMP_Video_GetParam(handler, &param);
    int format = param.format;

    if(record){

        if(format == CHD_FMT_MJPEG){
            CHD_WMP_Mjpeg2Avi_End(handler);
        }
#ifdef H264_SUPPORT
        else if(format == CHD_FMT_H264){
            h264file.close();
        }
#endif
        ui->frame_ctrl->setEnabled(true);
        ui->pushButton_record->setText(QString("录像 "));

        record = 0;
    }
    else{

        QDir dirtemp;

        /* check directory is exist */
        if(dirtemp.exists(RECORD_PATH) == false){
            dirtemp.mkdir(RECORD_PATH);
        }

        CHD_WMP_Video_GetParam(handler, &param);
        if(param.format == CHD_FMT_YUYV) {
            return ;
        }

        QDateTime time;
        QString qstr;
        time = QDateTime::currentDateTime();
        qstr = time.toString("yy-M-d_h-m-s ");

        int w = param.resolu >> 16 & 0xffff;
        int h = param.resolu  & 0xffff;

        char filename[100];
        if(format == CHD_FMT_MJPEG){
            sprintf(filename, "%s%s_%dx%d_%dfps.avi", RECORD_PATH, qstr.toLocal8Bit().data(), w, h, param.fps);
        }
        else if(format == CHD_FMT_H264){
            sprintf(filename, "%s%s_%dx%d_%dfps.h264", RECORD_PATH, qstr.toLocal8Bit().data(), w, h, param.fps);
        }

        qDebug("file = %s", filename);

        if(format == CHD_FMT_MJPEG){
            CHD_WMP_Mjpeg2Avi_Begin(handler, filename);
            CHD_WMP_Mjpeg2Avi_SetParam(handler, w, h, 30);
        }
#ifdef H264_SUPPORT
        else{
            h264file.setFileName(filename);
            h264file.open(QIODevice::WriteOnly);
        }
#endif
        ui->frame_ctrl->setEnabled(false);
        ui->pushButton_record->setText(QString("停止录像 "));
        record = 1;
    }
}

void MainWindow::on_pushButton_videoabilityvidew_clicked()
{
    if(vacd->isHidden() == true){
        vacd->Active(handler);
    }
    else{
        vacd->Inactive();
    }
}

void MainWindow::on_pushButton_diglog_i2c_clicked()
{
      if(i2cd->isHidden() == true){
          i2cd->Active(handler);
      }
      else{
          i2cd->Inactive();
      }
}


void CHD_Video_AnalysisResolu(int resolu, unsigned int *w, unsigned int *h)
{
    *w = resolu >> 16&0xffff;
    *h = resolu&0xFFFF;
}

void MainWindow::on_pushButton_Web_clicked()
{
    QDesktopServices::openUrl(QUrl("http://" + ui->lineEdit_IP->text()));
}


void MainWindow::on_pushButton_Web_2_clicked()
{
#ifdef OFFICAIAL_WEB
    QDesktopServices::openUrl(QUrl(OFFICAIAL_WEB));
#endif
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
     oldMousepot = event->globalPos();
     oldMainwpot = pos();

     isMousePressed = true;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    if(true == isMousePressed){
        move((oldMainwpot - (oldMousepot - event->globalPos())));
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent * event)
{
    isMousePressed = false;
}


