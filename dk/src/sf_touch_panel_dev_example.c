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
 * File Name    : sf_touch_panel_dev_example.c
 * Description  : This module is a ThreadX-aware Touch Panel Framework which scans for touch events and posts them to
 *                the Messaging Framework for distribution to touch event subscribers.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           TODO  1.00    Initial Release
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * @file
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
 ***********************************************************************************************************************/
#include <stdint.h>
#include "dev_example_common.h"
#include "dev_example_thread.h"
#include "sf_touch_panel_demo_thread.h"


/***********************************************************************************************************************
Macro definitions
 ***********************************************************************************************************************/
/** touch commands */
#define         SF_TOUCH_START_CMD      "start"
#define         SF_TOUCH_STOP_CMD       "stop"

/** touch command help menu */
#define         SF_TOUCH_START_HELP     "Start scanning for touch events."
#define         SF_TOUCH_STOP_HELP      "Stop the scanning"

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/

void sf_touch_start     (sf_console_cb_args_t * p_args);
void sf_touch_stop      (sf_console_cb_args_t * p_args);


/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/
sf_message_header_t         *p_payload=NULL;
sf_message_post_cfg_t       *p_post_cfg=NULL;
uint8_t                     retv=0;


/** SF_TOUCH console menu commands **/
sf_console_command_t sf_touch_commands[]=
{
 {
  .command  =   (uint8_t*)SF_TOUCH_START_CMD,
  .help     =   (uint8_t*)SF_TOUCH_START_HELP,
  .callback =   sf_touch_start,
  .context  =   NULL
 },
 {
  .command  =   (uint8_t*)SF_TOUCH_STOP_CMD,
  .help     =   (uint8_t*)SF_TOUCH_STOP_HELP,
  .callback =   sf_touch_stop,
  .context  =   NULL
 },
};



/** sf_touch  menu */
const sf_console_menu_t g_sf_touch_menu =
{
 .menu_prev      =  &g_sf_console_root_menu,
 .menu_name      =  (uint8_t*)"sf_touch_panel_i2c",
 .num_commands   =  (sizeof(sf_touch_commands)) / (sizeof(sf_touch_commands[0])),
 .command_list   =  sf_touch_commands
};


/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 *  @brief      Start scanning for touch events.
 *  @example    'start' in command prompt.
 *  @retval     VOID
 **********************************************************************************************************************/
void sf_touch_start(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    /** start the i2c touch panel fw */
    ssp_err = g_sf_touch_panel_i2c.p_api->start(g_sf_touch_panel_i2c.p_ctrl);
    print_to_console("touch panel start --");
    ssp_error_code_to_string(ssp_err);
}

/***********************************************************************************************************************
 *  @brief      stop scanning for touch events.
 *  @example    'stop' in command prompt.
 *  @retval     VOID
 **********************************************************************************************************************/
void sf_touch_stop(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    /** stop the i2c touch framework*/
    ssp_err_t ssp_err = g_sf_touch_panel_i2c.p_api->stop(g_sf_touch_panel_i2c.p_ctrl);
    print_to_console("touch panel stop --");
    ssp_error_code_to_string(ssp_err);
}
