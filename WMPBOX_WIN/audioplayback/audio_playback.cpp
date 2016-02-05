#include "audio_playback.h"
// The buffer length is assigned in audioLength function
// now it is set the audio duraton length and get the audio length thereafter


bufferPlayback::bufferPlayback(int rate, int bits, int channels)
{
    // Set up the format, eg.
    format.setSampleRate(rate);
    format.setChannelCount(channels);
    format.setSampleSize(bits);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }
    audio = new QAudioOutput(format, this);

    m_temp = audio->start();
    if(m_temp == NULL){
        return ;
    }

    bool ret = m_temp->open(QIODevice::WriteOnly);
    if(ret == false){
        m_temp = NULL;
    }
}


bufferPlayback::~bufferPlayback()
{
    if(m_temp != NULL){
        m_temp->close();
    }

    delete audio;
}

void bufferPlayback::play(char * pdata, int size)
{
    if(m_temp != NULL){
        memcpy(buffer, pdata, size);
        m_temp->write(buffer, size);
    }
    else{
        m_temp = audio->start();
        if(m_temp == NULL){
        }

        bool ret = m_temp->open(QIODevice::WriteOnly);
        if(ret == false){
            m_temp = NULL;
        }
    }
}

