
/**
*
*	Chird Wireless Multimedia Platform
*	
*/

#ifndef _CHD_WMP_TYPE_
#define _CHD_WMP_TYPE_

//-----------------------------------------------------------------
#ifdef _WIN32
    #pragma comment(lib,"ws2_32.lib")
    #ifdef CHD_WMP_EXPORTS
        #define CHD_WMP_API extern "C" __declspec(dllexport)
    #else
        #define CHD_WMP_API extern "C" __declspec(dllexport)
    #endif
#endif

#if defined __linux__ || __APPLE__
    #define CHD_WMP_API
#endif
//------------------------------------------------------------------

#define IN				// in  parameter
#define OUT				// out parameter

typedef unsigned long 	CHD_WMP_T;	// handle type of chird wireless multimedia platform 

typedef unsigned char  	CHD_U8;
typedef char			CHD_S8;

typedef unsigned short	CHD_U16;
typedef short			CHD_S16;

typedef unsigned int 	CHD_U32;
typedef int			 	CHD_S32;

typedef unsigned long	CHD_UL32;
typedef long			CHD_SL32;

typedef void 			CHD_VOID;

#define CONST const

typedef unsigned int	CHD_TIMESTAMP_T;	// timestamp	millisecond	

typedef enum{
	CHD_TRUE,
	CHD_FALSE,
}CHD_BOOL;

typedef enum{
	BLOCK,
	NOBLOCK,
}CHD_BLOCK_E;

#endif

