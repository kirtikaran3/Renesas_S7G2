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
 * File Name    : hw_dac_common.h
 * Description  : Low Level Driver functions
 **********************************************************************************************************************/

#ifndef HW_DAC_COMMON_H
#define HW_DAC_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define DAC_BASE 0x4005E000UL
#define HW_DAC   ((R_DAC_Type *) DAC_BASE)

/** DAC Open API channel number. Note: S124 MCUs only use channel 0. */
#define DAC_CHANNEL_0 ((uint8_t)0)
#define DAC_CHANNEL_1 ((uint8_t)1)


/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_dac_vref
{
    DAC_VREF_AVCC0_AVSS0 = 1,
    DAC_VREF_INTERNAL    = 3,
    DAC_VREF_VREFH_VREFL = 6,
} dac_vref_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

__STATIC_INLINE void HW_DAC_PowerOn (void)
{
    R_MSTP->MSTPCRD_b.MSTPD20 = 0;
}

__STATIC_INLINE void HW_DAC_PowerOff (void)
{
    R_MSTP->MSTPCRD_b.MSTPD20 = 1;
}

__STATIC_INLINE void HW_DAC_Ch0Control (hw_dac_control_t value)
{
    HW_DAC->DACR_b.DAOE0 = value;
}

__STATIC_INLINE void HW_DAC_Ch1Control (hw_dac_control_t value)
{
    HW_DAC->DACR_b.DAOE1 = value;
}

__STATIC_INLINE void HW_DAC_DADRWrite (uint8_t channel, uint16_t value)
{
    HW_DAC->DADRn[channel] = value;
}

__STATIC_INLINE void HW_DAC_DADRAddressGet (uint8_t channel, volatile uint16_t ** p_addr)
{
    *p_addr = &HW_DAC->DADRn[channel];
}

__STATIC_INLINE void HW_DAC_DADPRCfg (uint8_t value)
{
    HW_DAC->DADPR_b.DPSEL = (uint8_t)(0x01 & value);
}

__STATIC_INLINE void HW_DAC_DAADSCRCfg (uint8_t value)
{
    HW_DAC->DAADSCR_b.DAADST = (uint8_t)(0x01 & value);
}

__STATIC_INLINE void HW_DAC_DAAMPCRCfg (dac_cfg_t const * const p_cfg)
{
    /** Each channel has its own amplifier enable bit. */
    if (DAC_CHANNEL_0 == p_cfg->channel)
    {
        if (p_cfg->output_amplifier_enabled)
        {
            /** Enable channel 0 amplifier. */
            HW_DAC->DAAMPCR_b.DAAMP0 = 1;
        }
        else
        {
            /** Disable channel 0 amplifier. */
            HW_DAC->DAAMPCR_b.DAAMP0 = 0;
        }
    }
    else
    {
        if (p_cfg->output_amplifier_enabled)
        {
            /** Enable channel 1 amplifier. */
            HW_DAC->DAAMPCR_b.DAAMP1 = 1;
        }
        else
        {
            /** Disable channel 1 amplifier. */
            HW_DAC->DAAMPCR_b.DAAMP1 = 0;
        }
    }
}

__STATIC_INLINE void HW_DAC_VrefSet(dac_vref_t vref)
{
#if BSP_MCU_GROUP_S3A7 || BSP_MCU_GROUP_S124
    HW_DAC->DAVREFCR_b.REF = vref;
#else
    SSP_PARAMETER_NOT_USED(vref);
#endif
}

#endif /* HW_DAC_COMMON_H */

