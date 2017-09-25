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
 * File Name    :   r_wdt_demo_thread_entry.c
 * Description  :   The thread where WDT detects the abnormal behaviour by system and reset's the system.
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
/* WDT Thread entry function */
#include "r_wdt_demo_thread.h"

/***********************************************************************************************************************
Macro definitions
 ***********************************************************************************************************************/


#define RED_LED_PIN IOPORT_PORT_08_PIN_08
#define GREEN_LED_PIN IOPORT_PORT_08_PIN_07

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/

void hal_wdt_thread_entry(void);
#include "dev_example_thread.h"
#include "dev_example_common.h"

/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/

uint32_t loop_counter;

/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   This thread is a sample where your code may face Abnormal behaviour and hence WDT resets the system.
 *          Here we are simply turning OFF the Green led and turning ON Red led by which we can observe that it is
 *          coming inside our thread and reseting the WDT due to longer time loop execution without refreshing WDT.
 *
 *  @retval VOID
 ***********************************************************************************************************************/
void r_wdt_demo_thread_entry(void)
{
    ssp_err_t ssp_err;
    uint32_t p_count;
    char counter_buffer[20];
    /* Turn green LED off */
    g_ioport.p_api->pinWrite(GREEN_LED_PIN, IOPORT_LEVEL_LOW);
    g_sf_thread_monitor.p_api->close(g_sf_thread_monitor.p_ctrl);
    /* Checking weather the system is in debugging mode.
     * In debugging mode count value is zero */
    ssp_err = g_wdt.p_api->counterGet(g_wdt.p_ctrl,&p_count);
    if((ssp_err == SSP_SUCCESS))
    {
        /* Any Abnormal condition where you want your WDT to check working properly or not.
               If not, WDT should reset the device */
        if(p_count != 0)
        {
            while(1)
            {
                /* ON the red LED and tickle the WDT for a few seconds */
                g_ioport.p_api->pinWrite(RED_LED_PIN, IOPORT_LEVEL_HIGH);
                /* Get the WDT counter value by printing it */
                ssp_err = g_wdt.p_api->counterGet(g_wdt.p_ctrl,&p_count);
                sprintf (counter_buffer,"Counter  =  %u\r\n",(unsigned int) p_count);
                print_to_console(counter_buffer);

            }
        }
        else
        {
            print_to_console("WDT will not reset the system in debugging mode\r\n");
        }
    }
    else
    {
        ssp_error_code_to_string(ssp_err);
    }
}
