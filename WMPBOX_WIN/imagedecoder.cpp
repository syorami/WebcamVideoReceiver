#include "imagedecoder.h"
#include "videodisplay.h"

ImageDecoder::ImageDecoder(QObject *parent) : QObject(parent)
{
    tjpeg = tjInitDecompress();

#ifdef H264_SUPPORT
    h264decoder.openDecoder();
#endif
}


ImageDecoder::~ImageDecoder()
{
    tjDestroy(tjpeg);
#ifdef H264_SUPPORT
    h264decoder.closeDecoder();
#endif
}

unsigned char ImageDecoder::clip(int value)
{
    if (value < 0)	value = 0;
    else if (value > 255)	value = 255;

    return value;
}

int ImageDecoder::yuyv_to_rgb565(unsigned char * dst,const unsigned char * src,int npixel)
{
    unsigned int i;
    int y1, y2, u, v = 0;
    unsigned char r, g, b;

    /* Input format is Cr(i)Y(i + 1)Cb(i)Y(i) */
    for (i = 0; i < npixel; i += 2) {
        y2 = *src++;
        v  = *src++;
        y1 = *src++;
        u  = *src++;

        y1 -= 16;
        u  -= 128;
        v -= 128;
        y2 -= 16;

        b = clip(( 298 * y1           + 409 * v + 128) >> 8);
        g = clip(( 298 * y1 - 100 * u - 208 * v + 128) >> 8);
        r = clip(( 298 * y1 + 516 * u           + 128) >> 8);
        *dst++ = (( g & 0x1c) << 3) | ( b >> 3);
        *dst++ = ( r & 0xf8) | ( g >> 5);

        b = clip(( 298 * y2           + 409 * v + 128) >> 8);
        g = clip(( 298 * y2 - 100 * u - 208 * v + 128) >> 8);
        r = clip(( 298 * y2 + 516 * u           + 128) >> 8);
        *dst++ = (( g & 0x1c) << 3) | ( b >> 3);
        *dst++ = ( r & 0xf8) | ( g >> 5);

    }
    return 0;
}

#include <QDebug>

void ImageDecoder::imageDecode(unsigned int handler, CHD_VIDEO_FRAME_S * videopacket)
{

    static int oldtimestamp = -1;
    static unsigned int w, h;
    int ret;

    QImage * imageData = NULL;

    CHD_Video_AnalysisResolu(videopacket->resolu, &w, &h);

    if(videopacket->format == CHD_FMT_YUYV){
        if(videopacket->timestamp - oldtimestamp >= 40){
            oldtimestamp = videopacket->timestamp;
            yuyv_to_rgb565((uchar*)pRGBBuf, (uchar*)videopacket->pData,  w*h);
            imageData = new QImage((uchar*)pRGBBuf, w, h, QImage::Format_RGB16);
        }
    }
    else if(videopacket->format == CHD_FMT_MJPEG){
        if(videopacket->timestamp - oldtimestamp >= 40){
             oldtimestamp = videopacket->timestamp;

            int ret = tjDecompress2(tjpeg, (unsigned char*)videopacket->pData, videopacket->datalen,
                          (unsigned char*)pRGBBuf, w, w*3, h, TJPF_RGB, 0);

            if(ret >= 0){
                imageData = new QImage((uchar*)pRGBBuf, w, h, QImage::Format_RGB888);
            }
        }
    }
#ifdef H264_SUPPORT
    else if(videopacket->format == CHD_FMT_H264){

        ret = h264decoder.h264_decoding((unsigned char*)videopacket->pData, videopacket->datalen, (unsigned char*)pRGBBuf);
        if(ret == 0){
            if(videopacket->timestamp - oldtimestamp >= 40){
                oldtimestamp = videopacket->timestamp;
                imageData = new QImage((uchar*)pRGBBuf, w, h, QImage::Format_RGB32);
            }
            h264decoder.h264_decoding_free();
        }
        else{
            qDebug("h264 decode error ret = %d", ret);
        }
    }
#endif

    if(imageData != NULL){
        // main thread display
        emit imagetoDisplay(imageData, videopacket->bexist, videopacket->format, w, h, videopacket->fps);
    }

    CHD_WMP_Video_ReleaseVideoData((CHD_WMP_T)handler, videopacket);
}

