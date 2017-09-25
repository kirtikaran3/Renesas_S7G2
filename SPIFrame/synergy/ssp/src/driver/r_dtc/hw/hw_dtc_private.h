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
 * File Name    : hw_dtc_private.h
 * Description  : DTC register access prototypes.
 **********************************************************************************************************************/

#ifndef HW_DTC_PRIVATE_H
#define HW_DTC_PRIVATE_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_dtc.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_dtc_read_skip
{
    DTC_READ_SKIP_DISABLED = 0,
    DTC_READ_SKIP_ENABLED  = 1
} dtc_read_skip_t;

typedef enum e_dtc_startstop
{
    DTC_STOP  = 0,
    DTC_START = 1
} dtc_startstop_t;

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
__STATIC_INLINE void        HW_DTC_PowerOn (void);

__STATIC_INLINE void        HW_DTC_VectorTableAddressSet (void * const p_vectors);

__STATIC_INLINE void        * HW_DTC_VectorTableAddressGet (void);

__STATIC_INLINE void        HW_DTC_ReadSkipEnableSet (dtc_read_skip_t const read_skip);

__STATIC_INLINE void        HW_DTC_StartStop (dtc_startstop_t on_off);

__STATIC_INLINE void        HW_ICU_DTCEnable (IRQn_Type irq);

__STATIC_INLINE void        HW_ICU_DTCDisable (IRQn_Type irq);

__STATIC_INLINE uint16_t    HW_DTC_StatusGet (void);

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Select implementation based on BSP here */
#include "common/hw_dtc_common.h"
#endif /* HW_DTC_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup DTC)
 **********************************************************************************************************************/
