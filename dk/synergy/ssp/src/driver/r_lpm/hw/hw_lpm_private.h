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
 * File Name    : hw_lpm_private.h
 * Description  : LPM
 **********************************************************************************************************************/


#ifndef HW_LPM_PRIVATE_H
#define HW_LPM_PRIVATE_H

#if defined(__GNUC__) && defined(__ARM_EABI__)
/*LDRA_INSPECTED 293 S */
#define INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define INLINE_ATTRIBUTE
#endif

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_lpm.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
__STATIC_INLINE void     HW_LPM_RegisterLock (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_RegisterUnLock (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_MSTPCRASet (uint32_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE uint32_t HW_LPM_MSTPCRAGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_MSTPCRBSet (uint32_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE uint32_t HW_LPM_MSTPCRBGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_MSTPCRCSet (uint32_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE uint32_t HW_LPM_MSTPCRCGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_MSTPCRDSet (uint32_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE uint32_t HW_LPM_MSTPCRDGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_PowerModeSet (uint8_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_SubOscSet (uint8_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t  HW_LPM_SubOscGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_SNZESet (uint8_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_RXDREQENSet (uint8_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_SNZDTCENSet (uint8_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_SNZREQCRSet (uint32_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_SNZEDCRSet (uint8_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_SSBYSet (uint16_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void     HW_LPM_WUPENSet (uint32_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE uint32_t HW_LPM_WUPENGet (void) INLINE_ATTRIBUTE;

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "common/hw_lpm_common.h"
#endif /* HW_LPM_PRIVATE_H */
