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
 * File Name    : hw_sdmmc.c
 * Description  : Hardware related  LLD functions for the SDMMC HAL
 **********************************************************************************************************************/

#include "hw_sdmmc_private.h"
#include "bsp_api.h"
#include "bsp_clock_cfg.h"
#include "hw_sdmmc.h"
#include "../sdcard.h"
#include "r_sdmmc_cfg.h"
#include "r_ioport.h"
#include "r_elc_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if BSP_MCU_GROUP_S7G2
#define SDHI_REG(channel)                   \
    ((R_SDHI0_Type *) ((uint32_t) R_SDHI0 + \
                       ((uint32_t) R_SDHI1 - (uint32_t) R_SDHI0) * (channel)))
#elif BSP_MCU_GROUP_S3A7
#define SDHI_REG(channel)  ( SSP_PARAMETER_NOT_USED(channel), R_SDHI0)
#endif


/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 *   This function sets normally untouched I/O register bits to the default state.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_RegisterDefaultSet (uint8_t channel)
{
    /* Command busy should be checked prior to calling this function. */
    SDHI_REG(channel)->SD_CLK_CTRL_b.SDCLKOFFEN = 0x00;                  /* Automatic clock control disabled. */
    SDHI_REG(channel)->SD_OPTION_b.TOUTMASK     = 0x00;                  /* Activate timeout. */
    SDHI_REG(channel)->SDIO_MODE                = (uint32_t) 0x00;       /* Not in SDIO mode initially. */
    SDHI_REG(channel)->SD_DMAEN                 = (uint32_t) 0x00;       /* Not in DMA mode initially. */
    SDHI_REG(channel)->SDIF_MODE                = (uint32_t) 0x00;       /* CRC check is valid. */
    SDHI_REG(channel)->EXT_SWAP                 = (uint32_t) 0x00;       /* SD_BUF normal read and write operation. */
}


/*******************************************************************************************************************//**
 *   This function sends the SD command.
 *   @param[in] channel - channel of SDHI
 *   @param[in] command - command to send
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_CommandSend (uint8_t channel, uint16_t command)
{
    SDHI_REG(channel)->SD_CMD = command; /* write command to command register */
}

/*******************************************************************************************************************//**
 *   This function returns the state of the response end.
 *   @param[in] channel - channel of SDHI
 *   @retval state of flag.
 **********************************************************************************************************************/

bool HW_SDMMC_ResponseEndGet (uint8_t channel)
{
    return SDHI_REG(channel)->SD_INFO1_b.RSPEND;
}

/*******************************************************************************************************************//**
 *   This function clears the response end flag.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_ResponseEndClear (uint8_t channel)
{
    //SDHI_REG(channel)->SD_INFO1_b.RSPEND=0;
    SDHI_REG(channel)->SD_INFO1 &= (~SDHI_INFO1_RESPONSE_END);
}

/*******************************************************************************************************************//**
 *   This function enables SDHI DMA mode.
 *   @param[in] channel - channel of SDHI
 *   @retval read data
 **********************************************************************************************************************/
void HW_SDMMC_DMAModeEnable (uint8_t channel, bool enable)
{

    if (enable)
    {
        SDHI_REG(channel)->SD_DMAEN |= (uint32_t) 0x02;
    }
    else
    {
        SDHI_REG(channel)->SD_DMAEN &= (uint32_t) ~0x02;
    }

}


/*******************************************************************************************************************//**
 *   This function returns data read from the media card.
 *   @param[in] channel - channel of SDHI
 *   @retval read data
 **********************************************************************************************************************/
uint32_t HW_SDMMC_DataGet (uint8_t channel)
{

    return (SDHI_REG(channel)->SD_BUF0_b.SD_BUF);

}

/*******************************************************************************************************************//**
 *   This function sets up data to be written to the media card.
 *   @param[in] channel - channel of SDHI
 *   @param[in] Pointer to data to be written.
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_DataWrite(uint8_t channel, uint32_t *data)
{

    SDHI_REG(channel)->SD_BUF0_b.SD_BUF = *data;

}

/*******************************************************************************************************************//**
 *   This function returns the state of the card detect pin.
 *   @param[in] channel - channel of SDHI
 *   @retval - State of the card detect pin.
 **********************************************************************************************************************/

bool HW_SDMMC_CardInsertedGet (uint8_t channel)
{

    return ((SDHI_REG(channel)->SD_INFO1_b.CDS ));

}

/*******************************************************************************************************************//**
 *   This function returns the state of the write protect pin.
 *   @param[in] channel - channel of SDHI
 *   @retval State of the write protect pin.
 **********************************************************************************************************************/

bool HW_SDMMC_WriteProtectGet (uint8_t channel)
{

    return (!(SDHI_REG(channel)->SD_INFO1_b.WPS));

}

/*******************************************************************************************************************//**
 *   This function returns the illegal read data error if it exists; if the data register was read when it was empty.
 *   @param[in] channel - channel of SDHI
 *   @retval State of the ILR flag.
 **********************************************************************************************************************/

bool HW_SDMMC_ReadDataError (uint8_t channel)
{
    return (SDHI_REG(channel)->SD_INFO2_b.ILR);
}

/*******************************************************************************************************************//**
 *   This function returns the SDHI hardware version.
 *   @param[in] none
 *   @retval Unmasked error flags.
 **********************************************************************************************************************/

uint32_t HW_SDMMC_VersionGet (void)
{
    return (SDHI_REG(0)->VERSION);
}

/*******************************************************************************************************************//**
 *   This function returns the unmasked errors.
 *   @param[in] channel - channel of SDHI
 *   @retval Unmasked error flags.
 **********************************************************************************************************************/

uint32_t HW_SDMMC_ErrorGet (uint8_t channel)
{
    return (SDHI_REG(channel)->SD_INFO2 & SD_INFO2_ERROR);
}

/*******************************************************************************************************************//**
 *   This function returns the value of the info1 register..
 *   @param[in] channel - channel of SDHI
 *   @retval Info1 register contents
 **********************************************************************************************************************/

uint32_t HW_SDMMC_Info1Get (uint8_t channel)
{
    return SDHI_REG(channel)->SD_INFO1;
}

/*******************************************************************************************************************//**
 *   This function returns the value of the info2 register.
 *   @param[in] channel - channel of SDHI
 *   @retval Info2 register contents
 **********************************************************************************************************************/


uint32_t HW_SDMMC_Info2Get (uint8_t channel)
{
    return SDHI_REG(channel)->SD_INFO2;
}

/*******************************************************************************************************************//**
 *   This function returns the value of the SDIO info1 register.
 *   @param[in] channel - channel of SDHI
 *   @retval SDIO Info1 register contents
 **********************************************************************************************************************/

uint32_t HW_SDMMC_SDIO_Info1Get (uint8_t channel)
{
    return SDHI_REG(channel)->SDIO_INFO1;
}

/*******************************************************************************************************************//**
 *   This function sets the value of the info1 register.
 *   @param[in] channel - channel of SDHI
 *   @param[in] set_value - value to set to the Info1 register
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_Info1Set (uint8_t channel, uint32_t set_value)
{
    SDHI_REG(channel)->SD_INFO1 &= (~set_value);
}

/*******************************************************************************************************************//**
 *   This function sets the value of the info2 register.
 *   @param[in] channel - channel of SDHI
 *   @param[out] set_value - value to set to the Info2 register
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_Info2Set (uint8_t channel, uint32_t set_value)
{
    SDHI_REG(channel)->SD_INFO2 &= (~set_value);
}

/*******************************************************************************************************************//**
 *   This function returns the value of the info2 register BRE bit.
 *   @param[in] channel - channel of SDHI
 *   @retval bit status
 **********************************************************************************************************************/

bool HW_SDMMC_ReadBufferReadyGet(uint8_t channel)
{
    return (SDHI_REG(channel)->SD_INFO2_b.BRE);
}


/*******************************************************************************************************************//**
 *   This function sets the value of the sdio info1 register.
 *   @param[in] channel - channel of SDHI
 *   @param[in] set_value - value to set to the sdio Info1 register
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_SDIO_Info1Set (uint8_t channel, uint32_t set_value)
{
    SDHI_REG(channel)->SDIO_INFO1 &= (~set_value);
}

/*******************************************************************************************************************//**
 *   This function sets the value of the sdio mode register.
 *   @param[in] channel - channel of SDHI
 *   @param[in] set_value - value to set to the sdio Info1 register
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_SDIO_ModeSet (uint8_t channel, bool enable)
{
    SDHI_REG(channel)->SDIO_MODE_b.IOMOD = enable;
}



/*******************************************************************************************************************//**
 *   This function returns the state of the command busy flag.
 *   @param[in] channel - channel of SDHI
 *   @retval false - command not busy
 *   @retval true - command busy
 **********************************************************************************************************************/

bool HW_SDMMC_CommandBusyGet (uint8_t channel)
{
    return SDHI_REG(channel)->SD_INFO2_b.CBSY;
}

/*******************************************************************************************************************//**
 *   This function returns the state of the card DAT0, indicating busy if held low for an extended period.
 *   @param[in] channel - channel of SDHI
 *   @retval low - card busy
 *   @retval high - card not busy
 **********************************************************************************************************************/

bool HW_SDMMC_CardBusyGet (uint8_t channel)
{
    return SDHI_REG(channel)->SD_INFO2_b.DAT0;
}

/*******************************************************************************************************************//**
 *   This function clears status registers info1 and info2.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_StatusClear (uint8_t channel)
{
    SDHI_REG(channel)->SD_INFO1 = 0;
    SDHI_REG(channel)->SD_INFO2 = 0;
}

/*******************************************************************************************************************//**
 *   This function enables the clock.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_ClockEnable (uint8_t channel)
{
    SDHI_REG(channel)->SD_CLK_CTRL_b.SCLKEN = 1;
}

/*******************************************************************************************************************//**
 *   This function disables the clock.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_ClockDisable (uint8_t channel)
{
    SDHI_REG(channel)->SD_CLK_CTRL_b.SCLKEN = 0;

}

/*******************************************************************************************************************//**
 *   This function returns the state of the clockdivenable register bit.
 *   @param[in] channel - channel of SDHI
 *   @retval true - bit is high
 *   @retval false - bit is low
 **********************************************************************************************************************/

bool HW_SDMMC_ClockDivEnableGet(uint8_t channel)
{

return (SDHI_REG(channel)->SD_INFO2_b.SCLKDIVEN);

}

/*******************************************************************************************************************//**
 *   This function sets the argument registers for the command.
 *   @param[in] channel - channel of SDHI
 *   @param[in] argument - SD command argument
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_SetArguments (uint8_t channel, uint32_t argument)
{
    SDHI_REG(channel)->SD_ARG = (argument & 0x0ffff);
    SDHI_REG(channel)->SD_ARG1 = ((argument >> 16) & 0x0ffff);
}

/*******************************************************************************************************************//**
 *   This function sets the block size.
 *   @param[in] channel - channel of SDHI
 *   @param[in] size - size of blocks
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_BlockSizeSet (uint8_t channel, uint16_t size)
{
    SDHI_REG(channel)->SD_SIZE_b.LEN = (uint16_t)(size & 0x3ff);
}

/*******************************************************************************************************************//**
 *   This function enables and disables block counting for multiple block transfers.
 *   @param[in] channel - channel of SDHI
 *   @param[in] enable  - enable/disable data stop
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_DataStop (uint8_t channel, bool enable)
{
    SDHI_REG(channel)->SD_STOP_b.STP = (enable==true)?1:0;
}

/*******************************************************************************************************************//**
 *   This function enables and disables stop bit for block counting of multiple block transfers.
 *   @param[in] channel - channel of SDHI
 *   @param[in] enable - enable/disable block count
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_BlockCountEnable (uint8_t channel, bool enable)
{
    SDHI_REG(channel)->SD_STOP_b.SEC = enable;
}

/*******************************************************************************************************************//**
 *   This function sets the block count for multiple block transfers.
 *   @param[in] channel - channel of SDHI
 *   @param[in] count - number of blocks to transfer
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_BlockCountSet (uint8_t channel, uint32_t count)
{
    SDHI_REG(channel)->SD_SECCNT = count;
}

/*******************************************************************************************************************//**
 *   This function gets the R2 response.
 *   @param[in] channel - channel of SDHI
 *   @param[in,out] - pointer to csd register data
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_ResponseR2Get (uint8_t channel, csd_reg_t * csd)
{

    /* SDResponseR2 are bits from 8-127, first 8 MSBs are reserved */
    csd->reg.sdrsp10 =SDHI_REG(channel)->SD_RSP10;
    csd->reg.sdrsp32 =SDHI_REG(channel)->SD_RSP32;
    csd->reg.sdrsp54 =SDHI_REG(channel)->SD_RSP54;
    csd->reg.sdrsp76 =SDHI_REG(channel)->SD_RSP76;

}

/*******************************************************************************************************************//**
 *   This function gets the response.
 *   @param[in] channel - channel of SDHI
 *   @retval card status - status of card
 **********************************************************************************************************************/

void HW_SDMMC_ResponseGet (uint8_t channel, card_status_t * response)
{
    response->status = SDHI_REG(channel)->SD_RSP10;
}

/*******************************************************************************************************************//**
 *   This function gets the response to a multiple block command..
 *   @param[in] channel - channel of SDHI
 *   @retval card status - status of card
 **********************************************************************************************************************/

void HW_SDMMC_Response54Get (uint8_t channel, card_status_t * response)
{
    response->status = SDHI_REG(channel)->SD_RSP54;
}

/*******************************************************************************************************************//**
 *   This function sets the bus width.
 *   @param[in] channel - channel of SDHI
 *   @param[in] bus_width - width of the SD data bus
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_BusWidthSet (uint8_t channel, uint8_t bus_width)
{
    switch (bus_width)
    {
    case 1:
        SDHI_REG(channel)->SD_OPTION_b.WIDTH = 0x1;
        SDHI_REG(channel)->SD_OPTION_b.WIDTH8 = 0x0;
        break;
    case 4:
        SDHI_REG(channel)->SD_OPTION_b.WIDTH = 0x0;
        SDHI_REG(channel)->SD_OPTION_b.WIDTH8 = 0x0;
        break;
    case 8:
        SDHI_REG(channel)->SD_OPTION_b.WIDTH = 0x0;
        SDHI_REG(channel)->SD_OPTION_b.WIDTH8 = 0x1;
        break;
    default:
        SDHI_REG(channel)->SD_OPTION_b.WIDTH = 0x0;
        SDHI_REG(channel)->SD_OPTION_b.WIDTH8 = 0x0;
        break;
    }
}

/*******************************************************************************************************************//**
 *   This function sets the clock rate.
 *   @param[in] channel - channel of SDHI
 *   @param[in] clock divider
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_ClockRateSet (uint8_t channel, uint8_t clock_divider)
{
    SDHI_REG(channel)->SD_CLK_CTRL_b.SCLKEN = 0;
    SDHI_REG(channel)->SD_CLK_CTRL_b.DIV=clock_divider;
    SDHI_REG(channel)->SD_CLK_CTRL_b.SCLKEN = 1;
}

/*******************************************************************************************************************//**
 *   This function asserts reset of SDHI.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_ResetAssert (uint8_t channel)
{
    /* Reset. */
    SDHI_REG(channel)->SOFT_RST_b.SDRST = 0x0;
}

/*******************************************************************************************************************//**
 *   This function releases reset of SDHI.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_ResetRelease (uint8_t channel)
{
    /* Release reset.  */
    SDHI_REG(channel)->SOFT_RST_b.SDRST = 0x1;
}

/*******************************************************************************************************************//**
 *   This function sets Info1 mask.
 *   @param[in] channel - channel of SDHI
 *   @param[in] mask to set
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_InterruptMaskInfo1Set(uint8_t channel, uint32_t mask)
{
    SDHI_REG(channel)->SD_INFO1_MASK= mask;

}

/*******************************************************************************************************************//**
 *   This function sets Info2 mask.
 *   @param[in] channel - channel of SDHI
 *   @param[in] mask to set
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_InterruptMaskInfo2Set(uint8_t channel, uint32_t mask)
{
    SDHI_REG(channel)->SD_INFO2_MASK= mask;
}

/*******************************************************************************************************************//**
 *   This function sets SDIO Info1 mask.
 *   @param[in] channel - channel of SDHI
 *   @param[in] mask to set
 *   @retval none
 **********************************************************************************************************************/
void HW_SDMMC_InterruptMaskSdioInfo1Set (uint8_t channel, uint32_t mask)
{
    SDHI_REG(channel)->SDIO_INFO1_MASK= mask;
}

/*******************************************************************************************************************//**
 *   This function sets the card detect timer..
 *   @param[in] channel - channel of SDHI
 *   @param[in] count
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_CardDetectTimeCounterSet(uint8_t channel, uint8_t count)
{
    SDHI_REG(channel)->SD_OPTION_b.CTOP = (uint8_t)(count & 0x0f);
}

/*******************************************************************************************************************//**
 *   This function sets the timeout counter.
 *   @param[in] channel - channel of SDHI
 *   @param[in] count
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_TimeoutCounterSet(uint8_t channel, uint8_t count)
{
    SDHI_REG(channel)->SD_OPTION_b.TOP  = (uint8_t)(count & 0x0f);
}

/*******************************************************************************************************************//**
 *   This function powers on the channel.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_PowerOn(uint8_t channel)
{

    if (SDMMC_CHANNEL_0 == channel)
	{
		R_MSTP->MSTPCRC_b.MSTPC12 = 0;                                      /* Power on channel 0. */
	}
	else
	{
		R_MSTP->MSTPCRC_b.MSTPC11 = 0;                                      /* Power on channel 1. */
	}
}

/*******************************************************************************************************************//**
 *   This function powers off the channel.
 *   @param[in] channel - channel of SDHI
 *   @retval none
 **********************************************************************************************************************/

void HW_SDMMC_PowerOff(uint8_t channel)
{

    if (SDMMC_CHANNEL_0 == channel)
    {
        R_MSTP->MSTPCRC_b.MSTPC12 = 1;                                      /* Power off channel 0. */
    }
    else
    {
        R_MSTP->MSTPCRC_b.MSTPC11 = 1;                                      /* Power off channel 1. */
    }
}

/*******************************************************************************************************************//**
 *   This function returns data buffer address from the media card.
 *   @param[in] channel - channel of SDHI
 *   @retval Data buffer register address.
 **********************************************************************************************************************/
uint32_t * HW_SDMMC_DataBufferAddressGet (uint8_t channel)
{

    return (uint32_t *) (&SDHI_REG(channel)->SD_BUF0);

}

/*******************************************************************************************************************//**
 *   This function returns the ELC event for DMA by channel.
 *   @param[in] channel - channel of SDHI
 *   @retval elc event
 **********************************************************************************************************************/
elc_event_t HW_SDMMC_DmaEventGet (uint8_t channel)
{
    elc_event_t event = ELC_EVENT_SDHIMMC0_DMA_REQ;

#if SDMMC_MAX_CHANNEL_NUMBER > SDMMC_CHANNEL_0
    if (SDMMC_CHANNEL_1 == channel)
    {
        event = ELC_EVENT_SDHIMMC1_DMA_REQ;
    }
#else
    SSP_PARAMETER_NOT_USED(channel);
#endif

    return event;
}

/*******************************************************************************************************************//**
 *   This function returns the irq number for DMA by channel.
 *   @param[in] channel - channel of SDHI
 *   @retval elc event
 **********************************************************************************************************************/
IRQn_Type HW_SDMMC_DmaIrqGet (uint8_t channel)
{
    IRQn_Type irq;
    irq = (IRQn_Type) 0;
#if defined (BSP_MCU_GROUP_S7G2) || (defined BSP_MCU_GROUP_S3A7)

 #if (BSP_IRQ_CFG_SDHIMMC0_DMA_REQ != BSP_IRQ_DISABLED)
#ifndef DMA_CHANNEL_USED
#define DMA_CHANNEL_USED (1)
#endif
    if (SDMMC_CHANNEL_0 == channel)
    {
        irq = SDHIMMC0_DMA_REQ_IRQn;
    }
#endif
#endif

#ifdef    BSP_MCU_GROUP_S7G2
#if (BSP_IRQ_CFG_SDHIMMC1_DMA_REQ != BSP_IRQ_DISABLED)
#ifndef DMA_CHANNEL_USED
#define DMA_CHANNEL_USED (1)
#endif
    if (SDMMC_CHANNEL_1 == channel)
    {
        irq = SDHIMMC1_DMA_REQ_IRQn;
    }
#endif
#endif
#ifndef DMA_CHANNEL_USED
    SSP_PARAMETER_NOT_USED(channel);
#endif
    return irq;
}

/*******************************************************************************************************************//**
 *   This function returns the irq number for SDIO by channel.
 *   @param[in] channel - channel of SDHI
 *   @retval elc event
 **********************************************************************************************************************/
IRQn_Type HW_SDMMC_SdioIrqGet (uint8_t channel)
{
    IRQn_Type irq;
    irq = (IRQn_Type)0;
#if defined (BSP_MCU_GROUP_S7G2) || (defined BSP_MCU_GROUP_S3A7)

 #if (BSP_IRQ_CFG_SDHIMMC0_SDIO != BSP_IRQ_DISABLED)
#ifndef IRQ_CHANNEL_USED
#define IRQ_CHANNEL_USED (1)
#endif

    if (SDMMC_CHANNEL_0 == channel)
    {
        irq = SDHIMMC0_SDIO_IRQn;
    }
#endif
#endif

#ifdef    BSP_MCU_GROUP_S7G2
#if (BSP_IRQ_CFG_SDHIMMC1_SDIO != BSP_IRQ_DISABLED)
#ifndef IRQ_CHANNEL_USED
#define IRQ_CHANNEL_USED (1)
#endif
    if (SDMMC_CHANNEL_1 == channel)
    {
        irq = SDHIMMC1_SDIO_IRQn;
    }
#endif
#endif
#ifndef IRQ_CHANNEL_USED
    SSP_PARAMETER_NOT_USED(channel);
#endif

    return irq;
}

