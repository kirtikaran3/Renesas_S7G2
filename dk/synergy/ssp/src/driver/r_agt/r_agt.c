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
 * File Name    : r_agt.c
 * Description  : Driver API for Asynchronous General Purpose Timer
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "hw/hw_agt_private.h"
#include "r_agt_private.h"
#include "r_agt_private_api.h"
#include "r_agt.h"
#include "r_cgc.h"


/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static ssp_err_t agt_period_set(uint8_t const ch, timer_size_t const period, timer_unit_t const unit);
static ssp_err_t agt_counter_stop_wait(uint8_t channel);
static uint32_t agt_clock_frequency_get(uint8_t const channel);

/* Helper function prototypes exposed for use by Unit Tests. */
ssp_err_t agt_ns_to_counts  (timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts);
ssp_err_t agt_us_to_counts  (timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts);
ssp_err_t agt_ms_to_counts  (timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts);
ssp_err_t agt_s_to_counts   (timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts);
ssp_err_t agt_hz_to_counts  (timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts);
ssp_err_t agt_khz_to_counts (timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts);

/* ISRs. */
void agt0_int_isr (void);
void agt1_int_isr (void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** Bit vector of channels in use */
static uint32_t     g_agt_in_use_flag = 0;

/** Bit vector of channels in one-shot mode */
static uint32_t     g_agt_one_shot_flag = 0;

/** Array of control blocks for AGT channels. */
static timer_ctrl_t * gp_ctrls[AGT_MAX_CH] =
{
    NULL
};

/** Array of timer period configuration values. */
static timer_size_t      g_timer_period[AGT_MAX_CH];

#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t s_agt_version =
{
    .api_version_minor  = TIMER_API_VERSION_MINOR,
    .api_version_major  = TIMER_API_VERSION_MAJOR,
    .code_version_minor = AGT_CODE_VERSION_MINOR,
    .code_version_major = AGT_CODE_VERSION_MAJOR,
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/** AGT Implementation of General Timer Driver  */

const timer_api_t g_timer_on_agt =
{
    .open       = R_AGT_TimerOpen,
    .stop       = R_AGT_Stop,
    .start      = R_AGT_Start,
    .reset      = R_AGT_Reset,
    .counterGet = R_AGT_CounterGet,
    .periodSet  = R_AGT_PeriodSet,
    .dutyCycleSet = R_AGT_DutyCycleSet,
    .infoGet    = R_AGT_InfoGet,
    .close      = R_AGT_Close,
    .versionGet = R_AGT_VersionGet
};

/** @addtogroup AGT
 * @{*/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Open the AGT channel as a timer, handles required initialization described in hardware manual. Implements
 * timer_api_t::open.
 *
 * The Timer Open function configures a single AGT channel for timer mode with parameters specified in the timer
 * Configuration structure. It also sets up the control block for use with subsequent AGT Timer APIs.
 * 
 * This function must be called once prior to calling any other AGT API functions. After a channel is opened, the Open
 * function should not be called again for the same channel without first calling the associated Close function.
 *
 * The AGT hardware does not support one-shot functionality natively.  The one-shot feature is therefore implemented in
 * the AGT HAL layer. For a timer configured as a one-shot timer, the timer is stopped upon the first timer expiration.
 *
 * The AGT implementation of the general timer can accept a ::timer_on_agt_cfg_t extension parameter.  For AGT, the
 * extension specifies the clock to be used as timer source.  If the extension parameter is not specified, the default
 * clock PCLKB is used.  The clock divider is selected based on the source clock frequency and the timer period supplied
 * by the caller.
 *
 * @retval SSP_SUCCESS           Initialization was successful and timer has started.
 * @retval SSP_ERR_ASSERTION     One of the following parameters may be NULL: p_cfg, p_ctrl,  or the
 *                               configuration channel ID exceeds AGT_MAX_CH, or the configuration mode is invalid.
 * @retval SSP_ERR_IN_USE        The channel specified has already been opened.
 * @retval SSP_ERR_IRQ_BSP_DISABLED A required interrupt has not been enabled in the BSP.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 **********************************************************************************************************************/
ssp_err_t R_AGT_TimerOpen(timer_ctrl_t      * const p_ctrl,
                          timer_cfg_t const * const p_cfg)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(p_cfg->channel < AGT_MAX_CH);
#endif

    ssp_err_t   error;
    agt_mode_t  agt_mode = AGT_MODE_TIMER;

    /** Verify channel is not already used */
    /* All AGT channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    bsp_hw_lock_t lock    = (bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_AGT0 + p_cfg->channel);
    ssp_err_t     bsp_err = R_BSP_HardwareLock(lock);
    AGT_ERROR_RETURN((SSP_SUCCESS == bsp_err), SSP_ERR_IN_USE);
    
    uint32_t ch_mask;
    ch_mask = (uint32_t) (1UL << p_cfg->channel);
    AGT_ERROR_RETURN((!(ch_mask == (g_agt_in_use_flag & ch_mask))), SSP_ERR_IN_USE);

    /** Power on the AGT channel. */
    HW_AGT_PowerOn(p_cfg->channel);

    /** Wait for counter to stop. */
    error = agt_counter_stop_wait(p_cfg->channel);
    if (SSP_SUCCESS == error)
    {
        /** Clear AGTO output */
        HW_AGT_IOModeSet(p_cfg->channel, 0);

        /** Clear TEDGSEL bit to normal output. */
        HW_AGT_IOTEdgeSelectSet(p_cfg->channel, 0);

        /** Enable IRQ if user supplied a callback function,
         *  or if the timer is a one-shot timer (so the driver is able to
         *  turn off the timer after one period. */
        if ((NULL != p_cfg->p_callback) || (TIMER_MODE_ONE_SHOT == p_cfg->mode))
        {
            IRQn_Type irq = HW_AGT_IRQGet(p_cfg->channel);

            /* Return if IRQ is disabled. */
            if (BSP_MAX_NUM_IRQn > irq)
            {
                /* Clear flags in control register. */
                HW_AGT_CounterFlagsClear(p_cfg->channel);

                /** Clear any pending interrupts, and then enable interrupt. */
                HW_ICU_InterruptClear(irq);
                NVIC_ClearPendingIRQ(irq);
                NVIC_EnableIRQ(irq);

                /* Record user supplied callback routine. */
                p_ctrl->p_callback = p_cfg->p_callback;
                p_ctrl->p_context  = p_cfg->p_context;
            }
            else
            {
                error = SSP_ERR_IRQ_BSP_DISABLED;
            }
        }
    }

    if (SSP_SUCCESS == error)
    {
        /** If AGT timer configuration structure is present, pick up the counter source and output
         *  pin configuration.
         *  Valid values for counter source are:
         * - AGT_TCK_PCLKB
         * - AGT_TCK_LOCO
         * - AGT_TCK_FSUB
         *
         *  AGTO pin and AGTIO pin can be independently enabled for output.  The following describes the mode
         *  the AGT channel is configured to, based on user-specified AGTO and AGTIO pin output.
         *
         * If both AGT0 and AGTIO are disabled, AGT is open in timer mode with TOE bit cleared.
         * If AGT0 is enabled and AGTIO is disabled,  AGT is open in timer mode with TOE bit set.
         * If AGT0 is disabled and AGTIO is enabled, AGT is open in pulse output mode with TOE bit cleared.
         * If both AGT0 and AGTIO are enabled,  AGT is open in pulse output mode, TOE bit set.
         */
        if (NULL != p_cfg->p_extend)
        {
            timer_on_agt_cfg_t const * p_ext = (timer_on_agt_cfg_t const *) p_cfg->p_extend;
            HW_AGT_CountSourceSet(p_cfg->channel, p_ext->count_source);

            if (true == p_ext->agtio_output_enabled)
            {
                /* If AGTIO output pin is enabled, put the AGT in Pulse Output mode. Otherwise, use Timer Mode. */
                agt_mode = AGT_MODE_PULSE_OUTPUT;
            }

            if (true == p_ext->agto_output_enabled)
            {
                /* Set the TOE bit if AGTO is enabled.  AGTO can be enabled in any mode. */
                HW_AGT_IOModeSet(p_cfg->channel, (uint8_t) 1U);
            }

            if ((true == p_ext->agto_output_enabled) || (true == p_ext->agtio_output_enabled))
            {
                uint8_t invert;
                if (p_ext->output_inverted)
                {
                    invert = 1U;
                }
                else
                {
                    invert = 0U;
                }
                /* Configure TEDGSEL bit based on user input. */
                HW_AGT_IOTEdgeSelectSet(p_cfg->channel, invert);
            }
        }
        else
        {
            /** If AGT timer configuration structure is not present, use default clock AGT_TCK_PCLKB as the clock
             *  source for the timer mode. */
            HW_AGT_CountSourceSet(p_cfg->channel, AGT_CLOCK_PCLKB);
        }

        /** Set the AGT mode based on agt_mode value.  */
        HW_AGT_ModeSet(p_cfg->channel, agt_mode);

        /** Make sure period is valid, then set period */
        error = agt_period_set(p_cfg->channel, p_cfg->period, p_cfg->unit);
    }
    if (SSP_SUCCESS != error)
    {
        /* Initialization not successful, power off and release hardware lock. */
        HW_AGT_PowerOff(p_cfg->channel);
        R_BSP_HardwareUnlock(lock);
    }
    AGT_ERROR_RETURN(SSP_SUCCESS == error, error);

    /** Mark channel as 'in use' and initialize control block. */
    g_agt_in_use_flag       |= ch_mask;
    p_ctrl->channel          = p_cfg->channel;
    gp_ctrls[p_cfg->channel] = p_ctrl;
    if (TIMER_MODE_ONE_SHOT == p_cfg->mode)
    {
        g_agt_one_shot_flag |= ch_mask;
    }

    /** Start the timer if requested by user */
    if (p_cfg->autostart)
    {
        HW_AGT_CounterStartStop(p_cfg->channel, AGT_START);
    }

    /** All done.  */
    return SSP_SUCCESS;
} /* End of function R_AGT_TimerOpen */

/*******************************************************************************************************************//**
 * Stops counter, disables interrupts, disables output pins, and clears internal driver data.
 *
 * @retval     SSP_SUCCESS          The AGT Timer channel is successfully closed.
 * @retval     SSP_ERR_ASSERTION    The parameter p_ctrl is NULL.
 * @retval     SSP_ERR_NOT_OPEN     The AGT channel is not opened.
 **********************************************************************************************************************/

ssp_err_t R_AGT_Close(timer_ctrl_t * const p_ctrl)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(AGT_MAX_CH > p_ctrl->channel);
#endif

    /** Make sure channel is open. If it is not, return with error code SSP_ERR_NOT_OPEN. */
    uint32_t ch_mask;
    ch_mask = (uint32_t)1UL << p_ctrl->channel;

#ifdef AGT_CFG_PREREQUISITE_CHECKING_ENABLE
    AGT_ERROR_RETURN((g_agt_in_use_flag & ch_mask), SSP_ERR_NOT_OPEN);
#endif

    /** Clear driver internal flags. */
    g_agt_in_use_flag   &= (~ch_mask);
    g_agt_one_shot_flag &= (~ch_mask);

    /** Cleanup the device: Stop counter, disable interrupts, and power down if no other channels are in use. */
    ssp_err_t error;
    error = agt_counter_stop_wait(p_ctrl->channel);
    AGT_ERROR_RETURN((SSP_SUCCESS == error), error);

    /** Clear the TOE (output enable) bit */
    HW_AGT_IOModeSet(p_ctrl->channel, 0);

    /** Clear the TEDGSEL bit */
    HW_AGT_IOTEdgeSelectSet(p_ctrl->channel, 0);

    IRQn_Type irq = HW_AGT_IRQGet(p_ctrl->channel);
    if (BSP_MAX_NUM_IRQn > irq)
    {
        NVIC_DisableIRQ(irq);
    }

    HW_AGT_PowerOff(p_ctrl->channel);

    /** Unlock channel */
    /* All AGT channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    bsp_hw_lock_t lock    = (bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_AGT0 + p_ctrl->channel);
    R_BSP_HardwareUnlock(lock);

    return SSP_SUCCESS;
} /* End of function R_AGT_Close */

/*******************************************************************************************************************//**
 * Retrieve and store counter value in provided p_value pointer. Implements timer_api_t::CounterGet.
 *
 * @retval SSP_SUCCESS           Counter value read, p_value is valid.
 * @retval SSP_ERR_ASSERTION     The p_ctrl or p_value parameter was null
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_AGT_CounterGet(timer_ctrl_t * const p_ctrl,
                           timer_size_t * const p_value)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure parameters are valid */
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_value);
#endif
#ifdef AGT_CFG_PREREQUISITE_CHECKING_ENABLE
    AGT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Read counter value */
    *p_value = HW_AGT_CounterGet(p_ctrl->channel);
    return SSP_SUCCESS;
} /* End of function R_AGT_CounterGet */

/*******************************************************************************************************************//**
 * Sets period value provided. Implements timer_api_t::periodSet.
 *
 * @retval SSP_SUCCESS           Period value written successfully.
 * @retval SSP_ERR_ASSERTION     The p_ctrl parameter was null.
 * @retval SSP_ERR_INVALID_ARG   One of the following is invalid:
 *                                    - p_period->unit: must be one of the options from timer_size_t::unit
 *                                    - p_period->value: must result in a period supported by the clock source
 *                                      specified during the Open call.
 *
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_AGT_PeriodSet (timer_ctrl_t    * const p_ctrl,
                           timer_size_t const    period,
                           timer_unit_t const      unit)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure handle is valid. */
    SSP_ASSERT(NULL != p_ctrl);
#endif
#ifdef AGT_CFG_PREREQUISITE_CHECKING_ENABLE
    AGT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Make sure period is valid, then set period */
    ssp_err_t err = agt_period_set(p_ctrl->channel, period, unit);
    AGT_ERROR_RETURN(SSP_SUCCESS == err, err);

    return SSP_SUCCESS;
} /* End of function R_AGT_PeriodSet */

/*******************************************************************************************************************//**
 * Setting duty cycle is not supported by this driver. Implements timer_api_t::periodSet.
 *
 * @retval SSP_ERR_UNSUPPORTED   Unsupported driver feature.
 **********************************************************************************************************************/
ssp_err_t R_AGT_DutyCycleSet (timer_ctrl_t * const p_ctrl, timer_size_t const duty_cycle, timer_pwm_unit_t const unit,
        uint8_t const pin)
{
    SSP_PARAMETER_NOT_USED(p_ctrl);
    SSP_PARAMETER_NOT_USED(duty_cycle);
    SSP_PARAMETER_NOT_USED(unit);
    SSP_PARAMETER_NOT_USED(pin);

    return SSP_ERR_UNSUPPORTED;
} /* End of function R_AGT_DutyCycleSet */

/*******************************************************************************************************************//**
 * Resets the counter value to the period that was originally set. Implements timer_api_t::reset.
 *
 * @retval SSP_SUCCESS           Counter value written successfully.
 * @retval SSP_ERR_ASSERTION     The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_AGT_Reset(timer_ctrl_t * const p_ctrl)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure handle is valid. */
    SSP_ASSERT(NULL != p_ctrl);
#endif
#ifdef AGT_CFG_PREREQUISITE_CHECKING_ENABLE
    AGT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Save running status for restart, if already running. */
    agt_start_status_t status = HW_AGT_CounterStartBitGet(p_ctrl->channel);

    ssp_err_t error;
    error = agt_counter_stop_wait(p_ctrl->channel);
    AGT_ERROR_RETURN((SSP_SUCCESS == error), error);

    /** Reset the counter to the period originally sent in. */
    HW_AGT_CounterSet(p_ctrl->channel, g_timer_period[p_ctrl->channel]);

    /** Restart the AGT channel if it was running. */
    HW_AGT_CounterStartStop(p_ctrl->channel, status);

    return SSP_SUCCESS;
} /* End of function R_AGT_Reset */

/*******************************************************************************************************************//**
 * Starts timer. Implements timer_api_t::start.
 *
 * @retval SSP_SUCCESS           Timer successfully started.
 * @retval SSP_ERR_ASSERTION     The p_ctrl parameter is null.
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_AGT_Start(timer_ctrl_t * const p_ctrl)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure handle is valid. */
    SSP_ASSERT(NULL != p_ctrl);
#endif
#ifdef AGT_CFG_PREREQUISITE_CHECKING_ENABLE
    AGT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Start timer */
    HW_AGT_CounterStartStop(p_ctrl->channel, AGT_START);
    return SSP_SUCCESS;
} /* End of function R_AGT_Start */

/*******************************************************************************************************************//**
* @brief  Get timer information and store it in provided pointer p_info.
* Implements timer_api_t::infoGet.
*
* @retval SSP_SUCCESS           Period, status, count direction, frequency value written to caller's
*                               structure successfully.
* @retval SSP_ERR_ASSERTION     The p_ctrl or p_period_counts parameter was null.
* @retval SSP_ERR_NOT_OPEN      The channel is not opened.
* @retval SSP_ERR_INVALID_HW_CONDITION   Invalid hardware setting is detected.
**********************************************************************************************************************/
ssp_err_t R_AGT_InfoGet (timer_ctrl_t * const p_ctrl, timer_info_t * const p_info)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
   /** Make sure parameters are valid. */
   SSP_ASSERT(NULL != p_ctrl);
   SSP_ASSERT(NULL != p_info);
   AGT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

   /** Get and store period */
   p_info->period_counts = g_timer_period[p_ctrl->channel];

   /** Get and store clock frequency */
   p_info->clock_frequency = agt_clock_frequency_get(p_ctrl->channel);
   if (0U == p_info->clock_frequency)
   {
       return SSP_ERR_INVALID_HW_CONDITION;
   }

   /** AGT supports only counting down direction */
   p_info->count_direction = TIMER_DIRECTION_DOWN;

   bool status = HW_AGT_CounterStartBitGet(p_ctrl->channel);
   if (status)
   {
       p_info->status = TIMER_STATUS_COUNTING;
   }
   else
   {
       p_info->status = TIMER_STATUS_STOPPED;
   }

   return SSP_SUCCESS;
} /* End of function R_AGT_InfoGet */

/*******************************************************************************************************************//**
 * Stops the AGT channel specified by the handle (control block). This API implements timer_api_t::stop.
 * This API does not reset the channel or power it down.
 *
 * @retval SSP_SUCCESS           Timer successfully stopped.
 * @retval SSP_ERR_INVALID_PTR   The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_AGT_Stop(timer_ctrl_t * const p_ctrl)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure handle is valid. */
    SSP_ASSERT(NULL != p_ctrl);
#endif
#ifdef AGT_CFG_PREREQUISITE_CHECKING_ENABLE
    AGT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    ssp_err_t   error;
    error = agt_counter_stop_wait(p_ctrl->channel);
    return error;
} /* End of function R_AGT_Stop */

/*******************************************************************************************************************//**
 * Sets driver version based on compile time macros.
 *
 * @retval     SSP_SUCCESS          Successful close.
 * @retval     SSP_ERR_ASSERTION    The parameter p_version is NULL.
 **********************************************************************************************************************/
ssp_err_t R_AGT_VersionGet(ssp_version_t * const p_version)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    /** Verify parameters are valid */
    SSP_ASSERT(NULL != p_version);
#endif

    p_version->version_id = s_agt_version.version_id;

    return SSP_SUCCESS;
} /* End of function R_AGT_VersionGet */


 /** @} (end addtogroup AGT) */


/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*********************************************************************************************************************
 * agt_period_set:  Converts user input period and unit values to PCLK counts and with appropriate
 *                  divisor settings, then sets compare match value and divisor.
 *
 * @param[in]  ch      Channel to set
 * @param[in]  period  User defined period
 * @param[in]  unit    Unit of user defined period
 *
 * @retval     SSP_SUCCESS          Delay and divisor were set successfully.
 * @retval     SSP_ERR_INVALID_ARG  One of the following is invalid:
 *                                    - p_period->unit: must be one of the options from timer_size_t::unit
 *                                    - p_period->value: must result in a period in the following range:
 *                                        - Lower bound: (1 / (PCLK frequency))
 *                                        - Upper bound: (0xFFFFFFFF * 1024 / (PCLK frequency))
 **********************************************************************************************************************/
static ssp_err_t agt_period_set(uint8_t const ch, timer_size_t const period, timer_unit_t const unit)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    AGT_ERROR_RETURN((AGT_MAX_CH > ch), SSP_ERR_INVALID_ARGUMENT);
    AGT_ERROR_RETURN((0U != period), SSP_ERR_INVALID_ARGUMENT);
#endif

    ssp_err_t               error;
    uint32_t                pclk_freq_hz;
    uint64_t                temp_period;
    agt_clock_division_t    temp_div = AGT_CKS_DIV_1;


    /* Registers values casted to an enumerated list for readability. */
    agt_count_source_t count_source = (agt_count_source_t) HW_AGT_CountSourceGet(ch);

    /** If Clock source is PCLKB or derived from PCLKB */
    if ((AGT_CLOCK_PCLKB == count_source) || (AGT_CLOCK_PCLKB_DIV_8 == count_source) ||
        (AGT_CLOCK_PCLKB_DIV_2 == count_source))
    {
        /** Call CGC function to obtain current PCLKB clock frequency. */
        error = g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKB, &pclk_freq_hz);
        AGT_ERROR_RETURN((SSP_SUCCESS == error), error);
    }
    else
    {
        /** Else either fSUB clock or LOCO clock is used.  The frequency is set to 32Khz  (32768) */
        pclk_freq_hz = (uint32_t)FSUB_FREQUENCY_HZ;
    }

    /** Initialize for cases with no failure path. */
    error = SSP_SUCCESS;

    /** Convert period to input clock counts so it can be set in hardware. */
    switch (unit)
    {
        case TIMER_UNIT_PERIOD_RAW_COUNTS:
            temp_period = (uint64_t)period;
            break;
        case TIMER_UNIT_FREQUENCY_KHZ:
            error = agt_khz_to_counts(period, pclk_freq_hz, &temp_period);
            break;
        case TIMER_UNIT_FREQUENCY_HZ:
            error = agt_hz_to_counts(period, pclk_freq_hz, &temp_period);
            break;
        case TIMER_UNIT_PERIOD_NSEC:
            error = agt_ns_to_counts(period, pclk_freq_hz, &temp_period);
            break;
        case TIMER_UNIT_PERIOD_USEC:
            error = agt_us_to_counts(period, pclk_freq_hz, &temp_period);
            break;
        case TIMER_UNIT_PERIOD_MSEC:
            error = agt_ms_to_counts(period, pclk_freq_hz, &temp_period);
            break;
        case TIMER_UNIT_PERIOD_SEC:
            error = agt_s_to_counts(period, pclk_freq_hz, &temp_period);
            break;
        default:
            AGT_ERROR_RETURN(0, SSP_ERR_INVALID_ARGUMENT);
            break;
    }
    AGT_ERROR_RETURN((SSP_SUCCESS == error), error);

    /** Value in temp_period now PCLK counts. Could potentially overflow 16 bits.  If so, convert it, set divisor. */

    /** Set divisor according to the selected count source */
    if ((AGT_CLOCK_PCLKB == count_source) || (AGT_CLOCK_PCLKB_DIV_8 == count_source) ||
        (AGT_CLOCK_PCLKB_DIV_2 == count_source))
    {
        /* PCLKB is used, select available divisor.  It can be one of the three: Div 1, Div 2 and Div 8 */

        /* Does temp_period exceed AGT_MAX_CLOCK_COUNTS? */
        if (temp_period > AGT_MAX_CLOCK_COUNTS)
        {
            /** temp_period exceeds 16 bits, select divisor 2 (and cut temp_period into half). */
            count_source = AGT_CLOCK_PCLKB_DIV_2;
            temp_period /= 2U;
        }

        /* If temp_period still exceeds AGT_MAX_CLOCK_COUNTS, divide temp_period further by 4, effectively divide the
         * original
         * value by 8.  Select DIV_8. */
        if (AGT_MAX_CLOCK_COUNTS < temp_period)
        {
            /* temp_period still exceeds 16 bits, select divisor 8 */
            count_source = AGT_CLOCK_PCLKB_DIV_8;
            temp_period /= 4U;
        }
    }
    else
    {
        /** Sub clock is used, select appropriate sub clock divisor */
        while ((AGT_MAX_CLOCK_COUNTS < temp_period) && (AGT_CKS_DIV_128 != temp_div))
        {
            switch (temp_div)
            {
            case AGT_CKS_DIV_1:
                temp_period /= 2UL;
                temp_div = AGT_CKS_DIV_2;
                break;
            case AGT_CKS_DIV_2:
                temp_period /= 2UL;
                temp_div = AGT_CKS_DIV_4;
                break;
            case AGT_CKS_DIV_4:
                temp_period /= 2UL;
                temp_div = AGT_CKS_DIV_8;
                break;
            case AGT_CKS_DIV_8:
                temp_period /= 2UL;
                temp_div = AGT_CKS_DIV_16;
                break;
            case AGT_CKS_DIV_16:
                temp_period /= 2UL;
                temp_div = AGT_CKS_DIV_32;
                break;
            case AGT_CKS_DIV_32:
                temp_period /= 2UL;
                temp_div = AGT_CKS_DIV_64;
                break;
            case AGT_CKS_DIV_64:
                temp_period /= 2UL;
                temp_div = AGT_CKS_DIV_128;
                break;
            default:
                AGT_ERROR_RETURN(0, SSP_ERR_ASSERTION);
                break;
            }
        }
    }

    /** Validate period parameter.  If period is valid, set it.  Otherwise return error */
    AGT_ERROR_RETURN((temp_period <= AGT_MAX_CLOCK_COUNTS), SSP_ERR_INVALID_ARGUMENT);

    /** Store current status, then stop timer before setting divisor register */
    agt_start_status_t status = HW_AGT_CounterStartBitGet(ch);

    error = agt_counter_stop_wait(ch);
    AGT_ERROR_RETURN((SSP_SUCCESS == error), error);

    HW_AGT_CountSourceSet(ch, count_source);
    HW_AGT_ClockDivideSet(ch, temp_div);

    /** Store the counter value so it can be retrieved later. */
    g_timer_period[ch] = (timer_size_t)temp_period;

    /** Set counter to period minus one. */
    temp_period -= 1U;
    HW_AGT_CounterSet(ch, (uint32_t)temp_period);

    /** Reset counter in case new cycle is less than current count value, then restore state (counting or stopped). */
    HW_AGT_CounterStartStop(ch, status);

    return SSP_SUCCESS;
} /* End of function agt_period_set. */

/*******************************************************************************************************************//**
 * @brief  A local function to obtain the clock frequency of AGT
 * @param[in]  channel      Channel to get the frequency
 * @retval Clock frequency in Hz.
 **********************************************************************************************************************/
static uint32_t agt_clock_frequency_get(uint8_t const channel)
{
    uint32_t clock_frequency  = 0;
    uint32_t divisor_value ;

    agt_count_source_t count_source = (agt_count_source_t)HW_AGT_CountSourceGet(channel);
    agt_clock_division_t divisor    = HW_AGT_ClockDivideGet(channel);

    switch (divisor)
    {
    case AGT_CKS_DIV_1:
        divisor_value = 1;
        break;
    case AGT_CKS_DIV_2:
        divisor_value = 2;
        break;
    case AGT_CKS_DIV_4:
        divisor_value = 4;
        break;
    case AGT_CKS_DIV_8:
        divisor_value = 8;
        break;
    case AGT_CKS_DIV_16:
        divisor_value = 16;
        break;
    case AGT_CKS_DIV_32:
        divisor_value = 32;
        break;
    case AGT_CKS_DIV_64:
        divisor_value = 64;
        break;
    case AGT_CKS_DIV_128:
        divisor_value = 128;
        break;
    default:
        divisor_value = 1;
        break;
    }

    switch (count_source)
    {
    case AGT_CLOCK_PCLKB:
    case AGT_CLOCK_PCLKB_DIV_8:
    case AGT_CLOCK_PCLKB_DIV_2:
        g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKB, &clock_frequency);
        if (AGT_CLOCK_PCLKB_DIV_8 == count_source)
        {
            clock_frequency /= 8;
        }
        else if (AGT_CLOCK_PCLKB_DIV_2 == count_source)
        {
            clock_frequency /= 2;
        }
        break;
    case AGT_CLOCK_LOCO:
    case AGT_CLOCK_FSUB:
        clock_frequency = 32768UL / divisor_value;
        break;
    case AGT_CLOCK_AGT0_UNDERFLOW:
        if (0 == channel)
        {
            return 0;  ///< This is the illegal case should not be happen when this function is called.
        }
        else
        {
            /** Recursive call to get the frequency of AGT0. */
            clock_frequency = agt_clock_frequency_get(0);

            /** Get a period of AGT channel 0 and calculate the frequency */
            clock_frequency /= g_timer_period[0];
        }
        break;
    default:
        break;
    }

    return clock_frequency;
} /* End of function agt_clock_frequency_get */

/*********************************************************************************************************************
 * AGT helper function: Convert timer underflow event period in nanoseconds to counts.
 *
 * @param[in]   period          Timer underflow event period in nanoseconds.
 * @param[in]   clk_freq_hz     AGT input clock frequency in Hz.
 * @param[in]   p_counts        Pointer to caller's timer count value.
 * @param[out]  *p_counts       Timer count value.
 *
 * @retval      SSP_SUCCESS             No errors detected.
 * @retval      SSP_ERR_INVALID_POINTER Caller's pointer not valid.
 **********************************************************************************************************************/
ssp_err_t agt_ns_to_counts(timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_counts);
#endif

    uint64_t    counts;

    /** Counts = period (ns) * frequency (Hz) / (ns per second)*/
    counts = (uint64_t) period;
    counts *= (uint64_t) clk_freq_hz;
    counts /= (uint64_t) AGT_NANOSECONDS_PER_SECOND;
    *p_counts = counts;
    return SSP_SUCCESS;
}

/*********************************************************************************************************************
 * AGT helper function: Convert timer underflow event period in microseconds to counts.
 *
 * @param[in]   period          Timer underflow event period in microseconds.
 * @param[in]   clk_freq_hz     AGT input clock frequency in Hz.
 * @param[in]   p_counts        Pointer to caller's timer count value.
 * @param[out]  *p_counts       Timer count value.
 *
 * @retval      SSP_SUCCESS             No errors detected.
 * @retval      SSP_ERR_INVALID_POINTER Caller's pointer not valid.
 **********************************************************************************************************************/
ssp_err_t agt_us_to_counts(timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_counts);
#endif

    uint64_t    counts;

    /** Counts = period (us) * frequency (Hz) / (us per second)*/
    counts = (uint64_t) period;
    counts *= (uint64_t) clk_freq_hz;
    counts /= (uint64_t) AGT_MICROSECONDS_PER_SECOND;
    *p_counts = counts;
    return SSP_SUCCESS;
}

/*********************************************************************************************************************
 * AGT helper function: Convert timer underflow event period in milliseconds to counts.
 *
 * @param[in]   period          Timer underflow event period in milliseconds.
 * @param[in]   clk_freq_hz     AGT input clock frequency in Hz.
 * @param[in]   p_counts        Pointer to caller's timer count value.
 * @param[out]  *p_counts       Timer count value.
 *
 * @retval      SSP_SUCCESS             No errors detected.
 * @retval      SSP_ERR_INVALID_POINTER Caller's pointer not valid.
 **********************************************************************************************************************/
ssp_err_t agt_ms_to_counts(timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_counts);
#endif

    uint64_t    counts;

    /** Counts = period (ms) * frequency (Hz) / (ms per second)*/
    counts = (uint64_t) period;
    counts *= (uint64_t) clk_freq_hz;
    counts /= (uint64_t) AGT_MILLISECONDS_PER_SECOND;
    *p_counts = counts;
    return SSP_SUCCESS;
}

/*********************************************************************************************************************
 * AGT helper function: Convert timer underflow event period in seconds to counts.
 *
 * @param[in]   period          Timer underflow event period in seconds.
 * @param[in]   clk_freq_hz     AGT input clock frequency in Hz.
 * @param[in]   p_counts        Pointer to caller's timer count value.
 * @param[out]  *p_counts       Timer count value.
 *
 * @retval      SSP_SUCCESS             No errors detected.
 * @retval      SSP_ERR_INVALID_POINTER Caller's pointer not valid.
 **********************************************************************************************************************/
ssp_err_t agt_s_to_counts(timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_counts);
#endif

    uint64_t    counts;

    /** ((x) * ((freq_hz) * 1ULL)) */
    counts = (uint64_t)period;
    counts *= (uint64_t)clk_freq_hz;
    *p_counts = counts;
    return SSP_SUCCESS;
}

/*********************************************************************************************************************
 * AGT helper function: Convert timer underflow event rate in Hz to counts.
 *
 * @param[in]   period          Timer underflow event rate in Hz.
 * @param[in]   clk_freq_hz     AGT input clock frequency in Hz.
 * @param[in]   p_counts        Pointer to caller's timer count value.
 * @param[out]  *p_counts       Timer count value.
 *
 * @retval      SSP_SUCCESS             No errors detected.
 * @retval      SSP_ERR_INVALID_POINTER Caller's pointer not valid.
 **********************************************************************************************************************/
ssp_err_t agt_hz_to_counts(timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_counts);
#endif

    uint64_t    counts;

    /** (((freq_hz) * 1ULL) / (x)) */
    counts = (uint64_t)clk_freq_hz;
    counts /= (uint64_t)period;
    *p_counts = counts;
    return SSP_SUCCESS;
}

/*********************************************************************************************************************
 * AGT helper function: Convert timer underflow event rate in kHz to counts.
 *
 * @param[in]   period          Timer underflow event rate in kHz.
 * @param[in]   clk_freq_hz     AGT input clock frequency in Hz.
 * @param[in]   p_counts        Pointer to caller's timer count value.
 * @param[out]  *p_counts       Timer count value.
 *
 * @retval      SSP_SUCCESS             No errors detected.
 * @retval      SSP_ERR_INVALID_POINTER Caller's pointer not valid.
 **********************************************************************************************************************/
ssp_err_t agt_khz_to_counts(timer_size_t const period, uint32_t clk_freq_hz, uint64_t *p_counts)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_counts);
#endif

    uint64_t    counts;

    /** (((freq_hz) * 1ULL) / (1000 * (x))) */
    counts = (uint64_t)clk_freq_hz;
    counts /= (uint64_t)period;
    counts /= (uint64_t)AGT_KHZ_TO_HZ;
    *p_counts = counts;
    return SSP_SUCCESS;
}

/*********************************************************************************************************************
 * AGT internal function: Wait for counter to stop.
 *
 * @param[in]   channel   Channel ID that has IRQ pending
 *
 * @retval      SSP_ERR_INVALID_CHANNEL Input parameter out of range.
 * @retval      SSP_SUCCESS             Counter stopped before timeout.
 * @retval      SSP_ERR_TIMEOUT         Counter did not stop before timeout.
 **********************************************************************************************************************/
static ssp_err_t agt_counter_stop_wait(uint8_t channel)
{
#if AGT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(AGT_MAX_CH > channel);
#endif

    /** Stop timer */
    HW_AGT_CounterStartStop(channel, AGT_STOP);

    uint32_t    timeout = AGT_32BIT_MAX_VALUE;
    uint8_t     count_status;

    do
    {
        count_status = HW_AGT_CountStatusGet(channel);
        timeout -= 1;
    }
    while ((1U == count_status) && (0U < timeout));

    if ((0U == count_status) && (0U < timeout))
    {
        return SSP_SUCCESS;
    }
    else
    {
        return SSP_ERR_TIMEOUT;
    }
}


#if ((BSP_IRQ_CFG_AGT0_COMPARE_A != BSP_IRQ_DISABLED) || (BSP_IRQ_CFG_AGT1_COMPARE_A != BSP_IRQ_DISABLED) || \
    (BSP_IRQ_CFG_AGT0_COMPARE_B != BSP_IRQ_DISABLED) || (BSP_IRQ_CFG_AGT1_COMPARE_B != BSP_IRQ_DISABLED) ||  \
    (BSP_IRQ_CFG_AGT0_INT != BSP_IRQ_DISABLED) || (BSP_IRQ_CFG_AGT1_INT != BSP_IRQ_DISABLED))

/*********************************************************************************************************************
 * AGT internal function:  Handle AGT-related interrupts.  SF_CONTEXT_SAVE and RESTORE is handled by
 *             the caller.
 *
 * @param[in]  ch        Channel ID that has IRQ pending
 * @param[in]  irq_type  Type of IRQ
 *
 * @retval     None
 **********************************************************************************************************************/
static void agt_isr_handle_common (uint32_t ch, IRQn_Type irq_type)
{
    timer_callback_args_t cb_data;
    timer_ctrl_t          * p_ctrl;

    /** Clear pending IRQ to make sure it doesn't fire again after exiting */
    HW_ICU_InterruptClear(irq_type);

    /* Do nothing if pointer not valid. */
    if (NULL == gp_ctrls[ch])
    {
        return;
    }
    else
    {
        /** Pick up the AGT channel handle by Channel ID */
        p_ctrl = gp_ctrls[ch];
    }
    /** If the channel is configured to be one-shot mode, stop the timer. */
    uint32_t ch_mask;
    ch_mask = 1U << ch;
    if (ch_mask == (g_agt_one_shot_flag & ch_mask))
    {
        HW_AGT_CounterStartStop((uint8_t)ch, AGT_STOP);
        HW_AGT_CounterSet((uint8_t)ch, 0);
    }    

    /** Invoke the callback function if it is set. */
    if ((NULL != p_ctrl) && (NULL != p_ctrl->p_callback))
    {
        /** Setup parameters for the user-supplied callback function. */
        cb_data.p_context = p_ctrl->p_context;
        cb_data.event     = TIMER_EVENT_EXPIRED;
        p_ctrl->p_callback(&cb_data);
    }
}
#endif /* if ((BSP_IRQ_CFG_AGT0_COMPARE_A != BSP_IRQ_DISABLED) || (BSP_IRQ_CFG_AGT1_COMPARE_A != BSP_IRQ_DISABLED) ||
        *(BSP_IRQ_CFG_AGT0_COMPARE_B != BSP_IRQ_DISABLED) || (BSP_IRQ_CFG_AGT1_COMPARE_B != BSP_IRQ_DISABLED) ||
        *(BSP_IRQ_CFG_AGT0_INT != BSP_IRQ_DISABLED) || (BSP_IRQ_CFG_AGT1_INT != BSP_IRQ_DISABLED)) */

#if (BSP_IRQ_CFG_AGT0_INT != BSP_IRQ_DISABLED)
/** @cond SKIP */
/*********************************************************************************************************************
 * AGT0 AGTI interrupt ISR routine.  If present, this ISR replaces the weak one in BSP start code.
 *             The IRQ handling is done in agt_isr_handle_common().
 *
 * @param[in]  None
 *
 * @retval     None
 **********************************************************************************************************************/
void agt0_int_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Call AGT common ISR routine to handle this interrupt. */
    agt_isr_handle_common (0U, AGT0_INT_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_AGT0_INT != BSP_IRQ_DISABLED) */

#if (BSP_IRQ_CFG_AGT0_COMPARE_A != BSP_IRQ_DISABLED)

/*********************************************************************************************************************
 * AGT1 AGTCMAI interrupt ISR routine.  If present, this ISR replaces the weak one in BSP start code.
 *             The IRQ handling is done in agt_isr_handle_common().
 *
 * @param[in]  None
 *
 * @retval     None
 **********************************************************************************************************************/
void agt0_compare_a_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Call AGT common ISR routine to handle this interrupt. */
    agt_isr_handle_common (0U, AGT0_COMPARE_A_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_AGT0_COMPARE_A != BSP_IRQ_DISABLED) */

#if (BSP_IRQ_CFG_AGT0_COMPARE_B != BSP_IRQ_DISABLED)

/*********************************************************************************************************************
 * AGT0 AGTCMBI interrupt ISR routine.  If present, this ISR replaces the weak one in BSP start code.
 *             The IRQ handling is done in agt_isr_handle_common().
 *
 * @param[in]  None
 *
 * @retval     None
 **********************************************************************************************************************/
void agt0_compare_b_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Call AGT common ISR routine to handle this interrupt. */
    agt_isr_handle_common (0U, AGT0_COMPARE_B_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_AGT0_COMPARE_B != BSP_IRQ_DISABLED) */

#if (BSP_IRQ_CFG_AGT1_INT != BSP_IRQ_DISABLED)

/*********************************************************************************************************************
 * AGT1 AGTI interrupt ISR routine.  If present, this ISR replaces the weak one in BSP start code.
 *             The IRQ handling is done in agt_isr_handle_common().
 *
 * @param[in]  None
 *
 * @retval     None
 **********************************************************************************************************************/
void agt1_int_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Call AGT common ISR routine to handle this interrupt. */
    agt_isr_handle_common (1U, AGT1_INT_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_AGT1_INT != BSP_IRQ_DISABLED) */

#if (BSP_IRQ_CFG_AGT1_COMPARE_A != BSP_IRQ_DISABLED)

/*********************************************************************************************************************
 * AGT1 AGTCMAI interrupt ISR routine.  If present, this ISR replaces the weak one in BSP start code.
 *             The IRQ handling is done in agt_isr_handle_common().
 *
 * @param[in]  None
 *
 * @retval     None
 **********************************************************************************************************************/
void agt1_compare_a_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Call AGT common ISR routine to handle this interrupt. */
    agt_isr_handle_common (1U, AGT1_COMPARE_A_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_AGT1_COMPARE_A != BSP_IRQ_DISABLED) */

#if (BSP_IRQ_CFG_AGT1_COMPARE_B != BSP_IRQ_DISABLED)

/*********************************************************************************************************************
 * AGT1 AGTCMBI interrupt ISR routine.  If present, this ISR replaces the weak one in BSP start code.
 *             The IRQ handling is done in agt_isr_handle_common().
 *
 * @param[in]  None
 *
 * @retval     None
 **********************************************************************************************************************/
void agt1_compare_b_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Call AGT common ISR routine to handle this interrupt. */
    agt_isr_handle_common (1U, AGT1_COMPARE_B_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_AGT1_COMPARE_B != BSP_IRQ_DISABLED) */


