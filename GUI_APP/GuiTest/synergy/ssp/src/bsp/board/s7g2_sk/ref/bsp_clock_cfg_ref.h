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
* File Name    : bsp_clock_cfg_ref.h
* Description  : BSP clock configuration file for S7G2 device.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_CONFIG_S7G2SK
 * @defgroup BSP_CONFIG_S7G2SK_CLOCKS Build Time Configurations - Clock
 *
 * This file contains build-time clock configuration options. The BSP will use these macros to setup the MCU's clocks
 * for the user before main() is executed.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_CLOCK_CFG
#define BSP_CLOCK_CFG

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/** Clock source select (CKSEL). The chosen clock will be the base clock provided for the system clock and all
 * peripheral clocks. It is also used for the flash clock and the external bus clocks.
 *
 * Set the macro to one of the enumerations shown.
 *
 * <PRE>
 * Clock                                        -   Enumeration to use for macro
 * High Speed On-Chip Oscillator   (HOCO)       -   CGC_CLOCK_HOCO
 * Middle Speed On-Chip Oscillator (MOCO)       -   CGC_CLOCK_MOCO
 * Low Speed On-Chip Oscillator    (LOCO)       -   CGC_CLOCK_LOCO
 * Main Clock Oscillator                        -   CGC_CLOCK_MAIN_OSC
 * Sub-Clock Oscillator                         -   CGC_CLOCK_SUBCLOCK
 * PLL Circuit                                  -   CGC_CLOCK_PLL
 * </PRE>
 */
#define BSP_CFG_CLOCK_SOURCE                        (CGC_CLOCK_PLL)

/** XTAL - Input clock frequency in Hz */
#define BSP_CFG_XTAL_HZ                             (24000000)

/** The HOCO can operate at several different frequencies. Choose which one using the macro below. The frequency
 * used out of reset depends upon the OFS1.HOCOFRQ0 bits.
 *
 * <PRE>
 * Available frequency settings:
 * 0 = 16MHz
 * 1 = 18MHz
 * 2 = 20MHz
 * </PRE>
 */
#define BSP_CFG_HOCO_FREQUENCY                      (0)

/** PLL clock source (PLLSRCEL). Choose which clock source to input to the PLL circuit.
 *
 * <PRE>
 * Available clock sources:     -   Enumeration to use for macro
 * 0 = Main clock (default)     -   CGC_CLOCK_MAIN_OSC
 * 1 = HOCO                     -   CGC_CLOCK_HOCO
 * </PRE>
 */
#define BSP_CFG_PLL_SOURCE                          (CGC_CLOCK_MAIN_OSC)

/** PLL Input Frequency Division Ratio Select (PLIDIV).
 *
 * Available divisors = - /1 (no division), - /2, - /3
 *
 * @note Set macro definition to 'CGC_PLL_DIV_' + your divider selection.
 */
#define BSP_CFG_PLL_DIV                             (CGC_PLL_DIV_2)

/** PLL Frequency Multiplication Factor Select (PLLMUL).
 *
 * Available multipliers = x10.0 to x30.0 in 0.5 increments (e.g. 10.0, 10.5, 11.0, 11.5, ..., 29.0, 29.5, 30.0)
 */
#define BSP_CFG_PLL_MUL                             (20.0)

/** System Clock Divider (ICK).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'CGC_SYS_CLOCK_DIV_' + your divider selection.
 */
#define BSP_CFG_ICK_DIV                             (CGC_SYS_CLOCK_DIV_1)

/** Peripheral Module Clock A Divider (PCKA).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'CGC_SYS_CLOCK_DIV_' + your divider selection.
 */
#define BSP_CFG_PCKA_DIV                            (CGC_SYS_CLOCK_DIV_2)

/** Peripheral Module Clock B Divider (PCKB).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'CGC_SYS_CLOCK_DIV_' + your divider selection.
 */
#define BSP_CFG_PCKB_DIV                            (CGC_SYS_CLOCK_DIV_4)

/** Peripheral Module Clock C Divider (PCKC).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'CGC_SYS_CLOCK_DIV_' + your divider selection.
 */
#define BSP_CFG_PCKC_DIV                            (CGC_SYS_CLOCK_DIV_4)

/** Peripheral Module Clock D Divider (PCKD).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'CGC_SYS_CLOCK_DIV_' + your divider selection.
 */
#define BSP_CFG_PCKD_DIV                            (CGC_SYS_CLOCK_DIV_2)

/** External Bus Clock Divider (BCLK).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'CGC_SYS_CLOCK_DIV_' + your divider selection.
 */
#define BSP_CFG_BCK_DIV                             (CGC_SYS_CLOCK_DIV_2)

/** Flash IF Clock Divider (FCK).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'CGC_SYS_CLOCK_DIV_' + your divider selection.
 */
#define BSP_CFG_FCK_DIV                             (CGC_SYS_CLOCK_DIV_4)

/** USB Clock Divider Select.
 *
 * Available divisors = /3, /4, /5
 *
 * @note Set macro definition to 'CGC_USB_CLOCK_DIV_' + your divider selection.
 */
#define BSP_CFG_UCK_DIV                             (CGC_USB_CLOCK_DIV_5)

/** Configure BCLK output pin
 *
 * Available options:
 * - 0 = No output
 * - 1 = BCK frequency
 * - 2 = BCK/2 frequency
 *
 * @note This macro is only effective when the external bus is enabled
 */
#define BSP_CFG_BCLK_OUTPUT                         (1)

/** Configure SDCLK output pin
 *
 * Available options:
 * - 0 = No output
 * - 1 = BCK frequency
 *
 * @note This macro is only effective when the external bus is enabled
 */
#define BSP_CFG_SDCLK_OUTPUT                        (1)

/** @} (end defgroup BSP_CONFIG_S7G2SK_CLOCKS) */

#endif /* BSP_CLOCK_CFG */
