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
 * File Name    : sf_el_nx_comms_dev_example.c
 * Description  :SSP Telnet communications framework descriptors
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
#include "dev_example_thread.h"
#include "dev_example_common.h"


extern TX_THREAD sf_el_nx_comms_demo_thread;
static bool start_flag = false;

/***********************************************************************************************************************
Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
void sf_el_nx_comms_start(sf_console_cb_args_t * p_args);
void sf_el_nx_comms_stop(sf_console_cb_args_t *p_args);


const sf_console_command_t g_sf_el_nx_comms_commands[] =
{

 {
  (uint8_t*)"start",
  (uint8_t*)"Starts communication between telnet server and host PC",
  sf_el_nx_comms_start,
  NULL
 },

 {
  (uint8_t*)"stop",
  (uint8_t*)"Stops telnet server",
  sf_el_nx_comms_stop,
  NULL
 },
};



/** comms telnet menu */
const sf_console_menu_t g_sf_el_nx_comms_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"sf_el_nx_comms",
 .num_commands = (sizeof(g_sf_el_nx_comms_commands)) / (sizeof(g_sf_el_nx_comms_commands[0])),
 .command_list = g_sf_el_nx_comms_commands

};


/*******************************************************************************************************************//**
 * @brief   Starts the application. works as an echo server and prints what ever user types after start
 *          on the telnet console.

 *
 *  @example start
 *  @retval VOID
 ***********************************************************************************************************************/


void sf_el_nx_comms_start(sf_console_cb_args_t * p_args)
{

    UNUSED(p_args);
    if(start_flag == false)
    {
        print_to_console("Application Started\r\nConnect to Telnet Server\r\n");
        start_flag = true;
        tx_thread_resume(&sf_el_nx_comms_demo_thread);
    }

    else
    {
        print_to_console("Application already started\r\n");
    }
}


/*******************************************************************************************************************//**
 * @brief   Stops the application. Suspends the sf_el_nx_comms_thread thread.
 *
 *  @example stop
 *  @retval VOID
 ***********************************************************************************************************************/


void sf_el_nx_comms_stop(sf_console_cb_args_t *p_args)
{

    UNUSED(p_args);
    if(start_flag == true)
    {
        print_to_console("Application Stopped\r\n");
        start_flag = false;
        tx_thread_suspend(&sf_el_nx_comms_demo_thread);
    }
    else
    {
        print_to_console("Application not started\r\n");
    }


}

