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
* File Name    : bsp_rom_registers.c
* Description  : Defines MCU registers that are in ROM (e.g. OFS) and must be set at compile-time. All registers
*                can be set using bsp_cfg.h.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_S7G2
 * @defgroup BSP_MCU_ROM_REGISTER ROM Registers
 *
 * Defines MCU registers that are in ROM (e.g. OFS) and must be set at compile-time. All registers can be set
 * using bsp_cfg.h.
 *
 * @{
 **********************************************************************************************************************/

/** @} (end defgroup BSP_MCU_ROM_REGISTER) */

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

#if defined(BSP_MCU_GROUP_S7G2)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** OR in the HOCO frequency setting from bsp_clock_cfg.h with the OFS1 setting from bsp_cfg.h. */
#define BSP_ROM_REG_OFS1_SETTING        ((BSP_CFG_ROM_REG_OFS1 & 0xFFFFF9FF) | (BSP_CFG_HOCO_FREQUENCY << 9))

/** Build up SECMPUAC register based on MPU settings. */
#define BSP_ROM_REG_MPU_CONTROL_SETTING     ((0xFFFFFCF0) | \
                                             (BSP_CFG_ROM_REG_MPU_PC0_ENABLE << 8) | \
                                             (BSP_CFG_ROM_REG_MPU_PC1_ENABLE << 9) | \
                                             (BSP_CFG_ROM_REG_MPU_REGION0_ENABLE) | \
                                             (BSP_CFG_ROM_REG_MPU_REGION1_ENABLE << 1) | \
                                             (BSP_CFG_ROM_REG_MPU_REGION2_ENABLE << 2) | \
                                             (BSP_CFG_ROM_REG_MPU_REGION3_ENABLE << 3)   \
                                            )

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/** ROM registers defined here. Some have masks to make sure reserved bits are set appropriately. */
BSP_DONT_REMOVE static const uint32_t g_bsp_rom_registers[] BSP_PLACE_IN_SECTION(BSP_SECTION_ROM_REGISTERS) =
{
    BSP_CFG_ROM_REG_OFS0,
    BSP_ROM_REG_OFS1_SETTING,
    (BSP_CFG_ROM_REG_MPU_PC0_START      & 0xFFFFFFFC),
    (BSP_CFG_ROM_REG_MPU_PC0_END        | 0x00000003),    (BSP_CFG_ROM_REG_MPU_PC1_START      & 0xFFFFFFFC),    (BSP_CFG_ROM_REG_MPU_PC1_END        | 0x00000003),    (BSP_CFG_ROM_REG_MPU_REGION0_START  & 0x00FFFFFC),    ((BSP_CFG_ROM_REG_MPU_REGION0_END   & 0x00FFFFFF) | 0x00000003),    (BSP_CFG_ROM_REG_MPU_REGION1_START  & 0xFFFFFFFC),    (BSP_CFG_ROM_REG_MPU_REGION1_END    | 0x00000003),    ((BSP_CFG_ROM_REG_MPU_REGION2_START & 0x407FFFFC) | 0x40000000),    ((BSP_CFG_ROM_REG_MPU_REGION2_END   & 0x407FFFFC) | 0x40000003),    ((BSP_CFG_ROM_REG_MPU_REGION3_START & 0x407FFFFC) | 0x40000000),    ((BSP_CFG_ROM_REG_MPU_REGION3_END   & 0x407FFFFC) | 0x40000003),
    BSP_ROM_REG_MPU_CONTROL_SETTING
};

#endif


