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
 * File Name    : hw_lpm_common.h
 * Description  : LPM
 **********************************************************************************************************************/


/*******************************************************************************************************************//**

 **********************************************************************************************************************/

#ifndef HW_LPM_COMMON_H
#define HW_LPM_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_lpm_api.h"

#if defined(BSP_MCU_GROUP_S7G2)
#include "../target/s7g2/hw_lpm_s7g2.h"
#endif

#if defined(BSP_MCU_GROUP_S3A7)
#include "../target/s3a7/hw_lpm_s3a7.h"
#endif

#if defined(BSP_MCU_GROUP_S124)
#include "../target/s124/hw_lpm_s124.h"
#endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define OPCCR_OPCM_SHIFT        (0U)
#define OPCCR_OPCM_MASK         (3U << OPCCR_OPCM_SHIFT)
#define SOPCCR_SOPCM_SHIFT      (0U)
#define SOPCCR_SOPCM_MASK       (1U << SOPCCR_SOPCM_SHIFT)
#define SNZCR_RXDREQEN_SHIFT    (0U)
#define SNZCR_RXDREQEN_MASK     (1U << SNZCR_RXDREQEN_SHIFT)
#define SNZCR_SNZDTCEN_SHIFT    (1U)
#define SNZCR_SNZDTCEN_MASK     (1U << SNZCR_SNZDTCEN_SHIFT)
#define SNZCR_SNZE_SHIFT        (7U)
#define SNZCR_SNZE_MASK         (1U << SNZCR_SNZE_SHIFT)
#define SBYCR_SSBY_SHIFT        (15U)
#define SBYCR_SSBY_MASK         (1U << SBYCR_SSBY_SHIFT)
#define SBYCR_OPE_SHIFT         (14U)
#define SBYCR_OPE_MASK          (1U << SBYCR_OPE_SHIFT)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

bool HW_LPM_CHECK_MSTP(lpm_mstp_t module);
bool HW_LPM_CHECK_OPERATING_POWER_MODE(lpm_operating_power_t power_mode);
bool HW_LPM_CHECK_LOW_POWER_MODE(lpm_low_power_mode_t low_power_mode);
void HW_LPM_WaitForPowerTransition (void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      This function locks LPM registers
 * @retval     noone
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_RegisterLock (void)
{
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
}

/*******************************************************************************************************************//**
 * @brief      This function unlocks LPM registers
 * @retval     none
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_RegisterUnLock (void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
}

/*******************************************************************************************************************//**
 * Set the Module Stop Control Register A
 * @param  value    The value to set
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_MSTPCRASet (uint32_t value)
{
    R_SYSTEM->MSTPCRA = value;
}

/*******************************************************************************************************************//**
 * Get the value of the Module Stop Control Register A
 * @return  The value of the register
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S */
__STATIC_INLINE uint32_t HW_LPM_MSTPCRAGet (void)
{
    return R_SYSTEM->MSTPCRA;
}

/*******************************************************************************************************************//**
 * Set the Module Stop Control Register B
 * @param  value    The value to set
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_MSTPCRBSet (uint32_t value)
{
    R_MSTP->MSTPCRB = value;
}

/*******************************************************************************************************************//**
 * Get the value of the Module Stop Control Register B
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t HW_LPM_MSTPCRBGet (void)
{
    return R_MSTP->MSTPCRB;
}

/*******************************************************************************************************************//**
 * Set the Module Stop Control Register C
 * @param  value    The value to set
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_MSTPCRCSet (uint32_t value)
{
    R_MSTP->MSTPCRC = value;
}

/*******************************************************************************************************************//**
 * Get the value of the Module Stop Control Register C
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t HW_LPM_MSTPCRCGet (void)
{
    return R_MSTP->MSTPCRC;
}

/*******************************************************************************************************************//**
 * Set the Module Stop Control Register D
 * @param  value    The value to set
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_MSTPCRDSet (uint32_t value)
{
    R_MSTP->MSTPCRD = value;
}

/*******************************************************************************************************************//**
 * Get the value of the Module Stop Control Register D
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t HW_LPM_MSTPCRDGet (void)
{
    return R_MSTP->MSTPCRD;
}

/*******************************************************************************************************************//**
 * Set the value of power mode
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_PowerModeSet (uint8_t value)
{
    uint8_t OPCCR_value = ((uint8_t)~OPCCR_OPCM_MASK) & R_SYSTEM->OPCCR;
    OPCCR_value |= (uint8_t)OPCCR_OPCM_MASK & (uint8_t)(value << OPCCR_OPCM_SHIFT);
    R_SYSTEM->OPCCR = OPCCR_value;
}

/*******************************************************************************************************************//**
 * Set the value of the subosc
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_SubOscSet (uint8_t value)
{
    uint8_t SOPCCR_value = ((uint8_t)~SOPCCR_SOPCM_MASK) & R_SYSTEM->SOPCCR;
    SOPCCR_value |= (uint8_t)SOPCCR_SOPCM_MASK & (uint8_t)(value << SOPCCR_SOPCM_SHIFT);
    R_SYSTEM->SOPCCR = SOPCCR_value;
}

/*******************************************************************************************************************//**
 * Get the value of the subosc
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_SubOscGet (void)
{
    return (R_SYSTEM->SOPCCR & (uint8_t)SOPCCR_SOPCM_MASK) >> SOPCCR_SOPCM_SHIFT;
}

/*******************************************************************************************************************//**
 * Set the value of the SNZE
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_SNZESet (uint8_t value)
{
    uint8_t SNZCR_value = ((uint8_t)~SNZCR_SNZE_MASK) & R_SYSTEM->SNZCR;
    SNZCR_value |= (uint8_t)SNZCR_SNZE_MASK & (uint8_t)(value << SNZCR_SNZE_SHIFT);
    R_SYSTEM->SNZCR = SNZCR_value;
}

/*******************************************************************************************************************//**
 * Set the value of the RXDREQEN
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_RXDREQENSet (uint8_t value)
{
    uint8_t SNZCR_value = ((uint8_t)~SNZCR_RXDREQEN_MASK) & R_SYSTEM->SNZCR;
    SNZCR_value |= (uint8_t)SNZCR_RXDREQEN_MASK & (uint8_t)(value << SNZCR_RXDREQEN_SHIFT);
    R_SYSTEM->SNZCR = SNZCR_value;
}

/*******************************************************************************************************************//**
 * Set the value of the SNZDTCEN
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_SNZDTCENSet (uint8_t value)
{
    uint8_t SNZCR_value = ((uint8_t)~SNZCR_SNZDTCEN_MASK) & R_SYSTEM->SNZCR;
    SNZCR_value |= (uint8_t)SNZCR_SNZDTCEN_MASK & (uint8_t)(value << SNZCR_SNZDTCEN_SHIFT);
    R_SYSTEM->SNZCR = SNZCR_value;
}

/*******************************************************************************************************************//**
 * Set the value of the SNZREQCR
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_SNZREQCRSet (uint32_t value)
{
    R_SYSTEM->SNZREQCR = value;
}

/*******************************************************************************************************************//**
 * Set the value of the SNZEDCR
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_SNZEDCRSet (uint8_t value)
{
    R_SYSTEM->SNZEDCR = value;
}

/*******************************************************************************************************************//**
 * Set the value of the SSBY
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_SSBYSet (uint16_t value)
{
    uint16_t SBYCR_value = ((uint16_t)~SBYCR_SSBY_MASK) & R_SYSTEM->SBYCR;
    SBYCR_value |= (uint16_t)SBYCR_SSBY_MASK & (uint16_t)(value << SBYCR_SSBY_SHIFT);
    R_SYSTEM->SBYCR = SBYCR_value;
}

/*******************************************************************************************************************//**
 * Set the value of the OPE
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_OPESet (uint16_t value)
{
    uint16_t SBYCR_value = ((uint16_t)~SBYCR_OPE_MASK) & R_SYSTEM->SBYCR;
    SBYCR_value |= (uint16_t)SBYCR_OPE_MASK & (uint16_t)(value << SBYCR_OPE_SHIFT);
    R_SYSTEM->SBYCR = SBYCR_value;
}

/*******************************************************************************************************************//**
 * Set the value of the Wake Up Interrupt Enable Register WUPEN
 * @param  value    The value to set
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_WUPENSet (uint32_t value)
{
    R_ICU->WUPEN = value;
}

/*******************************************************************************************************************//**
 * Get the value of the Wake Up Interrupt Enable Register WUPEN
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t HW_LPM_WUPENGet (void)
{
    return R_ICU->WUPEN;
}

#endif /* HW_LPM_COMMON_H */
