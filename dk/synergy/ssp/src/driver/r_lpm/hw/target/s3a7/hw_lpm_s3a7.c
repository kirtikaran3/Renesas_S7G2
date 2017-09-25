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
 * File Name    : hw_lpm_s3a7.c
 * Description  : LPM S3A7 hardware implementation.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Only build this file if this device is chosen. */
#if defined(BSP_MCU_GROUP_S3A7)

/* Private header file for this module. */
#include "../../hw_lpm_private.h"
#include "hw_lpm_s3a7.h"

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
    if((LPM_OPERATING_POWER_HIGH_SPEED_MODE     == power_mode)  ||
       (LPM_OPERATING_POWER_MIDDLE_SPEED_MODE   == power_mode)  ||
       (LPM_OPERATING_POWER_LOW_SPEED_MODE      == power_mode)  ||
       (LPM_OPERATING_POWER_LOW_VOLTAGE_MODE    == power_mode)
       )
    {
        valid = true;
    }
    return valid;
}

bool HW_LPM_CHECK_LOW_POWER_MODE(lpm_low_power_mode_t low_power_mode)
{
    bool valid = false;
    if((LPM_LOW_POWER_MODE_SLEEP     == low_power_mode)  ||
       (LPM_LOW_POWER_MODE_STANDBY   == low_power_mode)
       )
    {
        valid = true;
    }
    return valid;
}

#endif
