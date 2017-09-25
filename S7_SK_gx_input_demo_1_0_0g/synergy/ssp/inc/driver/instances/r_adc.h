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
* File Name    : r_adc.h
* @brief       : Functions for configuring and using the ADC
***********************************************************************************************************************/

#ifndef R_ADC_H
#define R_ADC_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <stdlib.h>
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>
#include "bsp_api.h"
#include "r_adc_cfg.h"
#include "r_adc_api.h"

/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup ADC ADC
 * @brief Driver for the 14-Bit A/D Converter (ADC14) and 12-bit A/D Converter (ADC12).
 *
 * This module supports the ADC14 and ADC12 peripherals.  It implements
 * the following interfaces:
 *   - @ref ADC_API
 * @{
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Typical values that can be used to modify the sample states*/
#define ADC_SAMPLE_STATE_COUNT_MIN     (5)
#define ADC_SAMPLE_STATE_COUNT_MAX     (255)
#define ADC_SAMPLE_STATE_COUNT_DEFAULT (11)

/** Typical values that can be used for the sample and hold counts for the channels 0-2*/
#define ADC_SAMPLE_STATE_HOLD_COUNT_MIN      (4)    ///< Minimum sample and hold states
#define ADC_SAMPLE_STATE_HOLD_COUNT_MAX      (255)  ///< Maximum sample and hold states
#define ADC_SAMPLE_STATE_HOLD_COUNT_DEFAULT  (24)   ///< Default sample and hold states

/** For ADC Scan configuration      */
/** Use bitwise OR to combine these masks for desired channels and sensors.    */
/** Used for all commands containing a "mask" or "flags" field.          */
#define ADC_MASK_CHANNEL_0    (1<<0)    ///< ADC scan mask channel 0
#define ADC_MASK_CHANNEL_1    (1<<1)    ///< ADC scan mask channel 1
#define ADC_MASK_CHANNEL_2    (1<<2)    ///< ADC scan mask channel 2
#define ADC_MASK_CHANNEL_3    (1<<3)    ///< ADC scan mask channel 3
#define ADC_MASK_CHANNEL_4    (1<<4)    ///< ADC scan mask channel 4
#define ADC_MASK_CHANNEL_5    (1<<5)    ///< ADC scan mask channel 5
#define ADC_MASK_CHANNEL_6    (1<<6)    ///< ADC scan mask channel 6
#define ADC_MASK_CHANNEL_7    (1<<7)    ///< ADC scan mask channel 7
#define ADC_MASK_CHANNEL_8    (1<<8)    ///< ADC scan mask channel 8
#define ADC_MASK_CHANNEL_9    (1<<9)    ///< ADC scan mask channel 9
#define ADC_MASK_CHANNEL_10   (1<<10)   ///< ADC scan mask channel 10
#define ADC_MASK_CHANNEL_11   (1<<11)   ///< ADC scan mask channel 11
#define ADC_MASK_CHANNEL_12   (1<<12)   ///< ADC scan mask channel 12
#define ADC_MASK_CHANNEL_13   (1<<13)   ///< ADC scan mask channel 13
#define ADC_MASK_CHANNEL_14   (1<<14)   ///< ADC scan mask channel 14
#define ADC_MASK_CHANNEL_15   (1<<15)   ///< ADC scan mask channel 15
#define ADC_MASK_CHANNEL_16   (1<<16)   ///< ADC scan mask channel 16
#define ADC_MASK_CHANNEL_17   (1<<17)   ///< ADC scan mask channel 17
#define ADC_MASK_CHANNEL_18   (1<<18)   ///< ADC scan mask channel 18
#define ADC_MASK_CHANNEL_19   (1<<19)   ///< ADC scan mask channel 19
#define ADC_MASK_CHANNEL_20   (1<<20)   ///< ADC scan mask channel 20
#define ADC_MASK_CHANNEL_21   (1<<21)   ///< ADC scan mask channel 21
#define ADC_MASK_CHANNEL_22   (1<<22)   ///< ADC scan mask channel 22
#define ADC_MASK_CHANNEL_23   (1<<23)   ///< ADC scan mask channel 23
#define ADC_MASK_CHANNEL_24   (1<<24)   ///< ADC scan mask channel 24
#define ADC_MASK_CHANNEL_25   (1<<25)   ///< ADC scan mask channel 25
#define ADC_MASK_CHANNEL_26   (1<<26)   ///< ADC scan mask channel 26
#define ADC_MASK_CHANNEL_27   (1<<27)   ///< ADC scan mask channel 27
#define ADC_MASK_TEMPERATURE  (1<<28)   ///< ADC scan mask temperature sensor
#define ADC_MASK_VOLT         (1<<29)   ///< ADC scan mask internal reference voltage
#define ADC_MASK_SENSORS (ADC_MASK_TEMPERATURE | ADC_MASK_VOLT)
#define ADC_MASK_GROUP_B_OFF  (0)       ///< ADC scan mask group B off
#define ADC_MASK_ADD_OFF      (0)       ///< ADC scan mask add off
#define ADC_MASK_SAMPLE_HOLD_OFF (0)    ///< ADC scan mask sample hold off

/** Sample and hold Channel mask */
#define ADC_SAMPLE_HOLD_CHANNELS     (0x07)     ///< Sample and hold is only available for channel 0,1,2
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Interface Structure for user access */
extern const adc_api_t g_adc_on_adc;
/** @endcond */


/*******************************************************************************************************************//**
 * @} (end defgroup ADC)
 **********************************************************************************************************************/
#endif /* R_ADC_H */

