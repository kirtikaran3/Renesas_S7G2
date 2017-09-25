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
* File Name    : bsp_pin_cfg_ref.h
* Description  : This configuration header file has information for setting up pins out of reset.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_CONFIG_S7G2SK
 * @defgroup BSP_CONFIG_S7G2SK_PINS Build Time Configurations - Pin Settings
 *
 * This file contains an array of pin configurations. During start-up, and before main() is executed, the BSP will
 * iterate over this array and initialize the MCU's port pins based on the settings in the array.
 *
 * @note It is recommended to use the ISDE Pin Configurator to create this file.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_PIN_CFG_REF_H_
#define BSP_PIN_CFG_REF_H_

/***********************************************************************************************************************
Pin Configuration Table
***********************************************************************************************************************/

/* This conditional makes sure that this table is only allocated once. Without this check the table would be allocated
 * in every module that includes this header file. */
#if defined(BSP_PIN_ALLOCATE)

/* Used for getting IOPORT types. */
#include "r_ioport.h"

/** Array of pin configurations. */
const ioport_pin_cfg_t g_bsp_pin_cfg_data[] =
{
    {
        .pin     = IOPORT_PORT_00_PIN_00,
        .pin_cfg = (IOPORT_CFG_ANALOG_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_01,
        .pin_cfg = (IOPORT_CFG_ANALOG_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_02,
        .pin_cfg = (IOPORT_CFG_ANALOG_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_03,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_04,
        .pin_cfg = (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_05,
        .pin_cfg = (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_06,
        .pin_cfg = (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_07,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_08,
        .pin_cfg = (IOPORT_CFG_ANALOG_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_09,
        .pin_cfg = (IOPORT_CFG_ANALOG_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_10,
        .pin_cfg = (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_14,
        .pin_cfg = (IOPORT_CFG_ANALOG_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_00_PIN_15,
        .pin_cfg = (IOPORT_CFG_ANALOG_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_RSPI),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_RSPI),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_RSPI),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_RSPI),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_07,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_08,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_DEBUG),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_09,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_DEBUG),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_10,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_DEBUG),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_11,
        .pin_cfg = (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_12,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_13,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_14,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_15,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_00,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_01,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_03,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CTSU),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CTSU),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CTSU),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_07,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CTSU),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_12,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_13,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_DEBUG),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_03,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_07,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_08,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_09,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_10,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_11,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_12,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_13,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_14,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_15,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_00,
        .pin_cfg = (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CAN),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CAN),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_07,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_FS),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_08,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CTSU),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_09,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CTSU),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_10,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_11,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_12,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_13,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_14,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CTSU),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_15,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CTSU),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_06,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_07,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_11,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_12,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_13,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_00,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_01,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_02,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_03,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_04,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_05,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_07,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_08,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_09,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_10,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_11,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_12,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_13,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_14,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_15,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI1_3_5_7_9),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_07,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI1_3_5_7_9),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_00,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_01,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_05,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_INPUT),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_06,
        .pin_cfg = (IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_09_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_09_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_09_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_09_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_09_PIN_07,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_09_PIN_08,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_10_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_10_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_10_PIN_08,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_10_PIN_09,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_10_PIN_10,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_LCD_GRAPHICS),
    },
    {
        .pin     = IOPORT_PORT_11_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_HS),
    },
    {
        .pin     = IOPORT_PORT_11_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_HS),
    },
};

/** Has pointer to pin configuration array and number of pins to be configured. */
const ioport_cfg_t g_bsp_pin_cfg =
{
    .number_of_pins = sizeof(g_bsp_pin_cfg_data)/sizeof(ioport_pin_cfg_t),
    .p_pin_cfg_data = &g_bsp_pin_cfg_data[0],
};

#endif /* BSP_PIN_ALLOCATE */

/** @} (end defgroup BSP_CONFIG_S7G2SK_PINS) */

#endif /* BSP_PIN_CFG_REF_H_ */
