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
 * File Name    : r_adc_commands.c
 * Description  : The ADC interface provides standard ADC functionality including one-shot mode (single scan),
continuous scan and group scan. It also allows configuration of hardware and software triggers for starting scans.
After each conversion an interrupt can be triggered, and if a callback function is provided, the call back is invoked
with the appropriate event information..
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
#include <inttypes.h>
#include "dev_example_thread.h"
#include "dev_example_common.h"

/***********************************************************************************************************************
Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
/* Prototypes for the callback functions called by ADC console commands */


void adc_hal_open(sf_console_cb_args_t * p_args);
void adc_hal_read(sf_console_cb_args_t * p_args);
void adc_hal_scanCfg(sf_console_cb_args_t * p_args);
void adc_hal_sampleStateCountSet(sf_console_cb_args_t * p_args);
void adc_hal_scanStart(sf_console_cb_args_t * p_args);
void adc_hal_scanStop(sf_console_cb_args_t * p_args);
void adc_hal_infoget(sf_console_cb_args_t * p_args);
void adc_hal_close(sf_console_cb_args_t * p_args);
void adc_hal_versionGet(sf_console_cb_args_t * p_args);
void adc_hal_scanStatusGet(sf_console_cb_args_t * p_args);

/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/


/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/
static bsp_leds_t leds;

const sf_console_command_t g_adc_hal_commands[] =
{
 { (uint8_t*)"open",
   (uint8_t*)"Initilize adc unit",
   adc_hal_open,
   NULL
 },
 { (uint8_t*)"scanCfg",
   (uint8_t*)"Configure the scan including channel and groups",
   adc_hal_scanCfg,
   NULL
 },
 { (uint8_t*)"scanStart",
   (uint8_t*)"Start the scan or enable the hardware trigger ",
   adc_hal_scanStart,
   NULL
 },
 { (uint8_t*)"scanStop",
   (uint8_t*)"Stop the ADC scan or disable the hardware trigger ",
   adc_hal_scanStop,
   NULL
 },
 { (uint8_t*)"scanStatusGet",
   (uint8_t*)"Check scan status.",
   adc_hal_scanStatusGet,
   NULL
 },
 { (uint8_t*)"read",
   (uint8_t*)"Read ADC conversion result(s). ",
   adc_hal_read,
   NULL
 },
 { (uint8_t*)"infoGet",
   (uint8_t*)"Read ADC information for configured channel ",
   adc_hal_infoget,
   NULL
 },
 { (uint8_t*)"sampleStateCountSet",
   (uint8_t*)"Set sample state count\r\n\tSynopsis:- sampleStateCountSet reg_id# num_states#"
   "\r\n\tExample:- sampleStateCountSet reg_id2 num_states7",
   adc_hal_sampleStateCountSet,
   NULL
 },
 { (uint8_t*)"versionGet",
   (uint8_t*)"Gets the version of API",
   adc_hal_versionGet,
   NULL
 },
 { (uint8_t*)"close",
   (uint8_t*)"Close the specified ADC unit",
   adc_hal_close,
   NULL
 },

};

/** Previous menu */
extern const sf_console_menu_t g_sf_console_root_menu;


/** adc menu */
const sf_console_menu_t g_adc_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"r_adc",
 .num_commands = (sizeof(g_adc_hal_commands)) / (sizeof(g_adc_hal_commands[0])),
 .command_list = g_adc_hal_commands

};

/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief  adc_hal_open calls the adc instance open api callback as per the configuration.
 *
 * @retval  void
 ***********************************************************************************************************************/

void adc_hal_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_adc.p_api->open(g_adc.p_ctrl,g_adc.p_cfg);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  adc_hal_scanCfg calls the adc instance scancfg api callback as per the configuration.
 * @retval  void

 ***********************************************************************************************************************/

void adc_hal_scanCfg(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_adc.p_api->scanCfg(g_adc.p_ctrl,g_adc.p_channel_cfg);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  adc_hal_scanStart calls the adc instance start api callback i.e to trigger start of convertion.
 * @retval  void

 ***********************************************************************************************************************/

void adc_hal_scanStart(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_adc.p_api->scanStart(g_adc.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  adc_hal_scanStop calls the adc instance stop api callback i.e to stop scan condition.
 * @retval  void

 ***********************************************************************************************************************/

void adc_hal_scanStop(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_adc.p_api->scanStop(g_adc.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  adc_hal_scanStatusGet calls the adc instance scanStatusGet api callback.
 * @retval  void
 *
 ***********************************************************************************************************************/

void adc_hal_scanStatusGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_adc.p_api->scanStatusGet(g_adc.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief  Read ADC conversion result(s)
 * @retval  void
 * @note    adc_hal_read calls the adc instance read api callback which reads the adc data and prints on the console.
 ***********************************************************************************************************************/

void adc_hal_read(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    char data_string[10] = {0};
    uint16_t data;
    ssp_err_t ssp_err = g_adc.p_api->read(g_adc.p_ctrl, ADC_REG_CHANNEL_0, &data);
    ssp_error_code_to_string(ssp_err);
    sprintf(data_string,"%"PRIu16"\r\n",data);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)data_string, TX_WAIT_FOREVER);

}

/*******************************************************************************************************************//**
 * @brief   adc_hal_infoget calls R_ADC_InfoGet() to get specific details of the specified channels.
 * @retval  void
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *
 * @note    adc_hal_infoget calls the adc instance infoGet api callback
 *          prints the address of the lowest number configured channel and the total number of bytes.
 ***********************************************************************************************************************/

void adc_hal_infoget(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    char   info_get[32];
    adc_info_t adc_info;
    ssp_err_t ssp_err = g_adc.p_api->infoGet(g_adc.p_ctrl,&adc_info);
    if(ssp_err == SSP_SUCCESS)
    {
        sprintf (info_get,"adc_info.p_address =  %d\r\n",adc_info.p_address[0]);
        g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t*)info_get,
                                  TX_WAIT_FOREVER);
        sprintf (info_get,"adc_info.length =  %d\r\n",(int)adc_info.length);
        g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t*)info_get,
                                  TX_WAIT_FOREVER);
        if(adc_info.elc_peripheral == 8)
        {
            g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                      (uint8_t*)"adc_info.elc_peripheral = ELC_PERIPHERAL_ADC0 \r\n",
                                      TX_WAIT_FOREVER);
        }
        if(adc_info.elc_event == 75)
        {
            g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                      (uint8_t*)"adc_info.elc_event = ELC_EVENT_ADC0_SCAN_END\r\n",
                                      TX_WAIT_FOREVER);
        }

    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"adc_info_Get ", TX_WAIT_FOREVER);
        ssp_error_code_to_string(ssp_err);
    }
}

/*******************************************************************************************************************//**
 * @brief  Set sample state count for the specified channels.
 * @retval  void
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *
 * @note    adc_hal_sampleStateCountSet calls the adc instance sampleStateCountSet api callback
 *           which configures the sample count with respect to the channels.
 ***********************************************************************************************************************/

void adc_hal_sampleStateCountSet(sf_console_cb_args_t * p_args)
{
    adc_sample_state_t sample;
    int32_t   ref_reg_id;
    int32_t   ref_num_states;

    if(!get_arg_as_uint("reg_id", p_args->p_remaining_string, (uint32_t*)&ref_reg_id,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("num_states", p_args->p_remaining_string, (uint32_t*)&ref_num_states,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validating adc_sample_state_t  */
    if(((ref_num_states >= 5) && (ref_num_states <= 255)) &&
            ((ref_reg_id >= 0) && (ref_reg_id <= 18)))
    {
        sample.reg_id = ref_reg_id;
        sample.num_states = (uint8_t)ref_num_states;
        /** calls sampleStateCountSet API */
        ssp_err_t ssp_err = g_adc.p_api->sampleStateCountSet(g_adc.p_ctrl, &sample);
        if(ssp_err == SSP_SUCCESS)
            ssp_error_code_to_string(ssp_err);

    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }
}

/*******************************************************************************************************************//**
 * @brief Get the version of Adc APIs
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval  void
 ***********************************************************************************************************************/

void adc_hal_versionGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    char   version[32];
    ssp_version_t version_details;
    ssp_err_t ssp_err = g_adc.p_api->versionGet(&version_details);
    if(ssp_err == SSP_SUCCESS)
    {
        sprintf (version,"Code version  =  %d.%d\r\n",
                 version_details.code_version_major,
                 version_details.code_version_minor);

        g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t*)version,
                                  TX_WAIT_FOREVER);

        sprintf (version,"\r\nAPI version  =  %d.%d\r\n",
                version_details.api_version_major,
                version_details.api_version_minor);

        g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t*)version,
                                  TX_WAIT_FOREVER);
    }
    else
    {
        ssp_error_code_to_string(ssp_err);
    }

}

/*******************************************************************************************************************//**
 * @brief  De-initialize adc APIs
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval  void
 ***********************************************************************************************************************/

void adc_hal_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_adc.p_api->close(g_adc.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief  Callback function.

 ***********************************************************************************************************************/

void Adc_callback(adc_callback_args_t * p_args)
{
    UNUSED(p_args);
}

