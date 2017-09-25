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
* File Name    : r_sci_i2c.h
* Description  : SCI HAL driver for Simple I2C interface.
***********************************************************************************************************************/


/******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup SIIC Simple I2C on SCI
 * @brief Driver for the Simple IIC on SCI.
 *
 * This module supports the SCI in I2C mode. It implements the following interfaces:
 *   - @ref I2C_API r_i2c_api.h
 * @{
**********************************************************************************************************************/

#ifndef R_SCI_SIIC_H
#define R_SCI_SIIC_H

#include "bsp_api.h"
#include "r_sci_i2c_cfg.h"
#include "r_i2c_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SCI_SIIC_MASTER_CODE_VERSION_MAJOR   (1)
#define SCI_SIIC_MASTER_CODE_VERSION_MINOR   (2)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern i2c_api_master_t const g_i2c_master_on_sci;
/** @endcond */

#endif // R_SCI_SIIC_H

/*******************************************************************************************************************//**
 * @} (end defgroup SIIC)
***********************************************************************************************************************/
