/*
 * audio_player_events.c
 *
 *  Created on: 7 Dec 2015
 *      Author: karol.saja
 */

/***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************/
#include "application_define.h"

#include "gx_studio_project_resources.h"
#include "gx_studio_project_specifications.h"

/***********************************************************************************************************************
 * Private function prototypes
 ***********************************************************************************************************************/
static UINT app_gx_goto_window(char *name, GX_WIDGET * p_current, GX_WIDGET * p_new, bool destroy);

extern GX_WINDOW_ROOT *p_window_root;

static USHORT adc_reading;
static USHORT click_count = 10;

/** Main Screen Event Handler */
UINT w_main_event(GX_WINDOW * widget, GX_EVENT * event_ptr)
{
    UINT status;

    switch (event_ptr->gx_event_type)
    {
        case GX_EVENT_SHOW:
        {
            status = gx_progress_bar_pixelmap_set(&w_main.w_main_bar_temp, GX_PIXELMAP_ID_BOX_RED);
            APP_ERROR_TRAP(status)

            status = gx_progress_bar_pixelmap_set(&w_main.w_main_bar_buttons, GX_PIXELMAP_ID_BOX_YEL);
            APP_ERROR_TRAP(status)

            status = g_temp_adc.p_api->open(g_temp_adc.p_ctrl, g_temp_adc.p_cfg);
            APP_ERROR_TRAP(status)

            status = g_temp_adc.p_api->scanCfg(g_temp_adc.p_ctrl, g_temp_adc.p_channel_cfg);
            APP_ERROR_TRAP(status)

            status = g_temp_adc.p_api->scanStart(g_temp_adc.p_ctrl);
            APP_ERROR_TRAP(status)

            status = gx_system_timer_start(widget, APP_ADC_TIMER_ID, APP_ADC_TIMER_COUNT, APP_ADC_TIMER_COUNT);
            APP_ERROR_TRAP(status)

            return gx_window_event_process(widget, event_ptr);
        }

        case APP_EVENT_SW4_PRESS:
        {
            if(20 > click_count)
            {
                click_count++;
                status = gx_progress_bar_value_set(&w_main.w_main_bar_buttons, (INT) click_count);
                APP_ERROR_TRAP(status)
            }

            return gx_window_event_process(widget, event_ptr);
        }

        case APP_EVENT_SW5_PRESS:
        {
            if(0 < click_count)
            {
                click_count--;
                status = gx_progress_bar_value_set(&w_main.w_main_bar_buttons, (INT) click_count);
                APP_ERROR_TRAP(status)
            }

            return gx_window_event_process(widget, event_ptr);
        }

        case GX_EVENT_TIMER:
        {
            if(APP_ADC_TIMER_ID == event_ptr->gx_event_payload.gx_event_timer_id)
            {
                status = g_temp_adc.p_api->read(g_temp_adc.p_ctrl, ADC_REG_TEMPERATURE, &adc_reading);
                APP_ERROR_TRAP(status)

                adc_reading = (USHORT) (APP_NORMALIZE_TEMP(adc_reading) * 10);

                status = gx_progress_bar_value_set(&w_main.w_main_bar_temp, adc_reading);
                APP_ERROR_TRAP(status)
            }

            return gx_window_event_process(widget, event_ptr);
        }

        default:
            return gx_window_event_process(widget, event_ptr);
    }

    return 0;
}

/***********************************************************************************************************************
Functions
***********************************************************************************************************************/

static UINT app_gx_goto_window(char *name, GX_WIDGET * p_current, GX_WIDGET * p_new, bool destroy)
{
    UINT status;

    status = gx_studio_named_widget_create(name, GX_NULL, GX_NULL);
    APP_ERROR_RETURN(status)

    if(destroy)
    {
        status = gx_widget_delete(p_current);
        APP_ERROR_RETURN(status)
    }

    if (p_window_root->gx_widget_first_child == GX_NULL)
    {
        status = gx_widget_attach(p_window_root, p_new);
        APP_ERROR_RETURN(status)
    }

    return GX_SUCCESS;
}
