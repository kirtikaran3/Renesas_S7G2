
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
 * File Name    : r_fmi_commands.c
 * Description  : This file is used to exercise the APIs of Factory MCU Information module.
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
#define            FMI_PRODUCTINFOGET   "productInfoGet"
#define            FMI_VERSIONGET       "versionGet"


/**  HELP MENU   */

#define            HELP_FMI_PRODUCTINFOGET       "Open the External IRQ instance"
#define            HELP_FMI_VERSIONGET           "Gets the Version of the API"


/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
void hal_fmi_productInfoGet(sf_console_cb_args_t *p_args);
void hal_fmi_versionGet(sf_console_cb_args_t *p_args);


/* Global variables */
/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/

/* FMI Commands Deceleration  */
const sf_console_command_t g_hal_fmi_commands[] =
{
 {
    .command   =  (uint8_t*)FMI_PRODUCTINFOGET,
    .help      =  (uint8_t*)HELP_FMI_PRODUCTINFOGET,
    .callback  =  hal_fmi_productInfoGet,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)FMI_VERSIONGET,
    .help      =  (uint8_t*)HELP_FMI_VERSIONGET,
    .callback  =  hal_fmi_versionGet,
    .context   =  NULL
 },
};//end//



sf_console_menu_t g_hal_fmi_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .num_commands = sizeof(g_hal_fmi_commands)/sizeof(g_hal_fmi_commands[0]),
 .menu_name = (uint8_t*)"r_fmi",
 .command_list = g_hal_fmi_commands
};


/***********************************************************************************************************************
Function
 ***********************************************************************************************************************/


/**********************************************************************************************************************
 * @brief Gives the product information
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_fmi_productInfoGet(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    int count = 0;
    char   str[32];
    fmi_product_info_t *p_product_info;

    ssp_err = g_fmi.p_api->productInfoGet(&p_product_info);
    ssp_error_code_to_string(ssp_err);

    /** Print the details of the device if productInfoGet() returns success*/
    if(SSP_SUCCESS == ssp_err )
    {
        sprintf (str,"contents = %u\n\r",p_product_info->header.contents);
        print_to_console(str);

        sprintf (str,"count = %u\n\r",p_product_info->header.count);
        print_to_console(str);

        sprintf (str,"variant = %u\n\r",p_product_info->header.variant );
        print_to_console(str);

        sprintf (str,"major = %u\n\r",p_product_info->header.major);
        print_to_console(str);

        sprintf (str,"minor = %u\n\r",p_product_info->header.minor);
        print_to_console(str);

        print_to_console("unique id = ");

        /** printing 16 byte unique ID byte by byte */
        for(count=0; count<16; count++)
        {
            sprintf (str,"%x ",p_product_info->unique_id[count]);
            print_to_console(str);
        }

        print_to_console("\r\n");

        sprintf (str,"product name = %.16s\n\r",p_product_info->product_name);
        print_to_console(str);

        sprintf (str,"product marking = %.16s\n\r",p_product_info->product_marking);
        print_to_console(str);

        sprintf (str,"mask revision = %u\n\r",p_product_info->mask_revision);
        print_to_console(str);

        sprintf (str,"pin count = %u\n\r",p_product_info->pin_count);
        print_to_console(str);

        sprintf (str,"package type = %u\n\r",p_product_info->pkg_type);
        print_to_console(str);

        if(p_product_info->temp_range == 2)
        {
            print_to_console("temperature range is -40'C to 85'C\n\r");
        }
        else if(p_product_info->temp_range == 3)
        {
            print_to_console("temperature range is -40'C to 105'C\n\r");
        }

        sprintf (str,"quality code = %u\n\r",p_product_info->quality_code);
        print_to_console(str);

        sprintf (str,"maximum frequency = %uMHz\n\r",p_product_info->max_freq);
        print_to_console(str);
    }
}


/**********************************************************************************************************************
 * @brief Gets the Version of the API
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_fmi_versionGet(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    char   version[32];
    ssp_version_t version_details;

    ssp_err = g_fmi.p_api->versionGet(&version_details);
    ssp_error_code_to_string(ssp_err);

    /** Print the version value only if versionGet() returns success*/
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


