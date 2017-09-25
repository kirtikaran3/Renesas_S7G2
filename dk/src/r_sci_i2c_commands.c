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
 * File Name    : r_sci_i2c_commands.c
 * Description  : I2C Master mode driver implementation on the SCI_I2C peripheral.
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

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
/* Prototypes for the callback functions called by I2C console commands */

void sci_i2c_hal_open(sf_console_cb_args_t * p_args);
void sci_i2c_hal_close(sf_console_cb_args_t * p_args);
void sci_i2c_hal_read(sf_console_cb_args_t * p_args);
void sci_i2c_hal_write(sf_console_cb_args_t * p_args);
void sci_i2c_hal_versionGet(sf_console_cb_args_t * p_args);
void sci_i2c_hal_reset(sf_console_cb_args_t * p_args);


/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/


/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/

const sf_console_command_t g_sci_i2c_hal_commands[] =
{
 {
  (uint8_t*)"open",
  (uint8_t*)"Opens the i2c device",
  sci_i2c_hal_open,
  NULL
 },

 {
  (uint8_t*)"read",
  (uint8_t*)"Reads data from i2c device\r\n\t\tSynopsis:- read length# restart#\r\n\t\t"
  "example:- read length2 restart1",
  sci_i2c_hal_read,
  NULL
 },

 {
  (uint8_t*)"write",
  (uint8_t*)"Writes to i2c device\r\n\t\tSynopsis:- write length# restart# data#\r\n\t\t"
  "example:- write length3 restart1 data06 FF 0F",
  sci_i2c_hal_write,
  NULL
 },

 {
  (uint8_t*)"close",
  (uint8_t*)"Closes the i2c device",
  sci_i2c_hal_close,
  NULL
 },

 {
  (uint8_t*)"versionGet",
  (uint8_t*)"Gets the version of API",
  sci_i2c_hal_versionGet,
  NULL
 },

 {
  (uint8_t*)"reset",
  (uint8_t*)"Resets the driver instance",
  sci_i2c_hal_reset,
  NULL
 },
};


/** sci i2c menu */
const sf_console_menu_t g_sci_i2c_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"r_sci_i2c",
 .num_commands = (sizeof(g_sci_i2c_hal_commands)) / (sizeof(g_sci_i2c_hal_commands[0])),
 .command_list = g_sci_i2c_hal_commands
};

static uint8_t process_buffer[128];

/*******************************************************************************************************************//**
 * @brief   Opens the I2C device. Currently this instance is mapped to the ioport expander on S7G2 kit connected
 * 			SCI channel 7.
 *
 *  This function will call the SCI_I2C HAL driver API OPEN
 *
 *	@example open
 *  @retval VOID
 ***********************************************************************************************************************/
void sci_i2c_hal_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    /** calls SCI_I2C_OPEN API */
    ssp_err = g_i2c_sci.p_api->open(g_i2c_sci.p_ctrl,g_i2c_sci.p_cfg);

    ssp_error_code_to_string(ssp_err);
}


/*******************************************************************************************************************//**
 * @brief   Read from the I2C slave device.
 *
 *  This function will call the SCI HAL driver API READ
 *  @example read length2 restart0
 *  @retval VOID
 ***********************************************************************************************************************/
void sci_i2c_hal_read(sf_console_cb_args_t * p_args)
{
    uint8_t input_buff[16] = {0x00};
    uint32_t length = 0;
    uint32_t restart = 0;
    uint32_t buff_index =0;

    /** Extract length and restart value given from CLI */

    if(!get_arg_as_uint("length", p_args->p_remaining_string, (uint32_t*)&length,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("restart", p_args->p_remaining_string, (uint32_t*)&restart,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validating length (valid length is 1 to 15) */
    if((length > 0) && (length < 16) )
    {
        /** calls SCI_I2C_READ API */
        ssp_err_t ssp_err = g_i2c_sci.p_api->read(g_i2c_sci.p_ctrl,input_buff, length, (restart == 1));

        if(SSP_SUCCESS == ssp_err )
        {
            for(unsigned int i =0; i < length; i++)
            {
                buff_index += (uint32_t)sprintf((char *)(process_buffer + buff_index), "%02X ",input_buff[i]);
            }

            sprintf((char *)(process_buffer + buff_index), "\r\n");

            g_sf_console.p_api->write(g_sf_console.p_ctrl,process_buffer, TX_WAIT_FOREVER);
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }
    }
    else
    {
        print_to_console("Invalid Arguments\r\n");
    }
}


/*******************************************************************************************************************//**
 * @brief   Write into the I2C slave device.
 *
 *  This function will call the SCI HAL driver API write
 *  @example write length2 restart0 data02 ff
 *  @retval VOID
 ***********************************************************************************************************************/

void sci_i2c_hal_write(sf_console_cb_args_t * p_args)
{
    uint32_t restart = 0;
    int32_t index = 0;

    uint8_t write_buffer[16];
    uint8_t count = 0;
    uint32_t hex_data = 0;
    uint32_t length = 0;

    /** Extract length, restart and data value given from CLI */

    if(!get_arg_as_uint("restart", p_args->p_remaining_string, (uint32_t*)&restart,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("length", p_args->p_remaining_string, (uint32_t*)&length,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    g_sf_console.p_api->argumentFind((uint8_t*)"data",p_args->p_remaining_string,&index,NULL);
    if(index == -1)
    {
        print_to_console("data Arguments was not found\r\n");
        return;
    }
    strncpy((char *)process_buffer, (char *)(&(p_args->p_remaining_string[index])), 128);

    if(strlen((char *)process_buffer))
    {
        char* token = strtok((char *)process_buffer," ");
        while(token != NULL)
        {
            /** converts data which is in string to hex */
            hex_data = strtoul(token, NULL, 16);

            if(hex_data <= 0xFF)
            {
                /** copy hex data to write buffer */
                write_buffer[count] = (uint8_t) hex_data;
                count++;
            }
            token = strtok(NULL," ");
        }
    }
    if((count == length) && (length > 0))
    {
        /** calls SCI_I2C_WRITE API */
        ssp_err_t ssp_err = g_i2c_sci.p_api->write(g_i2c_sci.p_ctrl,write_buffer,length, (restart == 1));
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        print_to_console("Invalid Arguments\r\n");
    }
}


/*******************************************************************************************************************//**
 * @brief   Close the I2C device. May power off IIC peripheral
 *
 *  This function will call the SCI_I2C HAL driver API CLOSE, with Channel Number configurable in CLI for CLOSE.
 *	@example close
 *  @retval VOID
 ***********************************************************************************************************************/

void sci_i2c_hal_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    /** calls SCI_I2C_CLOSE API */
    ssp_err_t ssp_err = g_i2c_sci.p_api->close(g_i2c_sci.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief   Reset the I2C device.
 *
 *  This function will call the SCI_I2C HAL driver API RESET, none configurable from CLI for RESET.
 *	@example reset
 *  @retval VOID
 ***********************************************************************************************************************/
void sci_i2c_hal_reset(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    /** calls SCI_I2C_RESET API */
    ssp_err_t ssp_err = g_i2c_sci.p_api->reset(g_i2c_sci.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}


/*******************************************************************************************************************//**
 * @brief   Prints the version of the API
 *
 *  This function will call the SCI_I2C HAL driver API versionGet, none configurable from CLI for versionGet.
 * @example versionGet
 *  @retval VOID
 ***********************************************************************************************************************/
void sci_i2c_hal_versionGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    char   version[32];
    ssp_version_t version_details;
    /** calls SCI_I2C_VERSIONGET API */
    ssp_err_t ssp_err = g_i2c_sci.p_api->versionGet(&version_details);
    ssp_error_code_to_string(ssp_err);

    sprintf (version,"Code version  =  %d.%d\r\n", version_details.code_version_major,
            version_details.code_version_minor);
    print_to_console(version);

    sprintf (version,"\r\nAPI version  =  %d.%d\r\n", version_details.api_version_major,
            version_details.api_version_minor);
    print_to_console(version);
}



