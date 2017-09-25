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
 * File Name    : hw_agt_common.h
 * Description  : Asynchronous General Purpose Timer (AGT) Module hardware common header file.
 **********************************************************************************************************************/

#ifndef HW_AGT_COMMON_H
#define HW_AGT_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** Pointer to AGT channels */

#define AGT_CH(ch) (gs_agt_RegBase[ch])

static R_AGT0_Type     * gs_agt_RegBase[2] =
{
    R_AGT0, R_AGT1
};

static const IRQn_Type irq_lookup[] =
{
#if (BSP_IRQ_CFG_AGT0_INT != BSP_IRQ_DISABLED)
    AGT0_INT_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if (BSP_IRQ_CFG_AGT1_INT != BSP_IRQ_DISABLED)
    AGT1_INT_IRQn
#else
    BSP_MAX_NUM_IRQn
#endif
};

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Powers on the AGT block.
 * @param  ch    The channel corresponds to the hardware channel number.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_PowerOn (uint8_t const ch)
{
    /* Powers on AGT channel */
    if (0 == ch)
    {
        /* MSTPD3 controls AGT0 */
        R_MSTP->MSTPCRD_b.MSTPD3 = 0;
    }
    else
    {
        /* MSTPD2 controls AGT1 */
        R_MSTP->MSTPCRD_b.MSTPD2 = 0;
    }
}

/*******************************************************************************************************************//**
 * Powers off the AGT block.
 * @param  ch    The channel corresponds to the hardware channel number.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_PowerOff (uint8_t const ch)
{
    /* Powers off AGT channel */

    if (0 == ch)
    {
        /* MSTPD3 controls AGT0 */
        R_MSTP->MSTPCRD_b.MSTPD3 = 1;
    }
    else
    {
        /* MSTPD2 controls AGT1 */
        R_MSTP->MSTPCRD_b.MSTPD2 = 1;
    }
}

/*******************************************************************************************************************//**
 * Clears interrupts for specified channel at ICU.
 * @param  irq_num       The channel corresponds to the hardware channel number.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ICU_InterruptClear (IRQn_Type irq_num)
{
    /* Clear the interrupt request flag */
    R_ICU->IELSRn_b[irq_num].IR = 0;
}

/*******************************************************************************************************************//**
 * Returns the value of the start bit, indicating if the counter is counting or not.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @return  Start bit value
 **********************************************************************************************************************/
__STATIC_INLINE agt_start_status_t HW_AGT_CounterStartBitGet (uint8_t const ch)
{
    return (agt_start_status_t) AGT_CH(ch)->AGTCR_b.TSTART;
}

/*******************************************************************************************************************//**
 * Starts or stops the counter.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @param   start_stop   Specify start or stop operation
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_CounterStartStop (uint8_t const ch, agt_start_status_t start_stop)
{
    AGT_CH(ch)->AGTCR_b.TSTART = start_stop;
}

/*******************************************************************************************************************//**
 * Clears flags in AGT control register.
 * @param   ch           The channel corresponds to the hardware channel number.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_CounterFlagsClear (uint8_t const ch)
{
    AGT_CH(ch)->AGTCR &= 0x0F;
}

/*******************************************************************************************************************//**
 * Sets the counter value.
 * @param  ch           The channel corresponds to the hardware channel number.
 * @param  value        Any number from 0 to 0xFFFF.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_CounterSet (uint8_t const ch, uint32_t value)
{
    /* Set counter value */
    AGT_CH(ch)->AGT = (uint16_t)(value & 0x0FFFF);
}

/*******************************************************************************************************************//**
 * Returns the current counter value.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @return  Current counter value in the range 0 to 0xFFFF.
 **********************************************************************************************************************/
__STATIC_INLINE timer_size_t HW_AGT_CounterGet (uint8_t const ch)
{
    /* Get counter value */
    return AGT_CH(ch)->AGT;
}

/*******************************************************************************************************************//**
 * Reads count status flag.
 * @param   ch           The channel corresponds to the hardware channel number.
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_AGT_CountStatusGet (uint8_t const ch)
{
   return  AGT_CH(ch)->AGTCR_b.TCSTF;
}

/*******************************************************************************************************************//**
 * Sets the timer count source.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @param   value        Specify the count source
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_CountSourceSet (uint8_t const ch, uint8_t value)
{
    AGT_CH(ch)->AGTMR1_b.TCK = (uint8_t)(value & 0x07);
}

/*******************************************************************************************************************//**
 * Gets the timer count source.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @return  Current count source
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_AGT_CountSourceGet (uint8_t const ch)
{
    return AGT_CH(ch)->AGTMR1_b.TCK;
}

/*******************************************************************************************************************//**
 * Sets the division ratio for fSUB/LOCO clock.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @param   value        Specify division ratio
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_ClockDivideSet (uint8_t const ch, uint8_t value)
{
    AGT_CH(ch)->AGTMR2_b.CKS = (uint8_t)(value & 0x07);
}

/*******************************************************************************************************************//**
 * Gets the division ratio for fSUB/LOCO clock.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @retval  Division ratio
 **********************************************************************************************************************/
__STATIC_INLINE agt_clock_division_t HW_AGT_ClockDivideGet (uint8_t const ch)
{
    uint8_t cks = AGT_CH(ch)->AGTMR2_b.CKS;
    agt_clock_division_t ret_value = (agt_clock_division_t)cks;
    return (ret_value);
}

/*******************************************************************************************************************//**
 * Sets the low power mode.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @param   value        Specify low power mode
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_LowPowerModeSet (uint8_t const ch, uint8_t value)
{
    AGT_CH(ch)->AGTMR2_b.LPM = (uint8_t)(value & 0x01);
}

/*******************************************************************************************************************//**
 * Sets the I/O mode.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @param   value        Specify TOE mode
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_IOModeSet (uint8_t const ch, uint8_t value)
{
    AGT_CH(ch)->AGTIOC_b.TOE = (uint8_t)(value & 0x01);
}

/*******************************************************************************************************************//**
 * Sets the I/O mode.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @param   value        Specify TOE mode
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_IOTEdgeSelectSet (uint8_t const ch, uint8_t value)
{
    AGT_CH(ch)->AGTIOC_b.TEDGSEL = (uint8_t)(value & 0x01);
}

/*******************************************************************************************************************//**
 * Sets the timer mode.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @param   value        Specify mode of operation
 **********************************************************************************************************************/
__STATIC_INLINE void HW_AGT_ModeSet (uint8_t const ch, uint8_t value)
{
    AGT_CH(ch)->AGTMR1_b.TMOD = (uint8_t)(value & 0x07);
}

/*******************************************************************************************************************//**
 * Returns the IRQ number required for NVIC calls.
 * @param   ch           The channel corresponds to the hardware channel number.
 * @return  The IRQ number required for NVIC calls.
 **********************************************************************************************************************/
__STATIC_INLINE IRQn_Type HW_AGT_IRQGet (uint8_t const ch)
{
    return irq_lookup[ch];
}
#endif /* HW_AGT_COMMON_H */
