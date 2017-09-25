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
 * File Name    : sdhi.c
 * Description  : SD Host Interface hardware related helper functions for the SDMMC protocol.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "sdhi.h"
#include ".\hw\hw_sdmmc_private.h"
#include "r_cgc.h"
#include "r_ioport.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static bool command_busy_get (uint8_t channel);

static bool clock_div_enable_get (uint8_t channel);

static void delay (uint32_t time);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
volatile sdhi_event_t      g_sdhi_event_flag[SDMMC_MAX_NUM_CHANNELS];
volatile sdhi_sdio_event_t g_sdhi_sdio_event_flag[SDMMC_MAX_NUM_CHANNELS];
static sdhi_int_status_t   g_int_status[SDMMC_MAX_NUM_CHANNELS];

static const uint8_t      g_sd_divider[SD_NO_OF_DIVIDERS] =
{
    SD_CLK_CTRL_DIV512,
    SD_CLK_CTRL_DIV256,
    SD_CLK_CTRL_DIV128,
    SD_CLK_CTRL_DIV64,
    SD_CLK_CTRL_DIV32,
    SD_CLK_CTRL_DIV16,
    SD_CLK_CTRL_DIV8,
    SD_CLK_CTRL_DIV4,
    SD_CLK_CTRL_DIV2,
    SD_CLK_CTRL_DIV
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  SDHI_DmaModeSet Setup DMA mode.
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval true if successful.
 **********************************************************************************************************************/
bool SDHI_DmaModeSet (uint8_t channel, bool enable)
{
    if (true == enable)
    {
        if (command_busy_get(channel))
        {
            return false;        /* command stayed busy over timeout */
        }

        HW_SDMMC_DMAModeEnable(channel, true);
        HW_SDMMC_InterruptMaskInfo2Set(channel, SDHI_INFO2_DMA_MASK);     /* Mask interrupts */
        g_int_status[channel].info2_mask = ~SDHI_INFO2_DMA_MASK;
        HW_SDMMC_StatusClear(channel);
        SDHI_DmaTriggerClear(channel);
        return true;
    }
    else
    {
        if (command_busy_get(channel))
        {
            return false;        /* command stayed busy over timeout */
        }

        HW_SDMMC_DMAModeEnable(channel, false);
        HW_SDMMC_InterruptMaskInfo2Set(channel, SDHI_INFO2_MASK);     /* Mask interrupts */
        g_int_status[channel].info2_mask = ~SDHI_INFO2_MASK;
        HW_SDMMC_StatusClear(channel);
        SDHI_DmaTriggerClear(channel);
        return true;
    }
}

/*******************************************************************************************************************//**
 * @brief  SDHI_DmaTriggerClear Clear IRQ.
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval none
 **********************************************************************************************************************/
void SDHI_DmaTriggerClear (uint8_t channel)
{
    IRQn_Type irq;
    irq = HW_SDMMC_DmaIrqGet(channel);
    if (irq != 0)
    {
        R_BSP_IrqStatusClear(irq);
    }
}

/*******************************************************************************************************************//**
 * @brief  SDHI_InterruptFlagsClear SD Host Interrupt Flag.
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval none
 **********************************************************************************************************************/
void SDHI_InterruptFlagsClear (uint8_t channel)
{
    /* Read interrupt flag registers. */
    uint32_t info1;
    uint32_t info2;
    uint32_t info3;

    info1  = HW_SDMMC_Info1Get(channel);
    info2  = HW_SDMMC_Info2Get(channel);
    info3  = HW_SDMMC_SDIO_Info1Get(channel);

    info1 &= ~SDHI_INFO1_MASK;                                          /* Mask off status bits. */
    info2 &= ~SDHI_INFO2_MASK;
    info3 &= ~SDHI_SDIO_INFO1_MASK;

    if (info1)
    {
        HW_SDMMC_Info1Set(channel, info1);                              /* Clear interrupt flags */
    }

    if (info2)
    {
        HW_SDMMC_Info2Set(channel, info2);                              /* Clear interrupt flags */
    }

    if (info3)
    {
        HW_SDMMC_SDIO_Info1Set(channel, info3);                             /* Clear interrupt flags */
    }
}

/*******************************************************************************************************************//**
 * @brief  SDHI_AccessISR SD Host Interrupt Flag.
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval access interrupt flags
 **********************************************************************************************************************/

void SDHI_AccessISR (uint8_t channel, sdhi_event_t * event_flags)
{
    uint32_t       info1;
    uint32_t       info2;
    sdhi_event_t   flags;
    flags.word = g_sdhi_event_flag[channel].word;

    /* Read interrupt flag registers. */

    info1        = HW_SDMMC_Info1Get(channel);
    info2        = HW_SDMMC_Info2Get(channel);

    info1       &= ~SDHI_INFO1_ACCESS_MASK;                                     /* Mask off status bits. */
    info2       &= g_int_status[channel].info2_mask;

    flags.word |= (info1 | (info2 << 16));                            /* Combine all flags in one 32 bit word. */
    if (flags.word & SDMMC_ACCESS_ERROR_MASK)                         /* Combine only errors in one bit. */
    {
        flags.bit.event_error = 1;
    }

    if (flags.bit.bre)
    {
        HW_SDMMC_InterruptMaskInfo2Set(channel, SDHI_INFO2_MASK);     /* Mask interrupts */
    }

    g_sdhi_event_flag[channel] = flags;
    *event_flags = flags;

    if (info1)
    {
        HW_SDMMC_Info1Set(channel, info1);                              /* Clear interrupt flags */
    }

    if (info2)
    {
        HW_SDMMC_Info2Set(channel, info2);                              /* Clear interrupt flags */
    }
}

/*******************************************************************************************************************//**
 * @brief  SDHI_SdioEventFlagsUpdate SD Host Interrupt Flag.
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval access interrupt flags
 **********************************************************************************************************************/

void SDHI_SdioEventFlagsUpdate (uint8_t channel)
{
    sdhi_sdio_event_t result;
    uint32_t          info1;
    result.word = g_sdhi_sdio_event_flag[channel].word;

    /* Read interrupt flag registers. */

    info1                           = HW_SDMMC_SDIO_Info1Get(channel);

    info1                          &= ~SDHI_SDIO_INFO1_MASK; /* Mask off status bits. */

    result.word                    |= info1;                 /* Combine all flags in one 32 bit word. */

    g_sdhi_sdio_event_flag[channel] = result;
    HW_SDMMC_InterruptMaskSdioInfo1Set(channel, SDHI_SDIO_ALL_MASK);         /* disable interrupt, user must re-enable */
    if (info1)
    {
        HW_SDMMC_SDIO_Info1Set(channel, info1);                              /* Clear interrupt flags */
    }
}

/*******************************************************************************************************************//**
 * @brief  SDHI_EventFlagsClear SD Host Event Flags.
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval none
 **********************************************************************************************************************/

void SDHI_EventFlagsClear (uint8_t channel)
{
    g_sdhi_event_flag[channel].word = (uint32_t)0x00;
}

/*******************************************************************************************************************//**
 * @brief  SDHI_BitEventErrorGet
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval                true if bit set.
 **********************************************************************************************************************/
bool SDHI_BitEventErrorGet (uint8_t channel)
{
    return (bool) g_sdhi_event_flag[channel].bit.event_error;
}

/*******************************************************************************************************************//**
 * @brief  SDHI_ReadBufferReadyGet
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval                true if bit set.
 **********************************************************************************************************************/
bool SDHI_ReadBufferReadyGet (uint8_t channel)
{
    bool ret_val;
    ret_val = false;
    if ((HW_SDMMC_ReadBufferReadyGet(channel)) || (g_sdhi_event_flag[channel].bit.bre))
    {
        ret_val =  true;
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 * @brief  SDHI_ReadBufferReadyClear
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval                none
 **********************************************************************************************************************/
void SDHI_ReadBufferReadyClear (uint8_t channel)
{
    g_sdhi_event_flag[channel].bit.bre = (uint32_t)0x00;
}

/*******************************************************************************************************************//**
 * @brief  SDHI_WriteBufferReadyGet
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval                true if bit set.
 **********************************************************************************************************************/
bool SDHI_WriteBufferReadyGet (uint8_t channel)
{
    return (bool)g_sdhi_event_flag[channel].bit.bwe;
}

/*******************************************************************************************************************//**
 * @brief  SDHI_WriteBufferReadyClear
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval                none
 **********************************************************************************************************************/
void SDHI_WriteBufferReadyClear (uint8_t channel)
{
    g_sdhi_event_flag[channel].bit.bwe = (uint32_t)0x00;
}

/*******************************************************************************************************************//**
 * @brief  SDHI_AccessEndGet
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval                true if bit set.
 **********************************************************************************************************************/
bool SDHI_AccessEndGet (uint8_t channel)
{
    return (bool)g_sdhi_event_flag[channel].bit.access_end;
}

/*******************************************************************************************************************//**
 * @brief  SDHI_AccessEndClear
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval                none
 **********************************************************************************************************************/
void SDHI_AccessEndClear (uint8_t channel)
{
    g_sdhi_event_flag[channel].bit.access_end = (uint32_t)0x00;
}

/*******************************************************************************************************************//**
 * @brief  SDHI_ResponseEndGet
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval                true if bit set.
 **********************************************************************************************************************/
bool SDHI_ResponseEndGet (uint8_t channel)
{
    return (bool)g_sdhi_event_flag[channel].bit.response_end;
}

/*******************************************************************************************************************//**
 *   This function clears the response end flag.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void SDHI_ResponseEndClear (uint8_t channel)
{
    g_sdhi_event_flag[channel].bit.response_end = (uint32_t)0x00;
}

/*******************************************************************************************************************//**
 * @brief  SDHI_ErrorGet
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @retval                true if bit set.
 **********************************************************************************************************************/
bool SDHI_ErrorGet (uint8_t channel)
{
    return (bool)g_sdhi_event_flag[channel].bit.event_error;
}


/*******************************************************************************************************************//**
 *   Interrupt for SDIO data response.
 *   @param[in] channel     Channel of SDHI.
 *   @retval none
 **********************************************************************************************************************/
void SDHI_SdioISR (uint8_t channel)
{
    SDHI_SdioEventFlagsUpdate(channel);
}

/*******************************************************************************************************************//**
 *   Enable interrupt for SDIO.
 *   @param[in] channel     Channel of SDHI.
 *   @retval none
 **********************************************************************************************************************/

bool     SDHI_Sdio_Int_Enable (uint8_t channel,  bool enable)
{
    uint32_t mask;
    if (true == enable)
    {
        mask = SDHI_SDIO_INFO1_MASK;

    }
    else
    {
        mask = SDHI_SDIO_ALL_MASK;
    }
    HW_SDMMC_InterruptMaskSdioInfo1Set(channel, mask);
    return true;
}

/*******************************************************************************************************************//**
 *   Interrupt function for card insertion/removal.
 *   @param[in] channel     Channel of SDHI.
 *   @retval none
 **********************************************************************************************************************/
bool SDHI_CardRemovedGet (uint8_t channel)
{
    sdhi_event_t info1;
    bool         ret_val = false;
    info1.word  = HW_SDMMC_Info1Get(channel);

    info1.word &= ~SDHI_INFO1_CARD_MASK;                                     /* Mask off status bits. */
    if (info1.bit.card_removed)
    {
        ret_val = true;
    }

    if (info1.word)
    {
        HW_SDMMC_Info1Set(channel, info1.word);                              /* Clear interrupt flags */
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 * @brief  Send a command to the SD or MMC card.
 *
 * The SDHI_CommandSend function sends a command to the SD or MMC card.
 *
 * .
 * @param[in]  channel    channel of SD Host Interface (0 or 1)
 * @param[in] command   SDHI command to send
 * @param[in] argument  Extra information used with the command
 *
 * @retval true             send command succeeded
 * @retval false            send command failed
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 *
 **********************************************************************************************************************/

bool SDHI_CommandSend (uint8_t channel, uint16_t command, uint32_t argument)
{
    bool              error;
    volatile uint32_t i;
    bool              ret_val;
    ret_val = true;

    /* Wait for CBSY to clear */

    if (command_busy_get(channel))
    {
        ret_val = false;       /* command stayed busy over timeout */
    }

    if (true == ret_val)
    {
        /* Clear Status */
        SDHI_EventFlagsClear(channel);

        /* Enable Clock */

        if (!(SDHI_ClockEnable(channel)))
        {
            ret_val = false;         /* clock failed to turn on */
        }
    }

    if (true == ret_val)
    {
        /* Setup argument registers */
        HW_SDMMC_SetArguments(channel, argument);

        HW_SDMMC_CommandSend(channel, command); /* write command to command register */

        /* wait for end of response, error or timeout */
        if (SD_CMD_ERASE == command)
        {
            i = SDHI_ERASE_TIMEOUT;         /* Set longer timeout for erase command. */
        }
        else
        {
            i = SDHI_TIMEOUT;               /* Set normal timeout. */
        }
        error = false;
        while (((!(SDHI_ResponseEndGet(channel))) && (false == error)) && i)
        {
            i--;
            delay(SDHI_DELAY_RESPONSE_GET);
            error = SDHI_ErrorGet(channel);
        }

        SDHI_ResponseEndClear(channel);

        /* Check response, return error if one exists or if timed out */
        if (error || (0 == i))
        {
            ret_val = false;
        }
    }

    return ret_val;
}

/*******************************************************************************************************************//**
 *   Get busy status of card.
 *   @param[in]  channel    channel of SD Host Interface (0 or 1)
 *   @retval true - card is busy
 *   @retval false - card is not busy
 **********************************************************************************************************************/

bool SDHI_CardBusyGet (uint8_t channel)
{
    volatile uint32_t i;
    i = SDHI_TIMEOUT;

    /* Wait for CBSY to clear */
    while ((!(HW_SDMMC_CardBusyGet(channel))) && i) /* DAT0 stays at 0, card is busy  */
    {
        i--;
        delay(SDHI_DELAY_BUSY_GET);
    }

    /* return error if timed out */
    if (0 == i)
    {
        return true;
    }

    return false;
}

/*******************************************************************************************************************//**
 *   Get SD Host Interface hardware version.
 *   @param[in]  none
 *   @retval sd version
 **********************************************************************************************************************/

uint32_t SDHI_SD_VersionGet (void)
{
    return HW_SDMMC_VersionGet();
}

/*******************************************************************************************************************//**
 *   Initialize SD Host Interface hardware.
 *   @param[in]  channel    channel of SD Host Interface (0 or 1)
 *   @retval true - no errors
 *   @retval false - hardware failed to respond properly
 **********************************************************************************************************************/

bool SDHI_Init (uint8_t channel)
{
    /* Set Card detect time counter and timeout counter to maximum */
    if (command_busy_get(channel))
    {
        return false;                               /* cannot write timeout counters while command busy */
    }
    HW_SDMMC_RegisterDefaultSet (channel);          /* Set normally untouched registers to default */
    HW_SDMMC_CardDetectTimeCounterSet(channel, SDHI_CD_TIMEOUT_2_10);
    HW_SDMMC_TimeoutCounterSet(channel, SDHI_TIMEOUT_2_27);
    if (!SDHI_ClockEnable(channel))
    {
        return false;
    }

    HW_SDMMC_ResetAssert(channel);
    delay(SDHI_DELAY_RESET);
    HW_SDMMC_ResetRelease(channel);
    delay(SDHI_DELAY_RESET);

    HW_SDMMC_InterruptMaskInfo1Set(channel, SDHI_INFO1_MASK);
    HW_SDMMC_InterruptMaskInfo2Set(channel, SDHI_INFO2_MASK);
    HW_SDMMC_InterruptMaskSdioInfo1Set(channel, SDHI_SDIO_ALL_MASK); /* SDIO interrupt is masked initially */
    g_int_status[channel].info2_mask      = ~SDHI_INFO2_MASK;
    if ((uint8_t)SDMMC_CHANNEL_0 == channel )
    {
#if (BSP_IRQ_CFG_SDHIMMC0_ACCS  == BSP_IRQ_DISABLED)
        return false;                                               /* Fail init if interrupt disabled. */
#endif
    }
    if ((uint8_t)SDMMC_CHANNEL_1 == channel )
    {
#if (BSP_IRQ_CFG_SDHIMMC1_ACCS  == BSP_IRQ_DISABLED)
        return false;                                               /* Fail init if interrupt disabled. */
#endif
    }

    return true;
}

/*******************************************************************************************************************//**
 *   Check write protect pin.
 *   @param[in]  channel    channel of SD Host Interface (0 or 1)
 *   @retval true - card is write protected
 *   @retval false - card is not write protected
 **********************************************************************************************************************/

bool SDHI_WriteProtectGet (uint8_t channel)
{
    return HW_SDMMC_WriteProtectGet(channel);
}

/*******************************************************************************************************************//**
 *   Check for card inserted.
 *   @param[in]  channel    channel of SD Host Interface (0 or 1)
 *   @retval true - card is inserted
 *   @retval false - no card inserted
 **********************************************************************************************************************/

bool SDHI_CardInsertStatusGet (uint8_t channel)
{
    uint32_t i;
    i = INSERT_TIME;

    /* Check for card inserted. */
    while (i && (!(HW_SDMMC_CardInsertedGet(channel))))
    {
        i--;
    }

#ifndef  BSP_MCU_GROUP_S3A7
    return (0 != i);              /* true if card inserted, not timed out */
#else
    return true;
#endif
}

/*******************************************************************************************************************//**
 *   Read data from SD or (e)MMC card
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in,out] p_data - pointer to read data
 *   @retval none
 **********************************************************************************************************************/

void SDHI_DataRead (uint8_t channel, uint32_t * const p_data)
{
    *p_data = HW_SDMMC_DataGet(channel);
}

/*******************************************************************************************************************//**
 *   Write data to SD bus.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in,out] p_data - pointer to write data
 *   @retval none
 **********************************************************************************************************************/

void SDHI_DataWrite (uint8_t channel, uint32_t * const p_data)
{
    HW_SDMMC_DataWrite(channel, p_data);
}

/*******************************************************************************************************************//**
 *   End processing of data after data read or write.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @retval true - no errors
 *   @retval false - read data buffer empty
 **********************************************************************************************************************/

bool SDHI_EndProcessData (uint8_t channel)
{
    if (SDHI_ClockDisable(channel))
    {
        HW_SDMMC_BlockCountEnable(channel, false);
        return true;
    }

    return false;
}

/*******************************************************************************************************************//**
 *   Set the block count.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[out] block count
 *   @retval true - block count was set
 *   @retval false - hardware timeout
 **********************************************************************************************************************/

bool SDHI_BlockCountSet (uint8_t channel, uint32_t block_count)
{
    /* Wait for CBSY to clear */
    if (command_busy_get(channel))
    {
        return false;       /* command stayed busy over timeout */
    }

    HW_SDMMC_BlockCountEnable(channel, true);
    HW_SDMMC_BlockCountSet(channel, block_count);
    return true;
}

/*******************************************************************************************************************//**
 *   Set the block size.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in]  block_size - block size to set
 *   @retval true - block size was set
 *   @retval false - hardware timeout
 **********************************************************************************************************************/

bool SDHI_BlockSizeSet (uint8_t channel, uint16_t block_size)
{
    /* Wait for CBSY to clear */
    if (command_busy_get(channel))
    {
        return false;        /* command stayed busy over timeout */
    }

    HW_SDMMC_BlockSizeSet(channel, block_size);
    return true;
}

/*******************************************************************************************************************//**
 *   Set the Sd data bus width.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in]  bus width - bus width of data bus to set
 *   @retval none
 **********************************************************************************************************************/

void SDHI_BusWidthSet (uint8_t channel, uint8_t bus_width)
{
    HW_SDMMC_BusWidthSet(channel, bus_width);
}

/*******************************************************************************************************************//**
 *   Set the Sdio mode.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in]  enable:  true = enable, false = disable
 *   @retval none
 **********************************************************************************************************************/
bool SDHI_SDIO_Mode (uint8_t channel, bool enable)
{
    bool ret_val;
    ret_val = true;
    if (command_busy_get(channel))
    {
        ret_val = false;
    }
    HW_SDMMC_SDIO_ModeSet(channel, enable);
    return ret_val;
}

/*******************************************************************************************************************//**
 *   Get the SD card response.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in,out] p_response - pointer to response data.
 *   @retval none
 **********************************************************************************************************************/

void SDHI_ResponseGet (uint8_t channel, card_status_t * const p_response)
{
    HW_SDMMC_ResponseGet(channel, p_response);
}

/*******************************************************************************************************************//**
 *   Get the SD card response to a Multiple data block read or write command.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in,out] p_response - pointer to response data.
 *   @retval none
 *********************************************************************************************************************/
void SDHI_ResponseMultipleBlockGet (uint8_t channel, card_status_t * const p_response)
{
    HW_SDMMC_Response54Get(channel, p_response);
}

/*******************************************************************************************************************//**
 *   Get R2 type response.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in,out] p_csd - pointer to response data.
 *   @retval none
 **********************************************************************************************************************/

void  SDHI_R2ResponseGet (uint8_t channel, csd_reg_t * const p_csd)
{
    HW_SDMMC_ResponseR2Get(channel, p_csd);
}

/*******************************************************************************************************************//**
 *   Set the maximum available clock rate given the card maximum rate, PCLKA speed and dividers available.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in,out] p_rate - pointer to rate value
 *   @retval true - success
 *   @retval false - operation failed
 **********************************************************************************************************************/

bool SDHI_MaxClockFreqSet (uint8_t channel, uint32_t * p_rate)
{
    uint32_t  divider;
    uint32_t  i;
    ssp_err_t err;
    uint32_t  frequency;
    uint8_t   setting;
    uint32_t  max_rate;

    max_rate = *p_rate;
    /* Get the runtime frequency of PCLKA, the source of the SD clock */
    err      = g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKA, &frequency);
    if (err != SSP_SUCCESS)
    {
        return false;
    }

    /* The rate must be greater than or equal to lowest clock speed */
    if (max_rate < (frequency / SD_MAX_CLOCK_DIVIDER))
    {
        return false;
    }

    /* select the divider which will provide a clock rate lower than, or equal to, the maximum transfer rate. */
    divider = SD_MAX_CLOCK_DIVIDER;
    i       = (uint32_t)0x00;

    do
    {
        /* While the rate is greater than the clock with divider, */
        *p_rate   = (frequency / divider);
        setting   = g_sd_divider[i];                                         /* save the current divider setting and */
        divider >>= 1;                                                       /* test the next (smaller) divider, higher
                                                                              * frequency, */
        i++;                                                                 /* until the last divider setting in the
                                                                              * array, if necessary. */
    } while ((divider != (uint32_t)0 ) && (max_rate >= (frequency / divider)));

    if (SDHI_ClockRateSet(channel, setting))                                 /* set the clock frequency */
    {
        return true;
    }

    return false;
}

/*******************************************************************************************************************//**
 *   Enable SD bus clock.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @retval true - operation successful
 *   @retval false - operation failed
 **********************************************************************************************************************/

bool SDHI_ClockEnable (uint8_t channel)
{
    if (clock_div_enable_get(channel))
    {
        return false;               /* clock divider enable timeout */
    }

    HW_SDMMC_ClockEnable(channel);
    return true;
}

/*******************************************************************************************************************//**
 *    Disable SD bus clock.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @retval true - operation successful
 *   @retval false - operation failed
 **********************************************************************************************************************/

bool SDHI_ClockDisable (uint8_t channel)
{
    if (clock_div_enable_get(channel))
    {
        return false;               /* clock divider enable timeout */
    }

    HW_SDMMC_ClockDisable(channel);
    return true;
}

/*******************************************************************************************************************//**
 *   Set the clock rate with the given divider.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @param[in] divider
 *   @retval true - operation successful
 *   @retval false - operation failed
 **********************************************************************************************************************/

bool SDHI_ClockRateSet (uint8_t channel, uint8_t divider)
{
    if (clock_div_enable_get(channel))
    {
        return false;               /* clock divider enable timeout */
    }

    HW_SDMMC_ClockRateSet(channel, divider);
    return true;
}

/*******************************************************************************************************************//**
 *   Get the clock divider enable state.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @retval true - bit is low, can't write
 *   @retval false - bit is high, can write
 **********************************************************************************************************************/

static bool clock_div_enable_get (uint8_t channel)
{
    volatile uint32_t i;

    i = SDHI_TIMEOUT;
    while ((!(HW_SDMMC_ClockDivEnableGet(channel))) && i)
    {
        /* can't write while this is low */
        i--;
    }

    /* return error if timed out */
    if (0 == i)
    {
        return true;
    }

    return false;
}

/*******************************************************************************************************************//**
 *   Get command busy status.
 *   @param[in]  channel: channel of SD Host Interface (0 or 1)
 *   @retval true - command is busy.
 *   @retval false - command not busy.
 **********************************************************************************************************************/
static bool command_busy_get (uint8_t channel)
{
    volatile uint32_t i;
    i = SDHI_TIMEOUT;

    /* Wait for CBSY to clear */
    while ((HW_SDMMC_CommandBusyGet(channel)) && i) /* 0 = command being executed ??? */
    {
        i--;
        delay(SDHI_DELAY_BUSY_GET);
    }

    /* return error if timed out */
    if ((uint32_t)0 == i)
    {
        return true;
    }

    return false;
}

/*******************************************************************************************************************//**
 *   Delay.
 *   @param[in] <First parameter>
 *   @param[out] <Second parameter>
 *   @retval none
 **********************************************************************************************************************/
static void delay (uint32_t time)
{
    R_BSP_SoftwareDelay(time, BSP_DELAY_UNITS_MICROSECONDS);
}
