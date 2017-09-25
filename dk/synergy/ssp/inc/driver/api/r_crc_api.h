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
 * File Name    : r_crc_api.h
 * Description  : CRC Interface
 **********************************************************************************************************************/

#ifndef DRV_CRC_API_H
#define DRV_CRC_API_H

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup CRC_API CRC Interface
 *
 * @brief Interface for cyclic redundancy checking.
 *
 * @section CRC_API_SUMMARY Summary
 * The CRC (Cyclic Redundancy Check) calculator generates CRC codes using five different polynomials including 8 bit,
 * 16 bit, and 32 bit variations. Calculation can be performed by sending data to the block using the CPU or by snooping
 * on read or write activity on one of 10 SCI channels.
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * CRC Interface description: @ref HALCRCInterface
 *
 * @{
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Register definitions, common services and error codes. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define CRC_API_VERSION_MAJOR (1)
#define CRC_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** CRC Generating Polynomial Switching (GPS).  */
typedef enum e_crc_polynomial
{
    CRC_POLYNOMIAL_CRC_8 = 1,      ///< 8-bit CRC-8 (X8 + X2 + X + 1)
    CRC_POLYNOMIAL_CRC_16,         ///< 16-bit CRC-16 (X16 + X15 + X2 + 1)
    CRC_POLYNOMIAL_CRC_CCITT,      ///< 16-bit CRC-CCITT (X16 + X12 + X5 + 1)
    /** 32-bit CRC-32 (X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 + X8 + X7 + X5 + X4 + X2 + X + 1) */
    CRC_POLYNOMIAL_CRC_32,
    /** 32-bit CRC-32C (X32 + X28 + X27 + X26 + X25 + X23 + X22 + X20 + X19 + X18 + X14 + X13 + X11 + X10 + X9 + X8 +
     * X6 + 1) */
    CRC_POLYNOMIAL_CRC_32C,
} crc_polynomial_t;

/** CRC Calculation Switching (LMS) */
typedef enum e_crc_bit_order
{
    CRC_BIT_ORDER_LMS_LSB = 0,                ///< Generates CRC for LSB first communication
    CRC_BIT_ORDER_LMS_MSB,                    ///< Generates CRC for MSB first communication
} crc_bit_order_t;

/** Snoop-On-Write/Read Switch (CRCSWR) */
typedef enum e_crc_snoop_direction
{
    CRC_SNOOP_DIRECTION_RECEIVE = 0,          ///< Snoop-on-read
    CRC_SNOOP_DIRECTION_TRANSMIT,             ///< Snoop-on-write
} crc_snoop_direction_t;

/** Driver handle structure. */
typedef struct st_crc_hdl
{
    crc_polynomial_t  polynomial;   ///< CRC Generating Polynomial Switching (GPS).
    crc_bit_order_t   bit_order;    ///< CRC Calculation Switching (LMS).
} crc_ctrl_t;

/** User configuration structure, used in open function */
typedef struct st_crc_cfg
{
    crc_polynomial_t  polynomial;       ///< CRC Generating Polynomial Switching. (GPS)
    crc_bit_order_t   bit_order;        ///< CRC Calculation Switching (LMS)
    void const      * p_extend;         ///< CRC Hardware Dependent Configuration.
} crc_cfg_t;

/** Snoop configuration */
typedef struct st_crc_snoop_cfg
{
    uint32_t               snoop_channel;   ///< Register Snoop Address (CRCSA)
    crc_snoop_direction_t  snoop_direction; ///< Snoop-On-Write/Read Switch (CRCSWR)
} crc_snoop_cfg_t;

/** CRC driver structure. General CRC functions implemented at the HAL layer will follow this API. */
typedef struct st_crc_api
{
    /** Open the CRC driver module.
     * @par Implemented as
     * - R_CRC_Open()
     *
     * @param[in] p_ctrl               Pointer to CRC device handle.
     * @param[in] p_cfg                Pointer to a configuration structure.
     **/
    ssp_err_t (* open)(crc_ctrl_t * const p_ctrl, crc_cfg_t const * const p_cfg);

    /** Close the CRC module driver
     * @par Implemented as
     * - R_CRC_Close()
     *
     * @param[in] p_ctrl               Pointer to crc device handle
     * @retval SSP_SUCCESS             Configuration was successful.
     **/
    ssp_err_t (* close)(crc_ctrl_t * const p_ctrl);

    /** Return the current calculated value.
     * @par Implemented as
     * - R_CRC_CalculatedValueGet()
     *
     * @param[in]  p_ctrl              Pointer to CRC device handle.
     * @param[out] crc_result          The calculated value from the last CRC calculation.
     **/
    ssp_err_t (* crcResultGet)(crc_ctrl_t * const p_ctrl, uint32_t * crc_result);

    /** Enable snooping.
     * @par Implemented as
     * - R_CRC_SnoopEnable()
     *
     * @param[in] p_ctrl               Pointer to CRC device handle.
     * @param[in] crc_seed             CRC seed.
     **/
    ssp_err_t (* snoopEnable)(crc_ctrl_t * const p_ctrl, uint32_t crc_seed);

    /** Disable snooping.
     * @par Implemented as
     * - R_CRC_SnoopDisable()
     *
     * @param[in] p_ctrl               Pointer to crc device handle.
     **/
    ssp_err_t (* snoopDisable)(crc_ctrl_t * const p_ctrl);

    /** Configure the snoop channel and direction.
     * @par Implemented as
     * - R_CRC_SnoopCfg()
     *
     * @param[in] p_ctrl               Pointer to crc device handle.
     * @param[in] p_snoopCfg           Snoop configuration.
     **/
    ssp_err_t (* snoopCfg)(crc_ctrl_t * const p_ctrl, crc_snoop_cfg_t * const p_snoop_cfg);

    /** Perform a CRC calculation on a block of data.
     * @par Implemented as
     * - R_CRC_Calculate()
     *
     * @param[in]  p_ctrl         Pointer to crc device handle.
     * @param[in]  input_buffer   A pointer to an array of data values.
     * @param[in]  num_bytes      The number of bytes (not elements) in the array.
     * @param[in]  crc_seed       The seeded value for crc calculations.
     * @param[out] crc_result     The calculated value of the CRC calculation.
     **/
    ssp_err_t (* calculate)(crc_ctrl_t * const p_ctrl,
                            void             * p_input_buffer,
                            uint32_t           num_bytes,
                            uint32_t           crc_seed,
                            uint32_t         * p_crc_result);

    /** Get the driver version based on compile time macros.
     * @par Implemented as
     * - R_CRC_VersionGet()
     **/
    ssp_err_t (* versionGet)(ssp_version_t * version);
} crc_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_crc_instance
{
    crc_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    crc_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    crc_api_t const * p_api;     ///< Pointer to the API structure for this instance
} crc_instance_t;


/*******************************************************************************************************************//**
 * @} (end addtogroup CRC_API)
 **********************************************************************************************************************/
#endif /* DRV_CRC_API_H */
