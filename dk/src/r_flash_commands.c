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
 * File Name    : r_flash_commands.c
 * Description  : TODO
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           TODO       1.00    Initial Release.
 ***********************************************************************************************************************/

#include <stdint.h>
#include "bsp_api.h"
#include "dev_example_thread.h"
#include "dev_example_common.h"
/*******************************************************************************************************************//**
 * @ingroup DeveloperExample
 * @defgroup hal_hp_flash
 *
 * Developer Example using S7G2 internal flash
 *
 * @{
 ***********************************************************************************************************************/


/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define CODE_FLASH			0
#define DATA_FLASH			1

#define FLASH_CF_BLOCK_1    0x00002000            /*   8KB: 0x00002000 - 0x00003FFF */
#define FLASH_CF_BLOCK_4    0x00008000
#define FLASH_CF_BLOCK_7    0x0000E000
#define FLASH_CF_BLOCK_133  0x003F8000

#define FLASH_DF_BLOCK_0    0x40100000            /*   64B: 0x40100000 - 0x4010003F */
#define FLASH_DF_BLOCK_1    0x40100040            /*   64B: 0x40100040 - 0x4010007F */

#define FLASH_DF_BLOCK_1023 0x4010FFC0
#define FLASH_CF_BLOCK_90   0x002A0000
#define FLASH_CF_BLOCK_80   0x00250000
#define FLASH_CF_BLOCK_81   0x00258000

#define FLASH_NUM_BLOCKS_CF        (134)        ///< Total number of Code Flash Blocks
#define FLASH_NUM_BLOCKS_DF        (1024)       ///< Total number of Data Flash Blocks
#define FLASH_MIN_PGM_SIZE_DF      (4)          ///< Minimum program size for Data Flash
#define FLASH_MIN_PGM_SIZE_CF      (256)        ///< Minimum program size for Code Flash
#define FLASH_CODE_SMALL_BLOCK_SZ  (8192)       ///< Smallest Code Flash block size
#define FLASH_CODE_LARGE_BLOCK_SZ  (32768)      ///< Largest Code Flash block size

#define FLASH_DATA_BLOCK_SZ        (64)         ///< Data Block size
#define FLASH_DATA_BLANK_CHECK_MAX (65536)      ///< Max # of bytes that may be blank check for Data Flash
#define FLASH_DATA_BLANK_CHECK_MIN (4)          ///< Min # of bytes that may be blank check for Data Flash

#define FLASH_CF_BLOCK_0           0            ///<   Start of Code Flash Area
#define FLASH_CF_BLOCK_8           0x00010000   ///<   First 32K Code block/
#define FLASH_CF_BLOCK_END         0x003FFFFF   ///<   End of Code Flash Area
#define FLASH_DF_BLOCK_0           0x40100000   ///<   Start of Data Flash area    /*   64B: 0x40100000 - 0x4010003F */
#define FLASH_DF_BLOCK_END         0x4010FFFF   ///<   End of Data Flash Area

#define NUM_CF_SMALL_BLOCKS			8 			///< Number of code flash blocks with 8k size

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
void flash_open(sf_console_cb_args_t * p_args);
void flash_read(sf_console_cb_args_t * p_args);
void flash_write(sf_console_cb_args_t * p_args);
void flash_erase(sf_console_cb_args_t * p_args);
void flash_blank_check(sf_console_cb_args_t * p_args);
void flash_close(sf_console_cb_args_t * p_args);
void flash_status_get(sf_console_cb_args_t * p_args);
void flash_reset(sf_console_cb_args_t * p_args);
void flash_access_window_set(sf_console_cb_args_t * p_args);
void flash_access_window_clear(sf_console_cb_args_t * p_args);
void flash_startup_area_select(sf_console_cb_args_t * p_args);
void flash_update_clock_frequency(sf_console_cb_args_t * p_args);
void flash_version_get(sf_console_cb_args_t * p_args);

uint32_t flash_get_address(uint32_t flash_type, uint32_t block_id);
uint32_t flash_get_op_size(uint32_t flash_type);
void flash_print_value(uint32_t address, uint8_t* buffer, uint32_t size);


/***********************************************************************************************************************
 * Private variables and functions
 **********************************************************************************************************************/
/** flash command list */
static const sf_console_command_t g_flash_commands[] =
{
 {
  (uint8_t*)"open",
  (uint8_t*)"Open the driver",
  flash_open,
  NULL
 },

 {
  (uint8_t*)"read",
  (uint8_t*)"Read flash block(only first 256Bytes for CF)\r\n\tSynopsis:-read TYPE#(0-code"
  " 1-data) BLOCK#(0-1023)\r\n\tExample:-read TYPE1 BLOCK4",
  flash_read,
  NULL
 },

 {
  (uint8_t*)"write",
  (uint8_t*)"Writes pattern byte into block\r\n\tSynopsis:-write TYPE#(0 or 1)"
  "BLOCK#(0-1023) PATTERN#\r\n\tExample:-write TYPE1 BLOCK2 PATTERN8",
  flash_write,
  NULL
 },

 {
  (uint8_t*)"erase",
  (uint8_t*)"Erase a flash block\r\n\tSynopsis:-erase TYPE#(0-code 1-data) BLOCK#(0-1023)"
  "\r\n\tExample:-erase TYPE1 BLOCK5",
  flash_erase,
  NULL
 },

 {
  (uint8_t*)"blankCheck",
  (uint8_t*)"Check whether flash block is blank\r\n\tSynopsis:-blankCheck TYPE#(0 or 1)"
  " BLOCK#(0-1023)\r\n\tExample:-blankCheck TYPE0 BLOCK5",
  flash_blank_check,
  NULL
 },

 {
  (uint8_t*)"close",
  (uint8_t*)"Close the Flash driver",
  flash_close,
  NULL
 },

 {
  (uint8_t*)"statusGet",
  (uint8_t*)"Gets the status of the flash peripheral",
  flash_status_get,
  NULL
 },


 {
  (uint8_t*)"reset",
  (uint8_t*)"Reset the flash device",
  flash_reset,
  NULL
 },

 {
  (uint8_t*)"accessWindowSet",
  (uint8_t*)"Set access window\r\n\tSynopsis:-accessWindowSet SBLOCK#(0-133) EBLOCK#(0-133)"
  "\r\n\tExample:-accessWindowSet SBLOCK104 EBLOCK14",
  flash_access_window_set,
  NULL
 },

 {
  (uint8_t*)"accessWindowClear",
  (uint8_t*)"Clear the access window",
  flash_access_window_clear,
  NULL
 },

 {
  (uint8_t*)"startupAreaSelect",
  (uint8_t*)"Select the startup area\r\n\tSynopsis:-startupAreaSelect BLOCK(0-1)(Only temporary)"
  "\r\n\tExample:-startupAreaSelect BLOCK0",
  flash_startup_area_select,
  NULL
 },

 {
  (uint8_t*)"updateClockFreq",
  (uint8_t*)"Update the frequency of flash clock ",
  flash_update_clock_frequency,
  NULL
 },

 {
  (uint8_t*)"versionGet",
  (uint8_t*)"Get the API version ",
  flash_version_get,
  NULL
 },

};



/** Flash menu */
const sf_console_menu_t g_flash_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"r_flash_hp",
 .num_commands = (sizeof(g_flash_commands)) / (sizeof(g_flash_commands[0])),
 .command_list = g_flash_commands
};

uint8_t flash_buffer[FLASH_MIN_PGM_SIZE_CF] = {0x00};


/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Calculates the flash address based on flash type and block ID
 *
 * @param[in] flash_type - type of flash, block_id - ID number of the block
 ***********************************************************************************************************************/
uint32_t flash_get_address(uint32_t flash_type, uint32_t block_id)
{
    uint32_t address = FLASH_CF_BLOCK_0;

    if(flash_type == DATA_FLASH)
    {
        address = FLASH_DF_BLOCK_0 + (block_id * FLASH_DATA_BLOCK_SZ);
    }
    else
    {
        if( block_id <= NUM_CF_SMALL_BLOCKS)
        {
            address = FLASH_CF_BLOCK_0 + (block_id * FLASH_CODE_SMALL_BLOCK_SZ);
        }
        else
        {
            address = FLASH_CF_BLOCK_8 + \
                    ((block_id - NUM_CF_SMALL_BLOCKS)  * FLASH_CODE_LARGE_BLOCK_SZ);
        }
    }

    return address;
}

/*******************************************************************************************************************//**
 * @brief  Adjust the size of flash buffer so that ops wont fail
 *
 * @param[in] flash_type - type of flash, user_size - size input by the user
 ***********************************************************************************************************************/
uint32_t flash_get_op_size(uint32_t flash_type)
{
    return flash_type ? 64 : 256;
}


/*******************************************************************************************************************//**
 * @brief  Intializes the Flash HAL APIs.
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    ssp_err_t ssp_err = g_flash_hp.p_api->open(g_flash_hp.p_ctrl, g_flash_hp.p_cfg);
    ssp_error_code_to_string(ssp_err);
}


/*******************************************************************************************************************//**
 * @brief  Reads from a flash address
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_read(sf_console_cb_args_t * p_args)
{
    uint32_t flash_type = 0;
    uint32_t block_id  = 0;
    uint32_t read_size = 0;
    uint32_t flash_address = 0;

    if(!get_arg_as_uint("TYPE", p_args->p_remaining_string, (uint32_t*)&flash_type,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("BLOCK", p_args->p_remaining_string, (uint32_t*)&block_id,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if((flash_type > DATA_FLASH) || (block_id >= FLASH_NUM_BLOCKS_DF ))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Arguments\r\n",
                                  TX_WAIT_FOREVER);
    }
    else if((flash_type == CODE_FLASH) && (block_id >= FLASH_NUM_BLOCKS_CF))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Invalid BLOCK ID for code flash", TX_WAIT_FOREVER);
    }
    else
    {
        memset(flash_buffer,0xFF,sizeof(flash_buffer));

        flash_address = flash_get_address(flash_type, block_id);
        read_size     = flash_get_op_size(flash_type);

        /* Perform the read from flash */
        ssp_err_t ssp_err = g_flash_hp.p_api->read(g_flash_hp.p_ctrl,flash_buffer, flash_address, read_size);

        if(ssp_err == SSP_SUCCESS)
        {
            format_and_print(flash_address,flash_buffer,read_size);
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }
    }

}

/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief  writes to a flash address.
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_write(sf_console_cb_args_t * p_args)
{
    uint32_t flash_type = 0;
    uint32_t block_id = 0;
    uint32_t pattern_byte = 0;
    uint32_t write_size = 0;
    uint32_t flash_address = 0;


    if(!get_arg_as_uint("TYPE", p_args->p_remaining_string, (uint32_t*)&flash_type,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("BLOCK", p_args->p_remaining_string, (uint32_t*)&block_id,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("PATTERN", p_args->p_remaining_string, (uint32_t*)&pattern_byte,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if((flash_type > DATA_FLASH) || (block_id >= FLASH_NUM_BLOCKS_DF ))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }
    else if((flash_type == CODE_FLASH) && (block_id >= FLASH_NUM_BLOCKS_CF))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Invalid BLOCK ID for code flash", TX_WAIT_FOREVER);
    }
    else
    {
        flash_address = flash_get_address(flash_type, block_id);

        /* Make sure that you are not writing into the first half of the code flash, otherwise
         * it will corrupt the program
         */
        if(flash_address < 0x00200000)
        {
            g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                      (uint8_t*)"Write may corrupt the current binary. Aborting ... \r\n",
                                      TX_WAIT_FOREVER);
        }
        else
        {
            write_size = flash_get_op_size(flash_type);

            /* Fill the buffer with pattern */
            for( uint32_t i = 0; i < write_size; i++)
            {
                flash_buffer[i]=(uint8_t)pattern_byte;
            }

            /* Perform the write into flash */
            ssp_err_t ssp_err = g_flash_hp.p_api->write(g_flash_hp.p_ctrl,(uint32_t)flash_buffer, flash_address,
                                                        write_size );

            ssp_error_code_to_string(ssp_err);
        }
    }

}


/*******************************************************************************************************************//**
 * @brief  Erase flash blocks
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_erase(sf_console_cb_args_t * p_args)
{
    uint32_t flash_type = 0;
    uint32_t block_id = 0;
    uint32_t flash_address = 0;


    if(!get_arg_as_uint("TYPE", p_args->p_remaining_string, (uint32_t*)&flash_type,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("BLOCK", p_args->p_remaining_string, (uint32_t*)&block_id,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if((flash_type > DATA_FLASH) || (block_id >= FLASH_NUM_BLOCKS_DF ))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }
    else if((flash_type == CODE_FLASH) && (block_id >= FLASH_NUM_BLOCKS_CF))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Invalid BLOCK ID for code flash", TX_WAIT_FOREVER);
    }
    else
    {
        flash_address = flash_get_address(flash_type, block_id);

        if(flash_address < 0x00200000)
        {
            g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                      (uint8_t*)"Erase may corrupt the current binary. Aborting...\r\n",
                                      TX_WAIT_FOREVER);
        }
        else
        {
            ssp_err_t ssp_err = g_flash_hp.p_api->erase(g_flash_hp.p_ctrl, flash_address, 1);
            ssp_error_code_to_string(ssp_err);
        }
    }
}


/*******************************************************************************************************************//**
 * @brief  Perform blank check on the flash blocks
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_blank_check(sf_console_cb_args_t * p_args)
{
    uint32_t flash_type = 0;
    uint32_t block_id = 0;
    uint32_t blank_size = 0;
    uint32_t flash_address = 0;

    if(!get_arg_as_uint("TYPE", p_args->p_remaining_string, (uint32_t*)&flash_type,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("BLOCK", p_args->p_remaining_string, (uint32_t*)&block_id,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if((flash_type > DATA_FLASH) || (block_id >= FLASH_NUM_BLOCKS_DF ))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Invalid Arguments\r\n", TX_WAIT_FOREVER);
    }
    else if((flash_type == CODE_FLASH) && (block_id >= FLASH_NUM_BLOCKS_CF))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Invalid BLOCK ID for code flash", TX_WAIT_FOREVER);
    }
    else
    {
        flash_address = flash_get_address(flash_type, block_id);
        flash_result_t blank_check_result = {0};
        blank_size = flash_get_op_size(flash_type);

        ssp_err_t ssp_err = g_flash_hp.p_api->blankCheck(g_flash_hp.p_ctrl, flash_address, blank_size,
                                                         &blank_check_result);
        if(ssp_err != SSP_SUCCESS)
        {
            ssp_error_code_to_string(ssp_err);
        }
        else
        {
            uint8_t result_string[16];
            switch (blank_check_result)
            {
                case FLASH_RESULT_NOT_BLANK:
                    sprintf((char*)result_string,"%s\r\n","FLASH_RESULT_NOT_BLANK");
                    break;

                case FLASH_RESULT_BGO_ACTIVE:
                    sprintf((char*)result_string,"%s\r\n","FLASH_RESULT_BGO_ACTIVE");
                    break;

                default:
                    sprintf((char*)result_string,"%s\r\n","FLASH_RESULT_BLANK");
                    break;
            }

            g_sf_console.p_api->write(g_sf_console.p_ctrl,result_string, TX_WAIT_FOREVER);
        }
    }
}

/*******************************************************************************************************************//**
 * @brief  De-initialize flash APIs
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_flash_hp.p_api->close(g_flash_hp.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}


/*******************************************************************************************************************//**
 * @brief  Gets the status of the flash
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_status_get(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_flash_hp.p_api->statusGet(g_flash_hp.p_ctrl);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Reset the flash device
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_reset(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_flash_hp.p_api->reset(g_flash_hp.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief set the access window for flash
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_access_window_set(sf_console_cb_args_t * p_args)
{
    uint32_t sblock_id = 0;
    uint32_t eblock_id = 0;
    uint32_t flash_start_address = 0;
    uint32_t flash_end_address = 0;

    if(!get_arg_as_uint("SBLOCK", p_args->p_remaining_string, (uint32_t*)&sblock_id,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("EBLOCK", p_args->p_remaining_string, (uint32_t*)&eblock_id,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if((sblock_id >= FLASH_NUM_BLOCKS_CF) || (eblock_id >= FLASH_NUM_BLOCKS_CF))
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Invalid BLOCK ID for code flash", TX_WAIT_FOREVER);
    }
    else
    {
        flash_start_address = flash_get_address(CODE_FLASH,sblock_id);
        flash_end_address  = flash_get_address(CODE_FLASH,eblock_id);
        ssp_err_t ssp_err = g_flash_hp.p_api->accessWindowSet(g_flash_hp.p_ctrl,
                                                              flash_start_address,
                                                              flash_end_address);
        ssp_error_code_to_string(ssp_err);
    }
}

/*******************************************************************************************************************//**
 * @brief Clear the access window for flash
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_access_window_clear(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_flash_hp.p_api->accessWindowClear(g_flash_hp.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief Select the startup area for flash
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_startup_area_select(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    uint32_t block_id = 0;

    if(!get_arg_as_uint("BLOCK", p_args->p_remaining_string, (uint32_t*)&block_id,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(block_id > 1)
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Invalid Argument\r\n",
                                  TX_WAIT_FOREVER);
    }
    else
    {
        ssp_err_t ssp_err = g_flash_hp.p_api->startupAreaSelect(g_flash_hp.p_ctrl, block_id,false);
        ssp_error_code_to_string(ssp_err);
    }
}

/*******************************************************************************************************************//**
 * @brief Update the flash clock frequency
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_update_clock_frequency(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_flash_hp.p_api->updateFlashClockFreq(g_flash_hp.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief Get the version of flash APIs
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void flash_version_get(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    char   version[32];
    ssp_version_t version_details;
    ssp_err_t ssp_err = g_flash_hp.p_api->versionGet(&version_details);
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


