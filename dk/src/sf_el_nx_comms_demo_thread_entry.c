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
 * File Name    : sf_el_nx_comms_demo_thread_entry.c
 * Description  : sf_el_nx_comms_thread reads and writes from Telnet echo server via ethernet cable.
 *                User can control the thread by using start and stop command.
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
#include "sf_el_nx_comms_demo_thread.h"

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/


/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/


/***********************************************************************************************************************
Functions
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 *  @brief  sf_el_nx_comms_thread uses framework API's of Comms over Telnet connection and shows how echo server works
 *
 *  @retval VOID
 **********************************************************************************************************************/

void sf_el_nx_comms_demo_thread_entry(void)
{
    ssp_err_t ssp_err;
    uint8_t opt;
    char buffer[] = "\r\n";
    while (1)
    {
        ssp_err = g_sf_comms_telnet.p_api->read(g_sf_comms_telnet.p_ctrl, &opt, 1, TX_WAIT_FOREVER);
        if(ssp_err == SSP_SUCCESS)
        {
            if(opt == '\r')
            {
                ssp_err = g_sf_comms_telnet.p_api->write(g_sf_comms_telnet.p_ctrl,(uint8_t *)buffer, 2,TX_WAIT_FOREVER);
            }
            else
            {
                ssp_err = g_sf_comms_telnet.p_api->write(g_sf_comms_telnet.p_ctrl,&opt, 1,TX_WAIT_FOREVER);
            }
            ssp_error_code_to_string(ssp_err);
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }


    }
}
