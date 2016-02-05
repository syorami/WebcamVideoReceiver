

#ifndef _CHD_WMP_H_
#define _CHD_WMP_H_


#include "CHD_WMP_Type.h"
#include "CHD_WMP_Return.h"

#ifdef __cplusplus
extern "C" {
#endif

#define		MAX_ONLINE_DEVICE				50			// 搜索设备最大数量

#define 	CHD_ALIAS_LEN					20			// 设备别名长度
#define 	CHD_IPADDR_LEN					20			// 设备地址长度


typedef enum{
	CHD_TRANSMODE_TCP,
	CHD_TRANSMODE_EFAST,		
}CHD_TRANSMODE_E;

typedef struct{
    CHD_U16 year;
    CHD_U8 	month;
    CHD_U8 	mday;
    CHD_U8 	wday;
    CHD_U8 	hour;
    CHD_U8 	min;
    CHD_U8 	sec;
}CHD_TIME_S;



typedef struct{
	CHD_U32	Id;
	CHD_S8	Alias[CHD_ALIAS_LEN];	
	CHD_S8	IpAddr[CHD_IPADDR_LEN];
}CHD_DEVINFO_S;

typedef struct
{
    char Mac[6];
}CHD_MAC_S;


typedef enum{
    CHD_STREAM_VIDEO    		= 0x00,
    CHD_STREAM_PICTURE  		= 0x01,
    CHD_STREAM_AUDIO    		= 0x02,
    CHD_STREAM_SERIAL  	 		= 0x03,
	CHD_PARAM_CHANGE_VABILITY	= 0X04,
	CHD_PARAM_CHANGE_VPARAM		= 0X05,
	CHD_PARAM_CHANGE_VCTRL		= 0X06,
	CHD_PARAM_CHANGE_AUDEO		= 0X07,
	CHD_PARAM_CHANGE_SERIAL		= 0X08,
	CHD_PARAM_CHANGE_GPIO		= 0X09,
}CHD_STREAM_TYPE_E;




/****************************************************************************************
*	Chird Platform Serach Modules Interface
*****************************************************************************************/
CHD_WMP_API CHD_RET_E CHD_WMP_ScanDevice_Init(IN CHD_U32 SearchInterTime);
CHD_WMP_API CHD_RET_E CHD_WMP_Scan_GetDeviceInfo(OUT CHD_DEVINFO_S * dev, OUT CHD_U32 * Number);
CHD_WMP_API CHD_RET_E CHD_WMP_ScanDevice_UnInit();


/****************************************************************************************
*	Chird Platform Control Interface
*****************************************************************************************/
CHD_WMP_API CHD_RET_E CHD_WMP_ConnectDevice	(OUT CHD_WMP_T * phandle, IN  CHD_S8 * IPAddress);
CHD_WMP_API CHD_RET_E CHD_WMP_Disconnect	(IN CHD_WMP_T handle);

CHD_WMP_API CHD_RET_E CHD_WMP_GetEncrypt	(IN CHD_WMP_T handle, OUT CHD_S8 * Encrypt);

CHD_WMP_API CHD_RET_E CHD_WMP_GetDeviceInfo	(IN CHD_WMP_T handle, OUT CHD_DEVINFO_S * dev);
        
CHD_WMP_API CHD_RET_E CHD_WMP_GetMac		(IN CHD_WMP_T handle, OUT CHD_MAC_S * Mac);
CHD_WMP_API CHD_RET_E CHD_WMP_GetSystemTime	(IN CHD_WMP_T handle, OUT CHD_TIME_S * stime);

CHD_WMP_API CHD_RET_E CHD_WMP_SetTransMode	(IN CHD_WMP_T handle, IN  CHD_TRANSMODE_E emode);
CHD_WMP_API CHD_RET_E CHD_WMP_SetSystemTime	(IN CHD_WMP_T handle, IN  CONST CHD_TIME_S * stime);

/* this interface must be in a loop */
CHD_WMP_API CHD_RET_E CHD_WMP_Poll			(IN CHD_WMP_T handle,	
											OUT CHD_STREAM_TYPE_E * type, 
											IN CHD_U32 timeout_sec, 
											IN CHD_U32 timeout_msec);

#ifdef __cplusplus
}
#endif
#endif







