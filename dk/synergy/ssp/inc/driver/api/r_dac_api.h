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
 * File Name    : r_dac_api.h
 * Description  : DAC interface layer APIs. 
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup DAC_API DAC Interface
 * @brief Interface for D/A converters.
 *
 * @section DAC_API_SUMMARY Summary
 * The DAC interface provides standard Digital/Analog Converter functionality.  A DAC application writes digital
 * sample data to the device and generates analog output on the DAC output pin.
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * DAC Interface description: @ref HALDACInterface
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_DAC_API_H
#define DRV_DAC_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Common error codes and definitions. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Data type to store DAC output value.  */
typedef uint16_t dac_size_t;

/** DAC Open API AD/DA data format settings. */
typedef enum e_dac_data_format
{
    DAC_DATA_FORMAT_FLUSH_RIGHT = 0,  ///< LSB of data is flush to the right leaving the top 4 bits unused.
    DAC_DATA_FORMAT_FLUSH_LEFT  = 1   ///< MSB of data is flush to the left leaving the bottom 4 bits unused.
} dac_data_format_t;

/** DAC Open API configuration parameter */
typedef struct st_dac_cfg
{
    uint8_t            channel;                        ///< ID associated with this DAC channel
    bool               ad_da_synchronized;             ///< AD/DA synchronization
    dac_data_format_t  data_format;                    ///< Data format
    bool               output_amplifier_enabled;       ///< Output amplifier enable
    void const       * p_extend;
} dac_cfg_t;

/** DAC channel control block */
typedef struct st_dac_ctrl_s
{
    uint8_t        channel;                            ///< ID associated with this DAC channel
    uint8_t        channel_started;                    ///< DAC operation on channel started
    uint8_t        channel_opened;                     ///< DAC channel open
    uint8_t        reserved1;
} dac_ctrl_t;

/** DAC driver structure.  General DAC functions implemented at the HAL layer follow this API. */
typedef struct st_dac_api
{
    /** Initial configuration.
     * @par Implemented as
     * - R_DAC_Open()
     *
     * @param[in]   p_ctrl     Pointer to control block. Must be declared by user. Elements set here.
     * @param[in]   p_cfg      Pointer to configuration structure. All elements of this structure must be set by user.
     */
    ssp_err_t (* open)(dac_ctrl_t * p_ctrl, dac_cfg_t const * const p_cfg);

    /** Close the D/A Converter.
     * @par Implemented as
     * - R_DAC_Close()
     *
     * @param[in]   p_ctrl     Control block set in dac_api_t::open call for this timer.
     */
    ssp_err_t (* close)(dac_ctrl_t * p_ctrl);

    /** Write sample value to the D/A Converter.
     * @par Implemented as
     * - R_DAC_Write()
     *
     * @param[in]   p_ctrl     Control block set in dac_api_t::open call for this timer.
     * @param[in]   value      Sample value to be written to the D/A Converter.
     */
    ssp_err_t (* write)(dac_ctrl_t * p_ctrl, dac_size_t value);

    /** Start the D/A Converter if it has not been started yet.
     * @par Implemented as
     * - R_DAC_Start()
     *
     * @param[in]   p_ctrl     Control block set in dac_api_t::open call for this timer.
     */
    ssp_err_t (* start)(dac_ctrl_t * p_ctrl);

    /** Stop the D/A Converter if the converter is running.
     * @par Implemented as
     * - R_DAC_Stop()
     *
     * @param[in]   p_ctrl     Control block set in dac_api_t::open call for this timer.
     */
    ssp_err_t (* stop)(dac_ctrl_t * p_ctrl);

    /** Get version and store it in provided pointer p_version.
     * @par Implemented as
     * - R_DAC_VersionGet()
     *
     * @param[out]  p_version  Code and API version used.
     */
    ssp_err_t (* versionGet)(ssp_version_t * p_version);
} dac_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_dac_instance
{
    dac_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    dac_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    dac_api_t const * p_api;     ///< Pointer to the API structure for this instance
} dac_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup DAC_API)
 **********************************************************************************************************************/
#endif //DRV_DAC_API_H
