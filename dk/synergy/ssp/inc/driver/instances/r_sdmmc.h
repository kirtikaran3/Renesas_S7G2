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
 * File Name    : r_sdmmc.h
 * Description  : SDMMC driver header file.
 **********************************************************************************************************************/

#ifndef R_SDMMC_H
#define R_SDMMC_H

/*******************************************************************************************************************//**
 * @file r_sdmmc.h
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_sdmmc_cfg.h"
#include "r_sdmmc_api.h"


/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup SDMMC SDMMC
 * @brief Driver for the SD/MMC Host Interface (SDHI).
 *
 * SDMMC Drivers to access SD and MMC memory devices, including eMMC.
 * @{
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SDMMC_CODE_VERSION_MAJOR ((uint8_t)1)
#define SDMMC_CODE_VERSION_MINOR ((uint8_t)0)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const sdmmc_api_t g_sdmmc_on_sdmmc;
/** @endcond */
#endif // R_SDMMC_H

/*******************************************************************************************************************//**
 * @} (end addtogroup SDMMC)
 **********************************************************************************************************************/
