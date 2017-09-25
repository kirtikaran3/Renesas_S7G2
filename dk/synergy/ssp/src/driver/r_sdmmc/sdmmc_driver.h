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
 * File Name    : sdmmc_driver.h
 * Description  : SDMMC protocol driver for SDHI/SDMMC device.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file sdmmc_driver.h
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief middle level drivers for SD MMC cards.
 *
 * SDMMC Drivers to access SD and MMC memory devices, including eMMC.
 **********************************************************************************************************************/
#ifndef SDMMC_DRIVER_H_
#define SDMMC_DRIVER_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_sdmmc.h"
#include "r_sdmmc_api.h"
#include "sdcard.h"
#include "bsp_api.h"

extern const transfer_api_t g_transfer_on_dtc;
extern const transfer_api_t g_transfer_on_dmac;

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* Extended CSD definitions */

#define EXT_CSD_SIZE                   (512)                                //< Size, in bytes, of extended CSD
#define EXT_CSD_BUFFER_SIZE            (32)                                 //< Buffer size limitation to keep stack
                                                                            // size down
#define EXT_CSD_READ_BUFFER_ITERATIONS (EXT_CSD_SIZE / EXT_CSD_BUFFER_SIZE) //< Iterations required to read ext. CSD
/* Offsets */
#define EXT_CSD_BUS_WIDTH_OFFSET    (183)                                    //< Ext CSD Bus Width offset
#define EXT_CSD_HS_TIMING_OFFSET    (185)                                    //< Ext CSD Bus Width offset
#define EXT_CSD_REVISION_OFFSET     (192)                                    //< Ext CSD revision offset
#define EXT_CSD_DEVICE_TYPE_OFFSET  (196)                                    //< Ext CSD device type offset

/* Iterations */
#define EXT_CSD_BUS_WIDTH_ITERATION    (EXT_CSD_BUS_WIDTH_OFFSET / EXT_CSD_BUFFER_SIZE)  //< Ext CSD Bus Width
#define EXT_CSD_HS_TIMING_ITERATION    (EXT_CSD_HS_TIMING_OFFSET / EXT_CSD_BUFFER_SIZE)  //< Ext CSD Bus Width
#define EXT_CSD_REVISION_ITERATION     (EXT_CSD_REVISION_OFFSET / EXT_CSD_BUFFER_SIZE)    //< Ext CSD Revision
#define EXT_CSD_DEVICE_TYPE_ITERATION  (EXT_CSD_DEVICE_TYPE_OFFSET / EXT_CSD_BUFFER_SIZE) //< Ext CSD Device type

/* Ext CSD BUS_WIDTH location in buffer */
#define EXT_CSD_BUS_WIDTH (EXT_CSD_BUS_WIDTH_OFFSET - (EXT_CSD_BUS_WIDTH_ITERATION * EXT_CSD_BUFFER_SIZE))
/* Ext CSD HS_TIMING location in buffer */
#define EXT_CSD_HS_TIMING (EXT_CSD_HS_TIMING_OFFSET - ( EXT_CSD_HS_TIMING_ITERATION * EXT_CSD_BUFFER_SIZE))
/* Ext CSD REVISION location in buffer */
#define EXT_CSD_REVISION (EXT_CSD_REVISION_OFFSET - (EXT_CSD_REVISION_ITERATION * EXT_CSD_BUFFER_SIZE))
/* Ext CSD DEVICE_TYPE location in buffer */
#define EXT_CSD_DEVICE_TYPE (EXT_CSD_DEVICE_TYPE_OFFSET - ( EXT_CSD_DEVICE_TYPE_ITERATION * EXT_CSD_BUFFER_SIZE))


/* Driver Status definitions */
#define MMCSD_CARD_FAILED             (1)
#define MMCSD_CARD_OK                 (0)
#define MMCSD_DATA_SIZE_UNINITIALIZED (2)

/* These defines should match the bit position in the sdmmmc_event_t structure below */
#define SDMMC_CARD_CMD_ERR (1 << 16)               //< Command error
#define SDMMC_CARD_CRC_ERR (1 << 17)               //< CRC error
#define SDMMC_CARD_END_ERR (1 << 18)               //< End bit error
#define SDMMC_CARD_DTO     (1 << 19)               //< Data Timeout
#define SDMMC_CARD_ILW     (1 << 20)               //< Illegal write address
#define SDMMC_CARD_ILR     (1 << 21)               //< Illegal read address
#define SDMMC_CARD_RSPT    (1 << 22)               //< Response timeout
#define SDMMC_CARD_BRE     (1 << 24)               //< Buffer read enable
#define SDMMC_CARD_BWE     (1 << 25)               //< Buffer write enable
#define SDMMC_CARD_ILA_ERR (1 << 31)               //< Illegal access

#define SDMMC_ACCESS_ERROR_MASK  (uint32_t)                                                    \
    (SDMMC_CARD_CMD_ERR | SDMMC_CARD_CRC_ERR | SDMMC_CARD_END_ERR | SDMMC_CARD_DTO | \
     SDMMC_CARD_ILW | SDMMC_CARD_ILR | SDMMC_CARD_RSPT | SDMMC_CARD_ILA_ERR)

/* These defines should match the bit position in the sdmmmc_event_sdio_t structure below */

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef enum e_sdmmc_error_type
{
    SDMMC_ERROR_NO_ERROR,
    SDMMC_ERROR_COMMAND_ERROR,
    SDMMC_ERROR_EVENT_ERROR,
    SDMMC_ERROR_BWE_ERROR,
    SDMMC_ERROR_BRE_ERROR,
    SDMMC_ERROR_ACCESS_END_ERROR,
    SDMMC_ERROR_PROCESS_END_ERROR,
    SDMMC_ERROR_CARD_BUSY,
    SDMMC_ERROR_WRITE_PROTECTED,
    SDMMC_ERROR_READ_ERROR,
    SDMMC_ERROR_INVALID_PARAMETER,
    SDMMC_ERROR_TRANSFER,
} sdmmc_error_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
ssp_err_t SDMMC_DriverInit (sdmmc_ctrl_t * const p_ctrl, sdmmc_cfg_t const * const p_cfg);

ssp_err_t SDMMC_SectorRead (sdmmc_ctrl_t * const p_ctrl, uint8_t * p_data, uint32_t start_block, uint32_t block_count);

ssp_err_t SDMMC_SectorWrite (sdmmc_ctrl_t * const p_ctrl,
                             const uint8_t        * p_data,
                             uint32_t             start_block,
                             uint32_t             block_count);

sdmmc_event_t SDMMC_TransferEnd (sdmmc_ctrl_t * const p_ctrl);

ssp_err_t SDMMC_InterruptError (sdmmc_ctrl_t * const p_ctrl);

ssp_err_t SDMMC_SectorErase (sdmmc_ctrl_t * const p_ctrl, uint32_t start_block, uint32_t sector_count);

ssp_err_t SDMMC_CMD53Read (sdmmc_ctrl_t * const     p_ctrl,
                           uint8_t * const          p_dest,
                           uint32_t const           function,
                           uint32_t const           address,
                           uint32_t * const         count,
                           sdmmc_io_transfer_mode_t transfer_mode,
                           sdmmc_io_address_mode_t  address_mode);

ssp_err_t SDMMC_CMD53Write (sdmmc_ctrl_t * const     p_ctrl,
                            uint8_t const * const    p_source,
                            uint32_t const           function,
                            uint32_t const           address,
                            uint32_t const           count,
                            sdmmc_io_transfer_mode_t transfer_mode,
                            sdmmc_io_address_mode_t  address_mode);

ssp_err_t SDMMC_CMD52Read (sdmmc_ctrl_t * const p_ctrl,
                           uint8_t  * const     p_data,
                           uint32_t const       function,
                           uint32_t const       address);

ssp_err_t SDMMC_CMD52Write (sdmmc_ctrl_t * const        p_ctrl,
                            uint8_t  * const            p_data,
                            uint32_t const              function,
                            uint32_t const              address,
                            sdmmc_io_write_mode_t const read_after_write);

ssp_err_t SDMMC_InfoGet (sdmmc_ctrl_t * const p_ctrl);
#endif /* SDMMC_DRIVER_H_ */
