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
 * File Name    : sf_touch_panel_demo_thread_entry.c
 * Description  : Touch Thread entry funtion.It will print the touch data on console screen from the message queue.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           TODO  1.00    Initial Release
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
 ***********************************************************************************************************************/
#include "sf_touch_panel_demo_thread.h"
#include <stdint.h>
#include "dev_example_common.h"
#include "dev_example_thread.h"


/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
void sf_touch_thread_entry(void);

/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/
sf_message_header_t         *p_message = NULL;
char                        temp_buff[64];

char *touch_event[]=
{
 "SF_TOUCH_PANEL_EVENT_INVALID",
 "SF_TOUCH_PANEL_EVENT_HOLD",
 "SF_TOUCH_PANEL_EVENT_MOVE",
 "SF_TOUCH_PANEL_EVENT_DOWN",
 "SF_TOUCH_PANEL_EVENT_UP",
 "SF_TOUCH_PANEL_EVENT_NONE"
};

extern TX_QUEUE sf_touch_panel_demo_thread_message_queue;

/***********************************************************************************************************************
 *  @brief      Touch Thread entry funtion
 *
 *  @retval     VOID
 **********************************************************************************************************************/
void sf_touch_panel_demo_thread_entry(void)
{

    g_sf_touch_panel_i2c.p_api->stop(g_sf_touch_panel_i2c.p_ctrl);

    while (1)
    {

        /** Wait for an update message on the system queue.  Process events as follows. */
        ssp_err_t ssp_err = g_sf_message.p_api->pend(g_sf_message.p_ctrl,
                                                     &sf_touch_panel_demo_thread_message_queue,
                                                     (sf_message_header_t **) &p_message,
                                                     TX_WAIT_FOREVER);
        if(ssp_err == SSP_SUCCESS)
        {
            switch(p_message->event_b.class)
            {
                case SF_MESSAGE_EVENT_CLASS_TOUCH:
                {
                    sf_touch_panel_payload_t *p_touch_payload = (sf_touch_panel_payload_t *)p_message;

                    sprintf(temp_buff,"x: %d, y: %d, event: %s\n\r",
                            p_touch_payload->x,
                            p_touch_payload->y,
                            touch_event[p_touch_payload->event_type]);

                    /** printing X,Y Value with touch event type to the console screen */
                    print_to_console(temp_buff);

                }
                break;
            }
            g_sf_message.p_api->bufferRelease(g_sf_message.p_ctrl,
                                              (sf_message_header_t *)p_message,
                                              SF_MESSAGE_RELEASE_OPTION_ACK);
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }
    }
}
