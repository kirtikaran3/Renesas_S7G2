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
 * File Name    : r_crc_commands.c
 * Description  : The CRC (Cyclic Redundancy Check) calculator generates CRC codes using five different polynomials
 *                including 8 bit,16 bit, and 32 bit variations. Calculation can be performed by sending data to the
 *                block using the CPU or by snooping on read or write activity on one of 10 SCI channels
 *
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
#define CRC_SNOOP_MAX_CHANNEL_PRESENT 10
/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
/* Prototypes for the callback functions called by CRC console commands */

void crc_hal_open(sf_console_cb_args_t * p_args);
void crc_hal_close(sf_console_cb_args_t * p_args);
void crc_hal_crcResultGet(sf_console_cb_args_t * p_args);
void crc_hal_snoopEnable(sf_console_cb_args_t * p_args);
void crc_hal_snoopDisable(sf_console_cb_args_t * p_args);
void crc_hal_snoopCfg(sf_console_cb_args_t * p_args);
void crc_hal_calculate(sf_console_cb_args_t * p_args);
void crc_hal_versionGet(sf_console_cb_args_t * p_args);


/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Extern global variables
 ***********************************************************************************************************************/

/** Previous menu */
extern const sf_console_menu_t g_sf_console_root_menu;


/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/
static char process_buff[128];

/** crc console command menu */

const sf_console_command_t g_crc_hal_commands[] =
{
 {
  (uint8_t*)"open",
  (uint8_t*)"Open the CRC driver module",
  crc_hal_open,
  NULL
 },
 {
  (uint8_t*)"close",
  (uint8_t*)"Close the CRC module driver",
  crc_hal_close,
  NULL
 },
 {
  (uint8_t*)"crcResultGet",
  (uint8_t*)"Return the current calculated value.",
  crc_hal_crcResultGet,
  NULL
 },
 {
  (uint8_t*)"snoopEnable",
  (uint8_t*)"Enable snooping\r\n\t\t  Synopsis:- snoopEnable seed#\r\n\t\t"
  "  Example:- snoopEnable seed32",
  crc_hal_snoopEnable,
  NULL
 },
 {
  (uint8_t*)"snoopDisable",
  (uint8_t*)"Disable snooping",
  crc_hal_snoopDisable,
  NULL
 },
 {
  (uint8_t*)"snoopCfg",
  (uint8_t*)"Configure snoop\r\n\t\t  Synopsis:- snoopCfg channel#(0 -9) direction#( 0 or 1)\r\n\t\t"
  "  Example:- snoopCfg channel7 direction1",
  crc_hal_snoopCfg,
  NULL
 },
 {
  (uint8_t*)"calculate",
  (uint8_t*)"Perform a CRC calculation\r\n\t\t  Synopsis:- calculate length# seed# data#\r\n\t\t"
  "  Example:- calculate length02 seed25 data3f d4",
  crc_hal_calculate,
  NULL
 },
 {
  (uint8_t*)"versionGet",
  (uint8_t*)"Get the driver version based on compile time macros",
  crc_hal_versionGet,
  NULL
 },
};


/** crc console menu */

const sf_console_menu_t g_crc_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"r_crc",
 .num_commands = (sizeof(g_crc_hal_commands)) / (sizeof(g_crc_hal_commands[0])),
 .command_list = g_crc_hal_commands

};


/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/



/*******************************************************************************************************************//**
 * @brief Open the CRC driver module. R_CRC_Open() and powerONS the CRC hardware
 * Open the CRC driver module and initialize the block according to the passed-in configuration structure.
 *
 * @example open
 *
 *  @retval VOID
 ***********************************************************************************************************************/

void crc_hal_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_crc.p_api->open(g_crc.p_ctrl,g_crc.p_cfg);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Close the CRC module driver R_CRC_Close() and power offs the CRC hardware
 *
 *  @example close
 *  @retval VOID
 ***********************************************************************************************************************/

void crc_hal_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_crc.p_api->close(g_crc.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}
/*******************************************************************************************************************//**
 * @brief  Return the current calculated value from the last CRC calculation. R_CRC_CalculatedValueGet()
 *
 * @example crcresultget
 *  @retval VOID
 ***********************************************************************************************************************/

void crc_hal_crcResultGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    uint8_t   result[32];
    uint32_t crc_result = 0;
    ssp_err_t ssp_err = g_crc.p_api->crcResultGet(g_crc.p_ctrl,&crc_result);
    ssp_error_code_to_string(ssp_err);
    /** if crcResultGet() results success print crc value on console */
    if( SSP_SUCCESS == ssp_err )
    {
        sprintf ((char *)result,"CRC result  =  %d\r\n", (int)crc_result );
        g_sf_console.p_api->write(g_sf_console.p_ctrl, result, TX_WAIT_FOREVER);

    }


}

/*******************************************************************************************************************//**
 * @brief  Enable snooping. R_CRC_SnoopEnable()
 *
 * @example snoopEnable seed#
 *  @retval VOID
 ***********************************************************************************************************************/

void crc_hal_snoopEnable(sf_console_cb_args_t * p_args)
{
    int32_t crc_seed;

    if(!get_arg_as_uint("seed", p_args->p_remaining_string, (uint32_t*)&crc_seed,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    ssp_err_t ssp_err = g_crc.p_api->snoopEnable(g_crc.p_ctrl,(uint32_t)crc_seed);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Disable snooping. R_CRC_SnoopDisable()
 *
 * @example snoopDisable
 *  @retval VOID
 ***********************************************************************************************************************/

void crc_hal_snoopDisable(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_crc.p_api->snoopDisable(g_crc.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Configure the snoop channel and direction. R_CRC_SnoopCfg()
 *
 * @example snoopCfg channel# direction# - channel(0 to 9) and direction(read=0 and write 1)
 *  @retval VOID
 ***********************************************************************************************************************/

void crc_hal_snoopCfg(sf_console_cb_args_t * p_args)
{
    crc_snoop_cfg_t snoopCfg;
    ssp_err_t ssp_err;

    /** Extract channel number and direction(read = 0 and write = 1) from console*/

    if(!get_arg_as_uint("channel", p_args->p_remaining_string, (uint32_t*)&(snoopCfg.snoop_channel),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("direction", p_args->p_remaining_string, (uint32_t*)&(snoopCfg.snoop_direction),DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validate provided channel number is in the range*/
    if(CRC_SNOOP_MAX_CHANNEL_PRESENT > snoopCfg.snoop_channel)
    {
        /** validate provided direction is valid*/
        if((snoopCfg.snoop_direction == 0) || (snoopCfg.snoop_direction == 1) )
        {
            ssp_err = g_crc.p_api->snoopCfg(g_crc.p_ctrl,&snoopCfg);
            ssp_error_code_to_string(ssp_err);
        }
        else
        {
            g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
        }
    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }

}

/*******************************************************************************************************************//**
 * @brief  Perform a CRC calculation on a block of data. R_CRC_Calculate()
 *
 * @example calculate length# seed# data#
 *  @retval VOID
 ***********************************************************************************************************************/

void crc_hal_calculate(sf_console_cb_args_t * p_args)
{
    int32_t index = 0;
    ssp_err_t ssp_err;
    uint8_t write_buffer[16];
    uint32_t count = 0;
    uint32_t hex_data = 0;
    uint32_t  cal_val;
    char   result[32];
    uint32_t length = 0;
    uint32_t crc_seed = 0;

    /** Extract length, data and seed from console*/

    if(!get_arg_as_uint("length", p_args->p_remaining_string, (uint32_t*)&length,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("seed", p_args->p_remaining_string, (uint32_t*)&crc_seed,DEC))
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

    /** copy complete string provided as data */
    strncpy(process_buff,(char *) &(p_args->p_remaining_string[index]), 128);


    if(strlen(process_buff))
    {
        char* token = strtok(process_buff," ");
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
    /** validate the number of data and length are same */
    if((count == length) && (length > 0))
    {
        /** calls R_CRC_Calculate API */
        ssp_err = g_crc.p_api->calculate(g_crc.p_ctrl,write_buffer,count,crc_seed,&cal_val);
        ssp_error_code_to_string(ssp_err);
        if( SSP_SUCCESS == ssp_err )
        {
            sprintf (result,"crc result  =  %d\r\n", (int)cal_val );
            g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*) result, TX_WAIT_FOREVER);

        }
    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }

}


/*******************************************************************************************************************//**
 * @brief  Get the driver version based on compile time macros. R_CRC_VersionGet()
 *
 * @example versionGet
 *  @retval VOID
 ***********************************************************************************************************************/

void crc_hal_versionGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    uint8_t   version[32];
    ssp_version_t version_details;
    ssp_err = g_crc.p_api->versionGet(&version_details);
    ssp_error_code_to_string(ssp_err);
    /** Print the version value only if versionGet() results success*/
    if(SSP_SUCCESS == ssp_err )
    {
        sprintf ((char *)version,"Code version  =  %d.%d\r\n", version_details.code_version_major,
                 version_details.code_version_minor);
        g_sf_console.p_api->write(g_sf_console.p_ctrl, version, TX_WAIT_FOREVER);

        sprintf ((char *)version,"\r\nAPI version  =  %d.%d\r\n", version_details.api_version_major,
                version_details.api_version_minor);
        g_sf_console.p_api->write(g_sf_console.p_ctrl, version, TX_WAIT_FOREVER);
    }
}

