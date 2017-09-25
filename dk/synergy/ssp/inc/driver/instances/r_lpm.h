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
 * File Name    : r_lpm.h
 * Description  : LPM HAP API header file
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup LPM LPM
 * @brief Driver for Low Power Modes.
 *
 * The LPM (Low Power Mode) Driver provides several functions for controlling power consumption including stopping
 * modules, selecting the operating mode, configuring low power modes, and transitioning to low power modes.
 * The LPM driver supports configuration of MCU operating modes and MCU low power modes using the LPM hardware
 * peripheral. The LPM driver supports operating modes low-voltage, low-speed, middle-speed, high-speed, and
 * suboscillator mode.
 *
 * The LPM driver supports low power modes deep standby, standby, sleep, and snooze. The LPM
 * driver supports reducing power consumption when in deep standby mode via internal power supply control and
 * reseting the states of IO ports. The LPM driver supports disabling and enabling of the MCU's other hardware
 * peripherals.
 *
 * @note Not all operating modes are available on all MCU's. Not all low power modes are available on all MCU's.
 * @{
 **********************************************************************************************************************/

#ifndef R_LPM_H
#define R_LPM_H

#if defined(__GNUC__) && defined(__ARM_EABI__)
/*LDRA_INSPECTED 293 S */
#define INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define INLINE_ATTRIBUTE
#endif

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_lpm_cfg.h"
#include "r_lpm_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define LPM_CODE_VERSION_MAJOR (2)
#define LPM_CODE_VERSION_MINOR (2)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const lpm_api_t g_lpm_on_lpm;
/** @endcond */


#endif /* R_LPM_H */

/*******************************************************************************************************************//**
 * @} (end defgroup LPM_DRV)
 **********************************************************************************************************************/
