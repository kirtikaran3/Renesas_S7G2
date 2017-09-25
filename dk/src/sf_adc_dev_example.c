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
* File Name    : sf_adc_dev_example.c
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
#include "sf_adc_demo_thread.h"
#include "dev_example_thread.h"
#include "dev_example_common.h"
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define I2C_INIT_BUFF_SIZE 2
/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/
/* Prototypes for the callback functions called by ADC console commands */


void sf_adc_open(sf_console_cb_args_t * p_args);
void sf_adc_read(sf_console_cb_args_t * p_args);
void sf_adc_scanStart(sf_console_cb_args_t * p_args);
void sf_adc_scanStop(sf_console_cb_args_t * p_args);
void sf_adc_close(sf_console_cb_args_t * p_args);
void sf_adc_versionGet(sf_console_cb_args_t * p_args);


/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/
extern TX_THREAD sf_adc_demo_thread;
ioport_level_t level;
static bsp_leds_t leds;

CHAR *name;
UINT state;
UINT priority;


const uint8_t i2c_init_buffer[I2C_INIT_BUFF_SIZE] = {0x06, 0x00};

const sf_console_command_t g_sf_adc_commands[] =
{
    {
      (uint8_t*)"open",
      (uint8_t*)"Initialize ADC periodic framework",
      sf_adc_open,
      NULL
    },

    {
      (uint8_t*)"scanStart",
      (uint8_t*)"Start the scan",
      sf_adc_scanStart,
      NULL
    },

    {
      (uint8_t*)"scanStop",
      (uint8_t*)"Stop the scan",
      sf_adc_scanStop,
      NULL
    },

    {
      (uint8_t*)"close",
      (uint8_t*)"Close the ADC periodic framework",
      sf_adc_close,
      NULL
    },

    {
      (uint8_t*)"versionGet",
      (uint8_t*)"Gets the version of API",
      sf_adc_versionGet,
      NULL
    },

};

/** Previous menu */
extern const sf_console_menu_t g_sf_console_root_menu;


/** adc menu */
const sf_console_menu_t g_sf_adc_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"sf_adc_periodic",
 .num_commands = (sizeof(g_sf_adc_commands)) / (sizeof(g_sf_adc_commands[0])),
 .command_list = g_sf_adc_commands

};

/***********************************************************************************************************************
Functions
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief  It initializes ADC and also I2C for a Pcall to verify application
 *
 *  @retval VOID
***********************************************************************************************************************/

void sf_adc_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    UINT tx_err;
    ssp_err_t ssp_err;

    /* verify adc_thread current state,to resuming adc_thread from suspend state to enable state */
    tx_err = tx_thread_info_get(&sf_adc_demo_thread, &name,&state,NULL,&priority,NULL,NULL, NULL,NULL);
    if(tx_err == TX_SUCCESS)
    {
        if((state == TX_SUSPENDED) ||(state == TX_SEMAPHORE_SUSP))
        {
            tx_thread_resume(&sf_adc_demo_thread);
        }
        else
        {
            g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                      (uint8_t*)"ADC thread not Suspended\r\n" , TX_WAIT_FOREVER);
        }
    }

    /* Opening the ADC Periodic Framework*/

    print_to_console("Opening ADC periodic framework\r\n");
    ssp_err = g_sf_adc_periodic.p_api->open(g_sf_adc_periodic.p_ctrl,g_sf_adc_periodic.p_cfg);
    ssp_error_code_to_string(ssp_err);

    /* Open sci_i2c to configure pcall and select register of led's on the target board */
    print_to_console("Open I2C HAL driver\r\n");
    ssp_err = g_i2c_sci.p_api->open(g_i2c_sci.p_ctrl,g_i2c_sci.p_cfg);
    ssp_error_code_to_string(ssp_err);

    print_to_console("Initialize PCAL\r\n");
    ssp_err = g_i2c_sci.p_api->write(g_i2c_sci.p_ctrl,(uint8_t*)i2c_init_buffer,I2C_INIT_BUFF_SIZE, false);
    ssp_error_code_to_string(ssp_err);

}


/*******************************************************************************************************************//**
 * @brief  start's the scan or enable the hardware trigger
 *
 *  @retval VOID
***********************************************************************************************************************/

void sf_adc_scanStart(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_sf_adc_periodic.p_api->start(g_sf_adc_periodic.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Stop the ADC scan or disable the hardware trigger
 *
 *  @retval VOID
***********************************************************************************************************************/

void sf_adc_scanStop(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_sf_adc_periodic.p_api->stop(g_sf_adc_periodic.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  @brief  De-initialize specified I2C and ADC FrameWork also Suspends and reset the adc_thread unit.
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval  void
***********************************************************************************************************************/

void sf_adc_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    /** calls SCI_I2C_CLOSE API */
    ssp_err_t ssp_err = g_i2c_sci.p_api->close(g_i2c_sci.p_ctrl);
    if(ssp_err == SSP_SUCCESS)
    {
        ssp_err = g_sf_adc_periodic.p_api->close(g_sf_adc_periodic.p_ctrl);
        ssp_error_code_to_string(ssp_err);
        if(ssp_err == SSP_SUCCESS)
        {
            ssp_err = tx_thread_info_get(&sf_adc_demo_thread, &name,&state,NULL,&priority,NULL,NULL, NULL,NULL);
            if((ssp_err == SSP_SUCCESS) || (state == TX_SEMAPHORE_SUSP))
            {
                tx_thread_suspend(&sf_adc_demo_thread);
            }
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }
    }
    else
    {
        ssp_error_code_to_string(ssp_err);
    }
}


/*******************************************************************************************************************//**
 * @brief Get the version of ADC FrameWork APIs
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
***********************************************************************************************************************/

void sf_adc_versionGet(sf_console_cb_args_t * p_args)
{
    char   version[32];
    ssp_version_t version_details;
    UNUSED(p_args);
    /** calls FRAMEWORK_ADC_VERSIONGET API */
    ssp_err_t ssp_err = g_sf_adc_periodic.p_api->versionGet(&version_details);
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
 * @brief  Callback function.

***********************************************************************************************************************/

void g_adc_framework_user_callback(sf_adc_periodic_callback_args_t * p_args)
{
    UNUSED(p_args);
    tx_semaphore_put(&adc_semaphore);
    R_BSP_LedsGet(&leds);
    g_ioport.p_api->pinRead(leds.p_leds[1], &level);  // Read current level
    g_ioport.p_api->pinWrite(leds.p_leds[1], !level);

}



