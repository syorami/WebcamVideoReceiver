/********************************************************************************
**                                    chd_type.h
**                           
**
**	File  Name   :  chd_type.h
**	Description  : 
**	Modification : 2011-03-28		MFJ		Create the file
				   2014-05-11		MFJ		Modify & Refresh
*********************************************************************************/

#ifndef CHD_TYPE_H_
#define CHD_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CHD_Main		main

/* ANSI C ͷ�ļ� */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <errno.h>

/* Linux ϵͳͷ�ļ� */

#if defined __linux__ || __APPLE__
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/shm.h>			/* TimYu changed */
#include <sys/mount.h>
#include <sys/poll.h>
#include <syslog.h>
#include <dirent.h>

/* Linux�����׽��� */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/select.h>
#include <netinet/in.h>

#endif
    
#if defined __linux__
#include <getopt.h>
#include <net/if_arp.h>   
#include <sys/sysmacros.h>
#endif


#define IN				/* ���� */
#define OUT				/* ��� */
#define VRET			/* value reuslt  ֵ��� */


typedef short				CHD_SINT16;
typedef unsigned short		CHD_UINT16;
typedef int					CHD_SINT32;
typedef unsigned int		CHD_UINT32;
typedef char				CHD_SINT8;
typedef unsigned char		CHD_UINT8;
typedef float 				CHD_FLOAT;
typedef double				CHD_DOUBLE;


typedef unsigned char		BYTE;
typedef unsigned short		WORD;


typedef int 				Dev_t;

#ifndef SUCCESS
#define SUCCESS         	0
#endif

#ifndef FAILURE
#define FAILURE       		-1
#endif

#define PARAM_FAILED		-3

#ifndef TRUE
#define TRUE 				1
#endif

#ifndef FALSE
#define FLASE 				0
#endif

#ifndef NULL
#define NULL				0
#endif

#ifndef ON
#define ON					1
#endif

#ifndef OFF
#define OFF					0
#endif

/* ���ֵ */
#ifndef MAX
#define MAX(a,b)		(((a) > (b)) ? (a) : (b))
#endif

/* ��Сֵ */
#ifndef MIN
#define MIN(a,b)		(((a) < (b)) ? (a) : (b))
#endif

/* ����ֵ  */
#ifndef ABS
#define ABS(x) ((x)>=0?(x):-(x))
#endif

/* ��ʱ����, ���ᱻ�źż�ϵͳ���ô�� */

#define CHD_USLEEP(sec, usec) {\
	struct timeval tv = {sec, usec};	\
	do{}while(select(0, NULL, NULL, NULL, &tv) != 0);	\
}


/* �������С */
#define TABLE_SIZE(table)		(sizeof(table)/sizeof(table[0]))

/* λ���� */
#define BITMSK(bit)				(1 << (bit))
/* λ��0 */
#define BITSET0(v, bit)			(v) &= ~ BITMSK(bit)
/* λ�� 1*/
#define BITSET1(v, bit)			(v) |= BITMSK(bit)
/* ȡ��8λ */
#define CHD_LOBYTE(v)			((CHD_UINT8)(v))
/* ȡ��8λ */
#define CHD_HIBYTE(v)			((CHD_UINT8)(((CHD_UINT8)(v) >> 8) & 0xFFFF))
/* ȡ��16λ */
#define CHD_LOWORD(v)           ((CHD_UINT32)(v))
/* ȡ��16λ */
#define CHD_HIWORD(v)           ((CHD_UINT32)(((CHD_UINT32)(v) >> 16) & 0xFFFF))
/* ��ϳ�16λ */
#define CHD_MAKEWORD(a, b)      ((CHD_UINT16)(((CHD_UINT8)(a)) | ((CHD_UINT16)((CHD_UINT8)(b))) << 8))
/* ��ϳ�32λ */
#define CHD_MAKELONG(a, b)      ((CHD_UINT32)(((CHD_UINT16)(a)) | ((CHD_UINT32)((CHD_UINT16)(b))) << 16))

/* ��ӡ */
//#define CHD_PRINT(fmt, arg ...) 		printf(fmt, ## arg)

/* ������ */
//#define CHD_ERR_PRINT(fmt,arg ...) 		fprintf(stderr, fmt, ## arg)

/* ���Դ�ӡ���� */
#define CHD_DEBUG_LINE(i) 				CHD_ERR_PRINT("%d\n", i)

/* ���������ӡ */
/*
#define CHD_ERR_PARAM(fmt,arg ...) do{	\
	CHD_ERR_PRINT("[%s: %s] Parameter error line : %d "fmt"\n" ,\
	__FILE__, __FUNCTION__, __LINE__, ## arg);	\
	return PARAM_FAILED;	\
}while(0)
*/
/* ���ؼ�� */
#define CHD_CHECK_RET(condition)do{ \
	if((condition)){ \
		CHD_ERR_PRINT("CHD_CHECK_ERROR at <%s : %d> errno : %s!\n",	\
							 __FILE__, __LINE__, strerror(errno));	\
		return FAILURE;	\
	}\
}while(0)



#ifdef __cplusplus
}
#endif

#endif

