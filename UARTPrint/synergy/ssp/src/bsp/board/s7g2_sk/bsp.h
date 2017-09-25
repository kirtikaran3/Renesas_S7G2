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
 * @defgroup BSP_BOARD_S7G2SK BSP for the SK-S7G2 Board
 * @brief BSP for the SK-S7G2 Board
 *
 * The SK-S7G2 is a starter kit for the Renesas SynergyTM S7G2 microcontroller in a LQFP176 package.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_H_
#define BSP_H_

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* BSP Common Includes. */
#include "../../src/bsp/mcu/all/bsp_common.h"
#include "../../inc/ssp_common_api.h"
/*******************************************************************************************************************//**
 * @ingroup BSP_BOARD_S7G2SK
 * @defgroup BSP_CONFIG_S7G2SK Build Time Configurations
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
#include "bsp_clock_cfg.h"
#include "bsp_irq_cfg.h"
/** @} (end defgroup BSP_CONFIG_S7G2SK) */
#if defined(__GNUC__)
/* CMSIS-CORE currently generates 2 warnings when compiling with GCC. One in core_cmInstr.h and one in core_cm4_simd.h.
 * We are not modifying these files so we will ignore these warnings temporarily. */
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
/* CMSIS-CORE Renesas Device Files. */
#include "../../src/bsp/cmsis/Device/RENESAS/S7G2/Include/R7FS7G2x.h"
#include "../../src/bsp/cmsis/Device/RENESAS/S7G2/Include/system_S7G2.h"
/* CMSIS-CORE Standard Files. */
#include "core_cm4.h"
#include "core_cmInstr.h"
#include "core_cmFunc.h"
#include "core_cm4_simd.h"
#if defined(__GNUC__)
/* Restore warning settings for 'conversion' and 'sign-conversion' to as specified on command line. */
#pragma GCC diagnostic pop
#endif
/* BSP MCU Specific Includes. */
#include "../../src/bsp/mcu/s7g2/bsp_mcu_info.h"
#include "../../src/bsp/mcu/s7g2/bsp_register_protection.h"
#include "../../src/bsp/mcu/s7g2/bsp_locking.h"
#include "../../src/bsp/mcu/s7g2/bsp_hw_locks.h"
#include "../../src/bsp/mcu/s7g2/bsp_irq.h"
#include "../../src/bsp/mcu/s7g2/bsp_group_irq.h"
#include "../../src/bsp/mcu/s7g2/bsp_clocks.h"
/* BSP Dependencies. */
#include "../../src/driver/r_ioport/hw/target/s7g2/hw_ioport_s7g2.h"
#include "r_ioport_api.h"
/* BSP MCU Specific Includes that depend on pin configuration. */
#include "../../src/bsp/mcu/s7g2/bsp_vbatt.h"
/* BSP Common Includes (Other than bsp_common.h) */
#include "../../src/bsp/mcu/all/bsp_common_leds.h"
#include "../../src/bsp/mcu/all/bsp_delay.h"
/* BSP Board Specific Includes. */
#include "../../src/bsp/board/s7g2_sk/bsp_init.h"
#include "../../src/bsp/board/s7g2_sk/bsp_leds.h"

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

void        R_BSP_RegisterProtectEnable(bsp_reg_protect_t regs_to_protect);
void        R_BSP_RegisterProtectDisable(bsp_reg_protect_t regs_to_protect);
void        R_BSP_SoftwareLockInit(bsp_lock_t * p_lock);
ssp_err_t   R_BSP_SoftwareLock(bsp_lock_t * p_lock);
void        R_BSP_SoftwareUnlock(bsp_lock_t * p_lock);
ssp_err_t   R_BSP_HardwareLock(bsp_hw_lock_t hw_resource);
void        R_BSP_HardwareUnlock(bsp_hw_lock_t hw_resource);
ssp_err_t   R_BSP_GroupIrqWrite(bsp_grp_irq_t irq,  void (* p_callback)(bsp_grp_irq_t irq));
void        R_BSP_IrqStatusClear(IRQn_Type irq);
ssp_err_t   R_BSP_LedsGet(bsp_leds_t * p_leds);
void        R_BSP_SoftwareDelay(uint32_t delay, bsp_delay_units_t units);
ssp_err_t   R_BSP_VersionGet(ssp_version_t * p_version);

/** @} (end defgroup BSP_BOARD_S7G2SK) */

#endif /* BSP_H_ */
