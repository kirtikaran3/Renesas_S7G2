/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_adc_private_s3a7.c
* Description  : Implements ADC LLD with S3A7 specific details
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup ADC
 * @{
***********************************************************************************************************************/


/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#include "bsp_api.h"
#include "r_adc_api.h"
#include "../../../r_adc_private.h"
#if (BSP_MCU_GROUP_S3A7 == 1)
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
 Global Variables
**********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   r_adc_open_cfg_check_unique
 *
 * This function checks the ADC open configuration arguments
 *
 * @retval  SSP_ERR_INVALID_ARGUMENT   Parameter has invalid value
 * @retval  SSP_SUCCESS                Call successful.
***********************************************************************************************************************/
ssp_err_t r_adc_open_cfg_check_unique(uint16_t unit, adc_mode_t const  mode, adc_cfg_t const * const p_cfg)
{
	SSP_PARAMETER_NOT_USED(mode);
    if ((ADC_UNIT_MAX <= unit)
    || ((ADC_RESOLUTION_12_BIT != p_cfg->resolution)
         && (ADC_RESOLUTION_14_BIT != p_cfg->resolution))
    || ((ADC_ALIGNMENT_RIGHT != p_cfg->alignment) && (ADC_ALIGNMENT_LEFT != p_cfg->alignment))
    || ((ADC_ADD_FOUR < p_cfg->add_average_count)
            && (ADC_ADD_AVERAGE_TWO != p_cfg->add_average_count)
            && (ADC_ADD_AVERAGE_FOUR != p_cfg->add_average_count)
            && (ADC_ADD_AVERAGE_SIXTEEN != p_cfg->add_average_count))
    || ((ADC_CLEAR_AFTER_READ_OFF != p_cfg->clearing) && (ADC_CLEAR_AFTER_READ_ON != p_cfg->clearing))
    || ((ADC_TRIGGER_SYNC_ELC != p_cfg->trigger)
            && (ADC_TRIGGER_SOFTWARE != p_cfg->trigger)
            && (ADC_TRIGGER_ASYNC_EXT_TRG0 != p_cfg->trigger)))
   {
       return SSP_ERR_INVALID_ARGUMENT;
   }

    return SSP_SUCCESS;

}

/*******************************************************************************************************************//**
 * @} (end addtogroup ADC)
***********************************************************************************************************************/
#endif
