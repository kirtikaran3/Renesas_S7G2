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
* File Name    : sf_adc_demo_thread_entry.c
* Description  : This is the thread which runs in the background and gives back the console for the user to control
*                audio from CLI
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

#include "sf_adc_demo_thread.h"
#include "dev_example_common.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define I2C_WRITE_BUFF_SIZE 2

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/
adc_data_size_t adc_data;
uint8_t i2c_write_buffer[I2C_WRITE_BUFF_SIZE];

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions
***********************************************************************************************************************/

void sf_adc_demo_thread_entry(void)
{
    while (1)
    {

        tx_semaphore_get(&adc_semaphore, TX_WAIT_FOREVER);
        i2c_write_buffer[0] = (uint8_t)0x02;
        adc_data = g_sf_adc_periodic.p_ctrl->p_data_buffer[0];
        if(adc_data < 32)
        {
            i2c_write_buffer[1] = 0x0;
        }
        else if(((adc_data) >= 32) & ((adc_data) < 64))
        {
            i2c_write_buffer[1] = 0x01;
        }
        else if(((adc_data) >=64 ) & ((adc_data) < 96))
        {
            i2c_write_buffer[1] = 0x03;
        }
        else if(((adc_data) >= 96) & ((adc_data) < 128))
        {
            i2c_write_buffer[1] = 0x07;
        }
        else if(((adc_data) >=128) & ((adc_data) < 160))
        {
            i2c_write_buffer[1] = 0x0F;
        }
        else if(((adc_data) >= 160) & ((adc_data) < 192))
        {
            i2c_write_buffer[1] = 0x1F;
        }
        else if(((adc_data) >= 192) & ((adc_data) < 224))
        {
            i2c_write_buffer[1] = 0x3F;
        }
        else if(((adc_data) >= 224) & ((adc_data) < 255))
        {
            i2c_write_buffer[1] = 0x7F;
        }
        else
        {
            i2c_write_buffer[1] = 0xFF;
        }
        g_i2c_sci.p_api->write(g_i2c_sci.p_ctrl,i2c_write_buffer,I2C_WRITE_BUFF_SIZE, false);

    }
}
