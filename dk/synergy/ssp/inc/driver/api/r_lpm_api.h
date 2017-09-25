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
 * File Name    : r_lpm_api.h
 * Description  : LPM interface
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup LPM_API Low Power Modes Interface
 * @brief Interface for accessing low power modes.
 *
 * @section LPM_API_SUMMARY Summary
 * This section defines the API for the LPM (Low Power Mode) Driver.
 * The LPM Driver provides several functions for controlling power consumption including stopping
 * modules, selecting the operating mode, configuring low power modes, and transitioning to low power modes.
 * The LPM driver supports configuration of MCU operating modes and mcu low power modes using the LPM hardware
 * peripheral. The LPM driver supports operating modes low-voltage, low-speed, middle-speed, high-speed, and suboscillator
 * mode. The LPM driver supports low power modes deep standby, standby, sleep, and snooze. The LPM
 * driver supports reducing power consumption when in deep standby mode via internal power supply control and
 * reseting the states of IO ports. The LPM driver supports disabling and enabling of the MCU's other hardware
 * peripherals.
 * @note Not all operating modes are available on all MCUs. Not all low power modes are available on all MCUs.
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * LPM Interface description: @ref HALLPMInterface
 *
 * @{
 **********************************************************************************************************************/
#ifndef DRV_LPM_API_H
#define DRV_LPM_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Register definitions, common services and error codes. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define LPM_API_VERSION_MAJOR (2)
#define LPM_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Deep Standby Interrupt Edge */
typedef enum st_lpm_cancel_request_edge
{
    LPM_CANCEL_REQUEST_EDGE_FALLING,     ///< A cancel request is generated at a falling edge
    LPM_CANCEL_REQUEST_EDGE_RISING,      ///< A cancel request is generated at a rising edge
} lpm_cancel_request_edge_t;

/** Deep Standby pins and signals */
typedef enum st_lpm_deep_standby
{
    LPM_DEEP_STANDBY_IRQ0_DS      = 0x00UL,       ///< IRQ0-DS Pin
    LPM_DEEP_STANDBY_IRQ1_DS      = 0x00UL + 1UL, ///< IRQ1-DS Pin
    LPM_DEEP_STANDBY_IRQ2_DS      = 0x00UL + 2UL, ///< IRQ2-DS Pin
    LPM_DEEP_STANDBY_IRQ3_DS      = 0x00UL + 3UL, ///< IRQ3-DS Pin
    LPM_DEEP_STANDBY_IRQ4_DS      = 0x00UL + 4UL, ///< IRQ4-DS Pin
    LPM_DEEP_STANDBY_IRQ5_DS      = 0x00UL + 5UL, ///< IRQ5-DS Pin
    LPM_DEEP_STANDBY_IRQ6_DS      = 0x00UL + 6UL, ///< IRQ6-DS Pin
    LPM_DEEP_STANDBY_IRQ7_DS      = 0x00UL + 7UL, ///< IRQ7-DS Pin

    LPM_DEEP_STANDBY_IRQ8_DS      = 0x10UL,       ///< IRQ8-DS Pin
    LPM_DEEP_STANDBY_IRQ9_DS      = 0x10UL + 1UL, ///< IRQ9-DS Pin
    LPM_DEEP_STANDBY_IRQ10_DS     = 0x10UL + 2UL, ///< IRQ10-DS Pin
    LPM_DEEP_STANDBY_IRQ11_DS     = 0x10UL + 3UL, ///< IRQ11-DS Pin
    LPM_DEEP_STANDBY_IRQ12_DS     = 0x10UL + 4UL, ///< IRQ12-DS Pin
    LPM_DEEP_STANDBY_IRQ13_DS     = 0x10UL + 5UL, ///< IRQ13-DS Pin
    LPM_DEEP_STANDBY_IRQ14_DS     = 0x10UL + 6UL, ///< IRQ14-DS Pin
    LPM_DEEP_STANDBY_IRQ15_DS     = 0x10UL + 7UL, ///< IRQ15-DS Pin

    LPM_DEEP_STANDBY_LVD1         = 0x20UL,       ///< LVD1
    LPM_DEEP_STANDBY_LVD2         = 0x20UL + 1UL, ///< LVD2
    LPM_DEEP_STANDBY_RTC_INTERVAL = 0x20UL + 2UL, ///< RTC Interval Interrupt
    LPM_DEEP_STANDBY_RTC_ALARM    = 0x20UL + 3UL, ///< RTC Alarm Interrupt
    LPM_DEEP_STANDBY_NMI          = 0x20UL + 4UL, ///< NMI Pin

    LPM_DEEP_STANDBY_USBFS        = 0x30UL,       ///< USBFS Suspend/Resume
    LPM_DEEP_STANDBY_USBHS        = 0x30UL + 1UL, ///< USBHS Suspend/Resume
    LPM_DEEP_STANDBY_AGT1         = 0x30UL + 2UL, ///< AGT1 Underflow
} lpm_deep_standby_t;

/** Low power modes */
typedef enum st_lpm_low_power_mode
{
    LPM_LOW_POWER_MODE_SLEEP,     ///< Sleep mode (ARM Cortex sleep mode)
    LPM_LOW_POWER_MODE_STANDBY,   ///< Software Standby mode
    LPM_LOW_POWER_MODE_DEEP,      ///< Deep Software Standby mode
} lpm_low_power_mode_t;

/** Output port enable */
typedef enum st_lpm_output_port_enable
{
    LPM_OUTPUT_PORT_ENABLE_HIGH_IMPEDANCE,  ///< 0: In Software Standby Mode or Deep Software Standby Mode, the
                                            ///< address output pins, data output pins, and other bus control signal
                                            ///< output pins are set to the high-impedance state. In Snooze, the
                                            ///< status of the address bus and bus control signals are same as
                                            ///< before entering Software Standby Mode.
    LPM_OUTPUT_PORT_ENABLE_RETAIN,          ///< 1: In Software Standby Mode, the address output pins, data output
                                            ///< pins, and other bus control signal output pins retain the
                                            ///< output state.
} lpm_output_port_enable_t;

/** Select between Software Standby and Deep Software Standby */
typedef enum st_lpm_dpsby
{
    LPM_DPSBY_SOFTWARE_STANDBY,      ///< Sleep mode (SBYCR.SSBY = 0) / Software Standby mode (SBYCR.SSBY = 1)
    LPM_DPSBY_DEEP_SOFTWARE_STANDBY, ///< Sleep mode (SBYCR.SSBY = 0) / Deep Software Standby mode (SBYCR.SSBY = 1)
} lpm_dpsby_t;

/** I/O port state after Deep Software Standby mode */
typedef enum st_lpm_io_port
{
    LPM_IO_PORT_RESET,     ///< When the Deep Software Standby mode is canceled, the I/O ports are in the reset state
    LPM_IO_PORT_NO_CHANGE, ///< When the Deep Software Standby mode is canceled, the I/O ports are in the same state as
                           ///< in the Deep Software Standby mode
} lpm_io_port_t;

/** Power supply control */
typedef enum st_lpm_power_supply
{
    LPM_POWER_SUPPLY_DEEPCUT0 = 0UL,  ///< Power to the standby RAM, Low-speed on-chip oscillator, AGTn, and USBFS/HS resume
                                      ///< detecting unit is supplied in deep software standby mode
    LPM_POWER_SUPPLY_DEEPCUT1 = 1UL,  ///< Power to the standby RAM, Low-speed on-chip oscillator, AGTn, and USBFS/HS resume
                                      ///< detecting unit is not supplied in deep software standby mode
    LPM_POWER_SUPPLY_DEEPCUT3 = 3UL,  ///< Power to the standby RAM, Low-speed on-chip oscillator, AGTn, and USBFS/HS resume
                                      ///< detecting unit is not supplied in deep software standby mode. In addition, LVD is
                                      ///< disabled and the low power function in a poweron reset circuit is enabled
} lpm_power_supply_t;

/** Power save memory control */
typedef enum st_lpm_power_save_memory
{
    LPM_POWER_SAVE_MEMORY_ALL_RAM, ///< All RAM on in Software Standby mode
    LPM_POWER_SAVE_MEMORY_48KB,    ///< 48KB RAM (2000 0000h to 2000 BFFFh) on in Software Standby mode
} lpm_power_save_memory_t;

/** Code flash operation mode */
typedef enum st_lpm_code_flash
{
    LPM_CODE_FLASH_OPERATES, ///< Code flash memory operates
    LPM_CODE_FLASH_STOPS,    ///< Code flash memory stops
} lpm_code_flash_t;

/** Snooze end control */
typedef enum st_lpm_snooze_request
{
    LPM_SNOOZE_REQUEST_RXD0_FALLING   = (      0),   ///< Enable RXD0 falling edge snooze request
                                                     //   If using the RXD0 falling edge as the snooze
                                                     //   request, any event other than the RXD0 falling
                                                     //   edge must not be enabled as snooze request.
    LPM_SNOOZE_REQUEST_IRQ0           = (1 << 0),    ///< Enable IRQ0 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ1           = (1 << 1),    ///< Enable IRQ1 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ2           = (1 << 2),    ///< Enable IRQ2 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ3           = (1 << 3),    ///< Enable IRQ3 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ4           = (1 << 4),    ///< Enable IRQ4 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ5           = (1 << 5),    ///< Enable IRQ5 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ6           = (1 << 6),    ///< Enable IRQ6 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ7           = (1 << 7),    ///< Enable IRQ7 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ8           = (1 << 8),    ///< Enable IRQ8 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ9           = (1 << 9),    ///< Enable IRQ9 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ10          = (1 << 10),   ///< Enable IRQ10 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ11          = (1 << 11),   ///< Enable IRQ11 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ12          = (1 << 12),   ///< Enable IRQ12 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ13          = (1 << 13),   ///< Enable IRQ13 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ14          = (1 << 14),   ///< Enable IRQ14 pin snooze request
    LPM_SNOOZE_REQUEST_IRQ15          = (1 << 15),   ///< Enable IRQ15 pin snooze request
    LPM_SNOOZE_REQUEST_KR             = (1 << 17),   ///< Enable KR snooze request
    LPM_SNOOZE_REQUEST_COMPARATOR_OC0 = (1 << 22),   ///< Enable Comparator-OC0 snooze request
    LPM_SNOOZE_REQUEST_COMPARATOR_LP  = (1 << 23),   ///< Enable Comparator-LP snooze request
    LPM_SNOOZE_REQUEST_RTC_ALARM      = (1 << 24),   ///< Enable RTC alarm snooze request
    LPM_SNOOZE_REQUEST_RTC_PERIOD     = (1 << 25),   ///< Enable RTC period snooze request
    LPM_SNOOZE_REQUEST_AGT1_UNDERFLOW = (1 << 28),   ///< Enable AGT1 underflow snooze request
    LPM_SNOOZE_REQUEST_AGT1_COMPARE_A = (1 << 29),   ///< Enable AGT1 compare match A snooze request
    LPM_SNOOZE_REQUEST_AGT1_COMPARE_B = (1 << 30),   ///< Enable AGT1 compare match B snooze request
} lpm_snooze_request_t;

/** Snooze end control */
typedef enum st_lpm_snooze_end
{
    LPM_SNOOZE_END_VIA_WUPEN                    = (0),        ///< Transition from Snooze to Normal mode directly
    LPM_SNOOZE_END_AGT1_UNDERFLOW               = (1 << 0),   ///< AGT1 underflow
    LPM_SNOOZE_END_DTC_TRANS_COMPLETE           = (1 << 1),   ///< Last DTC transmission completion
    LPM_SNOOZE_END_DTC_TRANS_COMPLETE_NEGATED   = (1 << 2),   ///< Not Last DTC transmission completion
    LPM_SNOOZE_END_ADC0_COMPARE_MATCH           = (1 << 3),   ///< ADC Channel 0 compare match
    LPM_SNOOZE_END_ADC0_COMPARE_MISMATCH        = (1 << 4),   ///< ADC hannel 0 compare mismatch
    LPM_SNOOZE_END_ADC1_COMPARE_MATCH           = (1 << 5),   ///< ADC 1 compare match
    LPM_SNOOZE_END_ADC1_COMPARE_MISMATCH        = (1 << 6),   ///< ADC 1 compare mismatch
    LPM_SNOOZE_END_SCI0_ADDRESS_MATCH           = (1 << 7),   ///< SCI0 address unmatch
} lpm_snooze_end_t;

/** RXD0 Snooze Request Enable */
typedef enum st_lpm_snooze_rxd0
{
    LPM_SNOOZE_RXD0_FALLING_EDGE_IGNORE,   ///< Ignore RXD0 falling edge in Software Standby mode
    LPM_SNOOZE_RXD0_FALLING_EDGE_DETECT,   ///< Detect RXD0 falling edge in Software Standby mode as a request to
                                           ///< transit to Snooze mode. Do not set to 1 other than in asynchronous mode.
} lpm_snooze_rxd0_t;

/** DTC Enable in Snooze Mode */
typedef enum st_lpm_snooze_dtc
{
    LPM_SNOOZE_DTC_DISABLE,   ///< Disable DTC operation
    LPM_SNOOZE_DTC_ENABLE,    ///< Enable DTC operation
} lpm_snooze_dtc_t;

/** Operating power mode */
typedef enum st_lpm_operating_power
{
    LPM_OPERATING_POWER_HIGH_SPEED_MODE,   ///< High speed mode
    LPM_OPERATING_POWER_MIDDLE_SPEED_MODE, ///< Middle speed mode
    LPM_OPERATING_POWER_LOW_VOLTAGE_MODE,  ///< Low voltage mode
    LPM_OPERATING_POWER_LOW_SPEED_MODE,    ///< Low speed mode
} lpm_operating_power_t;

/** Subosc speed mode select */
typedef enum st_lpm_subosc
{
    LPM_SUBOSC_OTHER,   ///< Other than Subosc-speed mode
    LPM_SUBOSC_SELECT,  ///< Subosc-speed mode
} lpm_subosc_t;

/** Module stop bits */
typedef enum st_lpm_mstp
{
    LPM_MSTP_RAM0           = 0x0a00UL,         ///< RAM0
    LPM_MSTP_RAM1           = 0x0a00UL + 1UL,   ///< RAM1
    LPM_MSTP_HSRAM          = 0x0a00UL + 5UL,   ///< HSRAM
    LPM_MSTP_ECCRAM         = 0x0a00UL + 6UL,   ///< ECCRAM
    LPM_MSTP_STANDBYRAM     = 0x0a00UL + 7UL,   ///< STANDBYRAM
    LPM_MSTP_DMAC_DTC       = 0x0a00UL + 22UL,  ///< DMAC_DTC

    LPM_MSTP_RCAN1          = 0x0b00UL + 1UL,   ///< RCAN1
    LPM_MSTP_RCAN0          = 0x0b00UL + 2UL,   ///< RCAN0
    LPM_MSTP_IRDA           = 0x0b00UL + 5UL,   ///< IRDA
    LPM_MSTP_QSPI           = 0x0b00UL + 6UL,   ///< QSPI
    LPM_MSTP_I2C2           = 0x0b00UL + 7UL,   ///< I2C2
    LPM_MSTP_I2C1           = 0x0b00UL + 8UL,   ///< I2C1
    LPM_MSTP_I2C0           = 0x0b00UL + 9UL,   ///< I2C0
    LPM_MSTP_USBFS          = 0x0b00UL + 11UL,  ///< USBFS
    LPM_MSTP_USBHS          = 0x0b00UL + 12UL,  ///< USBHS
    LPM_MSTP_EPTPC_PTPEDMAC = 0x0b00UL + 13UL,  ///< EPTPC_PTPEDMAC
    LPM_MSTP_ETHERC1_EDMAC1 = 0x0b00UL + 14UL,  ///< ETHERC1_EDMAC1
    LPM_MSTP_ETHERC0_EDMAC0 = 0x0b00UL + 15UL,  ///< ETHERC0_EDMAC0
    LPM_MSTP_RSPI1          = 0x0b00UL + 18UL,  ///< RSPI1
    LPM_MSTP_RSPI0          = 0x0b00UL + 19UL,  ///< RSPI0
    LPM_MSTP_SCI9           = 0x0b00UL + 22UL,  ///< SCI9
    LPM_MSTP_SCI8           = 0x0b00UL + 23UL,  ///< SCI8
    LPM_MSTP_SCI7           = 0x0b00UL + 24UL,  ///< SCI7
    LPM_MSTP_SCI6           = 0x0b00UL + 25UL,  ///< SCI6
    LPM_MSTP_SCI5           = 0x0b00UL + 26UL,  ///< SCI5
    LPM_MSTP_SCI4           = 0x0b00UL + 27UL,  ///< SCI4
    LPM_MSTP_SCI3           = 0x0b00UL + 28UL,  ///< SCI3
    LPM_MSTP_SCI2           = 0x0b00UL + 29UL,  ///< SCI2
    LPM_MSTP_SCI1           = 0x0b00UL + 30UL,  ///< SCI1
    LPM_MSTP_SCI0           = 0x0b00UL + 31UL,  ///< SCI0

    LPM_MSTP_CAC            = 0x0c00UL,         ///< CAC
    LPM_MSTP_CRC            = 0x0c00UL + 1UL,   ///< CRC
    LPM_MSTP_PDC            = 0x0c00UL + 2UL,   ///< PDC
    LPM_MSTP_CTSU           = 0x0c00UL + 3UL,   ///< CTSU
    LPM_MSTP_GLCDC          = 0x0c00UL + 4UL,   ///< GLCDC
    LPM_MSTP_JPEG           = 0x0c00UL + 5UL,   ///< JPEG
    LPM_MSTP_2GD            = 0x0c00UL + 6UL,   ///< 2GD
    LPM_MSTP_SSI1           = 0x0c00UL + 7UL,   ///< SSI1
    LPM_MSTP_SSI0           = 0x0c00UL + 8UL,   ///< SSI0
    LPM_MSTP_SRC            = 0x0c00UL + 9UL,   ///< SRC
    LPM_MSTP_SDH_MMC1       = 0x0c00UL + 11UL,  ///< SDH_MMC1
    LPM_MSTP_SDH_MMC0       = 0x0c00UL + 12UL,  ///< SDH_MMC0
    LPM_MSTP_DOC            = 0x0c00UL + 13UL,  ///< DOC
    LPM_MSTP_ELC            = 0x0c00UL + 14UL,  ///< ELC
    LPM_MSTP_TSIP           = 0x0c00UL + 31UL,  ///< TSIP

    LPM_MSTP_AGT1           = 0x0d00UL + 2UL,   ///< AGT1
    LPM_MSTP_AGT0           = 0x0d00UL + 3UL,   ///< AGT0
    LPM_MSTP_GPT_CH7_0      = 0x0d00UL + 5UL,   ///< GPT_CH7_0
    LPM_MSTP_GPT_CH13_8     = 0x0d00UL + 6UL,   ///< GPT_CH13_8
    LPM_MSTP_PGI            = 0x0d00UL + 14UL,  ///< PGI
    LPM_MSTP_S12AD_UNIT1    = 0x0d00UL + 15UL,  ///< S12AD_UNIT1
    LPM_MSTP_S12AD_UNIT0    = 0x0d00UL + 16UL,  ///< S12AD_UNIT0
    LPM_MSTP_D_A0           = 0x0d00UL + 20UL,  ///< D_A0
    LPM_MSTP_TEMP_SENSE     = 0x0d00UL + 22UL,  ///< TEMP_SENSE
    LPM_MSTP_COMP_OC5       = 0x0d00UL + 23UL,  ///< COMP_OC5
    LPM_MSTP_COMP_OC4       = 0x0d00UL + 24UL,  ///< COMP_OC4
    LPM_MSTP_COMP_OC3       = 0x0d00UL + 25UL,  ///< COMP_OC3
    LPM_MSTP_COMP_OC2       = 0x0d00UL + 26UL,  ///< COMP_OC2
    LPM_MSTP_COMP_OC1       = 0x0d00UL + 27UL,  ///< COMP_OC1
    LPM_MSTP_COMP_OC0       = 0x0d00UL + 28UL,  ///< COMP_OC0
    LPM_MSTP_COMP_LP        = 0x0d00UL + 29UL,  ///< COMP_LP
    LPM_MSTP_COMP_RD        = 0x0d00UL + 30UL,  ///< COMP_RD
    LPM_MSTP_COMP_OPAMP     = 0x0d00UL + 31UL,  ///< COMP_OPAMP
} lpm_mstp_t;

/** User configuration structure, used in open function */
typedef struct st_lpm_cfg
{
    lpm_operating_power_t  operating_power;         ///< Operating power mode
    lpm_subosc_t           sub_oscillator;          ///< Sub oscillator
    lpm_code_flash_t       code_flash;              ///< Enable the code flash
} lpm_cfg_t;

/** lpm driver structure. General lpm functions implemented at the HAL layer will follow this API. */
typedef struct st_lpm_api
{
    /** Open the LPM driver module
     * Initialized the LPM block according to the passed in config structure
     * @param[in] p_cfg                Pointer to a configuration structure
     **/
    ssp_err_t (* init)(lpm_cfg_t const * const p_cfg);

    /** Set the value of all the Module Stop Control Registers
     * @param[in] mstpcra_value       The value for MSTPCRA
     * @param[in] mstpcrb_value       The value for MSTPCRB
     * @param[in] mstpcrc_value       The value for MSTPCRC
     * @param[in] mstpcrd_value       The value for MSTPCRD
     **/
    ssp_err_t (* mstpcrSet)(uint32_t mstpcra_value,
                            uint32_t mstpcrb_value,
                            uint32_t mstpcrc_value,
                            uint32_t mstpcrd_value);

    /** Get the values of all the Module Stop Control Registers
     * @param[in, out] mstpcra_value  The value from MSTPCRA
     * @param[in, out] mstpcrb_value  The value from MSTPCRB
     * @param[in, out] mstpcrc_value  The value from MSTPCRC
     * @param[in, out] mstpcrd_value  The value from MSTPCRD
     **/
    ssp_err_t (* mstpcrGet)(uint32_t * mstpcra_value,
                            uint32_t * mstpcrb_value,
                            uint32_t * mstpcrc_value,
                            uint32_t * mstpcrd_value);

    /** Stop a module
     * @param[in] module               The module to set the state of
     **/
    ssp_err_t (* moduleStop)(lpm_mstp_t module);

    /** Run a module
     * @param[in] module               The module to set the state of
     **/
    ssp_err_t (* moduleStart)(lpm_mstp_t module);

    /** Set the operating power mode
     * @param[in] power_mode           The power mode to set the chip to
     * @param[in] subosc               Select the sub oscillator or other oscillator
     **/
    ssp_err_t (* operatingPowerModeSet)(lpm_operating_power_t power_mode, lpm_subosc_t subosc);

    /** Configure and enable snooze mode
     * @param[in] rdx0_mode            Enter Snooze mode on the falling edge of RXD0 (only in asynchronous mode)
     * @param[in] dtc_mode             Use DTC and RAM while in Snooze Mode
     * @param[in] requests             Specify the events that cause an entry into Snooze mode
     * @param[in] triggers             Specify the events that cause an exit from snooze mode
     **/
    ssp_err_t (* snoozeEnable)(lpm_snooze_rxd0_t    rdx0_mode,
                               lpm_snooze_dtc_t     dtc_mode,
                               lpm_snooze_request_t requests,
                               lpm_snooze_end_t     triggers);

    /** Disable snooze mode
     * @retval SSP_SUCCESS             Snooze mode successfully disabled
     **/
    ssp_err_t (* snoozeDisable)(void);

    /** Configure a low power mode
     * @param[in] low_power_mode       Which low power mode to enter
     * @param[in] output_port_enable   Retain port output status in sleep or standby
     * @param[in] power_supply         What remains powered in Deep Software Standby
     * @param[in] io_port_state        I/O port state after Deep Software Standby mode
     **/
    ssp_err_t (* lowPowerCfg)(lpm_low_power_mode_t     power_mode,
                              lpm_output_port_enable_t output_port_enable,
                              lpm_power_supply_t       power_supply,
                              lpm_io_port_t            io_port_state);

    /** Set the value of the Wake Up Interrupt Enable Register WUPEN
     * @param[in] wupen_value       The value for WUPEN
     **/
    ssp_err_t (* wupenSet)(uint32_t wupen_value);

    /** Get the value of the Wake Up Interrupt Enable Register WUPEN
     * @param[in, out] wupen_value  The value from WUPEN
     **/
    ssp_err_t (* wupenGet)(uint32_t * wupen_value);

    /** Enable a Deep Standby Cancel Request
     * @param[in] pin_signal           The pin or signal associated with deep standby mode
     * @param[in] rising_falling       Which edge causes the cancel request (ignore with RTC interrupts)
     **/
    ssp_err_t (* deepStandbyCancelRequestEnable)(lpm_deep_standby_t        pin_signal,
                                                 lpm_cancel_request_edge_t rising_falling);

    /** Disable a Deep Standby Cancel Request
     * @param[in] pin_signal           The pin or signal associated with deep standby mode
     **/
    ssp_err_t (* deepStandbyCancelRequestDisable)(lpm_deep_standby_t pin_signal);

    /** Enter low power mode (sleep/standby/deep standby) using WFI macro.
     *  Function will return after waking from low power mode.
     **/
    ssp_err_t (* lowPowerModeEnter)(void);

    /** Get the driver version based on compile time macros.
     * @retval     SSP_SUCCESS          Successful close.
     * @retval     SSP_ERR_INVALID_PTR  p_version is NULL.
     **/
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);
} lpm_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_lpm_instance
{
    lpm_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    lpm_api_t const * p_api;     ///< Pointer to the API structure for this instance
} lpm_instance_t;

#endif /* DRV_LPM_API_H */

/*******************************************************************************************************************//**
 * @} (end defgroup LPM_API)
 **********************************************************************************************************************/
