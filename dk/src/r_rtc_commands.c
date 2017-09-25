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
 * File Name    : r_rtc_commands.c
 * Description  : Interface for the Real Time Clock.
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
/* Prototypes for the callback functions called by RTC console commands */

void rtc_hal_open(sf_console_cb_args_t * p_args);
void rtc_hal_close(sf_console_cb_args_t * p_args);
void rtc_hal_calendarTimeSet(sf_console_cb_args_t * p_args);
void rtc_hal_calendarTimeGet(sf_console_cb_args_t * p_args);
void rtc_hal_calendarAlarmSet(sf_console_cb_args_t * p_args);
void rtc_hal_calendarAlarmGet(sf_console_cb_args_t * p_args);
void rtc_hal_calendarCounterStart(sf_console_cb_args_t * p_args);
void rtc_hal_calendarCounterStop(sf_console_cb_args_t * p_args);
void rtc_hal_irqEnable(sf_console_cb_args_t * p_args);
void rtc_hal_irqDisable(sf_console_cb_args_t * p_args);
void rtc_hal_periodicIrqRateSet(sf_console_cb_args_t * p_args);
void rtc_hal_infoGet(sf_console_cb_args_t * p_args);
void rtc_hal_versionGet(sf_console_cb_args_t * p_args);

/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/


/***********************************************************************************************************************
Extern global variables
 ***********************************************************************************************************************/
static bsp_leds_t leds;

/** Previous menu */
extern const sf_console_menu_t g_sf_console_root_menu;

/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/
const sf_console_command_t g_rtc_hal_commands[] =
{
 {
  (uint8_t*)"open",
  (uint8_t*)"Open the RTC driver.",
  rtc_hal_open,
  NULL
 },
 {
  (uint8_t*)"close",
  (uint8_t*)"Close the RTC driver.",
  rtc_hal_close,
  NULL
 },
 {
  (uint8_t*)"calendarTimeSet",
  (uint8_t*)"Set calendar\r\n\t\tSynopsis:- calendarTimeSet sec# min# hour# mday# month# year# start#",
  rtc_hal_calendarTimeSet,
  NULL
 },
 {
  (uint8_t*)"calendarTimeGet",
  (uint8_t*)"Get the calendar time",
  rtc_hal_calendarTimeGet,
  NULL
 },
 {
  (uint8_t*)"calendarAlarmSet",
  (uint8_t*)"Set alarm\r\n\t\tSynopsis:- calendarAlarmSet sec# min# hour#"
  "\r\n\t\tExample:- calendarAlarmSet sec30 min45 hour12",
  rtc_hal_calendarAlarmSet,
  NULL
 },
 {
  (uint8_t*)"calendarAlarmGet",
  (uint8_t*)"Get the calendar alarm time.",
  rtc_hal_calendarAlarmGet,
  NULL
 },
 {
  (uint8_t*)"calendarCounterStart",
  (uint8_t*)"Start the calendar counter..",
  rtc_hal_calendarCounterStart,
  NULL
 },
 {
  (uint8_t*)"calendarCounterStop" ,
  (uint8_t*)"Stop the calendar counter.",
  rtc_hal_calendarCounterStop,
  NULL
 },
 {
  (uint8_t*)"irqEnable",
  (uint8_t*)"Enable the alarm irq\r\n\t\tSynopsis:- irqEnable irq#(0 to 2)"
  "\r\n\t\tExample:- irqEnable irq1",
  rtc_hal_irqEnable,
  NULL
 },
 {
  (uint8_t*)"irqDisable",
  (uint8_t*)"Disable the alarm irq\r\n\t\tSynopsis:- irqDisable irq#(0 to 2)"
  "\r\n\t\tExample:- irqDisable irq0",
  rtc_hal_irqDisable,
  NULL
 },
 {
  (uint8_t*)"periodicIrqRateSet",
  (uint8_t*)"Set the periodic irq rate\r\n\t\tSynopsis:- periodicIrqRateSet rate#(6 t0 15)"
  "\r\n\t\tExample:- periodicIrqRateSet rate7",
  rtc_hal_periodicIrqRateSet,
  NULL
 },
 {
  (uint8_t*)"infoGet",
  (uint8_t*)"Gets information about the driver including the source clock",
  rtc_hal_infoGet,
  NULL
 },
 {
  (uint8_t*)"versionGet",
  (uint8_t*)"Gets the version of API  ",
  rtc_hal_versionGet,
  NULL
 },
};


/** rtc menu */

const sf_console_menu_t g_rtc_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"r_rtc",
 .num_commands = (sizeof(g_rtc_hal_commands)) / (sizeof(g_rtc_hal_commands[0])),
 .command_list = g_rtc_hal_commands

};

/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief  Opens and configures the RTC driver module.
 * Configuration includes clock source, error correction, 12/24 hour mode, and interrupt callback function.
 * If the sub-clock oscillator is the clock source it is started in this function.
 *
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void rtc_hal_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_rtc.p_api->open(g_rtc.p_ctrl,g_rtc.p_cfg);
    ssp_error_code_to_string(ssp_err);
    R_BSP_LedsGet(&leds);
}


/*******************************************************************************************************************//**
 * @brief  Close the RTC driver
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void rtc_hal_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_rtc.p_api->close(g_rtc.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief   Set the calendar time
 *
 * @example calendertimeset sec# min# hour# mday# month# year# start#
 *
 *  @retval VOID
 ***********************************************************************************************************************/

void rtc_hal_calendarTimeSet(sf_console_cb_args_t * p_args)
{
    rtc_time_t time;
    bool clock_start;

    /** Extract the Calendar time and date parameters from console*/

    if(!get_arg_as_uint("sec", p_args->p_remaining_string, (uint32_t*)&(time.tm_sec),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("min", p_args->p_remaining_string, (uint32_t*)&(time.tm_min),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("hour", p_args->p_remaining_string, (uint32_t*)&(time.tm_hour),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("mday", p_args->p_remaining_string, (uint32_t*)&(time.tm_mday),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("month", p_args->p_remaining_string, (uint32_t*)&(time.tm_mon),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("year", p_args->p_remaining_string, (uint32_t*)&(time.tm_year),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("start", p_args->p_remaining_string, (uint32_t*)&clock_start,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validate the Alarm time parameters extracted from console*/
    if((0 > time.tm_sec) || (time.tm_sec >= 60) || (0 > time.tm_min) || (time.tm_min >= 60) || (0 > time.tm_hour)\
            || (time.tm_hour >= 24) || (0 > time.tm_mday) || (time.tm_mday >= 32) || (0 >= time.tm_mon) ||\
            (time.tm_mon >= 13) || (0 > clock_start ) || (clock_start  >= 2) || (0 > time.tm_year ) || \
            (time.tm_year >= 100))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }
    else
    {
        ssp_err_t ssp_err = g_rtc.p_api->calendarTimeSet(g_rtc.p_ctrl,&time,clock_start);
        ssp_error_code_to_string(ssp_err);
    }
}

/*******************************************************************************************************************//**
 * @brief  Get the calendar time
 *
 *  @retval VOID
 ***********************************************************************************************************************/

void rtc_hal_calendarTimeGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    rtc_time_t time = {0};
    char   result[32];
    ssp_err_t ssp_err = g_rtc.p_api->calendarTimeGet(g_rtc.p_ctrl,&time);
    ssp_error_code_to_string(ssp_err);
    if(SSP_SUCCESS == ssp_err )
    {
        sprintf (result,"%d hr :%d min :%d sec %d/%d/%d\r\n",
                 time.tm_hour, time.tm_min, time.tm_sec, time.tm_mday, time.tm_mon, time.tm_year);
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*) result, TX_WAIT_FOREVER);
    }
}

/*******************************************************************************************************************//**
 * @brief  Set the calendar alarm time
 *  The calendar counter must be running before the alarm can be set.
 *
 *  @example calendarAlarmSet sec# min# hour#
 *
 *  @retval VOID
 ***********************************************************************************************************************/

void rtc_hal_calendarAlarmSet(sf_console_cb_args_t * p_args)
{
    rtc_alarm_time_t alarm  = {.time = {0}};
    bool irq_enable_flag = 1;

    /** Enabling second match, minute match, hour match and interrupt*/
    alarm.sec_match = 1;
    alarm.min_match = 1;
    alarm.hour_match = 1;


    /** Extract the Alarm time parameters from console*/

    if(!get_arg_as_uint("sec", p_args->p_remaining_string, (uint32_t*)&(alarm.time.tm_sec),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("min", p_args->p_remaining_string, (uint32_t*)&(alarm.time.tm_min),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("hour", p_args->p_remaining_string, (uint32_t*)&(alarm.time.tm_hour),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validate the Alarm time parameters extracted from console*/
    if((0 > alarm.time.tm_sec) || (alarm.time.tm_sec >= 60) || (0 > alarm.time.tm_min) ||  (alarm.time.tm_min >= 60) || (0 > alarm.time.tm_hour)\
            || (alarm.time.tm_hour >= 24) )

    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }
    else
    {
        ssp_err_t ssp_err = g_rtc.p_api->calendarAlarmSet(g_rtc.p_ctrl,&alarm,irq_enable_flag);
        ssp_error_code_to_string(ssp_err);
    }
}

/*******************************************************************************************************************//**
 * @brief  Get the calendar alarm time
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void rtc_hal_calendarAlarmGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    rtc_alarm_time_t alarm = {.time = {0}};
    char   result[32];
    ssp_err_t ssp_err = g_rtc.p_api->calendarAlarmGet(g_rtc.p_ctrl,&alarm);
    ssp_error_code_to_string(ssp_err);
    /** Print the Alarm and matching fields details only if calendarAlarmGet() results success*/
    if(SSP_SUCCESS == ssp_err )
    {
        sprintf (result,"%dhr :%dmin :%dsec \r\n",
                 alarm.time.tm_hour, alarm.time.tm_min, alarm.time.tm_sec);
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*) result, TX_WAIT_FOREVER);
    }
}

/*******************************************************************************************************************//**
 * @brief  Start the calendar counter
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void rtc_hal_calendarCounterStart(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_rtc.p_api->calendarCounterStart(g_rtc.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief  Stop the calendar counter
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void rtc_hal_calendarCounterStop(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_rtc.p_api->calendarCounterStop(g_rtc.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief  Enable the alarm or periodic or carry interrupt
 *
 * @example irqEnable irq#
 *  if irq is 0 then Real Time Clock ALARM IRQ is enabled
 *  if irq is 1 then Real Time Clock PERIODIC IRQ is enabled
 *  if irq is 2 then Real Time Clock CARRY IRQ is enabled
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void rtc_hal_irqEnable(sf_console_cb_args_t * p_args)
{
    rtc_event_t irq;

    if(!get_arg_as_uint("irq", p_args->p_remaining_string, (uint32_t*)&irq,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validate range of the irq before calling irqEnable() */
    if((irq == 0) || (irq == 1) || (irq == 2))
    {
        ssp_err_t ssp_err = g_rtc.p_api->irqEnable(g_rtc.p_ctrl,irq);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }

}

/*******************************************************************************************************************//**
 * @brief Disable the alarm or periodic or carry irq
 *
 *  @example irqDisable irq#
 *  if irq is 0 then Real Time Clock ALARM IRQ is disabled
 *  if irq is 1 then Real Time Clock PERIODIC IRQ is disabled
 *  if irq is 2 then Real Time Clock CARRY IRQ is disabled
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void rtc_hal_irqDisable(sf_console_cb_args_t * p_args)
{
    rtc_event_t irq;

    if(!get_arg_as_uint("irq", p_args->p_remaining_string, (uint32_t*)&irq,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validate range of the irq before calling irqDisable() */
    if((irq == 0) || (irq == 1) || (irq == 2))
    {
        ssp_err_t ssp_err = g_rtc.p_api->irqDisable(g_rtc.p_ctrl,irq);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }
}

/*******************************************************************************************************************//**
 * @brief  Set the periodic irq rate
 *
 *  @example periodicIrqRateSet rate#
 *  if rate = 6 then periodic irq is generated for every 1/256 second
 *  if rate = 7 then periodic irq is generated for every 1/128 second
 *  if rate = 8 then periodic irq is generated for every 1/64 second
 *  if rate = 9 then periodic irq is generated for every 1/32 second
 *  if rate = 10 then periodic irq is generated for every 1/16 second
 *  if rate = 11 then periodic irq is generated for every 1/8 second
 *  if rate = 12 then periodic irq is generated for every 1/4 second
 *  if rate = 13 then periodic irq is generated for every 1/2 second
 *  if rate = 14 then periodic irq is generated for every 1 second
 *  if rate = 15 then periodic irq is generated for every 2 second
 *  @retval VOID
 ***********************************************************************************************************************/

void rtc_hal_periodicIrqRateSet(sf_console_cb_args_t * p_args)
{
    rtc_periodic_irq_select_t rate;

    if(!get_arg_as_uint("rate", p_args->p_remaining_string, (uint32_t*)&rate,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validate range of the rate before calling periodicIrqRateSet() */
    if((rate >= 6) && (rate <= 15))
    {
        ssp_err_t ssp_err = g_rtc.p_api->periodicIrqRateSet(g_rtc.p_ctrl,rate);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }
}


/*******************************************************************************************************************//**
 * @brief  Get driver version based on compile time macros
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void rtc_hal_versionGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    uint8_t   version[32];
    ssp_version_t version_details;
    ssp_err = g_rtc.p_api->versionGet(&version_details);
    ssp_error_code_to_string(ssp_err);
    /** Print the version value only if versionGet() results success*/
    if(SSP_SUCCESS == ssp_err )
    {
        sprintf ((char *)version,"Code version  =  %d.%d\r\n", version_details.code_version_major,
                 version_details.code_version_minor);
        g_sf_console.p_api->write(g_sf_console.p_ctrl, version, TX_WAIT_FOREVER);

        sprintf ((char *)version,"\r\nAPI version  =  %d.%d\r\n", version_details.api_version_major,
                version_details.api_version_minor);
        g_sf_console.p_api->write(g_sf_console.p_ctrl, version, TX_WAIT_FOREVER);
    }
}

/*******************************************************************************************************************//**
 * @brief  Gets information about the driver including the source clock
 *
 *  @retval VOID
 ***********************************************************************************************************************/


void rtc_hal_infoGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    rtc_info_t rtc_info;
    ssp_err_t ssp_err = g_rtc.p_api->infoGet(g_rtc.p_ctrl,&rtc_info);
    ssp_error_code_to_string(ssp_err);
    /** Print the Clock Source only if infoGet() results success*/
    if(SSP_SUCCESS == ssp_err )
    {
        if(rtc_info.clock_source == RTC_CLOCK_SOURCE_SUBCLK)
        {
            g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"SUBCLK is used\r\n", TX_WAIT_FOREVER);
        }
        else
        {
            g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"LOCO is used\r\n", TX_WAIT_FOREVER);
        }
    }
}

/*******************************************************************************************************************//**
 * @brief  function is called when interrupt is generated
 *
 *  @retval VOID
 ***********************************************************************************************************************/

void rtc_callback(rtc_callback_args_t * p_args)
{
    ioport_level_t level;

    if(p_args->event == RTC_EVENT_PERIODIC_IRQ)
    {
        g_ioport.p_api->pinRead(leds.p_leds[0], &level);  // Read current level
        g_ioport.p_api->pinWrite(leds.p_leds[0], !level); // Write opposite level
    }
    else if(p_args->event == RTC_EVENT_ALARM_IRQ)
    {
        g_ioport.p_api->pinRead(leds.p_leds[2], &level);  // Read current level
        g_ioport.p_api->pinWrite(leds.p_leds[2], !level); // Write opposite level
    }
}


