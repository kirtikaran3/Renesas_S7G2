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
 * File Name    : r_dac.c
 * Description  : DAC HLD implementation
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <string.h>
#include "r_dac.h"
#include "r_dac_private.h"
#include "r_dac_private_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Macro for error logger. */
#ifndef DAC_ERROR_RETURN
#define DAC_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), g_module_name, &g_dac_version)
#endif

/** DAC Open API channel number. Note: S124 MCUs only use channel 0. */
#define DAC_CHANNEL_0 ((uint8_t)0)
#define DAC_CHANNEL_1 ((uint8_t)1)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
const dac_api_t g_dac_on_dac =
{
    .open            = R_DAC_Open,
    .close           = R_DAC_Close,
    .write           = R_DAC_Write,
    .start           = R_DAC_Start,
    .stop            = R_DAC_Stop,
    .versionGet      = R_DAC_VersionGet
};

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
static const ssp_version_t g_dac_version =
{
    .api_version_minor  = DAC_CODE_VERSION_MINOR,
    .api_version_major  = DAC_CODE_VERSION_MAJOR,
    .code_version_major = DAC_CODE_VERSION_MAJOR,
    .code_version_minor = DAC_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/** Name of module used by error logger macro */
static const char g_module_name[] = "dac";

static uint8_t g_dac_instance_counter = 0;

/*******************************************************************************************************************//**
 * @addtogroup DAC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/******************************************************************************************************************//**
 * @brief Perform required initialization described in hardware manual.  Implements dac_api_t::open.
 * Configures a single DAC channel, starts the channel, and provides a handle for use with the
 * DAC API Write and Close functions.  Must be called once prior to calling any other DAC API
 * functions.  After a channel is opened, Open should not be called again for the same channel
 * without calling Close first.
 *
 * @retval SSP_SUCCESS           The channel was successfully opened.
 * @retval SSP_ERR_ASSERTION     One of the following parameters may be NULL: p_ctrl or p_cfg
 *                               Channel ID requested in p_cfg may not available on the device selected in
 *                               r_bsp_config.h
 *                               data_format value in p_cfg is out of range.
 *                               ad_da_synchronized value in p_cfg is out of range.
 * @retval  SSP_ERR_HW_LOCKED    DAC resource is locked.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 **********************************************************************************************************************/
ssp_err_t   R_DAC_Open (dac_ctrl_t * p_ctrl, dac_cfg_t const * const p_cfg)
{
    /** Validate the input parameter. */
#if DAC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT((p_cfg->channel == DAC_CHANNEL_0) || (p_cfg->channel == DAC_CHANNEL_1));
    SSP_ASSERT(0UL == (0xFFFFFFFEUL & (uint32_t)(p_cfg->ad_da_synchronized)));
    SSP_ASSERT((p_cfg->data_format == DAC_DATA_FORMAT_FLUSH_RIGHT) || (p_cfg->data_format == DAC_DATA_FORMAT_FLUSH_LEFT));
    SSP_ASSERT(0UL == (0xFFFFFFFEUL & (uint32_t)(p_cfg->output_amplifier_enabled)));
#endif /* if DAC_CFG_PARAM_CHECKING_ENABLE */

    /** Lock the DAC Hardware Resource. */
    if (DAC_CHANNEL_0 == p_cfg->channel)
    {
        DAC_ERROR_RETURN(SSP_SUCCESS == R_BSP_HardwareLock(BSP_HW_LOCK_DAC0), SSP_ERR_HW_LOCKED);
    }
    else
    {
        DAC_ERROR_RETURN(SSP_SUCCESS == R_BSP_HardwareLock(BSP_HW_LOCK_DAC1), SSP_ERR_HW_LOCKED);
    }

    if (0 == g_dac_instance_counter)
    {
        /** Power on the DAC device. */
        HW_DAC_PowerOn();
    }

    g_dac_instance_counter++;

    /** Initialize the control block */
    memset(p_ctrl, 0, sizeof(dac_ctrl_t));

    /** Stop the channel. */
    if (DAC_CHANNEL_0 == p_cfg->channel)
    {
        HW_DAC_Ch0Control(HW_DAC_CONTROL_DISABLE);
    }
    else
    {
        HW_DAC_Ch1Control(HW_DAC_CONTROL_DISABLE);
    }

    /** Configure data format: left or right justified. */
    HW_DAC_DADPRCfg(p_cfg->data_format);

    /** Configure D/A-A/D Synchronous Start Control Register(DAADSCR). */
    HW_DAC_DAADSCRCfg((uint8_t) p_cfg->ad_da_synchronized);

    /** Set output amplifier configuration for the channel. */
    HW_DAC_DAAMPCRCfg(p_cfg);

    /** Set the reference voltage. */
    HW_DAC_VrefSet(DAC_VREF_AVCC0_AVSS0);

    /** Initialize the channel state information. */
    p_ctrl->channel         = p_cfg->channel;
    p_ctrl->channel_started = false;
    p_ctrl->channel_opened  = true;

    /** All done.  Return. */
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief    Stop the D/A conversion, stop output, and close the DAC channel.
 * @retval   SSP_SUCCESS           The channel is successfully closed.
 * @retval   SSP_ERR_ASSERTION     p_ctrl is NULL.
 * @retval   SSP_ERR_NOT_OPEN      Channel associated with p_ctrl has not been opened.
 **********************************************************************************************************************/
ssp_err_t   R_DAC_Close (dac_ctrl_t * p_ctrl)
{
#if DAC_CFG_PARAM_CHECKING_ENABLE
    /** Validate the handle parameter */
    SSP_ASSERT(NULL != p_ctrl);
	SSP_ASSERT((p_ctrl->channel == DAC_CHANNEL_0) || (p_ctrl->channel == DAC_CHANNEL_1));
#endif
    /** Validate that the channel is opened. */
    DAC_ERROR_RETURN(p_ctrl->channel_opened, SSP_ERR_NOT_OPEN);

    /** Disable D/A conversion and stop the output if the channel is enabled. */
    if (p_ctrl->channel_started)
    {
        /** Stop the channel */
        if (DAC_CHANNEL_0 == p_ctrl->channel)
        {
            HW_DAC_Ch0Control(HW_DAC_CONTROL_DISABLE);
        }
        else
        {
            HW_DAC_Ch1Control(HW_DAC_CONTROL_DISABLE);
        }
    }

    /** Update the channel state information. */
    p_ctrl->channel_opened  = false;
    p_ctrl->channel_started = false;

    g_dac_instance_counter--;

    if(0 == g_dac_instance_counter)
    {
        /** Power off the DAC device. */
        HW_DAC_PowerOff();
    }

    /** Unlock the DAC Hardware Resource */
    if (DAC_CHANNEL_0 == p_ctrl->channel)
    {
        R_BSP_HardwareUnlock(BSP_HW_LOCK_DAC0);
    }
    else
    {
        R_BSP_HardwareUnlock(BSP_HW_LOCK_DAC1);
    }

    /* All done, return success. */
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief    Write data to the D/A converter and enable the output if it has not been enabled.
 *
 * @retval   SSP_SUCCESS           Data is successfully written to the D/A Converter.
 * @retval   SSP_ERR_ASSERTION     p_ctrl is NULL.
 * @retval   SSP_ERR_NOT_OPEN      Channel associated with p_ctrl has not been opened.
 *
 * @note     Write function automatically starts the D/A conversion after data is successfully written
 *           to the channel.
 **********************************************************************************************************************/
ssp_err_t   R_DAC_Write (dac_ctrl_t * p_ctrl, dac_size_t value)
{
#if DAC_CFG_PARAM_CHECKING_ENABLE
    /** Validate the handle parameter */
    SSP_ASSERT(NULL != p_ctrl);
	SSP_ASSERT((p_ctrl->channel == DAC_CHANNEL_0) || (p_ctrl->channel == DAC_CHANNEL_1));
#endif

    /** Validate that the channel is opened. */
    DAC_ERROR_RETURN(p_ctrl->channel_opened, SSP_ERR_NOT_OPEN);

    /** Write the value to D/A converter. */
    HW_DAC_DADRWrite(p_ctrl->channel, value);

    /** Start the converter if it has been idle. */
    if (!p_ctrl->channel_started)
    {
        /** Start the channel */
        if (0 == p_ctrl->channel)
        {
            HW_DAC_Ch0Control(HW_DAC_CONTROL_ENABLE);
        }
        else
        {
            HW_DAC_Ch1Control(HW_DAC_CONTROL_ENABLE);
        }

        p_ctrl->channel_started = true;
    }

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief    Start the D/A conversion output if it has not been started.
 *
 * @retval   SSP_SUCCESS           The channel is started successfully.
 * @retval   SSP_ERR_ASSERTION     p_ctrl is NULL.
 * @retval   SSP_ERR_NOT_OPEN      Channel associated with p_ctrl has not been opened.
 **********************************************************************************************************************/
ssp_err_t   R_DAC_Start (dac_ctrl_t * p_ctrl)
{
#if DAC_CFG_PARAM_CHECKING_ENABLE
    /** Validate the handle parameter */
    SSP_ASSERT(NULL != p_ctrl);
	SSP_ASSERT((p_ctrl->channel == DAC_CHANNEL_0) || (p_ctrl->channel == DAC_CHANNEL_1));
#endif

    /** Validate that the channel is opened. */
    DAC_ERROR_RETURN(p_ctrl->channel_opened, SSP_ERR_NOT_OPEN);

    /** If the channel output is already enabled, do nothing. */
    if (p_ctrl->channel_started)
    {
        return SSP_SUCCESS;
    }

    /** Enable the output. */
    if (DAC_CHANNEL_0 == p_ctrl->channel)
    {
        HW_DAC_Ch0Control(HW_DAC_CONTROL_ENABLE);
    }
    else
    {
        HW_DAC_Ch1Control(HW_DAC_CONTROL_ENABLE);
    }

    /** Update the internal state. */
    p_ctrl->channel_started = true;

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief  Stop the D/A conversion and disable the output signal.
 *
 * @retval  SSP_SUCCESS           The control is successfully stopped.
 * @retval  SSP_ERR_ASSERTION     p_ctrl is NULL.
 * @retval  SSP_ERR_NOT_OPEN      Channel associated with p_ctrl has not been opened.
 **********************************************************************************************************************/
ssp_err_t   R_DAC_Stop (dac_ctrl_t * p_ctrl)
{
#if DAC_CFG_PARAM_CHECKING_ENABLE
    /** Validate the handle parameter */
    SSP_ASSERT(NULL != p_ctrl);
	SSP_ASSERT((p_ctrl->channel == DAC_CHANNEL_0) || (p_ctrl->channel == DAC_CHANNEL_1));
#endif

    /** Validate that the channel is opened. */
    DAC_ERROR_RETURN(p_ctrl->channel_opened, SSP_ERR_NOT_OPEN);

    /** If the channel output is already disabled, do nothing. */
    if (!p_ctrl->channel_started)
    {
        return SSP_SUCCESS;
    }

    /** Disable the output. */
    if (DAC_CHANNEL_0 == p_ctrl->channel)
    {
        HW_DAC_Ch0Control(HW_DAC_CONTROL_DISABLE);
    }
    else
    {
        HW_DAC_Ch1Control(HW_DAC_CONTROL_DISABLE);
    }

    /** Mark the internal state. */
    p_ctrl->channel_started = false;

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief  Get version and store it in provided pointer p_version.
 * @retval     SSP_SUCCESS          Successfully retrieved version information.
 **********************************************************************************************************************/
ssp_err_t   R_DAC_VersionGet (ssp_version_t * p_version)
{
#if DAC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif

    p_version->code_version_major = g_dac_version.code_version_major;
    p_version->code_version_minor = g_dac_version.code_version_minor;
    p_version->api_version_major  = g_dac_version.api_version_major;
    p_version->api_version_minor  = g_dac_version.api_version_minor;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup DAC)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/


