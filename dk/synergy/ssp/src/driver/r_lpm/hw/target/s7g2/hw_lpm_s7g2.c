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
 * File Name    : hw_lpm_s7g2.c
 * Description  : LPM S7G2 hardware implementation.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Only build this file if this device is chosen. */
#if defined(BSP_MCU_GROUP_S7G2)

/* Private header file for this module. */
#include "../../hw_lpm_private.h"
#include "hw_lpm_s7g2.h"

/* Device specific header file */

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

bool HW_LPM_CHECK_OPERATING_POWER_MODE(lpm_operating_power_t power_mode)
{
    bool valid = false;
    if((LPM_OPERATING_POWER_HIGH_SPEED_MODE == power_mode)  ||
       (LPM_OPERATING_POWER_LOW_SPEED_MODE  == power_mode)
        )
    {
        valid = true;
    }
    return valid;
}

bool HW_LPM_CHECK_LOW_POWER_MODE(lpm_low_power_mode_t low_power_mode)
{
    bool valid = false;
    if((LPM_LOW_POWER_MODE_SLEEP    == low_power_mode)  ||
       (LPM_LOW_POWER_MODE_STANDBY  == low_power_mode)  ||
       (LPM_LOW_POWER_MODE_DEEP     == low_power_mode)
       )
    {
        valid = true;
    }
    return valid;
}

bool HW_LPM_CHECK_DEEP_POWER_SUPPLY(lpm_power_supply_t power_supply)
{
    bool valid = false;
    if((LPM_POWER_SUPPLY_DEEPCUT0   == power_supply)  ||
       (LPM_POWER_SUPPLY_DEEPCUT1   == power_supply)  ||
       (LPM_POWER_SUPPLY_DEEPCUT3   == power_supply)
       )
    {
        valid = true;
    }
    return valid;
}

bool HW_LPM_CHECK_DEEP_STANDBY_PIN(lpm_deep_standby_t pin_signal)
{
    bool valid = false;
    uint32_t r_bits;
    uint32_t r_reg;

    r_reg  = (uint32_t)pin_signal >> 4U;   /* the register nibble */
    r_bits = (uint32_t)pin_signal & 0x0fU; /* the bit position nibble */

    switch (r_reg)
    {
        case 0U:
            // Fall through to case 1
        case 1U:
            if(0U == (0x8U & r_bits))
            {
                valid = true;
            }
            break;
        case 2U:
            if(4U >= r_bits)
            {
                valid = true;
            }
            break;
        case 3U:
            if(2U >= r_bits)
            {
                valid = true;
            }
            break;
        default:
            break;
    }
    return valid;
}

#endif
