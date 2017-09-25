/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
 * display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
 * purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
 * SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
 * NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
 * INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
 * be subject to different terms.
 **********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : sf_thread_monitor_demo_thread_entry.c
* Description  : This file registers for monitoring. If misbehave 1 toggle red led of LED1 for 10 times and spin in
*                while(1). If misbehave 0 toggle green led of LED1 for infinite time.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           TODO  1.00    Initial Release
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include "sf_thread_monitor_demo_thread.h"
#include "dev_example_thread.h"
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#define GREEN_LED_PIN IOPORT_PORT_08_PIN_07
#define GRN_LED_DELAY_COUNT 1200000
#define RED_LED_PIN IOPORT_PORT_08_PIN_08
#define RED_LED_DELAY_COUNT 1500000

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/
void sf_Thread_monitor_thread1_entry(void);

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/

volatile uint32_t delay_counter;

/***********************************************************************************************************************
Extern global variables
***********************************************************************************************************************/

extern volatile int misbehave ;
extern bsp_leds_t ledpresent;

/***********************************************************************************************************************
Functions
***********************************************************************************************************************/



/*******************************************************************************************************************//**
 * @brief Registers for monitoring, resumes sf_thread_monitor_thread2 thread and If misbehave 1 toggle red led of LED1 for 10 times and spin in
*                while(1). If misbehave 0 toggle green led of LED1 for infinite time.
 *  @retval VOID
***********************************************************************************************************************/


void sf_thread_monitor_demo_thread_entry(void)
{
    sf_thread_monitor_counter_min_max_t min_max_values;
    uint8_t count;
    ioport_level_t level;

    min_max_values.minimum_count = 1;
    min_max_values.maximum_count = 4;

    g_sf_thread_monitor.p_api->threadRegister(g_sf_thread_monitor.p_ctrl,
                                                                   &min_max_values);

    while(1)
    {

       if(misbehave)
        {
          g_ioport.p_api->pinRead(ledpresent.p_leds[2], &level);  // Read current level
          g_ioport.p_api->pinWrite(ledpresent.p_leds[2], !level); // Write opposite level

          for(count=0; count<10 ; count++ )
          {
                g_ioport.p_api->pinWrite(RED_LED_PIN, IOPORT_LEVEL_HIGH);
                /* Delay */
                for(delay_counter=0; delay_counter<RED_LED_DELAY_COUNT; delay_counter++);
                /* Turn red off */
                g_ioport.p_api->pinWrite(RED_LED_PIN, IOPORT_LEVEL_LOW);
                /* Delay */
                for(delay_counter=0; delay_counter<RED_LED_DELAY_COUNT; delay_counter++);

                g_sf_thread_monitor.p_api->countIncrement(g_sf_thread_monitor.p_ctrl);

          }
           while(1);
        }
       else
       {
           /* Turn green LED on */
           g_ioport.p_api->pinWrite(GREEN_LED_PIN, IOPORT_LEVEL_HIGH);
           /* Delay */
           for(delay_counter=0; delay_counter<GRN_LED_DELAY_COUNT; delay_counter++);
           /* Turn green off */
           g_ioport.p_api->pinWrite(GREEN_LED_PIN, IOPORT_LEVEL_LOW);
           /* Delay */
           for(delay_counter=0; delay_counter<GRN_LED_DELAY_COUNT; delay_counter++);

           g_sf_thread_monitor.p_api->countIncrement(g_sf_thread_monitor.p_ctrl);

       }

       tx_thread_sleep (1);

    }
}
