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
 * File Name    : r_fmi_api.h
 * Description  : FMI interface
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup FMI_API FMI Interface
 *
 * @brief Interface for reading on-chip factory information.
 *
 * @section FMI_API_SUMMARY Summary
 * The FMI (Factory MCU Information) module provides a function for reading the Product Information record.
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * FMI Interface description: @ref HALFMIInterface
 * @{
 **********************************************************************************************************************/
#ifndef DRV_FMI_API_H
#define DRV_FMI_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Register definitions, common services and error codes. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define FMI_API_VERSION_MAJOR (1)
#define FMI_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef struct st_fmi_header
{
	uint32_t contents :8; // [0:7]
	uint32_t variant :8;  // [8:15]
	uint32_t count :8;    // [16:23]
	uint32_t minor :4;    // [24:27]
	uint32_t major :4;    // [28:31]
} fmi_header_t;

typedef struct st_fmi_product_info
{
	fmi_header_t	header;
	uint8_t			unique_id[16];
	uint8_t			product_name[16];    /* No guarantee of null terminator. */
	uint8_t			product_marking[16]; /* No guarantee of null terminator. */
	struct
	{
		uint32_t mask_revision :8; /*  [0:7]  */
		uint32_t pin_count :10;    /*  [8:17] */
		uint32_t pkg_type :3;      /* [18:20] */
		uint32_t temp_range :3;    /* [21:23] */
		uint32_t quality_code :3;  /* [24:26] */
		uint32_t reserved :5;      /* [27:32] */
	};
	struct
	{
		uint32_t max_freq :8;       /* [0:7] */
		uint32_t reserved1;
	};
} fmi_product_info_t;

/** fmi driver structure. General fmi functions implemented at the HAL layer will follow this API. */
typedef struct st_fmi_api
{
    /** Get the product information record address into caller's pointer.
     * @par Implemented as
     * - R_FMI_ProductInfoGet()
     **/
    ssp_err_t (* productInfoGet)(fmi_product_info_t ** pp_product_info);

    /** Get the driver version based on compile time macros.
     * @par Implemented as
     * - R_FMI_VersionGet()
     **/
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);
} fmi_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_fmi_instance
{
    fmi_api_t const * p_api;     ///< Pointer to the API structure for this instance
} fmi_instance_t;

#endif /* DRV_FMI_API_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup FMI_API)
 **********************************************************************************************************************/
