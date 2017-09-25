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
 * File Name    : r_sci_uart.h
 * Description  : UART on SCI HAL module public header file
 **********************************************************************************************************************/

#ifndef R_SCI_UART_H
#define R_SCI_UART_H

/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup UARTonSCI UART on SCI
 * @brief Driver for the UART on SCI.
 *
 * @section UARTonSCI_SUMMARY Summary
 * This module supports the UART on SCI. It implements the UART interface and drives SCI as a full-duplex UART
 * communication port. This module can drive all SCI channels as UART ports.
 *
 * Extends @ref UART_API.
 *
 * @note This module can use either the 16-stage hardware FIFO or a DTC transfer implementation to write multiple bytes.
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_uart_api.h"
#include "r_sci_common.h"
#include "r_sci_uart_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SCI_UART_CODE_VERSION_MAJOR (1)
#define SCI_UART_CODE_VERSION_MINOR (2)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** UART on SCI device Configuration */
typedef struct st_uart_on_sci_cfg
{
    sci_clk_src_t       clk_src;                            /**< Use SCI_CLK_SRC_INT/EXT8X/EXT16X    */
    bool                baudclk_out;                        /**< Baud rate clock output enable   */
    bool                rx_edge_start;                      /**< Start reception on falling edge */
    bool                noisecancel_en;                     /**< Noise cancel enable     */
    void (* p_extpin_ctrl)(uint32_t channel, uint32_t level); /**< Pointer to the user callback function to control external GPIO pin control used as RTS signal*/
} uart_on_sci_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const uart_api_t g_uart_on_sci;
/** @endcond */

/*******************************************************************************************************************//**
 * @} (end defgroup UARTonSCI)
 **********************************************************************************************************************/
#endif /* R_SCI_UART_H */
