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
* File Name    : sf_i2c_dev_example.c
* Description  : I2C Framework implementation on the SCI-IIC peripheral. Two I2C devices, one on each breakout board
*                and mainboard each trying write at same time which results in LED turn ON or OFF. The Synchronization
*                is handled by the framework.
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


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* This App version */
#define APP_VERSION                 ("1.00")

#define SF_I2C_stop_COMMAND         "stop"
#define SF_I2C_start_COMMAND        "start"

#define STR_NOT_SUPPORTED           "Not supported in this version"

#define SF_I2C_stop_HELP_STRING     "Stop the application"
#define SF_I2C_start_HELP_STRING    "Start the application"

/***********************************************************************************************************************
Extern global variables
***********************************************************************************************************************/

extern const sf_i2c_instance_t g_sf_i2c_device_breakoutboard;
extern const sf_i2c_instance_t g_sf_i2c_device_mainboard;
extern TX_THREAD sf_i2c_pcal1_thread;
extern TX_THREAD sf_i2c_pcal2_thread;
//extern volatile int start;

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/

void i2c_sf_stop(sf_console_cb_args_t * p_args);
void i2c_sf_start(sf_console_cb_args_t * p_args);

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/

uint8_t running = 0;
uint8_t MainBoardLedConfigOutput[2] = {0x06,0x00};
uint8_t BreakOutBoardConfigOutput[2] = {0x07,0x00};


/** i2c framework console menu commands */
const sf_console_command_t g_sf_i2c_commands[] =
{
         {
           .command  = (uint8_t*)SF_I2C_start_COMMAND,
           .help     = (uint8_t*)SF_I2C_start_HELP_STRING,
           .callback = i2c_sf_start,
           .context  = NULL
         },
         {
          .command  = (uint8_t*)SF_I2C_stop_COMMAND,
          .help     = (uint8_t*)SF_I2C_stop_HELP_STRING,
          .callback = i2c_sf_stop,
          .context  = NULL
        },
};

/** i2c framework console menu */
const sf_console_menu_t g_sf_i2c_menu =
{
        .menu_prev = &g_sf_console_root_menu,
        .menu_name = (uint8_t*)"sf_i2c",
        .num_commands = (sizeof(g_sf_i2c_commands)) / (sizeof(g_sf_i2c_commands[0])),
        .command_list = g_sf_i2c_commands
};

/***********************************************************************************************************************
Functions
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief   Starts the application. Open two IIC device and Configure it as Output and Start the application
 *
 *  Checks whether application already running or not. If not started, this function will call the SF_I2C driver API
 *  OPEN and WRITE else informs user that application already running.
 *
 *  @example start
 *  @retval VOID
***********************************************************************************************************************/

void i2c_sf_start(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    /* Close the SCI I2C HAL driver incase it is opened from its console command
     * This is required for framework code to work */
    g_i2c_sci.p_api->close(g_i2c_sci.p_ctrl);

    /** check Application running or not */
    if(!running)
    {
        g_sf_i2c_device_breakoutboard.p_api->open(g_sf_i2c_device_breakoutboard.p_ctrl,g_sf_i2c_device_breakoutboard.p_cfg);
        /** configuring break out board PCAL port 1 as output  */
        g_sf_i2c_device_breakoutboard.p_api->write(g_sf_i2c_device_breakoutboard.p_ctrl, BreakOutBoardConfigOutput, 2, false, TX_WAIT_FOREVER);
        g_sf_i2c_device_mainboard.p_api->open(g_sf_i2c_device_mainboard.p_ctrl,g_sf_i2c_device_mainboard.p_cfg);
        /** configuring main board PCAL port 0 as output  */
        g_sf_i2c_device_mainboard.p_api->write(g_sf_i2c_device_mainboard.p_ctrl, MainBoardLedConfigOutput, 2, false, TX_WAIT_FOREVER);

        tx_thread_resume(&sf_i2c_pcal1_thread);
        tx_thread_resume(&sf_i2c_pcal2_thread);

        g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t*)"Application started \r\n", TX_WAIT_FOREVER);

        running = 1;

    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t*)"Application already running\r\n", TX_WAIT_FOREVER);

    }
}

/*******************************************************************************************************************//**
 * @brief   Stops the application. Closes two IIC device and Stops the application
 *
 *  Checks whether application already stopped or not. If not stopped, this function will call the SF_I2C driver API
 *  CLOSE else informs user that application not running presently.
 *
 *  @example stop
 *  @retval VOID
***********************************************************************************************************************/

void i2c_sf_stop(sf_console_cb_args_t * p_args)
{

    UNUSED(p_args);

    /** check Application running or not */
    if(running)
    {
        g_sf_i2c_device_mainboard.p_api->close(g_sf_i2c_device_mainboard.p_ctrl);
        g_sf_i2c_device_breakoutboard.p_api->close(g_sf_i2c_device_breakoutboard.p_ctrl);

        tx_thread_suspend(&sf_i2c_pcal1_thread);
        tx_thread_suspend(&sf_i2c_pcal2_thread);

        g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t*)"Application stopped \r\n", TX_WAIT_FOREVER);

        running = 0;
    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t*)"Start Application first to stop \r\n", TX_WAIT_FOREVER);
    }


}



