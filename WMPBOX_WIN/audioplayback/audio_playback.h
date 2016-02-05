
#ifndef AUDIOPLAYBACK_H
#define AUDIOPLAYBACK_H

#include <QtMultimedia/QAudioInput>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QtMultimedia/QAudioOutput>
#include <iostream>
#include <QByteArray>
#include <QBuffer>
#include <QCoreApplication>

class bufferPlayback: public QObject
{
    Q_OBJECT
public:
    bufferPlayback(int rate = 8000, int bits = 16, int channels = 2);
    ~bufferPlayback();

    void play(char * pdata, int size);

public slots:


private:
    char    buffer[200*1024];
    QTimer qtimer;
    QIODevice*          m_temp;
    QAudioFormat        format;
    QAudioDeviceInfo    m_audioOutputDevice;
    QAudioOutput*       audio;
};
#endif // AUDIOPLAYBACK_H
