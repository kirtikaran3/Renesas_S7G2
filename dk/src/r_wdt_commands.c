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
 * File Name    :   r_wdt_commands.c
 * Description  :   WDT hal commands implementation. Here some of the WDT commands are shown how to implement
 *                  and also a command is created where a sample thread is resumed where the WDT Reset's.
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

#include "r_wdt_demo_thread.h"
#include "dev_example_thread.h"
#include "dev_example_common.h"

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/

void wdt_hal_open(sf_console_cb_args_t * p_args);
void wdt_hal_statusGet(sf_console_cb_args_t * p_args);
void wdt_hal_statusClear(sf_console_cb_args_t * p_args);
void wdt_hal_cfgGet(sf_console_cb_args_t * p_args);
void wdt_hal_counterGet(sf_console_cb_args_t * p_args);
void wdt_hal_timeOutGet(sf_console_cb_args_t * p_args);
void wdt_hal_versionGet(sf_console_cb_args_t * p_args);
void wdt_demo(sf_console_cb_args_t * p_args);


/***********************************************************************************************************************
Extern global variables
 ***********************************************************************************************************************/
extern TX_THREAD r_wdt_demo_thread;

/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/
#define GREEN_LED_PIN IOPORT_PORT_08_PIN_07

/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/



/** WDT HAL Module console menu commands */
const sf_console_command_t g_hal_wdt_commands[] =
{
 {
  (uint8_t*)"open",
  (uint8_t*)"Initialize wdt unit",
  wdt_hal_open,
  NULL
 },
 {
  (uint8_t*)"cfgGet",
  (uint8_t*)"Initialize the WDT in register start mode",
  wdt_hal_cfgGet,
  NULL
 },
 {
  (uint8_t*)"statusGet",
  (uint8_t*)"Read the status of the WDT.",
  wdt_hal_statusGet,
  NULL
 },
 {
  (uint8_t*)"statusClear",
  (uint8_t*)"Clear the status of the WDT.",
  wdt_hal_statusClear,
  NULL
 },
 {
  (uint8_t*)"counterGet",
  (uint8_t*)"Read the current WDT counter value.",
  wdt_hal_counterGet,
  NULL
 },
 {
  (uint8_t*)"timeOutGet",
  (uint8_t*)"Read the WDT timeout value.",
  wdt_hal_timeOutGet,
  NULL
 },
 {
  (uint8_t*)"versionGet",
  (uint8_t*)"Give the version of API",
  wdt_hal_versionGet,
  NULL
 },
 {
  (uint8_t*)"wdtDemo",
  (uint8_t*)"Give the demo for wdt reset",
  wdt_demo,
  NULL
 }
};


/** WDT HAL Module console menu */
const sf_console_menu_t g_wdt_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"r_wdt",
 .num_commands = (sizeof(g_hal_wdt_commands)) / (sizeof(g_hal_wdt_commands[0])),
 .command_list = g_hal_wdt_commands
};

/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   Starts the application.In register start mode it Initialize the WDT by using wdt_open command.
 *          Auto start mode is not supporting open command.
 *
 *          To check weather it is working or not we Turned ON Green led.
 *
 *
 *  @example start
 *  @retval VOID
 ***********************************************************************************************************************/

void wdt_hal_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    /* Open the WDT */
    ssp_err = g_wdt.p_api->open(g_wdt.p_ctrl, (wdt_cfg_t *const)g_wdt.p_cfg);

    /* WDT is locked in thread monitor by default, again unlock WDT open and get success*/
    if(ssp_err == SSP_ERR_HW_LOCKED)
    {
        R_BSP_HardwareUnlock(BSP_HW_LOCK_WDT);
        ssp_err = g_wdt.p_api->open(g_wdt.p_ctrl, (wdt_cfg_t *const)g_wdt.p_cfg);
    }

    ssp_error_code_to_string(ssp_err);
    g_ioport.p_api->pinWrite(GREEN_LED_PIN, IOPORT_LEVEL_HIGH);

}

/*******************************************************************************************************************//**
 * @brief   To get the configuration values of the WDT this command is used.
 *
 *
 *  @example cfgget
 *  @retval VOID
 ***********************************************************************************************************************/

void wdt_hal_cfgGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    wdt_cfg_t wdt_cfg;
    char buffer[32];
    /* Get the configuration of WDT */
    ssp_err = g_wdt.p_api->cfgGet(g_wdt.p_ctrl,(wdt_cfg_t *const)g_wdt.p_cfg);

    if(ssp_err == SSP_SUCCESS)
    {
        /* Printing some important configuration of WDT on console */
        sprintf (buffer,"Timeout  =  %d\r\n",wdt_cfg.timeout);
        print_to_console(buffer);

        sprintf (buffer,"Clock Division  =  %d\r\n",wdt_cfg.clock_division);
        print_to_console(buffer);

        sprintf (buffer,"StartMode  =  %d\r\n",wdt_cfg.start_mode);
        print_to_console(buffer);
    }

    else
    {
        ssp_error_code_to_string(ssp_err);
    }

}


/*******************************************************************************************************************//**
 * @brief   To get the current status of the WDT this command is used.
 *
 *
 *  @example statusget
 *  @retval VOID
 ***********************************************************************************************************************/

void wdt_hal_statusGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    wdt_status_t wdt_stat;
    char buffer[32];
    /* Get the status of WDT */
    ssp_err = g_wdt.p_api->statusGet(g_wdt.p_ctrl, &wdt_stat);
    if(ssp_err == SSP_SUCCESS)
    {
        sprintf (buffer,"Status  =  %u\r\n",wdt_stat);
        print_to_console(buffer);
    }

    else
    {
        ssp_error_code_to_string(ssp_err);
    }
}


/*******************************************************************************************************************//**
 * @brief   To clear the current status of the WDT this command is used.
 *
 *
 *  @example statusclear status#
 *  @retval VOID
 ***********************************************************************************************************************/

void wdt_hal_statusClear(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ssp_err = g_wdt.p_api->statusClear(g_wdt.p_ctrl,WDT_STATUS_UNDERFLOW_AND_REFRESH_ERROR);
    ssp_error_code_to_string(ssp_err);

}


/*******************************************************************************************************************//**
 * @brief   To get the current counter value this command is used.
 *
 *  @example counterget
 *  @retval VOID
 ***********************************************************************************************************************/

void wdt_hal_counterGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    uint32_t p_count;
    char buffer[32];
    /* Get the counter value of WDT */
    ssp_err = g_wdt.p_api->counterGet(g_wdt.p_ctrl,&p_count);
    if(ssp_err == SSP_SUCCESS)
    {
        sprintf (buffer,"Counter  =  %u\r\n",(uint32_t )p_count);
        print_to_console(buffer);
    }

    else
    {
        ssp_error_code_to_string(ssp_err);
    }
}


/*******************************************************************************************************************//**
 * @brief   To get the timeout value of watchdog.
 *
 *  @example timeoutget
 *  @retval VOID
 ***********************************************************************************************************************/


void wdt_hal_timeOutGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    wdt_timeout_values_t timeout;
    char buffer[32];
    ssp_err = g_wdt.p_api->timeoutGet(g_wdt.p_ctrl, &timeout);
    if(ssp_err == SSP_SUCCESS)
    {
        sprintf(buffer,"Clock frequency = %lu\r\n",timeout.clock_frequency_hz);
        print_to_console(buffer);
        sprintf (buffer,"Timeout clocks  =  %lu\r\n",timeout.timeout_clocks);
        print_to_console(buffer);
    }
    else
    {
        ssp_error_code_to_string(ssp_err);
    }
}


/*******************************************************************************************************************//**
 * @brief Get the version of WDT HAL APIs
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @example counterget
 * @retval VOID
 ***********************************************************************************************************************/

void wdt_hal_versionGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    char   version[32];
    ssp_version_t p_data;
    /* Get the  of WDT */
    ssp_err = g_wdt.p_api->versionGet(&p_data);

    if(ssp_err == SSP_SUCCESS)
    {
        sprintf (version,"Code version  =  %d.%d\r\n",p_data.code_version_major,p_data.code_version_minor);
        print_to_console(version);
        sprintf (version,"\r\nAPI version  =  %d.%d\r\n", p_data.api_version_major,p_data.api_version_minor);
        print_to_console(version);
    }

    else
    {
        ssp_error_code_to_string(ssp_err);
    }
}


/*******************************************************************************************************************//**
 * @brief    It will show that the device will reset when the Processor goes inside a loop where Watchdog
 *           could not able to refresh for long time.
 *
 *  @example wdtdemo
 *  @retval VOID
 ***********************************************************************************************************************/

void wdt_demo(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    tx_thread_resume(&r_wdt_demo_thread);

}


