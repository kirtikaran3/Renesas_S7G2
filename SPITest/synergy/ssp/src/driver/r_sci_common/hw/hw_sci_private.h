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
* File Name    : hw_sci_private.h
* Description  : SC32 SCI LLD driver private header file
***********************************************************************************************************************/

#ifndef HW_SCI_PRIVATE_H
#define HW_SCI_PRIVATE_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* platform dependent files */
#include "bsp_api.h"
/* SCI header file */
#include "r_sci_common.h"

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/** Baud rate divisor information
 * BRR(N) = (PCLK / (divisor * Baudrate(B))) - 1
 * when ABCSE=1,                         divisor = 12*pow(2,2N-1)
 * when ABCSE=1, BGDM=1&&ABCS=1,         divisor = 16*pow(2,2N-1)
 * when ABCSE=0, one of BGDM or ABCS =1, divisor = 32*pow(2,2N-1)
 * when ABCSE=0, BGDM=0&&ABCS=0,         divisor = 64*pow(2,2N-1)
 */
typedef struct st_baud_setting_t
{
    uint16_t     div_coefficient;   /**< Divisor coefficient */
    uint8_t     bgdm;               /**< BGDM value to get divisor */
    uint8_t     abcs;               /**< ABCS value to get divisor */
    uint8_t     abcse;              /**< ABCSE value to get divisor */
    uint8_t     cks;                /**< CKS  value to get divisor (CKS = N) */
} baud_setting_t;

/** SCI function ON/OFF control */
typedef enum e_sci_pow_ctrl
{
    SCI_OFF = 0,            /**< SCI function OFF */
    SCI_ON                  /**< SCI function ON */
} sci_pow_ctrl_t;

/** SCI interrupt definition */
typedef enum e_sci_int
{
    SCI_RX_INT,                     /**< RXI interrupt */
    SCI_TX_INT,                     /**< TXI interrupt */
    SCI_TE_INT,                     /**< TEI interrupt */
    SCI_ALL_INT,                    /**< All interrupt */
    SCI_ERR_INT                     /**< Error interrupt */
} sci_int_t;

/**********************************************************************************************************************
Private global variables
***********************************************************************************************************************/

/**********************************************************************************************************************
Public Functions
***********************************************************************************************************************/

#endif // R_SCI_PRIVATE_H
