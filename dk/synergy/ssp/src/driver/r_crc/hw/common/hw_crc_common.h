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
 * File Name    : hw_crc_common.h
 * Description  : CRC LLD layer
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup CRC
 * @{
 **********************************************************************************************************************/

#ifndef HW_CRC_COMMON_H
#define HW_CRC_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* TODO_CRC RELEASE Remove mock reference */
#if !BSP_MCU_MOCKED

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if defined(BSP_BOARD_FPGA2R7FS)
#define CRC_BASE_REG  (R_CRC_BASE)
#define MSTP_BASE_REG (R_MSTP_BASE)
#else
#define CRC_BASE_REG  (R_CRC_BASE)
#define MSTP_BASE_REG (R_MSTP_BASE)
#endif
#define CRC_REG_SIZE  (0x100)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** Pointer to MSTP channels.  TODO_CRC Remove legacy code. */
#define P_MSTP ((R_MSTP_Type *) (MSTP_BASE_REG))

/** Pointer to CRC channels.  TODO_CRC Remove legacy code. */
#define P_CRC ((R_CRC_Type *) (CRC_BASE_REG))

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Powers on the CRC block.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_PowerOn (void)
{
    /* TODO: Replace these direct register calls with calls to the LPM equivalent calls when they become available */
	P_MSTP->MSTPCRC_b.MSTPC1 = 0;
}

/*******************************************************************************************************************//**
 * Powers off the CRC block.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_PowerOff (void)
{
    /* TODO: Replace these direct register calls with calls to the LPM equivalent calls when they become available */
	P_MSTP->MSTPCRC_b.MSTPC1 = 1;
}

/*******************************************************************************************************************//**
 * Sets the polynomial value.
 * @param  polynomial   The polynomial to be used for calculation.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_PolynomialSet (crc_polynomial_t polynomial)
{
    /* Set polynomial value */
    P_CRC->CRCCR0_b.GPS = polynomial;
}

/*******************************************************************************************************************//**
 * Gets the currently set polynomial value.
 **********************************************************************************************************************/
__STATIC_INLINE crc_polynomial_t HW_CRC_PolynomialGet (void)
{
    /* Set polynomial value */
    return (crc_polynomial_t)P_CRC->CRCCR0_b.GPS;
}

/*******************************************************************************************************************//**
 * Sets the bit order value.
 * @param  bitOrder   The bitOrder to be used for calculation.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_BitorderSet (crc_bit_order_t bitOrder)
{
    /* Set bit order value */
    P_CRC->CRCCR0_b.LMS = bitOrder;
}

/*******************************************************************************************************************//**
 * Sets the starting CRC Calculated value.
 * @param  presetValue   The value written to the CRC Data Output Register.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_CalculatedValueSet (uint32_t presetValue)
{
    /* Set bit order value */
    P_CRC->CRCDOR = presetValue;
}

/*******************************************************************************************************************//**
 * Gets the current 8-bit CRC Calculated value.
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_CRC_8bitCalculatedValueGet (void)
{
    /* Set bit order value */
    return P_CRC->CRCDOR_BY;
}

/*******************************************************************************************************************//**
 * Gets the current 16-bit CRC Calculated value.
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_CRC_16bitCalculatedValueGet (void)
{
    /* Set bit order value */
    return P_CRC->CRCDOR_HA;
}

/*******************************************************************************************************************//**
 * Gets the current 32-bit CRC Calculated value.
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t HW_CRC_32bitCalculatedValueGet (void)
{
    /* Set bit order value */
    return P_CRC->CRCDOR;
}

/*******************************************************************************************************************//**
 * Sets the starting 8-bit CRC Calculated value.
 * @param  presetValue   The value written to the CRC Data Output Register.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_8bitCalculatedValueSet (uint8_t presetValue)
{
    /* Set bit order value */
    P_CRC->CRCDOR = 0;
    P_CRC->CRCDOR_BY = presetValue;
}

/*******************************************************************************************************************//**
 * Sets the starting 16-bit CRC Calculated value.
 * @param  presetValue   The value written to the CRC Data Output Register.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_16bitCalculatedValueSet (uint16_t presetValue)
{
    /* Set bit order value */
    P_CRC->CRCDOR = 0;
    P_CRC->CRCDOR_HA = presetValue;
}

/*******************************************************************************************************************//**
 * Sets the starting 32-bit CRC Calculated value.
 * @param  presetValue   The value written to the CRC Data Output Register.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_32bitCalculatedValueSet (uint32_t presetValue)
{
    /* Set bit order value */
    P_CRC->CRCDOR = presetValue;
}

/*******************************************************************************************************************//**
 * Write an 8-bit value to the input register of the CRC Calculator.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_8bitInputWrite (uint8_t value)
{
    /* Set bit order value */
    P_CRC->CRCDIR_BY = value;
}

/*******************************************************************************************************************//**
 * Write a 32-bit value to the input register of the CRC Calculator.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_32bitInputWrite (uint32_t value)
{
    /* Set bit order value */
    P_CRC->CRCDIR = value;
}

/*******************************************************************************************************************//**
 * Enable snooping
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_SnoopEnable (void)
{
    /* Set bit order value */
    P_CRC->CRCCR1_b.CRCSEN = 1;
}

/*******************************************************************************************************************//**
 * Disable snooping
 **********************************************************************************************************************/
__STATIC_INLINE void HW_CRC_SnoopDisable (void)
{
    /* Set bit order value */
    P_CRC->CRCCR1_b.CRCSEN = 0;
}

/*******************************************************************************************************************//**
 * Set the SCI channel to snoop
 **********************************************************************************************************************/
#define CRC_SNOOP_STARTING_CHANNEL_ADDRESS (0x03)
#define CRC_SNOOP_CHANNEL_SPACING          (0x20)
__STATIC_INLINE void HW_CRC_SnoopChannelSet (uint32_t channel, crc_snoop_direction_t direction)
{
    /* Set bit order value */
    P_CRC->CRCCR1_b.CRCSWR = direction;

    /*
     * Only the following address can be used to CRCSA[13:0] bit
     *  4007 0003h: SCI0.TDR, 4007 0005h: SCI0.RDR
     *  4007 0023h: SCI1.TDR, 4007 0025h: SCI1.RDR
     *  4007 0043h: SCI2.TDR, 4007 0045h: SCI2.RDR
     *  4007 0063h: SCI3.TDR, 4007 0065h: SCI3.RDR
     *  4007 0083h: SCI4.TDR, 4007 0085h: SCI4.RDR
     *  4007 00A3h: SCI5.TDR, 4007 00A5h: SCI5.RDR
     *  4007 00C3h: SCI6.TDR, 4007 00C5h: SCI6.RDR
     *  4007 00E3h: SCI7.TDR, 4007 00E5h: SCI7.RDR
     *  4007 0103h: SCI8.TDR, 4007 0105h: SCI8.RDR
     *  4007 0123h: SCI9.TDR, 4007 0125h: SCI9.RDR
     *
     *  direction = 0 for read and 1 for write
     */

    P_CRC->CRCSAR_b.CRCSA =
        0x3FFF & (CRC_SNOOP_STARTING_CHANNEL_ADDRESS +      /* Start at the first channel address */
        (channel * CRC_SNOOP_CHANNEL_SPACING) +             /* Add the channel offset */
        (direction ? 0 : 2));                               /* Add the direction offset (0=read, 1=write) */
}
#endif /* BSP_MCU_MOCKED */
#endif /* HW_CRC_COMMON_H */
