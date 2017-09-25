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
 * File Name    : r_sdmmc_api.h
 * Description  : SDMMC driver API header file.
 **********************************************************************************************************************/

#ifndef DRV_SDMMC_API_H
#define DRV_SDMMC_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Register definitions, common services and error codes. */
#include "bsp_api.h"
#include "r_transfer_api.h"

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup SDMMC_API SDMMC Interface
 * @brief Interface for accessing SDMMC cards.
 *
 * @section SDMMC_API_SUMMARY Summary
 * The r_sdmmc interface provides standard SD, MMC and eMMC media functionality. A complete file system can be
 * implemented with FileX, sf_el_fx, sf_block_media_sdmmc and r_sdmmc modules.
 * The SDMMC interface is implemented by:
 * - @ref SDMMC
 *
 * Related SSP architecture topics:
 * - @ref ssp-interfaces
 * - @ref ssp-predefined-layers
 * - @ref using-ssp-modules
 *
 * SDMMC description: @ref ModuleSDMMC
 *
 * @{
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SDMMC_API_VERSION_MAJOR ((uint8_t)1)
#define SDMMC_API_VERSION_MINOR ((uint8_t)0)

#define SDMMC_MAX_BLOCK_SIZE   (512)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** SDMMC status */
typedef enum e_sdmmc_ready_status
{
    SDMMC_STATUS_CARD_NOT_READY = 0x00,     ///< SD card or eMMC device has not been initialized.
    SDMMC_STATUS_CARD_READY,                ///< SD card or eMMC device has been initialized and is ready to access.
} sdmmc_ready_status_t;

/** SDMMC media uses SD protocol or MMC protocol. */
typedef enum e_sdmmc_card_type
{
    SDMMC_CARD_TYPE_MMC,                    ///< The media is an MMC card or eMMC device.
    SDMMC_CARD_TYPE_SD                      ///< The media is an SD card.
} sdmmc_card_type_t;

/** SDMMC media is embedded or it can be inserted and removed.  */
typedef enum e_media_type
{
    SDMMC_MEDIA_TYPE_EMBEDDED,              ///< The media is an embedded card, or eMMC device.
    SDMMC_MEDIA_TYPE_CARD                   ///< The media is an pluggable card.
} sdmmc_media_type_t;

/** SDMMC data bus is 1, 4 or 8 bits wide. */
typedef enum e_sdmmc_bus_width
{
    SDMMC_BUS_WIDTH_1_BIT  = 1,             ///< Data bus is 1 bit wide.
    SDMMC_BUS_WIDTH_4_BITS = 4,             ///< Data bus is 4 bits wide.
    SDMMC_BUS_WIDTH_8_BITS = 8,             ///< Data bus is 8 bits wide.
} sdmmc_bus_width_t;

/** Channel, media type, bus width defined by the hardware. */
typedef struct s_sdmmc_hw
{
    uint8_t             channel;                ///< Channel of SD/MMC host interface.
    sdmmc_media_type_t  media_type;             ///< Embedded or pluggable card.
    sdmmc_bus_width_t   bus_width;              ///< Device bus width is 1, 4 or 8 bits wide.
} sdmmc_hw_t;

/** Status and other information obtained from the media device. */
typedef struct s_sdmmc_info
{
    sdmmc_card_type_t  card_type;               ///< SD or MMC
    bool               ready;                   ///< ready or not
    bool               hc;                      ///< true = Card is High Capacity card
    bool               sdio;                    ///< true = SDIO present
    bool               write_protected;         ///< Card is write protected
    bool               transfer_in_progress;    ///< DMA or DTC transfer status.
    uint8_t            csd_version;             ///< CSD version
    uint8_t            device_type;             ///< Speed and data rate
    sdmmc_bus_width_t  bus_width;               ///< Current media bus width
    uint8_t            hs_timing;               ///< High Speed status
    uint32_t           sdhi_rca;                ///< Relative Card Address
    uint32_t           max_clock_rate;          ///< Maximum clock rate for media card
    uint32_t           clock_rate;              ///< current clock rate
    uint32_t           sector_size;             ///< sector size
    uint32_t           sector_count;            ///< sector count
    uint32_t           erase_sector_count;      ///< Maximum number of sectors to erase at once
} sdmmc_info_t;

/** SDIO command */
typedef enum e_sdmmc_io_command
{
    SDMMC_IO_COMMAND_DIRECT_IO = 52,
    SDMMC_IO_COMMAND_DIRECT_IO_EXT = 53,

} sdmmc_io_command_t;

typedef enum e_sdmmc_io_transfer_mode
{
    SDMMC_IO_MODE_TRANSFER_BYTE = 0,
    SDMMC_IO_MODE_TRANSFER_BLOCK

} sdmmc_io_transfer_mode_t;

typedef enum e_sdmmc_io_address_mode
{
    SDMMC_IO_ADDRESS_MODE_FIXED = 0,
    SDMMC_IO_ADDRESS_MODE_INCREMENT
} sdmmc_io_address_mode_t;

typedef enum e_sdmmc_io_write_mode
{
    SDMMC_IO_WRITE_MODE_NO_READ = 0,
    SDMMC_IO_WRITE_READ_AFTER_WRITE
} sdmmc_io_write_mode_t;



typedef struct s_sdmmc_io_mode
{
    sdmmc_io_command_t         command;                ///< SDIO command.
    sdmmc_io_transfer_mode_t   transfer_mode;          ///< SDIO transfer type.
    sdmmc_io_address_mode_t    address_mode;           ///< SDIO addresss mode.
    sdmmc_io_write_mode_t      write_mode;             ///< SDIO write mode.

} sdmmc_io_mode_t;



/** Events that can trigger a callback function */
typedef enum e_sdmmc_event
{
    SDMMC_EVENT_CARD_REMOVED,            ///< Card removed event.
    SDMMC_EVENT_CARD_INSERTED,           ///< Card inserted event.
    SDMMC_EVENT_ACCESS,                  ///< Access event.
    SDMMC_EVENT_SDIO,                    ///< IO event.
    SDMMC_EVENT_TRANSFER_COMPLETE,       ///< Read or write complete.
    SDMMC_EVENT_TRANSFER_ERROR,          ///< Read or write failed.
    SDMMC_EVENT_NONE,                    ///< No event.

} sdmmc_event_t;

/** Callback function parameter data */
typedef struct st_sdmmc_callback_args
{
    sdmmc_event_t  event;               ///< The event can be used to identify what caused the callback.
    void const   * p_context;           ///< Placeholder for user data.
} sdmmc_callback_args_t;

/** SDMMC Configuration */
typedef struct st_sdmmc_cfg
{
    /* SDMMC generic configuration */
    sdmmc_hw_t                   hw;                           ///<  Channel, media type, bus width defined by the hardware.
    transfer_instance_t  const * p_lower_lvl_transfer;         ///< Transfer instance used to move data with DMA or DTC

    /* Configuration for SDMMC Event processing */
    void (* p_callback)(sdmmc_callback_args_t * p_args);  ///<  Pointer to callback function
    void const * p_context;                               ///<  User defined context passed into callback function

    /* Pointer to SDMMC peripheral specific configuration */
    void const * p_extend;                                ///<  SDMMC hardware dependent configuration
} sdmmc_cfg_t;

/** SDMMC control block */
typedef struct st_sdmmc_ctrl
{
    /* Parameters to control SDMMC peripheral device */
    sdmmc_hw_t                  hw;                              ///< Channel, media type, bus width defined by the hardware.
    transfer_instance_t const * p_lower_lvl_transfer;            ///< Transfer instance used to move data with DMA or DTC
    sdmmc_info_t                status;                          ///< Media status information.
    bool                        transfer_in_progress;            ///< DMA or DTC transfer status.
    /* Parameters to process SDMMC Event */
    void (* p_callback)(sdmmc_callback_args_t * p_args);         ///<  Pointer to callback function
    void const * p_context;                                      ///<  Pointer to the higher level device context
} sdmmc_ctrl_t;

/** SDMMC functions implemented at the HAL layer API. */
typedef struct st_sdmmc_api
{
    /** Open device channel for read/write and control. Initialize driver and hardware on first call.
     * @par Implemented as
     * R_SDMMC_Open()
     *
     * @param[out]    p_ctrl    Handle for channel (pointer to SDMMC channel device context).
     * @param[in]     p_cfg     Pointer to the SDMMC configuration structure for a channel.
     */
    ssp_err_t (* open)(sdmmc_ctrl_t * const p_ctrl, sdmmc_cfg_t const * const p_cfg);

    /** Close open SDMMC channel. Turns off hardware if last channel open.
     * @par Implemented as
     * R_SDMMC_Close()
     *
     * @param[in]   p_ctrl  Handle for channel (pointer to SDMMC channel device context).
     */
    ssp_err_t (* close)(sdmmc_ctrl_t * const p_ctrl);

    /** Read data from an SDMMC channel.
     * @par Implemented as
     * R_SDMMC_Read()
     *
     * @param[in]   p_ctrl          Handle for channel (pointer to SDMMC channel device context).
     * @param[out]  p_dest          Pointer to data buffer to read data to.
     * @param[in]   start_sector    Beginning sector address to read.
     * @param[in]   sector_count    Number of sectors to read.
     */
    ssp_err_t (* read)(sdmmc_ctrl_t * const p_ctrl,
                            uint8_t * const p_dest,
                            uint32_t const  start_sector,
                            uint32_t const  sector_count);

    /** Write data to SDMMC channel.
     * @par Implemented as
     * R_SDMMC_Write()
     *
     * @param[in]   p_ctrl          Handle for channel (pointer to channel control block).
     * @param[in]   p_source        Pointer to data buffer to write data from.
     * @param[in]   start_sector    Beginning sector address to write to.
     * @param[in]   sector_count    Number of sectors to write.
     */
    ssp_err_t (* write)(sdmmc_ctrl_t * const  p_ctrl,
                        uint8_t const * const p_source,
                        uint32_t const        start_sector,
                        uint32_t const        sector_count);

    /** The Control function sends control commands to and receives info from the SD/MMC port.
     * @par Implemented as
     * R_SDMMC_Control()
     *
     * @param[in]       p_ctrl      Handle for channel (pointer to channel control block).
     * @param[in]       command     Command to execute.
     * @param[in,out]   p_data      Void pointer to data in or out.
     */
    ssp_err_t (* control)(sdmmc_ctrl_t * const p_ctrl, ssp_command_t const command, void * p_data);

    /** Read I/O data from an SDMMC channel.
     * @par Implemented as
     * R_SDMMC_ReadIo()
     *
     * @param[in]   p_ctrl          Handle for channel (pointer to SDMMC channel device context).
     * @param[out]  p_data          Pointer to data buffer to read data to.
     * @param[in]   function        SDIO Function Number.
     * @param[in]   address         SDIO register address.
     */
    ssp_err_t (* readIo)(sdmmc_ctrl_t * const p_ctrl,
                              uint8_t * const p_data,
                              uint32_t const  function,
                              uint32_t const  address);

    /** Write I/O data to SDMMC channel.
     * @par Implemented as
     * R_SDMMC_WriteIo()
     *
     * @param[in]   p_ctrl              Handle for channel (pointer to channel control block).
     * @param[in]   p_data              Pointer to data buffer to write data from.
     * @param[in]   function            SDIO Function Number.
     * @param[in]   address             SDIO register address.
     * @param[in]   read_after_write    Set to true for reading after writing.
     */
    ssp_err_t (* writeIo)(sdmmc_ctrl_t * const  p_ctrl,
                               uint8_t * const  p_data,
                                uint32_t const  function,
                                uint32_t const  address,
                   sdmmc_io_write_mode_t const  read_after_write);

    /** Read I/O data, extended, from an SDMMC channel.
     * @par Implemented as
     * R_SDMMC_ReadIoExt()
     *
     * @param[in]   p_ctrl          Handle for channel (pointer to SDMMC channel device context).
     * @param[out]  p_dest          Pointer to data buffer to read data to.
     * @param[in]   function        SDIO Function Number.
     * @param[in]   address         SDIO register address.
     * @param[in]   count           Number of bytes or blocks to read.
     * @param[in]   transfer_mode   Byte mode = 0, Block mode = 1.
     * @param[in]   address_mode    0 = fixed address, 1 = Incrementing address.
     */
    ssp_err_t (* readIoExt)(sdmmc_ctrl_t * const p_ctrl,
                                 uint8_t * const p_dest,
                                  uint32_t const function,
                                uint32_t   const address,
                                uint32_t * const count,
                        sdmmc_io_transfer_mode_t transfer_mode,
                         sdmmc_io_address_mode_t address_mode);

    /** Write I/O data, extended, to SDMMC channel.
     * @par Implemented as
     * R_SDMMC_WriteIoExt()
     *
     * @param[in]   p_ctrl          Handle for channel (pointer to channel control block).
     * @param[in]   p_source        Pointer to data buffer to write data from.
     * @param[in]   function_number SDIO Function Number.
     * @param[in]   address         SDIO register address.
     * @param[in]   count           Number of bytes or blocks to write.
     * @param[in]   transfer_mode   Byte mode = 0, Block mode = 1.
     * @param[in]   address_mode    0 = fixed address, 1 = Incrementing address.
     */
    ssp_err_t (* writeIoExt)(sdmmc_ctrl_t * const  p_ctrl,
                            uint8_t const * const  p_source,
                                   uint32_t const  function,
                                   uint32_t const  address,
                                  uint32_t  const  count,
                         sdmmc_io_transfer_mode_t  transfer_mode,
                          sdmmc_io_address_mode_t  address_mode);

    /** Enables SDIO interrupt for SDMMC channel.
     * @par Implemented as
     * R_SDMMC_IoIntEnable
     *
     * @param[in]   p_ctrl          Handle for channel (pointer to channel control block).
     * @param[in]   enable          Interrupt enable = true, interrupt disable = false.
     */
    ssp_err_t (* IoIntEnable)(sdmmc_ctrl_t * const p_ctrl, bool enable);

    /** Returns the version of the SDMMC driver.
     * @par Implemented as
     * R_SDMMC_VersionGet()
     *
     * @param[out]  p_version       Pointer to return version information to.
     */
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);

    /** Get SDMMC channel info.
     * @par Implemented as
     * R_SDMMC_InfoGet()
     *
     * @param[in]   p_ctrl  Handle for channel (pointer to channel control block)
     * @param[out]  p_info  Memory address to return card specific data to.
     */
    ssp_err_t (* infoGet)(sdmmc_ctrl_t * const p_ctrl, sdmmc_info_t * const p_info);

    /** Erase SDMMC sectors.
     * @par Implemented as
     * R_SDMMC_Erase
     *
     * @param[in] p_ctrl            Handle for channel (pointer to channel control block).
     * @param[in] start_sector      Beginning sector to erase.
     * @param[in] sector_count      Number of sectors to erase.
     */
    ssp_err_t (* erase)(sdmmc_ctrl_t * const p_ctrl, uint32_t const start_sector, uint32_t const sector_count);
} sdmmc_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sdmmc_instance
{
    sdmmc_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    sdmmc_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    sdmmc_api_t const * p_api;     ///< Pointer to the API structure for this instance
} sdmmc_instance_t;


#endif /* DRV_SDMMC_API_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup SDMMC_API)
 **********************************************************************************************************************/
