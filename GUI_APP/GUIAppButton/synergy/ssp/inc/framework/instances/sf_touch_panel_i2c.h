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
 * File Name    : sf_touch_panel_i2c.h
 * Description  : RTOS integrated touch panel API extension for I2C touch panel chips.
 **********************************************************************************************************************/

#ifndef SF_TOUCH_PANEL_I2C_API_H
#define SF_TOUCH_PANEL_I2C_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @defgroup SF_TOUCH_PANEL_I2C I2C Touch Panel Framework
 * @brief RTOS-integrated touch panel Framework implementation for external I2C touch chips.
 *
 * @section SF_TOUCH_PANEL_I2C_API_SUMMARY Summary
 * This is a ThreadX touch panel framework implemented for external I2C touch controllers with IRQ pins used to notify
 * the application when new data is available.
 *
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Include driver API and ThreadX API */
#include "sf_touch_panel_api.h"
#include "sf_external_irq_api.h"
#include "r_i2c_api.h"
#include "r_ioport.h"
#include "tx_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_TOUCH_PANEL_I2C_CODE_VERSION_MAJOR (1)
#define SF_TOUCH_PANEL_I2C_CODE_VERSION_MINOR (1)

#define SF_TOUCH_PANEL_I2C_RESET_PIN_UNUSED   (0xFFFF)

/** Stack size for I2C touch panel thread. */
#define SF_TOUCH_PANEL_I2C_STACK_SIZE (1024)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Chip definition. */
typedef struct st_sf_touch_panel_i2c_chip
{
    /** Reads the touch chip and fills in the touch payload data.
     * @param[in,out] p_ctrl     Pointer to a structure allocated by user. This control structure is initialized in
     *                           this function.
     * @param[out]    p_payload  Pointer to the payload to data structure.  Touch data provided should be processed to
     *                           logical pixel values.
     */
    ssp_err_t (* payloadGet)(sf_touch_panel_ctrl_t * const p_ctrl, sf_touch_panel_payload_t * const p_payload);

    /** Resets the touch chip.
     * @param[in,out] p_ctrl     Pointer to a structure allocated by user. This control structure is initialized in
     *                           this function.
     */
    ssp_err_t (* reset)(sf_touch_panel_ctrl_t * const p_ctrl);
} sf_touch_panel_i2c_chip_t;

/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when SF_TOUCH_PANEL_I2C_Open is called */
typedef struct st_sf_touch_panel_i2c_ctrl
{
    /** Port pin connected to reset line on touch controller chip. */
    ioport_port_pin_t  pin;

    /** Lower level I2C. */
    i2c_master_instance_t const  * p_lower_lvl_i2c;

    /** Lower level external IRQ. */
    sf_external_irq_instance_t const  * p_lower_lvl_irq;

    /** Stack for touch panel thread. */
    uint8_t  stack[SF_TOUCH_PANEL_I2C_STACK_SIZE];

    /** Chip specific functions and definitions. */
    sf_touch_panel_i2c_chip_t const  * p_chip;
} sf_touch_panel_i2c_ctrl_t;

/** Configuration for RTOS touch panel driver. */
typedef struct st_sf_touch_panel_i2c_cfg
{
    /** Pointer to lower level I2C. */
    i2c_master_instance_t const  * p_lower_lvl_i2c;

    /** Pointer to lower level external IRQ. */
    sf_external_irq_instance_t const  * p_lower_lvl_irq;

    /** Pointer to memory allocated for the lower level control block. DO NOT INITIALIZE.  Initialization occurs when
     * SF_TOUCH_PANEL_I2C_Open is called. */
    sf_touch_panel_i2c_ctrl_t * const  p_lower_lvl_ctrl;

    /** Port pin connected to reset line on touch controller chip. Set to SF_TOUCH_PANEL_I2C_RESET_PIN_UNUSED if
     * unused. */
    ioport_port_pin_t  pin;

    /** Selected touch controller chip. */
    sf_touch_panel_i2c_chip_t const * const  p_chip;
} sf_touch_panel_i2c_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const sf_touch_panel_api_t      g_sf_touch_panel_on_sf_touch_panel_i2c;
extern const sf_touch_panel_i2c_chip_t g_sf_touch_panel_i2c_chip_ft5x06;
extern const sf_touch_panel_i2c_chip_t g_sf_touch_panel_i2c_chip_sx8654;
/** @endcond */

/*******************************************************************************************************************//**
 * @} (end defgroup SF_TOUCH_PANEL_I2C)
 **********************************************************************************************************************/
#endif /* SF_TOUCH_PANEL_I2C_API_H */
