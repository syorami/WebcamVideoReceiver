#-------------------------------------------------
#
# Project created by QtCreator 2015-07-01T01:40:27
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WMPBOX_WIN
TEMPLATE = app

#DEFINES += H264_SUPPORT
#DEFINES += MULTI_DISPLAY

SOURCES += main.cpp\
        mainwindow.cpp \
    advanced_ctrl.cpp \
    gpio_ctrl.cpp \
    i2c_ctrl.cpp \
    serial_ctrl.cpp \
    video_ctrl.cpp \
    videoabiltiy_view.cpp \
    videodisplay.cpp \
    imagedecoder.cpp \
    audioplayback/audio_playback.cpp

#contains(DEFINES, H264_SUPPORT) {
#SOURCES += ffmpegDecoder/ffmpegDecoder.cpp
#}

HEADERS  += mainwindow.h \
    Apis/chd_base.h \
    Apis/CHD_WMP.h \
    Apis/CHD_WMP_Audio.h \
    Apis/CHD_WMP_Code.h \
    Apis/CHD_WMP_Gpio.h \
    Apis/CHD_WMP_I2C.h \
    Apis/CHD_WMP_Return.h \
    Apis/CHD_WMP_Serial.h \
    Apis/CHD_WMP_Type.h \
    Apis/CHD_WMP_Video.h \
    Apis/turbojpeg.h \
    Apis/chd_common.h \
    advanced_ctrl.h \
    custom.h \
    gpio_ctrl.h \
    i2c_ctrl.h \
    process_code.h \
    serial_ctrl.h \
    video_ctrl.h \
    videoabiltiy_view.h \
    videodisplay.h \
    imagedecoder.h \
    audioplayback/audio_playback.h

contains(DEFINES, H264_SUPPORT) {
HEADERS  += ffmpegDecoder ffmpegInclude
}

INCLUDEPATH += ffmpegInclude

FORMS   += mainwindow.ui \
    advanced_ctrl.ui \
    gpio_ctrl.ui \
    i2c_ctrl.ui \
    serial_ctrl.ui \
    video_ctrl.ui \
    videoabiltiy_view.ui \
    videodisplay.ui

RC_FILE += resource/myapp.rc

RESOURCES += resource/resource.qrc

LIBS += -L./lib \
            -lWs2_32 \
            -lIPHLPAPI \
            -lwinmm \
            -lchd_base \
            -lCHD_WMP \
            -lturbojpeg

# -lchd_efast \

contains( DEFINES, H264_SUPPORT) {
LIBS +=     -lavcodec-56 \
            -lavdevice-56 \
            -lavfilter-5 \
            -lavformat-56 \
            -lavutil-54 \
            -lswresample-1 \
            -lswscale-3
}
