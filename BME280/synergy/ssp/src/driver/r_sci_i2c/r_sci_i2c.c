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
* File Name    : r_sci_i2c.c
* Description  : This module contains API functions and HLD layer functions for SCI simple I2C module.
**********************************************************************************************************************/


/**********************************************************************************************************************
Includes
**********************************************************************************************************************/
#include "r_sci_i2c.h"
#include "r_sci_i2c_private_api.h"
#include "r_sci_common_cfg.h"
#include "r_sci_common.h"
#include "../r_sci_common/r_sci_private.h"

#include "r_cgc.h"
#include "math.h"
#include "string.h"

#if (SCI_CFG_SIIC_INCLUDED)
/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/** Macro for error logger. */
#ifndef SCI_SIIC_ERROR_RETURN
#define SCI_SIIC_ERROR_RETURN(a, err)  SSP_ERROR_RETURN((a), (err), "siic", NULL)
#endif

#define I2C_CODE_READ   (0x01)
#define I2C_CODE_10BIT  (0xF0)

#define SCI_SIIC_NUM_DIVISORS_SYNC       (4)   // Number of synchronous divisors
#define SCI_SIIC_BRR_MAX                 (255) // Maximum Bit Rate Register (BRR)
#define SCI_SIIC_BRR_MIN                 (0)   // Minimum Bit Rate Register (BRR)
#define DUMMY_DATA                       (0xFF) // Dummy data for read operation

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/** Data transfer record */
typedef struct st_sci_siic_transfer
{
    i2c_ctrl_t  *   p_ctrl;         /**< Holds the channel and callback information */
    uint8_t     *   p_buff;         /**< Holds the data associated with the transfer */
    uint32_t        total;          /**< Holds the total number of data bytes to transfer */
    uint32_t        remain;         /**< Tracks the remaining data bytes to transfer */
    uint32_t        loaded;         /**< Tracks the number of data bytes written to the register */

    uint8_t         addr_low;       /**< Holds the last address byte to issue */
    uint8_t         addr_high;      /**< Holds the first address byte to issue in 10-bit mode  */
    uint8_t         addr_total;     /**< Holds the total number of address bytes to transfer */
    uint8_t         addr_remain;    /**< Tracks the remaining address bytes to transfer */
    uint8_t         addr_loaded;    /**< Tracks the number of address bytes written to the register */

    volatile bool   read;           /**< Holds the direction of the data byte transfer */
    volatile bool   restart;        /**< Holds whether or not the restart should be issued when done */
    volatile bool   err;            /**< Tracks whether or not an error occurred during processing */
    volatile bool   restarted;      /**< Tracks whether or not a restart was issued during the previous transfer */
    volatile bool   transaction_completed; /**< Tracks if the transaction started earlier was completed  */
} sci_siic_transfer_t;
 
/**********************************************************************************************************************
Private function prototypes
**********************************************************************************************************************/
/* Convenience functions */
static void        sci_siic_notify            (sci_siic_transfer_t  * const p_transfer,
                                               i2c_event_t          const         event);
static ssp_err_t   sci_siic_clock_settings    (uint32_t * const   p_rate,
		                                       uint8_t  * const   p_brr,
		                                       uint8_t  * const   p_divisor);
static ssp_err_t   sci_siic_abort_seq_master  (i2c_ctrl_t const   * const p_ctrl,
                                               bool       const           all);

/* Functions that manipulate hardware */
static ssp_err_t   sci_siic_open_hw_master    (i2c_ctrl_t         * const p_ctrl);
static void        sci_siic_close_hw_master   (i2c_ctrl_t         * const p_ctrl);
static void        sci_siic_abort_hw_master   (i2c_ctrl_t const   * const p_ctrl);
static ssp_err_t   sci_siic_run_hw_master     (sci_siic_transfer_t    * const p_transfer);


/**********************************************************************************************************************
Private global variables
**********************************************************************************************************************/
/** Open device count per channel */
static uint32_t g_ch_opened[SCI_PHY_CH_MAX];

/** Lowest stored speed per channel */
static uint32_t g_ch_speeds[SCI_PHY_CH_MAX];

/** In-progress transfer per channel */
static sci_siic_transfer_t g_in_progress[SCI_PHY_CH_MAX]  =
{ { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };

#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static ssp_version_t const g_sci_siic_master_version =
{
	.api_version_minor  = I2C_MASTER_API_VERSION_MINOR,
    .api_version_major  = I2C_MASTER_API_VERSION_MAJOR,
	.code_version_minor = SCI_SIIC_MASTER_CODE_VERSION_MINOR,
    .code_version_major = SCI_SIIC_MASTER_CODE_VERSION_MAJOR,
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif


/**********************************************************************************************************************
Global variables
**********************************************************************************************************************/
/** SCI control block array for channel <br> */
extern sci_ctrl_t g_sci_ctrl_blk[SCI_PHY_CH_MAX];

/** SCI SIIC Implementation of I2C device master interface */
i2c_api_master_t const g_i2c_master_on_sci =
{
    .versionGet = R_SCI_SIIC_MasterVersionGet,
    .open       = R_SCI_SIIC_MasterOpen,
    .close      = R_SCI_SIIC_MasterClose,
    .read       = R_SCI_SIIC_MasterRead,
    .write      = R_SCI_SIIC_MasterWrite,
    .reset      = R_SCI_SIIC_MasterReset
};

/** Baud rate divisor information(simple I2C mode) */
static const baud_setting_t sync_baud[SCI_SIIC_NUM_DIVISORS_SYNC] =
{
    {   32, 1, 1, 1, 0 }, /* divisor, BGDM, ABCS, ABCSE, n */
    {  128, 1, 1, 1, 1 },
    {  512, 1, 1, 1, 2 },
    { 2048, 1, 1, 1, 3 }
};

/** @addtogroup SIIC
 * @{
 */

/**********************************************************************************************************************
Functions
**********************************************************************************************************************/

/******************************************************************************************************************//**
 * @brief   Sets driver version based on compile time macros.
 *
 * @retval  SSP_SUCCESS                 Successful version get.
 * @retval  SSP_ERR_INVALID_POINTER     p_version is NULL.
**********************************************************************************************************************/
ssp_err_t R_SCI_SIIC_MasterVersionGet(ssp_version_t * const p_version)
{
#if SCI_SIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_version != NULL);
#endif

    p_version->version_id = g_sci_siic_master_version.version_id;

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief   Opens the I2C device. Power on I2C peripheral and perform initialization described in hardware manual.
 *
 *  This function will reconfigure the clock settings of the peripheral when a device with a lower rate than
 *  previously configured is opened.
 *
 * @retval  SSP_SUCCESS                 - Requested clock rate was set exactly.
 *                                      - Opened identical configuration of already open instance.
 * @retval  SSP_ERR_APPROXIMATION       Found settings to run close to the requested clock rate.
 * @retval  SSP_ERR_CLAMPED             Limited clock rate due to slower device having been opened previously.
 * @retval  SSP_ERR_ALREADY_OPEN        Attempted to open incompatible configuration of already open instance.
 * @retval  SSP_ERR_INVALID_POINTER     p_ctrl or p_cfg is NULL.
 * @retval  SSP_ERR_INVALID_CHANNEL     A bad channel was specified in p_cfg.
 * @retval  SSP_ERR_INVALID_RATE        The requested rate cannot be set.
 * @retval  SSP_ERR_UNSUPPORTED         Requesting features that are not supported yet.
**********************************************************************************************************************/
ssp_err_t R_SCI_SIIC_MasterOpen(i2c_ctrl_t         * const p_ctrl,
                                i2c_cfg_t  const   * const p_cfg)
{
	ssp_err_t err = SSP_SUCCESS;

#if SCI_SIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
    SSP_ASSERT(p_cfg != NULL);
    SSP_ASSERT(p_cfg->rate <= I2C_RATE_FAST);
    /* Check channel number. */
    err = r_sci_channel_param_check(p_cfg->channel);
    SCI_SIIC_ERROR_RETURN((SSP_SUCCESS == err), err);
#endif

    /* Get the transfer record associated with the channel */
    sci_siic_transfer_t * p_transfer;

    /* Check mode is not off, someone is using it, exist */
    if (SCI_MODE_OFF != g_sci_ctrl_blk[p_cfg->channel].mode)
    {
        return SSP_ERR_IN_USE;
    }

    /* Check if this device was already opened */
    if (p_ctrl->open)
    {
        err = SSP_ERR_IN_USE;
    }
    else
    {
        /* Record the configuration on the device for use later */
        memcpy(&p_ctrl->info, p_cfg, sizeof(p_ctrl->info));

        /* Open the hardware in master mode */
        err = sci_siic_open_hw_master(p_ctrl);

        /* Now handle any fatal errors that may have occurred */
        if (SSP_SUCCESS == err)
        {
            /* Finally, we can consider the device opened */
            p_ctrl->open = true;

            /* We need to track the number of unique devices open on the channel as well */
            g_ch_opened[p_ctrl->info.channel]++;

            p_transfer = &g_in_progress[p_ctrl->info.channel];

            p_transfer->transaction_completed = true;
        }

        /* Set control block for SCI channel to Simple I2C mode operation */
        g_sci_ctrl_blk[p_ctrl->info.channel].mode = SCI_MODE_SIIC;
    }

    return err;
}

/******************************************************************************************************************//**
 * @brief   Closes the I2C device. Power down I2C peripheral.
 *
 *  This function will safely terminate any in-progress I2C transfer with the device. If a transfer is aborted, the user
 *  will be notified via callback with an abort event. Since the callback is optional, this function will also return
 *  a specific error code in this situation.
 *
 *  @note   Since the device may be opened by multiple users, the only time a transfer will be aborted is if there are
 *          no more users of the device.
 *
 * @retval  SSP_SUCCESS                 Device closed without issue.
 * @retval  SSP_ERR_INVALID_POINTER     p_ctrl is NULL.
 * @retval  SSP_ERR_ABORTED             Device was closed while a transfer was in-progress.
 * @retval  SSP_ERR_NOT_OPEN            Device was not even opened.
**********************************************************************************************************************/
ssp_err_t R_SCI_SIIC_MasterClose(i2c_ctrl_t * const p_ctrl)
{
#if SCI_SIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
#endif

    ssp_err_t err = SSP_SUCCESS;

    /* Check if the device is even open, return an error if not */
    if (!p_ctrl->open)
    {
        err = SSP_ERR_NOT_OPEN;
    }
    else
    {
        /* Abort an in-progress transfer with this device only */
        err = sci_siic_abort_seq_master(p_ctrl, false);

        /* The device is now considered closed */
        p_ctrl->open = false;

        /* Remove this device from the record of unique devices opened on the channel */
        g_ch_opened[p_ctrl->info.channel]--;

        /* De-configure everything if there are no devices open on the channel */
        if (0 == g_ch_opened[p_ctrl->info.channel])
        {
        	sci_siic_close_hw_master(p_ctrl);
        }

        /* Set control block for SCI channel to off operation */
        g_sci_ctrl_blk[p_ctrl->info.channel].mode = SCI_MODE_OFF;
    }

    return err;
}

/******************************************************************************************************************//**
 * @brief   Performs a read from the I2C device.
 *
 *  This function will fail if there is already an in-progress I2C transfer on the associated channel. Otherwise, the
 *  I2C read operation will begin. When no callback is provided by the user, this function performs a blocking read.
 *  Otherwise, the read operation is non-blocking and the caller will be notified when the operation has finished by
 *  an SF_EVENT_RX_COMPLETE in the callback.
 *
 * @retval  SSP_SUCCESS                 Function executed without issue.
 * @retval  SSP_ERR_INVALID_POINTER     p_ctrl or p_dest is NULL.
 * @retval  SSP_ERR_INVALID_ARGUMENT    bytes is 0.
 * @retval  SSP_ERR_IN_USE              Another transfer was in-progress.
**********************************************************************************************************************/
ssp_err_t R_SCI_SIIC_MasterRead(i2c_ctrl_t * const p_ctrl,
                                uint8_t    * const p_dest,
                                uint32_t     const bytes,
                                bool         const restart)
{
#if SCI_SIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
    SSP_ASSERT(p_dest != NULL);
    SSP_ASSERT(bytes != 0);
#endif

    ssp_err_t err = SSP_SUCCESS;
    SCI_SIIC_ERROR_RETURN(p_ctrl->open != 0, SSP_ERR_NOT_OPEN);

    /* Get the transfer record associated with the channel */
    sci_siic_transfer_t * const p_transfer = &g_in_progress[p_ctrl->info.channel];

    /* Fail if there is already a transfer in progress */
    if (false == p_transfer->transaction_completed)
    {
        err = SSP_ERR_IN_USE;
    }
    else
    {
        /* Record the new information about this transfer */
        p_transfer->p_ctrl  = p_ctrl;
        p_transfer->p_buff  = p_dest;
        p_transfer->total   = bytes;
        p_transfer->remain  = bytes;
        p_transfer->restart = restart;
        p_transfer->read    = true;

        /* Handle the different addressing modes */
        if (p_ctrl->info.addr_mode == I2C_ADDR_MODE_7BIT)
        {
            /* Set the address bytes according to a 7-bit slave read command */
            p_transfer->addr_high  = 0;
            p_transfer->addr_low   = (uint8_t)(p_ctrl->info.slave << 1) | I2C_CODE_READ;
            p_transfer->addr_total = 1;
        }
        else
        {
            /* Check if this is a new transaction or a continuation */
            if (!p_transfer->restarted)
            {
                /* Set the address bytes according to a 10-bit slave read command */
                p_transfer->addr_high   = (uint8_t)((p_ctrl->info.slave >> 7) | I2C_CODE_10BIT) | I2C_CODE_READ;
                p_transfer->addr_low    = (uint8_t)p_ctrl->info.slave;
                /* Note: The address must be issued slightly differently when reading a slave with a 10-bit address */
                p_transfer->addr_total  = 3;
            }
            else
            {
                /* We will only need to issue the high address byte, but store it like a 7-bit read */
                p_transfer->addr_low    = (uint8_t)((p_ctrl->info.slave >> 7) | I2C_CODE_10BIT) | I2C_CODE_READ;
                p_transfer->addr_total  = 1;
            }
        }

        /* Kickoff the read operation as a master */
        err = sci_siic_run_hw_master(p_transfer);
    }

    return err;
}

/******************************************************************************************************************//**
 * @brief   Performs a write to the I2C device.
 *
 *  This function will fail if there is already an in-progress I2C transfer on the associated channel. Otherwise, the
 *  I2C write operation will begin. When no callback is provided by the user, this function performs a blocking write.
 *  Otherwise, the write operation is non-blocking and the caller will be notified when the operation has finished by
 *  an SF_EVENT_TX_COMPLETE in the callback.
 *
 * @retval  SSP_SUCCESS                 Function executed without issue.
 * @retval  SSP_ERR_INVALID_POINTER     p_ctrl or p_src is NULL.
 * @retval  SSP_ERR_INVALID_ARGUMENT    bytes is 0.
 * @retval  SSP_ERR_IN_USE              Another transfer was in-progress.
**********************************************************************************************************************/
ssp_err_t R_SCI_SIIC_MasterWrite(i2c_ctrl_t * const p_ctrl,
                                 uint8_t    * const p_src,
                                 uint32_t     const bytes,
                                 bool         const restart)
{
#if SCI_SIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
    SSP_ASSERT(p_src != NULL);
#endif

    ssp_err_t err = SSP_SUCCESS;

    SCI_SIIC_ERROR_RETURN(p_ctrl->open != 0, SSP_ERR_NOT_OPEN);

    /* Get the transfer record associated with the channel */
    sci_siic_transfer_t *  const p_transfer = &g_in_progress[p_ctrl->info.channel];

    /* Fail if there is already a transfer in progress */
    if (false == p_transfer->transaction_completed)
    {
        err = SSP_ERR_IN_USE;
    }
    else
    {
        /* Record the new information about this transfer */
        p_transfer->p_ctrl  = p_ctrl;
        p_transfer->p_buff  = p_src;
        p_transfer->total   = bytes;
        p_transfer->remain  = bytes;
        p_transfer->restart = restart;
        p_transfer->read    = false;

        /* Handle the different addressing modes */
        if (p_ctrl->info.addr_mode == I2C_ADDR_MODE_7BIT)
        {
            /* Set the address bytes according to a 7-bit slave write command */
            p_transfer->addr_high  = 0;
            p_transfer->addr_low   = (uint8_t)(p_ctrl->info.slave << 1) & ~I2C_CODE_READ;
            p_transfer->addr_total = 1;
        }
        else
        {
            /* Check if this is a new transaction or a continuation */
            if (!p_transfer->restarted)
            {
                /* Set the address bytes according to a 10-bit slave write command */
                p_transfer->addr_high   = (uint8_t)(((p_ctrl->info.slave >> 7) | I2C_CODE_10BIT) & ~I2C_CODE_READ);
                p_transfer->addr_low    = (uint8_t)p_ctrl->info.slave;
                p_transfer->addr_total  = 2;
            }
            else
            {
                /* We will only need to issue the high address byte, but store it like a 7-bit write */
                p_transfer->addr_low    = (uint8_t)(((p_ctrl->info.slave >> 7) | I2C_CODE_10BIT) & ~I2C_CODE_READ);
                p_transfer->addr_total  = 1;
            }
        }

        /* Kickoff the write operation as a master */
        err = sci_siic_run_hw_master(p_transfer);
    }
    return err;
  }

/**
 * @param[in]   p_ctrl      Pointer to I2C control struct.
 */
/******************************************************************************************************************//**
 * @brief   Aborts any in-progress transfer and forces the I2C peripheral into a ready state.
 *
 *  This function will safely terminate any in-progress I2C transfer with the device. If a transfer is aborted, the user
 *  will be notified via callback with an abort event. Since the callback is optional, this function will also return
 *  a specific error code in this situation.
 *
 * @retval  SSP_SUCCESS                 Channel was reset without issue.
 * @retval  SSP_ERR_INVALID_POINTER     p_ctrl is NULL.
 * @retval  SSP_ERR_ABORTED             A transfer was aborted while resetting the hardware.
 * @retval  SSP_ERR_NOT_OPEN            Device was not even opened.
**********************************************************************************************************************/
ssp_err_t R_SCI_SIIC_MasterReset(i2c_ctrl_t * const p_ctrl)
{
#if SCI_SIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
#endif

    SCI_SIIC_ERROR_RETURN(p_ctrl->open != 0, SSP_ERR_NOT_OPEN);

    /* Abort any transfer happening on the channel */
    ssp_err_t err = sci_siic_abort_seq_master(p_ctrl, true);

    return err;
}

/******************************************************************************************************************//**
 * @} (end defgroup SIIC)
**********************************************************************************************************************/

/**********************************************************************************************************************
Private Functions
**********************************************************************************************************************/

/******************************************************************************************************************//**
 * @brief   Single point for managing the logic around notifying a transfer has finished.
 *
 * @param[in]       p_transfer  Pointer to transfer that is ending.
 * @param[in]       event       The event code to pass to the callback.
**********************************************************************************************************************/
void  sci_siic_notify(sci_siic_transfer_t  * const p_transfer,
                      i2c_event_t         const         event)
{
    /* Check if we can notify the caller of the abort via callback */
    if (NULL != p_transfer->p_ctrl->info.p_callback)
    {
        /* Fill in the argument to the callback */
        i2c_callback_args_t args =
        {
            .bytes      = p_transfer->total - p_transfer->remain,
            .p_context  = p_transfer->p_ctrl->info.p_context,
            .event      = event
        };

        /* Now do the callback here */
        p_transfer->p_ctrl->info.p_callback(&args);

    }

    /** Set the flag indicating that the transaction is completed*/
    p_transfer->transaction_completed = true;

}

/*********************************************************************************************************************
 * @brief   This function changes baud rate. It evaluates and determines the best possible settings for the baud rate
 * registers.
 * @param[in] channel      Channel number of SCI module
 * @param[in] bitrate      bitrate[bps] e.g. 10000,100000; 400000(max), etc.
 * @retval  SSP_SUCCESS                  Baud rate is set successfully
 * @retval  SSP_ERR_INVALID_ARGUMENT     Baud rate is '0' or cannot set properly
 * @note    The application must pause for 1 bit time after the BRR register is loaded
 *          before transmitting/receiving to allow time for the clock to settle.
 ********************************************************************************************************************/
static ssp_err_t sci_siic_clock_settings(uint32_t * const   p_rate,
                                         uint8_t  * const   p_brr,
                                         uint8_t  * const   p_divisor)
{
    uint32_t         i             = 0;
    uint32_t         temp_brr      = SCI_SIIC_BRR_MIN;
    baud_setting_t   * p_baudinfo;
    ssp_err_t        result;
    uint32_t         clock_mhz;
    uint32_t         bitrate;

#if (BSP_CFG_MCU_PART_SERIES == 1)
    result = g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKB, &clock_mhz);
#else
    result = g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKA, &clock_mhz);
#endif


    if (SSP_SUCCESS != result)
    {
        return SSP_ERR_ASSERTION;
    }

    p_baudinfo = (baud_setting_t *) sync_baud;
    bitrate = *p_rate;

    /* FIND BEST_BRR_VALUE
     *  In table sync_baud", divisor value is associated with
     *  the divisor coefficient and the n values (clock divisor) , so once best divisor value is found,
     *  baud rate related register setting values are decided.
     *  The formula to calculate BRR is as follows and it must be 255 or less.
     *  BRR = (PCLKA/(div_coefficient * baud)) - 1
     */
    result = SSP_ERR_ASSERTION;
    for (i = 0; i < SCI_SIIC_NUM_DIVISORS_SYNC; i++)
    {
        temp_brr = clock_mhz / (p_baudinfo[i].div_coefficient * bitrate);
        if ((SCI_SIIC_BRR_MIN < temp_brr) && (temp_brr < (SCI_SIIC_BRR_MAX + 1)))
        {
            /* save the calculated value */
        	*p_brr = (uint8_t)(temp_brr - 1);
            *p_divisor = (uint8_t)i;

            /* re-calculate the bit rate based on the temp_brr and divisor coefficient */
            *p_rate = clock_mhz / (temp_brr * (p_baudinfo[i].div_coefficient) );

            /* Now set the bit rate register in the hardware */
            result = SSP_SUCCESS;
            break;
        }
    }

    /* if the appropriate BRR value is not found, zero out all the value */
    if (SSP_SUCCESS != result)
    {
    	*p_rate = 0;
    	*p_brr = 0;
    	*p_divisor = 0;
    }
    return result;
}

/******************************************************************************************************************//**
 * @brief   Single point for managing the logic around aborting a transfer when operating as a master.
 *
 * @param[in]       p_ctrl  Pointer to control struct of specific device
**********************************************************************************************************************/
static ssp_err_t sci_siic_abort_seq_master (i2c_ctrl_t const * const p_ctrl,
                                            bool       const   allDevices)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Get the transfer record associated with the channel */
    sci_siic_transfer_t * const p_transfer = &g_in_progress[p_ctrl->info.channel];

    /* Determines whether we abort for the entire channel, or only this device */
    bool match = (p_ctrl == p_transfer->p_ctrl) || allDevices;

    /* Check if there is an in-progress transfer associated with the match */
    if (match && (0 != p_transfer->remain))
    {
        /* Safely stop the hardware from operating */
        sci_siic_abort_hw_master(p_ctrl);

        /* Notify anyone waiting that the transfer is finished */
        sci_siic_notify(p_transfer, I2C_EVENT_ABORTED);

        /* Update the transfer descriptor to show no longer in-progress and an error */
        p_transfer->remain = 0;
        p_transfer->err    = true;

        /* Update the transfer descriptor to make sure interrupts no longer process */
        p_transfer->addr_loaded = p_transfer->addr_total;
        p_transfer->loaded     = p_transfer->total;

        /* Notify the caller via error code since the callback is optional */
        err = SSP_ERR_ABORTED;
    }
    return err;
}

/******************************************************************************************************************//**
 * @brief   Performs the hardware initialization sequence when operating as a master.
 *
 * @param[in]       p_ctrl  Pointer to control structure of specific device
**********************************************************************************************************************/
static ssp_err_t sci_siic_open_hw_master(i2c_ctrl_t * const p_ctrl)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Figure out if the hardware channel hasn't yet been configured */
    bool do_full_cfg  = !g_ch_opened[p_ctrl->info.channel];
    bool do_clock_cfg = do_full_cfg;
    if (do_full_cfg)
    {
        /* Perform the first part of the initialization sequence */
        HW_SCI_PowerOn(p_ctrl->info.channel);

        /* Disable all interrupts and clear all pending interrupts */
        HW_SCI_InterruptEnable (p_ctrl->info.channel, SCI_ALL_INT, false);

        /* Clear the Serial Status register */
        HW_SCI_SerialStatusClear(p_ctrl->info.channel);

        /* Clear the Serial Control register */
        HW_SCI_SerialControlClear(p_ctrl->info.channel);

        /* Clear the Smart Card Mode register */
        HW_SCI_SmartCardModeClear(p_ctrl->info.channel);

        /* Clear the Serial Mode register - SMR */
        HW_SCI_SerialModeClear(p_ctrl->info.channel);

        /* Disable FIFO mode */
        HW_SCI_SIIC_FIFODisable(p_ctrl->info.channel);

        /* Set SDA and SCL output pin to high-impedance state */
        HW_SCI_SIIC_Output(p_ctrl->info.channel, SCI_SIIC_OUTPUT_OFF);

        HW_SCI_ClockSynModeClear(p_ctrl->info.channel);

        /* Set the CKE bit in the SCR register to use internal clock */
        HW_SCI_BaudRateGenInternalClkSelect(p_ctrl->info.channel);

        /* Set bit order to be MSB first for I2C mode */
        HW_SCI_TransferDirection (p_ctrl->info.channel, true);

        /* No data inversion for I2C mode */
        HW_SCI_DataInversionEnable(p_ctrl->info.channel, false);

        /* No Smart Card selection for I2C mode */
        HW_SCI_SmartCardEnable(p_ctrl->info.channel, false);

        /* Disable Bit Rate Modulation function for Bit rate error correction */
    	HW_SCI_BitRateModulationEnable(p_ctrl->info.channel, false);
    }
    else
    {
        /* Do no re-configure the hardware if it is currently being used */
        if (0 != g_in_progress[p_ctrl->info.channel].remain)
        {
            return SSP_ERR_IN_USE;
        }

        /* Re-configure the clock rate if this device is slower than the current clock rate */
        do_clock_cfg = (p_ctrl->info.rate < g_ch_speeds[p_ctrl->info.channel]);
    }

    /* May need to configure a new clock rate for the channel */
    uint32_t bestMatch;
    uint8_t brr=0;
    uint8_t clk_divisor=0;
    if (do_clock_cfg)
    {
        /* We will be looking for the closest rate less-than-or-equal-to the requested rate */
        bestMatch = p_ctrl->info.rate;
        sci_siic_clock_settings(&bestMatch, &brr, &clk_divisor);

        /* Abort if we could not find good settings */
        if (bestMatch == 0)
        {
            err = SSP_ERR_INVALID_RATE;

            /* Undo the initialization if we were in the middle of it */
            if (do_full_cfg)
            {
                HW_SCI_PowerOff(p_ctrl->info.channel);
            }
            return err;
        }

        /* Record the found clock speed */
        g_ch_speeds[p_ctrl->info.channel] = bestMatch;

        /* Set the bit rate register in the hardware */
        HW_SCI_BitRateBRRSet(p_ctrl->info.channel, brr, clk_divisor);
    }

    /* Finish the initialization sequence, if this is its first opening */
    if (do_full_cfg)
    {
        /* Enable Noise Cancellation function  and set level to 1 */
    	HW_SCI_NoiseFilterSet(p_ctrl->info.channel, (noise_cancel_lvl_t)1);

    	/* Set this SCI channel to operate in simple I2C mode */
        HW_SCI_SIIC_ModeSet(p_ctrl->info.channel, SCI_SIIC_MODE_I2C);

        /* Set SDA Output Delay */
        HW_SCI_SIIC_DataOutputDelay (p_ctrl->info.channel, 0);

        /* Enable Clock Synchronization feature */
        HW_SCI_SIIC_ClockSynchronizationEnable (p_ctrl->info.channel, true);

        /* Set ACK NACK transmission */
        HW_SCI_SIIC_ACKNACKEnable(p_ctrl->info.channel, true);

        /* Select Interrupt Source */
        HW_SCI_SIIC_InterruptSource (p_ctrl->info.channel, SCI_SIIC_INTERRUPT_SOURCE_RECEPTION_TRANSMISSION);

        /* Clear SPI Mode register (SPMR) */
        HW_SCI_SPIModeClear(p_ctrl->info.channel);

        /* Enable both transmitter and receiver */
        HW_SCI_TransmitterReceiverEnable(p_ctrl->info.channel);
    }
    return err;
}

/******************************************************************************************************************//**
 * @brief   Performs the hardware initialization sequence when operating as a master.
 *
 * @param[in]       p_ctrl  Pointer to control structure of specific device
**********************************************************************************************************************/
static void sci_siic_close_hw_master(i2c_ctrl_t * const p_ctrl)
{
    /* Disables all interrupts */
    HW_SCI_InterruptEnable(p_ctrl->info.channel, SCI_ALL_INT, false);

    /* Clear command issuing flag and sets SDA/SCL to high-impedance state */
    HW_SCI_SIIC_LineHalt(p_ctrl->info.channel);

    /* Shutdown the hardware to save some power */
    HW_SCI_PowerOff(p_ctrl->info.channel);
}

/******************************************************************************************************************//**
 * @brief   Safely stops the current data transfer when operating as a master.
 *
 * @param[in]       p_ctrl  Pointer to control structure of specific device
**********************************************************************************************************************/
static void sci_siic_abort_hw_master(i2c_ctrl_t const * const p_ctrl)
{
    /* Disable channel interrupts */
    HW_SCI_InterruptEnable(p_ctrl->info.channel, SCI_ALL_INT, false);   /**< disables all interrupts */

    /* Check if the peripheral is holding the line low */
    if (HW_SCI_SIIC_SDALow(p_ctrl->info.channel))
    {
        /* Release the line manually by setting the SDA and SCL line to high impedance, command flag to 0 */
    	HW_SCI_SIIC_LineHalt(p_ctrl->info.channel);
    }

    /* Try to issue the stop condition now */
    HW_SCI_SIIC_SendStop(p_ctrl->info.channel);

    /* Wait until the stop condition is sent */
    while (HW_SCI_SIIC_StopPending(p_ctrl->info.channel))
    {

    }

    /* Shut off the SDA/SCL output pins anyway */
    HW_SCI_SIIC_LineHalt(p_ctrl->info.channel);
}

/******************************************************************************************************************//**
 * @brief   Performs the data transfer described by the parameters when operating as a master.
 *
 * @param[in]       p_transfer  Pointer to transfer that needs to be done.
**********************************************************************************************************************/
static ssp_err_t sci_siic_run_hw_master(sci_siic_transfer_t * const p_transfer)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Initialize fields used during transfer */
    p_transfer->addr_loaded = 0;
    p_transfer->loaded      = 0;
    p_transfer->remain      = p_transfer->total;
    p_transfer->addr_remain = p_transfer->addr_total;
    p_transfer->err         = false;

    /** Set the flag indicating that the transaction is not completed*/
    p_transfer->transaction_completed = false;

    /* Check if this is a new transaction or a continuation */
    if (!p_transfer->restarted)
    {
        /* Set SDA and SCL output pin to high-impedance state */
        HW_SCI_SIIC_Output(p_transfer->p_ctrl->info.channel, SCI_SIIC_OUTPUT_OFF);

    	/* Enables all interrupts */
        HW_SCI_InterruptEnable(p_transfer->p_ctrl->info.channel, SCI_TE_INT, true);

        /* Set the I2C into master mode and start interrupt processing */
        HW_SCI_SIIC_SendStart(p_transfer->p_ctrl->info.channel);
    }
    else
    {
        /* Clear for next transfer */
        p_transfer->restarted = false;

        /* This is a continuous operation, so send first address byte here instead of start condition*/
        /* Enable TX interrupt */
        HW_SCI_InterruptEnable(p_transfer->p_ctrl->info.channel, SCI_TX_INT, true);

        /* Sends out the first byte of address, TX will send out the 2nd byte if it is 10-bit address */
        /* If 7-bit address mode, issue the single address byte */
        if (1 == p_transfer->addr_total)
        {
            HW_SCI_SIIC_Wait_TXReady(p_transfer->p_ctrl->info.channel);
            HW_SCI_Write(p_transfer->p_ctrl->info.channel, p_transfer->addr_low);

            /* Update the number of address bytes remain for next pass */
            p_transfer->addr_remain--;
            /* Update the number of address bytes loaded for next pass */
            p_transfer->addr_loaded++;
        }
    }

    /* Check if we must block until the transfer is done */
    if (NULL == p_transfer->p_ctrl->info.p_callback)
    {
        /* Note: There is a hardware timeout that will allow this loop to exit */
    	while (false == p_transfer->transaction_completed)
        {
            /* The transfer descriptor is updated during interrupt processing */
        }

        if (p_transfer->err)
        {
            err = SSP_ERR_ABORTED;
        }
    }

    return err;
}

/******************************************************************************************************************//**
 * @brief   Handles the receive data full interrupt when operating as a master.
 *
 * @param[in]       channel     The target I2C block's channel number.
**********************************************************************************************************************/
void r_sci_siic_rxi_master(uint32_t const channel)
{
	/* Get the in-progress transfer */
    sci_siic_transfer_t * p_transfer = &g_in_progress[channel];

    /* Check if there are any bytes remaining */
    if (0 != p_transfer->remain)
    {
        uint32_t const current = p_transfer->total - p_transfer->remain;

        HW_SCI_SIIC_Wait_RXReady (channel);

        p_transfer->p_buff[current] = HW_SCI_Read(channel);

        /* Update the number of bytes received */
        p_transfer->remain--;
        /* update the number of bytes read */
        p_transfer->loaded++;
    }
}

/******************************************************************************************************************//**
 * @brief   Handles the transmit data empty interrupt when operating as a master.
 *
 * @param[in]       channel     The target I2C block's channel number.
**********************************************************************************************************************/
void r_sci_siic_txi_master(uint32_t const channel)
{
	/* Force sequencing between tx end and next byte load by disabling tx empty here, tx end will re-enable the tx empty interrupt */
	HW_SCI_InterruptEnable(channel, SCI_TE_INT, false);

    /* Get the in-progress transfer */
    sci_siic_transfer_t * p_transfer = &g_in_progress[channel];

    /* If it is 10 bit address, the second byte has to be sent out here */
    if (p_transfer->addr_total != p_transfer->addr_loaded)
    {
    	/* TX only handles the 2nd byte of 10-bit address */
    	/* If 10-bit address mode, issue the second byte here */
        if (1 != p_transfer->addr_total)
        {
            /* 10-bit addressing mode, address issuance changes between read and write */
            if (p_transfer->read)
            {
                /* The address must be issued once with the write code, and again with the read code */
                switch (p_transfer->addr_loaded)
                {
                    case 0:
                        HW_SCI_SIIC_Wait_TXEnd(channel);
                        HW_SCI_Write(channel, p_transfer->addr_high & (uint8_t) ~I2C_CODE_READ);
                        break;
                    case 1:
                        HW_SCI_SIIC_Wait_TXEnd(channel);
                        HW_SCI_Write(channel, p_transfer->addr_low);
                        break;
                    case 2:
                    default:
                        HW_SCI_SIIC_Wait_TXEnd(channel);
                        HW_SCI_Write(channel, p_transfer->addr_high);
                        break;
                }
            }
            else
            {
                /* The address must be issued only once with the write code */
                if (0 == p_transfer->addr_loaded)
                {
                    HW_SCI_SIIC_Wait_TXEnd(channel);
                    HW_SCI_Write(channel, p_transfer->addr_high);
                }
                else
                {
                    HW_SCI_SIIC_Wait_TXEnd(channel);
                    HW_SCI_Write(channel, p_transfer->addr_low);
                }
            }
        }

    	/* Update tracker that we have completed an address byte */
        p_transfer->addr_remain--;
        /* Update the number of address bytes loaded for next pass */
        p_transfer->addr_loaded++;
    }
    else /* Process data */
    {
    	if (!p_transfer->read)
    	{
    	    /* Check if there are any bytes remaining to load for transfer */
            if (p_transfer->total != p_transfer->loaded)
            {
            	if (HW_SCI_SIIC_AckReceived(channel))
                {
                    HW_SCI_SIIC_Wait_TXEnd(channel);
       		        HW_SCI_Write(channel, p_transfer->p_buff[p_transfer->loaded]);

       		        /* Update tracker that we have completed a data byte */
       	            p_transfer->remain--;
                    /* Update the number of bytes loaded for next pass */
                    p_transfer->loaded++;
                }
            	else  /* No ACK, Set error condition and stop */
                {
                    /* Force sequencing between tx end and tx empty interrupt */
                    HW_SCI_InterruptEnable(channel, SCI_TE_INT, true);
                    /* Set error condition, then stop  */
                    p_transfer->err = true;

                    /* Send a stop to end communication and let someone else have the bus */
               	    HW_SCI_SIIC_SendStop(channel);
                }
    	    }
        	else  /* there is no byte remaining to load for transfer */
            {
        	    /* Check if all data  bytes got transferred */
                if (p_transfer->total == p_transfer->loaded)
                {
                    /* Clear error condition, then restart or stop  */
                    p_transfer->err = false;

                    /* Disable TX interrupt */
                    HW_SCI_InterruptEnable(channel, SCI_TX_INT, false);

                    /* Force sequencing between tx end and tx empty interrupt */
                    HW_SCI_InterruptEnable(channel, SCI_TE_INT, true);

                    /* Write operation is done, send stop or restart if user wants to keep the bus */
                    if (p_transfer->restart)
                    {
                        /* Set restarted flag true*/
                        p_transfer->restarted = true;

                    	HW_SCI_SIIC_SendRestart(channel);
                    }
                    else
                    {
            			/* Send a stop to end communication and let someone else have the bus */
                	    HW_SCI_SIIC_SendStop(channel);
                   }
                }
            }
    	}
    	else /* This is a read operation */
    	{
            /* This must be the first byte to read */
    		if (0 == p_transfer->loaded)
    		{
            	if (HW_SCI_SIIC_AckReceived(channel))
                {
                    /* Reset the remain count for read */
            	    p_transfer->remain = p_transfer->total;
   				    if (p_transfer->loaded == p_transfer->total-1)
					{
					  /* Set NACK (SIMR2.IICACKT=1) if next byte is the last byte according to the manual */
					  HW_SCI_SIIC_ACKNACKEnable (channel, true);
					}
					else
					{
					  /* Set ACK (SIMR2.IICACKT=0) according to the manual */
					  HW_SCI_SIIC_AckSet (channel);
					}

                    /* Enable the RX interrupt */
            		HW_SCI_InterruptEnable(channel, SCI_RX_INT, true);
                    HW_SCI_SIIC_Wait_TXEnd(channel);
         			HW_SCI_Write(channel, DUMMY_DATA);
                }
                else /* No ACK, Set error condition and stop */
                {
                    /* Enable TE interrupt */
                    HW_SCI_InterruptEnable(channel, SCI_TE_INT, true);

                    /* Set error condition, then stop  */
                    p_transfer->err = true;

                    /* Send a stop to end communication and let someone else have the bus */
               	    HW_SCI_SIIC_SendStop(channel);
                }
    	     }
   		     else if (p_transfer->total != p_transfer->loaded) /* More data to read */
             {
             	if (HW_SCI_SIIC_AckReceived(channel))
                 {
             	     p_transfer->err = false;
   				     if (p_transfer->loaded == p_transfer->total-1)
 					 {
 					   /* Set NACK (SIMR2.IICACKT=1) if next byte is the last byte according to the manual */
 					   HW_SCI_SIIC_ACKNACKEnable (channel, true);
 					 }
 					 else
 					 {
 					   /* Set ACK (SIMR2.IICACKT=0) according to the manual */
 					   HW_SCI_SIIC_AckSet (channel);
 					 }
    	             /* Enable RX interrupt */
   	        	     HW_SCI_InterruptEnable(channel, SCI_RX_INT, true);
    	             HW_SCI_SIIC_Wait_TXEnd(channel);
    	             /* Write dummy data 'FF" to TDR  to get to the RX interrupt handler */
    	             HW_SCI_Write(channel, DUMMY_DATA);
                 }
                else /* No ACK, Set error condition and stop */
                {
                    /* Force sequencing between tx end and tx empty interrupt */
                    HW_SCI_InterruptEnable(channel, SCI_TE_INT, true);
                    /* Set error condition, then stop  */
                    p_transfer->err = true;
                    /* Send a stop to end communication and let someone else have the bus */
               	    HW_SCI_SIIC_SendStop(channel);
                }
    	     }
             else /* Read operation is completed, exit */
             {
    	         HW_SCI_InterruptEnable(channel, SCI_TE_INT, true);

                 /* Read operation is done, send stop or restart if user wants to keep the bus */
                 if (p_transfer->restart)
                 {
                     /* Set restarted flag true*/
                     p_transfer->restarted = true;

                     HW_SCI_SIIC_SendRestart(channel);
                 }
                 else
                 {
        		     /* Send a stop to end communication and let someone else have the bus */
            	     HW_SCI_SIIC_SendStop(channel);
                 }
            }
    	}
    }
}

/******************************************************************************************************************//**
 * @brief   Handles the transmit end interrupt when operating as a master.
 *
 * @param[in]       channel     The target I2C block's channel number.
**********************************************************************************************************************/
void r_sci_siic_tei_master(uint32_t const channel)
{
    /* Disable TE interrupt */
	HW_SCI_InterruptEnable(channel, SCI_TE_INT, false);

    /* Get the in-progress transfer */
    sci_siic_transfer_t * p_transfer = &g_in_progress[channel];

	/* TE handler handles the first byte of slave address according to the manual,
	 * the next byte will be handled by TX handler */
    /* Check if we are issuing the slave address */
    if (p_transfer->addr_total != p_transfer->addr_loaded)
    {
    	/* Enable TX interrupt */
    	HW_SCI_InterruptEnable(channel, SCI_TX_INT, true);

    	/* TE sends out the first byte of address, TX will send out the 2nd byte if it is 10-bit address */
    	/* If 7-bit address mode, issue the single address byte */
        if (1 == p_transfer->addr_total)
        {
            HW_SCI_SIIC_Wait_TXReady(channel);
            HW_SCI_Write(channel, p_transfer->addr_low);

            /* Update the number of address bytes remain for next pass */
            p_transfer->addr_remain--;
            /* Update the number of address bytes loaded for next pass */
            p_transfer->addr_loaded++;
        }
        return;
    }

    /* Make sure there were no errors during processing */
    if (!p_transfer->err)
    {
        /* Write operation completed, exit */
        if ((!p_transfer->read) && (0 == p_transfer->remain))
        {
            /* Notify anyone waiting that the transfer is finished */
            sci_siic_notify(p_transfer, I2C_EVENT_TX_COMPLETE);

        	/* Disable TX interrupt */
        	HW_SCI_InterruptEnable(channel, SCI_TX_INT, false);

        	/* Send stop or restart if user wants to keep the bus */
            if (p_transfer->restart)
            {
            	/* Do Nothing, leave the bus on */
            }
            else
            {
            	/* Set SDA and SCL output pin to high-impedance state */
            	HW_SCI_SIIC_LineHalt(channel);
            }
        }
        else
        {
            /* Read operation completed, exit */
            if ((p_transfer->read) && (0 == p_transfer->remain))
            {
                /* Notify anyone waiting that the transfer is finished */
            	sci_siic_notify(p_transfer, I2C_EVENT_RX_COMPLETE);

            	/* Disable RX interrupt */
            	HW_SCI_InterruptEnable(channel, SCI_RX_INT, false);

            	/* Send stop or restart if user wants to keep the bus */
                if (p_transfer->restart)
                {
                	/* Do Nothing, leave the bus on */
                }
                else
                {
                	/* Set SDA and SCL output pin to high-impedance state */
                	HW_SCI_SIIC_LineHalt(channel);
                }
            }
        }
    }
    else /* Any error, just notify and stop */
    {
        /* Error occurred, exit */
    	sci_siic_notify(p_transfer, I2C_EVENT_ABORTED);
    	HW_SCI_SIIC_LineHalt(channel);
    }
}

/******************************************************************************************************************//**
 * @brief   Handles the error interrupts when operating as a master.
 *
 * @param[in]       channel     The target I2C block's channel number.
**********************************************************************************************************************/
void r_sci_siic_err_master(uint32_t const channel)
{
    /* Get the in-progress transfer */
    sci_siic_transfer_t * p_transfer = &g_in_progress[channel];

    /* Abort an in-progress transfer with the current device */
    sci_siic_abort_seq_master(p_transfer->p_ctrl, false);
}

#endif /* end of SCI_CFG_SIIC_INCLUDED */

/* end of r_sci_siic.c */


