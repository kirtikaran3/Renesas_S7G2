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
* File Name    : hw_adc_private.c
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
#include "../r_adc_private.h"
#include "r_adc.h"
#include "hw_adc_private.h"
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

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief HW_ADC_Open
 *
 * The Open function applies power to the A/D peripheral, sets the operational mode, trigger sources, and
 * configurations common to all channels and sensors.
 *
 * @param[in]  p_cfg  : Pointer to configuration structure
 *
 * @retval  None
***********************************************************************************************************************/
void HW_ADC_Open(adc_cfg_t const * const p_cfg)
{
    /** Local pointer to the register set for the appropriate unit*/
    aregs_t   *p_regs;

    /** Apply power to peripheral   */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
    if (ADC_UNIT_0 == p_cfg->unit)
    {
        R_MSTP->MSTPCRD_b.MSTPD16 = 0U;      /** ADC 0 power on   */
    }
    else
    {
        R_MSTP->MSTPCRD_b.MSTPD15 = 0U;      /** ADC 1 power on   */
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);

    /**Retrieve the pointer to the registers for the current unit*/
    p_regs = GET_REGS_PTR(p_cfg->unit);
    /** Clear all settings including disabling interrupts*/
    p_regs->ADCSR = 0UL;
    /** Clear the sensor settings*/
    p_regs->ADEXICR = 0UL;

    /** Set mode related register fields    */
    if (ADC_MODE_GROUP_SCAN == p_cfg->mode)
    {
        p_regs->ADCSR_b.ADCS = ADC_ADCS_GROUP_SCAN;
    }
    else
    {
        if ((ADC_MODE_CONTINUOUS_SCAN == p_cfg->mode))
        {
            p_regs->ADCSR_b.ADCS = ADC_ADCS_CONT_SCAN;
        }
        else
        {
            /** Do Nothing: Branch created to meet coding standard*/
        }
    /** other modes have ADCS=0  */
    }

    /** Disable double trigger since this is not currently supported */
    p_regs->ADCSR_b.DBLE = 0UL;

    /** Configure trigger if its a hardware trigger*/
    if (ADC_TRIGGER_SYNC_ELC == p_cfg->trigger)
    {
    	/** If the unit is 0 then set the ELC value for Unit 0 normal/groupA trigger*/
        if (ADC_UNIT_0 == p_cfg->unit)
        {
        	p_regs->ADSTRGR_b.TRSA = ADC_UNIT_0_ELC_TRIGGER;
        }
        /** If the unit is 1 then set the ELC value for Unit 1 normal/groupA trigger*/
        else if (ADC_UNIT_1 == p_cfg->unit)
        {
        	p_regs->ADSTRGR_b.TRSA = ADC_UNIT_1_ELC_TRIGGER;
        }
        else
        {
            /** Will not get here since no units other that 0 and 1 exist
             *  Branch created to meet coding standard*/
        }
    }
    /** Configure external trigger for async trigger*/
    else if (ADC_TRIGGER_ASYNC_EXT_TRG0 == p_cfg->trigger)
    {
        /** Enable Asynchronous external trigger*/
        p_regs->ADCSR_b.EXTRG = 1U;
        /** Set TRSA to 0x00 if using Asynchronous external trigger*/
        p_regs->ADSTRGR_b.TRSA = 0U;
    }
    else
    {
        /** Do Nothing: Branch created to meet coding standard*/
    }

    /** Configure ADC resolution for this unit */
    p_regs->ADCER_b.ADPRC = p_cfg->resolution;
    /** Configure ADC alignment for this unit */
    p_regs->ADCER_b.ADRFMT = (ADC_ALIGNMENT_LEFT == p_cfg->alignment) ? 1U : 0U;
    /** Configure is the result register should be cleared after reading for this unit */
    p_regs->ADCER_b.ACE = (ADC_CLEAR_AFTER_READ_ON == p_cfg->clearing) ? 1U : 0U;
    /** Configure the count for result addition or averaging*/
    p_regs->ADADC = p_cfg->add_average_count;

}
/*******************************************************************************************************************//**
 * @brief   HW_ADC_ScanCfg
 *
 * This function does extensive checking on channel mask settings based upon operational mode. Mask registers are
 * initialized and interrupts enabled in peripheral. Interrupts are also enabled in ICU if corresponding priority
 * is not 0.
 *
 * @param[in]  unit  : ADC unit number
 * @param[in]  p_cfg : Pointer to channel configuration structure containing masks
 *
 * @retval  SSP_SUCCESS -               Successful
 * @retval  SSP_ERR_INVALID_POINTER -   p_cfg pointer is NULL
 * @retval  SSP_ERR_INVALID_ARGUMENT -  Parameter has invalid/illegal value.
***********************************************************************************************************************/
ssp_err_t HW_ADC_ScanCfg(uint16_t const unit,  adc_channel_cfg_t const * const p_cfg)
{
    aregs_t   *p_regs = GET_REGS_PTR(unit);
    ssp_err_t err = SSP_SUCCESS;

    /** parameter checking  */
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    err = HW_ADC_ScanCfgCheck(unit, p_cfg);
    if (SSP_SUCCESS != err)
    {
        return err;
    }
#endif

    /** Set mask for channels and sensors for channels 0 to 15  */
    p_regs->ADANSA0 = (uint16_t) (p_cfg->scan_mask & 0x0000FFFFU);
    p_regs->ADANSB0 = (uint16_t) (p_cfg->scan_mask_group_b & 0x0000FFFFU);
    p_regs->ADADS0 = (uint16_t) (p_cfg->add_mask & 0x0000FFFFU);
    /** Set mask for channels and sensors for higher channels:
     * 3F is valid for unit 0 (21 channels)
     * 1F is valid for unit 1 (20 channels),
     * but since invalid channels check has already passed at this point, 3F is used for simplicity   */
    p_regs->ADANSA1 = (uint16_t) ((p_cfg->scan_mask >> 16) & 0x0000003FU);
    p_regs->ADANSB1 = (uint16_t) ((p_cfg->scan_mask_group_b >> 16) & 0x0000003FU);
    p_regs->ADADS1 = (uint16_t) ((p_cfg->add_mask >> 16) & 0x0000003FU);

    /** Configure Sensors*/
    HW_ADC_SensorCfg(unit, p_cfg);

    /** NOTE: S&H adds to scan time because normal state machine still runs.
     adds 12 or more sample_hold_states ADCLKS to scan time */
    HW_ADC_ScanCfgSetSampleHold(unit, p_cfg);

    /** Set group A priority action (not interrupt priority!)
     * This must be set prior to configuring the interrupts for all modes other than ADC_GROUP_A_GROUP_B_CONTINUOUS_SCAN */
    if (ADC_GROUP_A_GROUP_B_CONTINUOUS_SCAN !=  p_cfg->priority_group_a)
    {
        p_regs->ADGSPCR = p_cfg->priority_group_a;
    }

    /** For all modes other than continuous scan mode, enable the scan completion interrupt*/
    if (ADC_MODE_CONTINUOUS_SCAN != g_dcb[unit].mode)
    {
        /** Enable Group B scan completion interrupt */
        if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR_b.ADCS)
        {
            /** If the group priority is not set to group B continuous scan mode, then enable the group B interrupt*/
            if (ADC_GROUP_A_GROUP_B_CONTINUOUS_SCAN !=  p_cfg->priority_group_a)
            {
                HW_ADC_S12gbadiEnable(unit);
                /** If the unit is 0 then set the ELC value for Unit 0 groupB trigger*/
                if (ADC_UNIT_0 == unit)
                {
                    p_regs->ADSTRGR_b.TRSB = ADC_UNIT_0_ELC_TRIGGER_GROUP_B;
                }
                /** If the unit is 1 then set the ELC value for Unit 1 groupB trigger*/
                else if (ADC_UNIT_1 == unit)
                {
                    p_regs->ADSTRGR_b.TRSB = ADC_UNIT_1_ELC_TRIGGER_GROUP_B;
                }
                else
                {
                    /**Nothing here. Created for LDRA*/
                }
            }
            /** If the group priority mode is group B in continuous scan mode, then disable the group B
             * synchronous trigger and disable the group B interrupt to prevent the ISR from triggering continuously */
            else
            {
                p_regs->ADSTRGR_b.TRSB = ADC_UNIT_n_ELC_TRIGGER_DISABLED;
                HW_ADC_S12gbadiDisable(unit);
            }
        }
        /** Enable the Normal Mode/Group A scan completion interrupt */
        HW_ADC_S12adiEnable(unit);
    }

    /** Set group A priority action (not interrupt priority!)
     * This will also start the Group B scans if configured for ADC_GROUP_A_GROUP_B_CONTINUOUS_SCAN.
     * This must be configured after the interrupts/triggers have been setup for the ADC_GROUP_A_GROUP_B_CONTINUOUS_SCAN */
    if (ADC_GROUP_A_GROUP_B_CONTINUOUS_SCAN ==  p_cfg->priority_group_a)
    {
        p_regs->ADGSPCR = p_cfg->priority_group_a;
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   HW_ADC_ScanCfgCheck
 *
 * This function does extensive checking on channel mask settings based upon operational mode.
 *
 * NOTE: A negative number is stored in two's complement form.
 *       A quick way to change a binary number into two's complement is to
 *       start at the right (LSB) and moving left, don't change any bits
 *       until after the first "1" is reached.
 *       Number          2's complement
 *       0010 0110       1101 1010
 *       Another way is to do a 1's complement on the number, then add 1 to that.
 *       Number          1's complement  + 1
 *       0010 0110       1101 1001       1101 1010
 *
 * @param[in]  unit     :  ADC unit number
 * @param[in]  p_cfg    :  Pointer to channel configuration structure containing masks
 *
 * @retval  SSP_SUCCESS -               Successful
 * @retval  SSP_ERR_INVALID_ARGUMENT -  Parameter has invalid/illegal value.
***********************************************************************************************************************/
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
ssp_err_t HW_ADC_ScanCfgCheck(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
    ssp_err_t err;
    uint32_t tmp_mask = 0;
    aregs_t  *p_regs = GET_REGS_PTR(unit);

    /** Verify at least one bonded channel is selected  */
    if ((0U == p_cfg->scan_mask)
     || (0U != (p_cfg->scan_mask & ADC_INVALID_CH_MASK(unit))))
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }

 /** Verify at least one unique bonded channel is selected for Group B
     * and GroupA priority setting is valid. */
    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR_b.ADCS)
    {
        if ((0U == p_cfg->scan_mask_group_b)
         || (0U != (p_cfg->scan_mask_group_b & ADC_INVALID_CH_MASK(unit))))
        {
            return SSP_ERR_INVALID_ARGUMENT;
        }
        else if (0 != (p_cfg->scan_mask & p_cfg->scan_mask_group_b))
        {
            return SSP_ERR_INVALID_ARGUMENT;         /*** Same channel in both groups */
        }
        else
        {
            tmp_mask = p_cfg->scan_mask_group_b;
        }

        if ((ADC_GROUP_A_PRIORITY_OFF != p_cfg->priority_group_a)
         && (ADC_GROUP_A_GROUP_B_WAIT_FOR_TRIGGER != p_cfg->priority_group_a)
         && (ADC_GROUP_A_GROUP_B_RESTART_SCAN != p_cfg->priority_group_a)
         && (ADC_GROUP_A_GROUP_B_CONTINUOUS_SCAN != p_cfg->priority_group_a))
        {
           return SSP_ERR_INVALID_ARGUMENT;
        }
    }
    /*** If Group mode was not enabled, but channels were selected for group B*/
    else
    {
        if (ADC_MASK_GROUP_B_OFF != p_cfg->scan_mask_group_b)
        {
            return SSP_ERR_INVALID_ARGUMENT;
        }
    }

    /** Verify sensors are in legal combination */
    err = HW_ADC_ScanCfgCheckSensors(unit, p_cfg);
    if (SSP_SUCCESS != err)
    {
        return err;
    }
    /** Verify that if addition is enabled, then at least one proper channel is selected*/
    if (ADC_ADD_OFF != p_regs->ADADC_b.ADC)
    {
        /** Addition mask should not include bits from inactive channels.
         * This also serves as a check for valid channels in the addition mask */
        tmp_mask |= p_cfg->scan_mask;   /*** tmp_mask is Group A and B combined   */
        /*** Bit-AND with 1s-complement   */
        if (0 != (p_cfg->add_mask & ~tmp_mask))
        {
            return SSP_ERR_INVALID_ARGUMENT;
        }
    }
    /** Channels are selected for addition despite addition being disabled.
        WARNING! Other features messed up if add_mask is non-zero when addition is turned off! */
    else
    {
        if (ADC_MASK_ADD_OFF != p_cfg->add_mask)
        {
             return SSP_ERR_INVALID_ARGUMENT;
        }
    }

    err = HW_ADC_ScanCfgCheckSampleHold(unit, p_cfg);

    return err;
}
#endif

/*******************************************************************************************************************//**
 * @brief   HW_ADC_SensorCfg
 *
 * This function set the sensor bits taking into account group inclusion and addition/average mode.
 *
 * @param[in]  p_cfg          : The configuration argument passed to R_ADC_ScanConfigure.
 * @param[in]  unit           :  ADC unit number
***********************************************************************************************************************/
void HW_ADC_SensorCfg(uint16_t const unit, adc_channel_cfg_t const * const p_cfg)
{
    aregs_t *p_regs = GET_REGS_PTR(unit);

    /** Temperature sensor  */
    if ((p_cfg->scan_mask & ADC_MASK_TEMPERATURE) || (p_cfg->scan_mask_group_b & ADC_MASK_TEMPERATURE))
    {


        HW_ADC_SensorCfgTemperature(unit, p_cfg);

    }

    /** Internal reference voltage sensor   */
    if ((p_cfg->scan_mask & ADC_MASK_VOLT) || (p_cfg->scan_mask_group_b & ADC_MASK_VOLT))
    {
        if (p_cfg->scan_mask & ADC_MASK_VOLT)
        {
            HW_ADC_SensorCfgVoltageGroupA(unit);    /** Select voltage output to GroupA  */
        }
        else
        {
            HW_ADC_SensorCfgVoltageGroupB(unit);      /** Select voltage output to GroupB  */
        }

        p_regs->ADEXICR_b.OCSAD = (p_cfg->add_mask & ADC_MASK_VOLT) ? 1U : 0U; /** Enable voltage addition mode if set */
    }

}

/*******************************************************************************************************************//**
 * @} (end addtogroup ADC)
***********************************************************************************************************************/
