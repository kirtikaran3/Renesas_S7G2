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
 * File Name    : r_crc.c
 * Description  : HAL API code for the CRC module
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_crc.h"
#include "r_crc_private.h"
#include "r_crc_private_api.h"
#include <string.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define CRC_SNOOP_MAX_CHANNEL (10)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t s_crc_version =
{
	.api_version_minor  = CRC_API_VERSION_MINOR,
	.api_version_major  = CRC_API_VERSION_MAJOR,
    .code_version_major = CRC_CODE_VERSION_MAJOR,
    .code_version_minor = CRC_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/** Filled in Interface API structure for this Instance. */
const crc_api_t g_crc_on_crc = 
{
    .open           = R_CRC_Open,
    .close          = R_CRC_Close,
    .crcResultGet   = R_CRC_CalculatedValueGet,
    .snoopEnable    = R_CRC_SnoopEnable,
    .snoopDisable   = R_CRC_SnoopDisable,
    .snoopCfg       = R_CRC_SnoopCfg,
    .calculate      = R_CRC_Calculate,
    .versionGet     = R_CRC_VersionGet
};

/*******************************************************************************************************************//**
 * @addtogroup CRC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Open the CRC driver module
 *
 *  Implements crc_api_t::open
 *
 * Open the CRC driver module and initialize the block according to the passed-in configuration structure.
 *
 * @retval SSP_SUCCESS             Configuration was successful.
 * @retval SSP_ERR_ASSERTION       p_ctrl or p_cfg is NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_CRC_Open (crc_ctrl_t * const p_ctrl, crc_cfg_t const * const p_cfg)
{
#if CRC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_cfg);
#endif

    /* Store these configurations for later calculation */
    p_ctrl->bit_order  = p_cfg->bit_order;
    p_ctrl->polynomial = p_cfg->polynomial;

    /* Power on CRC */
    HW_CRC_PowerOn();

    /* Disable snooping */
    HW_CRC_SnoopDisable();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Close the CRC module driver.
 *
 *  Implements crc_api_t::close
 *
 * @retval SSP_SUCCESS             Configuration was successful.
 * @retval SSP_ERR_ASSERTION       p_ctrl is NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_CRC_Close (crc_ctrl_t * const p_ctrl)
{
#if CRC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    memset(p_ctrl, 0, sizeof(crc_ctrl_t));

    /* Power off CRC */
    HW_CRC_PowerOff();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Return the current calculated value.
 *
 *  Implements crc_api_t::crcResultGet
 *
 * CRC calculation operates on a running value. This function returns the current calculated value.
 *
 * @retval SSP_SUCCESS             Return of calculated value successful.
 * @retval SSP_ERR_INVALID_ARG     Either bitOrder or polynomial was not a valid value.
 *
 **********************************************************************************************************************/
ssp_err_t R_CRC_CalculatedValueGet (crc_ctrl_t * const p_ctrl, uint32_t * calculatedValue)
{
#if CRC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(calculatedValue);
#endif

    switch (p_ctrl->polynomial)
    {
        case CRC_POLYNOMIAL_CRC_8:
            *calculatedValue = (uint32_t) HW_CRC_8bitCalculatedValueGet();
            break;

        case CRC_POLYNOMIAL_CRC_16:
        case CRC_POLYNOMIAL_CRC_CCITT:
            *calculatedValue = (uint32_t) HW_CRC_16bitCalculatedValueGet();
            break;

        case CRC_POLYNOMIAL_CRC_32:
        case CRC_POLYNOMIAL_CRC_32C:
            *calculatedValue = HW_CRC_32bitCalculatedValueGet();
            break;
        default:
            break;
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable snooping.
 *
 *  Implements crc_api_t::snoopEnable
 *
 * @retval SSP_SUCCESS             Snoop enabled.
 *
 **********************************************************************************************************************/
ssp_err_t R_CRC_SnoopEnable (crc_ctrl_t * const p_ctrl, uint32_t crc_seed)
{
#if CRC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    switch (p_ctrl->polynomial)
    {
        case CRC_POLYNOMIAL_CRC_8:
            HW_CRC_8bitCalculatedValueSet(0xFF & crc_seed);
            break;

        case CRC_POLYNOMIAL_CRC_16:
        case CRC_POLYNOMIAL_CRC_CCITT:
            HW_CRC_16bitCalculatedValueSet(0xFFFF & crc_seed);
            break;

        case CRC_POLYNOMIAL_CRC_32:
        case CRC_POLYNOMIAL_CRC_32C:
            HW_CRC_32bitCalculatedValueSet(crc_seed);
            break;
        default:
            break;
    }

    HW_CRC_SnoopEnable();
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Disable snooping.
 *
 *  Implements crc_api_t::snoopDisable
 *
 * @retval SSP_SUCCESS             Snoop disabled.
 *
 **********************************************************************************************************************/
ssp_err_t R_CRC_SnoopDisable (crc_ctrl_t * const p_ctrl)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);
    
#if CRC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    HW_CRC_SnoopDisable();
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure the snoop channel and direction.
 *
 *  Implements crc_api_t::snoopCfg
 *
 * The CRC calculator can operate on reads and writes over any of the first ten SCI channels.
 * For example, if set to channel 0, transmit, every byte written out SCI channel 0 is also
 * sent to the CRC calculator as if the value was explicitly written directly to the CRC calculator.
 *
 * @retval SSP_SUCCESS             Snoop configured successfully.
 * @retval SSP_ERR_ASSERTION       p_snoop_cfg is NULL.
 * @retval SSP_ERR_INVALID_ARG     Either the channel was out of range or the direction was an invalid value.
 *
 **********************************************************************************************************************/
ssp_err_t R_CRC_SnoopCfg (crc_ctrl_t * const p_ctrl, crc_snoop_cfg_t * const p_snoop_cfg)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

#if CRC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_snoop_cfg);
    SSP_ASSERT(CRC_SNOOP_MAX_CHANNEL > p_snoop_cfg->snoop_channel);
#endif

    HW_CRC_SnoopChannelSet(p_snoop_cfg->snoop_channel, p_snoop_cfg->snoop_direction);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Perform a CRC calculation on a block of 8-bit data.
 *
 *  Implements crc_api_t::calculate
 *
 * This function performs a CRC calculation on an array of 8-bit values and returns an 8-bit calculated value
 *
 * @retval SSP_SUCCESS             Calculation successful.
 * @retval SSP_ERR_ASSERTION       Either p_ctrl, inputBuffer, or calculatedValue is NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_CRC_Calculate (crc_ctrl_t * const p_ctrl,
                           void               * inputBuffer,
                           uint32_t           length,
                           uint32_t           crc_seed,
                           uint32_t           * calculatedValue)
{
    uint32_t i;

#if CRC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(inputBuffer);
    SSP_ASSERT(calculatedValue);
#endif

    HW_CRC_BitorderSet(p_ctrl->bit_order);
    HW_CRC_PolynomialSet(p_ctrl->polynomial);

    /* Write each element of the inputBuffer to the CRC Data Input Register. Each write to the
     * Data Input Register generates a new calculated value in the Data Output Register.
     */
    switch (p_ctrl->polynomial)
    {
        case CRC_POLYNOMIAL_CRC_8:
        {
            uint8_t * p_data = (uint8_t *) inputBuffer;

            HW_CRC_8bitCalculatedValueSet(0xFF & crc_seed);

            for (i = 0; i < length; i++)
            {
                HW_CRC_8bitInputWrite(*p_data++);
            }

            /* Return the calculated value */
            *calculatedValue = (uint8_t) HW_CRC_8bitCalculatedValueGet();
            break;
        }

        case CRC_POLYNOMIAL_CRC_16:
        case CRC_POLYNOMIAL_CRC_CCITT:
        {
            uint8_t * p_data = (uint8_t *) inputBuffer;

            HW_CRC_16bitCalculatedValueSet(0xFFFF & crc_seed);

            for (i = 0; i < length; i++)
            {
                HW_CRC_8bitInputWrite(*p_data++);
            }

            /* Return the calculated value */
            *calculatedValue = (uint32_t) HW_CRC_16bitCalculatedValueGet();
            break;
        }

        case CRC_POLYNOMIAL_CRC_32:
        case CRC_POLYNOMIAL_CRC_32C:
        {
            uint32_t * p_data = (uint32_t *) inputBuffer;

            HW_CRC_32bitCalculatedValueSet(crc_seed);

            for (i = 0; i < length / 4; i++)
            {
                HW_CRC_32bitInputWrite(*p_data++);
            }

            /* Return the calculated value */
            *calculatedValue = HW_CRC_32bitCalculatedValueGet();
            break;
        }
        default:
            break;
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      Get the driver version based on compile time macros.
 *
 *   Implements crc_api_t::versionGet
 *
 * @retval     SSP_SUCCESS          Successful close.
 * @retval     SSP_ERR_ASSERTION    p_version is NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_CRC_VersionGet (ssp_version_t * const p_version)
{
#if CRC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_version);
#endif

    p_version->version_id = s_crc_version.version_id;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup CRC)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/


