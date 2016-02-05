

#ifndef _CHD_WMP_SERIAL_H
#define _CHD_WMP_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif


#include "CHD_WMP_Type.h"
#include "CHD_WMP_Return.h"

typedef enum{
    CHD_SERIAL_BS300 		= 300,
    CHD_SERIAL_BS1200 		= 1200,
    CHD_SERIAL_BS2400 		= 2400,   
    CHD_SERIAL_BS4800 		= 4800,
    CHD_SERIAL_BS9600 		= 9600,
    CHD_SERIAL_BS19200 		= 19200,
    CHD_SERIAL_BS38400		= 38400,
    CHD_SERIAL_BS57600 		= 57600,
    CHD_SERIAL_BS115200 	= 115200,
    CHD_SERIAL_BS230400 	= 230400,
}CHD_SERIAL_SPEED_E;

typedef enum{
    CHD_SERIAL_DATABIT_7 = 7,
    CHD_SERIAL_DATABIT_8 = 8,
}CHD_SERIAL_DATABIT_E;

typedef enum{
    CHD_SERIAL_STOPBIT_1 = 1,
    CHD_SERIAL_STOPBIT_0 = 0,
}CHD_SERIAL_STOPBIT_E;

typedef enum{
    CHD_SERIAL_PARITY_EVEN  = 69, 
    CHD_SERIAL_PARITY_NONE  = 78, 
    CHD_SERIAL_PARITY_ODD   = 79, 
    CHD_SERIAL_PARITY_SPACE = 83, 
}CHD_SERIAL_PARITY_E;

typedef struct{
    CHD_SERIAL_SPEED_E      speed;
    CHD_SERIAL_DATABIT_E    databit;
    CHD_SERIAL_STOPBIT_E    stopbit;
    CHD_SERIAL_PARITY_E     parity;
    CHD_U32                 timeout;
}CHD_SERIAL_PARAM_S;

/****************************************************************************************
*	Chird Serial Control Interface
*****************************************************************************************/
/* Serial Control */
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_GetSpeed	(IN CHD_WMP_T handle, OUT CHD_SERIAL_SPEED_E * pspeed);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_GetDataBit	(IN CHD_WMP_T handle, OUT CHD_SERIAL_DATABIT_E * pdatabit);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_GetStopBit	(IN CHD_WMP_T handle, OUT CHD_SERIAL_STOPBIT_E * pstopbit);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_GetParity	(IN CHD_WMP_T handle, OUT CHD_SERIAL_PARITY_E  * pparity);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_GetTimeout	(IN CHD_WMP_T handle, OUT CHD_U32  * ptimeout);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_GetParam	(IN CHD_WMP_T handle, OUT CHD_SERIAL_PARAM_S  * pparam);

CHD_WMP_API CHD_RET_E CHD_WMP_Serial_GetRxTotalNum(IN CHD_WMP_T handle, OUT CHD_U32 * pcnt);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_GetTxTotalNum(IN CHD_WMP_T handle, OUT CHD_U32 * pcnt);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_GetCurRxCacheSize(IN CHD_WMP_T handle, OUT CHD_U32 * pcnt);


CHD_WMP_API CHD_RET_E CHD_WMP_Serial_SetSpeed	(IN CHD_WMP_T handle, IN CHD_SERIAL_SPEED_E  speed);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_SetDataBit	(IN CHD_WMP_T handle, IN CHD_SERIAL_DATABIT_E databit);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_SetStopBit	(IN CHD_WMP_T handle, IN CHD_SERIAL_STOPBIT_E stopbit);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_SetParity	(IN CHD_WMP_T handle, IN CHD_SERIAL_PARITY_E  parity);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_SetTimeout	(IN CHD_WMP_T handle, IN CHD_U32  timeout);

CHD_WMP_API CHD_RET_E CHD_WMP_Serial_Begin		(IN CHD_WMP_T handle);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_End		(IN CHD_WMP_T handle);

/*  Serial Send Data */
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_SendData(IN CHD_WMP_T handle, OUT CONST CHD_VOID * pdata, OUT CHD_U32 datalen);

/*  Serial Receive Data */
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_RequestData(IN CHD_WMP_T handle, OUT CHD_VOID ** pdata, OUT CHD_U32 * datalen);
CHD_WMP_API CHD_RET_E CHD_WMP_Serial_ReleaseData(IN CHD_WMP_T handle, IN CONST CHD_VOID * pdata);



#ifdef __cplusplus
}
#endif
#endif

