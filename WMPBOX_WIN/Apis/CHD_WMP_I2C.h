
/**
*
*	Chird Wireless Multimedia Platform
*	
*/

#ifndef _CHD_WMP_IIC_
#define _CHD_WMP_IIC_

#include "CHD_WMP_Type.h"
#include "CHD_WMP_Return.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	unsigned char 	address;		/* i2c设备地址 */
	unsigned char 	subaddress;		/* i2c设备子地址 */
	unsigned char	readmode;		/* i2c读数据时，选择有无停止位: 0(无停止位) 1(有停止位，默认) */
									/* 在读数据时，需要先写入设备地址、子地址、读标志，该成员控制是否在写入这三个数据之后有停止位发送 */
	unsigned char 	datasize;		/* 读写数据大小 */			
	unsigned char   data[128];  	/* 写入/读取数据的缓冲区  */
}CHD_IIC_DATA_S;


/****************************************************************************************
*	Chird IIC Read And Write Interface
*****************************************************************************************/

CHD_WMP_API CHD_RET_E CHD_WMP_I2C_GetValue   	(IN CHD_WMP_T handle,  IN CHD_IIC_DATA_S * pdata);
CHD_WMP_API CHD_RET_E CHD_WMP_I2C_SetValue		(IN CHD_WMP_T handle,  IN CHD_IIC_DATA_S * pdata);



#ifdef __cplusplus
}
#endif
#endif

