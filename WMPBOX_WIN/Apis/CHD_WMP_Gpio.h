

#ifndef _CHD_WMP_GPIO_H_
#define _CHD_WMP_GPIO_H_

#include "CHD_WMP_Type.h"
#include "CHD_WMP_Return.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum{
	GPIO1 = 0x0,
	GPIO2,
	GPIO3,
	GPIO4,
	GPIO5,
	GPIO_MAX,
}CHD_GPIO_E;

typedef enum{
	CHD_GPIO_STATE_LOW,
	CHD_GPIO_STATE_HIGH = 1,
}CHD_GPIO_STATE_E;

/****************************************************************************************
*	Chird Gpio Control Interface
*****************************************************************************************/
CHD_WMP_API CHD_RET_E CHD_WMP_Gpio_GetAll(IN CHD_WMP_T handle, CHD_GPIO_STATE_E *data);
CHD_WMP_API CHD_RET_E CHD_WMP_Gpio_SetAll(IN CHD_WMP_T handle, CHD_GPIO_STATE_E *data);

CHD_WMP_API CHD_RET_E CHD_WMP_Gpio_GetStatus   	(IN CHD_WMP_T handle, IN CHD_GPIO_E gpio, OUT CHD_GPIO_STATE_E * estate);
CHD_WMP_API CHD_RET_E CHD_WMP_Gpio_SetStatus	(IN CHD_WMP_T handle, IN CHD_GPIO_E gpio, IN  CHD_GPIO_STATE_E   estate);

#ifdef __cplusplus
}
#endif
#endif

