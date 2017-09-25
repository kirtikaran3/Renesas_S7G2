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
* File Name    : sf_uart_comms_cfg_ref.h
* Description  : Synergy Framework UART configuration
***********************************************************************************************************************/

#ifndef SF_UART_COMMS_CFG_H
#define SF_UART_COMMS_CFG_H

/*******************************************************************************************************************//**
 * @ingroup SF_UART_COMMS
 * @defgroup SF_UART_COMMS_CFG Build Time Configurations
 * @{
 **********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Specify whether to include code for API parameter checking
*  Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
*  Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define SF_UART_COMMS_CFG_PARAM_CHECKING_ENABLE                (BSP_CFG_PARAM_CHECKING_ENABLE)

/* Maximum amount of received UART data stored before read must be called to clear the queue. */
#define SF_UART_COMMS_CFG_QUEUE_SIZE_WORDS    (5)

/*******************************************************************************************************************//**
 * @} (end defgroup SF_UART_COMMS_CFG)
 **********************************************************************************************************************/



#endif /* SF_UART_COMMS_CFG_H */
