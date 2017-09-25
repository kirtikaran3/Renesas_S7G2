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
* File Name    : r_sci_private.h
* Description  : SCI HAL module private header file
***********************************************************************************************************************/

#ifndef R_SCI_PRIVATE_H
#define R_SCI_PRIVATE_H

#include "./hw/hw_sci_private.h"
#include "./hw/hw_sci_common.h"
#include "r_sci_common_cfg.h"

#if (SCI_CFG_ASYNC_INCLUDED)
#include "../r_sci_uart/hw/hw_sci_uart_private.h"
#endif
#if (SCI_CFG_SPI_INCLUDED)
#include "../r_sci_spi/hw/hw_sci_spi_private.h"
#endif
#if (SCI_CFG_SIIC_INCLUDED)
#include "../r_sci_i2c/hw/hw_sci_i2c_private.h"
#endif

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SCI_FIFO_STAGE_NUM      (16)     ///< SCI FIFO stage number

#if (SCI_CFG_ASYNC_INCLUDED)
#define NUM_DIVISORS_ASYNC      (13)
#endif

/**********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Public Functions
***********************************************************************************************************************/
ssp_err_t   r_sci_hardware_lock (uint32_t channel);
void        r_sci_hardware_unlock (uint32_t channel);
void        r_sci_register_default_set (uint32_t const channel);
void        r_sci_close_common (uint32_t channel);
ssp_err_t   r_sci_channel_param_check (uint32_t channel);

/* Interrupt handlers for UART interface */
void        r_sci_uart_txi_common (uint32_t const channel);
void        r_sci_uart_rxi_common (uint32_t const channel);
void        r_sci_uart_tei (uint32_t const channel);
void        r_sci_uart_eri_common (uint32_t const channel);

/* Interrupt handlers for SCI SPI interface */
void        r_sci_spi_tx_rx_common(uint32_t channel, sci_int_t whichInterrupt);
void        r_sci_spi_spei_isr_common(uint32_t channel);

/* Interrupt handlers for SIIC interface */
void        r_sci_siic_rxi_master(uint32_t const channel);
void        r_sci_siic_txi_master(uint32_t const channel);
void        r_sci_siic_tei_master(uint32_t const channel);
void        r_sci_siic_err_master(uint32_t const channel);

#endif /* R_SCI_PRIVATE_H */
