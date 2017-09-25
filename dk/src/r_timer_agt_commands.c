
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
 * File Name    : r_timer_agt_commands.c
 * Description  : The Asynchronous general timer interface provides standard timer functionality including periodic mode, one-shot
 * mode,Pulse output mode,event counter mode,pulse width measurement mode, pulse period measurement mode
 * and free-running timer mode. After each timer cycle (overflow or underflow), an interrupt can be triggered.
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
/** Command Menu */
#define            TIMER_OPEN          "open"
#define            CLOSE               "close"
#define            COUNTER_GET         "counterGet"
#define            PERIOD_SET          "periodSet"
#define            DUTY_CYCLE_SET      "dutyCycleSet"
#define            RESET               "Reset"
#define            START               "Start"
#define            INFO_GET            "Info"
#define            STOP                "Stop"
#define            VERSION_GET         "Version"

/**  HELP MENU   */

#define            HELP_TIMER_OPEN     "Open the AGT channel as a timer"
#define            HELP_CLOSE          "Stops counter, disables interrupts and output pins"\
		                                "\r\n\t and clears internal driver data"
#define            HELP_COUNTER_GET    "Retrieve and store counter value"
#define            HELP_PERIOD_SET     "Sets period value\r\n\tSynopsis:- periodSet#(Value in msecs)\r\n\tExample:- "\
		                                "periodSet 1000"
#define            HELP_DUTY_CYCLE_SET "Setting duty cycle is not supported by this driver"
#define            HELP_RESET          "Resets the counter value to the period that was originally set"
#define            HELP_START          "Starts timer"
#define            HELP_INFOGET        "Get timer information"
#define            HELP_STOP           "Stops the AGT channel"
#define            HELP_VERSIONGET     "Driver version"


/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
void timer_agt_hal_on               (sf_console_cb_args_t *p_args);
void timer_agt_hal_stop             (sf_console_cb_args_t *p_args);
void timer_agt_hal_start            (sf_console_cb_args_t *p_args);
void timer_agt_hal_reset            (sf_console_cb_args_t *p_args);
void timer_agt_hal_counterGet       (sf_console_cb_args_t *p_args);
void timer_agt_hal_periodSet        (sf_console_cb_args_t *p_args);
void timer_agt_hal_duty_cycle_set   (sf_console_cb_args_t *p_args);
void timer_agt_hal_versionGet       (sf_console_cb_args_t *p_args);
void timer_agt_hal_infoGet          (sf_console_cb_args_t *p_args);
void timer_agt_hal_close            (sf_console_cb_args_t *p_args);


/* Global variables */
/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/
static char temp_buff[64];
static bsp_leds_t leds;
static ioport_level_t level = IOPORT_LEVEL_LOW;


/* Timer Commands Deceleration  */
const sf_console_command_t g_timer_hal_gpt_commands[] =
{
 {
  .command   =  (uint8_t*)TIMER_OPEN,
  .help      =  (uint8_t*)HELP_TIMER_OPEN,
  .callback  =  timer_agt_hal_on,
  .context   =  NULL
 },
 {
  .command   =   (uint8_t*)CLOSE,
  .help      =  (uint8_t*)HELP_CLOSE,
  .callback  =  timer_agt_hal_close,
  .context   =  NULL
 },
 {
  .command   =  (uint8_t*)COUNTER_GET,
  .help      =  (uint8_t*)HELP_COUNTER_GET,
  .callback  =  timer_agt_hal_counterGet,
  .context   =  NULL
 },
 {
  .command   =  (uint8_t*)PERIOD_SET,
  .help      =  (uint8_t*)HELP_PERIOD_SET,
  .callback  =  timer_agt_hal_periodSet,
  .context   =  NULL
 },
 {
  .command   =  (uint8_t*)DUTY_CYCLE_SET,
  .help      =  (uint8_t*)HELP_DUTY_CYCLE_SET,
  .callback  =  timer_agt_hal_duty_cycle_set,
  .context   =  NULL
 },
 {
  .command   =  (uint8_t*)RESET,
  .help      =  (uint8_t*)HELP_RESET,
  .callback  =  timer_agt_hal_reset,
  .context   =  NULL
 },
 {
  .command   =  (uint8_t*)START,
  .help      =  (uint8_t*)HELP_START,
  .callback  =  timer_agt_hal_start,
  .context   =  NULL
 },
 {
  .command   =  (uint8_t*)INFO_GET,
  .help      =  (uint8_t*)HELP_INFOGET,
  .callback  =  timer_agt_hal_infoGet,
  .context   =  NULL
 },
 {
  .command   =  (uint8_t*)STOP,
  .help      =  (uint8_t*)HELP_STOP,
  .callback  =  timer_agt_hal_stop,
  .context   =  NULL
 },
 {
  .command   =  (uint8_t*)VERSION_GET,
  .help      =  (uint8_t*)HELP_VERSIONGET,
  .callback  =  timer_agt_hal_versionGet,
  .context   =  NULL
 },
};//end//



sf_console_menu_t g_timer_agt_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .num_commands = sizeof(g_timer_hal_gpt_commands)/sizeof(g_timer_hal_gpt_commands[0]),
 .menu_name = (uint8_t*)"r_agt",
 .command_list = g_timer_hal_gpt_commands
};

/*Function Bodies */
/**********************************************************************************************************************
 * @brief  Configures Asynchronous General Purpose timer(AGT) based on user input to toggle LED.
 *         the time is clocked via local oscillator(LOCO) at 32768 Hz.
 *         The timer thread configures a user specified timer to toggle a user specified LED at a user
 *         specified period rate.
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_agt_hal_on(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err =  g_timer_agt.p_api->open(g_timer_agt.p_ctrl,g_timer_agt.p_cfg);
    ssp_error_code_to_string(ssp_err);
    ssp_err = R_BSP_LedsGet(&leds);
    ssp_error_code_to_string(ssp_err);
}
/**********************************************************************************************************************
 * @brief       Stops the timer (stops flashing of LED)
 * @pre         "open" command should be entered before "stop".
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_agt_hal_stop(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    level = IOPORT_LEVEL_LOW;
    ssp_err_t ssp_err =  g_timer_agt.p_api->stop(g_timer_agt.p_ctrl);
    ssp_error_code_to_string(ssp_err);
    g_ioport.p_api->pinWrite(leds.p_leds[0],level);
}
/***********************************************************************************************************************
 * @brief       Starts the timer (resumes flashing of LED)
 * @pre         "open" command should be entered before "start".
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_agt_hal_start(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_timer_agt.p_api->start(g_timer_agt.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}
/***********************************************************************************************************************
 * @brief       Reset the timer (stops flashing of LED)
 * @pre         "stop" command should be entered before "reset".
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_agt_hal_reset(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_timer_agt.p_api->reset(g_timer_agt.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}
/**********************************************************************************************************************
 * @brief       get the timer counter current value
 * @pre         "open" command should be entered before "get".
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_agt_hal_counterGet(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    timer_size_t  timer_val;
    ssp_err_t ssp_err = g_timer_agt.p_api->counterGet(g_timer_agt.p_ctrl,&timer_val);
    if(SSP_SUCCESS == ssp_err)
    {
        sprintf(temp_buff,"Counter Value = %u\r\n",(unsigned int)timer_val);
        print_to_console(temp_buff);
    }
    else
    {
        ssp_error_code_to_string(ssp_err);
    }
}
/***********************************************************************************************************************
 * @brief       set the period of timer
 * @pre         "open" command should be entered before "set".
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_agt_hal_periodSet(sf_console_cb_args_t *p_args)
{
    timer_size_t value = (uint32_t)strtoul((char*)p_args->p_remaining_string,NULL,10);
    ssp_err_t ssp_err = g_timer_agt.p_api->periodSet(g_timer_agt.p_ctrl,value,TIMER_UNIT_PERIOD_MSEC);
    ssp_error_code_to_string(ssp_err);
}
/***********************************************************************************************************************
 * @brief       set the duty cycle of the timer(!!NOT SUPPORTED BY AGT TIMER DRIVER)
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_agt_hal_duty_cycle_set(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    timer_size_t size=0;
    timer_pwm_unit_t pmw_unit=0;
    uint8_t value = 0;
    ssp_err_t ssp_err = g_timer_agt.p_api->dutyCycleSet(g_timer_agt.p_ctrl,size,pmw_unit,value);
    if(SSP_SUCCESS!=ssp_err)
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
void timer_agt_hal_versionGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_version_t version;
    char version_info[32];
    ssp_err_t ssp_err = g_timer_agt.p_api->versionGet(&version);
    if(SSP_SUCCESS!=ssp_err)
    {
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        sprintf(version_info,"\r\nCode Version = %d.%d\r\n",version.code_version_major,version.code_version_minor);
        print_to_console(version_info);

        sprintf(version_info,"\r\nAPI  Version = %d.%d\r\n",version.api_version_major,version.api_version_minor);
        print_to_console(version_info);
    }

}

/**********************************************************************************************************************
 * @brief       information about clock frequency,direction of count,status,period counts
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_agt_hal_infoGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    timer_info_t info;
    ssp_err_t ssp_err = g_timer_agt.p_api->infoGet(g_timer_agt.p_ctrl,&info);
    if(SSP_SUCCESS!=ssp_err)
    {
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        char* direc =  info.count_direction ? "UP" : "DOWN";
        char* status = info.status ? "STOPPED":"COUNTING";
        sprintf(temp_buff,"\r\n"
                "Direction     =  %s\r\n"
                "Frequency     =  %u\r\n"
                "Period_counts =  %u\r\n"
                "Status        =  %s\r\n",
                direc,(unsigned int)info.clock_frequency,(unsigned int)info.period_counts,status);

        print_to_console(temp_buff);
    }
}
/**********************************************************************************************************************
 * @brief       close the timer
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void timer_agt_hal_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_timer_agt.p_api->close(g_timer_agt.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}
/***********************************************************************************************************************
 * @brief       callback function of timer interrupt
 *
 * @param[in]   p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void AGT_timer_INTERRUPT(timer_callback_args_t * p_args)
{
    UNUSED(p_args);
    level = !level;
    g_ioport.p_api->pinWrite(leds.p_leds[0],level);
    // you can add your own logic when timer interrupt occur
}
