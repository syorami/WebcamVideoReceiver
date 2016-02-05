
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
	unsigned char 	address;		/* i2c�豸��ַ */
	unsigned char 	subaddress;		/* i2c�豸�ӵ�ַ */
	unsigned char	readmode;		/* i2c������ʱ��ѡ������ֹͣλ: 0(��ֹͣλ) 1(��ֹͣλ��Ĭ��) */
									/* �ڶ�����ʱ����Ҫ��д���豸��ַ���ӵ�ַ������־���ó�Ա�����Ƿ���д������������֮����ֹͣλ���� */
	unsigned char 	datasize;		/* ��д���ݴ�С */			
	unsigned char   data[128];  	/* д��/��ȡ���ݵĻ�����  */
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

