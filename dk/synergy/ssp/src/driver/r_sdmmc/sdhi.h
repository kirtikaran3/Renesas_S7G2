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
 * File Name    : sdhi.h
 * Description  : Hardware related helper functions for the SDMMC protocol.
 **********************************************************************************************************************/
#ifndef SDHI_H_
#define SDHI_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "bsp_api.h"
#include "sdcard.h"
#include "sdmmc_driver.h"
#include "r_sdmmc_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define SDHI_TIMEOUT                  ((uint32_t)10000)    // Timeout loop counter for polling
#define SDHI_ERASE_TIMEOUT            ((uint32_t)100000)   // Timeout loop counter for erase command
#define SDHI_DELAY_COMMAND_SEND       ((uint32_t)500)
#define SDHI_DELAY_BUSY_GET           ((uint32_t)500)
#define SDHI_DELAY_RESET              ((uint32_t)5000)
#define SDHI_DELAY_RESPONSE_GET       ((uint32_t)10)
#define INSERT_TIME                   ((uint32_t)25)

/* SD_INFO1 */
#define SDHI_INFO1_RESPONSE_END       (uint32_t) (1 << 0)     //< Response End
#define SDHI_INFO1_ACCESS_END         (uint32_t) (1 << 2)     //< Access End
#define SDHI_INFO1_CARD_REMOVED       (uint32_t) (1 << 3)     //< Card Removed
#define SDHI_INFO1_CARD_INSERTED      (uint32_t) (1 << 4)     //< Card Inserted
#define SDHI_INFO1_CARD_WRITE_PROTECT (uint32_t) (1 << 7)     //< Card is Write Protected
#define SDHI_INFO1_CARD_DAT3_REMOVED  (uint32_t) (1 << 8)     //< Card Removed
#define SDHI_INFO1_CARD_DAT3_INSERTED (uint32_t) (1 << 9)     //< Card Inserted
#define SDHI_INFO1_CARD_STATE         (uint32_t) (1 << 10)    //< Card Insert State
#define SDHI_INFO2_CARD_CMD_ERR       (uint32_t) (1 << 0)     //< Command error
#define SDHI_INFO2_CARD_CRC_ERR       (uint32_t) (1 << 1)     //< CRC error
#define SDHI_INFO2_CARD_END_ERR       (uint32_t) (1 << 2)     //< End bit error
#define SDHI_INFO2_CARD_DTO           (uint32_t) (1 << 3)     //< Data Timeout
#define SDHI_INFO2_CARD_ILW           (uint32_t) (1 << 4)     //< Illegal write address
#define SDHI_INFO2_CARD_ILR           (uint32_t) (1 << 5)     //< Illegal read address
#define SDHI_INFO2_CARD_RSPT          (uint32_t) (1 << 6)     //< Response timeout
#define SDHI_INFO2_CARD_BRE           (uint32_t) (1 << 8)     //< Buffer read enable
#define SDHI_INFO2_CARD_BWE           (uint32_t) (1 << 9)     //< Buffer write enable
#define SDHI_INFO2_CARD_CMDBUSY       (uint32_t) (1 << 14)    //< Command busy
#define SDHI_INFO2_CARD_ILA_ERR       (uint32_t) (1 << 15)    //< Illegal access
#define SDIO_INFO1_IOIRQ              (uint32_t) (1 << 0)     //< SDIO interrupt status
#define SDIO_INFO1_EXPUB52            (uint32_t) (1 << 14)    //< SDIO EXPUB52 status
#define SDIO_INFO1_EXWT               (uint32_t) (1 << 15)    //< SDIO EXWT status

#ifdef BSP_MCU_GROUP_S7G2                          /* SDINFO1 mask for boards with SD card detection */
#define SDHI_INFO1_MASK   (uint32_t)                                                            \
    (~(SDHI_INFO1_RESPONSE_END | SDHI_INFO1_ACCESS_END | SDHI_INFO1_CARD_REMOVED | \
       SDHI_INFO1_CARD_INSERTED))
#else                                         /* SDINFO1 mask for boards without SD card detection */
#define SDHI_INFO1_MASK (uint32_t) (~(SDHI_INFO1_RESPONSE_END | SDHI_INFO1_ACCESS_END))
#endif

#define SDHI_INFO2_MASK (uint32_t)                                                                                   \
    (~(SDHI_INFO2_CARD_CMD_ERR | SDHI_INFO2_CARD_CRC_ERR  | SDHI_INFO2_CARD_END_ERR |                                \
       SDHI_INFO2_CARD_DTO | SDHI_INFO2_CARD_ILW | SDHI_INFO2_CARD_ILR | SDHI_INFO2_CARD_BRE | SDHI_INFO2_CARD_BWE | \
       SDHI_INFO2_CARD_RSPT | SDHI_INFO2_CARD_ILA_ERR))

#define SDHI_INFO2_DMA_MASK   (uint32_t)                                                        \
    (~(SDHI_INFO2_CARD_CMD_ERR | SDHI_INFO2_CARD_CRC_ERR  | SDHI_INFO2_CARD_END_ERR |           \
       SDHI_INFO2_CARD_DTO | SDHI_INFO2_CARD_ILW | SDHI_INFO2_CARD_ILR | SDHI_INFO2_CARD_RSPT | \
       SDHI_INFO2_CARD_ILA_ERR))

#define SDHI_INFO1_ACCESS_MASK  (uint32_t) (~(SDHI_INFO1_RESPONSE_END | SDHI_INFO1_ACCESS_END))
#define SDHI_INFO1_CARD_MASK    (uint32_t) (~(SDHI_INFO1_CARD_REMOVED | SDHI_INFO1_CARD_INSERTED))

#define SDHI_SDIO_INFO1_MASK   (uint32_t) (~(SDIO_INFO1_IOIRQ))
#define SDHI_SDIO_ALL_MASK     (uint32_t) (0xffff)
#define SD_MAX_CLOCK_DIVIDER   (512)

#define SD_NO_OF_DIVIDERS      (10)
#define SD_CLK_CTRL_DIV512     (1 << 7)             //  pclka / 512
#define SD_CLK_CTRL_DIV256     (1 << 6)             //  pclka / 256 - default
#define SD_CLK_CTRL_DIV128     (1 << 5)             //  pclka / 128
#define SD_CLK_CTRL_DIV64      (1 << 4)             //  pclka / 64
#define SD_CLK_CTRL_DIV32      (1 << 3)             //  pclka / 32
#define SD_CLK_CTRL_DIV16      (1 << 2)             //  pclka / 16
#define SD_CLK_CTRL_DIV8       (1 << 1)             //  pclka / 8
#define SD_CLK_CTRL_DIV4       (1 << 0)             //  pclka / 4
#define SD_CLK_CTRL_DIV2       0                    //  pclka / 2
#define SD_CLK_CTRL_DIV        0xff                 //  pclka

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef struct s_sdhi_int_status_type
{
    uint32_t  info2_mask;
} sdhi_int_status_t;

typedef union
{
    uint32_t  word;

    struct s_sdhi_event_type
    {
        uint32_t  response_end       : 1;          //< response end detected
        uint32_t  reserved_1         : 1;
        uint32_t  access_end         : 1;          //< access end detected
        uint32_t  card_removed       : 1;          //< card removal detected by cd pin
        uint32_t  card_inserted      : 1;          //< card insertion detected by cd pin
        uint32_t  reserved_5         : 3;
        uint32_t  card_dat3_removed  : 1;          //< card removal detected by dat3 pin
        uint32_t  card_dat3_inserted : 1;          //< card inserion detected by dat3 pin
        uint32_t  reservered_10      : 6;
        uint32_t  cmd_err            : 1;          //< command error
        uint32_t  crc_err            : 1;          //< crc error
        uint32_t  end_err            : 1;          //< end bit error
        uint32_t  dto                : 1;          //< data timeout
        uint32_t  ilw                : 1;          //< illegal write address
        uint32_t  ilr                : 1;          //< illegal read address
        uint32_t  rspt               : 1;          //< response timeout
        uint32_t  event_error        : 1;          //< all error flags combined
        uint32_t  bre                : 1;          //< buffer read enable
        uint32_t  bwe                : 1;          //< buffer write enable
        uint32_t  reserved_26        : 5;
        uint32_t  ila_err            : 1;          //< illegal access
    }  bit;
} sdhi_event_t;

typedef union
{
    uint32_t  word;

    struct s_sdhi_sdio_event_type
    {
        uint32_t  ioirq       : 1;                 //< sdio interrupt status
        uint32_t  event_error : 1;
        uint32_t  reserved_2  : 12;
        uint32_t  expub52     : 1;                 //< EXPUB52 status
        uint32_t  exwt        : 1;                 //< sdio exwt status
        uint32_t  reserved_16 : 16;
    }  bit;
} sdhi_sdio_event_t;

typedef enum e_sdhi_timeout
{
    SDHI_TIMEOUT_2_13,
    SDHI_TIMEOUT_2_14,
    SDHI_TIMEOUT_2_15,
    SDHI_TIMEOUT_2_16,
    SDHI_TIMEOUT_2_17,
    SDHI_TIMEOUT_2_18,
    SDHI_TIMEOUT_2_19,
    SDHI_TIMEOUT_2_20,
    SDHI_TIMEOUT_2_21,
    SDHI_TIMEOUT_2_22,
    SDHI_TIMEOUT_2_23,
    SDHI_TIMEOUT_2_24,
    SDHI_TIMEOUT_2_25,
    SDHI_TIMEOUT_2_26,
    SDHI_TIMEOUT_2_27
} sdhi_timeout_t;

typedef enum e_sdhi_cd_timeout
{
    SDHI_CD_TIMEOUT_2_10,
    SDHI_CD_TIMEOUT_2_11,
    SDHI_CD_TIMEOUT_2_12,
    SDHI_CD_TIMEOUT_2_13,
    SDHI_CD_TIMEOUT_2_14,
    SDHI_CD_TIMEOUT_2_15,
    SDHI_CD_TIMEOUT_2_16,
    SDHI_CD_TIMEOUT_2_17,
    SDHI_CD_TIMEOUT_2_18,
    SDHI_CD_TIMEOUT_2_19,
    SDHI_CD_TIMEOUT_2_20,
    SDHI_CD_TIMEOUT_2_21,
    SDHI_CD_TIMEOUT_2_22,
    SDHI_CD_TIMEOUT_2_23,
    SDHI_CD_TIMEOUT_2_24
} sdhi_cd_timeout_t;

/***********************************************************************************************************************
 * Function prototypes
 **********************************************************************************************************************/
void     SDHI_DmaTriggerClear (uint8_t channel);

bool     SDHI_DmaModeSet (uint8_t channel, bool enable);

void     SDHI_InterruptFlagClear (uint8_t channel, sdhi_event_t flag);

void     SDHI_InterruptFlagsClear (uint8_t channel);

void     SDHI_EventFlagsClear (uint8_t channel);

bool     SDHI_BitEventErrorGet (uint8_t channel);

bool     SDHI_ReadBufferReadyGet (uint8_t channel);

void     SDHI_ReadBufferReadyClear (uint8_t channel);

bool     SDHI_WriteBufferReadyGet (uint8_t channel);

void     SDHI_WriteBufferReadyClear (uint8_t channel);

bool     SDHI_AccessEndGet (uint8_t channel);

void     SDHI_AccessEndClear (uint8_t channel);

bool     SDHI_ResponseEndGet (uint8_t channel);

void     SDHI_ResponseEndClear (uint8_t channel);

bool     SDHI_ErrorGet (uint8_t channel);

void     SDHI_SdioISR (uint8_t channel);

bool     SDHI_Sdio_Int_Enable (uint8_t channel,  bool enable);

bool     SDHI_CardRemovedGet (uint8_t channel);

void     SDHI_AccessISR (uint8_t channel, sdhi_event_t * event_flags);

void     SDHI_SdioEventFlagsUpdate (uint8_t channel);

bool     SDHI_CommandSend (uint8_t channel, uint16_t command, uint32_t argument);

bool     SDHI_CardBusyGet (uint8_t channel);

uint32_t SDHI_SD_VersionGet (void);

bool     SDHI_Init (uint8_t channel);

void     SDHI_DataRead (uint8_t channel, uint32_t * const data);

void     SDHI_SetArgument (uint8_t channel, uint32_t argument);

void     SDHI_StopData (uint8_t channel, uint8_t IsStop);

bool     SDHI_SDIO_Mode (uint8_t channel, bool enable);

void     SDHI_DataWrite (uint8_t channel, uint32_t * const data);

bool     SDHI_EndProcessData (uint8_t channel);

bool     SDHI_BlockSizeSet (uint8_t channel, uint16_t block_size);

bool     SDHI_BlockCountSet (uint8_t channel, uint32_t block_count);

void     SDHI_BusWidthSet (uint8_t channel, uint8_t bw);

void     SDHI_ResponseGet (uint8_t channel, card_status_t * const SD_Response);

void     SDHI_ResponseMultipleBlockGet (uint8_t channel, card_status_t * const sd_response);

void     SDHI_R2ResponseGet (uint8_t channel, csd_reg_t * const csd);

int32_t  SDHI_Open (uint8_t channel);

int32_t  SDHI_Close (uint8_t channel);

uint16_t SDHI_GetBlockCount (uint8_t channel);

uint16_t SDHI_GetBlockSize (uint8_t channel);

uint32_t SDHI_GetInterruptMask (uint8_t channel);

bool     SDHI_MaxClockFreqSet (uint8_t channel, uint32_t * const rate);

bool     SDHI_ClockRateSet (uint8_t channel, uint8_t divider);

bool     SDHI_ClockEnable (uint8_t channel);

bool     SDHI_ClockDisable (uint8_t channel);

int32_t  SDHI_PowerDown (uint8_t channel);

int32_t  SDHI_PowerUp (uint8_t channel);

uint32_t SDHI_ErrorStatus (uint8_t channel);

bool     SDHI_WriteProtectGet (uint8_t channel);

bool     SDHI_CardInsertStatusGet (uint8_t channel);
#endif  /* _SDHI_H */
