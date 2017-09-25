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
 * File Name    : hw_sci_common_s124.h
 * Description  : SCI COMMON S124 specific macros.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup SCI_COMMON
 * @{
 **********************************************************************************************************************/

#ifndef HW_SCI_COMMON_S124_H_
#define HW_SCI_COMMON_S124_H_

#include "bsp_api.h"
#include "r_elc.h"
/* Public interface header file for this module. */
#include "r_sci_common.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/** To stop compiler warning */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/
static elc_event_t rxi_event_list[] =
{
    ELC_EVENT_SCI0_RXI,
    ELC_EVENT_SCI1_RXI,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
    ELC_EVENT_SCI9_RXI,
};

static elc_event_t txi_event_list[] =
{
    ELC_EVENT_SCI0_TXI,
    ELC_EVENT_SCI1_TXI,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
	(elc_event_t) 0,
    ELC_EVENT_SCI9_TXI,
};

/***********************************************************************************************************************
Private Functions
***********************************************************************************************************************/
/*******************************************************************************************************************//**
* Event lookup for RXI.
* @param[in] channel  SCI channel
* @retval    ELC event
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE elc_event_t r_sci_rxi_event_lookup (uint32_t const channel)
{
    return rxi_event_list[channel];
}

/*******************************************************************************************************************//**
* Event lookup for TXI.
* @param[in] channel  SCI channel
* @retval    ELC event
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE elc_event_t r_sci_txi_event_lookup (uint32_t const channel)
{
    return txi_event_list[channel];
}

/*******************************************************************************************************************//**
* Omitted channel checker
* @param[in] channel  SCI channel
* @param[in] sci_int  SCI interrupt type (SCI_TX_INT|SCI_TE_INT|SCI_RE_INT|SCI_ERR_INT)
* @retval    IRQ number in interrupt controller
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE IRQn_Type r_sci_select_irqn (uint32_t const channel, sci_int_t sci_int)
{
    switch (channel)
    {
    case 0:
        if(SCI_TX_INT == sci_int)
        {
#if (BSP_IRQ_CFG_SCI0_TXI != BSP_IRQ_DISABLED)
            return SCI0_TXI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
        else if(SCI_TE_INT == sci_int)
        {
#if (BSP_IRQ_CFG_SCI0_TEI != BSP_IRQ_DISABLED)
            return SCI0_TEI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
        else if(SCI_RX_INT == sci_int)
        {
#if (BSP_IRQ_CFG_SCI0_RXI != BSP_IRQ_DISABLED)
            return SCI0_RXI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
        else
        {
#if (BSP_IRQ_CFG_SCI0_ERI != BSP_IRQ_DISABLED)
            return SCI0_ERI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
    break;
    case 1:
        if(SCI_TX_INT == sci_int)
        {
#if (BSP_IRQ_CFG_SCI1_TXI != BSP_IRQ_DISABLED)
            return SCI1_TXI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
        else if(SCI_TE_INT == sci_int)
        {
#if (BSP_IRQ_CFG_SCI1_TEI != BSP_IRQ_DISABLED)
            return SCI1_TEI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
        else if(SCI_RX_INT == sci_int)
        {
#if (BSP_IRQ_CFG_SCI1_RXI != BSP_IRQ_DISABLED)
            return SCI1_RXI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
        else
        {
#if (BSP_IRQ_CFG_SCI1_ERI != BSP_IRQ_DISABLED)
            return SCI1_ERI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
    break;
    case 9:
        if(SCI_TX_INT == sci_int)
        {
#if (BSP_IRQ_CFG_SCI9_TXI != BSP_IRQ_DISABLED)
            return SCI9_TXI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
        else if(SCI_TE_INT == sci_int)
        {
#if (BSP_IRQ_CFG_SCI9_TEI != BSP_IRQ_DISABLED)
            return SCI9_TEI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
        else if(SCI_RX_INT == sci_int)
        {
#if (BSP_IRQ_CFG_SCI9_RXI != BSP_IRQ_DISABLED)
            return SCI9_RXI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
        else
        {
#if (BSP_IRQ_CFG_SCI9_ERI != BSP_IRQ_DISABLED)
            return SCI9_ERI_IRQn;
#else
            return BSP_MAX_NUM_IRQn;
#endif
        }
    break;
    default:
    break;
    }
    return BSP_MAX_NUM_IRQn;
}  /* End of function r_sci_select_irqn() */
#endif /* HW_SCI_COMMON_S3A7_H_*/
/*******************************************************************************************************************//**
 * @} (end addtogroup SCI_COMMON)
 **********************************************************************************************************************/
