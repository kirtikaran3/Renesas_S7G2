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
 * File Name    : sdcard.h
 * Description  : SD Card information header file.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file sdcard.h
 **********************************************************************************************************************/
#ifndef SDCARD_H
#define SDCARD_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define CSD_VERSION_1_0                  (0)
#define CSD_VERSION_2_0                  (1)    /* value of 1 is CSD 2.0 according to SD spec. */

#define SD_VERSION_1_01                  (0)    /* SD version 1.0 and 1.01 */
#define SD_VERSION_1_10                  (1)    /* SD version 1.10  */
#define SD_VERSION_2_00                  (2)    /* SD version 2.00 or version 3.0x, check SD_SPEC3  */
#define SD_VERSION_3_0X                  (3)    /* SD version derived from 2.00 and SD_SPEC3 */

#define MMC_VERSION_4_1                  (1)    /* value of 1 is CSD 1.1, MMC 4.1 according to MMC spec. */
#define MMC_VERSION_4_2                  (2)    /* value of 2 is CSD 1.2, MMC 4.2 according to MMC spec. */
#define MMC_VERSION_4_3                  (3)    /* value of 3 is CSD 1.3, MMC 4.3 according to MMC spec. */
#define MMC_VERSION_OBSOLETE             (4)    /* value of 4 is CSD 1.4, Obsolete acording to MMC spec. */
#define MMC_VERSION_4_4_1                (5)    /* value of 5 is CSD 1.3, MMC 4.41 according to MMC spec. */
#define MMC_VERSION_4_5                  (6)    /* value of 6 is CSD 1.3, MMC 4.5, 4.51 according to MMC spec. */
#define MMC_VERSION_5_0                  (7)    /* value of 7 is CSD 1.3, MMC 5.0, 5.1 according to MMC spec. */

#define CSD_VERSION_EXT                  (3)

#define CSD_VERSION_2_0_MULT             (0x80000)

#define SD_CMD_GO_IDLE_STATE             (0)
#define SD_CMD_SEND_OP_COND              (1)
#define SD_CMD_ALL_SEND_CID              (2)
#define SD_CMD_SEND_RELATIVE_ADDR        (3)
#define SD_CMD_SET_DSR                   (4)
#define SD_CMD_SDIO                      (5)
#define SD_CMD_SET_BUS_WIDTH             (6)
#define SD_CMD_SWITCH                    (6)
#define SD_CMD_SEL_DES_CARD              (7)
#define SD_CMD_IF_COND                   (8)
#define SD_CMD_SEND_CSD                  (9)
#define SD_CMD_SEND_CID                  (10)
#define SD_CMD_READ_DAT_UNTIL_STOP       (11)
#define SD_CMD_STOP_TRANSMISSION         (12)
#define SD_CMD_SEND_STATUS               (13)
#define SD_CMD_GO_INACTIVE_STATE         (15)
#define SD_CMD_SET_BLOCKLEN              (16)
#define SD_CMD_READ_SINGLE_BLOCK         (17)
#define SD_CMD_READ_MULTIPLE_BLOCK       (18)
#define SD_CMD_WRITE_DAT_UNTIL_STOP      (20)
#define SD_CMD_SET_BLOCK_COUNT           (23)
#define SD_CMD_WRITE_SINGLE_BLOCK        (24)
#define SD_CMD_WRITE_MULTIPLE_BLOCK      (25)
#define SD_CMD_PROGRAM_CID               (26)
#define SD_CMD_PROGRAM_CSD               (27)
#define SD_CMD_SET_WRITE_PROT            (28)
#define SD_CMD_CLR_WRITE_PROT            (29)
#define SD_CMD_SEND_WRITE_PROT           (30)
#define SD_CMD_ERASE_WR_BLK_START        (32)
#define SD_CMD_ERASE_WR_BLK_END          (33)
#define SD_CMD_UNTAG_SECTOR              (34)
#define SD_CMD_TAG_ERASE_GROUP_START     (35)
#define SD_CMD_TAG_ERASE_GROUP_END       (36)
#define SD_CMD_UNTAG_ERASE_GROUP         (37)
#define SD_CMD_ERASE                     (38)
#define SD_CMD_FAST_IO                   (39)
#define SD_CMD_GO_IRQ_STATE              (40)
#define SD_CMD_SD_SEND_OP_COND           (41)
#define SD_CMD_LOCK_UNLOCK               (42)
#define SD_CMD_IO_RW_DIRECT              (52)
#define SD_CMD_IO_READ_EXT_SINGLE_BLOCK  (0x1c35)
#define SD_CMD_IO_READ_EXT_MULTI_BLOCK   (0x7c35)
#define SD_CMD_IO_WRITE_EXT_SINGLE_BLOCK (0x0c35)
#define SD_CMD_IO_WRITE_EXT_MULTI_BLOCK  (0x6c35)

#define SD_CMD_APP_CMD                   (55)
#define SD_CMD_GEN_CMD                   (56)
#define SD_CMD_READ_OCR                  (58)
#define SD_CMD_CRC_ON_OFF                (59)
#define SD_CMD_C_ACMD                    (1 << 6)   /* APP Command */

#define SD_CARD_SPEED_25MZ               (0x32)
#define SD_CARD_SPEED_50MZ               (0x5a)
#define SD_CARD_SPEED_100MZ              (0x0b)
#define SD_CARD_SPEED_200MZ              (0x2b)

#define SDMMC_DFLT_BLKSIZE               (512)
#define SDMMC_BLOCK_LENGTH               (SDMMC_DFLT_BLKSIZE / 4) /* Blocks are sent 4 bytes at a time */
#define  IF_COND_VOLTAGE                 (1UL)
#define  IF_COND_CHECK_PATTERN           (0xaa)

#define SD_SC_CAPACITY                   (0 << 30)
#define SD_HC_CAPACITY                   (1 << 30)

#define SD_VDD_2_7_V                     (1 << 15)
#define SD_VDD_2_8_V                     (1 << 16)
#define SD_VDD_2_9_V                     (1 << 17)
#define SD_VDD_3_0_V                     (1 << 18)
#define SD_VDD_3_1_V                     (1 << 19)
#define SD_VDD_3_2_V                     (1 << 20)
#define SD_VDD_3_3_V                     (1 << 21)
#define SD_VDD_3_4_V                     (1 << 22)
#define SD_VDD_3_5_V                     (1 << 23)
#define SD_VDD_SUPPORTED                                                        \
    (SD_VDD_2_7_V | SD_VDD_2_8_V | SD_VDD_2_9_V | SD_VDD_3_0_V | SD_VDD_3_1_V | \
     SD_VDD_3_2_V | SD_VDD_3_3_V | SD_VDD_3_4_V  | SD_VDD_3_5_V)

/* CMD52 Arguments */
#define SD_CMD52_READ                    (0)
#define SD_CMD52_WRITE                   (1)
#define SD_CMD52_NO_READ_AFTER_WRITE     (0)
#define SD_CMD52_READ_AFTER_WRITE        (1)


/* SWITCH command argument's bit postion */
#define ACCESS             (24)
#define INDEX              (16)
#define VALUE              (8)

#define EXT_CSD_SIZE                        (512)
#define SCR_SIZE                            (8)
#define STUFF_BITS                          (0x00)
#define SWITCH_STATUS_SIZE                  (64)
#define SWITCH_STATUS_BUFFER_SIZE           (32)
#define SWITCH_HIGH_SPEED_RESPONSE          (13)
#define SWITCH_HIGH_SPEED_ERROR_RESPONSE    (16)
#define SWITCH_HIGH_SPEED_ERROR             (0x0f)
#define SWITCH_HIGH_SPEED_OK                (0x02)


/* SD_CMD_SWITCH - for SD Cards
 * [31] Mode 0:Check function, 1:Switch function
 * [30:24] reserved (All '0')
 * [23:20] reserved for function group 6 (0h or Fh)
 * [19:16] reserved for function group 5 (0h or Fh)
 * [15:12] function group 4 for current limit
 * [11:8] function group 3 for drive strength
 * [7:4] function group 2 for command system
 * [3:0] function group 1 for access mode
 *
 */
#define MMC_CMD_SWITCH_WBUSY       (0x506)                      /* MMC CMD6 switch command "with response busy" */
#define MMC_CMD_SEND_EXT_CSD       (0x1c08)                     /* CMD 8, read data */
#define SD_CMD_SEND_SCR            (0x1c73)                     /* ACMD51, read data */
#define SD_HIGH_SPEED_MODE_SWITCH  (0x80FFFFF1)                 /* set SD high speed */
#define SD_HIGH_SPEED_MODE_CHECK   (0x00FFFFF1)                 /* check SD high speed function*/
#define EMMC_HIGH_SPEED_MODE       (0x03b90100)                 /* set eMMC high speed */
#define EMMC_HIGH_SPEED_CLOCK_RATE (52000000)                   /* 52 MHz */
#define SD_CLOCK_RATE              (25000000)                   /* 25 MHz */
#define SD_HIGH_SPEED_CLOCK_RATE   (50000000)                   /* 50 MHz */
#define SDIO_HIGH_SPEED_CLOCK_RATE (50000000)                   /* 50 MHz */

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_sd_state_type
{
    SD_STATE_IDLE,
    SD_STATE_READY,
    SD_STATE_IDENT,
    SD_STATE_STBY,
    SD_STATE_TRANSFER,
    SD_STATE_DATA,
    SD_STATE_RCV,
    SD_STATE_PROG,
    SD_STATE_DIS,
    SD_STATE_RSVD9,
    SD_STATE_RSVD10,
    SD_STATE_RSVD11,
    SD_STATE_RSVD12,
    SD_STATE_RSVD13,
    SD_STATE_RSVD14,
    SD_STATE_RSVD15
} sd_state_t;

typedef enum e_sd_response_type
{
    SD_RESPONSE_TYPE_R1,
    SD_RESPONSE_TYPE_R2,
    SD_RESPONSE_TYPE_R3,
    SD_RESPONSE_TYPE_R6,
    SD_RESPONSE_TYPE_R7
} sd_response_type_t;

typedef union
{
    uint32_t  status;
#if defined(__ICCARM__)
    __packed struct
#elif defined(  __GNUC__)
    struct __attribute__((__packed__))
#endif
    {
        uint16_t byte_0;

        uint16_t byte_1;
    } word;

#if defined(__ICCARM__)
    __packed struct
#elif defined(  __GNUC__)
    struct __attribute__((__packed__))
#endif
    {
        uint32_t   reserved_0         : 3;

        uint32_t   ake_seq_error      : 1;
        uint32_t   reserved_4         : 1;
        uint32_t   app_cmd            : 1;
        uint32_t   reserved_6         : 2;
        uint32_t   ready_for_data     : 1;
        sd_state_t current_state      : 4;
        uint32_t   erase_reset        : 1;
        uint32_t   card_ecc_disable   : 1;
        uint32_t   wp_erase_skip      : 1;
        uint32_t   csd_overwrite      : 1;
        uint32_t   reserved_17        : 2;
        uint32_t   error              : 1;
        uint32_t   cc_error           : 1;
        uint32_t   card_ecc_failed    : 1;
        uint32_t   illegal_command    : 1;
        uint32_t   com_crc_error      : 1;
        uint32_t   lock_unlock_failed : 1;
        uint32_t   card_is_locked     : 1;
        uint32_t   wp_violation       : 1;
        uint32_t   erase_param        : 1;
        uint32_t   erase_seq_error    : 1;
        uint32_t   block_len_error    : 1;
        uint32_t   address_error      : 1;
        uint32_t   out_of_range       : 1;
    } r1;
#if defined(__ICCARM__)
    __packed struct
#elif defined(  __GNUC__)
    struct __attribute__((__packed__))
#endif
    {
        uint32_t reserved_0           : 7;

        uint32_t reserved_lvr         : 1;
        uint32_t reserved_8           : 7;
        uint32_t v_27_28              : 1;
        uint32_t v_28_29              : 1;
        uint32_t v_29_30              : 1;
        uint32_t v_30_31              : 1;
        uint32_t v_31_32              : 1;
        uint32_t v_32_33              : 1;
        uint32_t v_33_34              : 1;
        uint32_t v_34_35              : 1;
        uint32_t v_35_36              : 1;
        uint32_t s18A                 : 1;
        uint32_t reserved_25          : 5;
        uint32_t card_capacity_status : 1;
        uint32_t power_up_status      : 1;
    } r3;
#if defined(__ICCARM__)
    __packed struct
#elif defined(  __GNUC__)
    struct __attribute__((__packed__))
#endif
    {
        uint32_t ocr          : 24;

        uint32_t reserved_24  : 3;
        uint32_t memory       : 1;
        uint32_t io_functions : 3;
        uint32_t ready        : 1;
    } r4;
#if defined(__ICCARM__)
    __packed struct
#elif defined(  __GNUC__)
    struct __attribute__((__packed__))
#endif
    {
        uint32_t read_write_data  : 8;

        uint32_t out_of_range     : 1;
        uint32_t invalid_function : 1;
        uint32_t rfu              : 1;
        uint32_t error            : 1;
        uint32_t current_state    : 2;
        uint32_t illegal_command  : 1;
        uint32_t crc_error        : 1;
        uint32_t reserved_16      : 16;
    } r5;
#if defined(__ICCARM__)
    __packed struct
#elif defined(  __GNUC__)
    struct __attribute__((__packed__))
#endif
    {
        uint32_t   reserved_0      : 3;

        uint32_t   ake_seq_error   : 1;
        uint32_t   reserved_4      : 1;
        uint32_t   app_cmd         : 1;
        uint32_t   reserved_6      : 2;
        uint32_t   ready_for_data  : 1;
        sd_state_t current_state   : 4;
        uint32_t   error           : 1;
        uint32_t   illegal_command : 1;
        uint32_t   com_crc_error   : 1;
        uint32_t   rca             : 16;                   // Relative Card Address
    } r6;
#if defined(__ICCARM__)
    __packed struct
#elif defined(  __GNUC__)
    struct __attribute__((__packed__))
#endif
    {
        uint32_t check_pattern    : 8;

        uint32_t voltage_accepted : 4;
        uint32_t reserved_11      : 20;
    } r7;
} card_status_t;

/** SDMMC card specific data */
typedef struct st_csd_reg
{
    union
    {
        uint8_t  array[16];

#if defined(__ICCARM__)
        __packed struct
#elif defined(  __GNUC__)
        struct __attribute__((__packed__))
#endif
        {
            uint32_t sdrsp10;

            uint32_t sdrsp32;
            uint32_t sdrsp54;
            uint32_t sdrsp76;
        } reg;
#if defined(__ICCARM__)
        __packed struct
#elif defined(  __GNUC__)
        struct __attribute__((__packed__))
#endif
        {
            uint32_t reserved_8         : 2;

            uint32_t file_format        : 2;              /**<   file format */
            uint32_t tmp_write_protect  : 1;              /**<   temporary write protection */
            uint32_t perm_write_protect : 1;              /**<   permanent write protection */
            uint32_t copy               : 1;              /**<   copy flag    */
            uint32_t file_format_grp    : 1;              /**<   file format group  */
            uint32_t reserved_20        : 5;
            uint32_t write_bl_partial   : 1;              /**<   partial blocks for write allowed  */
            uint32_t write_bl_len       : 4;              /**<   max. write data block length */
            uint32_t r2w_factor         : 3;              /**<   write speed factor  */
            uint32_t reserved_29        : 2;
            uint32_t wp_grp_enable      : 1;              /**<   write protect group enable */
            uint32_t wp_grp_size        : 7;              /**<   write protect group size */
            uint32_t sector_size        : 7;              /**<   erase sector size */
            uint32_t erase_blk_en       : 1;              /**<   erase single block enable */
            uint32_t c_size_mult        : 3;              /**<   device size multiplier*/
            uint32_t vdd_w_curr_max     : 3;              /**<   max. write current for vdd max */
            uint32_t vdd_w_curr_min     : 3;              /**<   max. write current for vdd min */
            uint32_t vdd_r_curr_max     : 3;              /**<   max. read current for vdd max */
            uint32_t vdd_r_curr_min     : 3;              /**<   max. read current for vdd min */
            uint32_t c_size             : 12;             /**<   device size  */
            uint32_t reserved_74        : 2;
            uint32_t dsr_imp            : 1;              /**<   dsr implemented */
            uint32_t read_blk_misalign  : 1;              /**<   read block misalignment  */
            uint32_t write_blk_misalign : 1;              /**<   write block misalignment */
            uint32_t read_bl_partial    : 1;              /**<   partial blocks for read allowed  */
            uint32_t read_bl_len        : 4;              /**<   max read data block length */
            uint32_t ccc                : 12;             /**<   card command classes */
            uint32_t tran_speed         : 8;              /**<   max. data transfer rate  */
            uint32_t nsac               : 8;              /**<   data read access-time-2 clk cycles */
            uint32_t taac               : 8;              /**<   data read access-time-1  */
            uint32_t reserved_120       : 6;
            uint32_t csd_structure      : 2;              /**<   csd structure     */
            uint32_t reserved_128       : 8;
        } csd_v1_b;

#if defined(__ICCARM__)
        __packed struct
#elif defined(  __GNUC__)
        struct __attribute__((__packed__))
#endif
        {
            uint32_t reserved_8         : 2;

            uint32_t file_format        : 2;                  /**<   file format */
            uint32_t tmp_write_protect  : 1;                  /**<   temporary write protection */
            uint32_t perm_write_protect : 1;                  /**<   permanent write protection */
            uint32_t copy               : 1;                  /**<   copy flag    */
            uint32_t file_format_grp    : 1;                  /**<   file format group  */
            uint32_t reserved_20        : 5;
            uint32_t write_bl_partial   : 1;                  /**<   partial blocks for write allowed  */
            uint32_t write_bl_len       : 4;                  /**<   max. write data block length */
            uint32_t r2w_factor         : 3;                  /**<   write speed factor  */
            uint32_t reserved_29        : 2;
            uint32_t wp_grp_enable      : 1;                  /**<   write protect group enable */
            uint32_t wp_grp_size        : 7;                  /**<   write protect group size */
            uint32_t sector_size        : 7;                  /**<   erase sector size */
            uint32_t erase_blk_en       : 1;                  /**<   erase single block enable */
            uint32_t reserved_47        : 1;
            uint32_t c_size             : 22;                 /**<   device size  */
            uint32_t reserved_70        : 6;
            uint32_t dsr_imp            : 1;                  /**<   dsr implemented */
            uint32_t read_blk_misalign  : 1;                  /**<   read block misalignment  */
            uint32_t write_blk_misalign : 1;                  /**<   write block misalignment */
            uint32_t read_bl_partial    : 1;                  /**<   partial blocks for read allowed  */
            uint32_t read_bl_len        : 4;                  /**<   max read data block length */
            uint32_t ccc                : 12;                 /**<   card command classes */
            uint32_t tran_speed         : 8;                  /**<   max. data transfer rate  */
            uint32_t nsac               : 8;                  /**<   data read access-time-2 clk cycles */
            uint32_t taac               : 8;                  /**<   data read access-time-1  */
            uint32_t reserved_120       : 6;
            uint32_t csd_structure      : 2;                  /**<   csd structure     */
            uint32_t reserved_128       : 8;
        } csd_v2_b;
#if defined(__ICCARM__)
        __packed struct
#elif defined(  __GNUC__)
        struct __attribute__((__packed__))
#endif
        {
            uint32_t reserved_0       : 1;

            uint32_t crc              : 7;
            uint32_t mfg_date         : 12;
            uint32_t reserved_23      : 4;
            uint32_t serial_number    : 32;
            uint32_t product_revision : 8;
            uint64_t product_name     : 40;
            uint32_t oem_id           : 16;
            uint32_t mfg_id           : 8;
            uint32_t reserved_128     : 8;
        } cid;
    };
} csd_reg_t;

/** SDMMC card specific data extended */
typedef struct st_csd_reg_ext
{
    union
    {
        uint8_t  array[512];

#if defined(__ICCARM__)
        __packed struct
#elif defined(  __GNUC__)
        struct __attribute__((__packed__))
#endif
        {
            uint8_t reserved_0[494];                    /**< */

            uint8_t large_unit_size_m1;                 /**< */
            uint8_t context_capabilities;               /**< */
            uint8_t tag_res_size;                       /**< */
            uint8_t tag_unit_size;                      /**< */
            uint8_t data_tag_support;                   /**< */
            uint8_t max_packed_writes;                  /**< */
            uint8_t max_packed_reads;                   /**< */
            uint8_t bkops_support;                      /**< */
            uint8_t hpi_features;                       /**< */
            uint8_t s_cmd_set;                          /**< */
            uint8_t ext_security_err;                   /**< */
            uint8_t reserved_506[6];                    /**< */
            uint8_t reserved_512;                       /**< */
        } csd_ext;
    };
} csd_reg_ext_t;

/** SD Card  Configuration Register */
typedef struct st_scr_reg
{
    union
    {
        uint32_t  word[2];

#if defined(__ICCARM__)
        __packed struct
#elif defined(  __GNUC__)
        struct __attribute__((__packed__))
#endif
        {
            uint32_t sd_spec               : 4;                 /**< SD Memory Card - Spec. Version */
            uint32_t scr_structure         : 4;                 /**< SCR structure */

            uint32_t sd_bus_widths         : 4;                 /**< DAT bus widths supported */
            uint32_t sd_security           : 3;                 /**< CPRM Security support */
            uint32_t data_stat_after_erase : 1;                 /**< data status after erases */

            uint32_t reserved_2            : 2;                  /**< Reserved */
            uint32_t sd_spec4              : 1;                 /**< Spec. Version 4.XX */
            uint32_t ex_security           : 4;                 /**< Extended security support */
            uint32_t sd_spec3              : 1;                 /**< Spec. Version 3.00 or higher */

            uint32_t cmd_support           : 4;                 /**< Command support bits */
            uint32_t reserved_4            : 4;                 /**< Reserved */
            uint32_t reserved_0            : 32;                /**< reserved for manufacturer usage*/
        } scr_bit;
    };
} scr_reg_t;

typedef union
{
    /* CMD52(IO_RW) Argument */
    uint32_t  arg;
    struct
    {
        uint32_t  data             : 8;  /* read/write data */
        uint32_t  stuff_8          : 1;  /* Stuff bit */
        uint32_t  register_address : 17; /* Register Address */
        uint32_t  stuff_26         : 1;  /* Stuff bit should be set to 1 */
        uint32_t  raw              : 1;  /* 1= Read after write. */
        uint32_t  function_number  : 3;  /* function0~function7 */
        uint32_t  rw_flag          : 1;  /* 0:read, 1:write */
    }  cmd_52_arg;

    struct
    {
        uint32_t  count            : 9;  /* Byte/Block Count */
        uint32_t  register_address : 17; /* Register Address */
        uint32_t  op_code          : 1;  /* 0:fixed address, 1:incrementing address */
        uint32_t  block_mode       : 1;  /* 0: Byte, 1:Block */
        uint32_t  function_number  : 3;  /* function0~function7 */
        uint32_t  rw_flag          : 1;  /* 0:read, 1:write */
    }  cmd_53_arg;
} sdio_arg_t;
#endif // ifndef SDCARD_H
