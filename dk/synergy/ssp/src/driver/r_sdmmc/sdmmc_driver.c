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
 * File Name    : sdmmc_driver.c
 * Description  : SDMMC protocol driver for SDHI/SDMMC device.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file sdmmc_driver.c
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief middle level drivers for SD MMC cards.
 *
 * SDMMC Drivers to access SD and MMC memory devices, including eMMC.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "sdhi.h"
#include "bsp_api.h"
#include "sdmmc_driver.h"
#include "sdcard.h"
#include "r_cgc_api.h"
#include "hw/hw_sdmmc_private.h"
#include "r_transfer_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define DELAY_TIME          ((uint32_t)50)                         // 50  microseconds
#define MMC_INIT_DELAY_TIME ((uint32_t)50000)                      // 50  microseconds
#define NCC_DELAY           ((uint32_t)25)                         // 25  microseconds
#define DATA_TIMEOUT        ((uint32_t)500000)                     // 500 milliseconds
#define ACCESS_TIMEOUT      ((uint32_t)250000)                     // 250 milliseconds

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static bool    card_init (sdmmc_ctrl_t * const p_ctrl);

static void    SDIO_reset (sdmmc_ctrl_t * const p_ctrl);

static bool    card_type_check (sdmmc_ctrl_t * const p_ctrl);

static bool    mmc_card_type_check (sdmmc_ctrl_t * const p_ctrl);

static bool    sd_card_type_check (sdmmc_ctrl_t * const p_ctrl);

static void    write_protect_get (sdmmc_ctrl_t * const p_ctrl);

static bool    clock_optimize (sdmmc_ctrl_t * const p_ctrl);

static void    SDIO_clock_optimize (sdmmc_ctrl_t * const p_ctrl);

static bool    sd_high_speed (sdmmc_ctrl_t * const p_ctrl, uint32_t mode);

static uint8_t SDIO_capability_get (sdmmc_ctrl_t * const p_ctrl);

static void    csd_save (sdmmc_ctrl_t * const p_ctrl, csd_reg_t * csd_reg);

static bool    csd_extended_get (sdmmc_ctrl_t * const p_ctrl);

static bool    data_pre_read (sdmmc_ctrl_t * const p_ctrl, uint16_t command, uint32_t argument, uint32_t byte_count);

static bool    data_post_access (sdmmc_ctrl_t * const p_ctrl);

static bool    csd_get (sdmmc_ctrl_t * const p_ctrl);

static bool    cid_get (sdmmc_ctrl_t * const p_ctrl);

static bool    card_address_get (sdmmc_ctrl_t * const p_ctrl);

static bool    card_select (sdmmc_ctrl_t * const p_ctrl);

static bool    card_status_get (sdmmc_ctrl_t * const p_ctrl);

static bool    bus_width_set (sdmmc_ctrl_t * const p_ctrl);

static bool    SDIO_bus_width_set (sdmmc_ctrl_t * const p_ctrl, uint8_t width);

static bool    block_length_set (sdmmc_ctrl_t * const p_ctrl);

static bool    csd_send (sdmmc_ctrl_t * const p_ctrl, csd_reg_t * csd_reg);

static bool    data_block_read (sdmmc_ctrl_t * const p_ctrl, uint8_t * p_data, uint32_t * index, uint32_t byte_count);

static bool    dma_transfer_read (sdmmc_ctrl_t * const p_ctrl, uint32_t block_count, uint32_t bytes,
                                  uint8_t * p_data);

static bool    dma_transfer_write (sdmmc_ctrl_t * const p_ctrl,
                                   uint32_t             block_count,
                                   uint32_t             bytes,
                                   const uint8_t        * p_data);

static bool      dma_transfer_end (sdmmc_ctrl_t * const p_ctrl);

static void      delay (uint32_t time);

static void      timeout_set (volatile uint32_t * p_timer, uint32_t timer_value);

static bool      timeout_get (volatile uint32_t * p_timer);

static ssp_err_t cmd52 (sdmmc_ctrl_t * const p_ctrl, sdio_arg_t * p_arg);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/* SD card TRAN_SPEED time values(multiplied times 10), from SD specification  */
static const uint8_t g_time_value_tenths[16] =
{
    0, 10, 12, 13, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80
};

/* SD card TRAN_SPEED transfer rate unit values(divided by 10), from SD specification  */
static const uint32_t g_unit_value_tens[4] =
{
    10000, 100000, 1000000, 10000000
};

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 *   Initializes driver and media card. Get type, size and other card specific information.
 *   @param[in] channel                 Channel of SDHI.
 *   @retval SSP_SUCCESS                Operation completed successfully.
 *   @retval SSP_ERR_CARD_INIT_FAILED   Media failed to initialize.
 *   @retval SSP_ERR_CARD_NOT_INSERTED  Card not inserted.
 **********************************************************************************************************************/
ssp_err_t SDMMC_DriverInit (sdmmc_ctrl_t * const p_ctrl, sdmmc_cfg_t const * const p_cfg)
{
    ssp_err_t ret_val;
    ret_val                      = SSP_SUCCESS;

    p_ctrl->hw.channel           = p_cfg->hw.channel;
    p_ctrl->hw.media_type        = p_cfg->hw.media_type;
    p_ctrl->hw.bus_width         = p_cfg->hw.bus_width;
    p_ctrl->p_lower_lvl_transfer = p_cfg->p_lower_lvl_transfer;
    p_ctrl->status.ready         = false;                            /* initial card status is not initialized or
                                                                      * installed */
    p_ctrl->status.hc            = false;

    p_ctrl->status.sdio          = false;
    p_ctrl->transfer_in_progress = false;


    /* Initialize Host Interface Hardware */

    if (!SDHI_Init(p_ctrl->hw.channel))
    {
        ret_val = SSP_ERR_CARD_INIT_FAILED; /* Hardware initialization failed */
    }

    /* Give channel time to power up before detecting card */
    R_BSP_SoftwareDelay((uint32_t)300, BSP_DELAY_UNITS_MILLISECONDS);
    if (SSP_SUCCESS == ret_val)
    {
        /* Check for inserted card. */
        /* eMMC card soldered to board, no insertion needed */

        if (SDMMC_MEDIA_TYPE_CARD == p_ctrl->hw.media_type)
        {
            if (!SDHI_CardInsertStatusGet(p_ctrl->hw.channel))
            {
                ret_val = SSP_ERR_CARD_NOT_INSERTED;
            }
        }

        /* card inserted, initialize */
    }

    if (SSP_SUCCESS == ret_val)
    {
        if (card_init(p_ctrl) != true)
        {
            ret_val = SSP_ERR_CARD_INIT_FAILED;     /* Card initialization failed, cannot do anything with this card */
        }
    }

    if (SSP_SUCCESS == ret_val)
    {
        SDHI_InterruptFlagsClear(p_ctrl->hw.channel);
        p_ctrl->status.ready = true;                            /* status could change if card removed */
                                                                /* card initialization succeeded */
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Read data sector(s) from card.
 *   @param[in]  channel            Channel of SDHI.
 *   @param[out] p_data             Pointer to data buffer to be read from card.
 *   @param[in]  start_block        Block address of first sector to be read.
 *   @param[in]  block_count        Number of contiguous blocks to be read.
 *   @retval SSP_SUCCESS            Operation completed successfully.
 *   @retval SSP_ERR_READ_FAILED    Operation failed.
 **********************************************************************************************************************/
ssp_err_t SDMMC_SectorRead (sdmmc_ctrl_t * const p_ctrl, uint8_t * p_data, uint32_t start_block, uint32_t block_count)
{
    uint16_t      command;
    uint32_t      argument;
    card_status_t response;
    ssp_err_t     ret_val;
    sdmmc_error_t error;
    error                        = SDMMC_ERROR_NO_ERROR; /* Initialize error status. */
    p_ctrl->transfer_in_progress = false;

    /*  SDHC or SDXC card use block address */
    if (true == p_ctrl->status.hc)
    {
        argument = start_block;             /* Uses block address, block length is fixed to 512 bytes regardless of
                                             * CMD16,
                                             **/
    }
    else
    {
        argument = (start_block * p_ctrl->status.sector_size);        /* SDSC and MMC cards use byte address */
    }

    if (block_count > (uint32_t)1)
    {
        command = SD_CMD_READ_MULTIPLE_BLOCK;                         /* Send read multiple block command if more than
                                                                       * one block. */
    }
    else
    {
        command = SD_CMD_READ_SINGLE_BLOCK;             /* Send read single block command if just one block. */
    }

    SDHI_BlockCountSet(p_ctrl->hw.channel, block_count);
    if (true == (dma_transfer_read(p_ctrl, block_count, SDMMC_DFLT_BLKSIZE, p_data))) /* Enable DMA/DTC transfer. */
    {
        p_ctrl->transfer_in_progress = true;
    }
    else
    {
        error = SDMMC_ERROR_TRANSFER;               /* Transfer went wrong. */
    }

    if (SDMMC_ERROR_NO_ERROR == error)              /* Continue, if no previous errors. */
    {
        SDHI_EventFlagsClear(p_ctrl->hw.channel);   /* Clear event flags before sending command. */

        /* Set block size */
        SDHI_BlockSizeSet(p_ctrl->hw.channel, SDMMC_DFLT_BLKSIZE);
    }

    if (SDMMC_ERROR_NO_ERROR == error)              /* Continue, if no previous errors. */
    {
        /* Send Read command. */
        if (!(SDHI_CommandSend(p_ctrl->hw.channel, command, argument)))
        {
            error = SDMMC_ERROR_COMMAND_ERROR;      /* Return error if command failed. */
        }

        if (SD_CMD_READ_MULTIPLE_BLOCK == command)
        {
            SDHI_ResponseMultipleBlockGet(p_ctrl->hw.channel, &response);
        }
        else
        {
            SDHI_ResponseGet(p_ctrl->hw.channel, &response);
        }
    }

    /** Transfer taking place here. */

    /** Exit non-blocking function */
    if (SDMMC_ERROR_NO_ERROR == error)
    {
        ret_val = SSP_SUCCESS;
    }
    else
    {
        p_ctrl->transfer_in_progress = false;
        dma_transfer_end(p_ctrl);
        ret_val                      = SSP_ERR_READ_FAILED;
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Write data sector(s) to card..
 *   @param[in] channel             Channel of SDHI.
 *   @param[in] p_data              Pointer to data to be written to card.
 *   @param[in] start_block         Block address of first sector to be written.
 *   @param[in] block_count         Number of contiguous blocks to be written.
 *   @retval SSP_SUCCESS            Operation completed successfully.
 *   @retval SSP_ERR_WRITE_FAILED   Operation failed.
 **********************************************************************************************************************/
ssp_err_t SDMMC_SectorWrite (sdmmc_ctrl_t * const p_ctrl,
                             const uint8_t        * p_data,
                             uint32_t             start_block,
                             uint32_t             block_count)
{
    uint8_t       command;
    uint32_t      argument;
    card_status_t response;
    ssp_err_t     ret_val;
    sdmmc_error_t error;
    error                        = SDMMC_ERROR_NO_ERROR;
    p_ctrl->transfer_in_progress = false;

    /* Check for write protected card */
    if (p_ctrl->status.write_protected)
    {
        error = SDMMC_ERROR_WRITE_PROTECTED;        /* Device is write protected, so it can't be written to. */
    }

    /*  SDHC or SDXC card use block address */
    if (true == p_ctrl->status.hc)
    {
        argument = start_block;        /* Use block address, block length is fixed to 512 bytes regardless of CMD16,
                                       **/
    }
    else
    {
        argument = (start_block *  p_ctrl->status.sector_size);        /* SDSC and MMC cards use byte address */
    }

    if (block_count > (uint32_t)1)
    {
        command = SD_CMD_WRITE_MULTIPLE_BLOCK;                         /* send write multiple block command if more than
                                                                        * one block */
    }
    else
    {
        command = SD_CMD_WRITE_SINGLE_BLOCK;        /* Send write single block command if just one block. */
    }

    SDHI_BlockCountSet(p_ctrl->hw.channel, block_count);
    if (true == (dma_transfer_write(p_ctrl, block_count, SDMMC_DFLT_BLKSIZE, p_data))) /* Enable DMA/DTC transfer. */
    {
        p_ctrl->transfer_in_progress = true;
    }
    else
    {
        error = SDMMC_ERROR_TRANSFER;               /* Transfer went wrong. */
    }

    if (SDMMC_ERROR_NO_ERROR == error)              /* Continue, if no previous errors. */
    {
        SDHI_EventFlagsClear(p_ctrl->hw.channel);   /* Clear event flags before sending command. */

        if  (SDHI_CardBusyGet(p_ctrl->hw.channel))  /* Check for card busy condition. */
        {
            error = SDMMC_ERROR_CARD_BUSY;          /* Card is busy, set error flag. */
        }
    }

    /* Set block size */
    SDHI_BlockSizeSet(p_ctrl->hw.channel, SDMMC_DFLT_BLKSIZE);

    if (SDMMC_ERROR_NO_ERROR == error)              /* Continue, if no previous errors. */
    {
        /* Send Write command */
        if (!(SDHI_CommandSend(p_ctrl->hw.channel, command, argument)))
        {
            error = SDMMC_ERROR_COMMAND_ERROR;      /* Command function returned an error. */
        }

        if (SD_CMD_WRITE_MULTIPLE_BLOCK == command)
        {
            SDHI_ResponseMultipleBlockGet(p_ctrl->hw.channel, &response);
        }
        else
        {
            SDHI_ResponseGet(p_ctrl->hw.channel, &response);
        }
    }

    /** Transfer taking place here. */

    /** Exit non-blocking function */
    if (SDMMC_ERROR_NO_ERROR == error)
    {
        ret_val = SSP_SUCCESS;
    }
    else
    {
        p_ctrl->transfer_in_progress = false;
        dma_transfer_end(p_ctrl);
        ret_val                      = SSP_ERR_WRITE_FAILED;
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Finish DMA/DTC transfer.
 *   @param[in] channel             Channel of SDHI.
 *   @retval SSP_SUCCESS            Operation completed successfully.
 *   @retval SSP_ERR_WRITE_FAILED   Operation failed.
 **********************************************************************************************************************/
sdmmc_event_t SDMMC_TransferEnd (sdmmc_ctrl_t * const p_ctrl)
{
    sdmmc_event_t ret_val;

    ret_val = SDMMC_EVENT_NONE;

    /* check for errors */

    if (SDHI_BitEventErrorGet(p_ctrl->hw.channel))
    {
        ret_val                      = SDMMC_EVENT_TRANSFER_ERROR;        /* Return transfer error. */
        p_ctrl->transfer_in_progress = false;                             /* Transfer complete (aborted). */
        dma_transfer_end(p_ctrl);                                         /* End DMA transfer. */
    }
    else            /* check for access end */
    {
        if (SDHI_AccessEndGet(p_ctrl->hw.channel))
        {
            SDHI_AccessEndClear(p_ctrl->hw.channel);                      /* Clear access end flag */
            if (true != dma_transfer_end(p_ctrl))
            {
                ret_val = SDMMC_EVENT_TRANSFER_ERROR;                     /* Return transfer error. */
            }
            else
            {
                ret_val = SDMMC_EVENT_TRANSFER_COMPLETE;                  /* Return success. */
            }
            p_ctrl->transfer_in_progress = false;                         /* Transfer complete. */
        }
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Erase card sector(s).
 *   @param[in] channel             Channel of SDHI.
 *   @param[in] start_block         Block address of first sector to be erased.
 *   @param[in] sector_count        Number of contiguous blocks to be erased.
 *   @retval SSP_SUCCESS            Operation completed successfully.
 *   @retval SSP_ERR_ERASE_FAILED   Operation failed.
 **********************************************************************************************************************/
ssp_err_t SDMMC_SectorErase (sdmmc_ctrl_t * const p_ctrl, uint32_t start_block, uint32_t sector_count)
{
    uint32_t start_address;
    uint32_t end_address;
    uint16_t start_command;
    uint16_t end_command;
    uint32_t argument;

    /* Check for valid sector count */
    if (sector_count > p_ctrl->status.erase_sector_count) /* v1 and v2 are the same for this field */
    {
        return SSP_ERR_ERASE_FAILED;                      /* can't erase them all */
    }

    /*  SDHC, SDXC and eMMC high capacity media use block address. */
    if (true == p_ctrl->status.hc)
    {
        start_address = start_block;         /* Uses block address, block length is fixed to 512 bytes regardless of
                                              * CMD16. */
        end_address   = ((start_block + sector_count) - 1);
    }
    else
    {
        start_address = (start_block * SDMMC_DFLT_BLKSIZE);    /* SDSC cards and 2GB, and under, eMMC devices use byte
                                                                * address. */
        end_address   = ((start_block + sector_count) * SDMMC_DFLT_BLKSIZE);
    }

    if (SDMMC_CARD_TYPE_MMC == p_ctrl->status.card_type)
    {
        start_command = SD_CMD_TAG_ERASE_GROUP_START;
        end_command   = SD_CMD_TAG_ERASE_GROUP_END;
        argument      = 0x01;
    }
    else
    {
        start_command = SD_CMD_ERASE_WR_BLK_START;
        end_command   = SD_CMD_ERASE_WR_BLK_END;
        argument      = (uint32_t)0xffffffff;
    }

    /* send ERASE_WR_BLK_START command */
    if (!(SDHI_CommandSend(p_ctrl->hw.channel, start_command, start_address)))
    {
        return SSP_ERR_ERASE_FAILED;
    }

    /* send ERASE_WR_BLK_END command */
    if (!(SDHI_CommandSend(p_ctrl->hw.channel, end_command, end_address)))
    {
        return SSP_ERR_ERASE_FAILED;
    }

    /* send erase command */
    if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_ERASE, argument)))
    {
        return SSP_ERR_ERASE_FAILED;
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 *   Get card status.
 *   @param[in]  channel     Channel of SDHI.
 *   @param[out] p_info      Pointer to card info.
 *   @retval SSP_SUCCESS     Operation completed successfully.
 **********************************************************************************************************************/
ssp_err_t SDMMC_InfoGet (sdmmc_ctrl_t * const p_ctrl)
{
    write_protect_get(p_ctrl);                                        /* update write protection status */

    if (SDMMC_MEDIA_TYPE_CARD ==  p_ctrl->hw.media_type)              /* Update insert status, pin is only used on
                                                                       * cards. */
    {
        if (false == (SDHI_CardInsertStatusGet(p_ctrl->hw.channel)))  /* If card was removed, set status to not ready.
                                                                      **/
        {
            p_ctrl->status.ready = false;                             /* it will have to be re-initialized before ready
                                                                       * again */
        }
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 *   SDIO Command 53 Read.
 *   @param[in] channel             Channel of SDHI.
 *   @param[in] block_count         Number of contiguous blocks to be written.
 *   @retval SSP_SUCCESS            Operation completed successfully.
 *   @retval SSP_ERR_WRITE_FAILED   Operation failed.
 **********************************************************************************************************************/
ssp_err_t SDMMC_CMD53Read (sdmmc_ctrl_t * const     p_ctrl,
                           uint8_t * const          p_dest,
                           uint32_t const           function,
                           uint32_t const           address,
                           uint32_t * const         count,
                           sdmmc_io_transfer_mode_t transfer_mode,
                           sdmmc_io_address_mode_t  address_mode)
{
    uint16_t          command;
    sdio_arg_t        argument;
    card_status_t     response;
    ssp_err_t         ret_val;
    sdmmc_error_t     error;
    volatile uint32_t byte_count;
    uint32_t          block_count;

    error   = SDMMC_ERROR_NO_ERROR;
    command = SD_CMD_IO_READ_EXT_SINGLE_BLOCK;                 /* Default to single block read. */

    if (SDMMC_IO_MODE_TRANSFER_BLOCK == transfer_mode)
    {
        block_count = *count;
        byte_count  = SDMMC_DFLT_BLKSIZE;
        if (block_count > (uint32_t)1)
        {
            command = SD_CMD_IO_READ_EXT_MULTI_BLOCK;             /* Multi Block read command. */
        }
    }
    else
    {
        block_count = 1;
        if (0 == (*count))
        {
            byte_count = SDMMC_DFLT_BLKSIZE;
        }
        else
        {
            byte_count = *count;
        }
    }

    if (true == (dma_transfer_read(p_ctrl, block_count, byte_count, p_dest)))   /* Enable DMA/DTC transfer if available.
                                                                                 **/
    {
        p_ctrl->transfer_in_progress = true;
    }
    else
    {
        error = SDMMC_ERROR_TRANSFER;               /* Transfer went wrong. */
    }

    /* Set block size */
    SDHI_BlockSizeSet(p_ctrl->hw.channel, (uint16_t) byte_count);
    SDHI_BlockCountSet(p_ctrl->hw.channel, block_count);
    SDHI_EventFlagsClear(p_ctrl->hw.channel);       /* Clear event flags before sending command. */
    if  (SDHI_CardBusyGet(p_ctrl->hw.channel))      /* Check for card busy condition. */
    {
        error = SDMMC_ERROR_CARD_BUSY;              /* Card is busy, set error flag. */
    }

    if (*count == (uint32_t)512)
    {
        *count = (uint32_t)0x00;                        /* According to SDIO spec, 512 = 0. */
    }

    if (SDMMC_ERROR_NO_ERROR == error)                  /* Continue, if no previous errors. */
    {
        argument.cmd_53_arg.count            = (*count & 0x1ff);
        argument.cmd_53_arg.function_number  = (function & 0x07);
        argument.cmd_53_arg.block_mode       = transfer_mode;
        argument.cmd_53_arg.op_code          = address_mode;
        argument.cmd_53_arg.register_address = (address & 0x1ffff);
        argument.cmd_53_arg.rw_flag          = (uint32_t)0;

        /* Send Read command. */
        if (!(SDHI_CommandSend(p_ctrl->hw.channel, command, argument.arg)))
        {
            error = SDMMC_ERROR_COMMAND_ERROR;              /* Return error if command failed. */
        }

        SDHI_ResponseGet(p_ctrl->hw.channel, &response);
    }

    /** Transfer taking place here. */

    if (SDMMC_ERROR_NO_ERROR == error)
    {
        ret_val = SSP_SUCCESS;
    }
    else
    {
        p_ctrl->transfer_in_progress = false;
        dma_transfer_end(p_ctrl);
        ret_val                      = SSP_ERR_READ_FAILED;
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   SDIO Command 53 Write.
 *   @param[in] channel             Channel of SDHI.
 *   @param[in] block_count         Number of contiguous blocks to be written.
 *   @retval SSP_SUCCESS            Operation completed successfully.
 *   @retval SSP_ERR_WRITE_FAILED   Operation failed.
 **********************************************************************************************************************/
ssp_err_t SDMMC_CMD53Write (sdmmc_ctrl_t * const     p_ctrl,
                            uint8_t const * const    p_source,
                            uint32_t const           function,
                            uint32_t const           address,
                            uint32_t const           count,
                            sdmmc_io_transfer_mode_t transfer_mode,
                            sdmmc_io_address_mode_t  address_mode)
{
    uint16_t      command;
    sdio_arg_t    argument;
    card_status_t response;
    ssp_err_t     ret_val;
    sdmmc_error_t error;
    uint32_t      byte_count;
    uint32_t      block_count;
    uint32_t      count_arg;

    error   = SDMMC_ERROR_NO_ERROR;
    command = SD_CMD_IO_WRITE_EXT_SINGLE_BLOCK;                             /* Default to single block write. */

    if (SDMMC_IO_MODE_TRANSFER_BLOCK == transfer_mode)
    {
        block_count = count;
        byte_count  = SDMMC_DFLT_BLKSIZE;
        if (block_count > (uint32_t)1)
        {
            command = SD_CMD_IO_WRITE_EXT_MULTI_BLOCK;                       /* Multi Block write command. */
        }
    }
    else
    {
        block_count = 1;
        if (0 == count)
        {
            byte_count = SDMMC_DFLT_BLKSIZE;
        }
        else
        {
            byte_count = count;
        }
    }

    if (true == (dma_transfer_write(p_ctrl, block_count, byte_count, p_source))) /* Enable DMA/DTC transfer if
                                                                                  *available. */
    {
        p_ctrl->transfer_in_progress = true;
    }
    else
    {
        error = SDMMC_ERROR_TRANSFER;          /* Transfer went wrong. */
    }

    /* Set block size */
    SDHI_BlockSizeSet(p_ctrl->hw.channel, (uint16_t) byte_count);
    SDHI_BlockCountSet(p_ctrl->hw.channel, block_count);
    SDHI_EventFlagsClear(p_ctrl->hw.channel);       /* Clear event flags before sending command. */
    if  (SDHI_CardBusyGet(p_ctrl->hw.channel))      /* Check for card busy condition. */
    {
        error = SDMMC_ERROR_CARD_BUSY;              /* Card is busy, set error flag. */
    }

    count_arg = count;
    if (count == (uint32_t)512)
    {
        count_arg = (uint32_t)0x00;                 /* According to SDIO spec, 512 = 0. */
    }

    if (SDMMC_ERROR_NO_ERROR == error)              /* Continue, if no previous errors. */
    {
        p_ctrl->transfer_in_progress         = true;
        argument.cmd_53_arg.count            = (count_arg & 0x1ff);
        argument.cmd_53_arg.function_number  = (function & 0x07);
        argument.cmd_53_arg.block_mode       = transfer_mode;
        argument.cmd_53_arg.op_code          = address_mode;
        argument.cmd_53_arg.register_address = (address & 0x1ffff);
        argument.cmd_53_arg.rw_flag          = 1;

        /* Send Write command */
        if (!(SDHI_CommandSend(p_ctrl->hw.channel, command, argument.arg)))
        {
            error = SDMMC_ERROR_COMMAND_ERROR;          /* Command function returned an error. */
        }

        SDHI_ResponseGet(p_ctrl->hw.channel, &response);
    }

    /** Transfer taking place here. */

    if (SDMMC_ERROR_NO_ERROR == error)
    {
        ret_val = SSP_SUCCESS;
    }
    else
    {
        p_ctrl->transfer_in_progress = false;
        dma_transfer_end(p_ctrl);
        ret_val                      = SSP_ERR_WRITE_FAILED;
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Command 52 Read.
 *   @param[in]  channel            Channel of SDHI.
 *   @retval SSP_SUCCESS            Operation completed successfully.
 *   @retval SSP_ERR_READ_FAILED    Operation failed.
 **********************************************************************************************************************/
ssp_err_t SDMMC_CMD52Read (sdmmc_ctrl_t * const p_ctrl,
                           uint8_t  * const     data,
                           uint32_t const       function,
                           uint32_t const       address)
{
    ssp_err_t  ret_val;
    sdio_arg_t argument;

    argument.arg                         = (uint32_t)0;
    argument.cmd_52_arg.function_number  = (function & 0x07);
    argument.cmd_52_arg.rw_flag          = SD_CMD52_READ;                    // read
    argument.cmd_52_arg.raw              = SD_CMD52_NO_READ_AFTER_WRITE;     // read after write
    argument.cmd_52_arg.register_address = (address & 0x1ffff);

    ret_val                              = cmd52(p_ctrl, &argument);

    *data                                = argument.cmd_52_arg.data;

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Command 52 Write.
 *   @param[in]  channel            Channel of SDHI.
 *   @retval SSP_SUCCESS            Operation completed successfully.
 *   @retval SSP_ERR_READ_FAILED    Operation failed.
 **********************************************************************************************************************/
ssp_err_t SDMMC_CMD52Write (sdmmc_ctrl_t * const        p_ctrl,
                            uint8_t  * const            data,
                            uint32_t const              function,
                            uint32_t const              address,
                            sdmmc_io_write_mode_t const read_after_write)
{
    ssp_err_t  ret_val;
    sdio_arg_t argument;

    argument.arg                         = (uint32_t)0;
    argument.cmd_52_arg.function_number  = (function & 0x07);
    argument.cmd_52_arg.rw_flag          = SD_CMD52_WRITE;           // write
    argument.cmd_52_arg.raw              = read_after_write;         // read after write
    argument.cmd_52_arg.register_address = (address & 0x1ffff);
    argument.cmd_52_arg.data             = *data;

    ret_val                              = cmd52(p_ctrl, &argument);

    *data                                = argument.cmd_52_arg.data;

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Command 52 .
 *   @param[in]  channel            Channel of SDHI.
 *   @retval SSP_SUCCESS            Operation completed successfully.
 *   @retval SSP_ERR_READ_FAILED    Operation failed.
 **********************************************************************************************************************/

static ssp_err_t cmd52 (sdmmc_ctrl_t * const p_ctrl, sdio_arg_t * p_arg)
{
    uint16_t      command;
    card_status_t response;
    ssp_err_t     ret_val;
    sdmmc_error_t error;
    error = SDMMC_ERROR_NO_ERROR;             /* Initialize error status. */

    SDHI_EventFlagsClear(p_ctrl->hw.channel); /* Clear event flags before sending command. */

    command = SD_CMD_IO_RW_DIRECT;
    /* Send Read or Write command. */
    if (!(SDHI_CommandSend(p_ctrl->hw.channel, command, p_arg->arg)))
    {
        error = SDMMC_ERROR_COMMAND_ERROR;              /* Return error if command failed. */
    }

    if (SDMMC_ERROR_NO_ERROR == error)
    {
        SDHI_ResponseGet(p_ctrl->hw.channel, &response);

        /* Read after write command? */
        if (p_arg->cmd_52_arg.rw_flag && p_arg->cmd_52_arg.raw)
        {
            /* Put read data into command object data */
            p_arg->cmd_52_arg.data = response.r5.read_write_data;
        }

        /* Read command? */
        if (false == (bool) p_arg->cmd_52_arg.rw_flag)
        {
            /* Put read data into data buffer. */
            p_arg->cmd_52_arg.data = response.r5.read_write_data;
        }
    }

    if (SDMMC_ERROR_NO_ERROR == error)
    {
        ret_val = SSP_SUCCESS;
    }
    else
    {
        ret_val = SSP_ERR_READ_FAILED;
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Initialize card.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Initialization successful.
 *   @retval false          Initialization failed.
 **********************************************************************************************************************/
static bool card_init (sdmmc_ctrl_t * const p_ctrl)
{
    /* Set the clock frequency to minimum. */
    if (!(SDHI_ClockRateSet(p_ctrl->hw.channel, SD_CLK_CTRL_DIV512)))
    {
        return false;
    }

    /* Set initial bus width to one bit wide. */
    SDHI_BusWidthSet(p_ctrl->hw.channel, (uint8_t)1);

    /* Put the card in idle state. */
    SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_GO_IDLE_STATE, 0);

    delay(DELAY_TIME);

    SDIO_reset(p_ctrl);

    /* See if it's an MMC card or an SD card.*/
    if (!card_type_check(p_ctrl))
    {
        return false;                       /* return if operation failed */
    }

    /* Check write protection. */
    write_protect_get(p_ctrl);

    /* Get card identification register, CMD2. */
    if (!cid_get(p_ctrl))
    {
        return false;                       /* return if operation failed */
    }

    /* Get relative card address, CMD3. */
    if (!card_address_get(p_ctrl))
    {
        return false;                       /* return if operation failed */
    }

    /* Select the Card that responded. */
    if (!card_select(p_ctrl))
    {
        return false;                       /* return if operation failed */
    }

    /* Get card status. */
    if (!card_status_get(p_ctrl))
    {
        return false;                       /* return if operation failed */
    }

    /* Set bus width  */
    if (!bus_width_set(p_ctrl))
    {
        return false;                       /* return if operation failed */
    }

    /* Set block length */
    if (!block_length_set(p_ctrl))
    {
        return false;                       /* return if operation failed */
    }

    /* Optimize clock to highest speed allowed by card */
    if (!clock_optimize(p_ctrl))
    {
        return false;                       /* return if operation failed */
    }

    if (true == p_ctrl->status.sdio) /* card identified as SDIO */
    {
        SDHI_SDIO_Mode(p_ctrl->hw.channel, true);
    }
    else
    {
        SDHI_SDIO_Mode(p_ctrl->hw.channel, false);     // SD Cards do not use SDIO mode
    }

    return true;
}

/*******************************************************************************************************************//**
 *   Reset SDIO card.
 *   @param[in] channel     Channel of SDHI.
 *   @retval  none
 **********************************************************************************************************************/

static void SDIO_reset (sdmmc_ctrl_t * const p_ctrl)
{
    uint8_t data;
    data = 0x08;
    SDMMC_CMD52Write(p_ctrl, &data, (uint32_t)0x00, (uint32_t)0x06, SDMMC_IO_WRITE_MODE_NO_READ);
}

/*******************************************************************************************************************//**
 *   Identify media card.
 *   @param[in] channel     Channel of SDHI.
 *   @retval  true          Identified card.
 *   @retval  false         Card not identified or failed to respond.
 **********************************************************************************************************************/
static bool card_type_check (sdmmc_ctrl_t * const p_ctrl)
{
    bool ret_val;

    /* Send MMC commands in attempt to identify card */
    if (true == mmc_card_type_check(p_ctrl))
    {
        p_ctrl->status.card_type = SDMMC_CARD_TYPE_MMC;       /* card identified as MMC card */
        ret_val                  = true;
    }
    else
    {
        /* Assume we have an SD card and try to identify it.  */
        if (true == sd_card_type_check(p_ctrl))
        {
            p_ctrl->status.card_type = SDMMC_CARD_TYPE_SD;   /* card identified as SD card */
            ret_val                  = true;
        }
        else
        {
            ret_val = false; /* failed to identify card */
        }
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Identify SD card.
 *   @param[in] channel     Channel of SDHI.
 *   @retval  true          Identified as SD card.
 *   @retval  false         Not identified as SD card or failed to respond.
 **********************************************************************************************************************/
static bool sd_card_type_check (sdmmc_ctrl_t * const p_ctrl)
{
    uint32_t      i;
    uint32_t      ocr;
    card_status_t response;
    uint32_t      capacity;
    capacity = SD_SC_CAPACITY;
    ocr      = SD_VDD_SUPPORTED;

    /* Send interface condition, version 2.0 command */

    if (SDHI_CommandSend(p_ctrl->hw.channel, (uint16_t)SD_CMD_IF_COND, (uint32_t)(((uint32_t)IF_COND_VOLTAGE << 8) | (uint32_t)IF_COND_CHECK_PATTERN)))
    {
        capacity = SD_HC_CAPACITY;                      /* SDHC cards supported */
        SDHI_ResponseGet(p_ctrl->hw.channel, &response);
        if (((uint32_t)IF_COND_CHECK_PATTERN != response.r7.check_pattern) || ((uint32_t)IF_COND_VOLTAGE != response.r7.voltage_accepted))
        {
            return false;
        }
    }

    /* Check for SDIO capabilities. */
    p_ctrl->status.sdio = false;
    if (SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SDIO, 0x00))
    {
        /* get response of CMD5 (R4) */
        SDHI_ResponseGet(p_ctrl->hw.channel, &response);
        if (response.r4.io_functions)
        {
            for (i = (uint32_t)0; i < (uint32_t)1000; i++)
            {
                delay(NCC_DELAY);
                if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SDIO, ocr)))
                {
                    return false;
                }

                /* get response of CMD5 (R4) */
                SDHI_ResponseGet(p_ctrl->hw.channel, &response);
                if (response.r4.ready)              /* 1 = IO ready */
                {
                    p_ctrl->status.sdio = true;     /* card identified as SDIO */
                    /* Is memory present as well? */
                    if ((uint32_t)0 == response.r4.memory)
                    {
                        return true;                /* card identified, memory not present */
                    }

                    break;
                }
            }
        }
    }

    for (i = (uint32_t)0; i < (uint32_t)1000; i++)
    {
        delay(NCC_DELAY);

        /*/Send App Command - CMD55 */
        if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_APP_CMD, 0)))
        {
            return false;
        }

        ocr  = SD_VDD_SUPPORTED;
        ocr |= capacity;

        /* ACMD41 */
        if (SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_C_ACMD | SD_CMD_SD_SEND_OP_COND, ocr))
        {
            /* get response of ACMD41 (R3) */
            SDHI_ResponseGet(p_ctrl->hw.channel, &response);

            /*  Power up ? */
            if (response.r3.power_up_status)          /* 1 = initialization complete */
            {
                /* check the rest of the response */
                /* High capacity card ? */
                if (response.r3.card_capacity_status) /*  0 = SDSC, 1 = SDHC or SDXC */
                {
                    p_ctrl->status.hc = true;         /*  SDHC or SDXC card */
                }
                else
                {
                    p_ctrl->status.hc = false;        /*  SDSC (standard capacity card) */
                }

                /*  S18A - switching to 1.8 v accepted? */
                if (response.r3.s18A)
                {
                    /* indicate 1.8 v accepted */
                }

                return true;
            }
        }
        else
        {
            return false;
        }
    }

    return false;
}

/*******************************************************************************************************************//**
 *   Identify (e)MMC card.
 *   @param[in] channel     Channel of SDHI.
 *   @retval  true          Identified as (e)MMC card.
 *   @retval  false         Not identified as (e)MMC card or failed to respond.
 **********************************************************************************************************************/
static bool mmc_card_type_check (sdmmc_ctrl_t * const p_ctrl)
{
    uint32_t      i;
    uint32_t      ocr;
    card_status_t response;
    uint32_t      capacity;
    capacity = SD_HC_CAPACITY;                      /* SDHC cards supported */

    for (i = (uint32_t)0; i < (uint32_t)100; i++)
    {
        delay(MMC_INIT_DELAY_TIME);

        /*  Format and send cmd: Volt. window is usually 0x00300000 (3.4-3.2v) */
        ocr  = SD_VDD_SUPPORTED;
        ocr |= capacity;

        if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SEND_OP_COND, ocr)))
        {
            return false; /* SD cards will not respond to CMD1  */
        }

        /* get response of CMD1  */
        SDHI_ResponseGet(p_ctrl->hw.channel, &response);

        /*  Power up ? */
        if (response.r3.power_up_status)          /* 1 = initialization complete */
        {
            if (response.r3.card_capacity_status) /*  0 = SDSC, 1 = SDHC or SDXC */
            {
                p_ctrl->status.hc = true;         /*  SDHC or SDXC card */
            }
            else
            {
                p_ctrl->status.hc = false;        /*  SDSC (standard capacity card) */
            }

            return true;
        }
    }

    return false;
}

/*******************************************************************************************************************//**
 *   Check card write protection.
 *   @param[in] channel     Channel of SDHI.
 *   @retval  true          Card is write protected.
 *   @retval  false         Card is not write protected.
 **********************************************************************************************************************/
static void write_protect_get (sdmmc_ctrl_t * const p_ctrl)
{
/* Check for write protection */
    p_ctrl->status.write_protected = false;              /* initialize, Card is not write protected */

    if (SDMMC_MEDIA_TYPE_CARD ==  p_ctrl->hw.media_type) /* Pin is only used on cards. */
    {
        if (SDHI_WriteProtectGet(p_ctrl->hw.channel))
        {
            p_ctrl->status.write_protected = true;       /* Card is write protected */
        }
    }
}

/*******************************************************************************************************************//**
 *   Set SDHI clock to fastest allowable speed for card.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Success.
 *   @retval false          Failed because media did not return CSD or the clock rate could not be set low enough.
 **********************************************************************************************************************/
static bool clock_optimize (sdmmc_ctrl_t * const p_ctrl)
{
    uint32_t rate;

    if (p_ctrl->status.sdio != true) /* card not identified as SDIO */
    {
        if (!csd_get(p_ctrl))        /* Get the Card Specific Data. */
        {
            return false;
        }
    }
    else
    {
        SDIO_clock_optimize(p_ctrl);
    }

    if ((SDMMC_CARD_TYPE_MMC == p_ctrl->status.card_type) && (p_ctrl->status.csd_version >= CSD_VERSION_EXT))
    {
        /* Ask eMMC to switch to high speed */
        if (!(SDHI_CommandSend(p_ctrl->hw.channel, MMC_CMD_SWITCH_WBUSY, EMMC_HIGH_SPEED_MODE)))
        {
            return false;
        }

        csd_extended_get(p_ctrl);                                   // read back extended csd
        p_ctrl->status.max_clock_rate = EMMC_HIGH_SPEED_CLOCK_RATE; // Set clock rate to 52 MHz maximum
    }
    else if ((SDMMC_CARD_TYPE_SD == p_ctrl->status.card_type) && (p_ctrl->status.csd_version >= SD_VERSION_1_10))
    {
        /* Ask SD card to switch to high speed */
        if (sd_high_speed(p_ctrl, SD_HIGH_SPEED_MODE_CHECK))
        {
            if (sd_high_speed(p_ctrl, SD_HIGH_SPEED_MODE_SWITCH))
            {
                p_ctrl->status.max_clock_rate = SD_HIGH_SPEED_CLOCK_RATE;   // Set clock rate to 50 MHz maximum
            }
        }
    }

    rate = p_ctrl->status.max_clock_rate;
    if (!SDHI_MaxClockFreqSet(p_ctrl->hw.channel, &rate))
    {
        return false;
    }

    p_ctrl->status.clock_rate = rate;
    return true;
}

/*******************************************************************************************************************//**
 *   SDIO high speed.
 *   @param[in] channel     Channel of SDHI.
 *   @retval  none
 **********************************************************************************************************************/

static void SDIO_clock_optimize (sdmmc_ctrl_t * const p_ctrl)
{
    ssp_err_t ret_val;
    uint8_t   data;

    p_ctrl->status.max_clock_rate = SD_CLOCK_RATE;          // Non High Speed clock rate is 25 MHz.

    ret_val                       =     SDMMC_CMD52Read(p_ctrl, &data, (uint32_t)0x00, (uint32_t)0x13);
    if (SSP_SUCCESS == ret_val)
    {
        /* High -Speed supported? */
        if (data & 0x01)
        {
            data    = 0x02;
            ret_val = SDMMC_CMD52Write(p_ctrl, &data, (uint32_t)0x00, (uint32_t)0x13, SDMMC_IO_WRITE_READ_AFTER_WRITE);

            if (SSP_SUCCESS == ret_val)
            {
                /* write successful? */
                if (data & 0x02)
                {
                    p_ctrl->status.max_clock_rate = SDIO_HIGH_SPEED_CLOCK_RATE;      // Card is in High-Speed mode.
                }
            }
        }
    }
}

/*******************************************************************************************************************//**
 *   SD high speed check.
 *   @param[in] channel     Channel of SDHI.
 *   @retval  none
 **********************************************************************************************************************/

static bool sd_high_speed (sdmmc_ctrl_t * const p_ctrl, uint32_t mode)
{
    uint8_t  p_data[SWITCH_STATUS_BUFFER_SIZE];
    uint32_t index;
    bool     ret_val;

    ret_val = true;

    if (!(data_pre_read(p_ctrl, SD_CMD_SWITCH, mode, (uint32_t)SWITCH_STATUS_SIZE)))
    {
        ret_val = false;
    }

    index = (uint32_t)0x00;
    if (!(data_block_read(p_ctrl, p_data, &index, (uint32_t)(SWITCH_STATUS_BUFFER_SIZE))))    /* read data block from
                                                                                   * response */
    {
        ret_val = false;
    }
    if (true == ret_val)
    {
        /* Check for error response to High speed Function. */
        if (SWITCH_HIGH_SPEED_ERROR == (p_data[SWITCH_HIGH_SPEED_ERROR_RESPONSE] & SWITCH_HIGH_SPEED_ERROR))
        {
            ret_val = false;
        }
        else if (SWITCH_HIGH_SPEED_OK == (p_data[SWITCH_HIGH_SPEED_RESPONSE] & SWITCH_HIGH_SPEED_OK))
        {
            ret_val = true;
        }
        else
        {
            ret_val = false;
        }
    }
    /* Attempt to read the rest of the block even if test failed. */
    index = (uint32_t)0x00;
    if (!(data_block_read(p_ctrl, p_data, &index, SWITCH_STATUS_BUFFER_SIZE)))    /* read the rest of the data */
    {
        ret_val = false;
    }

    if (!(data_post_access(p_ctrl)))
    {
        ret_val = false;
    }
    return ret_val;
}

/*******************************************************************************************************************//**
 *   Save Card Specific Data from SD card , version 1, 2, etc..
 *   @param[in] channel     Channel of SDHI.
 *   @param[in] p_csd_reg   Pointer to csd data to save.
 *   @retval    none
 **********************************************************************************************************************/
static void csd_save (sdmmc_ctrl_t * const p_ctrl, csd_reg_t * p_csd_reg)
{
    uint32_t unit;
    uint32_t rate;
    uint32_t mult;
    uint32_t write_sector_size;
    uint32_t read_sector_size;
    uint32_t sector_multiply;

    sector_multiply            = 1;

    p_ctrl->status.csd_version = (uint8_t)p_csd_reg->csd_v1_b.csd_structure;        /* Save csd version  */

    /* Calculate the maximum SD data transfer rate */
    unit                              = g_unit_value_tens[(p_csd_reg->csd_v1_b.tran_speed & 0x03)];

    rate                              =  g_time_value_tenths[((p_csd_reg->csd_v1_b.tran_speed >> 3) & 0x0f)];
    rate                             *= unit;

    p_ctrl->status.max_clock_rate     = rate;

    p_ctrl->status.erase_sector_count = (uint32_t) (p_csd_reg->csd_v1_b.sector_size + 1); /* v1 and v2 are the same for
                                                                                           * this
                                                                                           * field */
    write_sector_size                 = (uint32_t) ((uint32_t)1 << p_csd_reg->csd_v1_b.write_bl_len);
    read_sector_size                  = (uint32_t) ((uint32_t)1 << p_csd_reg->csd_v1_b.read_bl_len);
    if (write_sector_size < read_sector_size)
    {
        if (write_sector_size != (uint32_t)0)                          /* Avoid divide by zero */
        {
            sector_multiply = (read_sector_size / write_sector_size);
        }
        else
        {
            write_sector_size =  SDMMC_DFLT_BLKSIZE;
        }
    }

    if (write_sector_size > SDMMC_DFLT_BLKSIZE)
    {
        sector_multiply *= (write_sector_size / SDMMC_DFLT_BLKSIZE);
    }

    p_ctrl->status.sector_size = SDMMC_DFLT_BLKSIZE;

    if ((uint8_t)CSD_VERSION_1_0 == p_ctrl->status.csd_version)
    {
        mult                        =  (uint32_t) (1 << (p_csd_reg->csd_v1_b.c_size_mult + 2));
        p_ctrl->status.sector_count =  (uint32_t) ((uint32_t) (p_csd_reg->csd_v1_b.c_size + 1) * mult);
    }
    else if ((uint8_t)CSD_VERSION_2_0 == p_ctrl->status.csd_version)
    {
        mult                        = CSD_VERSION_2_0_MULT;
        p_ctrl->status.sector_count =
            (uint32_t) (((uint64_t) (p_csd_reg->csd_v2_b.c_size + 1) * mult) / p_ctrl->status.sector_size);
    }
    else if (CSD_VERSION_EXT == p_ctrl->status.csd_version)
    {
        mult                        = (uint32_t) (1 << (p_csd_reg->csd_v1_b.c_size_mult + 2));
        p_ctrl->status.sector_count = (uint32_t) ((uint32_t) (p_csd_reg->csd_v1_b.c_size + 1) * mult);
    }
    else
    {
        /* comment for code rules compliance */
    }

    p_ctrl->status.sector_count *= sector_multiply;
}

/*******************************************************************************************************************//**
 *   Get Card Specific Data Extended, decode response and put CSD in data structure
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Success.
 *   @retval false          Media failed to return extended CSD info.
 **********************************************************************************************************************/
static bool csd_extended_get (sdmmc_ctrl_t * const p_ctrl)
{
    uint8_t  p_data[EXT_CSD_BUFFER_SIZE];
    uint32_t index;
    uint32_t i;
    index = (uint32_t)0;

    /* Ask card to send extended CSD */
    if (!(data_pre_read(p_ctrl, MMC_CMD_SEND_EXT_CSD, p_ctrl->status.sdhi_rca << 16, EXT_CSD_SIZE)))
    {
        return false;
    }

    for (i = (uint32_t)0; i < (uint32_t)EXT_CSD_READ_BUFFER_ITERATIONS; i++)   // Must read all 16 words to end successfully.
    {
        index = (uint32_t)0;
        if (!(data_block_read(p_ctrl, p_data, &index, sizeof p_data)))        /* read data block from
                                                                               * response */
        {
            return false;
        }
        else
        {
            if (EXT_CSD_BUS_WIDTH_ITERATION == i)                                                        /* use the
                                                                                                          * right
                                                                                                          * iteration to
                                                                                                          * get
                                                                                                          * bus width */
            {
                p_ctrl->status.bus_width = (sdmmc_bus_width_t) ((uint8_t)1 << ((p_data[EXT_CSD_BUS_WIDTH] + (uint8_t)1)));
                                                                                                         // Byte 183 is
                                                                                                         // bus_width.
            }

            if (EXT_CSD_HS_TIMING_ITERATION == i)                         /* use the right iteration to get timing */
            {
                p_ctrl->status.hs_timing = p_data[EXT_CSD_HS_TIMING];     /* Byte 185 is HS_TIMING. */
            }

            if (EXT_CSD_REVISION_ITERATION == i)                          /* use the right iteration to get Revision */
            {
                p_ctrl->status.csd_version = p_data[EXT_CSD_REVISION];    /* Byte 192 is Extended CSD rev. */
            }

            if (EXT_CSD_DEVICE_TYPE_ITERATION == i)                       /* use the right iteration to get device type */
            {
                p_ctrl->status.device_type = p_data[EXT_CSD_DEVICE_TYPE]; /* Byte 196 is Device type, High_speed, DDR */
            }
        }
    }

    if (!(data_post_access(p_ctrl)))
    {
        return false;
    }
    /* Check for error response to High speed Function. */
    if (SWITCH_HIGH_SPEED_ERROR == (p_data[SWITCH_HIGH_SPEED_ERROR_RESPONSE] & SWITCH_HIGH_SPEED_ERROR))
    {
        return false;
    }
    else if (SWITCH_HIGH_SPEED_OK == (p_data[SWITCH_HIGH_SPEED_RESPONSE] & SWITCH_HIGH_SPEED_OK))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*******************************************************************************************************************//**
 *   Get SD Card Configuration Register.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Success.
 *   @retval false          Media failed to return SCR register.
 **********************************************************************************************************************/
static bool scr_get (sdmmc_ctrl_t * const p_ctrl)
{
    scr_reg_t scr;
    uint32_t  read_word;

    /* send SD CMD55, app command */
    if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_APP_CMD, p_ctrl->status.sdhi_rca << 16)))
    {
        /* SD_CMD_APP_CMD failed  */
        return false;
    }

    /* Ask card to send SCR */
    if (!(data_pre_read(p_ctrl, SD_CMD_SEND_SCR, STUFF_BITS, SCR_SIZE)))
    {
        return false;
    }

    if (!(SDHI_BitEventErrorGet(p_ctrl->hw.channel)))
    {
        SDHI_DataRead(p_ctrl->hw.channel, &read_word);                        /* read word from bus */
    }
    else
    {
        return false;
    }

    if (!(SDHI_BitEventErrorGet(p_ctrl->hw.channel)))
    {
        SDHI_DataRead(p_ctrl->hw.channel, &scr.word[1]);                     /* read word from bus */
    }
    else
    {
        return false;
    }

    scr.word[0]                = read_word;
    p_ctrl->status.bus_width   = (sdmmc_bus_width_t) scr.scr_bit.sd_bus_widths; /* get bus_width supported. */
    p_ctrl->status.csd_version = (uint8_t)scr.scr_bit.sd_spec;                           /* SD rev. */
    if (((uint8_t)SD_VERSION_2_00 == p_ctrl->status.csd_version) && ((uint32_t)1 == scr.scr_bit.sd_spec3))
    {
        p_ctrl->status.csd_version = SD_VERSION_3_0X;
    }

    if (!(data_post_access(p_ctrl)))
    {
        return false;
    }

    return true;
}

/*******************************************************************************************************************//**
 *   Set up to read register data.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Success.
 *   @retval false          Command failed.
 **********************************************************************************************************************/

static bool data_pre_read (sdmmc_ctrl_t * const p_ctrl, uint16_t command, uint32_t argument, uint32_t byte_count)
{
    card_status_t     response;
    volatile uint32_t timer;

    SDHI_EventFlagsClear(p_ctrl->hw.channel);           /* Clear event flags before sending command. */

    /* Set block size */
    SDHI_BlockSizeSet(p_ctrl->hw.channel, (uint16_t) byte_count);

    if (!(SDHI_CommandSend(p_ctrl->hw.channel, command, argument)))
    {
        return false;
    }

    SDHI_ResponseGet(p_ctrl->hw.channel, &response);

    /* Wait for the read buffer to fill up. */
    timeout_set(&timer, DATA_TIMEOUT);
    do
    {
        if (SDHI_BitEventErrorGet(p_ctrl->hw.channel))
        {
            return false;
        }
    } while ((!(timeout_get(&timer))) &&
             ((!(SDHI_ReadBufferReadyGet(p_ctrl->hw.channel)))));

    if (SDHI_ReadBufferReadyGet(p_ctrl->hw.channel))
    {
        SDHI_ReadBufferReadyClear(p_ctrl->hw.channel);
        return true;
    }

    return false;
}

/*******************************************************************************************************************//**
 *   Process end of data read or write.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Success.
 *   @retval false          End of transfer failed.
 **********************************************************************************************************************/

static bool data_post_access (sdmmc_ctrl_t * const p_ctrl)
{
    volatile uint32_t timer;

    /* check for access end */
    timeout_set(&timer, ACCESS_TIMEOUT);
    do
    {
        if (SDHI_BitEventErrorGet(p_ctrl->hw.channel))
        {
            return false;
        }
    } while ((!(timeout_get(&timer))) && (!(SDHI_AccessEndGet(p_ctrl->hw.channel))));

    if (0 == timer)
    {
        return false;
    }

    if (SDHI_AccessEndGet(p_ctrl->hw.channel))
    {
        SDHI_AccessEndClear(p_ctrl->hw.channel);
    }
    else
    {
        return false;
    }

    /* end of transfer data */
    if (!SDHI_EndProcessData(p_ctrl->hw.channel))
    {
        return false;
    }

    return true;
}

/*******************************************************************************************************************//**
 *   Get Card Specific Data, decode response and put CSD in data structure
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Success.
 *   @retval false          Media failed to return CSD info.
 **********************************************************************************************************************/
static bool csd_get (sdmmc_ctrl_t * const p_ctrl)
{
    /* send CMD9  to get CSD */
    csd_reg_t csd_reg;

    /* clear extended csd data */
    p_ctrl->status.bus_width   = (sdmmc_bus_width_t) 0; // clear bus width
    p_ctrl->status.hs_timing   = (uint8_t)0;                     // clear timing
    p_ctrl->status.csd_version = (uint8_t)0;                     // clear version
    p_ctrl->status.device_type = (uint8_t)0;                     // clear type

    /*  Ask card to send CSD */
    if (!(csd_send(p_ctrl, &csd_reg)))
    {
        return false; /* read CSD failed */
    }

    /* decode csd register depending on version of card */
    csd_save(p_ctrl, &csd_reg);
    if ((SDMMC_CARD_TYPE_MMC == p_ctrl->status.card_type) && (CSD_VERSION_EXT == p_ctrl->status.csd_version))
    {
        csd_extended_get(p_ctrl);
    }
    else if (SDMMC_CARD_TYPE_SD == p_ctrl->status.card_type)
    {
        scr_get(p_ctrl);
    }

    return true;
}

/*******************************************************************************************************************//**
 *   Get Card ID, decode response and put CID in data structure.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Success.
 *   @retval false          Media failed to return ID.
 **********************************************************************************************************************/
static bool cid_get (sdmmc_ctrl_t * const p_ctrl)
{
    csd_reg_t cid_reg;
    if (p_ctrl->status.sdio != true)                                         /* card identified as SDIO */
    {
        if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_ALL_SEND_CID, 0))) /* send SD CMD2 */
        {
            /*  SDHI_ALL_SEND_CID failed. */
            return false;
        }

        SDHI_R2ResponseGet(p_ctrl->hw.channel, &cid_reg);
    }

    return true;
}

/*******************************************************************************************************************//**
 *   Get relative card address, decode response and put in data structure.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Card responded with address.
 *   @retval false          Failed to get card address.
 **********************************************************************************************************************/
static bool card_address_get (sdmmc_ctrl_t * const p_ctrl)
{
    card_status_t response;
    uint32_t      rca;
    rca = (uint32_t) ((p_ctrl->hw.channel + 2) << 16);

    if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SEND_RELATIVE_ADDR, rca))) /* send SD CMD3 */
    {
        /* SD: SD_SEND_RELATIVE_ADDR failed. */
        return false;
    }

    SDHI_ResponseGet(p_ctrl->hw.channel, &response); /* get card address */
    if (SDMMC_CARD_TYPE_SD == p_ctrl->status.card_type)
    {
        /* card identified as SD card */
        p_ctrl->status.sdhi_rca = (response.r6.rca); /* save card address */
    }
    else
    {
        p_ctrl->status.sdhi_rca = (rca >> 16); /* save card address */
    }

    return true;
}

/*******************************************************************************************************************//**
 *   Select card by address.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Card responded to command.
 *   @retval false          Operation failed.
 **********************************************************************************************************************/
static bool card_select (sdmmc_ctrl_t * const p_ctrl)
{
    card_status_t response;

    /* send SD CMD7 */
    if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SEL_DES_CARD, p_ctrl->status.sdhi_rca << 16)))
    {
        /* SD: SD_SEL_DES_CARD failed */
        return false;
    }

    SDHI_ResponseGet(p_ctrl->hw.channel, &response); /* get card response */

    return true;
}

/*******************************************************************************************************************//**
 *   Get card status.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Card returned status.
 *   @retval false          Card did not respond to command.
 **********************************************************************************************************************/
static bool card_status_get (sdmmc_ctrl_t * const p_ctrl)
{
    card_status_t response;
    if (p_ctrl->status.sdio != true)          /* card identified as SDIO */
    {
        if (p_ctrl->status.sdhi_rca > (uint32_t)0)
        {
            /* send SD CMD13, send status */
            if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SEND_STATUS, p_ctrl->status.sdhi_rca << 16)))
            {
                /* SDHI_SEND_STATUS failed. */
                return false;
            }

            SDHI_ResponseGet(p_ctrl->hw.channel, &response); /* get card response */
            return true;
        }

        return false;
    }

    return true;
}

/*******************************************************************************************************************//**
 *   Set bus width.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Bus width successfully set.
 *   @retval false          Operation failed.
 **********************************************************************************************************************/
static bool bus_width_set (sdmmc_ctrl_t * const p_ctrl)
{
    uint32_t i;
    uint8_t  bus_width;
    uint8_t  bus_width_setting;
    uint8_t  capability;

    bus_width = p_ctrl->hw.bus_width;

    if (p_ctrl->status.sdio != true)                         /* card not identified as SDIO? */
    {
        if (SDMMC_CARD_TYPE_MMC == p_ctrl->status.card_type) /* card identified as MMC card? */
        {
            bus_width_setting = ((bus_width >> 2) & 0x03);
        }
        else
        {
            bus_width_setting = ((bus_width >> 1) & 0x03);
        }

        for (i = (uint32_t)0; i < SDHI_TIMEOUT; i++)
        {
            if (SDMMC_CARD_TYPE_MMC == p_ctrl->status.card_type) /* card identified as MMC card? */
            {
                if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SWITCH,
                                       (uint32_t) ((0x1 << ACCESS) | (183 << INDEX) | (bus_width_setting << VALUE)))))
                {
                    /* SD_CMD_SWITCH failed  */
                    return false;
                }

                SDHI_BusWidthSet(p_ctrl->hw.channel, bus_width);
                return true;
            }
            else                                                    /* card identified as SD card */
            {
                /* send SD CMD55, app command */
                if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_APP_CMD, p_ctrl->status.sdhi_rca << 16)))
                {
                    /* SD_CMD_APP_CMD failed  */
                    return false;
                }

                if (SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_C_ACMD | SD_CMD_SET_BUS_WIDTH, bus_width_setting))
                {
                    SDHI_BusWidthSet(p_ctrl->hw.channel, bus_width);
                    return true;
                }
            }

            delay(DELAY_TIME);
        }
    }
    else                         /* SDIO type */
    {
        capability = SDIO_capability_get(p_ctrl);

        if (!(capability & 0x40))             /* Not low speed card? */
        {
            bus_width_setting = ((bus_width >> 1) & 0x03);
            if (SDIO_bus_width_set(p_ctrl, bus_width_setting))
            {
                SDHI_BusWidthSet(p_ctrl->hw.channel, bus_width);
                return true;
            }
        }
    }

    return false;
}

/*******************************************************************************************************************//**
 *   SDIO bus width set.
 *   @param[in] channel     Channel of SDHI.
 *   @retval  none
 **********************************************************************************************************************/

static bool SDIO_bus_width_set (sdmmc_ctrl_t * const p_ctrl, uint8_t width)
{
    ssp_err_t ret_val;
    uint8_t   data;

    data    = width;
    ret_val = SDMMC_CMD52Write(p_ctrl, &data, (uint32_t)0x00, (uint32_t)0x07, SDMMC_IO_WRITE_READ_AFTER_WRITE);

    if (SSP_SUCCESS == ret_val)
    {
        if (data == width)
        {
            return true;
        }
    }

    return false;
}

/*******************************************************************************************************************//**
 *   SDIO capability.
 *   @param[in] channel     Channel of SDHI.
 *   @retval  none
 **********************************************************************************************************************/

static uint8_t SDIO_capability_get (sdmmc_ctrl_t * const p_ctrl)
{
    ssp_err_t ret_val;
    uint8_t   data;

    ret_val = SDMMC_CMD52Read(p_ctrl, &data, (uint32_t)0x00, (uint32_t)0x08);
    if (SSP_SUCCESS == ret_val)
    {
        /* High -Speed supported? */
        return data;
    }

    return 0;
}

/*******************************************************************************************************************//**
 *   Set block length.
 *   @param[in] channel     Channel of SDHI.
 *   @retval true           Operation succeeded.
 *   @retval false          Operation failed.
 **********************************************************************************************************************/
static bool block_length_set (sdmmc_ctrl_t * const p_ctrl)
{
    if (p_ctrl->status.sdio != true)          /* card identified as SDIO */
    {
        if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SET_BLOCKLEN, SDMMC_DFLT_BLKSIZE)))
        {
            /*  SD_CMD_SET_BLOCKLEN failed. */
            return false;
        }
    }

    return true;
}

/*******************************************************************************************************************//**
 *   Send CSD command and get R2 response.
 *   @param[in]     channel     Channel of SDHI.
 *   @param[out]    p_csd_reg   Buffer to receive response.
 *   @retval true               CSD was received.
 *   @retval false              No response received.
 **********************************************************************************************************************/
static bool csd_send (sdmmc_ctrl_t * const p_ctrl, csd_reg_t * p_csd_reg)
{
    /* Put the card to standby state */
    SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SEL_DES_CARD, 0);

    delay(DELAY_TIME);

    /* Ask card to send CSD */
    if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SEND_CSD, p_ctrl->status.sdhi_rca << 16)))
    {
        return false;
    }

    SDHI_R2ResponseGet(p_ctrl->hw.channel, p_csd_reg);

    /* Put the card back to data transfer mode */
    if (!(SDHI_CommandSend(p_ctrl->hw.channel, SD_CMD_SEL_DES_CARD, p_ctrl->status.sdhi_rca << 16)))
    {
        return false;
    }

    delay(DELAY_TIME);

    return true;
}

/*******************************************************************************************************************//**
 *   Read data block.
 *   @param[in]      channel     Channel of SDHI.
 *   @param[out]     p_data      Pointer to data read from media.
 *   @param[in,out]  index       Offset into data buffer.
 *   @retval true                Successfully read data.
 *   @retval false               Data not available.
 **********************************************************************************************************************/
static bool data_block_read (sdmmc_ctrl_t * const p_ctrl, uint8_t * p_data, uint32_t * index, uint32_t byte_count)
{
    uint32_t i;
    uint32_t read_data;
    bool     ret_val;
    ret_val = false;

    if (false == ret_val)
    {
        ret_val = true;
        for (i = (uint32_t)0; (i < (byte_count / 4)); i++)
        {
            if (!(SDHI_BitEventErrorGet(p_ctrl->hw.channel)))
            {
                SDHI_DataRead(p_ctrl->hw.channel, &read_data);                     /* read word from bus */
                /* store 32 bit word as 4 bytes */
                p_data[(*index)++] = (uint8_t) (read_data & 0xff);
                p_data[(*index)++] = (uint8_t) ((read_data >> 8) & 0xff);
                p_data[(*index)++] = (uint8_t) ((read_data >> 16) & 0xff);
                p_data[(*index)++] = (uint8_t) ((read_data >> 24) & 0xff);
            }
            else
            {
                ret_val = false;
            }
        }
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Set up transfer to read from media.
 *   @param[in]     p_ctrl         Handle to channel.
 *   @param[in]     block_count    Number of blocks to transfer.
 *   @param[in]     bytes          Bytes per block.
 *   @param[in]     p_data         Pointer to data to read from media.
 *   @retval true                  Transfer successfully setup.
 *   @retval false                 Transfer setup failed.
 **********************************************************************************************************************/
static bool dma_transfer_read (sdmmc_ctrl_t * const p_ctrl, uint32_t block_count, uint32_t bytes, uint8_t * p_data)
{
    bool           ret_val;
    ssp_err_t      ssp_ret_val;
    transfer_cfg_t cfg;
    ret_val = false;

    /* Check pointer for NULL, transfer function is optional. */
    if (NULL != p_ctrl->p_lower_lvl_transfer)
    {
        if (bytes < SDMMC_DFLT_BLKSIZE)
        {
            bytes+=3;
        }
        bytes /= 4;                 /* Four bytes written at a time. */

        cfg.p_info = p_ctrl->p_lower_lvl_transfer->p_cfg->p_info;

        if (SDHI_DmaModeSet(p_ctrl->hw.channel, true) == true)
        {
            cfg.p_callback             = p_ctrl->p_lower_lvl_transfer->p_cfg->p_callback;

            cfg.p_info->p_src          = HW_SDMMC_DataBufferAddressGet(p_ctrl->hw.channel);  /* Define source of
                                                                                              * data. */
            cfg.p_info->p_dest         = p_data;                                             /* Define
                                                                                              * destination for
                                                                                              * data. */
            cfg.p_info->num_blocks     = (uint16_t) block_count;                             /* Set up number of
                                                                                              * data blocks. */
            cfg.p_info->src_addr_mode  = TRANSFER_ADDR_MODE_FIXED;                           /* Source address
                                                                                              * stays the same.
                                                                                              **/
            cfg.p_info->dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;                     /* Destination
                                                                                              * address
                                                                                              * increments
                                                                                              * after each
                                                                                              * transfer block.
                                                                                              **/
            cfg.p_info->size           = TRANSFER_SIZE_4_BYTE;                               /* Address
                                                                                              * incremented by
                                                                                              * this size after
                                                                                              * each word. */
            cfg.p_info->mode           = TRANSFER_MODE_BLOCK;                                /* Data is
                                                                                              * transferred in
                                                                                              * bytes */
            cfg.p_info->length         = (uint16_t) bytes;                                   /* of this length.
                                                                                             **/
            cfg.p_info->repeat_area    = TRANSFER_REPEAT_AREA_SOURCE;                        /* Destination keeps
                                                                                              * incrementing for
                                                                                              * each block. */
            cfg.p_info->chain_mode     = TRANSFER_CHAIN_MODE_DISABLED;                       /* Chain mode not
                                                                                              * needed. */
            cfg.activation_source      = HW_SDMMC_DmaEventGet(p_ctrl->hw.channel);           /* Transfer is
                                                                                              * triggered by this
                                                                                              * elc event. */

            /* Open the transfer channel. */
            ssp_ret_val =
                p_ctrl->p_lower_lvl_transfer->p_api->open(p_ctrl->p_lower_lvl_transfer->p_ctrl, &cfg);

            /* Set transfer to begin when triggered by the DMA event. */
            if (SSP_SUCCESS == ssp_ret_val)
            {
                ssp_ret_val = p_ctrl->p_lower_lvl_transfer->p_api->reset(
                    p_ctrl->p_lower_lvl_transfer->p_ctrl,
                    NULL,
                    NULL,
                    (uint16_t) block_count);
            }

            if (SSP_SUCCESS == ssp_ret_val)
            {
                ret_val = true;
            }
        }
    }

    /* If DMA or DTC transfer is not available, just transfer by MCU. */
    if (false == ret_val)
    {
        SDHI_DmaModeSet(p_ctrl->hw.channel, false);
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Set up transfer to write to media.
 *   @param[in]     p_ctrl         Handle to channel.
 *   @param[in]     block_count    Number of blocks to transfer.
 *   @param[in]     bytes          Bytes per block.
 *   @param[in]     p_data         Pointer to data to write to media.
 *   @retval true                  Transfer successfully setup.
 *   @retval false                 Transfer setup failed.
 **********************************************************************************************************************/
static bool dma_transfer_write (sdmmc_ctrl_t * const p_ctrl,
                                uint32_t             block_count,
                                uint32_t             bytes,
                                const uint8_t      * p_data)
{
    bool           ret_val;
    ssp_err_t      ssp_ret_val;
    transfer_cfg_t cfg;
    ret_val = false;

    /* Check pointer for NULL, transfer function is optional. */
    if (NULL != p_ctrl->p_lower_lvl_transfer)
    {
        if (bytes < SDMMC_DFLT_BLKSIZE)
        {
            bytes+=3;
        }
        bytes /= 4;             /* Four bytes written at a time. */

        cfg.p_callback = p_ctrl->p_lower_lvl_transfer->p_cfg->p_callback;

        cfg.p_info     = p_ctrl->p_lower_lvl_transfer->p_cfg->p_info;

        if (SDHI_DmaModeSet(p_ctrl->hw.channel, true) == true)
        {
            cfg.p_info->p_src          = p_data;                                             /* Define source of
                                                                                              * data. */
            cfg.p_info->p_dest         =  HW_SDMMC_DataBufferAddressGet(p_ctrl->hw.channel); /* Define
                                                                                              * destination for
                                                                                              * data. */
            cfg.p_info->num_blocks     = (uint16_t) block_count;                             /* Set up number of
                                                                                              * data blocks. */
            cfg.p_info->src_addr_mode  = TRANSFER_ADDR_MODE_INCREMENTED;                     /* Source address
                                                                                              * increments after
                                                                                              * each transfer
                                                                                              * block. */
            cfg.p_info->dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;                           /* Destination
                                                                                              * address stays the
                                                                                              * same. */
            cfg.p_info->size           = TRANSFER_SIZE_4_BYTE;                               /* Address
                                                                                              * incremented by
                                                                                              * this size after
                                                                                              * each word. */
            cfg.p_info->mode           = TRANSFER_MODE_BLOCK;                                /* Data is
                                                                                              * transferred in
                                                                                              * blocks. */
            cfg.p_info->length         = (uint16_t) bytes;                                   /* of this length.
                                                                                             **/
            cfg.p_info->repeat_area    = TRANSFER_REPEAT_AREA_DESTINATION;                   /* Source keeps
                                                                                              * incrementing for
                                                                                              * each block. */
            cfg.p_info->chain_mode     = TRANSFER_CHAIN_MODE_DISABLED;                       /* Chain mode not
                                                                                              * needed. */
            cfg.activation_source      = HW_SDMMC_DmaEventGet(p_ctrl->hw.channel);           /* Transfer is
                                                                                              * triggered by this
                                                                                              * elc event. */

            /* Open the transfer channel. */
            ssp_ret_val =
                p_ctrl->p_lower_lvl_transfer->p_api->open(p_ctrl->p_lower_lvl_transfer->p_ctrl, &cfg);

            /* Set transfer to begin when triggered by the DMA event. */
            if (SSP_SUCCESS == ssp_ret_val)
            {
                ssp_ret_val = p_ctrl->p_lower_lvl_transfer->p_api->reset(
                    p_ctrl->p_lower_lvl_transfer->p_ctrl,
                    NULL,
                    NULL,
                    (uint16_t) block_count);
            }

            if (SSP_SUCCESS == ssp_ret_val)
            {
                ret_val = true;
            }
        }
    }

    /* If DMA or DTC transfer is not available, just transfer by MCU. */
    if (false == ret_val)
    {
        SDHI_DmaModeSet(p_ctrl->hw.channel, false);
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Close transfer Function and clear trigger.
 *   @param[in]     p_ctrl         Handle to channel.
 *   @retval true                  Transfer end successfully setup.
 *   @retval false                 Transfer end setup failed.
 **********************************************************************************************************************/
static bool dma_transfer_end (sdmmc_ctrl_t * const p_ctrl)
{
    bool      ret_val;
    ssp_err_t ssp_ret_val;
    ret_val = false;

    /* Close the transfer channel. */
    ssp_ret_val = p_ctrl->p_lower_lvl_transfer->p_api->close(p_ctrl->p_lower_lvl_transfer->p_ctrl);
    if (SSP_SUCCESS == ssp_ret_val)
    {
        ret_val = true;
    }

    SDHI_DmaModeSet(p_ctrl->hw.channel, false);
    SDHI_DmaTriggerClear(p_ctrl->hw.channel);
    return ret_val;
}

/*******************************************************************************************************************//**
 *   Set a timeout value.
 *   @param[in] p_timer     Pointer to a timer.
 *   @param[in] timer_value Value to set timer to.
 *   @retval none
 **********************************************************************************************************************/
static void timeout_set (volatile uint32_t * p_timer, uint32_t timer_value)
{
    *p_timer = timer_value;
}

/*******************************************************************************************************************//**
 *   Get a timeout value.
 *   @param[in] p_timer     Pointer to a timer.
 *   @retval true           Timeout reached.
 *   @retval false          Timeout not reached.
 *
 **********************************************************************************************************************/
static bool timeout_get (volatile uint32_t * p_timer)
{
    R_BSP_SoftwareDelay((uint32_t)1, BSP_DELAY_UNITS_MICROSECONDS);
    return (0 == (--(*(p_timer))));
}

/*******************************************************************************************************************//**
 *   Delay
 *   @param[in] time    Count to delay.
 *   @retval none
 **********************************************************************************************************************/
static void delay (uint32_t time)
{
    R_BSP_SoftwareDelay(time, BSP_DELAY_UNITS_MICROSECONDS);
}
