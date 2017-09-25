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
 * File Name    : sf_jpeg_decode.h
 * Description  : JPEG Decode Framework (SF_JPEG_DECODE) Module instance header file.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @defgroup SF_JPEG_DECODE JPEG Framework
 * @brief RTOS-integrated JPEG Framework.
 *
 * @{
 **********************************************************************************************************************/

#ifndef SF_JPEG_DECODE_H
#define SF_JPEG_DECODE_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "sf_jpeg_decode_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Version of code that implements the API defined in this file */
#define SF_JPEG_DECODE_CODE_VERSION_MAJOR (1)
#define SF_JPEG_DECODE_CODE_VERSION_MINOR (0)

 /**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const sf_jpeg_decode_api_t g_sf_jpeg_decode_on_sf_jpeg_decode;
/** @endcond */

/*******************************************************************************************************************//**
 * @} (end defgroup SF_JPEG_DECODE)
 **********************************************************************************************************************/

#endif /** SF_JPEG_DECODE_H */
