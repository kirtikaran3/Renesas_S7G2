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
 * File Name    : hw_crc_private.h
 * Description  : CRC LLD private header
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup CRC
 * @{
 **********************************************************************************************************************/

#ifndef HW_CRC_PRIVATE_H
#define HW_CRC_PRIVATE_H

#if defined(__GNUC__) && defined(__ARM_EABI__)
#define INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define INLINE_ATTRIBUTE
#endif

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_crc.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if BSP_MCU_MOCKED
#undef __STATIC_INLINE
#define __STATIC_INLINE
#ifndef BSP_MAX_NUM_IRQn
#define BSP_MAX_NUM_IRQn (-1)
#endif
#endif

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
__STATIC_INLINE void             HW_CRC_PolynomialSet (crc_polynomial_t polynomial) INLINE_ATTRIBUTE;

__STATIC_INLINE crc_polynomial_t HW_CRC_PolynomialGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void             HW_CRC_BitorderSet (crc_bit_order_t bitOrder) INLINE_ATTRIBUTE;

__STATIC_INLINE void             HW_CRC_8bitCalculatedValueSet (uint8_t presetValue) INLINE_ATTRIBUTE;

__STATIC_INLINE void             HW_CRC_16bitCalculatedValueSet (uint16_t presetValue) INLINE_ATTRIBUTE;

__STATIC_INLINE void             HW_CRC_32bitCalculatedValueSet (uint32_t presetValue) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t          HW_CRC_8bitCalculatedValueGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint16_t         HW_CRC_16bitCalculatedValueGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint32_t         HW_CRC_32bitCalculatedValueGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void             HW_CRC_8bitInputWrite (uint8_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void             HW_CRC_32bitInputWrite (uint32_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void             HW_CRC_SnoopEnable (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void             HW_CRC_SnoopDisable (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void             HW_CRC_SnoopChannelSet (uint32_t              channel,
                                                         crc_snoop_direction_t direction) INLINE_ATTRIBUTE;

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "common/hw_crc_common.h"
#endif /* HW_CRC_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup CRC)
 **********************************************************************************************************************/
