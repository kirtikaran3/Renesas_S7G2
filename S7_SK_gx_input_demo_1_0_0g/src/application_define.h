/*
 * application_define.h
 *
 *  Created on: 7 Dec 2015
 *      Author: karol.saja
 */

#ifndef APPLICATION_DEFINE_H_
#define APPLICATION_DEFINE_H_

#include <math.h>
#include <stdio.h>
#include "hal_data.h"
#include "gui_thread.h"
#include "input_thread.h"

#define APP_VERSION "A0.1"

extern TX_THREAD gui_thread;
extern TX_THREAD touch_thread;
extern TX_THREAD input_thread;

#define APP_EVENT_SW4_PRESS     (GX_FIRST_APP_EVENT + 1)
#define APP_EVENT_SW5_PRESS     (GX_FIRST_APP_EVENT + 2)
#define APP_EVENT_ADC_SCAN      (GX_FIRST_APP_EVENT + 3)

#define APP_ADC_TIMER_ID        (101)
#define APP_ADC_TIMER_COUNT     (50)

#define APP_NORMALIZE_TEMP(a)   ((3.3f * (((float) (a)) / 4095.0f) - 1.26f) / 0.041f + 25.0f)

#define APP_ERROR_RETURN(a) if(a) return a;
#define APP_ERROR_BREAK(a) if(a) break;
#define APP_ERROR_TRAP(a) if(a) while(1);

#endif /* APPLICATION_DEFINE_H_ */
