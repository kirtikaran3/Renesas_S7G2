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
/***********************************************************************************************************************
* File Name    : r_adc.c
* Description  : Primary source code for 12-bit A/D Converter driver.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#include "bsp_api.h"
#include "r_adc_api.h"
/* Configuration for this package. */
#include "r_adc_cfg.h"
/* Private header file for this package. */
#include "r_adc_private.h"
#include "r_adc_private_api.h"
#include "r_adc.h"
#include "./hw/hw_adc_private.h"

#if (BSP_MCU_GROUP_S3A7 == 1)
#include "./hw/target/s3a7/hw_adc_private_s3a7.h"
#elif (BSP_MCU_GROUP_S7G2 == 1)
#include "./hw/target/s7g2/hw_adc_private_s7g2.h"
#endif
/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Macro for error logger. */
#ifndef ADC_ERROR_RETURN
#define ADC_ERROR_RETURN(a, err)  SSP_ERROR_RETURN((a), (err), s_module_name, &s_adc_version)
#endif
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef volatile uint16_t * const  dregs_t;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
static ssp_err_t r_adc_open_cfg_check(uint16_t unit, adc_mode_t const mode, adc_cfg_t const * const p_cfg) ;
static ssp_err_t r_adc_sample_state_cfg_check(adc_ctrl_t * p_ctrl, adc_sample_state_t * p_sample);
#endif /* (1 == ADC_CFG_PARAM_CHECKING_ENABLE) */
/** Version data structure used by error logger macro. */
static const ssp_version_t s_adc_version =
{
        .api_version_minor  = ADC_API_VERSION_MINOR,
        .api_version_major  = ADC_API_VERSION_MAJOR,
        .code_version_major = ADC_CODE_VERSION_MAJOR,
        .code_version_minor = ADC_CODE_VERSION_MINOR
};
/** Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char s_module_name[] = "adc";
#endif
/***********************************************************************************************************************
 Global Variables
 **********************************************************************************************************************/
/** In RAM  */
/** Internal Control Block used for each unit */
adc_ctrlx_t g_dcb[2] =
{
        {
                .mode = ADC_MODE_SINGLE_SCAN,
                .callback = NULL,
                .trigger = ADC_TRIGGER_SOFTWARE,
                .opened = false,
                .p_context = NULL,
                .scan_mask = 0
        },
        {
                .mode = ADC_MODE_SINGLE_SCAN,
                .callback = NULL,
                .trigger = ADC_TRIGGER_SOFTWARE,
                .opened = false,
                .p_context = NULL,
                .scan_mask = 0
        }
};


/** ADC Implementation of ADC*/
const adc_api_t g_adc_on_adc =
{
        .open                   = R_ADC_Open,
        .scanCfg                = R_ADC_ScanConfigure,
        .infoGet                = R_ADC_InfoGet,
        .scanStart              = R_ADC_ScanStart,
        .scanStop               = R_ADC_ScanStop,
        .scanStatusGet          = R_ADC_CheckScanDone,
        .sampleStateCountSet    = R_ADC_SetSampleStateCount,
        .read                   = R_ADC_Read,
        .close                  = R_ADC_Close,
        .versionGet             = R_ADC_VersionGet
};

/*******************************************************************************************************************//**
 * @addtogroup ADC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief
 * The Open function applies power to the A/D peripheral, sets the operational mode, trigger sources, interrupt
 * priority, and configurations for the peripheral as a whole. If interrupt priority is non-zero in BSP_IRQ_Cfg.h,
 * the function takes a callback function pointer for notifying the user at interrupt level whenever a scan has completed.
 *
 * @retval  SSP_SUCCESS                Call successful.
 * @retval  SSP_ERR_ASSERTION          The parameter p_ctrl or p_cfg is NULL.
 * @retval  SSP_ERR_IN_USE             Peripheral is still running in another mode; perform R_ADC_Close() first.
 * @retval  SSP_ERR_INVALID_ARGUMENT   Mode or element of p_cfg structure has invalid value or is illegal based on mode.
 * @retval  SSP_ERR_IN_USE             Unit has already been initialized.
***********************************************************************************************************************/

ssp_err_t R_ADC_Open(adc_ctrl_t * p_ctrl,  adc_cfg_t const * const p_cfg)
{
    ssp_err_t err = SSP_SUCCESS;

    /**  Perform parameter checking */
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    /** Verify the pointers are valid */
    SSP_ASSERT (NULL != p_ctrl) ;
    SSP_ASSERT (NULL != p_cfg) ;

    /** Verify the configuration parameters are valid   */
    err = r_adc_open_cfg_check(p_cfg->unit, p_cfg->mode, p_cfg);
    if (SSP_SUCCESS != err)
    {
        return err;
    }
#endif

    /** Verify this unit has not already been initialized   */
    ADC_ERROR_RETURN(false == g_dcb[p_cfg->unit].opened, SSP_ERR_IN_USE);

    if (SSP_ERR_IN_USE == R_BSP_HardwareLock(ADC_LOCK(p_cfg->unit)))
    {
        return SSP_ERR_IN_USE;
    }

    /** Clear any pending interrupt requests in the NVIC or the peripheral*/
    HW_ADC_Close(p_cfg->unit);
    /** Initialize the hardware based on the configuration*/
    HW_ADC_Open(p_cfg);
    /** Save callback function pointer  */
    g_dcb[p_cfg->unit].callback = p_cfg->p_callback;
    /** Store the Unit number into the control structure*/
    p_ctrl->unit = p_cfg->unit;
    /** Store the user context into the control structure*/
    g_dcb[p_cfg->unit].p_context = p_cfg->p_context;
    /** Store the mode into the control structure*/
    g_dcb[p_cfg->unit].mode = p_cfg->mode;
    /** Save the regular mode/Group A trigger in the internal control block*/
    g_dcb[p_cfg->unit].trigger = p_cfg->trigger;
    /** Save the context */
    g_dcb[p_cfg->unit].p_context = p_cfg->p_context;
    /** Mark driver as opened for this unit */
    g_dcb[p_cfg->unit].opened = true;

    /** Return the error code */
    return err;
}

/*******************************************************************************************************************//**
 * @brief  Set the sample state count for individual channels. This only needs to be set
 *                                    for special use cases. Normally, use the default values out of Reset.
 *
 * @retval  SSP_SUCCESS                Call successful.
 * @retval  SSP_ERR_ASSERTION          The parameter p_ctrl or p_sample is NULL.
 * @retval  SSP_ERR_NOT_OPEN           Unit is not open.
 * @retval  SSP_ERR_INVALID_ARGUMENT   Parameter has invalid value.
***********************************************************************************************************************/
ssp_err_t R_ADC_SetSampleStateCount(adc_ctrl_t * p_ctrl, adc_sample_state_t * p_sample)
{
    ssp_err_t err = SSP_SUCCESS;

    /** Perform parameter checking */
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT (NULL != p_ctrl) ;
    SSP_ASSERT (NULL != p_sample) ;
    if (ADC_UNIT_MAX <= p_ctrl->unit)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }

    /** Ensure ADC Unit is already open */
    if (false == g_dcb[p_ctrl->unit].opened)
    {
        return SSP_ERR_NOT_OPEN;
    }
    /** Verify arguments are legal */
    err = r_adc_sample_state_cfg_check(p_ctrl, p_sample);
    if (SSP_SUCCESS != err)
    {
        return err;
    }
#else
    SSP_PARAMETER_NOT_USED(p_ctrl);
#endif

    /** Set the sample state count for the specified register */
    *(GET_SST_ARR(p_ctrl->unit))[p_sample->reg_id] = p_sample->num_states;

    /** Return the error code */
    return err ;
}

/*******************************************************************************************************************//**
 * @brief  Configure the ADC scan parameters. Channel specific settings are set in this function.
 *
 * @retval  SSP_SUCCESS                Call successful.
 * @retval  SSP_ERR_ASSERTION          The parameter p_ctrl or p_ch_cfg is NULL.
 * @retval  SSP_ERR_NOT_OPEN           Unit is not open.
 * @retval  SSP_ERR_INVALID_ARGUMENT   Parameter has invalid value.
***********************************************************************************************************************/
ssp_err_t R_ADC_ScanConfigure(adc_ctrl_t * p_ctrl, adc_channel_cfg_t const * const p_channel_cfg)
{
    ssp_err_t err = SSP_SUCCESS;

    /**  Perform parameter checking */
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT (NULL != p_ctrl);
    SSP_ASSERT (NULL != p_channel_cfg) ;
    if (ADC_UNIT_MAX <= p_ctrl->unit)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }

    /** Ensure ADC Unit is already open  */
    if (false == g_dcb[p_ctrl->unit].opened)
    {
        return SSP_ERR_NOT_OPEN;
    }
#endif

    /** Configure the hardware based on the configuration */
    err = HW_ADC_ScanCfg(p_ctrl->unit, p_channel_cfg);

    /** Save the scan mask locally; this is required for the infoGet function*/
    g_dcb[p_ctrl->unit].scan_mask = p_channel_cfg->scan_mask;

    /** Return the error code */
    return err ;
}

/*******************************************************************************************************************//**
 * @brief  This function returns the address of the lowest number configured channel and the total number of bytes
 * to be read in order to read the results of the configured channels and return the ELC Event name.
 *
 * @retval  SSP_SUCCESS                Call successful.
 * @retval  SSP_ERR_ASSERTION          The parameter p_ctrl is NULL.
 * @retval  SSP_ERR_NOT_OPEN           Unit is not open.
 * @retval  SSP_ERR_INVALID_ARGUMENT   Parameter has invalid value.
 * @retval  SSP_ERR_IN_USE             Running scan is still in progress
 *
 * @note: Currently this function call does not support Group Mode operation.
***********************************************************************************************************************/
ssp_err_t R_ADC_InfoGet(adc_ctrl_t * p_ctrl, adc_info_t * p_adc_info)
{
    ssp_err_t err = SSP_SUCCESS;
    uint32_t adc_mask, adc_mask_result = 0;
    uint32_t adc_mask_count = 0;
    volatile uint16_t * end_address;
    dregs_t *arr;

     /** Perform parameter checking  */
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT (NULL != p_ctrl) ;
    if (ADC_UNIT_MAX <= p_ctrl->unit)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
    /** Ensure ADC Unit is already open  */
    if (false == g_dcb[p_ctrl->unit].opened)
    {
        return SSP_ERR_NOT_OPEN;
    }

    /** Return an error if mode is Group Mode since that is not
     * supported currently */
    if (ADC_MODE_GROUP_SCAN == g_dcb[p_ctrl->unit].mode)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
#endif
    /** Get a pointer to the base register for the current unit */
    arr = GET_DATA_ARR(p_ctrl->unit);
    /** Retrieve the scan mask of active channels from the control structure */
    adc_mask = g_dcb[p_ctrl->unit].scan_mask;
    /** Determine the lowest channel that is configured*/
    while (0 == adc_mask_result)
    {
        adc_mask_result = (uint32_t)(adc_mask & ((uint32_t)1 << adc_mask_count));
        adc_mask_count++;
    }
    p_adc_info->p_address = arr[adc_mask_count-1];

    /** Determine the highest channel that is configured*/
    /** Set the mask count so that we start with the highest bit of the 32 bit mask */
    adc_mask_count = 31;
    /** Initialize the mask result */
    adc_mask_result = 0;
    while (0 == adc_mask_result)
    {
        adc_mask_result = (uint32_t)(adc_mask & ((uint32_t)1 << adc_mask_count));
        adc_mask_count--;
    }
    end_address = arr[adc_mask_count+1];

    /** Determine the size of data that must be read to read all the channels between and including the
     * highest and lowest channels.*/
    p_adc_info->length = (uint32_t)((end_address - p_adc_info->p_address) + 1);

    /** Specify the peripheral name in the ELC list */
    if(ADC_UNIT_0 == p_ctrl->unit)
    {
        p_adc_info->elc_peripheral = ELC_PERIPHERAL_ADC0;
        p_adc_info->elc_event = ELC_EVENT_ADC0_SCAN_END;
    }
    else
    {
        p_adc_info->elc_peripheral = ELC_PERIPHERAL_ADC1;
        p_adc_info->elc_event = ELC_EVENT_ADC1_SCAN_END;
    }

    return err;
}
/*******************************************************************************************************************//**
 * @brief  This function starts a software scan or enables the hardware trigger for a scan depending
 *                          on how the triggers were configured in the Open() call. If the Unit was configured for
 *                          hardware triggering, then this function  simply allows the trigger signal (hardware or
 *                          software) to get to the ADC Unit. The function is not able to control the generation of the
 *                          trigger itself. If the Unit was configured for software triggering, then this function
 *                          starts the software triggered scan.
 *
 * @retval  SSP_SUCCESS                Call successful.
 * @retval  SSP_ERR_ASSERTION          The parameter p_ctrl is NULL.
 * @retval  SSP_ERR_NOT_OPEN           Unit is not open.
 * @retval  SSP_ERR_INVALID_ARGUMENT   Parameter has invalid value.
 * @retval  SSP_ERR_IN_USE             Running scan is still in progress
***********************************************************************************************************************/
ssp_err_t R_ADC_ScanStart(adc_ctrl_t * p_ctrl)
{
    ssp_err_t err = SSP_SUCCESS;

    /** Perform parameter checking  */
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT (NULL != p_ctrl) ;
    if (ADC_UNIT_MAX <= p_ctrl->unit)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
    /** Ensure ADC Unit is already open  */
    if (false == g_dcb[p_ctrl->unit].opened)
    {
        return SSP_ERR_NOT_OPEN;
    }
#endif

    /** If the the normal/GroupA trigger is not set to software, then that the Unit is configured for hardware triggering */
    if (ADC_TRIGGER_SOFTWARE != g_dcb[p_ctrl->unit].trigger)
    {
        HW_ADC_ADCSR_TRGE_Set(p_ctrl->unit, HW_ADC_ADCSR_TRGE_SET);
    }
    /** Otherwise, enable software triggering */
    else
    {
        /** Check to see if there is an ongoing scan else start the scan */
        if (HW_ADC_ADCSR_ADCST_NOT_SET == HW_ADC_ADCSR_ADST_Get(p_ctrl->unit))
        {
            HW_ADC_ADCSR_ADST_Set(p_ctrl->unit, HW_ADC_ADCSR_ADCST_SET);
        }
        else
        {
            err = SSP_ERR_IN_USE;
        }
    }

    /** Return the error code */
    return err ;
}

/*******************************************************************************************************************//**
 * @brief  This function stops the software scan or disables the Unit from being triggered by the
 *                         hardware trigger (internal or external) based on what type of trigger the unit was configured
 *                         for in the Open() function. Stopping a hardware triggered scan via this function does not abort
 *                         an ongoing scan, but  prevents the next scan from occurring. Stopping a software triggered
 *                         scan aborts an ongoing scan.
 *
 * @retval  SSP_SUCCESS                Call successful.
 * @retval  SSP_ERR_ASSERTION          The parameter p_ctrl is NULL.
 * @retval  SSP_ERR_NOT_OPEN           Unit is not open.
 * @retval  SSP_ERR_INVALID_ARGUMENT   Parameter has invalid value.
 * @note    Stopping a software scan results in immediate stoppage of the scan irrespective of current state of
 *          of the scan. Stopping the hardware scan results in disabling the trigger to prevent future scans
 *          from starting but does not affect the current scan.
***********************************************************************************************************************/
ssp_err_t R_ADC_ScanStop(adc_ctrl_t * p_ctrl)
{

    /**  Perform parameter checking */
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT (NULL != p_ctrl) ;
    if (ADC_UNIT_MAX <= p_ctrl->unit)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }

    /** Ensure ADC Unit is already open  */
    if (false == g_dcb[ p_ctrl->unit].opened)
    {
        return SSP_ERR_NOT_OPEN;
    }
#endif

    /** If the trigger is not software scan, then disallow hardware triggering*/
    if (ADC_TRIGGER_SOFTWARE != g_dcb[p_ctrl->unit].trigger)
    {
        HW_ADC_ADCSR_TRGE_Set(p_ctrl->unit, HW_ADC_ADCSR_TRGE_NOT_SET);
    }
    /** Otherwise, disable software triggering*/
    else
    {
        HW_ADC_ADCSR_ADST_Set(p_ctrl->unit, HW_ADC_ADCSR_ADCST_NOT_SET);
    }

    /** Return the error code */
    return SSP_SUCCESS ;
}

/*******************************************************************************************************************//**
 * @brief  This function returns the status of any scan process that was started.
 *
 * @retval  SSP_SUCCESS                Successful; the scan is complete.
 * @retval  SSP_ERR_ASSERTION          The parameter p_ctrl is NULL.
 * @retval  SSP_ERR_NOT_OPEN           Unit is not open.
 * @retval  SSP_ERR_INVALID_ARGUMENT   Parameter has invalid value.
 * @retval  SSP_ERR_IN_USE             Running scan is still in progress.
 *
 * @note    If the peripheral was configured in single scan mode, then the return value of this function is an
 *          indication of the scan status. However, if the peripheral was configured in group mode, then the return
 *          value of this function could be an indication of either the group A or group B scan state. This is because
 *          the ADST bit is set when a scan is ongoing and cleared when the scan is done. This function should normally
 *          only be used when using software trigger in single scan mode.
***********************************************************************************************************************/
ssp_err_t R_ADC_CheckScanDone(adc_ctrl_t * p_ctrl)
{
    ssp_err_t err = SSP_SUCCESS;

    /**  Perform parameter checking */
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT (NULL != p_ctrl) ;
    if (ADC_UNIT_MAX <= p_ctrl->unit)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }

    /** Ensure ADC Unit is already open  */
    if (false == g_dcb[ p_ctrl->unit].opened)
    {
        return SSP_ERR_NOT_OPEN;
    }

#endif

    /** Read the status of the ADST bit*/
    if (HW_ADC_ADCSR_ADCST_SET == HW_ADC_ADCSR_ADST_Get(p_ctrl->unit))
    {
            err = SSP_ERR_IN_USE;
    }

    /** Return the error code */
    return err ;
}

/*******************************************************************************************************************//**
 * @brief   This function reads conversion results from a single channel or sensor
 *          register.
 * @retval  SSP_SUCCESS                Call successful.
 * @retval  SSP_ERR_ASSERTION          The parameter p_ctrl is NULL.
 * @retval  SSP_ERR_INVALID_POINTER    The parameter p_data is NULL.
 * @retval  SSP_ERR_NOT_OPEN           Unit is not open.
 * @retval  SSP_ERR_INVALID_ARGUMENT   Parameter has invalid value.
***********************************************************************************************************************/
ssp_err_t R_ADC_Read(adc_ctrl_t * p_ctrl, adc_register_t const  reg_id, adc_data_size_t * const p_data)
{
    dregs_t *arr;
    arr = GET_DATA_ARR(p_ctrl->unit);

    /** Perform parameter checking*/
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT (NULL != p_ctrl) ;
    if (ADC_UNIT_MAX <= p_ctrl->unit)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }

    /** Verify that the ADC is already open */
    if (false == g_dcb[p_ctrl->unit].opened)
    {
        return SSP_ERR_NOT_OPEN;
    }

    if (NULL == p_data)
    {
        return SSP_ERR_INVALID_POINTER;
    }

    if (NULL == arr[reg_id])
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
#else
    SSP_PARAMETER_NOT_USED(p_ctrl);
#endif

    /** Read the data from the requested ADC conversion register and return it */
    *p_data = (adc_data_size_t)*arr[reg_id];

    /** Return the error code */
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  This function ends any scan in progress, disables interrupts, and removes power to the A/D
 *                       peripheral.
 *
 * @retval  SSP_SUCCESS                Call successful.
 * @retval  SSP_ERR_ASSERTION          The parameter p_ctrl is NULL.
 * @retval  SSP_ERR_NOT_OPEN           Unit is not open.
 * @retval  SSP_ERR_INVALID_ARGUMENT   Parameter has invalid value.
***********************************************************************************************************************/
ssp_err_t R_ADC_Close(adc_ctrl_t * p_ctrl)
{
    /** Perform parameter checking*/
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT (NULL != p_ctrl) ;
    if (ADC_UNIT_MAX <= p_ctrl->unit)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }

    /** Verify that the ADC is already open */
    if (false == g_dcb[p_ctrl->unit].opened)
    {
        return SSP_ERR_NOT_OPEN;
    }
#endif

    /** Perform hardware stop for the specific unit*/
    HW_ADC_Close(p_ctrl->unit);

    /** Mark driver as closed   */
    g_dcb[p_ctrl->unit].opened = false;

    /** Release the lock*/
    R_BSP_HardwareUnlock(ADC_LOCK(p_ctrl->unit));

    /** Return the error code */
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief   Retrieve the API version number.
 *
 * @retval  SSP_SUCCESS        Successful return.
 * @retval  SSP_ERR_ASSERTION  The parameter p_version is NULL.
***********************************************************************************************************************/
ssp_err_t R_ADC_VersionGet(ssp_version_t * const p_version)
{
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
    /** Verify parameters are valid */
    SSP_ASSERT(NULL != p_version);
#endif
    /** Return the version number */
    p_version->version_id =  s_adc_version.version_id;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup ADC)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   r_adc_sample_state_cfg_check
 *
 * This function checks the Sample and Hold arguments
 *
 * @param[in]  p_ctrl          :  ADC control structure.
 * @param[in]  p_sample        :  Sample State Configuration
***********************************************************************************************************************/
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
ssp_err_t r_adc_sample_state_cfg_check(adc_ctrl_t * p_ctrl, adc_sample_state_t * p_sample)
{
    /** Used to prevent compiler warning */
      SSP_PARAMETER_NOT_USED(p_ctrl);
    /** If the passed reg_id is a NULL in the sreg0_ptrs or sreg1_ptrs arrays ,
     * then it does not exist on this part;
     * If the num_states value is beyond the allowed limit*/
    if ((NULL == (GET_SST_ARR(p_ctrl->unit))[p_sample->reg_id])
            || (ADC_SAMPLE_STATE_COUNT_MIN > p_sample->num_states))
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
    else
    {
        return SSP_SUCCESS;
    }
}
#endif /* (1 == ADC_CFG_PARAM_CHECKING_ENABLE) */
/*******************************************************************************************************************//**
 * @brief   r_adc_open_cfg_check : ADC check open function configuration
 *
 * This function validates the configuration arguments for illegal combinations or options.
 *
 * @param[in]  unit  :  ADC unit number
 * @param[in]  mode  :  Operational mode (see enumeration below)
 * @param[in]  p_cfg :  Pointer to configuration structure (see below)
 *
 * @retval  SSP_SUCCESS -       Successful
 * @retval  SSP_ERR_INVALID_ARGUMENT -  Mode or element of p_cfg structure has invalid value or is invalid based on mode
 * @retval  SSP_ERR_IRQ_BSP_DISABLED -  IRQ is disabled in the BSP when a callback function is passed
***********************************************************************************************************************/
#if (1 == ADC_CFG_PARAM_CHECKING_ENABLE)
ssp_err_t r_adc_open_cfg_check(uint16_t unit, adc_mode_t const  mode, adc_cfg_t const * const p_cfg)
{
    ssp_err_t err = SSP_SUCCESS;
    /** Check for valid argument values for options that are unique to the MCU */
    err = r_adc_open_cfg_check_unique(unit, mode, p_cfg);
    if (SSP_SUCCESS != err)
    {
        return err;
    }


    /** If a callback is passed, then it is necessary for the IRQ to be configured in the BSP.
     *  If the mode is continuous, then the callback has to be NULL */
    if ((p_cfg->p_callback != NULL))
    {
        if (p_cfg->mode == ADC_MODE_CONTINUOUS_SCAN)
        {
            return SSP_ERR_INVALID_ARGUMENT;
        }

        if (ADC_UNIT_0 == unit)
        {
#ifdef BSP_IRQ_CFG_ADC0_SCAN_END
            if(BSP_IRQ_CFG_ADC0_SCAN_END == BSP_IRQ_DISABLED)
            {
                err = SSP_ERR_IRQ_BSP_DISABLED;
            }
#endif
            if (SSP_SUCCESS == err)
            {
                /** If group mode is enabled and a callback is used, then make sure group mode interrupts
                 * are enabled */
                if (p_cfg->mode == ADC_MODE_GROUP_SCAN)
                {
#ifdef BSP_IRQ_CFG_ADC0_SCAN_END_B
#if (BSP_IRQ_CFG_ADC0_SCAN_END_B == BSP_IRQ_DISABLED)
                    err = SSP_ERR_IRQ_BSP_DISABLED;
#endif
#endif
                }
            }
        }
        else if (ADC_UNIT_1 == unit)
        {
#ifdef BSP_IRQ_CFG_ADC1_SCAN_END
            if(BSP_IRQ_CFG_ADC1_SCAN_END == BSP_IRQ_DISABLED)
            {
                err = SSP_ERR_IRQ_BSP_DISABLED;
            }
#endif
            if (SSP_SUCCESS == err)
            {
                /** If group mode is enabled and a callback is used, then make sure group mode interrupts
                 * are enabled */
                if (p_cfg->mode == ADC_MODE_GROUP_SCAN)
                {
#ifdef BSP_IRQ_CFG_ADC1_SCAN_END_B
#if (BSP_IRQ_CFG_ADC1_SCAN_END_B == BSP_IRQ_DISABLED)
                    err = SSP_ERR_IRQ_BSP_DISABLED;
#endif
#endif
                }
            }
        }
    }

    if (SSP_SUCCESS == err)
    {
        /** Group checking; only synchronous triggers (ELC) allowed; must be unique   */
        if ((ADC_MODE_GROUP_SCAN == mode))
        {
            if ((ADC_TRIGGER_SYNC_ELC  != p_cfg->trigger)
                    ||(ADC_TRIGGER_SYNC_ELC != p_cfg->trigger_group_b))
            {
                err = SSP_ERR_INVALID_ARGUMENT;
            }
        }
    }

    return err;
}
#endif

/*******************************************************************************************************************//**
 * @brief ADC Channel 0 ISR routine
 *
 * This function implements the unit 0 interrupt handler for normal/Group A/double trigger scan complete.
 *
***********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_ADC0_SCAN_END
#if (BSP_IRQ_CFG_ADC0_SCAN_END != BSP_IRQ_DISABLED)
void s12ad0_adi_isr(void);
void s12ad0_adi_isr(void)
{
    adc_callback_args_t   args;
    /** Save context if RTOS is used     */
    SF_CONTEXT_SAVE

     /** Clear the BSP IRQ Flag     */
    R_BSP_IrqStatusClear (ADC0_SCAN_END_IRQn) ;
    /** Store the event into the callback argument */
    args.event = ADC_EVENT_SCAN_COMPLETE;
    /** Store the Unit number into the callback argument */
    args.unit = ADC_UNIT_0;
    /** Populate the context field*/
    args.p_context = g_dcb[ADC_UNIT_0].p_context;
    /** If a callback was provided, call it with the argument */
    if (NULL != g_dcb[ADC_UNIT_0].callback)
    {
        g_dcb[ADC_UNIT_0].callback(&args);
    }

    SF_CONTEXT_RESTORE     /** Restore context if RTOS is used  */
}
#endif
#endif

/*******************************************************************************************************************//**
 * @brief ADC Channel 1 ISR routine
 *
 * This function implements the unit 1 interrupt handler for normal/Group A/double trigger scan complete.
 *
***********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_ADC1_SCAN_END
#if (BSP_IRQ_CFG_ADC1_SCAN_END != BSP_IRQ_DISABLED)
void s12ad1_adi_isr(void);
void s12ad1_adi_isr(void)
{
     adc_callback_args_t   args;
     SF_CONTEXT_SAVE        /** Save context if RTOS is used     */

     /** Clear the BSP IRQ Flag     */
     R_BSP_IrqStatusClear (ADC1_SCAN_END_IRQn) ;
     /** Store the event into the callback argument */
     args.event = ADC_EVENT_SCAN_COMPLETE;
     /** Store the Unit number into the callback argument */
     args.unit = ADC_UNIT_1;
     /** Populate the context field*/
     args.p_context = g_dcb[ADC_UNIT_1].p_context;
     /** If a callback was provided, call it with the argument */
     if (NULL != g_dcb[ADC_UNIT_1].callback)
     {
         g_dcb[ADC_UNIT_1].callback(&args);
     }

     SF_CONTEXT_RESTORE     /** Restore context if RTOS is used  */
}
#endif
#endif

/*******************************************************************************************************************//**
 * @brief   ADC Channel 0 Group ISR routine
 *
 * This function implements the unit 0 interrupt handler for Group B scan complete.
 *
***********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_ADC0_SCAN_END_B
#if (BSP_IRQ_CFG_ADC0_SCAN_END_B != BSP_IRQ_DISABLED)
void s12ad0_gbadi_isr(void);
void s12ad0_gbadi_isr(void)
{
    adc_callback_args_t   args;
    SF_CONTEXT_SAVE        /** Save context if RTOS is used     */

    /** Clear the BSP IRQ Flag     */
    R_BSP_IrqStatusClear (ADC0_SCAN_END_B_IRQn);
    /** Store the event into the callback argument */
    args.event = ADC_EVENT_SCAN_COMPLETE_GROUP_B;
    /** Store the Unit number into the callback argument */
    args.unit = ADC_UNIT_0;
    /** Populate the context field*/
    args.p_context = g_dcb[ADC_UNIT_0].p_context;
    /** If a callback was provided, call it with the argument */
    if (NULL != g_dcb[ADC_UNIT_0].callback)
    {
        g_dcb[ADC_UNIT_0].callback(&args);
    }

    SF_CONTEXT_RESTORE     /** Restore context if RTOS is used  */
}
#endif
#endif

/*******************************************************************************************************************//**
 * @brief   ADC Channel 1 Group ISR routine
 *
 * This function implements the unit 1 interrupt handler for Group B scan complete.
 *
***********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_ADC1_SCAN_END_B
#if (BSP_IRQ_CFG_ADC1_SCAN_END_B != BSP_IRQ_DISABLED)
void s12ad1_gbadi_isr(void);
void s12ad1_gbadi_isr(void)
{
    adc_callback_args_t   args;
    SF_CONTEXT_SAVE        /** Save context if RTOS is used     */

    /** Clear the BSP IRQ Flag     */
    R_BSP_IrqStatusClear (ADC1_SCAN_END_B_IRQn);
    /** Store the event into the callback argument */
    args.event = ADC_EVENT_SCAN_COMPLETE_GROUP_B;
    /** Store the Unit number into the callback argument */
    args.unit = ADC_UNIT_1;
    /** Populate the context field*/
    args.p_context = g_dcb[ADC_UNIT_1].p_context;
    /** If a callback was provided, call it with the argument */
    if (NULL != g_dcb[ADC_UNIT_1].callback)
    {
        g_dcb[ADC_UNIT_1].callback(&args);
    }

    SF_CONTEXT_RESTORE     /** Restore context if RTOS is used  */
}
#endif
#endif


