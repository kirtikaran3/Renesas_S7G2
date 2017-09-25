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
 * File Name    : hw_dtc_common.h
 * Description  : DTC register access functions.
 **********************************************************************************************************************/

#ifndef HW_DTC_COMMON_H
#define HW_DTC_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Number of ELC software events available for software start transfers. */
#define DTC_MAX_NUM_SOFTWARE_EVENTS (2)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static IRQn_Type dtc_elc_irq_lookup[DTC_MAX_NUM_SOFTWARE_EVENTS] =
{
#if BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_0 != BSP_IRQ_DISABLED
    ELC_SOFTWARE_EVENT_0_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_1 != BSP_IRQ_DISABLED
    ELC_SOFTWARE_EVENT_1_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
};

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Powers on the DTC block.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_DTC_PowerOn (void)
{
    R_SYSTEM->MSTPCRA_b.MSTPA22 = 0;
}

/*******************************************************************************************************************//**
 * Sets vector table base address
 * @param  p_vectors    Vector table base address.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_DTC_VectorTableAddressSet (void * p_vectors)
{
    R_DTC->DTCVBR = (uint32_t) p_vectors;
}

/*******************************************************************************************************************//**
 * Sets vector table base address
 * @returns Base address of vector table
 **********************************************************************************************************************/
__STATIC_INLINE void * HW_DTC_VectorTableAddressGet (void)
{
    return (void *) R_DTC->DTCVBR;
}

/*******************************************************************************************************************//**
 * Sets read skip enable bit (allows vector table read step to be skipped if vector is same as last transfer)
 * @param  read_skip    Whether to allow read skip
 **********************************************************************************************************************/
__STATIC_INLINE void HW_DTC_ReadSkipEnableSet (dtc_read_skip_t const read_skip)
{
    R_DTC->DTCCR_b.RRS = read_skip;
}

/*******************************************************************************************************************//**
 * Starts or stops the DTC (globally for all transfers).
 * @param  on_off       Whether to start or stop the DTC
 **********************************************************************************************************************/
__STATIC_INLINE void HW_DTC_StartStop (dtc_startstop_t on_off)
{
    R_DTC->DTCST_b.DTCST = on_off;
}

/*******************************************************************************************************************//**
 * Enables DTC transfer in ICU
 * @param  irq          Interrupt source used to activate transfer
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ICU_DTCEnable (IRQn_Type irq)
{
    R_ICU->IELSRn_b[irq].IR = 0;
    R_ICU->IELSRn_b[irq].DTCE = 1;
}

/*******************************************************************************************************************//**
 * Disables DTC transfer in ICU
 * @param  irq          Interrupt source used to deactivate transfer
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ICU_DTCDisable (IRQn_Type irq)
{
    R_ICU->IELSRn_b[irq].DTCE = 0;
}

/*******************************************************************************************************************//**
 * Returns DTC status register.
 * @returns DTC status register
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_DTC_StatusGet (void)
{
    return R_DTC->DTCSTS;
}

/*******************************************************************************************************************//**
 * Clears interrupts for specified channel at ICU.
 * @param  irq_num       The channel corresponds to the hardware channel number.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_DTC_ICUInterruptClear (IRQn_Type irq_num)
{
    R_ICU->IELSRn_b[irq_num].IR = 0;
}

/*******************************************************************************************************************//**
 * Returns the IRQ number required for NVIC calls.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @return  The IRQ number required for NVIC calls.
 **********************************************************************************************************************/
__STATIC_INLINE IRQn_Type HW_DTC_ElcIrqGet (int32_t index)
{
    return dtc_elc_irq_lookup[index];
}

#ifndef HW_DMAC_COMMON_H
__STATIC_INLINE elc_event_t HW_DTC_ICUEventGet (IRQn_Type irq)
{
    return (elc_event_t) R_ICU->IELSRn_b[irq].IELS;
}
#endif
#endif /* HW_DTC_COMMON_H */
