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
 * File Name    : r_external_IRQ_commands.c
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
#include "dev_example_thread.h"
#include "dev_example_common.h"
/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
/* Prototypes for the callback functions called by I2C console commands */

void extirq_hal_open(sf_console_cb_args_t * p_args);
void extirq_hal_close(sf_console_cb_args_t * p_args);
void extirq_hal_enable(sf_console_cb_args_t * p_args);
void extirq_hal_disable(sf_console_cb_args_t * p_args);
void extirq_hal_triggerSet(sf_console_cb_args_t * p_args);
void extirq_hal_filterEnable(sf_console_cb_args_t * p_args);
void extirq_hal_filterDisable(sf_console_cb_args_t * p_args);
void extirq_hal_versionget(sf_console_cb_args_t * p_args);

const sf_console_command_t g_extirq_hal_commands[] =
{
 {
  (uint8_t*)"open",
  (uint8_t*)"Initial configuration",
  extirq_hal_open,
  NULL
 },
 {
  (uint8_t*)"close",
  (uint8_t*)"Close the ExtIRQ",
  extirq_hal_close,
  NULL
 },
 {
  (uint8_t*)"enable",
  (uint8_t*)"Enable ExtIRQ ",
  extirq_hal_enable,
  NULL
 },
 {
  (uint8_t*)"disable",
  (uint8_t*)"Disable ExtIRQ",
  extirq_hal_disable,
  NULL
 },
 {
  (uint8_t*)"triggerSet",
  (uint8_t*)"Set trigger level\r\n\tSynopsis:- triggerSet IRQ_TRIG# 3-LEVEL_LOW 2-BOTH EDGE "
  "1-FALLING 0-RISING\r\n\tExample:- triggerSet IRQ_TRIG1",
  extirq_hal_triggerSet,
  NULL
 },
 {
  (uint8_t*)"filterEnable",
  (uint8_t*)"External IRQ filter Enable",
  extirq_hal_filterEnable,
  NULL
 },
 {
  (uint8_t*)"filterDisable",
  (uint8_t*)"External IRQ filterDisable",
  extirq_hal_filterDisable,
  NULL
 },
 {
  (uint8_t*)"versionGet",
  (uint8_t*)"Gets the version of API",
  extirq_hal_versionget,
  NULL
 }

};


/** Ext IRQ HAL menu */
const sf_console_menu_t g_extirq_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t const *)"r_icu",
 .num_commands = (sizeof(g_extirq_hal_commands)) / (sizeof(g_extirq_hal_commands[0])),
 .command_list = g_extirq_hal_commands

};



/*******************************************************************************************************************//**
 * @brief   Configures ExtIRQ based on user configuration via configuration.xml.
 * @example 'open'
 * @pre    open should be called before using the ext irq hal module
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void extirq_hal_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    ssp_err = g_external_irq.p_api->open(g_external_irq.p_ctrl,g_external_irq.p_cfg);
    ssp_error_code_to_string(ssp_err);

}
/*******************************************************************************************************************//**
 * @brief
 * @example
 * @pre    open should be called before closing the ext irq module
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void extirq_hal_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    ssp_err = g_external_irq.p_api->close(g_external_irq.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}
/*******************************************************************************************************************//**
 * @brief   Enable the IRQ callback when external IRQ occurs
 * @example 'enable'
 * @pre    open should be called before using the enable command
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void extirq_hal_enable(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    ssp_err = g_external_irq.p_api->enable(g_external_irq.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}
/*******************************************************************************************************************//**
 * @brief   Disable the IRQ callback when external IRQ occurs.
 * @example 'disable'
 * @pre    open should be called before using the disable command
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void extirq_hal_disable(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    ssp_err = g_external_irq.p_api->disable(g_external_irq.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}
/*******************************************************************************************************************//**
 * @brief   Set trigger for the PIN level
 * @example 'triggerSet IRQ_TRIG# ( EXTERNAL_IRQ_TRIG_FALLING    = 0,  ///< Falling edge trigger
 *                                  EXTERNAL_IRQ_TRIG_RISING     = 1,  ///< Rising edge trigger
 *                                  EXTERNAL_IRQ_TRIG_BOTH_EDGE  = 2,  ///< Both edges trigger
 *                                  EXTERNAL_IRQ_TRIG_LEVEL_LOW  = 3,  ///< Low level trigger
 *                                 )
 * @pre open should be called before using the triggerSet command
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void extirq_hal_triggerSet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    int32_t trigger_set = EXTERNAL_IRQ_TRIG_FALLING ;

    if(!get_arg_as_uint("IRQ_TRIG", p_args->p_remaining_string, (uint32_t*)&trigger_set,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(trigger_set < 0 || trigger_set > 3)
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"Invalid Argument\r\n", TX_WAIT_FOREVER);
    }
    else
    {
        ssp_err = g_external_irq.p_api->triggerSet(g_external_irq.p_ctrl,(external_irq_trigger_t)trigger_set);
        ssp_error_code_to_string(ssp_err);
    }
}
/*******************************************************************************************************************//**
 * @brief   Enables noise filter
 * @example 'filterEnable'
 * @pre     open should be called before using the filterEnable command
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void extirq_hal_filterEnable(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    ssp_err = g_external_irq.p_api->filterEnable(g_external_irq.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}
/*******************************************************************************************************************//**
 * @brief   Disable noise filter
 * @example 'filterDisable'
 * @pre      open should be called before using the filterDisable command
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void extirq_hal_filterDisable(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    ssp_err = g_external_irq.p_api->filterDisable(g_external_irq.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}
/*******************************************************************************************************************//**
 * @brief  Get the version of the Driver APIs
 * @example versionget
 * @pre
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void extirq_hal_versionget(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_version_t version_details;
    char   version[32];

    ssp_err_t ssp_err = g_external_irq.p_api->versionGet(&version_details);
    ssp_error_code_to_string(ssp_err);

    sprintf (version,"Code version  =  %d.%d\r\n", version_details.code_version_major,
            version_details.code_version_minor);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)version, TX_WAIT_FOREVER);

    sprintf (version,"\r\nAPI version  =  %d.%d\r\n", version_details.api_version_major,
            version_details.api_version_minor);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)version, TX_WAIT_FOREVER);

}

/*******************************************************************************************************************//**
 * @brief   The usercallback when the S1 switch is pressed(configured as external IRQ11)
 * @example
 * @pre    open and enable command has to be exercised to enable the irq callback
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void switch1_press_IRQ(external_irq_callback_args_t * p_args)
{

    UNUSED(p_args);
    bsp_leds_t leds;
    static ioport_level_t level = IOPORT_LEVEL_LOW;
    /*Get LED list from BSP*/
    R_BSP_LedsGet(&leds);

    level = !level;

    /* On the LED*/
    g_ioport.p_api->pinWrite(leds.p_leds[0],level);

}

