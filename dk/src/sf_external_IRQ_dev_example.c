
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
 * File Name    : sf_external.IRQ_dev_example.c
 * Description  : This file is used to exercise the APIs of External IRQ Framework
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
#define            EXTERNAL_IRQ_OPEN          "open"
#define            EXTERNAL_IRQ_WAIT          "wait"
#define            EXTERNAL_IRQ_CLOSE         "close"
#define            EXTERNAL_IRQ_VERSIONGET    "versionGet"


/**  HELP MENU   */

#define            HELP_EXTERNAL_IRQ_OPEN           "Open the External IRQ instance"
#define            HELP_EXTERNAL_IRQ_WAIT           "Wait for IRQ till timeout i.e wait till switch2 is pressed"
#define            HELP_EXTERNAL_IRQ_CLOSE          "Close the External IRQ instance"
#define            HELP_EXTERNAL_IRQ_VERSIONGET     "Gets the Version of the API"



/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
void sf_irq_open(sf_console_cb_args_t *p_args);
void sf_irq_wait(sf_console_cb_args_t *p_args);
void sf_irq_close(sf_console_cb_args_t *p_args);
void sf_irq_versionGet(sf_console_cb_args_t *p_args);


/* Global variables */
/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/

static uint32_t open_flag = 0;

/* External IRQ Framework Commands Declaration  */
const sf_console_command_t g_sf_external_IRQ_commands[] =
{
 {
    .command   =  (uint8_t*)EXTERNAL_IRQ_OPEN,
    .help      =  (uint8_t*)HELP_EXTERNAL_IRQ_OPEN,
    .callback  =  sf_irq_open,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)EXTERNAL_IRQ_WAIT,
    .help      =  (uint8_t*)HELP_EXTERNAL_IRQ_WAIT,
    .callback  =  sf_irq_wait,
    .context   =  NULL
 },
 {
     .command   =  (uint8_t*)EXTERNAL_IRQ_CLOSE,
     .help      =  (uint8_t*)HELP_EXTERNAL_IRQ_CLOSE,
     .callback  =  sf_irq_close,
     .context   =  NULL
 },
 {
    .command   =  (uint8_t*)EXTERNAL_IRQ_VERSIONGET,
    .help      =  (uint8_t*)HELP_EXTERNAL_IRQ_VERSIONGET,
    .callback  =  sf_irq_versionGet,
    .context   =  NULL
 }
};//end//



sf_console_menu_t g_sf_external_IRQ_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .num_commands = sizeof(g_sf_external_IRQ_commands)/sizeof(g_sf_external_IRQ_commands[0]),
 .menu_name = (uint8_t*)"sf_external_irq",
 .command_list = g_sf_external_IRQ_commands
};

extern bsp_leds_t ledpresent;
/***********************************************************************************************************************
Function
 ***********************************************************************************************************************/


/**********************************************************************************************************************
 * @brief Initialize the external Irq instance
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void sf_irq_open(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    ssp_err = g_sf_external_irq.p_api->open(g_sf_external_irq.p_ctrl,g_sf_external_irq.p_cfg);
    ssp_error_code_to_string(ssp_err);
    /** Setting open_flag is driver is opened successfully*/
    if(SSP_SUCCESS == ssp_err )
    {
        open_flag = 1;
    }
}


/**********************************************************************************************************************
 * @brief wait for user to press switch(switch 2 in this example)
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void sf_irq_wait(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    g_ioport.p_api->pinWrite(ledpresent.p_leds[0], IOPORT_LEVEL_HIGH);
    if(open_flag)
    {
        print_to_console("Press switch2 to exit from wait\r\n");
    }
    ssp_err = g_sf_external_irq.p_api->wait(g_sf_external_irq.p_ctrl,TX_WAIT_FOREVER);
    ssp_error_code_to_string(ssp_err);
    g_ioport.p_api->pinWrite(ledpresent.p_leds[0], IOPORT_LEVEL_LOW);

}


/**********************************************************************************************************************
 * @brief close the external Irq instance
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void sf_irq_close(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    ssp_err = g_sf_external_irq.p_api->close(g_sf_external_irq.p_ctrl);
    ssp_error_code_to_string(ssp_err);
    /** Setting open_flag is driver is opened successfully*/
    if(SSP_SUCCESS == ssp_err )
    {
        open_flag = 0;
    }
}


/**********************************************************************************************************************
 * @brief Gets the Version of the API
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void sf_irq_versionGet(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    char   version[32];
    ssp_version_t version_details;

    ssp_err = g_sf_external_irq.p_api->versionGet(&version_details);
    ssp_error_code_to_string(ssp_err);

    /** Print the version value only if versionGet() results success*/
    if(SSP_SUCCESS == ssp_err )
    {
        sprintf (version,"Code version  =  %d.%d\r\n", version_details.code_version_major,
                    version_details.code_version_minor);
        print_to_console(version);

        sprintf (version,"\r\nAPI version  =  %d.%d\r\n", version_details.api_version_major,
                    version_details.api_version_minor);
        print_to_console(version);
    }

}

