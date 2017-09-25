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
 * File Name    : r_fmi.c
 * Description  : FMI HAL API
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_fmi.h"
#include "r_fmi_private_api.h"
#include "hw/hw_fmi_private.h"
#include "hw/common/hw_fmi_common.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

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
static const ssp_version_t s_fmi_version =
{
    .api_version_minor  = FMI_API_VERSION_MINOR,
    .api_version_major  = FMI_API_VERSION_MAJOR,
    .code_version_major = FMI_CODE_VERSION_MAJOR,
    .code_version_minor = FMI_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
const fmi_api_t g_fmi_on_fmi =
{
	.productInfoGet	= R_FMI_ProductInfoGet,
    .versionGet		= R_FMI_VersionGet
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @addtogroup FMI
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief		Get pointer to Factory MCU Information first record.
 * @retval		SSP_SUCCESS          Caller's pointer set to Product Information Record.
 * @retval		SSP_ERR_ASSERTION    Caller's pointer is NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_FMI_ProductInfoGet(fmi_product_info_t ** pp_product_info)
{
#if FMI_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(pp_product_info);
#endif
    uint32_t	product_info_address;

	product_info_address = HW_FMI_RecordLocate(FMI_PRODUCT_INFORMATION);
	*pp_product_info = (fmi_product_info_t *)product_info_address;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      Get the driver version based on compile time macros.
 *
 * @retval     SSP_SUCCESS          Caller's structure written.
 * @retval     SSP_ERR_ASSERTION    Caller's pointer is NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_FMI_VersionGet(ssp_version_t * const p_version)
{
#if FMI_CFG_PARAM_CHECKING_ENABLE
	SSP_ASSERT(p_version);
#endif

	p_version->version_id = s_fmi_version.version_id;

	return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end defgroup FMI)
 **********************************************************************************************************************/
