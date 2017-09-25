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
 * File Name    : sf_ThreadMonitor_dev_example.c
 * Description  : Thread Monitor Sample application with CLI. This file contains required menu for Thread Monitor
 *                command. A method which starts the Thread Monitor App. This method takes a argument misbehave1 or
 *                misbehave0, with respect to argument application show two behavior like reseting and not reseting device.
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

#include <stdint.h>
#include "dev_example_thread.h"
#include "dev_example_common.h"
#include "sf_thread_monitor_demo_thread.h"

/***********************************************************************************************************************
Macro definitions
 ***********************************************************************************************************************/


/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Extern global variables
 ***********************************************************************************************************************/
extern TX_THREAD sf_thread_monitor_demo_thread;

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
/* Prototypes for the callback functions called by Thread Monitor console commands */

void start_app(sf_console_cb_args_t * p_args);

/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/
volatile uint32_t misbehave = 0;


const sf_console_command_t g_sf_thread_monitor_commands[] =
{
 {
  (uint8_t*)"demo_thread_monitor",
  (uint8_t*)"Starts Thread Monitor application  example:- demo_thread_monitor misbehave# (0 or 1)",
  start_app,
  NULL
 },

};


/** Thread Monitor menu */
const sf_console_menu_t g_sf_tread_monitor_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"sf_thread_monitor",
 .num_commands = (sizeof(g_sf_thread_monitor_commands)) / (sizeof(g_sf_thread_monitor_commands[0])),
 .command_list = g_sf_thread_monitor_commands
};


/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/



/*******************************************************************************************************************//**
 * @brief Resumes the thread which will be monitored.
 *
 * @example demo_thread_monitor misbehave# (0 or 1)
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void start_app(sf_console_cb_args_t * p_args)
{
    char   result[32];

    /** Extracting reset value from console*/

    if(!get_arg_as_uint("misbehave", p_args->p_remaining_string, (uint32_t*)&misbehave,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    /** validating reset value */
    if(misbehave < 2)
    {

        tx_thread_resume(&sf_thread_monitor_demo_thread);

        /**Used to print Min and Max count value for a given thread
         *  note : take care of number of threads registered for monitoring. For this example only
         *         two threads have been registered */
        while(g_sf_thread_monitor.p_ctrl->profiling_mode_enabled)
        {
            tx_thread_sleep (1);
            sprintf (result,"MIN[0] = %d\r\n",(int)g_sf_thread_monitor.p_ctrl->thread_counters[0].minimum_count);
            g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)result,TX_WAIT_FOREVER);
            sprintf (result,"MAX[0] = %d\r\n",(int)g_sf_thread_monitor.p_ctrl->thread_counters[0].maximum_count);
            g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)result,TX_WAIT_FOREVER);
        }

    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }


}


