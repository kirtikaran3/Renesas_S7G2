/* Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name    : r_qspi_flash_commands.c
* Description  : TODO
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           TODO       1.00    Initial Release.
***********************************************************************************************************************/

#include <stdint.h>
#include "dev_example_thread.h"
#include "dev_example_common.h"
#include "r_qspi.h"
#include "r_qspi_api.h"
/*******************************************************************************************************************//**
 * @ingroup DeveloperExample
 * @defgroup hal_hp_flash
 *
 * Developer Example using N25Q256A QSPI flash
 *
 * @{
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define QSPI_BUFF_SIZE 256

#define QSPI_BASE_ADDRESS   (0x60000000)

#define QSPI_MAX_ADDRESS    (0x63FFFFFF)

#define QSPI_PAGE_SIZE  (256)

#define QSPI_BANK_SIZE (64 * 1024 * 1024)

#define MAX_PAGE_NUM (QSPI_BANK_SIZE / QSPI_PAGE_SIZE)

#define QSPI_SECTOR_SIZE (4 * 1024)

#define MAX_SECTOR_NUM (QSPI_BANK_SIZE / QSPI_SECTOR_SIZE)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
 void qspi_flash_open(sf_console_cb_args_t * p_args);
 void qspi_flash_close(sf_console_cb_args_t * p_args);
 void qspi_flash_page_program(sf_console_cb_args_t * p_args);
 void qspi_flash_sector_erase(sf_console_cb_args_t * p_args);
 void qspi_flash_read(sf_console_cb_args_t * p_args);
 void qspi_flash_status_get(sf_console_cb_args_t * p_args);
 void qspi_flash_bank_select(sf_console_cb_args_t * p_args);
 void qspi_flash_version_get(sf_console_cb_args_t * p_args);

 void display_value(uint32_t address, uint8_t* buffer, uint32_t size);

/***********************************************************************************************************************
 * Private variables and functions
 **********************************************************************************************************************/

 /* The config and control structure for QSPI need to be hand coded as the configurator
  * will not auto generate it in this version
  */


/** QSPI flash command list **/
static const sf_console_command_t g_qspi_flash_commands[] =
{

 {
  (uint8_t*)"open",
  (uint8_t*)"Open the driver",
  qspi_flash_open,
  NULL
 },

 {
  (uint8_t*)"close",
  (uint8_t*)"Close the driver",
  qspi_flash_close,
  NULL
 },

 {
  (uint8_t*)"pageProgram",
  (uint8_t*)"Programs a page of QSPI with a pattern byte "
                          "args: address (60000000 - 63ffffff) pattern (00 - ff)",
  qspi_flash_page_program,
  NULL
 },

 {
  (uint8_t*)"sectorErase",
  (uint8_t*)"Erase a sector of QSPI flash containing the address args: address (60000000 - 63ffffff)",
  qspi_flash_sector_erase,
  NULL
 },

 {
  (uint8_t*)"read",
  (uint8_t*)"Read a page of data from QSPI flash args (input address will be aligned to the page size )"
                                                      " args: address (60000000 - 63ffffff)",
  qspi_flash_read,
  NULL
 },

 {
  (uint8_t*)"statusGet",
  (uint8_t*)"Tells whether the QSPI flash is in middle of a write operation",
  qspi_flash_status_get,
  NULL
 },

 {
  (uint8_t*)"bankSelect",
  (uint8_t*)"Select the QSPI flash bank\r\n\tSynopsis:-bankSelect bankid#(0 - 3)\r\n\t"
          "Example:-bankSelect bankid2",
  qspi_flash_bank_select,
  NULL
 },

 {
  (uint8_t*)"versionGet",
  (uint8_t*)"Get the version of the APIs",
  qspi_flash_version_get,
  NULL
 },

};

static uint8_t qspi_buffer[QSPI_PAGE_SIZE];

/** Flash menu */
const sf_console_menu_t g_qspi_flash_hal_menu =
{
    .menu_prev = &g_sf_console_root_menu,
    .menu_name = (uint8_t*)"r_qspi",
    .num_commands = (sizeof(g_qspi_flash_commands)) / (sizeof(g_qspi_flash_commands[0])),
    .command_list = g_qspi_flash_commands
};


/***********************************************************************************************************************
Functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Opens the qspi flash hal driver
 *
 * @param[in]  p_args Callback context
***********************************************************************************************************************/
void qspi_flash_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_qspi.p_api->open(g_qspi.p_ctrl, g_qspi.p_cfg);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Close the qspi flash hal driver
 *
 * @param[in]  p_args Callback context
***********************************************************************************************************************/
void qspi_flash_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_qspi.p_api->close(g_qspi.p_ctrl);
    ssp_error_code_to_string(ssp_err);
}


/*******************************************************************************************************************//**
 * @brief  Programs a page of the flash memory
 *
 * @param[in]  p_args Callback context
***********************************************************************************************************************/
void qspi_flash_page_program(sf_console_cb_args_t * p_args)
{
    uint32_t user_address;
    uint32_t address;
    uint32_t pattern;

    if(!get_arg_as_uint("address", p_args->p_remaining_string, &user_address, 16) ||
             (user_address < QSPI_BASE_ADDRESS) || (user_address > QSPI_MAX_ADDRESS))
    {
        print_to_console("Invalid Address\r\n");
        return;
    }

    if(!get_arg_as_uint("pattern", p_args->p_remaining_string, &pattern, 16) || (pattern >= 0xFF) )
    {
        print_to_console("Invalid Pattern\r\n");
        return;
    }

    memset(qspi_buffer, (int32_t)pattern, QSPI_BUFF_SIZE);

    /* Get the start address of the page */
    address = user_address - (user_address % QSPI_PAGE_SIZE);

    ssp_err_t ssp_err = g_qspi.p_api->pageProgram(g_qspi.p_ctrl,(uint8_t*)address,
                                                  qspi_buffer,QSPI_BUFF_SIZE);
    ssp_error_code_to_string(ssp_err);


}

/*******************************************************************************************************************//**
 * @brief  Erase a sector of the flash memory
 *
 * @param[in]  p_args Callback context
***********************************************************************************************************************/
void qspi_flash_sector_erase(sf_console_cb_args_t * p_args)
{
     uint32_t address;

     if(!get_arg_as_uint("address", p_args->p_remaining_string, &address, 16) ||
                  (address < QSPI_BASE_ADDRESS) || (address > QSPI_MAX_ADDRESS))
     {
         print_to_console("Invalid Address\r\n");
         return;
     }

     ssp_err_t ssp_err = g_qspi.p_api->sectorErase(g_qspi.p_ctrl,(uint8_t*)address);

     ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Read data from QSPI flash
 *
 * @param[in]  p_args Callback context
***********************************************************************************************************************/
void qspi_flash_read(sf_console_cb_args_t * p_args)
{
    uint32_t address;
    uint32_t user_address;

    if(!get_arg_as_uint("address", p_args->p_remaining_string, &user_address, 16) ||
               (user_address < QSPI_BASE_ADDRESS) || (user_address > QSPI_MAX_ADDRESS))
    {
        print_to_console("Invalid Address\r\n");
        return;
    }

    /* Get the start address of the page */
    address = user_address - (user_address % QSPI_PAGE_SIZE);

    ssp_err_t ssp_err = g_qspi.p_api->read(g_qspi.p_ctrl,(uint8_t*)address,
                                            qspi_buffer,QSPI_BUFF_SIZE);

    if(ssp_err !=SSP_SUCCESS)
    {
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        format_and_print(address,qspi_buffer,QSPI_BUFF_SIZE);
    }
}

/*******************************************************************************************************************//**
 * @brief  Select the QSPI flash bank
 *
 * @param[in]  p_args Callback context
***********************************************************************************************************************/
void qspi_flash_bank_select(sf_console_cb_args_t * p_args)
{
    int32_t bank_id;
    g_sf_console.p_api->argumentFind((uint8_t*)"bankid",p_args->p_remaining_string,
                                       NULL,(int32_t*)&bank_id);
    if((bank_id < 0) || (bank_id > 3))
    {
        print_to_console("Invalid Bank ID \r\n");
        return;
    }

    ssp_err_t ssp_err = g_qspi.p_api->bankSelect((uint32_t)bank_id);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Gets the status of the QSPI flash HAL driver
 *
 * @param[in]  p_args Callback context
***********************************************************************************************************************/
void qspi_flash_status_get(sf_console_cb_args_t * p_args)
{
    bool write_status;
    UNUSED(p_args);

    ssp_err_t ssp_err = g_qspi.p_api->statusGet(g_qspi.p_ctrl,&write_status);
    if( ssp_err != SSP_SUCCESS)
    {
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        sprintf((char*)qspi_buffer,"%sWRITE/ERASE IN PROGRESS\r\n",
                                                (write_status ? "" : "NO "));
        print_to_console((char*)qspi_buffer);
    }

}

/*******************************************************************************************************************//**
 * @brief  Get the version of the APIs
 *
 * @param[in]  p_args Callback context
***********************************************************************************************************************/
void qspi_flash_version_get(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    char   version[32];
    ssp_version_t version_details;
    ssp_err_t ssp_err = g_qspi.p_api->versionGet(&version_details);
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





