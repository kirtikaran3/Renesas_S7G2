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
* File Name    : hw_adc_private_s7g2.c
* Description  : Implements ADC LLD
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup ADC
 * @{
***********************************************************************************************************************/


/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#include "bsp_api.h"
#include "r_adc_api.h"
#include "r_adc_cfg.h"
#include "../../../r_adc_private.h"
#include "r_adc.h"
#include "../../hw_adc_private.h"
#include "hw_adc_private_s7g2.h"
#if (BSP_MCU_GROUP_S7G2 == 1)
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
 Global Variables
**********************************************************************************************************************/
/** In ROM. A/D Data Register pointers  */
/** Unit 0 data registers: Indexed into via the adc_register_t enum*/
volatile uint16_t * const dreg0_ptrs[] =
{
        (volatile uint16_t * )&R_S12ADC0->ADDRn[0],   /**< channel 0*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[1],   /**< channel 1*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[2],   /**< channel 2*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[3],   /**< channel 3*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[4],   /**< channel 4*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[5],   /**< channel 5*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[6],   /**< channel 6*/
        NULL,                   /**< channel 7:  Not present on this unit*/
        NULL,                   /**< channel 8:  Not present on this unit*/
        NULL,                   /**< channel 9:  Not present on this unit*/
        NULL,                   /**< channel 10: Not present on this unit*/
        NULL,                   /**< channel 11: Not present on this unit*/
        NULL,                   /**< channel 12: Not present on this unit*/
        NULL,                   /**< channel 13: Not present on this unit*/
        NULL,                   /**< channel 14: Not present on this unit*/
        NULL,                   /**< channel 15: Not present on this unit*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[16],  /**< channel 16*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[17],  /**< channel 17*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[18],  /**< channel 18*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[19],  /**< channel 19*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[20],  /**< channel 20*/
        (volatile uint16_t * )&R_S12ADC0->ADDRn[21],  /**< channel 21*/
        NULL,                   /**< channel 22: Not present on this unit*/
        NULL,                   /**< channel 23: Not present on this unit*/
        NULL,                   /**< channel 24: Not present on this unit*/
        NULL,                   /**< channel 25: Not present on this unit*/
        NULL,                   /**< channel 26: Not present on this unit*/
        NULL,                   /**< channel 27: Not present on this unit*/
        (volatile uint16_t * )&R_S12ADC0->ADTSDR,     /**< temperature sensor*/
        (volatile uint16_t * )&R_S12ADC0->ADOCDR,     /**< voltage sensor*/
        (volatile uint16_t * )&R_S12ADC0->ADDBLDR,    /**< double trigger register*/
        (volatile uint16_t * )&R_S12ADC0->ADDBLDRA,   /**< double trigger registerA*/
        (volatile uint16_t * )&R_S12ADC0->ADDBLDRB,   /**< double trigger registerB*/
};

/** Unit 1 data registers: Indexed into via the adc_register_t enum*/
volatile uint16_t * const  dreg1_ptrs[] =
{
        (volatile uint16_t * )&R_S12ADC1->ADDRn[0],   /**< channel 0*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[1],   /**< channel 1*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[2],   /**< channel 2*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[3],   /**< channel 3*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[4],   /**< channel 4*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[5],   /**< channel 5*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[6],   /**< channel 6*/
        NULL,                   /**< channel 7:  Not present on this unit*/
        NULL,                   /**< channel 8:  Not present on this unit*/
        NULL,                   /**< channel 9:  Not present on this unit*/
        NULL,                   /**< channel 10: Not present on this unit*/
        NULL,                   /**< channel 11: Not present on this unit*/
        NULL,                   /**< channel 12: Not present on this unit*/
        NULL,                   /**< channel 13: Not present on this unit*/
        NULL,                   /**< channel 14: Not present on this unit*/
        NULL,                   /**< channel 15: Not present on this unit*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[16],  /**< channel 16*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[17],  /**< channel 17*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[18],  /**< channel 18*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[19],  /**< channel 19*/
        (volatile uint16_t * )&R_S12ADC1->ADDRn[20],  /**< channel 20*/
        NULL,                   /**< channel 21: Not present on this unit */
        NULL,                   /**< channel 22: Not present on this unit*/
        NULL,                   /**< channel 23: Not present on this unit*/
        NULL,                   /**< channel 24: Not present on this unit*/
        NULL,                   /**< channel 25: Not present on this unit*/
        NULL,                   /**< channel 26: Not present on this unit*/
        NULL,                   /**< channel 27: Not present on this unit*/
        (volatile uint16_t * )&R_S12ADC1->ADTSDR,     /**< temperature sensor*/
        (volatile uint16_t * )&R_S12ADC1->ADOCDR,     /**< voltage sensor*/
        (volatile uint16_t * )&R_S12ADC1->ADDBLDR,    /**< double trigger register*/
        (volatile uint16_t * )&R_S12ADC1->ADDBLDRA,   /**< double trigger registerA*/
        (volatile uint16_t * )&R_S12ADC1->ADDBLDRB,   /**< double trigger registerB*/
};


/** In ROM. Sample State (SST) Register pointers    */
/** 8-bit register pointers */
/** Unit 0 sample state registers: Indexed into via the adc_sample_state_reg_t enum*/
volatile uint8_t * const  sreg0_ptrs[] =
{
        &R_S12ADC0->ADSSTR0n[0],   /**< channel 0*/
        &R_S12ADC0->ADSSTR0n[1],   /**< channel 1*/
        &R_S12ADC0->ADSSTR0n[2],   /**< channel 2*/
        &R_S12ADC0->ADSSTR0n[3],   /**< channel 3*/
        &R_S12ADC0->ADSSTR0n[4],   /**< channel 4*/
        &R_S12ADC0->ADSSTR0n[5],   /**< channel 5*/
        &R_S12ADC0->ADSSTR0n[6],   /**< channel 6*/
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &R_S12ADC0->ADSSTRL,       /**< channels 16-21*/
        &R_S12ADC0->ADSSTRT,       /**< temperature sensor*/
        &R_S12ADC0->ADSSTRO        /**< voltage sensor*/
};
/** Unit 1 sample state registers: Indexed into via the adc_sample_state_reg_t enum*/
volatile uint8_t * const  sreg1_ptrs[] =
{
        &R_S12ADC1->ADSSTR0n[0],   /**< channel 0*/
        &R_S12ADC1->ADSSTR0n[1],   /**< channel 1*/
        &R_S12ADC1->ADSSTR0n[2],   /**< channel 2*/
        &R_S12ADC1->ADSSTR0n[3],   /**< channel 3*/
        &R_S12ADC1->ADSSTR0n[4],   /**< channel 4*/
        &R_S12ADC1->ADSSTR0n[5],   /**< channel 5*/
        &R_S12ADC1->ADSSTR0n[6],   /**< channel 6*/
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &R_S12ADC1->ADSSTRL,       /**< channels 16-20*/
        &R_S12ADC1->ADSSTRT,       /**< temperature sensor*/
        &R_S12ADC1->ADSSTRO        /**< voltage sensor*/
};
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief   HW_ADC_ScanCfgCheckSampleHold
 *
 * This function checks the Sample and Hold arguments
 *
 * @param[in]  unit           :  ADC unit number
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
***********************************************************************************************************************/
ssp_err_t HW_ADC_ScanCfgCheckSampleHold(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
    uint32_t a_mask;
    uint32_t b_mask;

    /** If a valid value is set in the mask..*/
        if (0 != p_cfg->sample_hold_mask)
        {
            if (ADC_UNIT_1 == unit)              /** S&H only on unit 0   */
            {
                return SSP_ERR_INVALID_ARGUMENT;
            }

        /** Sample and Hold channels can only be 0,1,2 and must have at least minimum state count specified */
            if ((ADC_SAMPLE_HOLD_CHANNELS < p_cfg->sample_hold_mask)
             || (ADC_SAMPLE_STATE_HOLD_COUNT_MIN > p_cfg->sample_hold_states))
            {
                return SSP_ERR_INVALID_ARGUMENT;
            }

        /** Sample and Hold  channels cannot not be split across groups  */
            a_mask = p_cfg->sample_hold_mask & p_cfg->scan_mask;
            b_mask = p_cfg->sample_hold_mask & p_cfg->scan_mask_group_b;
            if (ADC_ADCS_GROUP_SCAN == R_S12ADC0->ADCSR_b.ADCS)
            {
                if ((a_mask != p_cfg->sample_hold_mask) && (b_mask != p_cfg->sample_hold_mask))
                {
                    return SSP_ERR_INVALID_ARGUMENT;
                }
            }
            else if (a_mask != p_cfg->sample_hold_mask)
            {
                return SSP_ERR_INVALID_ARGUMENT;
            }
            else
            {
                /** Will not get here: Branch created to meet coding standard */
            }


        /** Sample and Hold  channels cannot be a double trigger channel (can be in group B) */
            if ((1UL == R_S12ADC0->ADCSR_b.DBLE) && (0 != a_mask))
            {
                return SSP_ERR_INVALID_ARGUMENT;
            }

        /** Sample and Hold  channels cannot be in GroupB if GroupA priority enabled     */
            if ((0 != b_mask) && (ADC_GROUP_A_PRIORITY_OFF != p_cfg->priority_group_a))
            {
                return SSP_ERR_INVALID_ARGUMENT;
            }
        }
    return SSP_SUCCESS;
}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_ScanCfgSetSampleHold
 *
 * This function sets the sample and Hold arguments
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
***********************************************************************************************************************/

void HW_ADC_ScanCfgSetSampleHold(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
    aregs_t   *p_regs = GET_REGS_PTR(unit);
    if (ADC_MASK_CHANNEL_0 & p_cfg->sample_hold_mask)
    {
        p_regs->ADSHCR_b.SHANS0 = 1UL;
    }
    if (ADC_MASK_CHANNEL_1 & p_cfg->sample_hold_mask)
    {
        p_regs->ADSHCR_b.SHANS1 = 1UL;
    }
    if (ADC_MASK_CHANNEL_2 & p_cfg->sample_hold_mask)
    {
        p_regs->ADSHCR_b.SHANS2 = 1UL;
    }

    p_regs->ADSHCR_b.SSTSH = p_cfg->sample_hold_states;

}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_ScanCfgCheckSensors
 *
 * This function checks the Temperature and Voltage sensor arguments
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
***********************************************************************************************************************/

ssp_err_t HW_ADC_ScanCfgCheckSensors(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
    aregs_t *p_regs = GET_REGS_PTR(unit);

    /** If sensors are used, then Double Trigger and Disconnect Detection cannot be used */
    if ((p_cfg->scan_mask & ADC_MASK_SENSORS)||
            (p_cfg->scan_mask_group_b & ADC_MASK_SENSORS))
    {
        if ((1UL == p_regs->ADCSR_b.DBLE)  /*** Double trigger mode          */
         || (0UL != p_regs->ADDISCR))      /*** Disconnect detection assist enabled  */
        {
            return SSP_ERR_INVALID_ARGUMENT;
        }
    }

    return SSP_SUCCESS;
}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_SensorCfgTemperature
 *
 * This function set the sensor bits to enable the temperature sensor for group A/normal mode.
 *
 * @param[in]  unit           :  ADC unit number
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
***********************************************************************************************************************/

void HW_ADC_SensorCfgTemperature(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
    aregs_t *p_regs = GET_REGS_PTR(unit);

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
    R_MSTP->MSTPCRD_b.MSTPD22 = 0U;          /** Power on  the temperature sensor*/
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);

    if (p_cfg->scan_mask & ADC_MASK_TEMPERATURE)
    {
        HW_ADC_SensorCfgTemperatureGroupA(unit);      /** Select temperature output GroupA */
    }
    else
    {
        HW_ADC_SensorCfgTemperatureGroupB(unit);      /** Select temperature output GroupB */
    }
    /** Enable temperature addition mode if set */
    p_regs->ADEXICR_b.TSSAD = (p_cfg->add_mask & ADC_MASK_TEMPERATURE) ? 1U : 0U;

    R_TSN->TSCR_b.TSEN = 1UL;             /** Enable temperature sensor           */
    /** 30us delay  to stabilize the clock before an accurate reading can be taken.
     * This only applies when the sensor is powered on the first time */
    R_TSN->TSCR_b.TSOE = 1UL;             /** Enable temperature sensor output to ADC */

}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_SensorCfgTemperatureGroupA
 *
 * This function set the sensor bits to enable the temperature sensor for group A/normal mode.
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
***********************************************************************************************************************/
void HW_ADC_SensorCfgTemperatureGroupA(uint16_t const unit)
{
    aregs_t  *p_regs = GET_REGS_PTR(unit);
    p_regs->ADEXICR_b.TSSA = 1UL;      /** Select temperature output GroupA */
    p_regs->ADEXICR_b.TSSB = 0UL;
}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_SensorCfgTemperatureGroupB
 *
 * This function set the sensor bits to enable the temperature sensor for group B.
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
***********************************************************************************************************************/
void HW_ADC_SensorCfgTemperatureGroupB(uint16_t const unit)
{
    aregs_t  *p_regs = GET_REGS_PTR(unit);
    p_regs->ADEXICR_b.TSSA = 0UL;
    p_regs->ADEXICR_b.TSSB = 1UL;      /** Select temperature output GroupB */
}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_SensorCfgVoltageGroupA
 *
 * This function set the sensor  bits to enable the voltage sensor for group A/normal mode.
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
***********************************************************************************************************************/
void HW_ADC_SensorCfgVoltageGroupA(uint16_t const unit)
{
    aregs_t  *p_regs = GET_REGS_PTR(unit);
    p_regs->ADEXICR_b.OCSA = 1UL;      /** Select voltage output to GroupA  */
    p_regs->ADEXICR_b.OCSB = 0UL;
}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_SensorCfgVoltageGroupB
 *
 * This function set the sensor bits to enable the voltage sensor for group B.
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
***********************************************************************************************************************/
void HW_ADC_SensorCfgVoltageGroupB(uint16_t const unit)
{
    aregs_t  *p_regs = GET_REGS_PTR(unit);
    p_regs->ADEXICR_b.OCSA = 0UL;
    p_regs->ADEXICR_b.OCSB = 1UL;      /** Select voltage output to GroupB  */
}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_S12adiEnable
 *
 * This function clears the S12ADI interrupt flag and enables interrupts in the peripheral (for IR flag usage).
 * If priority is not 0, interrupts are enabled in the ICU.
 *
 * @param[in]  unit :  ADC unit number
 *
 * @note: This has nothing to do with enabling triggers.
***********************************************************************************************************************/
void HW_ADC_S12adiEnable(uint16_t unit)
{
    aregs_t *p_regs = GET_REGS_PTR(unit);

    if (ADC_UNIT_0 == unit)     /** Unit 0    */
    {
#if (BSP_IRQ_CFG_ADC0_SCAN_END != BSP_IRQ_DISABLED)
        R_BSP_IrqStatusClear (ADC0_SCAN_END_IRQn) ;    /*** Clear flag in ICU     */
        NVIC_ClearPendingIRQ (ADC0_SCAN_END_IRQn) ;    /*** Clear flag in NVIC    */
        NVIC_EnableIRQ (ADC0_SCAN_END_IRQn) ;          /*** Enable in NVIC        */
#endif

    }
    else if (ADC_UNIT_1 == unit)   /** Unit 1    */
    {
#if (BSP_IRQ_CFG_ADC1_SCAN_END != BSP_IRQ_DISABLED)
        R_BSP_IrqStatusClear (ADC1_SCAN_END_IRQn) ;    /*** Clear flag in ICU     */
        NVIC_ClearPendingIRQ (ADC1_SCAN_END_IRQn) ;    /*** Clear flag in NVIC    */
        NVIC_EnableIRQ (ADC1_SCAN_END_IRQn) ;          /*** Enable in NVIC        */
#endif
    }
    else
    {
        /** Will not get here: Branch created to meet coding standard */
    }

    p_regs->ADCSR_b.ADIE = 1UL;                /*** Enable in peripheral  */
}

/*******************************************************************************************************************//**
 * @brief   HW_ADC_S12gbadiEnable
 *
 * This function clears the S12GBADI interrupt flag and enables interrupts in the peripheral (for IR flag usage).
 * If priority is not 0, interrupts are enabled in the ICU.
 *
 * @param[in]  unit :  ADC unit number
 *
 * @note: This has nothing to do with enabling triggers.
***********************************************************************************************************************/
void HW_ADC_S12gbadiEnable(uint16_t unit)
{
    aregs_t *p_regs = GET_REGS_PTR(unit);

    if (ADC_UNIT_0 == unit)       /** Unit 0    */
    {
#if (BSP_IRQ_CFG_ADC0_SCAN_END_B != BSP_IRQ_DISABLED)
        R_BSP_IrqStatusClear (ADC0_SCAN_END_B_IRQn) ;    /*** Clear flag in ICU     */
        NVIC_ClearPendingIRQ (ADC0_SCAN_END_B_IRQn) ;    /*** Clear flag in NVIC    */
        NVIC_EnableIRQ (ADC0_SCAN_END_B_IRQn) ;          /*** Enable in NVIC        */
#endif
    }
    else if (ADC_UNIT_1 == unit)   /** Unit 1    */
    {
#if (BSP_IRQ_CFG_ADC1_SCAN_END_B != BSP_IRQ_DISABLED)
        R_BSP_IrqStatusClear (ADC1_SCAN_END_B_IRQn) ;    /*** Clear flag in ICU     */
        NVIC_ClearPendingIRQ (ADC1_SCAN_END_B_IRQn) ;    /*** Clear flag in NVIC    */
        NVIC_EnableIRQ (ADC1_SCAN_END_B_IRQn) ;          /*** Enable in NVIC        */
#endif
    }
    else
    {
        /** Will not get here: Branch created to meet coding standard */
    }

    p_regs->ADCSR_b.GBADIE = 1UL;                /*** Enable in peripheral  */
}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_S12gbadiDisable
 *
 * This function clears the S12GBADI interrupt flag and disables interrupts in the peripheral (for IR flag usage).
 *
 * @param[in]  unit :  ADC unit number
 *
 * @note: This has nothing to do with disabling triggers.
 **********************************************************************************************************************/
void HW_ADC_S12gbadiDisable(uint16_t unit)
{
    /** Used to prevent compiler warning */
    SSP_PARAMETER_NOT_USED(unit);
    aregs_t *p_regs = GET_REGS_PTR(unit);

    if (ADC_UNIT_0 == unit)       /** Unit 0    */
    {
#if (BSP_IRQ_CFG_ADC0_SCAN_END_B != BSP_IRQ_DISABLED)
        NVIC_DisableIRQ (ADC0_SCAN_END_B_IRQn) ;       /*** Disable interrupts in ICU    */
        NVIC_ClearPendingIRQ (ADC0_SCAN_END_B_IRQn) ;  /*** Clear flag in NVIC    */
        R_BSP_IrqStatusClear (ADC0_SCAN_END_B_IRQn) ;  /*** Clear flag in ICU     */
#endif
    }
    else if (ADC_UNIT_1 == unit)   /** Unit 1    */
    {
#if (BSP_IRQ_CFG_ADC1_SCAN_END_B != BSP_IRQ_DISABLED)
        NVIC_DisableIRQ (ADC1_SCAN_END_B_IRQn) ;       /*** Disable interrupts in ICU    */
        NVIC_ClearPendingIRQ (ADC1_SCAN_END_B_IRQn) ;  /*** Clear interrupt flag         */
        R_BSP_IrqStatusClear (ADC1_SCAN_END_B_IRQn);
#endif
    }
    else
    {
        /** Will not get here: Branch created to meet coding standard */
    }
    p_regs->ADCSR_b.GBADIE = 0UL;                /*** Disable in peripheral  */
}
/*******************************************************************************************************************//**
 * @brief HW_ADC_Close
 *
 * The Close function stops power and clocking to the A/D peripheral, disables interrupts and turns off the channels and
 * sensors.
 *
 * @param[in]  Unit  : The ADC Unit to stop
 *
 * @retval  None
***********************************************************************************************************************/
void HW_ADC_Close(uint16_t unit)
{
    aregs_t     *p_regs = GET_REGS_PTR(unit);

    /** Stop triggers & conversions, and disable peripheral interrupts */
    if (ADC_UNIT_0 == unit) /** Unit 0*/
    {
#if (BSP_IRQ_CFG_ADC0_SCAN_END != BSP_IRQ_DISABLED)
        NVIC_DisableIRQ (ADC0_SCAN_END_IRQn) ;         /*** Disable interrupts in ICU    */
        NVIC_ClearPendingIRQ (ADC0_SCAN_END_IRQn) ;    /*** Clear interrupt flag         */
        R_BSP_IrqStatusClear (ADC0_SCAN_END_IRQn) ;
#endif
#if (BSP_IRQ_CFG_ADC0_SCAN_END_B != BSP_IRQ_DISABLED)
        NVIC_DisableIRQ (ADC0_SCAN_END_B_IRQn) ;       /*** Disable interrupts in ICU    */
        NVIC_ClearPendingIRQ (ADC0_SCAN_END_B_IRQn) ;  /*** Clear interrupt flag         */
        R_BSP_IrqStatusClear (ADC0_SCAN_END_B_IRQn);
#endif
    }
    if (ADC_UNIT_1 == unit) /** Unit 1*/
    {
#if (BSP_IRQ_CFG_ADC1_SCAN_END != BSP_IRQ_DISABLED)
        NVIC_DisableIRQ (ADC1_SCAN_END_IRQn) ;         /*** Disable interrupts in ICU    */
        NVIC_ClearPendingIRQ (ADC1_SCAN_END_IRQn) ;    /*** Clear interrupt flag         */
        R_BSP_IrqStatusClear (ADC1_SCAN_END_IRQn) ;
#endif
#if (BSP_IRQ_CFG_ADC1_SCAN_END_B != BSP_IRQ_DISABLED)
        NVIC_DisableIRQ (ADC1_SCAN_END_B_IRQn) ;       /*** Disable interrupts in ICU    */
        NVIC_ClearPendingIRQ (ADC1_SCAN_END_B_IRQn) ;  /*** Clear interrupt flag         */
        R_BSP_IrqStatusClear (ADC1_SCAN_END_B_IRQn);
#endif
    }

    /** If Group Priority was enabled, the disable it*/
    if (1 == p_regs->ADGSPCR_b.PGS)
    {
        p_regs->ADGSPCR_b.PGS = 0UL;
    }

    /** If Group Mode was enabled, then disable the triggers for both groups */
    if (1 == p_regs->ADCSR_b.ADCS)
    {
        p_regs->ADSTRGR = 0x3F3F;
    }
    /** If Group Mode was not enabled, then disable the trigger for normal mode */
    else
    {
        p_regs->ADSTRGR_b.TRSA = 0x3F;
    }

    /** Clear all configurations*/
    p_regs->ADCSR = 0UL;

    /** Power down peripheral   */
    R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_OM_LPC_BATT) ;
    if (ADC_UNIT_0 == unit)
    {
        R_MSTP->MSTPCRD_b.MSTPD16 = 1UL ; /*** ADC 0 power off      */
    }
    else if (ADC_UNIT_1 == unit)
    {

        R_TSN->TSCR = 0UL;                /*** Disable temperature sensor      */
        R_MSTP->MSTPCRD_b.MSTPD22 = 1UL;  /*** Temperature sensor power off    */
        R_MSTP->MSTPCRD_b.MSTPD15 = 1UL; /*** ADC 1 power off      */
    }
    else
    {
        /** Will not get here: Branch created to meet coding standard */
    }
    R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_OM_LPC_BATT) ;

}

/*******************************************************************************************************************//**
 * @brief HW_ADC_ELCEventGet
 *
 * The HW_ADC_ELCEventGet retrieves the ELC event for the specified unit
 *
 * @param[in]  Unit  : The ADC Unit in use
 * @param[in]  Unit  : The ADC ELC structure
 * @retval  None
 **********************************************************************************************************************/

void HW_ADC_ELCEventGet(uint16_t unit, adc_info_t * p_adc_info)
{
    if(ADC_UNIT_0 == unit)
    {
        p_adc_info->elc_peripheral = ELC_PERIPHERAL_ADC0;
        p_adc_info->elc_event = ELC_EVENT_ADC0_SCAN_END;
    }
    else
    {
        p_adc_info->elc_peripheral = ELC_PERIPHERAL_ADC1;
        p_adc_info->elc_event = ELC_EVENT_ADC1_SCAN_END;
    }
}
/*******************************************************************************************************************//**
 * @} (end addtogroup ADC)
***********************************************************************************************************************/
#endif
