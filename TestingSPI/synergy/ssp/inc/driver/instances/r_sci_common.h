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
 * File Name    : r_sci_common.h
 * Description  : SCI HAL module common public header file
 **********************************************************************************************************************/

#ifndef R_SCI_COMMON_H
#define R_SCI_COMMON_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

#include "r_sci_common_cfg.h"

#if (SCI_CFG_ASYNC_INCLUDED)
#include "r_sci_uart_cfg.h"
#endif
#if (SCI_CFG_SSPI_INCLUDED)
#include "r_sci_spi_cfg.h"
#endif
#if (SCI_CFG_SIIC_INCLUDED)
#include "r_sci_i2c_cfg.h"
#endif
/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup SCI_COMMON SCI common
 * @brief Driver for the common functions of the Serial Communications Interface (SCI).
 *
 * @section SCI_COMMON_SUMMARY Summary
 *
 * This module is common to the  SCI modes described below.
 *   * UART  - Asynchronous mode. @ref UART_API is defined in r_uart_api.h.
 *   * SSPI  - Simple SPI mode.   @ref SPI_API is defined in r_spi_api.h.
 *   * SI2C  - Simple I2C mode.   @ref I2C_API is defined in r_i2c_api.h.
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#ifndef BSP_BOARD_MOCKED
#if   (BSP_MCU_GROUP_S7G2 == 1)
#define SCI_CH_MAX (10)                     /**< SCI total channel number (S7G2) */
#elif (BSP_MCU_GROUP_S3A7 == 1)
#define SCI_CH_MAX (6)                      /**< SCI total channel number (S3A7) */
#elif (BSP_MCU_GROUP_S124 == 1)
#define SCI_CH_MAX (3)                      /**< SCI total channel number (S124) */
#else
#error "No existing device group specified"
#endif
#endif // ifndef BSP_BOARD_MOCKED

#define SCI_PHY_CH_MAX (10)                 /**< Maximum number of SCI physical channel */

#ifndef SCI_COMMON_ERROR_RETURN
#define SCI_COMMON_ERROR_RETURN(a, err)       SSP_ERROR_RETURN((a), (err), "sci_common", NULL)
#endif

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** SCI operational modes */
typedef enum e_sci_mode
{
    SCI_MODE_OFF = 0,       /**< No mode is specified */
    SCI_MODE_ASYNC,         /**< UART mode */
    SCI_MODE_SYNC,          /**< Clock synchronous mode */
    SCI_MODE_SSPI,          /**< Simple SPI mode */
    SCI_MODE_SIIC,          /**< Simple I2C mode */
    SCI_MODE_SMART,         /**< Smart card interface mode (NOT SUPPORTED in this version) */
    SCI_MODE_MAX            /**< Number of SCI mode */
} sci_mode_t;

/** Enumeration for SCI clock source */
typedef enum e_sci_clk_src
{
    SCI_CLK_SRC_INT,        /**< Use internal clock for baud generation   */
    SCI_CLK_SRC_EXT,        /**< Use external clock for baud generation   */
    SCI_CLK_SRC_EXT8X,      /**< Use external clock 8x baud rate          */
    SCI_CLK_SRC_EXT16X      /**< Use external clock 16x baud rate         */
} sci_clk_src_t;

/** Noise filter setting definition */
typedef enum e_noise_cancel_lvl
{
    NOISE_CANCEL_LVL1,          /**< Noise filter level 1(weak) */
    NOISE_CANCEL_LVL2,          /**< Noise filter level 2 */
    NOISE_CANCEL_LVL3,          /**< Noise filter level 3 */
    NOISE_CANCEL_LVL4           /**< Noise filter level 4(strong) */
} noise_cancel_lvl_t;

/** SCI channel control block */
typedef struct st_sci_ctrl                                  /**<SCI channel control (for handle) */
{
    sci_mode_t  mode;                                       /**< Operational mode */
    bool        tx_busy;                                    /**< Transmission busy flag */
    void        * p_context;                                /**< Pointer to the function level device context (e.g.
                                                             *uart_ctrl_t type data ) */
    void        (* p_extpin_ctrl)(uint32_t channel, uint32_t level); /**< External pin control */
    bsp_lock_t  resource_lock_tx;                           /**< Resource lock for transmission */
    bsp_lock_t  resource_lock_rx;                           /**< Resource lock for reception */
} sci_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Public Functions
 **********************************************************************************************************************/
void sci_irq_status_clear (uint32_t const channel);

/** @} (end) */
#endif /* R_SCI_H */
