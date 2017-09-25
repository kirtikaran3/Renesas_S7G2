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
#define ADC_SAMPLE_STATE_COUNT_MIN     (5U)

/** Typical values that can be used for the sample and hold counts for the channels 0-2*/
/** Minimum sample and hold states */
#define ADC_SAMPLE_STATE_HOLD_COUNT_MIN      (4U)
/** Default sample and hold states */
#define ADC_SAMPLE_STATE_HOLD_COUNT_DEFAULT  (24U)

/** For ADC Scan configuration adc_channel_cfg_t::scan_mask, scan_mask_group_b,
 * add_mask and sample_hold_mask      */
/** Use bitwise OR to combine these masks for desired channels and sensors.    */
#define ADC_MASK_CHANNEL_0    (1<<0U)
#define ADC_MASK_CHANNEL_1    (1<<1U)
#define ADC_MASK_CHANNEL_2    (1<<2U)
#define ADC_MASK_CHANNEL_3    (1<<3U)
#define ADC_MASK_CHANNEL_4    (1<<4U)
#define ADC_MASK_CHANNEL_5    (1<<5U)
#define ADC_MASK_CHANNEL_6    (1<<6U)
#define ADC_MASK_CHANNEL_7    (1<<7U)
#define ADC_MASK_CHANNEL_8    (1<<8U)
#define ADC_MASK_CHANNEL_9    (1<<9U)
#define ADC_MASK_CHANNEL_10   (1<<10U)
#define ADC_MASK_CHANNEL_11   (1<<11U)
#define ADC_MASK_CHANNEL_12   (1<<12U)
#define ADC_MASK_CHANNEL_13   (1<<13U)
#define ADC_MASK_CHANNEL_14   (1<<14U)
#define ADC_MASK_CHANNEL_15   (1<<15U)
#define ADC_MASK_CHANNEL_16   (1<<16U)
#define ADC_MASK_CHANNEL_17   (1<<17U)
#define ADC_MASK_CHANNEL_18   (1<<18U)
#define ADC_MASK_CHANNEL_19   (1<<19U)
#define ADC_MASK_CHANNEL_20   (1<<20U)
#define ADC_MASK_CHANNEL_21   (1<<21U)
#define ADC_MASK_CHANNEL_22   (1<<22U)
#define ADC_MASK_CHANNEL_23   (1<<23U)
#define ADC_MASK_CHANNEL_24   (1<<24U)
#define ADC_MASK_CHANNEL_25   (1<<25U)
#define ADC_MASK_CHANNEL_26   (1<<26U)
#define ADC_MASK_CHANNEL_27   (1<<27U)
#define ADC_MASK_TEMPERATURE  (1<<28UL)
#define ADC_MASK_VOLT         (1<<29UL)
#define ADC_MASK_SENSORS (ADC_MASK_TEMPERATURE | ADC_MASK_VOLT)
#define ADC_MASK_GROUP_B_OFF  (0UL)
#define ADC_MASK_ADD_OFF      (0UL)
#define ADC_MASK_SAMPLE_HOLD_OFF (0U)

/** Sample and hold Channel mask. Sample and hold is only available for channel 0,1,2*/
#define ADC_SAMPLE_HOLD_CHANNELS     (0x07U)
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

