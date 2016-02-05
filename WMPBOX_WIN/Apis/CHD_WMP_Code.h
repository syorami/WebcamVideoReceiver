
#ifndef CHD_WMP_CODE_H_
#define CHD_WMP_CODE_H_


#include "CHD_WMP_Type.h"
#include "CHD_WMP_Return.h"

#ifdef __cplusplus
extern "C" {
#endif


CHD_WMP_API CHD_RET_E CHD_WMP_Yuyv2Argb8888(IN CHD_WMP_T handle, OUT CHD_U32 * Dest, IN CHD_U8 * Src, IN CHD_U32 width, IN CHD_U32 height);

CHD_WMP_API CHD_RET_E CHD_WMP_Mjpeg2Avi_Begin(IN CHD_WMP_T handle, CHD_S8 * filename);
CHD_WMP_API CHD_RET_E CHD_WMP_Mjpeg2Avi_PutData(IN CHD_WMP_T handle, IN CHD_S8* Dest, IN CHD_U32 datalen);
CHD_WMP_API	CHD_RET_E CHD_WMP_Mjpeg2Avi_SetParam(IN CHD_WMP_T handle, IN CHD_U32 width, IN CHD_U32 height, IN CHD_U32 fps);
CHD_WMP_API CHD_RET_E CHD_WMP_Mjpeg2Avi_End(IN CHD_WMP_T handle);

#ifdef __cplusplus
}
#endif
#endif

