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
* File Name    : r_timer_gpt_commands.c
* Description  : The general timer interface provides standard timer functionality including periodic mode, one-shot
mode, and free-running timer mode. After each timer cycle (overflow or underflow), an interrupt can be triggered.
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


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/
/* Prototypes for the callback functions called by I2C console commands */


void timer_on(sf_console_cb_args_t * p_args);
void timer_hal_stop(sf_console_cb_args_t * p_args);
void timer_hal_start(sf_console_cb_args_t * p_args);
void timer_hal_reset(sf_console_cb_args_t * p_args);
void timer_hal_counterGet(sf_console_cb_args_t * p_args);
void timer_hal_periodSet(sf_console_cb_args_t * p_args);
void timer_hal_periodGet(sf_console_cb_args_t * p_args);
void timer_hal_versionGet(sf_console_cb_args_t * p_args);
void timer_hal_statusGet(sf_console_cb_args_t * p_args);
void timer_hal_close(sf_console_cb_args_t * p_args);



/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

#define TIMER_LOG_BUFF_SIZE 64

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/

char timer_log_buffer[TIMER_LOG_BUFF_SIZE];

static bsp_leds_t leds;

const sf_console_command_t g_timer_hal_commands[] =
 {
         {
          (uint8_t*)"open",
          (uint8_t*)"Opens the GPT Timer driver",
          timer_on,
          NULL
         },

         { (uint8_t*)"stop",
           (uint8_t*)"Stop the timer ",
           timer_hal_stop,
           NULL
         },

         {
          (uint8_t*)"start",
          (uint8_t*)"Start the timer",
          timer_hal_start,
          NULL
         },

         { (uint8_t*)"reset",
           (uint8_t*)"Reset the timer to the initial value",
           timer_hal_reset,
           NULL
         },

         {
          (uint8_t*)"counterGet",
          (uint8_t*)"Get current timer value",
          timer_hal_counterGet,
          NULL
         },

         {
          (uint8_t*)"periodSet",
          (uint8_t*)"Set the timer period\r\n\tSynopsis:- periodSet#(Value in msecs)\r\n\tExample:- periodSet 2000",
          timer_hal_periodSet,
          NULL
         },

         {
          (uint8_t*)"versionGet",
          (uint8_t*)"Gets the version of API ",
          timer_hal_versionGet,
          NULL
         },

         {
          (uint8_t*)"infoGet",
          (uint8_t*)"Get information about the timer",
          timer_hal_statusGet,
          NULL
         },

         { (uint8_t*)"close",
           (uint8_t*)"Close the driver  ",
           timer_hal_close,
           NULL
         },

};


const sf_console_menu_t g_timer_hal_menu =
{
        .menu_prev = &g_sf_console_root_menu,
        .menu_name = (uint8_t*)"r_gpt",
        .num_commands = (sizeof(g_timer_hal_commands)) / (sizeof(g_timer_hal_commands[0])),
        .command_list = g_timer_hal_commands

};

/*******************************************************************************************************************//**
 * @brief  Configures timer based on user input to toggle LED.
 *
 * The timer thread configures a user specified timer to toggle a user specified LED at a user specified period rate.
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
***********************************************************************************************************************/
void timer_on(sf_console_cb_args_t * p_args)
{

    UNUSED(p_args);
    ssp_err_t ssp_err = g_timer_hal.p_api->open(g_timer_hal.p_ctrl, g_timer_hal.p_cfg);
    ssp_error_code_to_string(ssp_err);

    R_BSP_LedsGet(&leds);  // Get LED list from BSP
}

/*******************************************************************************************************************//**
 * @brief  Starts the timer (resumes flashing of LED)
 * @pre    "timer_on" command should be entered before "START".
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
***********************************************************************************************************************/
void timer_hal_start(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    /** Start the timer */
    ssp_err_t ssp_err = g_timer_hal.p_api->start(g_timer_hal.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief  Stops the timer (stops flashing of LED)
 * @pre    "ON" command should be entered before "STOP".
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
***********************************************************************************************************************/
void timer_hal_stop(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    /** Stops the timer */
    ssp_err_t ssp_err = g_timer_hal.p_api->stop(g_timer_hal.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Reset the timer (stops flashing of LED)
 * @pre    "ON" command should be entered before "STOP".
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
***********************************************************************************************************************/
void timer_hal_reset(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    /** Start timer */
    ssp_err_t ssp_err = g_timer_hal.p_api->reset(g_timer_hal.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/***********************************************************************************************************************
 * @brief       set the period of timer
 * @pre         "open" command should be entered before "set".
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/

void timer_hal_periodSet(sf_console_cb_args_t * p_args)
{
    uint32_t period;

    period = (uint32_t)strtoul((char*)p_args->p_remaining_string, NULL, 10);

    ssp_err_t ssp_err = g_timer_hal.p_api->periodSet(g_timer_hal.p_ctrl,period,TIMER_UNIT_PERIOD_MSEC);
    ssp_error_code_to_string(ssp_err);

}

/**********************************************************************************************************************
 * @brief       get the timer counter current value
 * @pre         "open" command should be entered before "get".
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_hal_counterGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    timer_size_t value;

    ssp_err_t ssp_err = g_timer_hal.p_api->counterGet(g_timer_hal.p_ctrl,&value);

    if(SSP_SUCCESS == ssp_err)
    {
        sprintf(timer_log_buffer, "CounterValue =  %u\r\n",(unsigned int)value);
        print_to_console(timer_log_buffer);
    }
    else
    {
        ssp_error_code_to_string(ssp_err);
    }
}

/**********************************************************************************************************************
 * @brief       Get the information about the Code version
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/

void timer_hal_versionGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    char   version[32];

    ssp_version_t version_details;
    /** calls SCI_I2C_VERSIONGET API */
    ssp_err_t ssp_err = g_timer_hal.p_api->versionGet(&version_details);
    ssp_error_code_to_string(ssp_err);

    sprintf (version,"Code version  =  %d.%d\r\n", version_details.code_version_major,
            version_details.code_version_minor);
    print_to_console(version);

    sprintf (version,"\r\nAPI version  =  %d.%d\r\n", version_details.api_version_major,
            version_details.api_version_minor);
    print_to_console(version);


}

/**********************************************************************************************************************
 * @brief       information about clock frequency,direction of count,status,period counts
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/

void timer_hal_statusGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    timer_info_t info;
    ssp_err_t ssp_err = g_timer_hal.p_api->infoGet(g_timer_hal.p_ctrl, &info);

    if(SSP_SUCCESS == ssp_err)
    {
        char* str =  info.count_direction ? "UP" : "DOWN";
        snprintf(timer_log_buffer, TIMER_LOG_BUFF_SIZE,"count_direction =  %s\r\n", str);
        print_to_console(timer_log_buffer);

        snprintf(timer_log_buffer, TIMER_LOG_BUFF_SIZE, "clock_frequency =  %u\r\n",
                                                            (unsigned int)info.clock_frequency);
        print_to_console(timer_log_buffer);

        snprintf(timer_log_buffer, TIMER_LOG_BUFF_SIZE, "period_counts =  %u\r\n",
                                                            (unsigned int)info.period_counts);
        print_to_console(timer_log_buffer);

        str = (info.status == TIMER_STATUS_COUNTING) ? "COUNTING" : "STOPPED";
        snprintf(timer_log_buffer, TIMER_LOG_BUFF_SIZE, "status =  %s\r\n", str);
        print_to_console(timer_log_buffer);
    }
    else
    {
        ssp_error_code_to_string(ssp_err);
    }
}

/**********************************************************************************************************************
 * @brief       close the timer
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/

void timer_hal_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    /** Close the timer APIs */
    ssp_err_t ssp_err = g_timer_hal.p_api->close(g_timer_hal.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}

/***********************************************************************************************************************
 * @brief       callback function of timer interrupt
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/

void Timer0_Interrupt(timer_callback_args_t * p_args)
{
    UNUSED(p_args);

    ioport_level_t level;

    g_ioport.p_api->pinRead(leds.p_leds[0], &level);  // Read current level
    g_ioport.p_api->pinWrite(leds.p_leds[0], !level); // Write opposite level

}

