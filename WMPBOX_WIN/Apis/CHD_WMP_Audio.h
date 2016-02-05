

#ifndef _CHD_WMP_AUDIO_H_
#define _CHD_WMP_AUDIO_H_


#include "CHD_WMP_Type.h"
#include "CHD_WMP_Return.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum{
	CHD_AUDIO_SAMPLE_8000 	= 8000,			
	CHD_AUDIO_SAMPLE_44100	= 44100,
	CHD_AUDIO_SAMPLE_48000	= 48000
}CHD_AUDIO_SAMPLE_E;

typedef enum{
	CHD_AUDIO_ADBITS_8  	= 8,		// 8位  
	CHD_AUDIO_ADBITS_16 	= 16,		// 16位
}CHD_AUDIO_ADBITS_E;

typedef enum{
	CHD_AUDIO_CHN_SIGNAL 	= 1,		// 单通道
	CHD_AUDIO_CHN_STEREO 	= 2, 		// 立体声
}CHD_AUDIO_CHN_E;

typedef struct {
	CHD_AUDIO_SAMPLE_E esample;			// 采样频率 
	CHD_AUDIO_CHN_E	   echn;			// 通道数 
	CHD_AUDIO_ADBITS_E eadbits;			// AD量化精度 
}CHD_AUDIO_PARAM_S;

typedef struct{
	
	CHD_AUDIO_SAMPLE_E	esample;		// 采样频率
	CHD_AUDIO_CHN_E	   	echn;			// 通道号
	CHD_AUDIO_ADBITS_E 	eadbits;		// 量化精度

	CHD_TIMESTAMP_T		timestamp;		// 时间戳
	CHD_U32				datalen;		// 音频数据长度
	CHD_S8				pData[0];		// 音频数据指针
}CHD_AUDIO_FRAME_S;

/****************************************************************************************
*	Chird Audio Control Interface
*****************************************************************************************/
/* Audio  Control */
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_GetSampleFreq(IN CHD_WMP_T handle, OUT CHD_AUDIO_SAMPLE_E * peSampleFeq);
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_GetADBits	(IN CHD_WMP_T  handle, OUT CHD_AUDIO_ADBITS_E * peADBits);
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_GetChannel 	(IN CHD_WMP_T  handle, OUT CHD_AUDIO_CHN_E * peChn);
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_GetParam	(IN CHD_WMP_T  handle, OUT CONST CHD_AUDIO_PARAM_S * pstParam);

/* 现在暂时不支持修改参数 */
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_SetSampleFreq(IN CHD_WMP_T handle, IN CHD_AUDIO_SAMPLE_E peSampleFeq);
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_SetADBits	(IN CHD_WMP_T  handle, IN CHD_AUDIO_ADBITS_E peADBits);
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_SetChannel 	(IN CHD_WMP_T  handle, IN CHD_AUDIO_CHN_E peChn);
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_SetParam	(IN CHD_WMP_T  handle, IN CONST CHD_AUDIO_PARAM_S * pstParam);

CHD_WMP_API CHD_RET_E CHD_WMP_Audio_Begin		(IN CHD_WMP_T handle);
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_End			(IN CHD_WMP_T handle);

CHD_WMP_API CHD_RET_E CHD_WMP_Audio_RequestData	(IN CHD_WMP_T handle, OUT CHD_AUDIO_FRAME_S ** pstData);
CHD_WMP_API CHD_RET_E CHD_WMP_Audio_ReleaseData(IN CHD_WMP_T handle, IN CHD_AUDIO_FRAME_S * pstData);

#ifdef __cplusplus
}
#endif
#endif

