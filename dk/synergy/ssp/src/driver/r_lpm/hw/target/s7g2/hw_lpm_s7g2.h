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
 * File Name    : hw_lpm_s7g2.h
 * Description  : LPM S7G2 specific macros.
 **********************************************************************************************************************/

#ifndef HW_LPM_S7G2_H_
#define HW_LPM_S7G2_H_

#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define HW_LPM_SNOOZE_REQUEST_MASK   0x7342FFFFU
#define HW_LPM_SNOOZE_END_MASK       0x000000FFU
#define HW_LPM_DPSIEGR2_MASK         0x00000013U
#define HW_LPM_MSTPCRA_MASK          0x004000E3U
#define HW_LPM_MSTPCRB_MASK          0xFFCCFBE6U
#define HW_LPM_MSTPCRC_MASK          0x80007BFFU
#define HW_LPM_MSTPCRD_MASK          0x1FD1C06CU
#define HW_LPM_WUPEN_MASK            0xFF4FFFFFU

#define DPSBYCR_DEEPCUT_SHIFT   (0U)
#define DPSBYCR_DEEPCUT_MASK    (3U << DPSBYCR_DEEPCUT_SHIFT)
#define DPSBYCR_DPSBY_SHIFT     (7U)
#define DPSBYCR_DPSBY_MASK      (1U << DPSBYCR_DPSBY_SHIFT)
#define DPSBYCR_IOKEEP_SHIFT    (6U)
#define DPSBYCR_IOKEEP_MASK     (1U << DPSBYCR_IOKEEP_SHIFT)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

bool HW_LPM_CHECK_DEEP_POWER_SUPPLY(lpm_power_supply_t power_supply);
bool HW_LPM_CHECK_DEEP_STANDBY_PIN(lpm_deep_standby_t pin_signal);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Set the value of the DPSBY.
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSBYSet (uint8_t value)
{
    uint8_t DSPBYCR_value = ((uint8_t)~DPSBYCR_DPSBY_MASK) & R_SYSTEM->DSPBYCR;
    DSPBYCR_value |= (uint8_t)DPSBYCR_DPSBY_MASK & (uint8_t)(value << DPSBYCR_DPSBY_SHIFT);
    R_SYSTEM->DSPBYCR = DSPBYCR_value;
}

/*******************************************************************************************************************//**
 * @brief Set the value of the DEEPCUT
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DEEPCUTSet (uint8_t value)
{
    uint8_t DSPBYCR_value = ((uint8_t)~DPSBYCR_DEEPCUT_MASK) & R_SYSTEM->DSPBYCR;
    DSPBYCR_value |= (uint8_t)DPSBYCR_DEEPCUT_MASK & (uint8_t)(value << DPSBYCR_DEEPCUT_SHIFT);
    R_SYSTEM->DSPBYCR = DSPBYCR_value;
}

/*******************************************************************************************************************//**
 * @brief Set the value of the IOKEEP
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_IOKEEPSet (uint8_t value)
{
    uint8_t DSPBYCR_value = ((uint8_t)~DPSBYCR_IOKEEP_MASK) & R_SYSTEM->DSPBYCR;
    DSPBYCR_value |= (uint8_t)DPSBYCR_IOKEEP_MASK & (uint8_t)(value << DPSBYCR_IOKEEP_SHIFT);
    R_SYSTEM->DSPBYCR = DSPBYCR_value;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIER0
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIER0Set (uint8_t value)
{
    R_SYSTEM->DPSIER0 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIER0
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIER0Get (void)
{
    return R_SYSTEM->DPSIER0;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIER1
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIER1Set (uint8_t value)
{
    R_SYSTEM->DPSIER1 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIER1
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIER1Get (void)
{
    return R_SYSTEM->DPSIER1;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIER2
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIER2Set (uint8_t value)
{
    R_SYSTEM->DPSIER2 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIER2
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIER2Get (void)
{
    return R_SYSTEM->DPSIER2;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIER3
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIER3Set (uint8_t value)
{
    R_SYSTEM->DPSIER3 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIER3
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIER3Get (void)
{
    return R_SYSTEM->DPSIER3;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIEGR0
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIEGR0Set (uint8_t value)
{
    R_SYSTEM->DPSIEGR0 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIEGR0
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIEGR0Get (void)
{
    return R_SYSTEM->DPSIEGR0;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIEGR1
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIEGR1Set (uint8_t value)
{
    R_SYSTEM->DPSIEGR1 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIEGR1
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIEGR1Get (void)
{
    return R_SYSTEM->DPSIEGR1;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIEGR2
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIEGR2Set (uint8_t value)
{
    R_SYSTEM->DPSIEGR2 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIEGR2
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIEGR2Get (void)
{
    return R_SYSTEM->DPSIEGR2;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIFR0
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIFR0Set (uint8_t value)
{
    R_SYSTEM->DPSIFR0 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIFR0
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIFR0Get (void)
{
    return R_SYSTEM->DPSIFR0;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIFR1
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIFR1Set (uint8_t value)
{
    R_SYSTEM->DPSIFR1 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIFR1
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIFR1Get (void)
{
    return R_SYSTEM->DPSIFR1;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIFR2
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIFR2Set (uint8_t value)
{
    R_SYSTEM->DPSIFR2 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIFR2
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIFR2Get (void)
{
    return R_SYSTEM->DPSIFR2;
}

/*******************************************************************************************************************//**
 * Set the value of DPSIFR3
 * @param  value    The value to set
 * @note This function assumes the register has been unlocked by the calling application
 **********************************************************************************************************************/
__STATIC_INLINE void HW_LPM_DPSIFR3Set (uint8_t value)
{
    R_SYSTEM->DPSIFR3 = value;
}

/*******************************************************************************************************************//**
 * Get the value of DPSIFR3
 * @return  The value of the register
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_LPM_DPSIFR3Get (void)
{
    return R_SYSTEM->DPSIFR3;
}

#endif /* HW_LPM_S7G2_H_ */
