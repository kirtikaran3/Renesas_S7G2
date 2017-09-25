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

/***********************************************************************************************************************
 * File Name    : r_sci_spi.h
 * Description  : SCI HAL driver for simple SPI interface.
 **********************************************************************************************************************/



#ifndef R_SCI_SPI_H
#define R_SCI_SPI_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_spi_api.h"

/*****************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup SCI_SPI Simple SPI on SCI
 * @brief Driver for the Simple SPI on SCI.
 *
 * This module supports simple SPI serial communication using the microcontroller's SCI peripheral.
 * The Interface is defined in r_spi_api.h.
 * This module implements @ref SPI_API.
 * @{
 ********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SCI_SPI_CODE_VERSION_MAJOR (1U)
#define SCI_SPI_CODE_VERSION_MINOR (3U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** Filled in Interface API structure for this Instance. */
extern const spi_api_t g_spi_on_sci;

/*******************************************************************************************************************//**
 * @} (end defgroup SCI_SPI)
 **********************************************************************************************************************/
#endif /* R_SCI_SPI_H */


