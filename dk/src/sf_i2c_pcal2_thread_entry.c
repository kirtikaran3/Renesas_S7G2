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
* File Name    : sf_i2c_pcal2_thread.c
* Description  : sf_i2c_pcal2_thread turns breakout board 1 LED ON and OFF Via PCAL through port 1 (address 0x23) when user
*                gives start and stop command respectively.
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

#include "sf_i2c_pcal2_thread.h"

void sf_i2c_pcal2_thread_entry(void);


/***********************************************************************************************************************
Extern global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief   sf_i2c_pcal2_thread turns breakout board 1 LED ON and OFF Via PCAL through port 1 (address 0x23) when user
 *                gives start and stop command respectively
 *
 *  @retval VOID
***********************************************************************************************************************/

void sf_i2c_pcal2_thread_entry(void)
{
    /* TODO: add your own code here */
    uint8_t BreakOutBoardLedOff[2] = {0x03,0x40};
    uint8_t BreakOutBoardLedOn[2] = {0x03,0x80};

     while(1)
    {
            g_sf_i2c_device_breakoutboard.p_api->write(g_sf_i2c_device_breakoutboard.p_ctrl, BreakOutBoardLedOn, 2, false, TX_WAIT_FOREVER);
            tx_thread_sleep(50);
            g_sf_i2c_device_breakoutboard.p_api->write(g_sf_i2c_device_breakoutboard.p_ctrl, BreakOutBoardLedOff, 2, false, TX_WAIT_FOREVER);
            tx_thread_sleep(50);
    }

}
