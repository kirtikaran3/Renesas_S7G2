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
* File Name    : hw_adc_common.h
* Description  : A/D Converter (ADC) Module hardware common header file.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup ADC
 * @{
***********************************************************************************************************************/

#ifndef HW_ADC_COMMON_H
#define HW_ADC_COMMON_H

/**********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "bsp_api.h"
#include "..\..\r_adc_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/** State of ADST bit*/
typedef enum e_hw_adc_adcsr_adst
{
    HW_ADC_ADCSR_ADCST_NOT_SET= (0x0),
    HW_ADC_ADCSR_ADCST_SET    = (0x1),
    HW_ADC_ADCSR_ADCST_MAX
} hw_adc_adcsr_adst_t;

/** State of TRGE bit*/
typedef enum e_hw_adc_adcsr_trge
{
    HW_ADC_ADCSR_TRGE_NOT_SET= (0x0),
    HW_ADC_ADCSR_TRGE_SET    = (0x1),
    HW_ADC_ADCSR_TRGE_MAX
} hw_adc_adcsr_trge_t;

/** State of ADIE bit*/
typedef enum e_hw_adc_adcsr_adie
{
    HW_ADC_ADCSR_ADIE_NOT_SET= (0x0),
    HW_ADC_ADCSR_ADIE_SET    = (0x1),
    HW_ADC_ADCSR_ADIE_MAX
} hw_adc_adcsr_adie_t;

/** State of GBADIE bit*/
typedef enum e_hw_adc_adcsr_gbadie
{
    HW_ADC_ADCSR_GBADIE_NOT_SET= (0x0),
    HW_ADC_ADCSR_GBADIE_SET    = (0x1),
    HW_ADC_ADCSR_GBADIE_MAX
} hw_adc_adcsr_gbadie_t;
/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Private Functions
***********************************************************************************************************************/
/** Set function for ADC.ADCSR.ADST */
__STATIC_INLINE void HW_ADC_ADCSR_ADST_Set(uint16_t unit, hw_adc_adcsr_adst_t value)
{
	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(unit);
    /** Get the pointer to registers for the appropriate unit*/
       aregs_t *p_regs = GET_REGS_PTR(unit);
       p_regs->ADCSR_b.ADST = value;
}
/** Get function for ADC.ADCSR.ADST */
__STATIC_INLINE hw_adc_adcsr_adst_t HW_ADC_ADCSR_ADST_Get(uint16_t unit)
{
	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(unit);
    /** Get the pointer to registers for the appropriate unit*/
    aregs_t *p_regs = GET_REGS_PTR(unit);
    return((hw_adc_adcsr_adst_t)p_regs->ADCSR_b.ADST);
}
/** Set function for ADC.ADCSR.TRGE */
__STATIC_INLINE void HW_ADC_ADCSR_TRGE_Set(uint16_t unit, hw_adc_adcsr_trge_t value)
{
	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(unit);
    /** Get the pointer to registers for the appropriate unit*/
       aregs_t *p_regs = GET_REGS_PTR(unit);
       p_regs->ADCSR_b.TRGE = value;
}
/** Set function for ADC.ADCSR.ADIE */
__STATIC_INLINE void HW_ADC_ADCSR_ADIE_Set(uint16_t unit, hw_adc_adcsr_adie_t value)
{
	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(unit);
    /** Get the pointer to registers for the appropriate unit*/
       aregs_t *p_regs = GET_REGS_PTR(unit);
       p_regs->ADCSR_b.ADIE = value;
}
/** Set function for ADC.ADCSR.GBADIE */
__STATIC_INLINE void HW_ADC_ADCSR_GBADIE_Set(uint16_t unit, hw_adc_adcsr_gbadie_t value)
{
	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(unit);
    /** Get the pointer to registers for the appropriate unit*/
       aregs_t *p_regs = GET_REGS_PTR(unit);
       p_regs->ADCSR_b.GBADIE = value;
}

#endif /* HW_ADC_COMMON_H */
