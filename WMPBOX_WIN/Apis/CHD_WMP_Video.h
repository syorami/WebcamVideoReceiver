

#ifndef _CHD_WMP_VIDEO_H_
#define _CHD_WMP_VIDEO_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "CHD_WMP_Type.h"
#include "CHD_WMP_Return.h"

typedef enum{
	CHD_FMT_YUYV = 0x1,
	CHD_FMT_MJPEG,
	CHD_FMT_H264
}CHD_VIDEO_FMT_E;

typedef enum{
    CHD_VCTRL_BRIGHTNESS = 0x0,		
    CHD_VCTRL_CONTRAST,
    CHD_VCTRL_SATURATION,
    CHD_VCTRL_HUE,
    CHD_VCTRL_WHITE_BALANCE,
    CHD_VCTRL_GAMMA,
    CHD_VCTRL_GAIN,
    CHD_VCTRL_SHARPNESS,
    CHD_VCTRL_BACKLIGHT,
    CHD_VCTRL_EXPOSURE,
	CHD_VCTRL_MAX,
}CHD_VCTRL_E;

#define FORMAT_MAX_NUM	4
#define RESOLU_MAX_NUM  12
typedef struct video_support_ability
{
    struct {
        CHD_U32 	format;
        struct {
            CHD_U32 	resolu;
            CHD_U32  	maxfps;
        }resolu[RESOLU_MAX_NUM];
    }format[FORMAT_MAX_NUM];
}CHD_VIDEO_ABILITY_S;

typedef struct{
    CHD_S32 	val_valid;
    CHD_S32 	minval;
    CHD_S32 	curval;
    CHD_S32 	maxval;
    CHD_S32 	stepval;
    CHD_S32 	defval;

    CHD_S32 	auto_valid;
    CHD_S32 	autoval;
}CHD_VIDEO_CTRL_S;




typedef struct{
    CHD_U32			format;			// format
    CHD_U32 		resolu;  		// resolution
    CHD_U32 		fps;			// fps
    CHD_U32 		maxfps;			// maxfps
}CHD_VIDEO_PARAM_S;


typedef struct{
    CHD_U32	    	bexist;			// camera is exist
	CHD_U32			format;			// format
	CHD_U32 		resolu;  		// resolution
	CHD_U32			fps;			
	CHD_U32			BPS;			
	CHD_U32			timestamp;
	CHD_U32			datalen;
	CHD_S8			pData[0];
}CHD_VIDEO_FRAME_S;




/****************************************************************************************
*	Chird Video Control Interface
*****************************************************************************************/

CHD_WMP_API void CHD_Video_AnalysisResolu(int resolu, unsigned int * w, unsigned int * h);
CHD_WMP_API int CHD_Video_GroupResolu(const unsigned int width, const unsigned int height);
/* Video  Control */
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetAbility	(IN CHD_WMP_T handle, OUT CHD_VIDEO_ABILITY_S * pstAbility);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetVideoCtrl(IN CHD_WMP_T handle, IN  CHD_VCTRL_E type, OUT CHD_VIDEO_CTRL_S * pstCtrl);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetFormat	(IN CHD_WMP_T handle, OUT CHD_U32 * pFmt);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetResolu	(IN CHD_WMP_T handle, OUT CHD_U32 * pResolu);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetFPS   	(IN CHD_WMP_T handle, OUT CHD_U32 * pFPS);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetPeerMaxFrameNum (IN CHD_WMP_T handle, OUT CHD_U32 * pNum);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetLocalMaxFrameNum (IN CHD_WMP_T handle, OUT CHD_U32 * pNum);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetCurVideoFrameNum(IN CHD_WMP_T handle, OUT CHD_U32 * pNum);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetCurPictureFrameNum(IN CHD_WMP_T handle, OUT CHD_U32 * pNum);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_GetParam	(IN CHD_WMP_T handle, OUT CONST CHD_VIDEO_PARAM_S * pstParam);

CHD_WMP_API CHD_RET_E CHD_WMP_Video_SetVideoCtrl(IN CHD_WMP_T handle, IN CHD_VCTRL_E type, IN CONST CHD_VIDEO_CTRL_S * pCtrl);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_ResetVCtrl(IN CHD_WMP_T handle);

CHD_WMP_API CHD_RET_E CHD_WMP_Video_SetFormat	(IN CHD_WMP_T handle, IN CHD_VIDEO_FMT_E Fmt);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_SetResolu	(IN CHD_WMP_T handle, IN CHD_U32 Resolu);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_SetFPS   	(IN CHD_WMP_T handle, IN CHD_U32 FPS);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_SetPeerMaxFrameNum (IN CHD_WMP_T handle, IN CHD_U32 Num);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_SetLocalMaxFrameNum (IN CHD_WMP_T handle, IN CHD_U32 Num);

/* Video Data */
CHD_WMP_API CHD_RET_E CHD_WMP_Video_Begin		(IN CHD_WMP_T handle);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_End			(IN CHD_WMP_T handle);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_RequestVideoData(IN CHD_WMP_T handle, OUT CHD_VIDEO_FRAME_S ** pvdata);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_ReleaseVideoData(IN CHD_WMP_T handle, IN CHD_VIDEO_FRAME_S * pvdata);

/* Picture Data */
CHD_WMP_API CHD_RET_E CHD_WMP_Video_SnapShot	  (IN CHD_WMP_T handle);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_SnapShotResolu(IN CHD_WMP_T handle, IN CHD_U32 Resolu);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_RequestPicData(IN CHD_WMP_T handle, OUT CHD_VIDEO_FRAME_S ** pvdata);
CHD_WMP_API CHD_RET_E CHD_WMP_Video_ReleasePicData(IN CHD_WMP_T handle, IN  CHD_VIDEO_FRAME_S * pvdata);



#ifdef __cplusplus
}
#endif

#endif

