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
* File Name    : dev_example_utilities.c
* Description  : TODO
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           TODO       1.00    Initial Release.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include<stdbool.h>
#include<tx_api.h>

#include "dev_example_common.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define PROCESS_BUFF_SIZE 128

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/



/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/




/*******************************************************************************************************************//**
 * @brief  Parse the input data string and generate a hex byte array
 *
 *
 * @param[in] arg_name - Name of the argument to be extracted
 * @param[in] console_string   - string passed on by the console framework
 * @param[in] p_value   - pointer to the converted integer
 * @param{in] base        - the base used to interpret the bytes (decimal, octal or hex)
 * @retval size of the filled buffer
***********************************************************************************************************************/
bool get_arg_as_uint(char* arg_name, const uint8_t *console_string, uint32_t* p_value, uint8_t base)
{
    bool retval = false;
    char* end_ptr;
    int32_t index = 0;

    g_sf_console.p_api->argumentFind((uint8_t*)arg_name,console_string,&index,NULL);

    if(index > 0)
    {
        *p_value = strtoul((char*)&console_string[index],&end_ptr,base);
        retval = ((char*)&console_string[index] != end_ptr);
    }

    return retval;
}


/*******************************************************************************************************************//**
 * @brief  Format and print the data
 *
 *@param{in] address - Address location of the data
 *@param{in] buffer  - Buffer holding the data
 *@param{in] size    - Total size of the buffer
***********************************************************************************************************************/
void format_and_print(uint32_t address, uint8_t* buffer, uint32_t size)
{
    char print_buffer[64];

    uint32_t rows = (size/16) + (size%16);
    uint32_t buff_index = 0;

    while((rows != 0) && (buff_index < size))
    {
        memset(print_buffer,0,64);
        int length = 0;

        length += sprintf(print_buffer, "%08X = ", (unsigned int) address);

        for (int i=0; (i<16) && (buff_index < size); i++, buff_index++)
        {
            length += sprintf(print_buffer+length, "%02X ", (unsigned int)
                                                          buffer[buff_index]);
        }

        sprintf(print_buffer+length, "\r\n");

        address += 16;

        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)print_buffer,
                                                                  TX_WAIT_FOREVER);

    }

}
