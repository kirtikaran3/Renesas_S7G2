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
* File Name    : r_adc_api.h
* Description  : ADC driver interface file.
***********************************************************************************************************************/

#ifndef DRV_ADC_API_H
#define DRV_ADC_API_H
/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup ADC_API ADC Interface
 * @brief Interface for A/D Converters.
 *
 * @section ADC_API_SUMMARY Summary
 * The ADC interface provides standard ADC functionality including one-shot mode (single scan), continuous scan and
 * group scan. It also allows configuration of hardware and software triggers for starting scans. After each conversion
 * an interrupt can be triggered, and if a callback function is provided, the call back is invoked with the
 * appropriate event information.
 *
 * Implemented by:
 * @ref ADC
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * ADC Interface description: @ref HALADCInterface
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Includes board and MCU related header files. */
#include "bsp_api.h"
#include "r_elc_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Version Number of API.  */
#define ADC_API_VERSION_MAJOR   (1)
#define ADC_API_VERSION_MINOR   (2)

/*****************************************************************************
Typedef definitions
******************************************************************************/
/** Largest supported ADC resolution size.
 */
typedef uint16_t adc_data_size_t;

/** ADC operation mode definitions  */
typedef enum e_adc_mode
{
    ADC_MODE_SINGLE_SCAN,       ///< Single scan - one or more channels
    ADC_MODE_CONTINUOUS_SCAN,   ///< Continuous scan - one or more channels
    ADC_MODE_GROUP_SCAN,        ///< Two trigger sources to trigger scan for two groups which contain one or more channels
} adc_mode_t;

/** ADC data resolution definitions */
typedef enum e_adc_resolution
{
    ADC_RESOLUTION_12_BIT = 0, ///< 12 bit resolution
    ADC_RESOLUTION_10_BIT = 1, ///< 10 bit resolution
    ADC_RESOLUTION_8_BIT  = 2, ///< 8 bit resolution
    ADC_RESOLUTION_14_BIT = 3, ///< 14 bit resolution
} adc_resolution_t;

/** ADC data alignment definitions  */
typedef enum e_adc_alignment
{
    ADC_ALIGNMENT_RIGHT = 0x0000,   ///< Data alignment right
    ADC_ALIGNMENT_LEFT  = 0x8000    ///< Data alignment left
} adc_alignment_t;

/** ADC data sample addition and averaging options */
typedef enum e_adc_add
{
    ADC_ADD_OFF             = 0,   ///< Addition turned off for channels/sensors
    ADC_ADD_TWO             = 1,   ///< Add two samples
    ADC_ADD_THREE           = 2,   ///< Add three samples
    ADC_ADD_FOUR            = 3,   ///< Add four samples
    ADC_ADD_AVERAGE_TWO     = 0x81,///< Average two samples
    ADC_ADD_AVERAGE_FOUR    = 0x83,///< Average four samples
    ADC_ADD_AVERAGE_SIXTEEN = 0x85,///< Average sixteen samples
} adc_add_t;

/** ADC clear after read definitions */
typedef enum e_adc_clear
{
    ADC_CLEAR_AFTER_READ_OFF = 0x0000,    ///< Clear after read off
    ADC_CLEAR_AFTER_READ_ON  = 0x0020     ///< Clear after read on
} adc_clear_t;

/** ADC trigger mode definitions */
typedef enum e_adc_trigger
{
    ADC_TRIGGER_ASYNC_EXT_TRG0,            ///< External asynchronous trigger; not for group modes
    ADC_TRIGGER_SYNC_ELC,                  ///< Synchronous trigger via ELC
    ADC_TRIGGER_SOFTWARE                   ///< Software trigger; not for group modes
} adc_trigger_t;

/** ADC sample state registers */
typedef enum e_adc_sample_state_reg
{
    ADC_SAMPLE_STATE_CHANNEL_0 = 0,         ///< Sample state register channel 0
    ADC_SAMPLE_STATE_CHANNEL_1,             ///< Sample state register channel 1
    ADC_SAMPLE_STATE_CHANNEL_2,             ///< Sample state register channel 2
    ADC_SAMPLE_STATE_CHANNEL_3,             ///< Sample state register channel 3
    ADC_SAMPLE_STATE_CHANNEL_4,             ///< Sample state register channel 4
    ADC_SAMPLE_STATE_CHANNEL_5,             ///< Sample state register channel 5
    ADC_SAMPLE_STATE_CHANNEL_6,             ///< Sample state register channel 6
    ADC_SAMPLE_STATE_CHANNEL_7,             ///< Sample state register channel 7
    ADC_SAMPLE_STATE_CHANNEL_8,             ///< Sample state register channel 8
    ADC_SAMPLE_STATE_CHANNEL_9,             ///< Sample state register channel 9
    ADC_SAMPLE_STATE_CHANNEL_10,             ///< Sample state register channel 10
    ADC_SAMPLE_STATE_CHANNEL_11,             ///< Sample state register channel 11
    ADC_SAMPLE_STATE_CHANNEL_12,             ///< Sample state register channel 12
    ADC_SAMPLE_STATE_CHANNEL_13,             ///< Sample state register channel 13
    ADC_SAMPLE_STATE_CHANNEL_14,             ///< Sample state register channel 14
    ADC_SAMPLE_STATE_CHANNEL_15,             ///< Sample state register channel 15
    ADC_SAMPLE_STATE_CHANNEL_16_TO_21 = 16,  ///< Sample state register channel 16 to 21 for unit 0 on S7G2
    ADC_SAMPLE_STATE_CHANNEL_16_TO_20 = 16,  ///< Sample state register channel 16 to 20 for unit 1 on S7G2
    ADC_SAMPLE_STATE_CHANNEL_16_TO_27 = 16,  ///< Sample state register channel 16 to 27 for unit 0 on S3A7
    ADC_SAMPLE_STATE_TEMPERATURE,           ///< Sample state register channel temperature
    ADC_SAMPLE_STATE_VOLTAGE                ///< Sample state register channel voltage
} adc_sample_state_reg_t;

/** ADC sample state configuration */
typedef struct st_adc_sample_state
{
    adc_sample_state_reg_t   reg_id;      ///< Sample state register ID
    uint8_t                  num_states;  ///< Number of sampling states for conversion. Ch16-20/21 use the same value.
} adc_sample_state_t;

/** ADC callback event definitions  */
typedef enum e_adc_cb_event
{
    ADC_EVENT_SCAN_COMPLETE,            ///< Normal/Group A scan complete
    ADC_EVENT_SCAN_COMPLETE_GROUP_B,    ///< Group B scan complete
} adc_cb_event_t;

/** ADC callback arguments definitions  */
typedef struct st_adc_callbackb_args
{
    uint16_t       unit;              ///< ADC device in use
    adc_cb_event_t event;             ///< ADC callback event
    void const   * p_context;         ///< Placeholder for user data
} adc_callback_args_t;

/** ADC action for group A interrupts group B scan.
 * This enumeration is used to specify the priority between Group A and B in group mode.  */
typedef enum e_adc_group_a
{
    ADC_GROUP_A_PRIORITY_OFF             = 0,       ///< Group A ignored and does not interrupt ongoing group B scan
    ADC_GROUP_A_GROUP_B_WAIT_FOR_TRIGGER = 1,       ///< Group A interrupts Group B(single scan) which restarts at next Group B trigger
    ADC_GROUP_A_GROUP_B_RESTART_SCAN     = 3,       ///< Group A interrupts Group B(single scan) which restarts immediately after Group A scan is complete
    ADC_GROUP_A_GROUP_B_CONTINUOUS_SCAN  = 0x8001,  ///< Group A interrupts Group B(continuous scan) which continues scanning without a new Group B trigger
} adc_group_a_t;

/** ADC registers used for the Read() argument */
typedef enum e_adc_register
{
    ADC_REG_CHANNEL_0 = 0,          ///< ADC  channel 0
    ADC_REG_CHANNEL_1 = 1,          ///< ADC  channel 1
    ADC_REG_CHANNEL_2 = 2,          ///< ADC  channel 2
    ADC_REG_CHANNEL_3 = 3,          ///< ADC  channel 3
    ADC_REG_CHANNEL_4 = 4,          ///< ADC  channel 4
    ADC_REG_CHANNEL_5 = 5,          ///< ADC  channel 5
    ADC_REG_CHANNEL_6 = 6,          ///< ADC  channel 6
    ADC_REG_CHANNEL_7 = 7,          ///< ADC  channel 7
    ADC_REG_CHANNEL_8 = 8,          ///< ADC  channel 8
    ADC_REG_CHANNEL_9 = 9,          ///< ADC  channel 9
    ADC_REG_CHANNEL_10 = 10,        ///< ADC  channel 10
    ADC_REG_CHANNEL_11 = 11,        ///< ADC  channel 11
    ADC_REG_CHANNEL_12 = 12,        ///< ADC  channel 12
    ADC_REG_CHANNEL_13 = 13,        ///< ADC  channel 13
    ADC_REG_CHANNEL_14 = 14,        ///< ADC  channel 14
    ADC_REG_CHANNEL_15 = 15,        ///< ADC  channel 15
    ADC_REG_CHANNEL_16 = 16,        ///< ADC  channel 16
    ADC_REG_CHANNEL_17 = 17,        ///< ADC  channel 17
    ADC_REG_CHANNEL_18 = 18,        ///< ADC  channel 18
    ADC_REG_CHANNEL_19 = 19,        ///< ADC  channel 19
    ADC_REG_CHANNEL_20 = 20,        ///< ADC  channel 20
    ADC_REG_CHANNEL_21 = 21,        ///< ADC  channel 21 (Unit 0 only)
    ADC_REG_CHANNEL_22 = 22,        ///< ADC  channel 22
    ADC_REG_CHANNEL_23 = 23,        ///< ADC  channel 23
    ADC_REG_CHANNEL_24 = 24,        ///< ADC  channel 24
    ADC_REG_CHANNEL_25 = 25,        ///< ADC  channel 25
    ADC_REG_CHANNEL_26 = 26,        ///< ADC  channel 26
    ADC_REG_CHANNEL_27 = 27,        ///< ADC  channel 27
    ADC_REG_TEMPERATURE,            ///< ADC  channel temperature
    ADC_REG_VOLT,                   ///< ADC  channel volt
} adc_register_t;

/** ADC Information Structure for Transfer Interface */
typedef struct st_adc_info
{
    volatile uint16_t * p_address;      ///< The address to start reading the data from
    uint32_t            length;         ///< The total number of bytes to read
    elc_peripheral_t    elc_peripheral; ///< Name of the peripheral in the ELC list
    elc_event_t         elc_event;      ///< Name of the ELC event for the peripheral
} adc_info_t;

/** ADC channel(s) configuration       */
typedef struct st_adc_channel_cfg
{
    uint32_t      scan_mask;          ///< Channels/bits: bit 0 is ch0; bit 15 is ch15. Use `#%define ADC_MASK_xxx`
                                      ///< from r_adc.h.
    uint32_t      scan_mask_group_b;  ///< Valid for group modes. Use `#%define ADC_MASK_xxx`  from r_adc.h.
    adc_group_a_t priority_group_a;   ///< Valid for group modes.
    uint32_t      add_mask;           ///< Valid if add enabled in Open(). Use `#%define ADC_MASK_xxx`  from r_adc.h.
    uint8_t       sample_hold_mask;   ///< Channels/bits 0-2. Use `#%define ADC_MASK_xxx`  from r_adc.h.
    uint8_t       sample_hold_states; ///< Number of states to be used for sample and hold. Affects channels 0-2.
} adc_channel_cfg_t;

/** ADC general configuration  */
typedef struct st_adc_cfg
{
    uint16_t         unit;             ///< ADC Unit to be used
    adc_mode_t       mode;             ///< ADC operation mode
    adc_resolution_t resolution;       ///< ADC resolution 8, 10, or 12-bit
    adc_alignment_t  alignment;        ///< Specify left or right alignment; ignored if addition used
    adc_add_t        add_average_count;///< Add or average samples
    adc_clear_t      clearing;         ///< Clear after read
    adc_trigger_t    trigger;          ///< Default and Group A trigger source
    adc_trigger_t    trigger_group_b;  ///< Group B trigger source; valid only for group mode
    void          (* p_callback)(adc_callback_args_t * p_args); ///< Callback function; set to NULL for none
    void const     * p_context;        ///< Placeholder for user data. Passed to the user callback in adc_api_t::adc_callback_args_t.
    void const     * p_extend;         ///< Extension parameter for hardware specific settings
} adc_cfg_t;

/** ADC control block. DO NOT INITIALIZE.  Initialized in adc_api_t::open(). */
typedef struct
{
    uint16_t  unit;      ///< ADC Unit in use
    void const * p_context; ///< Placeholder for user data
} adc_ctrl_t;

/** ADC functions implemented at the HAL layer will follow this API. */
typedef struct st_adc_api
{
    /** Initialize ADC Unit;  apply power, set the operational mode, trigger sources, interrupt priority,
     * and configurations common to all channels and sensors.
     * @par Implemented as
     *  - R_ADC_Open()
     *
     * @pre Configure peripheral clocks, ADC pins and IRQs prior to calling this function.
     * @param[in]  p_ctrl  Pointer to control handle structure
     * @param[in]  p_cfg   Pointer to configuration structure
     */
    ssp_err_t (* open)(adc_ctrl_t * const p_ctrl,  adc_cfg_t const * const p_cfg);

    /** Configure the scan including the channels, groups and scan triggers to be used for the unit that
     *  was initialized in the open call.
     * @par Implemented as
     *  - R_ADC_ScanConfigure()
     *
     * @param[in]  p_ctrl     Pointer to control handle structure
     * @param[in]  p_channel_cfg   Pointer to scan configuration structure
     */
    ssp_err_t (* scanCfg)(adc_ctrl_t * const p_ctrl,  adc_channel_cfg_t const * const p_channel_cfg);

    /** Start the scan (in case of a software trigger), or enable the hardware trigger.
     * @par Implemented as
     *  - R_ADC_ScanStart()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     */
    ssp_err_t (* scanStart)(adc_ctrl_t * const p_ctrl);

    /** Stop the ADC scan (in case of a software trigger), or disable the hardware trigger.
     * @par Implemented as
     *  - R_ADC_ScanStop()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     */
    ssp_err_t (* scanStop)(adc_ctrl_t * const p_ctrl);

    /** Check scan status.
     * @par Implemented as
     *  - R_ADC_CheckScanDone()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     */
    ssp_err_t (* scanStatusGet)(adc_ctrl_t * const p_ctrl);

    /** Read ADC conversion result(s).
     * @par Implemented as
     *  - R_ADC_Read()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     * @param[in]  reg_id   Id for the register to read (see enumeration adc_register_t)
     * @param[in]  p_data   Pointer to variable to load value into.
     */
    ssp_err_t (* read)(adc_ctrl_t  * const p_ctrl, adc_register_t const  reg_id,  adc_data_size_t * const p_data);

    /** Set the sample state count for the specified channel.
     * @par Implemented as
     *  - R_ADC_SetSampleStateCount()
     *
     * @param[in]  p_ctrl    Pointer to control handle structure
     * @param[in]  p_sample  Pointer to the ADC channels and corresponding sample states to be set
     */
    ssp_err_t (* sampleStateCountSet)(adc_ctrl_t * const p_ctrl,  adc_sample_state_t  * p_sample);

    /** Close the specified ADC unit by ending any scan in progress, disabling interrupts, and removing power to the
     * specified A/D unit.
     * @par Implemented as
     *  - R_ADC_Close()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     */
    ssp_err_t (* close)(adc_ctrl_t * const p_ctrl);

    /** Return the ADC data register address of the first (lowest number) channel and the total number of bytes
     * to be read in order for the DTC/DMAC to read the conversion results of all configured channels.
     * @par Implemented as
     *  - R_ADC_InfoGet()
     *
     * @param[in]   p_ctrl       Pointer to control handle structure
     * @param[out]  p_adc_info   Pointer to ADC information structure
     */
    ssp_err_t (* infoGet) (adc_ctrl_t * const p_ctrl, adc_info_t * const p_adc_info);

    /** Retrieve the API version.
     * @par Implemented as
     *  - R_ADC_VersionGet()
     *
     * @pre This function retrieves the API version.
     * @param[in]  p_version   Pointer to version structure
     */
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);

} adc_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_adc_instance
{
    adc_ctrl_t              * p_ctrl;         ///< Pointer to the control structure for this instance
    adc_cfg_t         const * p_cfg;          ///< Pointer to the configuration structure for this instance
    adc_channel_cfg_t const * p_channel_cfg;  ///< Pointer to the channel configuration structure for this instance
    adc_api_t         const * p_api;          ///< Pointer to the API structure for this instance
} adc_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup ADC_API)
 **********************************************************************************************************************/

#endif /* DRV_ADC_API_H */
