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
* File Name    : r_sci_uart_cfg_ref.h
* Description  : UART on SCI configuration
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup UARTonSCI
 * @defgroup UARTonSCI_CONFIG Build Time Configurations
 * @{
 **********************************************************************************************************************/

#ifndef R_SCI_UART_CFG_H
#define R_SCI_UART_CFG_H

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Specify whether to use one of the GPIO pins as UART RTS pin
 *  This is the enhancement for UART functionality with additional GPIO support
 * 1 : use GPIO as RTS pin
 * 0 : do not use GPIO as RTS pin
 */
#define SCI_UART_CFG_EXTERNAL_RTS_OPERATION   (1)

/** Specify whether to support reception
 * 1 : included
 * 0 : not included
 */
#define SCI_UART_CFG_RX_ENABLE       (1)

/** Specify whether to support transmission
 * 1 : included
 * 0 : not included
 */
#define SCI_UART_CFG_TX_ENABLE       (1)

/** Specify whether to include code for API parameter checking
 * Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
 * 1 : includes parameter checking
 * 0 : compiles out parameter checking
 */
#define SCI_UART_CFG_PARAM_CHECKING_ENABLE   (1)

#endif /* R_SCI_UART_CFG_H */

/*******************************************************************************************************************//**
 * @} (end defgroup UARTonSCI_CONFIG)
 **********************************************************************************************************************/
