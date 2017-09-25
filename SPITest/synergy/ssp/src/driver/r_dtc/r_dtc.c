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
 * File Name    : r_dtc.c
 * Description  : DTC implementation of the transfer interface.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <string.h>
#include "r_dtc.h"
#include "hw/hw_dtc_private.h"
#include "r_dtc_private_api.h"
#if DTC_CFG_SOFTWARE_START_ENABLE
#include "r_elc.h"
#endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Driver ID (DTC in ASCII). */
#define DTC_ID (0x44544300)

/** Macro for error logger. */
#ifndef DTC_ERROR_RETURN
#define DTC_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), "dtc", &g_dtc_version)
#endif

/** Size of vector table is based on number of vectors defined in BSP. */
#define DTC_VECTOR_TABLE_SIZE (BSP_MAX_NUM_IRQn)

/** Compiler specific macro to specify vector table section. */
#ifdef DTC_CFG_VECTOR_TABLE_SECTION_NAME
#ifdef __GNUC__
#define DTC_SECTION_ATTRIBUTE __attribute__((section DTC_CFG_VECTOR_TABLE_SECTION_NAME))
#else
#warning Check DTC section attribute.
#define DTC_SECTION_ATTRIBUTE
#endif
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#if DTC_CFG_SOFTWARE_START_ENABLE
/** Internal driver storage for p_callback, used by software start ISRs. */
typedef struct st_dtc_callback
{
    /** Callback for transfer end interrupt. Set to NULL for no CPU interrupt. */
    void (* p_callback)(transfer_callback_args_t * cb_data);

    /** Placeholder for user data.  Passed to the user p_callback in ::transfer_callback_args_t. */
    void const * p_context;
} dtc_callback_t;
#endif

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static IRQn_Type dtc_irq_lookup(elc_event_t const event);

/***********************************************************************************************************************
 * ISR function prototypes
 **********************************************************************************************************************/
void elc_software_event_0_isr(void);
void elc_software_event_1_isr(void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** Stores initialization state to skip initialization in ::R_DTC_Open after the first call. */
static bool g_dtc_state_initialized = false;

/** DTC vector table.  Allocated in RAM and aligned to 10 bits. TODO_DTC Make sure compiler doesn't waste space around
 *  this. */
#ifdef __GNUC__
#ifdef DTC_CFG_VECTOR_TABLE_SECTION_NAME
static transfer_info_t * gp_dtc_vector_table[DTC_VECTOR_TABLE_SIZE] __attribute__((aligned(1024))) DTC_SECTION_ATTRIBUTE;
#else
static transfer_info_t * gp_dtc_vector_table[DTC_VECTOR_TABLE_SIZE] __attribute__((aligned(1024)));
#endif
#elif __ICCARM__
#pragma data_alignment = 1024
#ifdef DTC_CFG_VECTOR_TABLE_SECTION_NAME
#pragma location = DTC_CFG_VECTOR_TABLE_SECTION_NAME
#endif
static transfer_info_t * gp_dtc_vector_table[DTC_VECTOR_TABLE_SIZE];
#endif

#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t g_dtc_version =
{
    .api_version_minor  = TRANSFER_API_VERSION_MINOR,
    .api_version_major  = TRANSFER_API_VERSION_MAJOR,
    .code_version_major = DTC_CODE_VERSION_MAJOR,
    .code_version_minor = DTC_CODE_VERSION_MINOR,
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** DTC implementation of transfer API. */
const transfer_api_t g_transfer_on_dtc =
{
    .open       = R_DTC_Open,
    .reset      = R_DTC_Reset,
    .infoGet    = R_DTC_InfoGet,
    .start      = R_DTC_Start,
    .stop       = R_DTC_Stop,
    .enable     = R_DTC_Enable,
    .disable    = R_DTC_Disable,
    .close      = R_DTC_Close,
    .versionGet = R_DTC_VersionGet
};

#if DTC_CFG_SOFTWARE_START_ENABLE
/** Callbacks for software start are stored here so they can be accessed by the appropriate ISR. */
static dtc_callback_t g_callbacks[DTC_MAX_NUM_SOFTWARE_EVENTS];
#endif

/*******************************************************************************************************************//**
 * @addtogroup DTC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Set transfer data in the vector table and enable transfer in ICU. Implements transfer_api_t::open.
 *
 * @retval SSP_SUCCESS           Successful open.  Transfer is configured and will start when trigger occurs.
 * @retval SSP_ERR_ASSERTION     An input parameter is invalid.
 * @retval SSP_ERR_IN_USE        The BSP hardware lock for the DTC is not available, or the index for this IRQ in the
 *                               DTC vector table is already configured.
 * @retval SSP_ERR_IRQ_BSP_DISABLED  The IRQ associated with the activation source is not enabled in the BSP.
 * @retval SSP_ERR_NOT_ENABLED   Auto-enable was requested, but enable failed due to an invalid input parameter.
 **********************************************************************************************************************/
ssp_err_t R_DTC_Open (transfer_ctrl_t         * const p_ctrl,
                      transfer_cfg_t const * const    p_cfg)
{
#if DTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(sizeof(transfer_info_t) == sizeof(dtc_reg_t));
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_cfg->p_info);
    SSP_ASSERT(1 != p_cfg->p_info->chain_mode);
    SSP_ASSERT(p_cfg->p_info->mode <= TRANSFER_MODE_BLOCK);
    SSP_ASSERT(p_cfg->p_info->size <= TRANSFER_SIZE_4_BYTE);
    SSP_ASSERT(1 != p_cfg->p_info->dest_addr_mode);
    SSP_ASSERT(1 != p_cfg->p_info->src_addr_mode);
    if (TRANSFER_MODE_NORMAL != p_cfg->p_info->mode)
    {
        SSP_ASSERT(p_cfg->p_info->length <= DTC_REPEAT_BLOCK_MAX_LENGTH);
    }
    DTC_ERROR_RETURN(p_ctrl->id != DTC_ID, SSP_ERR_IN_USE);
#endif /* if DTC_CFG_PARAM_CHECKING_ENABLE */

    IRQn_Type                irq     = dtc_irq_lookup(p_cfg->activation_source);
    p_ctrl->trigger                  = HW_DTC_ICUEventGet(irq);
    DTC_ERROR_RETURN(BSP_MAX_NUM_IRQn != irq, SSP_ERR_IRQ_BSP_DISABLED);

    /** DTC requires a one time initialization.  This will be handled only the first time this function
     *  is called. This initialization:
     *    -# Acquires the BSP hardware lock for the DTC to keep this section thread safe and prevent use of
     *       this driver if another driver has locked the DTC block.
     *    -# Powers on the DTC block.
     *    -# Initializes the vector table to NULL pointers.
     *    -# Sets the vector table base address.
     *    -# Enables DTC transfers. */
    if (!g_dtc_state_initialized)
    {
        ssp_err_t bsp_err = R_BSP_HardwareLock(BSP_HW_LOCK_DTC);
        DTC_ERROR_RETURN(SSP_SUCCESS == bsp_err, SSP_ERR_HW_LOCKED);
        HW_DTC_PowerOn();
        memset(&gp_dtc_vector_table, 0, BSP_MAX_NUM_IRQn * sizeof(transfer_info_t *));
#if DTC_CFG_SOFTWARE_START_ENABLE
        memset(&g_callbacks, 0, DTC_MAX_NUM_SOFTWARE_EVENTS * sizeof(dtc_callback_t));
#endif
        HW_DTC_VectorTableAddressSet(&gp_dtc_vector_table);
        HW_DTC_StartStop(DTC_START);
        g_dtc_state_initialized = true;
    }
    DTC_ERROR_RETURN(NULL == gp_dtc_vector_table[irq], SSP_ERR_IN_USE);

    /** For repeat and block modes, copy the data from the initial length into the reload length. */
    transfer_info_t * p_info_temp = p_cfg->p_info;
    do
    {
        if (TRANSFER_MODE_NORMAL != p_info_temp->mode)
        {
            dtc_reg_t * p_reg = (dtc_reg_t *) p_info_temp;
            p_reg->CRA_b.CRAH = p_reg->CRA_b.CRAL;
        }
        if(TRANSFER_CHAIN_MODE_DISABLED != p_info_temp->chain_mode)
        {/* There should be more elements on the chain */
        	p_info_temp++;
        }
        else
        {/* No more elements on the chain */
        	break;
        }
    } while (1);

#if DTC_CFG_SOFTWARE_START_ENABLE
    /** If p_callback is valid and trigger is set to ELC_EVENT_ELC_SOFTWARE_EVENT_0 or ELC_EVENT_ELC_SOFTWARE_EVENT_1,
     *  enable interrupts and store the p_callback in an array for access by ISR. */
    if ((NULL != p_cfg->p_callback) && ((ELC_EVENT_ELC_SOFTWARE_EVENT_0 == p_ctrl->trigger) ||
            (ELC_EVENT_ELC_SOFTWARE_EVENT_1 == p_ctrl->trigger)))
    {
        int32_t index = p_ctrl->trigger - ELC_EVENT_ELC_SOFTWARE_EVENT_0;
        IRQn_Type elc_irq = HW_DTC_ElcIrqGet(index);
        HW_DTC_ICUInterruptClear(elc_irq);
        NVIC_ClearPendingIRQ(elc_irq);
        NVIC_EnableIRQ(elc_irq);
        g_callbacks[index].p_callback = p_cfg->p_callback;
        g_callbacks[index].p_context  = p_cfg->p_context;
    }
#endif

    /** Configure the DTC transfer. See the hardware manual for details. */
    HW_DTC_ReadSkipEnableSet(DTC_READ_SKIP_DISABLED);
    gp_dtc_vector_table[irq] = p_cfg->p_info;

    /** Update internal variables. */
    p_ctrl->irq     = irq;
    p_ctrl->id      = DTC_ID;

    /** If auto_enable is true, enable transfer and ELC events if software start is used. */
    ssp_err_t err = SSP_SUCCESS;
    if (p_cfg->auto_enable)
    {
        err = R_DTC_Enable(p_ctrl);
    }

    /** Enable read skip after all settings are complete. */
    HW_DTC_ReadSkipEnableSet(DTC_READ_SKIP_ENABLED);

    DTC_ERROR_RETURN(SSP_SUCCESS == err, SSP_ERR_NOT_ENABLED);

    return SSP_SUCCESS;
} /* End of function R_DTC_Open */

/*******************************************************************************************************************//**
 * @brief  Reset transfer source, destination, and number of transfers.  Implements transfer_api_t::reset.
 *
 * @retval SSP_SUCCESS           Transfer reset successfully.
 * @retval SSP_ERR_ASSERTION     An input parameter is invalid.
 * @retval SSP_ERR_NOT_OPEN      Handle is not initialized.  Call R_DTC_Open to initialize the control block.
 * @retval SSP_ERR_NOT_ENABLED   Enable failed due to an invalid input parameter:
 *                                 - Transfer source must not be NULL.
 *                                 - Transfer destination must not be NULL.
 *                                 - Transfer length must not be 0.
 **********************************************************************************************************************/
ssp_err_t R_DTC_Reset (transfer_ctrl_t         * const   p_ctrl,
                       void const * volatile             p_src,
                       void                   * volatile p_dest,
                       uint16_t const                    num_transfers)
{
#if DTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(0 != num_transfers);
    DTC_ERROR_RETURN(p_ctrl->id == DTC_ID, SSP_ERR_NOT_OPEN);
#endif

#if DTC_CFG_PARAM_CHECKING_ENABLE
    if (TRANSFER_MODE_BLOCK == gp_dtc_vector_table[p_ctrl->irq]->mode)
    {
        SSP_ASSERT(0 != gp_dtc_vector_table[p_ctrl->irq]->length);
    }
#endif

    /** Disable transfers on this activation source. */
    HW_ICU_DTCDisable(p_ctrl->irq);

    /** Disable read skip prior to modifying settings. It will be enabled later. */
    HW_DTC_ReadSkipEnableSet(DTC_READ_SKIP_DISABLED);

    /** Reset transfer based on input parameters. */
    if (NULL != p_src)
    {
        gp_dtc_vector_table[p_ctrl->irq]->p_src = p_src;
    }

    if (NULL != p_dest)
    {
        gp_dtc_vector_table[p_ctrl->irq]->p_dest = p_dest;
    }

    if (TRANSFER_MODE_BLOCK == gp_dtc_vector_table[p_ctrl->irq]->mode)
    {
        gp_dtc_vector_table[p_ctrl->irq]->num_blocks = num_transfers;
    }
    else if (TRANSFER_MODE_NORMAL == gp_dtc_vector_table[p_ctrl->irq]->mode)
    {
        gp_dtc_vector_table[p_ctrl->irq]->length = num_transfers;
    }
    else /* (TRANSFER_MODE_REPEAT == gp_dtc_vector_table[p_ctrl->irq]->mode) */
    {
        /* Do nothing. */
    }

    /** Enables transfers on this activation source. */
    ssp_err_t err = R_DTC_Enable(p_ctrl);

    /** Enable read skip after all settings are complete. */
    HW_DTC_ReadSkipEnableSet(DTC_READ_SKIP_ENABLED);

    DTC_ERROR_RETURN(SSP_SUCCESS == err, SSP_ERR_NOT_ENABLED);

    return SSP_SUCCESS;
} /* End of function R_DTC_Reset */

/*******************************************************************************************************************//**
 * @brief  Start transfer. Implements transfer_api_t::start.
 *
 * @retval SSP_SUCCESS              Transfer started successfully.
 * @retval SSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval SSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DMAC_Open to initialize the control block.
 * @retval SSP_ERR_UNSUPPORTED      One of the following is invalid:
 *                                    - Handle was not configured for software activation.
 *                                    - Mode not set to TRANSFER_START_MODE_SINGLE.
 *                                    - DTC_SOFTWARE_START_ENABLE set to 0 (disabled) in ssp_cfg/driver/r_dtc_cfg.h.
 **********************************************************************************************************************/
ssp_err_t R_DTC_Start (transfer_ctrl_t         * const p_ctrl,
                       transfer_start_mode_t           mode)
{
#if DTC_CFG_SOFTWARE_START_ENABLE
#if DTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    DTC_ERROR_RETURN(p_ctrl->id == DTC_ID, SSP_ERR_NOT_OPEN);
    DTC_ERROR_RETURN(TRANSFER_START_MODE_SINGLE == mode, SSP_ERR_UNSUPPORTED);
    DTC_ERROR_RETURN((ELC_EVENT_ELC_SOFTWARE_EVENT_0 == p_ctrl->trigger) ||
            (ELC_EVENT_ELC_SOFTWARE_EVENT_1 == p_ctrl->trigger), SSP_ERR_UNSUPPORTED);
#endif

    if (ELC_EVENT_ELC_SOFTWARE_EVENT_0 == p_ctrl->trigger)
    {
        R_BSP_IrqStatusClear(ELC_SOFTWARE_EVENT_0_IRQn);
        g_elc_on_elc.softwareEventGenerate(ELC_SOFTWARE_EVENT_0);
    }
    else
    {
        R_BSP_IrqStatusClear(ELC_SOFTWARE_EVENT_1_IRQn);
        g_elc_on_elc.softwareEventGenerate(ELC_SOFTWARE_EVENT_1);
    }

    return SSP_SUCCESS;
#else
    SSP_PARAMETER_NOT_USED(p_ctrl);
    SSP_PARAMETER_NOT_USED(mode);
    return SSP_ERR_UNSUPPORTED;
#endif
} /* End of function R_DTC_Start */

/*******************************************************************************************************************//**
 * @brief  Placeholder for unsupported stop function. Implements transfer_api_t::stop.
 *
 * @retval SSP_ERR_UNSUPPORTED      DTC software start is not supported.
 **********************************************************************************************************************/
ssp_err_t R_DTC_Stop (transfer_ctrl_t         * const p_ctrl)
{
    /* This function isn't supported.  It is defined only to implement a required function of transfer_api_t.
     * Mark the input parameter as unused since this function isn't supported. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    return SSP_ERR_UNSUPPORTED;
} /* End of function R_DTC_Stop */

/*******************************************************************************************************************//**
 * @brief  Enable transfer and ELC events if they are used for software start. Implements transfer_api_t::enable.
 *
 * @retval SSP_SUCCESS              Counter value written successfully.
 * @retval SSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval SSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DTC_Open to initialize the control block.
 **********************************************************************************************************************/
ssp_err_t R_DTC_Enable (transfer_ctrl_t         * const p_ctrl)
{
#if DTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    transfer_info_t * p_local_info = gp_dtc_vector_table[p_ctrl->irq];
    SSP_ASSERT(NULL != p_local_info->p_dest);
    SSP_ASSERT(NULL != p_local_info->p_src);
    if (TRANSFER_MODE_NORMAL == p_local_info->mode)
    {
        SSP_ASSERT(p_local_info->length <= DTC_NORMAL_MAX_LENGTH);
    }
    DTC_ERROR_RETURN(p_ctrl->id == DTC_ID, SSP_ERR_NOT_OPEN);
#endif

    /** Enable transfer. */
    HW_ICU_DTCEnable(p_ctrl->irq);

#if DTC_CFG_SOFTWARE_START_ENABLE
    if ((ELC_EVENT_ELC_SOFTWARE_EVENT_0 == p_ctrl->trigger) ||
            (ELC_EVENT_ELC_SOFTWARE_EVENT_1 == p_ctrl->trigger))
    {
        /** If this is a software event, enable the IRQ. */
        int32_t index = (int32_t) (p_ctrl->trigger) - (int32_t) ELC_EVENT_ELC_SOFTWARE_EVENT_0;
        IRQn_Type irq = HW_DTC_ElcIrqGet(index);
        if (BSP_MAX_NUM_IRQn != irq)
        {
            NVIC_EnableIRQ(irq);
        }
    }
#endif

    return SSP_SUCCESS;
} /* End of function R_DTC_Enable */

/*******************************************************************************************************************//**
 * @brief  Disable transfer. Implements transfer_api_t::disable.
 *
 * @retval SSP_SUCCESS              Counter value written successfully.
 * @retval SSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval SSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DTC_Open to initialize the control block.
 **********************************************************************************************************************/
ssp_err_t R_DTC_Disable (transfer_ctrl_t         * const p_ctrl)
{
#if DTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    DTC_ERROR_RETURN(p_ctrl->id == DTC_ID, SSP_ERR_NOT_OPEN);
#endif

    /** Disable transfer. */
    HW_ICU_DTCDisable(p_ctrl->irq);

    return SSP_SUCCESS;
} /* End of function R_DTC_Disable */

/*******************************************************************************************************************//**
 * @brief  Set driver specific information. Implements transfer_api_t::infoGet.
 *
 * @retval SSP_SUCCESS              Counter value written successfully.
 * @retval SSP_ERR_ASSERTION        An input parameter is invalid.
 **********************************************************************************************************************/
ssp_err_t R_DTC_InfoGet   (transfer_ctrl_t        * const p_ctrl,
                           transfer_properties_t  * const p_info)
{
#if DTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_info);
#endif

    /** If a transfer is active, store it in p_in_progress. */
    uint16_t status = HW_DTC_StatusGet();
    p_info->in_progress = false;
    if (status >> 15)
    {
        IRQn_Type irq = (IRQn_Type) (status & 0x00FF);
        if (p_ctrl->irq == irq)
        {
            p_info->in_progress = true;
        }
    }

    /** Transfer information for the activation source is taken from DTC vector table. */
    transfer_info_t* p_dtc_transfer_info = gp_dtc_vector_table[p_ctrl->irq];
    if (TRANSFER_MODE_BLOCK == p_dtc_transfer_info->mode)
    {
        p_info->transfer_length_remaining = p_dtc_transfer_info->num_blocks;
    }
    else
    {
        p_info->transfer_length_remaining = p_dtc_transfer_info->length;

        /** Mask out the high byte in case of repeat transfer. */
        if (TRANSFER_MODE_REPEAT == p_dtc_transfer_info->mode)
        {
        	p_info->transfer_length_remaining &= 0x00FF;
        }
    }

    /** Store maximum transfer length. */
    if (TRANSFER_MODE_NORMAL == gp_dtc_vector_table[p_ctrl->irq]->mode)
    {
        p_info->transfer_length_max = DTC_NORMAL_MAX_LENGTH;
    }
    else
    {
        p_info->transfer_length_max = DTC_REPEAT_BLOCK_MAX_LENGTH;
    }

    return SSP_SUCCESS;
} /* End of function R_DTC_InfoGet */

/*******************************************************************************************************************//**
 * @brief      Disables transfer in the ICU, then clears transfer data from the DTC vector table. Implements
 *             transfer_api_t::close.
 *
 * @retval SSP_SUCCESS              Successful close.
 * @retval SSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval SSP_ERR_NOT_OPEN      Handle is not initialized.  Call R_DTC_Open to initialize the control block.
 **********************************************************************************************************************/
ssp_err_t R_DTC_Close (transfer_ctrl_t * const p_ctrl)
{
    ssp_err_t err = SSP_SUCCESS;

#if DTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    DTC_ERROR_RETURN(p_ctrl->id == DTC_ID, SSP_ERR_NOT_OPEN);
#endif

    /** Clear DTC enable bit in ICU. */
    HW_ICU_DTCDisable(p_ctrl->irq);

    /** Clear pointer in vector table. */
    gp_dtc_vector_table[p_ctrl->irq] = NULL;
    p_ctrl->id                       = 0U;

#if DTC_CFG_SOFTWARE_START_ENABLE
    if ((ELC_EVENT_ELC_SOFTWARE_EVENT_0 == p_ctrl->trigger) ||
            (ELC_EVENT_ELC_SOFTWARE_EVENT_1 == p_ctrl->trigger))
    {
        /** If this is a software event, disable the IRQ. */
        int32_t index = (int32_t) (p_ctrl->trigger) - (int32_t) ELC_EVENT_ELC_SOFTWARE_EVENT_0;
        IRQn_Type irq = HW_DTC_ElcIrqGet(index);
        if (BSP_MAX_NUM_IRQn != irq)
        {
            NVIC_DisableIRQ(irq);
        }
    }
#endif

    return err;
} /* End of function R_DTC_Close */

/*******************************************************************************************************************//**
 * @brief      Set driver version based on compile time macros.  Implements transfer_api_t::versionGet.
 *
 * @retval SSP_SUCCESS              Successful close.
 * @retval SSP_ERR_ASSERTION        An input parameter is invalid.
 **********************************************************************************************************************/
ssp_err_t R_DTC_VersionGet (ssp_version_t     * const p_version)
{
#if DTC_CFG_PARAM_CHECKING_ENABLE
    /** Verify parameters are valid */
    SSP_ASSERT(NULL != p_version);
#endif

    p_version->version_id = g_dtc_version.version_id;

    return SSP_SUCCESS;
} /* End of function R_DTC_VersionGet */

/*******************************************************************************************************************//**
 * @} (end addtogroup DTC)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      Finds the IRQ associated with the requested ELC event.
 *
 * @return     IRQ value if found, BSP_MAX_NUM_IRQn if not found.
 **********************************************************************************************************************/
static IRQn_Type dtc_irq_lookup(elc_event_t const event)
{
    for (IRQn_Type i = (IRQn_Type) 0; i < BSP_MAX_NUM_IRQn; i++)
    {
        if (event == HW_DTC_ICUEventGet(i))
        {
            /* This IRQ is tied to the requested ELC event.  Return this IRQ to be used as DTC trigger */
            return i;
        }
    }

    return BSP_MAX_NUM_IRQn;
}

#if DTC_CFG_SOFTWARE_START_ENABLE
/*******************************************************************************************************************//**
 * @brief      Interrupt called when transfer triggered from ELC software event 0 is complete.
 *
 * Saves context if RTOS is used, calls callback if one was provided in the open function, and restores context if
 * RTOS is used.
 **********************************************************************************************************************/
void elc_software_event_0_isr(void)
#if BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_0 != BSP_IRQ_DISABLED
{
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear(ELC_SOFTWARE_EVENT_0_IRQn);

    /** Call user p_callback */
    if (NULL != g_callbacks[0].p_callback)
    {
        transfer_callback_args_t args;
        args.p_context = g_callbacks[0].p_context;
        g_callbacks[0].p_callback(&args);
    }

    SF_CONTEXT_RESTORE
}
#endif

/*******************************************************************************************************************//**
 * @brief      Interrupt called when transfer triggered from ELC software event 1 is complete.
 *
 * Saves context if RTOS is used, calls callback if one was provided in the open function, and restores context if
 * RTOS is used.
 **********************************************************************************************************************/
#if BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_0 != BSP_IRQ_DISABLED
void elc_software_event_1_isr(void)
{
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear(ELC_SOFTWARE_EVENT_1_IRQn);

    /** Call user p_callback */
    if (NULL != g_callbacks[1].p_callback)
    {
        transfer_callback_args_t args;
        args.p_context = g_callbacks[1].p_context;
        g_callbacks[1].p_callback(&args);
    }

    SF_CONTEXT_RESTORE
}
#endif
#endif

/** TODO_DTC Find out how to make these interrupts trigger. */

#if BSP_IRQ_CFG_DTC_COMPLETE != BSP_IRQ_DISABLED
void dtc_complete_isr (void)
{
    /** Not sure how to make this trigger, so catch it if it does */
    while (1)
    {
        /* Do nothing */
    }
}
#endif

#if BSP_IRQ_CFG_DTC_END != BSP_IRQ_DISABLED
void dtc_end_isr (void)
{
    /** Not sure how to make this trigger, so catch it if it does */
    while (1)
    {
        /* Do nothing */
    }
}
#endif


