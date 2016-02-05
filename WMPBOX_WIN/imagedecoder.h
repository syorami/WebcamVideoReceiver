#ifndef IMAGEDECODER_H
#define IMAGEDECODER_H

#include <QImage>
#include <QSemaphore>

#include "imagedecoder.h"
#include "Apis/turbojpeg.h"
#include "Apis/CHD_WMP.h"
#include "Apis/CHD_WMP_Type.h"
#include "Apis/CHD_WMP_Video.h"
#include "Apis/chd_base.h"

#ifdef SUPPORT_H264
#include "ffmpegDecoder/ffmpegDecoder.h"
#endif

class ImageDecoder : public QObject
{
    Q_OBJECT
public:
    explicit ImageDecoder(QObject *parent = 0);
    ~ImageDecoder();

    unsigned int pRGBBuf[5*1024*1024];
    unsigned char clip(int value);
    int yuyv_to_rgb565(unsigned char * dst,const unsigned char * src,int npixel);
#ifdef H264_SUPPORT
    CFfmpegDecode h264decoder;
#endif
    tjhandle tjpeg;

public slots:
    void imageDecode(unsigned int handler, CHD_VIDEO_FRAME_S * videopacket);

signals:
    void imagetoDisplay(QImage * image, int index, int format, int w, int h, int fps);
};

#endif // IMAGEDECODER_H
