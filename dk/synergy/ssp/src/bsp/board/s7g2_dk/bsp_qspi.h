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
 * File Name    : bsp_qspi.h
 * Description  : Configures QSPI bus and device access.
 **********************************************************************************************************************/



/*******************************************************************************************************************//**
 * @ingroup BSP_BOARD_DK2M
 * @defgroup BSP_DK2M_QSPI Board QSPI
 * @brief QSPI configuration setup for this board.
 *
 * This is code specific to the DK2M board.
 *
 * @{
 **********************************************************************************************************************/

#ifndef BSP_QSPI_H_
#define BSP_QSPI_H_
/* This QSPI header file requires this be included here. */
#include "../../src/bsp/mcu/all/bsp_common.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* ID and type information for the N25Q256A Micron Serial NOR Flash Memory */
#define BSP_PRV_QSPI_MANUFACTURER_ID 0x20
#define BSP_PRV_QSPI_MEMORY_TYPE     0xba
#define BSP_PRV_QSPI_MEMORY_CAPACITY 0x19

/** Physical address the QSPI device is mapped to */
#define BSP_PRV_QSPI_DEVICE_PHYSICAL_ADDRESS (0x60000000)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Available QSPI clock rates for  Micron N25Q256A quad SPI flash device */
typedef enum e_qspi_clk
{
    /** Reset Operations */
    QSPI_CLK_60MHZ,         ///< SPI CLK runs at 60.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_40MHZ,         ///< SPI CLK runs at 40.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_30MHZ,         ///< SPI CLK runs at 30.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_24MHZ,         ///< SPI CLK runs at 24.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_20MHZ,         ///< SPI CLK runs at 20.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_17MHZ,         ///< SPI CLK runs at 17.14 MHz when PCLKA is set to 120MHz
    QSPI_CLK_15MHZ,         ///< SPI CLK runs at 15.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_13MHZ,         ///< SPI CLK runs at 13.33 MHz when PCLKA is set to 120MHz
    QSPI_CLK_12MHZ,         ///< SPI CLK runs at 12.00 MHz when PCLKA is set to 120MHz
} qspi_clk;

/** Available QSPI read modes for  Micron N25Q256A quad SPI flash device */
typedef enum e_qspi_readmode
{
    /** Reset Operations */
    QSPI_READMODE_STANDARD,         ///< Standard
    QSPI_READMODE_FAST,             ///< Fast read
    QSPI_READMODE_FAST_DUAL_OUTPUT, ///< Fast Read Dual Output
    QSPI_READMODE_FAST_DUAL_IO,     ///< Fast Read Dual I/O
    QSPI_READMODE_FAST_QUAD_OUTPUT, ///< Fast Read Quad Output
    QSPI_READMODE_FAST_QUAD_IO,     ///< Fast Read Quad I/O
} qspi_readmode;

/** Available QSPI commands for the Micron N25Q256A quad SPI flash device */
typedef enum e_qspi_command
{
    /** Reset Operations */
    QSPI_COMMAND_RESET_ENABLE              = 0x66,         ///< Enable reset
    QSPI_COMMAND_RESET_MEMORY              = 0x99,         ///< Reset memory

    /** Read Operations */
    QSPI_COMMAND_READ                      = 0x03,         ///< Read memory
    QSPI_COMMAND_4BYTE_READ                = 0x13,         ///< Read memory using a 4 byte address

    /** Write Operations */
    QSPI_COMMAND_WRITE_ENABLE              = 0x06,         ///< Write Enable
    QSPI_COMMAND_WRITE_DISABLE             = 0x04,         ///< Write Disable
    QSPI_COMMAND_PAGE_PROGRAM              = 0x02,         ///< Program a page
    QSPI_COMMAND_4BYTE_PAGE_PROGRAM        = 0x12,         ///< Program a page using a 4 byte address

    /** Register Operations */
    QSPI_COMMAND_READ_STATUS_REGISTER      = 0x05,         ///< Read Status register
    QSPI_COMMAND_READ_FLAG_STATUS_REGISTER = 0x70,         ///< Read Status register
    QSPI_COMMAND_READ_ID                   = 0x9F,         ///< Read the chip ID
    QSPI_COMMAND_READ_NONVOLATILE_CFGREG   = 0xB5,         ///< Read the non-volatile configuration register
    QSPI_COMMAND_READ_ENH_VOLATILE_CFGREG  = 0x65,         ///< Read the Ehn volatile cfg register
    QSPI_COMMAND_WRITE_NONVOLATILE_CFGREG  = 0xB1,         ///< Write the non-volatile configuration register
    QSPI_COMMAND_READ_VOLATILE_CFGREG      = 0x85,         ///< Read the volatile configuration register
    QSPI_COMMAND_WRITE_VOLATILE_CFGREG     = 0x81,         ///< Write the volatile configuration register
    QSPI_COMMAND_WRITE_ENH_VOLATILE_CFGREG = 0x61,         ///< Write the Ehn volatile cfg register

    /** Erase Operations */
    QSPI_COMMAND_SECTOR_ERASE              = 0xD8,         ///< Erase a sector
    QSPI_COMMAND_4BYTE_SECTOR_ERASE        = 0xDC,         ///< Erase a sector using a 4 byte address
    QSPI_COMMAND_SUBSECTOR_ERASE           = 0x20,         ///< Erase a subsector
    QSPI_COMMAND_4BYTE_SUBSECTOR_ERASE     = 0x21,         ///< Erase a subsector using a 4 byte address
    QSPI_COMMAND_CHIP_ERASE                = 0xC7,         ///< Erase the entire chip
} qspi_command;

/** non-volatile configuration register for the Micron N25Q256A quad SPI flash device */
typedef struct st_n25q256a_nv_cfg
{
    union
    {
        uint32_t    entire_cfg;
        struct
        {
        uint16_t  address_bytes              : 1; ///< Defines the number of address bytes for a command. 0 = enable 4 byte
                                                  // address, 1 = enable 3 byte address (default)
        uint16_t  segment_select_128mb       : 1; ///< Selects a 128Mb segment as default for 3B address operations.  0 =
                                                  // Upper 128Mb segment, 1 = Lower 128Mb segment (Default)
        uint16_t  dual_io_protocol           : 1; ///< Enables or disables dual I/O protocol. 0 = Enabled, 1 = Disabled
                                                  // (Default, Extended SPI protocol)
        uint16_t  quad_io_protocol           : 1; ///< Enables or disables quad I/O protocol. 0 = Enabled, 1 = Disabled
                                                  // (Default, Extended SPI protcocol)
        uint16_t  reset_hold                 : 1; ///< Enables or disables hold or reset. 0 = Disabled, 1 = Enabled
                                                  // (Default)
        uint16_t  reserved_0                 : 1; ///< reserved
        uint16_t  output_driver_strength     : 3; ///< Optimizes impedance at VCC/2 output voltage.
        uint16_t  xip_mode_at_power_on_reset : 3; ///< Enables the device to operate in the selected XIP mode immediately
                                                  // after power-on reset.
        uint16_t  number_dummy_clock_cycles  : 4; ///< Sets the number of dummy clock cycles subsequent to all FAST READ
                                                  // commands. The default setting targets the maximum allowed frequency and
                                                  // guarantees backward compatibility.
        };
    };
} n25q256a_nv_cfg;

/** volatile configuration register for the Micron N25Q256A quad SPI flash device */
typedef struct st_n25q256a_volatile_cfg
{
    union
    {
        uint32_t    entire_cfg;
        struct
        {
        uint8_t  wrap : 2;                   ///< Output data wraps within an aligned 16, 32, or 64 byte boundary starting
                                             // from the 3-byte address issued after the command code.
                                             // 00 = 16-byte boundary aligned
                                             // 01 = 32-byte boundary aligned
                                             // 10 = 64-byte boundary aligned
                                             // 11 = sequential (default)
        uint8_t  reserved_0 : 1;             ///< reserved
        uint8_t  xip        : 1;             ///< Enables or disables XIP. For device part numbers with feature digit equal
                                             // to 2 or 4, this bit is always "Don’t Care," so the device operates in XIP
                                             // mode without setting this bit.
        uint8_t  num_dummy_clock_cycles : 4; ///< Sets the number of dummy clock cycles subsequent to all FAST
                                             // READ commands. The default setting targets maximum allowed
                                             // frequency and guarantees backward compatibility. 0000 = 1111
        };
    };
} n25q256a_volatile_cfg;

/** volatile Enhanced configuration register for the Micron N25Q256A quad SPI flash device */
typedef struct st_n25q256a_enh_volatile_cfg
{
    union
    {
        uint32_t    entire_cfg;
        struct
        {
        uint8_t  ouput_drv_strength : 3;    ///< Output driver strength
                                            /// 111 = 30 (default)
        uint8_t  vpp_accel    : 1;          ///<
        uint8_t  reset_hold   : 1;          ///<
        uint8_t  reserved     : 1;          ///< Reserved Fixed value = 0b
        uint8_t  dual_io_prot : 1;          ///< Enable or disable Dual IO protocol
        uint8_t  quad_io_prot : 1;          ///< Enable or disable Quadf IO protocol
        };
    };
} n25q256a_enh_volatile_cfg;

/** status register for the Micron N25Q256A quad SPI flash device */
typedef struct st_n25q256a_status
{
    union
    {
        uint32_t    entire_cfg;
        struct
        {
        uint8_t  write_in_progress : 1;            ///< Indicates if one of the following command cycles is in progress:
                                                   // WRITE STATUS REGISTER
                                                   // WRITE NONVOLATILE CONFIGURATION REGISTER
                                                   // PROGRAM
                                                   // ERASE
                                                   // 0 = Ready, 1 = Busy
        uint8_t  write_enable_latch : 1;           ///< The device always powers up with this bit cleared to prevent
                                                   // inadvertent WRITE STATUS REGISTER, PROGRAM, or ERASE operations. To
                                                   // enable these operations, the WRITE ENABLE operation must be executed
                                                   // first to set this bit. 0 = Cleared (Default), 1 = Set
        uint8_t  block_protect_3_0 : 2;            ///< Defines memory to be software protected against PROGRAM or ERASE
                                                   // operations. When one or more block protect bits is set to 1, a
                                                   // designated
                                                   // memory area is protected from PROGRAM and ERASE operations.
        uint8_t  top_bottom : 1;                   ///< Determines whether the protected memory area defined by the block
                                                   // protect bits
                                                   // starts from the top or bottom of the memory array. 0 = Top, 1 = Bottom
        uint8_t  status_register_write_enable : 1; ///< Used with the W/VPP signal to enable or disable writing to the
                                                   // status register. 0 = Enabled, 1 = Disabled
        };
    };
} n25q256a_status;

/** flag status register for the Micron N25Q256A quad SPI flash device */
typedef struct st_n25q256a_flag_status
{
    union
    {
        uint32_t    entire_cfg;
        struct
        {
        uint8_t  addressing_4_byte_mode : 1;      ///< 0 = 3 bytes addressing, 1 = 4 bytes addressing
                                                  // 0 = Ready, 1 = Busy
        uint8_t  protection             : 1;      ///< Indicates whether a PROGRAM operation has attempted to modify the
                                                  // protected array sector or access the locked OTP space. 0 = Clear, 1 =
                                                  // Failure or protection error
        uint8_t  program_suspend             : 1; ///< Indicates whether a PROGRAM operation has been or is going to be
                                                  // suspended. 0 = Not in effect, 1 = In effect
        uint8_t  vpp                         : 1; ///< Indicates an invalid voltage on VPP during a PROGRAM or ERASE
                                                  // operation. 0 = Enabled, 1 = Disabled (Default)
        uint8_t  program                     : 1; ///< An attempt to program a 0 to a 1 when VPP = VPPH and the data pattern
                                                  // is a multiple of 64 bits. 0 = Clear, 1 = Failure or protection error
        uint8_t  erase                       : 1; ///< Indicates whether an ERASE operation has succeeded or failed. 0 =
                                                  // Clear, 1 = Failure or protection error
        uint8_t  erase_suspend               : 1; ///< Indicates whether an ERASE operation has been or is going to be
                                                  // suspended. 0 = Not in effect, 1 = In effect
        uint8_t  program_or_erase_controller : 1; ///< Indicates whether a PROGRAM, ERASE, WRITE STATUS REGISTER, or WRITE
                                                  // NONVOLATILE CONFIGURATION command cycle is in progress. 0 = Busy, 1 =
                                                  // Ready
        };
    };
} n25q256a_flag_status;

typedef enum qspi_protocol
{
    QSPI_EXTENDED_SPI_PROTOCOL = 0,
    QSPI_DUAL_SPI_PROTOCOL     = 1,
    QSPI_QUAD_SPI_PROTOCOL     = 2,
} qspi_protocol;

/** characteristics of the flash device */
typedef struct st_n25q256a_characteristics
{
    uint8_t  manufacturer_id;           ///< Manufacturer ID
    uint8_t  memory_type;               ///< Memory type
    uint8_t  memory_capacity;           ///< Memory capacity
} n25q256a_characteristics;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
void bsp_qspi_init (void);

void bsp_qspi_status_get (bool * p_write_in_progress);

void bsp_qspi_xip_enter (void);

void bsp_qspi_xip_exit (void);

void bsp_qspi_config_get (uint8_t  * p_manufacturer_id,
                          uint8_t  * p_memory_type,
                          uint8_t  * p_memory_capacity,
                          uint32_t * p_max_eraseable_size,
                          uint32_t * p_num_address_bytes,
                          uint32_t * p_spi_mode,
                          uint32_t * p_page_size,
                          bool     * p_xip_mode);

void bsp_qspi_flags_get (bool * p_addressing_4_byte_mode,
                         bool * p_program_suspended,
                         bool * p_erase_failure,
                         bool * p_program_failure,
                         bool * p_protection_failure);
#endif /* BSP_QSPI_H_ */
/** @} (end defgroup BSP_DK2M_QSPI) */
