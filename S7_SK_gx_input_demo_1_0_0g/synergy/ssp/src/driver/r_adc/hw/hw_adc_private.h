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
* File Name    : hw_adc_private.h
* Description  : ADC LLD implementation
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup ADC
 * @{
***********************************************************************************************************************/

#ifndef HW_ADC_PRIVATE_H
#define HW_ADC_PRIVATE_H

/**********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "bsp_api.h"
#include "r_adc.h"
#include "common/hw_adc_common.h"
/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/
void HW_ADC_Open(adc_cfg_t const * const p_cfg);
ssp_err_t HW_ADC_ScanCfgCheck(uint16_t const unit, adc_channel_cfg_t const * const p_cfg);
ssp_err_t HW_ADC_ScanCfgCheckSampleHold(uint16_t const unit, adc_channel_cfg_t const * const p_cfg);
void HW_ADC_ScanCfgSetSampleHold(uint16_t const unit, adc_channel_cfg_t const * const p_cfg);
ssp_err_t HW_ADC_ScanCfgCheckSensors(uint16_t const unit, adc_channel_cfg_t const * const p_cfg);
ssp_err_t HW_ADC_ScanCfg(uint16_t const unit, adc_channel_cfg_t const * const p_cfg);
void HW_ADC_SensorCfg(uint16_t const unit, adc_channel_cfg_t  const * const p_cfg);
void HW_ADC_SensorCfgTemperature(uint16_t const unit, adc_channel_cfg_t const * const p_cfg);
void HW_ADC_SensorCfgTemperatureGroupA(uint16_t const unit);
void HW_ADC_SensorCfgTemperatureGroupB(uint16_t const unit);
void HW_ADC_SensorCfgVoltageGroupA(uint16_t const unit);
void HW_ADC_SensorCfgVoltageGroupB(uint16_t const unit);
void HW_ADC_S12adiEnable(uint16_t unit);
void HW_ADC_S12gbadiEnable(uint16_t unit);
void HW_ADC_Close(uint16_t unit);

#endif /* HW_ADC_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup ADC)
***********************************************************************************************************************/
