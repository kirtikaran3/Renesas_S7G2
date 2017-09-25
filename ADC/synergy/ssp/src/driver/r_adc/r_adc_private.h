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
* File Name    : r_adc_private.h
* Description  : Private definitions used by the ADC implementation
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup ADC
 * @{
***********************************************************************************************************************/

#ifndef R_ADC_PRIVATE_H
#define R_ADC_PRIVATE_H

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Version of code that implements the API defined in this file */
#define ADC_CODE_VERSION_MAJOR  (1U)
#define ADC_CODE_VERSION_MINOR  (0U)
/** Defines the maximum number of Units on the ADC peripheral*/
#if (BSP_MCU_GROUP_S7G2 == 1)  /** Macro definitions for the S7G2 device */
#define ADC_UNIT_MAX 2U
#elif (BSP_MCU_GROUP_S3A7 == 1)  /** Macro definitions for the S3A7 device */
#define ADC_UNIT_MAX 1U
#elif (BSP_MCU_GROUP_S124 == 1)  /** Macro definitions for the S124 device */
#define ADC_UNIT_MAX 1U
#endif

/**Define Unit Numbers */
#define ADC_UNIT_0      0U
#define ADC_UNIT_1      1U

/** Defines the registers settings for the ADC synchronous ELC trigger*/
#define ADC_UNIT_0_ELC_TRIGGER            0x09
#define ADC_UNIT_0_ELC_TRIGGER_GROUP_B    0x0A
#define ADC_UNIT_1_ELC_TRIGGER            0x09
#define ADC_UNIT_1_ELC_TRIGGER_GROUP_B    0x0A
#define ADC_UNIT_n_ELC_TRIGGER_DISABLED   0x3F

/** Unit1 registers are a superset of Unit0 EXCEPT that they do not contain the S&H registers */

#if (BSP_MCU_GROUP_S7G2 == 1)  /** Macro definitions for the S7G2 device */
#define aregs_t R_S12ADC0_Type
#define GET_REGS_PTR(x)         ((x==0UL) ? R_S12ADC0 : R_S12ADC1)
#define GET_DATA_ARR(x)         ((x==0UL) ? dreg0_ptrs : dreg1_ptrs)
#define GET_SST_ARR(x)          ((x==0UL) ? sreg0_ptrs : sreg1_ptrs)
#define ADC_LOCK(x)             ((x==0UL) ? BSP_HW_LOCK_S12ADC0 : BSP_HW_LOCK_S12ADC1)
#define SST_REG_MAX(x)          ((x==0) ? ADC_SAMPLE_STATE_CHANNEL_16_TO_20 : ADC_SAMPLE_STATE_REG_MAX)
#define ADC_INVALID_CH_MASK(x)  ((x==0U) ? ADC_INVALID_CH_MASK0 : ADC_INVALID_CH_MASK1)
#elif (BSP_MCU_GROUP_S3A7 == 1)  /** Macro definitions for the S3A7 device */
#define aregs_t R_S14ADC_Type
#define GET_REGS_PTR(x)         (R_S14ADC)
#define GET_DATA_ARR(x)         (dreg0_ptrs)
#define GET_SST_ARR(x)          (sreg0_ptrs)
#define ADC_LOCK(x)             (BSP_HW_LOCK_S12ADC0)
#define SST_REG_MAX(x)          (ADC_SAMPLE_STATE_CHANNEL_16_TO_20)
#define ADC_INVALID_CH_MASK(x)  (ADC_INVALID_CH_MASK0)
#elif (BSP_MCU_GROUP_S124 == 1)  /** Macro definitions for the S124 device */
#define aregs_t R_S14ADC_Type
#define GET_REGS_PTR(x)         (R_S14ADC)
#define GET_DATA_ARR(x)         (dreg0_ptrs)
#define GET_SST_ARR(x)          (sreg0_ptrs)
#define ADC_LOCK(x)             (BSP_HW_LOCK_S12ADC0)
#define SST_REG_MAX(x)          (ADC_SAMPLE_STATE_CHANNEL_16_TO_20)
#define ADC_INVALID_CH_MASK(x)  (ADC_INVALID_CH_MASK0)
#endif

/** Bitmask of invalid channels on this package*/
#if (BSP_MCU_GROUP_S7G2 == 1)  /** Channels for the S7G2 device */
#define ADC_INVALID_CH_MASK0    (0xCFC0FF80U)	// Channels valid (0-6, 16-21 + sensors)
#define ADC_INVALID_CH_MASK1    (0xCFE0FF80U)   // Channels valid (0-6, 16-20 + sensors)
#elif (BSP_MCU_GROUP_S3A7 == 1)  /** Channels for the S3A7 device */
#define ADC_INVALID_CH_MASK0    (0xC0000000U)    // Channels valid (0-27 + sensors)
#elif (BSP_MCU_GROUP_S124 == 1)  /** Channels for the S124 device */
#define ADC_INVALID_CH_MASK0    (0xCF80F800U)    // Channels valid (0-10, 16-22 + sensors)
#endif

/**********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/** ADC Internal Control Block   */
typedef struct st_adc_ctrl
{
    adc_mode_t    mode;			    /** operational mode	*/
    void          (*callback)(adc_callback_args_t *p_args);
    adc_trigger_t trigger;          /** Trigger defined for normal mode*/
    bool          opened;           /** Boolean to verify that the Unit has been initialized*/
    uint32_t      scan_mask;        /** Scan mask used for Normal scan.*/
    void const  * p_context;        /** Placeholder for user data */
} adc_ctrlx_t;

/** ADCSR register ADCS field */
typedef enum e_adc_adcs
{
    ADC_ADCS_SINGLE_SCAN=0U,
    ADC_ADCS_GROUP_SCAN=1U,
    ADC_ADCS_CONT_SCAN=2U,
    ADC_ADCS_MAX
} adc_adcs_t;

extern adc_ctrlx_t g_dcb[2];
/**********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/
ssp_err_t r_adc_open_cfg_check_unique(uint16_t unit, adc_mode_t const  mode, adc_cfg_t const * const p_cfg);


#endif /* R_ADC_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup ADC)
***********************************************************************************************************************/
