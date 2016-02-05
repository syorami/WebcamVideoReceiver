#ifndef _CHD_WMP_WIRELESS_
#define _CHD_WMP_WIRELESS_

#include "CHD_WMP_Type.h"
#include "CHD_WMP_Return.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	char ssid[40];
	char key[40];
}NET_STA_INFO_S;


// …Ë÷√STA¡¨Ω”
CHD_WMP_API CHD_RET_E CHD_WMP_Wireless_SetStaMode(IN CHD_WMP_T handle, IN NET_STA_INFO_S *pst);


#ifdef __cplusplus
}
#endif
#endif

