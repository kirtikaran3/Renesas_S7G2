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
* File Name    : dev_example_common.h
* Description  : TODO
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           TODO       1.00    Initial Release.
***********************************************************************************************************************/
#ifndef DEV_EXAMPLE_COMMON_H_
#define DEV_EXAMPLE_COMMON_H_

#include "dev_example_thread.h"

#define UNUSED(x) (void)(x)

#define     HEX     16
#define     BIN     2
#define     DEC     10
#define     OCT     8

void ssp_error_code_to_string(ssp_err_t error_code);

bool get_arg_as_uint(char* arg_name, const uint8_t *console_string,
                                            uint32_t* p_value, uint8_t base);

void format_and_print(uint32_t address, uint8_t* buffer, uint32_t size);

/* Providing prototype to remove the warning */
static inline void print_to_console(char* msg)
{
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)msg, TX_WAIT_FOREVER);
}


#endif /* DEV_EXAMPLE_COMMON_H_ */
