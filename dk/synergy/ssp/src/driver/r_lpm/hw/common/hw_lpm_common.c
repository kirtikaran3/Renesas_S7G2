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
 * File Name    : hw_lpm_common.c
 * Description  : LPM
 **********************************************************************************************************************/


/*******************************************************************************************************************//**

 **********************************************************************************************************************/


/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

#include "hw_lpm_common.h"
/* Private header file for this module. */
#include "../hw_lpm_private.h"

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

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

 bool HW_LPM_CHECK_MSTP(lpm_mstp_t module)
{
    bool valid = false;
    uint32_t r_bit;
    uint32_t r_reg;
    uint32_t r_bit_shift;
    uint32_t mask;

    /* module is a 16-bit value which includes the MSTPCR register (a-d) in the upper byte and
     * the stop bit position in the lower byte */

    r_reg = (uint32_t)module >> 8U;          	/* r_reg holds the module stop bit register (a-d) to address */
    r_bit_shift = (uint32_t)module & 0xffU;
    r_bit = (uint32_t)(1U << r_bit_shift);     	/* r_bit is a bit mask that corresponds to the module to be stopped */

    switch (r_reg)
    {
        case 0x0aU:
            mask = HW_LPM_MSTPCRA_MASK;
            break;
        case 0x0bU:
            mask = HW_LPM_MSTPCRB_MASK;
            break;
        case 0x0cU:
            mask = HW_LPM_MSTPCRC_MASK;
            break;
        case 0x0dU:
            mask = HW_LPM_MSTPCRD_MASK;
            break;
        default:
            mask = 0U;
            break;
    }

    if(0U == (r_bit & ~mask))
    {
        valid = true;
    }

    return valid;
}

/*******************************************************************************************************************//**
 * Wait for a power transition to complete
 **********************************************************************************************************************/
void HW_LPM_WaitForPowerTransition (void)
{
    while ((0U != R_SYSTEM->OPCCR_b.OPCMTSF) || (0U != R_SYSTEM->SOPCCR_b.SOPCMTSF))
    {
        /* Wait for any pending transition to complete */
    }
}
