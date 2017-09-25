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
* File Name    : bsp.h
* Description  : Includes and API function available for this board.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_Boards
 * @defgroup BSP_BOARD_DK2M BSP for the DK-S7G2 Board
 * @brief BSP for the DK-S7G2 Board
 *
 * The DK-S7G2 is a development kit for the Renesas SynergyTM S7G2 microcontroller in a BGA224 package. It contains two
 * boards: the Main Board and the Breakout Board. The boards together provide easy-to-access interfaces and connectors
 * to the peripherals of the S7G2 for application development and testing: TFTLCD graphics, Ethernet, USB Host and
 * Device, SEGGER J-Link on-board debug, and four Pmod connectors, an SD card, and multiple LED indicators.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_H_
#define BSP_H_

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_BOARD_DK2M
 * @defgroup BSP_CONFIG_DK2M Build Time Configurations
 *
 * The BSP has multiple header files that contain build-time configuration options. Currently there are header files to
 * configure the following settings:
 *
 * - General BSP Options
 * - Clocks
 * - Interrupts
 * - Pin Configuration
 *
 * @{
 **********************************************************************************************************************/

/** @} (end defgroup BSP_CONFIG_DK2M) */
/* BSP MCU Specific Includes. */
#include "../../src/bsp/mcu/s7g2/bsp_mcu_info.h"

/* BSP Board Specific Includes. */
#include "../../src/bsp/board/s7g2_dk/bsp_init.h"
#include "../../src/bsp/board/s7g2_dk/bsp_sdram.h"
#include "../../src/bsp/board/s7g2_dk/bsp_qspi.h"
#include "../../src/bsp/board/s7g2_dk/bsp_leds.h"



/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

/** @} (end defgroup BSP_BOARD_DK2M) */

#endif /* BSP_H_ */
