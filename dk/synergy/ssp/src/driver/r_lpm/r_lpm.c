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
 * File Name    : r_lpm.c
 * Description  : LPM API implementation
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_lpm.h"
#include "r_lpm_private.h"
#include "r_lpm_private_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t s_lpm_version =
{
    .api_version_minor  = LPM_API_VERSION_MINOR,
    .api_version_major  = LPM_API_VERSION_MAJOR,
    .code_version_major = LPM_CODE_VERSION_MAJOR,
    .code_version_minor = LPM_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
const lpm_api_t g_lpm_on_lpm =
{
    .init                               = R_LPM_Init,
    .mstpcrSet                          = R_LPM_MSTPCRSet,
    .mstpcrGet                          = R_LPM_MSTPCRGet,
    .moduleStop                         = R_LPM_ModuleStop,
    .moduleStart                        = R_LPM_ModuleStart,
    .operatingPowerModeSet              = R_LPM_OperatingPowerModeSet,
    .snoozeEnable                       = R_LPM_SnoozeEnable,
    .snoozeDisable                      = R_LPM_SnoozeDisable,
    .lowPowerCfg                        = R_LPM_LowPowerConfigure,
    .deepStandbyCancelRequestEnable     = R_LPM_DeepStandbyCancelRequestEnable,
    .deepStandbyCancelRequestDisable    = R_LPM_DeepStandbyCancelRequestDisable,
    .lowPowerModeEnter                  = R_LPM_LowPowerModeEnter,
    .versionGet                         = R_LPM_VersionGet,
    .wupenSet                           = R_LPM_WUPENSet,
    .wupenGet                           = R_LPM_WUPENGet
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @addtogroup LPM
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Initialize the mcu operating power mode.
 *
 * Initialize the mcu operating power mode according to the passed in config structure.
 *
 * @retval SSP_SUCCESS             Configuration was successful
 * @retval SSP_ERR_ASSERTION       p_cfg was NULL
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_Init (lpm_cfg_t const * const p_cfg)
{
#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_cfg);
#endif

    return R_LPM_OperatingPowerModeSet(p_cfg->operating_power, p_cfg->sub_oscillator);
}

/*******************************************************************************************************************//**
 * @brief  Set the value of all the Module Stop Control Registers
 *
 * @retval SSP_SUCCESS            Value was successfully set
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_MSTPCRSet (uint32_t mstpcra_value,
                           uint32_t mstpcrb_value,
                           uint32_t mstpcrc_value,
                           uint32_t mstpcrd_value)
{
    HW_LPM_MSTPCRASet(mstpcra_value);
    HW_LPM_MSTPCRBSet(mstpcrb_value);
    HW_LPM_MSTPCRCSet(mstpcrc_value);
    HW_LPM_MSTPCRDSet(mstpcrd_value);
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Get the values of all the Module Stop Control Registers
 *
 * @retval SSP_SUCCESS            Value was successfully retrieved
 * @retval SSP_ERR_ASSERTION      One of the passed in pointers was NULL
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_MSTPCRGet (uint32_t * mstpcra_value,
                           uint32_t * mstpcrb_value,
                           uint32_t * mstpcrc_value,
                           uint32_t * mstpcrd_value)
{
#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(mstpcra_value);
    SSP_ASSERT(mstpcrb_value);
    SSP_ASSERT(mstpcrc_value);
    SSP_ASSERT(mstpcrd_value);
#endif

    *mstpcra_value = HW_LPM_MSTPCRAGet();
    *mstpcrb_value = HW_LPM_MSTPCRBGet();
    *mstpcrc_value = HW_LPM_MSTPCRCGet();
    *mstpcrd_value = HW_LPM_MSTPCRDGet();
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Stop a module
 *
 * Stop a module from running by setting the corresponding bit in the module stop register.
 *
 * @retval SSP_SUCCESS             Configuration was successful
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_ModuleStop (lpm_mstp_t module)
{
    uint32_t r_bit;
    uint32_t r_bit_shift;
    uint32_t r_reg;

#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(HW_LPM_CHECK_MSTP(module));
#endif

    /* module is a 16-bit value which includes the MSTPCR register (a-d) in the upper byte and
     * the stop bit position in the lower byte */

    r_reg = (uint32_t)module >> 8U;          /* r_reg holds the module stop bit register (a-d) to address */
    r_bit_shift = (uint32_t)module & 0xffU;
    r_bit = (uint32_t)(1U << r_bit_shift); /* r_bit is a bit mask that corresponds to the module to be stopped */

    switch (r_reg)
    {
        case 0x0aU:
            HW_LPM_MSTPCRASet(HW_LPM_MSTPCRAGet() | r_bit);
            break;
        case 0x0bU:
            HW_LPM_MSTPCRBSet(HW_LPM_MSTPCRBGet() | r_bit);
            break;
        case 0x0cU:
            HW_LPM_MSTPCRCSet(HW_LPM_MSTPCRCGet() | r_bit);
            break;
        case 0x0dU:
            HW_LPM_MSTPCRDSet(HW_LPM_MSTPCRDGet() | r_bit);
            break;
        default:
            break;
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Run a module
 *
 * Start a module running by clearing the corresponding bit in the module stop register.
 *
 * @retval SSP_SUCCESS             Configuration was successful
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_ModuleStart (lpm_mstp_t module)
{
    uint32_t r_bit;
    uint32_t r_bit_shift;
    uint32_t r_reg;

#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(HW_LPM_CHECK_MSTP(module));
#endif

    /* module is a 16-bit value which includes the MSTPCR register (a-d) in the upper byte and
     * the stop bit position in the lower byte */

    r_reg = (uint32_t)module >> 8U;          /* r_reg holds the module stop bit register (a-d) to address */
    r_bit_shift = (uint32_t)module & 0xffU;
    r_bit = (uint32_t)(1U << r_bit_shift); /* r_bit is a bit mask that corresponds to the module to be run */

    switch (r_reg)
    {
        case 0x0aU:
            HW_LPM_MSTPCRASet(HW_LPM_MSTPCRAGet() & (~r_bit));
            break;
        case 0x0bU:
            HW_LPM_MSTPCRBSet(HW_LPM_MSTPCRBGet() & (~r_bit));
            break;
        case 0x0cU:
            HW_LPM_MSTPCRCSet(HW_LPM_MSTPCRCGet() & (~r_bit));
            break;
        case 0x0dU:
            HW_LPM_MSTPCRDSet(HW_LPM_MSTPCRDGet() & (~r_bit));
            break;
        default:
            break;
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Set the operating power mode
 *
 * @retval SSP_SUCCESS             Operating power mode successfully set
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_OperatingPowerModeSet (lpm_operating_power_t power_mode, lpm_subosc_t subosc)
{

#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(HW_LPM_CHECK_OPERATING_POWER_MODE(power_mode));
#endif

    HW_LPM_RegisterUnLock();

    /* IMPORTANT: if in sub-osc mode, must clear SOPCCR.SOPCM before changing OPCCR.OPCM */
    if(LPM_SUBOSC_SELECT == HW_LPM_SubOscGet())
    {

        HW_LPM_WaitForPowerTransition();

        HW_LPM_SubOscSet(LPM_SUBOSC_OTHER);
    }

    HW_LPM_WaitForPowerTransition();

    HW_LPM_PowerModeSet(power_mode);

    HW_LPM_WaitForPowerTransition();

    HW_LPM_SubOscSet(subosc);

    HW_LPM_WaitForPowerTransition();

    HW_LPM_RegisterLock();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure and enable snooze mode
 *
 * NOTE: this function must be called before entering Software Standby mode, otherwise snooze mode will not be entered
 *
 * @retval SSP_SUCCESS             Snooze mode successfully enabled
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_SnoozeEnable (lpm_snooze_rxd0_t    rdx0_mode,
                              lpm_snooze_dtc_t     dtc_mode,
                              lpm_snooze_request_t requests,
                              lpm_snooze_end_t     triggers)
{

#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(0U == ((uint32_t)rdx0_mode & ~(uint32_t)1U));
    SSP_ASSERT(0U == ((uint32_t)dtc_mode & ~(uint32_t)1U));
    SSP_ASSERT(0U == ((uint32_t)requests & ~HW_LPM_SNOOZE_REQUEST_MASK));
    SSP_ASSERT(0U == ((uint32_t)triggers & ~HW_LPM_SNOOZE_END_MASK));
    /* Note: If using the RXD0 falling edge as the snooze request,
     * any event other than the RXD0 falling edge must not be
     * enabled as snooze request.
     */
    SSP_ASSERT((LPM_SNOOZE_RXD0_FALLING_EDGE_IGNORE == rdx0_mode)   ||
               (LPM_SNOOZE_REQUEST_RXD0_FALLING     == requests )
               );
#endif

    HW_LPM_RegisterUnLock();

    HW_LPM_SNZESet(0U);         /* Disable snooze mode while configuring it */
    HW_LPM_RXDREQENSet(rdx0_mode); /* Configure RXD0 falling edge detect */
    HW_LPM_SNZDTCENSet(dtc_mode);  /* Enable/disable DTC operation */
    HW_LPM_SNZREQCRSet((uint32_t)requests);  /* Set all the request conditions that can trigger entry in to snooze mode */
    HW_LPM_SNZEDCRSet((uint8_t)triggers);   /* Set all triggers that can cause an exit from snooze mode */
    HW_LPM_SNZESet((uint8_t)1U);          /* Enable snooze mode */

    HW_LPM_RegisterLock();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Disable snooze mode
 *
 * @retval SSP_SUCCESS             Snooze mode successfully disabled
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_SnoozeDisable (void)
{
    HW_LPM_RegisterUnLock();

    HW_LPM_SNZESet(0U);         /* Disable snooze mode while configuring it */

    HW_LPM_RegisterLock();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure a low power mode
 *
 * NOTE: This function does not enter the low power mode, it only configures parameters of the mode. Execution of
 *       the WFI instruction is what causes the low power mode to be entered.
 *
 * @retval SSP_SUCCESS             Sleep mode entered successfully
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_LowPowerConfigure (lpm_low_power_mode_t     power_mode,
                                   lpm_output_port_enable_t output_port_enable,
                                   lpm_power_supply_t       power_supply,
                                   lpm_io_port_t            io_port_state)
{

#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(HW_LPM_CHECK_LOW_POWER_MODE(power_mode));
    #if defined(BSP_MCU_GROUP_S7G2)
        SSP_ASSERT(HW_LPM_CHECK_DEEP_POWER_SUPPLY(power_supply));
    #endif /* defined(BSP_MCU_GROUP_S7G2) */
#endif

    HW_LPM_RegisterUnLock();

    switch (power_mode)
    {
        case LPM_LOW_POWER_MODE_SLEEP:
            HW_LPM_SSBYSet(0U);
            #if defined(BSP_MCU_GROUP_S7G2)
                HW_LPM_DPSBYSet(0U);
            #endif /* defined(BSP_MCU_GROUP_S7G2) */
            break;
        case LPM_LOW_POWER_MODE_STANDBY:
            #if !defined(BSP_MCU_GROUP_S124)
                HW_LPM_OPESet((uint16_t)output_port_enable);
            #else
                SSP_PARAMETER_NOT_USED(output_port_enable);
            #endif /* defined(BSP_MCU_GROUP_S124) */
            #if defined(BSP_MCU_GROUP_S7G2)
                HW_LPM_DPSBYSet(0U);
            #endif /* defined(BSP_MCU_GROUP_S7G2) */
            HW_LPM_SSBYSet((uint16_t)1U);
            break;
        case LPM_LOW_POWER_MODE_DEEP:
            #if defined(BSP_MCU_GROUP_S7G2)
                HW_LPM_DEEPCUTSet(power_supply);
                HW_LPM_IOKEEPSet(io_port_state);
                HW_LPM_DPSBYSet((uint8_t)1U);
                HW_LPM_SSBYSet((uint16_t)1U);
            #else
                SSP_PARAMETER_NOT_USED(power_supply);
                SSP_PARAMETER_NOT_USED(io_port_state);
            #endif /* defined(BSP_MCU_GROUP_S7G2) */
            break;
        default:
            break;
    }

    HW_LPM_RegisterLock();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Set the value of the Wake Up Interrupt Enable Register WUPEN
 *
 * @retval SSP_SUCCESS            Value was successfully set
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_WUPENSet (uint32_t wupen_value)
{
    HW_LPM_WUPENSet(wupen_value);
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Get the value of the Wake Up Interrupt Enable Register WUPEN
 *
 * @retval SSP_SUCCESS            Value was successfully retrieved
 * @retval SSP_ERR_ASSERTION      One of the passed in pointers was NULL
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_WUPENGet (uint32_t * wupen_value)
{
#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(wupen_value);
#endif

    *wupen_value = HW_LPM_WUPENGet();
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable a Deep Standby Cancel Request
 *
 * Enable a pin or signal that will cancel Deep Software Standby mode
 *
 * @retval SSP_SUCCESS             Deep Software Standby cancel request enabled
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_DeepStandbyCancelRequestEnable (lpm_deep_standby_t pin_signal, lpm_cancel_request_edge_t rising_falling)
{

#if defined(BSP_MCU_GROUP_S7G2)

    uint8_t r_bit;
    uint8_t r_bits;
    uint8_t r_reg;

#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(HW_LPM_CHECK_DEEP_STANDBY_PIN(pin_signal));
#endif

    /* pin_signal is an 8-bit value which includes the Deep Standby Interrupt register in the upper nibble and
     * the interrupt bit position in the lower nibble */

    r_reg  = (uint8_t)pin_signal >> 4U;   /* the register nibble */
    r_bits = (uint8_t)pin_signal & 0x0fU; /* the bit position nibble */
    r_bit  = (uint8_t)(1U << r_bits);     /* the bit mask */

    HW_LPM_RegisterUnLock();

    /* select one of the 4 Deep Standby Interrupt registers */
    switch (r_reg)
    {
        case 0U:
            HW_LPM_DPSIER0Set(HW_LPM_DPSIER0Get() | r_bit); /* set the interrupt enable bit */
            if (rising_falling == LPM_CANCEL_REQUEST_EDGE_RISING)
            {
                HW_LPM_DPSIEGR0Set(HW_LPM_DPSIEGR0Get() | r_bit); /* interrupt on a rising edge */
            }
            else
            {
                HW_LPM_DPSIEGR0Set(HW_LPM_DPSIEGR0Get() & ((uint8_t)~r_bit)); /* interrupt on a falling edge */
            }
            HW_LPM_DPSIFR0Get(); /* To clear DPSIFRn after modifying DPSIERn, */
                                 /* wait at least six PCLKB cycles, read, and then write 0. */
            HW_LPM_DPSIFR0Set(0U); /* Each DPSIFRn flag may be set when the setting of DPSIERn is modified */

            break;

        case 1U:
            HW_LPM_DPSIER1Set(HW_LPM_DPSIER1Get() | r_bit); /* set the interrupt enable bit */
            if (rising_falling == LPM_CANCEL_REQUEST_EDGE_RISING)
            {
                HW_LPM_DPSIEGR1Set(HW_LPM_DPSIEGR1Get() | r_bit); /* interrupt on a rising edge */
            }
            else
            {
                HW_LPM_DPSIEGR1Set(HW_LPM_DPSIEGR1Get() & ((uint8_t)~r_bit)); /* interrupt on a falling edge */
            }
            HW_LPM_DPSIFR1Get(); /* To clear DPSIFRn after modifying DPSIERn, */
                                 /* wait at least six PCLKB cycles, read, and then write 0. */
            HW_LPM_DPSIFR1Set(0U); /* Each DPSIFRn flag may be set when the setting of DPSIERn is modified */

            break;

        case 2U:
            HW_LPM_DPSIER2Set(HW_LPM_DPSIER2Get() | r_bit); /* set the interrupt enable bit */
            if (0U == (r_bit & ~HW_LPM_DPSIEGR2_MASK)) /* only set the edge on valid sources */
            {
                if (rising_falling == LPM_CANCEL_REQUEST_EDGE_RISING)
                {
                    HW_LPM_DPSIEGR2Set(HW_LPM_DPSIEGR2Get() | r_bit); /* interrupt on a rising edge */
                }
                else
                {
                    HW_LPM_DPSIEGR2Set(HW_LPM_DPSIEGR2Get() & ((uint8_t)~r_bit)); /* interrupt on a falling edge */
                }
            }
            HW_LPM_DPSIFR2Get(); /* To clear flag after modifying DPSIERn, */
                                 /* wait at least six PCLKB cycles, read, and then write 0. */
            HW_LPM_DPSIFR2Set(0U); /* Each DPSIFRn flag may be set when the setting of DPSIERn is modified */

            break;

        case 3U:
            HW_LPM_DPSIER3Set(HW_LPM_DPSIER3Get() | r_bit); /* set the interrupt enable bit */
            HW_LPM_DPSIER3Get(); /* Six or more PCLKB cycles can be secured, for example, by reading DPSIER0. */
            HW_LPM_DPSIFR3Get(); /* To clear DPSIFRn after modifying DPSIERn, */
                                 /* wait at least six PCLKB cycles, read, and then write 0. */
            HW_LPM_DPSIFR3Set(0U); /* Each DPSIFRn flag may be set when the setting of DPSIERn is modified */

            break;
        default:
            break;
    }

    HW_LPM_RegisterLock();

#else
    (void)pin_signal;
    (void)rising_falling;
#endif /* defined(BSP_MCU_GROUP_S7G2) */

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Disable a Deep Standby Cancel Request
 *
 * Disable a pin or signal that will cancel Deep Software Standby mode
 *
 * @retval SSP_SUCCESS             Deep Software Standby cancel request disabled
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_DeepStandbyCancelRequestDisable (lpm_deep_standby_t pin_signal)
{

#if defined(BSP_MCU_GROUP_S7G2)

    uint8_t r_bit;
    uint8_t r_bits;
    uint8_t r_reg;

#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(HW_LPM_CHECK_DEEP_STANDBY_PIN(pin_signal));
#endif

    /* pin_signal is an 8-bit value which includes the Deep Standby Interrupt register in the upper nibble and
     * the interrupt bit position in the lower nibble */

    r_reg  = (uint8_t)pin_signal >> 4U;   	/* the register nibble */
    r_bits = (uint8_t)pin_signal & 0x0fU; 	/* the bit position nibble */
    r_bit  = (uint8_t)(1U << r_bits);     	/* the bit mask */

    HW_LPM_RegisterUnLock();

    /* select one of the 4 Deep Standby Interrupt registers */
    switch (r_reg)
    {
        case 0U:
            HW_LPM_DPSIER0Set(HW_LPM_DPSIER0Get() & ((uint8_t)~r_bit)); /* clear the interrupt enable bit */
            break;

        case 1U:
            HW_LPM_DPSIER1Set(HW_LPM_DPSIER1Get() & ((uint8_t)~r_bit)); /* clear the interrupt enable bit */
            break;

        case 2U:
            HW_LPM_DPSIER2Set(HW_LPM_DPSIER2Get() & ((uint8_t)~r_bit)); /* clear the interrupt enable bit */
            break;

        case 3U:
            HW_LPM_DPSIER3Set(HW_LPM_DPSIER3Get() & ((uint8_t)~r_bit)); /* clear the interrupt enable bit */
            break;
        default:
            break;
    }

    HW_LPM_RegisterLock();

#else
    (void)pin_signal;
#endif /* if defined(BSP_MCU_GROUP_S7G2) */

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      Enter low power mode (sleep/standby/deep standby) using WFI macro.
 *
 * Function will return after waking from low power mode.
 *
 * @retval     SSP_SUCCESS          Successful.
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_LowPowerModeEnter (void)
{
    __WFI();
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      Get the driver version based on compile time macros.
 *
 * @retval     SSP_SUCCESS          Successful close.
 * @retval     SSP_ERR_INVALID_PTR  p_version is NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_LPM_VersionGet (ssp_version_t * const p_version)
{
#if LPM_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_version);
#endif

    p_version->version_id = s_lpm_version.version_id;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup LPM)
 **********************************************************************************************************************/
