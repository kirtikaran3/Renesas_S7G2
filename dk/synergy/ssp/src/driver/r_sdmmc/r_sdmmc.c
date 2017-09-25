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
 * File Name    : r_sdmmc.c
 * Description  : SDMMC driver for SDHI/SDMMC device.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file r_sdmmc.c
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_sdmmc.h"
#include "r_sdmmc_private.h"
#include "r_sdmmc_private_api.h"
#include "r_sdmmc_cfg.h"
#include "sdmmc_driver.h"
#include "r_ioport.h"
#include "./hw/hw_sdmmc_private.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Macro for error logger. */
#ifndef SDMMC_ERROR_RETURN
#define SDMMC_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), g_module_name, &g_sdmmc_version)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Internal driver storage for p_callback, used by ISRs. */
typedef struct st_sdmmc_callback
{
    /** Callback for interrupt. Set to NULL for no CPU interrupt. */
    void (* p_callback)(sdmmc_callback_args_t * cb_data);

    /** Placeholder for user data.  Passed to the user p_callback in ::sdmmc_callback_args_t. */
    void const  * p_context;
} sdmmc_callback_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
void        sdhimmc0_accs_isr (void);

void        sdhimmc0_card_isr (void);

void        sdhimmc0_dma_req_isr (void);

void        sdhimmc0_sdio_isr (void);

void        sdhimmc1_accs_isr (void);

void        sdhimmc1_card_isr (void);

void        sdhimmc1_dma_req_isr (void);

void        sdhimmc1_sdio_isr (void);

static void sdmmc_access_irq_enable (uint8_t channel);

static void sdmmc_access_irq_disable (uint8_t channel);

static void sdmmc_card_irq_enable (uint8_t channel);

static void sdmmc_card_irq_disable (uint8_t channel);

static void sdmmc_sdio_irq_enable (uint8_t channel);

static void sdmmc_sdio_irq_disable (uint8_t channel);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/** Array of callback functions and contexts */
static sdmmc_ctrl_t * g_p_ctrls[SDMMC_MAX_NUM_CHANNELS];

/** Card Specific Data  */

#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t g_sdmmc_version =
{
    .api_version_minor  = SDMMC_API_VERSION_MINOR,
    .api_version_major  = SDMMC_API_VERSION_MAJOR,
    .code_version_major = SDMMC_CODE_VERSION_MAJOR,
    .code_version_minor = SDMMC_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/** Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char s_module_name[] = "SDMMC";
#endif

/** Callbacks are stored here so they can be accessed by the appropriate ISR */
static sdmmc_callback_t g_callbacks[SDMMC_MAX_NUM_CHANNELS];

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/** SDMMC function pointers   */
const sdmmc_api_t g_sdmmc_on_sdmmc =
{
    .open        = R_SDMMC_Open,
    .close       = R_SDMMC_Close,
    .read        = R_SDMMC_Read,
    .write       = R_SDMMC_Write,
    .control     = R_SDMMC_Control,
    .readIo      = R_SDMMC_ReadIo,
    .writeIo     = R_SDMMC_WriteIo,
    .readIoExt   = R_SDMMC_ReadIoExt,
    .writeIoExt  = R_SDMMC_WriteIoExt,
    .IoIntEnable = R_SDMMC_IoIntEnable,
    .versionGet  = R_SDMMC_VersionGet,
    .infoGet     = R_SDMMC_InfoGet,
    .erase       = R_SDMMC_Erase
};

/*******************************************************************************************************************//**
 * @addtogroup SDMMC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Open device for read/write and control.
 *
 * The Open function opens a SD or MMC device port for read/write and control. This function initializes
 * the SDMMC driver and hardware the first time it is called out of reset.
 *
 * @retval SSP_SUCCESS              Port is available and is now open for read/write/control access.
 * @retval SSP_ERR_ASSERTION        Null Pointer.
 * @retval SSP_ERR_INVALID_ARGUMENT One or more configuration options is invalid.
 * @retval SSP_ERR_IN_USE           The channel specified has already been opened. No configurations were changed. Call
 *                                  the associated Close function or use associated Control commands to reconfigure the
 *                                  channel.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_Open (sdmmc_ctrl_t * const p_ctrl, sdmmc_cfg_t const * const p_cfg)
{
    ssp_err_t ret_val = SSP_SUCCESS;   // setup return value for success

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if ((NULL == p_ctrl) || (NULL == p_cfg))
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR for NULL pointers.
    }
    /* Check for valid parameters */
    else if (p_cfg->hw.channel > SDMMC_MAX_CHANNEL_NUMBER)
    {
        ret_val = SSP_ERR_INVALID_ARGUMENT; /**< channel number out of range */
    }
    else
    {
        /* comment following code rules */
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */

    if (SSP_SUCCESS == ret_val)
    {
        /* All SDMMC channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
         * the base channel 0 lock yields the channel's lock type. */
        if (SSP_ERR_IN_USE == R_BSP_HardwareLock((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_SDHI0 + p_cfg->hw.channel)))
        {
            /* Channel is already open so return error */
            ret_val = SSP_ERR_HW_LOCKED;
        }
    }

    if (SSP_SUCCESS == ret_val)
    {
        /* Channel not open so it's ok to open channel for use */

        /* Turn off module stop bit (turn module on). */
        HW_SDMMC_PowerOn(p_cfg->hw.channel);

        /* Initialize SDHI hardware and SD card, get card specific data, if not already done */
        g_p_ctrls[p_cfg->hw.channel] = p_ctrl;            /**< save handle */
        sdmmc_access_irq_enable(p_cfg->hw.channel);
        sdmmc_card_irq_enable(p_cfg->hw.channel);
        ret_val = SDMMC_DriverInit(p_ctrl, p_cfg);        /**< initialize SD or eMMC if not already initialized. */
    }

    if (SSP_SUCCESS == ret_val)
    {
        p_ctrl->p_context  = p_cfg->p_context;            /**< save device context inside HAL driver */
        p_ctrl->p_callback = p_cfg->p_callback;           /**< register callback function from higher layer */

        /** If p_callback is selected, enable interrupts and store the p_callback in an array for access by ISR. */
        if (NULL != p_cfg->p_callback)
        {
            g_callbacks[p_cfg->hw.channel].p_callback = p_cfg->p_callback;
            g_callbacks[p_cfg->hw.channel].p_context  = p_cfg->p_context;
        }

        /* Enable SDHI interrupts and DMA */
        if (true == p_ctrl->status.sdio)
        {
            sdmmc_sdio_irq_enable(p_cfg->hw.channel);
        }
    }
    else
    {
        /* Disable SDHI interrupts and DMA */
        sdmmc_access_irq_disable(p_ctrl->hw.channel);
        sdmmc_card_irq_disable(p_ctrl->hw.channel);
        sdmmc_sdio_irq_disable(p_cfg->hw.channel);

        /* Turn on module stop bit (turn module off). */
        HW_SDMMC_PowerOff(p_ctrl->hw.channel);

        /* unlock lock */

        /* All SDMMC channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
         * the base channel 0 lock yields the channel's lock type. */
        R_BSP_HardwareUnlock((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_SDHI0 + p_cfg->hw.channel));
    }

    /* Process errors before returning */

    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;     // MISRA rule 15.5  - A function should have a single point of exit.
}

/*******************************************************************************************************************//**
 * @brief      Close open device port.
 *
 * The close function closes an open SD/MMC device port.
 *
 * @retval     SSP_SUCCESS              Successful close.
 * @retval     SSP_ERR_ASSERTION        The parameter p_ctrl is NULL.
 * @retval     SSP_ERR_INVALID_ARGUMENT Bad channel number.
 * @retval     SSP_ERR_NOT_OPEN         The channel is not opened.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_Close (sdmmc_ctrl_t * const p_ctrl)
{
    ssp_err_t ret_val = SSP_SUCCESS;
#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointer for NULL values */
    if (NULL == p_ctrl)
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */
    /* Channel is not open so return an error */
    if (SSP_SUCCESS == ret_val)
    {
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;
        }
    }

    if (SSP_SUCCESS == ret_val)
    {
        if (true == p_ctrl->transfer_in_progress)
        {
            ret_val = SSP_ERR_TRANSFER_BUSY;
        }
    }

    if (SSP_SUCCESS == ret_val)
    {
        /* Disable SDHI interrupts and DMA */
        sdmmc_access_irq_disable(p_ctrl->hw.channel);
        sdmmc_card_irq_disable(p_ctrl->hw.channel);
        sdmmc_sdio_irq_disable(p_ctrl->hw.channel);

        /* Channel is open so close it. */

        /* Turn on module stop bit (turn module off). */
        HW_SDMMC_PowerOff(p_ctrl->hw.channel);

        /* Unlock lock. */

        /* All SDMMC channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
         * the base channel 0 lock yields the channel's lock type. */
        R_BSP_HardwareUnlock((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_SDHI0 + p_ctrl->hw.channel));

        /* Clear ready status */
        p_ctrl->status.ready = false;
        /* clear pointer  to control block */
        if (g_p_ctrls[p_ctrl->hw.channel])
        {
            g_p_ctrls[p_ctrl->hw.channel]->p_callback = NULL;
            g_p_ctrls[p_ctrl->hw.channel]->p_context  = 0;
            g_p_ctrls[p_ctrl->hw.channel]             = NULL;
        }
    }

    /* Process errors before returning. */
    if (SSP_SUCCESS != ret_val)
    {
        /* Log error or assertion. */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;     // MISRA rule 15.5  - A function should have a single point of exit.
}

/*******************************************************************************************************************//**
 * @brief  Read data from SD/MMC.
 *
 * The Read function reads data from an SD or MMC device channel.
 *
 * @retval  SSP_SUCCESS                  Data read successfully.
 * @retval  SSP_ERR_ASSERTION            NULL pointer.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Bad channel number or sector address.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_Read (sdmmc_ctrl_t * const p_ctrl, uint8_t * const p_dest, uint32_t const start_sector,
                        uint32_t const sector_count)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if ((NULL == p_ctrl) || (NULL == p_dest))
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */

    if (SSP_SUCCESS == ret_val)
    {
        /* If channel is not open, return an error */
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;         // Channel is not open, so return error.
        }
    }

    if (SSP_SUCCESS == ret_val)
    {
        if (true == p_ctrl->transfer_in_progress)
        {
            ret_val = SSP_ERR_TRANSFER_BUSY;
        }
    }

    /* Channel is open so process request */

    /* Has card been removed since initialized? */
    if (SSP_SUCCESS == ret_val)
    {
        SDMMC_InfoGet(p_ctrl);
        if (false == p_ctrl->status.ready)
        {
            ret_val = SSP_ERR_CARD_NOT_READY;
        }
    }

    /* Call SDMMC protocol read function */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = SDMMC_SectorRead(p_ctrl, p_dest, start_sector, sector_count);
    }

    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;     // MISRA rule 15.5  - A function should have a single point of exit.
}

/*******************************************************************************************************************//**
 * @brief  Write data to SDMMC channel.
 *
 * @retval  SSP_SUCCESS                  Card write finished successfully.
 * @retval  SSP_ERR_ASSERTION            Handle or Source address is NULL.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Bad channel number or sector address.
 * @retval  SSP_ERR_WRITE_PROTECTED      SD or MMC card is Write Protected.
 *
 * @note This function is reentrant for different channels.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_Write (sdmmc_ctrl_t * const p_ctrl, uint8_t const * const p_source, uint32_t const start_sector,
                         uint32_t const sector_count)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if ((NULL == p_ctrl) || (NULL == p_source))
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */
    if (SSP_SUCCESS == ret_val)
    {
        /* If channel is not open, return an error */
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;
        }
    }

    if (SSP_SUCCESS == ret_val)
    {
        if (true == p_ctrl->transfer_in_progress)
        {
            ret_val = SSP_ERR_TRANSFER_BUSY;
        }
    }

    /* Channel is open so process request */

    /* Has card been removed since initialized? */
    if (SSP_SUCCESS == ret_val)
    {
        SDMMC_InfoGet(p_ctrl);
        if (false == p_ctrl->status.ready)
        {
            ret_val = SSP_ERR_CARD_NOT_READY;
        }
    }

    /* Check for write protection */
    if (SSP_SUCCESS == ret_val)
    {
        if (true == p_ctrl->status.write_protected)
        {
            ret_val = SSP_ERR_CARD_WRITE_PROTECTED;
        }
    }

    /* Call SDMMC protocol write function */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = SDMMC_SectorWrite(p_ctrl, p_source, start_sector, sector_count);
    }

    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;     // MISRA rule 15.5  - A function should have a single point of exit.
}

/*******************************************************************************************************************//**
 * @brief  Send control commands to and receive status of the SD/MMC channel.
 *
 * The Control function sends control commands to and receives the status of the SD/MMC channel.
 *
 * @retval SSP_SUCCESS              Command executed successfully.
 * @retval SSP_ERR_ASSERTION        Null Pointer.
 * @retval SSP_ERR_INVALID_ARGUMENT Command is invalid.
 * @retval SF_INFO_NOT_AVAILABLE   Information not available possibly because card has been removed or is defective.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_Control (sdmmc_ctrl_t * const p_ctrl, ssp_command_t const command, void * p_data)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if ((NULL == p_ctrl) || (NULL == p_data))
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */
    if (SSP_SUCCESS == ret_val)
    {
        /* If channel is not open, return an error */
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;
        }
    }

    /* Get card info for channel */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = SDMMC_InfoGet(p_ctrl);
    }

    /* Get card info for channel */
    if (SSP_SUCCESS == ret_val)
    {
        switch (command)
        {
            case SSP_COMMAND_GET_SECTOR_COUNT:

                /* Get info if card has been read. */

                *((uint32_t *) p_data) = p_ctrl->status.sector_count;
                break;

            case SSP_COMMAND_GET_SECTOR_SIZE:

                /* Get info if card has been read. */
                *((uint32_t *) p_data) = p_ctrl->status.sector_size;

                break;

            case SSP_COMMAND_GET_WRITE_PROTECTED:
                *((bool *) p_data) =  p_ctrl->status.write_protected;

                break;

            /* Add additional commands as needed. */

            default:
                ret_val = SSP_ERR_INVALID_ARGUMENT;
                break;
        }
    }

    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 * @brief  Read data from SDIO.
 *
 * The Read function reads data from an SDIO channel.
 *
 * @retval  SSP_SUCCESS                  Data read successfully.
 * @retval  SSP_ERR_ASSERTION            NULL pointer.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Bad channel number or sector address.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_ReadIo (sdmmc_ctrl_t * const p_ctrl,
                          uint8_t * const      p_data,
                          uint32_t const       function,
                          uint32_t const       address)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if ((NULL == p_ctrl) || (NULL == p_data))
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */

    if (SSP_SUCCESS == ret_val)
    {
        /* If channel is not open, return an error */
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;         // Channel is not open, so return error.
        }
    }

    /* Channel is open so process request */

    /* Has card been removed since initialized? */
    if (SSP_SUCCESS == ret_val)
    {
        SDMMC_InfoGet(p_ctrl);
        if (false == p_ctrl->status.ready)
        {
            ret_val = SSP_ERR_CARD_NOT_READY;
        }
    }

    /* Call SDMMC protocol read function */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = SDMMC_CMD52Read(p_ctrl, p_data, function, address);
    }

    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;     // MISRA rule 15.5  - A function should have a single point of exit.
}

/*******************************************************************************************************************//**
 * @brief  Write data to SDIO.
 *
 * @retval  SSP_SUCCESS                  Card write finished successfully.
 * @retval  SSP_ERR_ASSERTION            Handle or Source address is NULL.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Bad channel number or address.
 *
 * @note This function is reentrant for different channels.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_WriteIo (sdmmc_ctrl_t * const        p_ctrl,
                           uint8_t * const             p_data,
                           uint32_t const              function,
                           uint32_t const              address,
                           sdmmc_io_write_mode_t const read_after_write)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if ((NULL == p_ctrl) || (NULL == p_data))
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */
    if (SSP_SUCCESS == ret_val)
    {
        /* If channel is not open, return an error */
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;
        }
    }

    /* Channel is open so process request */

    /* Has card been removed since initialized? */
    if (SSP_SUCCESS == ret_val)
    {
        SDMMC_InfoGet(p_ctrl);
        if (false == p_ctrl->status.ready)
        {
            ret_val = SSP_ERR_CARD_NOT_READY;
        }
    }

    /* Call SDMMC protocol write function */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = SDMMC_CMD52Write(p_ctrl, p_data, function, address, read_after_write);
    }

    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;     // MISRA rule 15.5  - A function should have a single point of exit.
}

/*******************************************************************************************************************//**
 * @brief  Read data from SDIO.
 *
 * The Read function reads data from an SDIO channel.
 *
 * @retval  SSP_SUCCESS                  Data read successfully.
 * @retval  SSP_ERR_ASSERTION            NULL pointer.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Bad channel number or sector address.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_ReadIoExt (sdmmc_ctrl_t * const     p_ctrl,
                             uint8_t * const          p_dest,
                             uint32_t const           function,
                             uint32_t const           address,
                             uint32_t * const         count,
                             sdmmc_io_transfer_mode_t transfer_mode,
                             sdmmc_io_address_mode_t  address_mode)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if ((NULL == p_ctrl) || (NULL == p_dest))
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */

    if (SSP_SUCCESS == ret_val)
    {
        /* If channel is not open, return an error */
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;         // Channel is not open, so return error.
        }
    }

    /* Channel is open so process request */

    /* Is transfer in progress? */
    if (SSP_SUCCESS == ret_val)
    {
        if (true == p_ctrl->transfer_in_progress)
        {
            ret_val = SSP_ERR_TRANSFER_BUSY;
        }
    }

    /* Has card been removed since initialized? */
    if (SSP_SUCCESS == ret_val)
    {
        SDMMC_InfoGet(p_ctrl);
        if (false == p_ctrl->status.ready)
        {
            ret_val = SSP_ERR_CARD_NOT_READY;
        }
    }

    /* Call SDMMC protocol read function */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = SDMMC_CMD53Read(p_ctrl, p_dest, function, address, count, transfer_mode, address_mode);
    }

    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;     // MISRA rule 15.5  - A function should have a single point of exit.
}

/*******************************************************************************************************************//**
 * @brief  Write data to SDIO.
 *
 * @retval  SSP_SUCCESS                  Card write finished successfully.
 * @retval  SSP_ERR_ASSERTION            Handle or Source address is NULL.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Bad channel number or address.
 *
 * @note This function is reentrant for different channels.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_WriteIoExt (sdmmc_ctrl_t * const     p_ctrl,
                              uint8_t const * const    p_source,
                              uint32_t const           function,
                              uint32_t const           address,
                              uint32_t const           count,
                              sdmmc_io_transfer_mode_t transfer_mode,
                              sdmmc_io_address_mode_t  address_mode)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if ((NULL == p_ctrl) || (NULL == p_source))
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */
    if (SSP_SUCCESS == ret_val)
    {
        /* If channel is not open, return an error */
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;
        }
    }

    /* Channel is open so process request */

    /* Is transfer in progress? */
    if (SSP_SUCCESS == ret_val)
    {
        if (true == p_ctrl->transfer_in_progress)
        {
            ret_val = SSP_ERR_TRANSFER_BUSY;
        }
    }

    /* Has card been removed since initialized? */
    if (SSP_SUCCESS == ret_val)
    {
        SDMMC_InfoGet(p_ctrl);
        if (false == p_ctrl->status.ready)
        {
            ret_val = SSP_ERR_CARD_NOT_READY;
        }
    }

    /* Call SDMMC protocol write function */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = SDMMC_CMD53Write(p_ctrl, p_source, function, address, count, transfer_mode, address_mode);
    }

    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;     // MISRA rule 15.5  - A function should have a single point of exit.
}

/*******************************************************************************************************************//**
 * @brief   Enable SDIO Interrupt.
 *
 * @retval  SSP_SUCCESS                  Card enabled or disabled SDIO interrupts successfully.
 * @retval  SSP_ERR_NOT_ENABLED          Operation failed.
 * @note This function is reentrant for different channels.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_IoIntEnable (sdmmc_ctrl_t * const p_ctrl, bool enable)
{
    ssp_err_t ret_val = SSP_SUCCESS;
    if (true != SDHI_Sdio_Int_Enable(p_ctrl->hw.channel, enable))
    {
        ret_val = SSP_ERR_NOT_ENABLED;
    }

    return ret_val;     // MISRA rule 15.5  - A function should have a single point of exit.
}

/*******************************************************************************************************************//**
 * @brief  Get version of the SD/MMC driver.
 *
 * The VersionGet function returns the version of the firmware and API.
 *
 * @retval SSP_ERR_ASSERTION        Null Pointer.
 * @retval SSP_SUCCESS              Function executed successfully.
 *
 * @note This function is reentrant.
 *
 **********************************************************************************************************************/
ssp_err_t R_SDMMC_VersionGet (ssp_version_t * const p_version)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if (NULL == p_version)
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
    else
    {
#endif
    p_version->version_id = g_sdmmc_version.version_id;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
}
#endif
    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 * @brief  Get the card specific data of the SD card.
 *
 * The InfoGet function returns the Card Specific Data (CSD) of the SD card.
 *
 * @note This function is reentrant.
 *
 **********************************************************************************************************************/

ssp_err_t R_SDMMC_InfoGet (sdmmc_ctrl_t * const p_ctrl, sdmmc_info_t * const p_info)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if ((NULL == p_ctrl) || (NULL == p_info))
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */
    if (SSP_SUCCESS == ret_val)
    {
        /* If channel is not open, return an error */
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;
        }
    }

    /* Get card info for channel */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val                      = SDMMC_InfoGet(p_ctrl);
        p_info->card_type            = p_ctrl->status.card_type;
        p_info->ready                = p_ctrl->status.ready;
        p_info->hc                   = p_ctrl->status.hc;
        p_info->write_protected      = p_ctrl->status.write_protected;
        p_info->transfer_in_progress = p_ctrl->transfer_in_progress;
        p_info->csd_version          = p_ctrl->status.csd_version;
        p_info->sdhi_rca             = p_ctrl->status.sdhi_rca;
        p_info->max_clock_rate       = p_ctrl->status.max_clock_rate;
        p_info->clock_rate           = p_ctrl->status.clock_rate;
        p_info->sector_size          = p_ctrl->status.sector_size;
        p_info->sector_count         = p_ctrl->status.sector_count;
        p_info->erase_sector_count   = p_ctrl->status.erase_sector_count;
    }

    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 * @brief  Erase sectors in the SDMMC card.
 *
 * The Erase function erases the SD card.
 *
 * @retval SSP_ERR_INVALID_ARGUMENT     Bad channel number or sector address.
 * @retval SSP_SUCCESS                  Device status available.
 *
 * @note This function is reentrant for different channels.
 *
 **********************************************************************************************************************/

ssp_err_t R_SDMMC_Erase (sdmmc_ctrl_t * const p_ctrl, uint32_t const start_sector, uint32_t const sector_count)
{
    ssp_err_t ret_val = SSP_SUCCESS;

#if    (SDMMC_CFG_PARAM_CHECKING_ENABLE)
    /* Check pointers for NULL values */
    if (NULL == p_ctrl)
    {
        ret_val = SSP_ERR_ASSERTION;        // return ASSERTION ERROR
    }
#endif /* if    (SDMMC_CFG_PARAM_CHECKING_ENABLE) */
    if (SSP_SUCCESS == ret_val)
    {
        /* If channel is not open, return an error */
        if (g_p_ctrls[p_ctrl->hw.channel] != p_ctrl)
        {
            ret_val = SSP_ERR_NOT_OPEN;
        }
    }

    if (SSP_SUCCESS == ret_val)
    {
        if (true == p_ctrl->transfer_in_progress)
        {
            ret_val = SSP_ERR_TRANSFER_BUSY;
        }
    }


    /* Channel is open so process request */
    /* Has card been removed since initialized? */
    if (SSP_SUCCESS == ret_val)
    {
        SDMMC_InfoGet(p_ctrl);
        if (false == p_ctrl->status.ready)
        {
            ret_val = SSP_ERR_CARD_NOT_READY;
        }
    }

    /* Check for write protection */
    if (SSP_SUCCESS == ret_val)
    {
        if (true == p_ctrl->status.write_protected)
        {
            ret_val = SSP_ERR_CARD_WRITE_PROTECTED;
        }
    }

    /* Erase sector for channel */
    /* Call SDMMC protocol erase function */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = SDMMC_SectorErase(p_ctrl, start_sector, sector_count);
    }

    /* Process errors before returning */
    if (SSP_SUCCESS != ret_val)
    {
        /* log error or assertion */
        if (SSP_ERR_ASSERTION == ret_val)
        {
            SSP_ASSERT_FAIL;
        }
        else
        {
            SSP_ERROR_LOG(ret_val, s_module_name, g_sdmmc_version);
        }
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup SDMMC)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      Channel 0 Access ISR.
 *
 * Saves context if RTOS is used, clears interrupts, saves events and restores context if RTOS is used.
 **********************************************************************************************************************/

void sdhimmc0_accs_isr (void)
{
#if (defined(BSP_IRQ_CFG_SDHIMMC0_ACCS) && (BSP_IRQ_CFG_SDHIMMC0_ACCS  != BSP_IRQ_DISABLED))
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE
    sdhi_event_t          event_flags;
    sdmmc_callback_args_t args;
    args.event = SDMMC_EVENT_ACCESS;

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear(SDHIMMC0_ACCS_IRQn);
    SDHI_AccessISR(SDMMC_CHANNEL_0, &event_flags);                     /* Service the interrupt flags. */

    if (NULL != g_p_ctrls[SDMMC_CHANNEL_0])
    {
        if (g_p_ctrls[SDMMC_CHANNEL_0]->transfer_in_progress)           /* Check For Transfer in progress */
        {
            args.event = SDMMC_TransferEnd(g_p_ctrls[SDMMC_CHANNEL_0]); /* Get event */
        }
    }

    /** Call user p_callback */
    if (NULL != g_callbacks[SDMMC_CHANNEL_0].p_callback)                /* Check for user callback function. */
    {
        args.p_context = g_callbacks[SDMMC_CHANNEL_0].p_context;
        g_callbacks[SDMMC_CHANNEL_0].p_callback(&args);                 /* Call user callback function. */
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
#endif /* if (BSP_IRQ_CFG_SDHIMMC0_ACCS  != BSP_IRQ_DISABLED) */
}

/*******************************************************************************************************************//**
 * @brief      Channel 0 Card ISR.
 *
 * Saves context if RTOS is used, clears interrupts, checks for card removal and closes the channel if the card was
 * removed, and restores context if RTOS is used.
 **********************************************************************************************************************/

void sdhimmc0_card_isr (void)
{
#if (defined(BSP_IRQ_CFG_SDHIMMC0_CARD) && (BSP_IRQ_CFG_SDHIMMC0_CARD  != BSP_IRQ_DISABLED))
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE
    sdmmc_callback_args_t args;

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear(SDHIMMC0_CARD_IRQn);

    if ((true == SDHI_CardRemovedGet(SDMMC_CHANNEL_0)) && (NULL != g_p_ctrls[0]))
    {
        R_SDMMC_Close(g_p_ctrls[0]);                    /* Close the channel if card was removed */
        args.event = SDMMC_EVENT_CARD_REMOVED;
    }
    else
    {
        args.event = SDMMC_EVENT_CARD_INSERTED;
    }

    /** Call user p_callback */
    if (NULL != g_callbacks[0].p_callback)
    {
        args.p_context = g_callbacks[0].p_context;
        g_callbacks[0].p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
#endif /* if (BSP_IRQ_CFG_SDHIMMC0_CARD  != BSP_IRQ_DISABLED) */
}

/*******************************************************************************************************************//**
 * @brief      Channel 0 DMA ISR.
 *
 * Saves context if RTOS is used, clears interrupts, checks for DMA Interrupt.
 **********************************************************************************************************************/

void sdhimmc0_dma_req_isr (void)
{
#if (defined(BSP_IRQ_CFG_SDHIMMC0_DMA_REQ) && (BSP_IRQ_CFG_SDHIMMC0_DMA_REQ  != BSP_IRQ_DISABLED))
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear (SDHIMMC0_DMA_REQ_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
#endif /* if (BSP_IRQ_CFG_SDHIMMC0_DMA_REQ  != BSP_IRQ_DISABLED) */
}

/*******************************************************************************************************************//**
 * @brief      Channel 0 SDIO ISR.
 *
 * Saves context if RTOS is used, clears interrupts, checks for SDIO Interrupt.
 **********************************************************************************************************************/

void sdhimmc0_sdio_isr (void)
{
#if (defined(BSP_IRQ_CFG_SDHIMMC0_SDIO) && (BSP_IRQ_CFG_SDHIMMC0_SDIO  != BSP_IRQ_DISABLED))
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear (SDHIMMC0_SDIO_IRQn);

    SDHI_SdioISR(SDMMC_CHANNEL_0);
    /** Call user p_callback */
    if (NULL != g_callbacks[SDMMC_CHANNEL_0].p_callback)
    {
        sdmmc_callback_args_t args;
        args.event     = SDMMC_EVENT_SDIO;
        args.p_context = g_callbacks[SDMMC_CHANNEL_0].p_context;
        g_callbacks[SDMMC_CHANNEL_0].p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
#endif /* if (BSP_IRQ_CFG_SDHIMMC0_SDIO  != BSP_IRQ_DISABLED) */
}


/*******************************************************************************************************************//**
 * @brief      Channel 1 Access ISR.
 *
 * Saves context if RTOS is used, clears interrupts, saves events and restores context if RTOS is used.
 **********************************************************************************************************************/

void sdhimmc1_accs_isr (void)
{
#if (defined(BSP_IRQ_CFG_SDHIMMC1_ACCS) && (BSP_IRQ_CFG_SDHIMMC1_ACCS  != BSP_IRQ_DISABLED))
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE
    sdhi_event_t          event_flags;
    sdmmc_callback_args_t args;
    args.event = SDMMC_EVENT_ACCESS;

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear(SDHIMMC1_ACCS_IRQn);
    SDHI_AccessISR(SDMMC_CHANNEL_1, &event_flags);                     /* Service the interrupt flags. */

    if (NULL != g_p_ctrls[SDMMC_CHANNEL_1])
    {
        if (g_p_ctrls[SDMMC_CHANNEL_1]->transfer_in_progress)           /* Check For Transfer in progress */
        {
            args.event = SDMMC_TransferEnd(g_p_ctrls[SDMMC_CHANNEL_1]); /* Get event */
        }
    }

    /** Call user p_callback */
    if (NULL != g_callbacks[SDMMC_CHANNEL_1].p_callback)                /* Check for user callback function. */
    {
        args.p_context = g_callbacks[SDMMC_CHANNEL_1].p_context;
        g_callbacks[SDMMC_CHANNEL_1].p_callback(&args);                 /* Call user callback function. */
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
#endif /* if (BSP_IRQ_CFG_SDHIMMC1_ACCS  != BSP_IRQ_DISABLED) */
}

/*******************************************************************************************************************//**
 * @brief      Channel 1 Card ISR.
 *
 * Saves context if RTOS is used, clears interrupts, checks for card removal and closes the channel if the card was
 * removed, and restores context if RTOS is used.
 **********************************************************************************************************************/

void sdhimmc1_card_isr (void)
{
#if (defined(BSP_IRQ_CFG_SDHIMMC1_CARD) && (BSP_IRQ_CFG_SDHIMMC1_CARD  != BSP_IRQ_DISABLED))
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE
    sdmmc_callback_args_t args;

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear(SDHIMMC1_CARD_IRQn);

    if ((true == SDHI_CardRemovedGet(SDMMC_CHANNEL_1)) && (NULL != g_p_ctrls[1]))
    {
        R_SDMMC_Close(g_p_ctrls[SDMMC_CHANNEL_1]);                    /* Close the channel if card was removed */
        args.event = SDMMC_EVENT_CARD_REMOVED;
    }
    else
    {
        args.event = SDMMC_EVENT_CARD_INSERTED;
    }

    /** Call user p_callback */
    if (NULL != g_callbacks[SDMMC_CHANNEL_1].p_callback)
    {
        args.p_context = g_callbacks[SDMMC_CHANNEL_1].p_context;
        g_callbacks[SDMMC_CHANNEL_1].p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
#endif /* if (BSP_IRQ_CFG_SDHIMMC1_CARD  != BSP_IRQ_DISABLED) */
}

/*******************************************************************************************************************//**
 * @brief      Channel 1 DMA ISR.
 *
 * Saves context if RTOS is used, clears interrupts, checks for DMA Interrupt.
 **********************************************************************************************************************/

void sdhimmc1_dma_req_isr (void)
{
#if (defined(BSP_IRQ_CFG_SDHIMMC1_DMA_REQ) && (BSP_IRQ_CFG_SDHIMMC1_DMA_REQ  != BSP_IRQ_DISABLED))
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear (SDHIMMC1_DMA_REQ_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
#endif /* if (BSP_IRQ_CFG_SDHIMMC1_DMA_REQ  != BSP_IRQ_DISABLED) */
}

/*******************************************************************************************************************//**
 * @brief      Channel 1 SDIO ISR.
 *
 * Saves context if RTOS is used, clears interrupts, checks for SDIO Interrupt.
 **********************************************************************************************************************/

void sdhimmc1_sdio_isr (void)
{
#if (defined(BSP_IRQ_CFG_SDHIMMC1_SDIO) && (BSP_IRQ_CFG_SDHIMMC1_SDIO  != BSP_IRQ_DISABLED))
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear (SDHIMMC1_SDIO_IRQn);

    SDHI_SdioISR(SDMMC_CHANNEL_1);

    if (NULL != g_callbacks[1].p_callback)
    {
        sdmmc_callback_args_t args;
        args.event     = SDMMC_EVENT_SDIO;
        args.p_context = g_callbacks[1].p_context;
        g_callbacks[1].p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
#endif /* if (BSP_IRQ_CFG_SDHIMMC1_SDIO  != BSP_IRQ_DISABLED) */
}

/*******************************************************************************************************************//**
 * @brief      Enable Access ISR.
 *
 **********************************************************************************************************************/
static void sdmmc_access_irq_enable (uint8_t channel)
{
    (void) channel;             /* Suppress warning if no interrupts enabled. */
#if (defined(BSP_IRQ_CFG_SDHIMMC0_ACCS) && (BSP_IRQ_CFG_SDHIMMC0_ACCS  != BSP_IRQ_DISABLED))
    if (0 == channel)
    {
        NVIC_EnableIRQ(SDHIMMC0_ACCS_IRQn);
    }
#endif
#if (defined(BSP_IRQ_CFG_SDHIMMC1_ACCS) && (BSP_IRQ_CFG_SDHIMMC1_ACCS  != BSP_IRQ_DISABLED))
    if (1 == channel)
    {
        NVIC_EnableIRQ(SDHIMMC1_ACCS_IRQn);
    }
#endif
}

/*******************************************************************************************************************//**
 * @brief      Disable Access ISR.
 *
 **********************************************************************************************************************/
static void sdmmc_access_irq_disable (uint8_t channel)
{
    (void) channel;             /* Suppress warning if no interrupts enabled. */
#if (defined(BSP_IRQ_CFG_SDHIMMC0_ACCS) && (BSP_IRQ_CFG_SDHIMMC0_ACCS  != BSP_IRQ_DISABLED))
    if (0 == channel)
    {
        NVIC_DisableIRQ(SDHIMMC0_ACCS_IRQn);
    }
#endif
#if (defined(BSP_IRQ_CFG_SDHIMMC1_ACCS) && (BSP_IRQ_CFG_SDHIMMC1_ACCS  != BSP_IRQ_DISABLED))
    if (1 == channel)
    {
        NVIC_DisableIRQ(SDHIMMC1_ACCS_IRQn);
    }
#endif
}

/*******************************************************************************************************************//**
 * @brief      Enable Card ISR.
 *
 **********************************************************************************************************************/
static void sdmmc_card_irq_enable (uint8_t channel)
{
    (void) channel;             /* Suppress warning if no interrupts enabled. */
#if (defined(BSP_IRQ_CFG_SDHIMMC0_CARD) && (BSP_IRQ_CFG_SDHIMMC0_CARD  != BSP_IRQ_DISABLED))
    if (0 == channel)
    {
        NVIC_EnableIRQ(SDHIMMC0_CARD_IRQn);
    }
#endif
#if (defined(BSP_IRQ_CFG_SDHIMMC1_CARD) && (BSP_IRQ_CFG_SDHIMMC1_CARD  != BSP_IRQ_DISABLED))
    if (1 == channel)
    {
        NVIC_EnableIRQ(SDHIMMC1_CARD_IRQn);
    }
#endif
}

/*******************************************************************************************************************//**
 * @brief      Disable Card ISR.
 *
 **********************************************************************************************************************/
static void sdmmc_card_irq_disable (uint8_t channel)
{
    (void) channel;             /* Suppress warning if no interrupts enabled. */
#if (defined(BSP_IRQ_CFG_SDHIMMC0_CARD) && (BSP_IRQ_CFG_SDHIMMC0_CARD  != BSP_IRQ_DISABLED))
    if (0 == channel)
    {
        NVIC_DisableIRQ(SDHIMMC0_CARD_IRQn);
    }
#endif
#if (defined(BSP_IRQ_CFG_SDHIMMC1_CARD) && (BSP_IRQ_CFG_SDHIMMC1_CARD  != BSP_IRQ_DISABLED))
    if (1 == channel)
    {
        NVIC_DisableIRQ(SDHIMMC1_CARD_IRQn);
    }
#endif
}

/*******************************************************************************************************************//**
 * @brief      Enable SDIO ISR.
 *
 **********************************************************************************************************************/
static void sdmmc_sdio_irq_enable (uint8_t channel)
{
    (void) channel;             /* Suppress warning if no interrupts enabled. */
#if (defined(BSP_IRQ_CFG_SDHIMMC0_SDIO) && (BSP_IRQ_CFG_SDHIMMC0_SDIO  != BSP_IRQ_DISABLED))
    if (0 == channel)
    {
        NVIC_EnableIRQ(SDHIMMC0_SDIO_IRQn);
    }
#endif
#if (defined(BSP_IRQ_CFG_SDHIMMC1_SDIO) && (BSP_IRQ_CFG_SDHIMMC1_SDIO  != BSP_IRQ_DISABLED))
    if (1 == channel)
    {
        NVIC_EnableIRQ(SDHIMMC1_SDIO_IRQn);
    }
#endif
}

/*******************************************************************************************************************//**
 * @brief      Disable SDIO ISR.
 *
 **********************************************************************************************************************/
static void sdmmc_sdio_irq_disable (uint8_t channel)
{
    (void) channel;             /* Suppress warning if no interrupts enabled. */
#if (defined(BSP_IRQ_CFG_SDHIMMC0_SDIO) && (BSP_IRQ_CFG_SDHIMMC0_SDIO  != BSP_IRQ_DISABLED))
    if (0 == channel)
    {
        NVIC_DisableIRQ(SDHIMMC0_SDIO_IRQn);
    }
#endif
#if (defined(BSP_IRQ_CFG_SDHIMMC1_SDIO) && (BSP_IRQ_CFG_SDHIMMC1_SDIO  != BSP_IRQ_DISABLED))
    if (1 == channel)
    {
        NVIC_DisableIRQ(SDHIMMC1_SDIO_IRQn);
    }
#endif
}
