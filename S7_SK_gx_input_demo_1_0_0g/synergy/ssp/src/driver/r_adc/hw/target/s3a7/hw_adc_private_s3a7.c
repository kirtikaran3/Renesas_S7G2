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
 * File Name    : hw_adc_private_s3a7.c
 * Description  : Implements ADC LLD with S3A7 specific details
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup ADC
 * @{
 **********************************************************************************************************************/


/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

#include "bsp_api.h"
#include "r_adc_api.h"
#include "r_adc_cfg.h"
#include "../../../r_adc_private.h"
#include "r_adc.h"
#include "../../hw_adc_private.h"
#include "hw_adc_private_s3a7.h"
#if (BSP_MCU_GROUP_S3A7 == 1)
/***********************************************************************************************************************
Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Global Variables
 **********************************************************************************************************************/
/** In ROM. A/D Data Register pointers  */
/** Unit 0 data registers: Indexed into via the adc_register_t enum*/
volatile uint16_t * const dreg0_ptrs[] =
{
        (volatile uint16_t * )&R_S14ADC->ADDRn[0],   /**< channel 0*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[1],   /**< channel 1*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[2],   /**< channel 2*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[3],   /**< channel 3*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[4],   /**< channel 4*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[5],   /**< channel 5*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[6],   /**< channel 6*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[7],   /**< channel 7*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[8],   /**< channel 8*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[9],   /**< channel 9*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[10],   /**< channel 10*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[11],   /**< channel 11*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[12],   /**< channel 12*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[13],   /**< channel 13*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[14],   /**< channel 14*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[15],   /**< channel 15*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[16],  /**< channel 16*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[17],  /**< channel 17*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[18],  /**< channel 18*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[19],  /**< channel 19*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[20],  /**< channel 20*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[21],  /**< channel 21*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[22],  /**< channel 22*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[23],  /**< channel 23*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[24],  /**< channel 24*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[25],  /**< channel 25*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[26],  /**< channel 26*/
        (volatile uint16_t * )&R_S14ADC->ADDRn[27],  /**< channel 27*/
        (volatile uint16_t * )&R_S14ADC->ADTSDR,     /**< temperature sensor*/
        (volatile uint16_t * )&R_S14ADC->ADOCDR,     /**< voltage sensor*/
        (volatile uint16_t * )&R_S14ADC->ADDBLDR,    /**< double trigger register*/
        (volatile uint16_t * )&R_S14ADC->ADDBLDRA,   /**< double trigger registerA*/
        (volatile uint16_t * )&R_S14ADC->ADDBLDRB,   /**< double trigger registerB*/
};


/** In ROM. Sample State (SST) Register pointers    */
/** 8-bit register pointers */
/** Unit 0 sample state registers: Indexed into via the adc_sample_state_reg_t enum*/
volatile uint8_t * const  sreg0_ptrs[] =
{
        &R_S14ADC->ADSSTR0n[0],   /**< channel 0*/
        &R_S14ADC->ADSSTR0n[1],   /**< channel 1*/
        &R_S14ADC->ADSSTR0n[2],   /**< channel 2*/
        &R_S14ADC->ADSSTR0n[3],   /**< channel 3*/
        &R_S14ADC->ADSSTR0n[4],   /**< channel 4*/
        &R_S14ADC->ADSSTR0n[5],   /**< channel 5*/
        &R_S14ADC->ADSSTR0n[6],   /**< channel 6*/
        &R_S14ADC->ADSSTR0n[7],   /**< channel 7*/
        &R_S14ADC->ADSSTR0n[8],   /**< channel 8*/
        &R_S14ADC->ADSSTR0n[9],   /**< channel 9*/
        &R_S14ADC->ADSSTR0n[10],  /**< channel 10*/
        &R_S14ADC->ADSSTR0n[11],  /**< channel 11*/
        &R_S14ADC->ADSSTR0n[12],  /**< channel 12*/
        &R_S14ADC->ADSSTR0n[13],  /**< channel 13*/
        &R_S14ADC->ADSSTR0n[14],  /**< channel 14*/
        &R_S14ADC->ADSSTR0n[15],  /**< channel 15*/
        &R_S14ADC->ADSSTRL,       /**< channels 16-27*/
        &R_S14ADC->ADSSTRT,       /**< temperature sensor*/
        &R_S14ADC->ADSSTRO,        /**< voltage sensor*/
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
 **********************************************************************************************************************/

ssp_err_t HW_ADC_ScanCfgCheckSampleHold(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(unit);
    if (0 != p_cfg->sample_hold_mask)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }

    return SSP_SUCCESS;
}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_ScanCfgSetSampleHold
 *
 * This function does nothing since Sample and Hold is not supported on the S3A7.
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
 **********************************************************************************************************************/

void HW_ADC_ScanCfgSetSampleHold(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(unit);
  	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(p_cfg);


}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_ScanCfgCheckSensors
 *
 * This function checks the Temperature and Voltage sensor arguments
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
 **********************************************************************************************************************/

ssp_err_t HW_ADC_ScanCfgCheckSensors(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
    aregs_t *p_regs = GET_REGS_PTR(unit);

    /** Sensors are not supported in Group B */
    if (p_cfg->scan_mask_group_b & ADC_MASK_SENSORS)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
    /** If sensors specified in Normal/Group A, verify in legal configuration
     * If sensors are used, then Double Trigger and Disconnect Detection cannot be used */
    if ((p_cfg->scan_mask & ADC_MASK_SENSORS))
    {
        if ((1 == p_regs->ADCSR_b.DBLE)  /*** Double trigger mode          */
                || (0 != p_regs->ADDISCR))      /*** Disconnect detection assist enabled  */
        {
            return SSP_ERR_INVALID_ARGUMENT;
        }
    }
    /** Temperature sensor cannot be used in any mode other than single scan mode on this MCU*/
    if ((p_cfg->scan_mask & ADC_MASK_TEMPERATURE) &&(ADC_MODE_SINGLE_SCAN != g_dcb[unit].mode))
    {
        return SSP_ERR_INVALID_ARGUMENT;
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
 **********************************************************************************************************************/

void HW_ADC_SensorCfgTemperature(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(unit);

    aregs_t *p_regs = GET_REGS_PTR(unit);

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
    R_MSTP->MSTPCRD_b.MSTPD22 = 0;          /** Power on  the temperature sensor*/
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);

    if (p_cfg->scan_mask & ADC_MASK_TEMPERATURE)
    {
        p_regs->ADEXICR_b.TSSA = 1;      /** Select temperature output GroupA */
    }

    /** Enable temperature addition mode if set */
    p_regs->ADEXICR_b.TSSAD = (p_cfg->add_mask & ADC_MASK_TEMPERATURE) ? 1 : 0;

    R_TSN->TSCR_b.TSEN = 1;             /** Enable temperature sensor           */
    /** 30us delay  to stabilize the clock before an accurate reading can be taken.
     * This only applies when the sensor is powered on the first time */
    R_TSN->TSCR_b.TSOE = 1;             /** Enable temperature sensor output to ADC */

}

/*******************************************************************************************************************//**
 * @brief   HW_ADC_SensorCfgVoltageGroupA
 *
 * This function set the sensor  bits to enable the voltage sensor for group A/normal mode.
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
 **********************************************************************************************************************/
void HW_ADC_SensorCfgVoltageGroupA(uint16_t const unit)
{
	/** Used to prevent compiler warning */
    SSP_PARAMETER_NOT_USED(unit);

    aregs_t  *p_regs = GET_REGS_PTR(unit);
    p_regs->ADEXICR_b.OCSA = 1;      /** Select voltage output to GroupA  */
}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_SensorCfgVoltageGroupB
 *
 * This function does nothing since the Voltage sensor cannot be used on Group B on this MCU
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
 **********************************************************************************************************************/
void HW_ADC_SensorCfgVoltageGroupB(uint16_t const unit)
{
	/** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(unit);
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
 **********************************************************************************************************************/
void HW_ADC_S12adiEnable(uint16_t unit)
{
	/** Used to prevent compiler warning */
    SSP_PARAMETER_NOT_USED(unit);

    aregs_t *p_regs = GET_REGS_PTR(unit);

#if (BSP_IRQ_CFG_ADC0_SCAN_END != BSP_IRQ_DISABLED)
    R_BSP_IrqStatusClear (ADC0_SCAN_END_IRQn) ;    /*** Clear flag in ICU     */
    NVIC_ClearPendingIRQ (ADC0_SCAN_END_IRQn) ;    /*** Clear flag in NVIC    */
    NVIC_EnableIRQ (ADC0_SCAN_END_IRQn) ;          /*** Enable in NVIC        */
#endif
    p_regs->ADCSR_b.ADIE = 1;                /*** Enable in peripheral  */
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
 **********************************************************************************************************************/
void HW_ADC_S12gbadiEnable(uint16_t unit)
{
	/** Used to prevent compiler warning */
    SSP_PARAMETER_NOT_USED(unit);

    aregs_t *p_regs = GET_REGS_PTR(unit);

#if (BSP_IRQ_CFG_ADC0_SCAN_END_B != BSP_IRQ_DISABLED)
    R_BSP_IrqStatusClear (ADC0_SCAN_END_B_IRQn) ;    /*** Clear flag in ICU     */
    NVIC_ClearPendingIRQ (ADC0_SCAN_END_B_IRQn) ;    /*** Clear flag in NVIC    */
    NVIC_EnableIRQ (ADC0_SCAN_END_B_IRQn) ;          /*** Enable in NVIC        */
#endif
    p_regs->ADCSR_b.GBADIE = 1;                /*** Enable in peripheral  */
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
 **********************************************************************************************************************/
void HW_ADC_Close(uint16_t unit)
{
    aregs_t     *p_regs = GET_REGS_PTR(unit);

    /** Stop triggers & conversions, and disable peripheral interrupts */
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

    /** If Group Priority was enabled, the disable it*/
    if (1 == p_regs->ADGSPCR_b.PGS)
    {
        p_regs->ADGSPCR_b.PGS = 0;
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
    p_regs->ADCSR = 0;

    /** Power down peripheral   */
    R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_OM_LPC_BATT) ;
    if (ADC_UNIT_0 == unit)
    {
        R_MSTP->MSTPCRD_b.MSTPD16 = 1 ; /*** ADC 0 power off      */
    }
    else if (ADC_UNIT_1 == unit)
    {

        R_TSN->TSCR = 0;                /*** Disable temperature sensor      */
        R_MSTP->MSTPCRD_b.MSTPD22 = 1;  /*** Temperature sensor power off    */
        R_MSTP->MSTPCRD_b.MSTPD15 = 1 ; /*** ADC 1 power off      */
    }
    R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_OM_LPC_BATT) ;

}
/*******************************************************************************************************************//**
 * @} (end addtogroup ADC)
 **********************************************************************************************************************/
#endif
