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
 * File Name    : hw_sdmmc_private.h
 * Description  : SDMMC LLD private header file.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file hw_sdmmc_private.h
 **********************************************************************************************************************/

#ifndef HW_SDMMC_PRIVATE_H
#define HW_SDMMC_PRIVATE_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_sdmmc.h"
#include "../sdhi.h"

/*******************************************************************************************************************//**
 * @addtogroup SDMMC
 * @{
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SDMMC_CHANNEL_0 		(0)
#define SDMMC_CHANNEL_1 		(1)
#if BSP_MCU_GROUP_S7G2
#define SDMMC_MAX_CHANNEL_NUMBER (SDMMC_CHANNEL_1)
#define SDMMC_MAX_NUM_CHANNELS (2)
#elif BSP_MCU_GROUP_S3A7
#define SDMMC_MAX_CHANNEL_NUMBER (SDMMC_CHANNEL_0)
#define SDMMC_MAX_NUM_CHANNELS (1)
#else
#if BSP_MCU_MOCKED
#define SDMMC_MAX_CHANNEL_NUMBER (SDMMC_CHANNEL_1)
#define SDMMC_MAX_NUM_CHANNELS (2)
#else
#define SDMMC_MAX_CHANNEL_NUMBER (SDMMC_CHANNEL_0)
#define SDMMC_MAX_NUM_CHANNELS (1)
#endif
#endif // if BSP_MCU_GROUP_S7G2




/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/


/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "common/hw_sdmmc_common.h"
#endif /* HW_SDMMC_PRIVATE_H */

void          HW_SDMMC_RegisterDefaultSet (uint8_t channel);

bool          HW_SDMMC_CommandBusyGet (uint8_t channel);

bool          HW_SDMMC_CardBusyGet (uint8_t channel);

void          HW_SDMMC_DMAModeEnable (uint8_t channel, bool enable);

uint32_t      HW_SDMMC_DataGet (uint8_t channel);

bool          HW_SDMMC_ReadDataError (uint8_t channel);

void          HW_SDMMC_DataWrite (uint8_t channel, uint32_t * data);

void          HW_SDMMC_StatusClear (uint8_t channel);

void          HW_SDMMC_ClockEnable (uint8_t channel);

bool          HW_SDMMC_ClockDivEnableGet (uint8_t channel);

void          HW_SDMMC_ClockDisable (uint8_t channel);

void          HW_SDMMC_ClockRateSet (uint8_t channel, uint8_t clock_divider);

void          HW_SDMMC_SetArguments (uint8_t channel, uint32_t argument);

void          HW_SDMMC_BlockSizeSet (uint8_t channel, uint16_t size);

void          HW_SDMMC_DataStop (uint8_t channel, bool enable);

void          HW_SDMMC_BlockCountEnable (uint8_t channel, bool enable);

void          HW_SDMMC_BlockCountSet (uint8_t channel, uint32_t count);

void          HW_SDMMC_CommandSend (uint8_t channel, uint16_t command);

bool          HW_SDMMC_ResponseEndGet (uint8_t channel);

void          HW_SDMMC_ResponseEndClear (uint8_t channel);

bool          HW_SDMMC_CardInsertedGet (uint8_t channel);

bool          HW_SDMMC_WriteProtectGet (uint8_t channel);

void          HW_SDMMC_ResponseR2Get (uint8_t channel, csd_reg_t * csd);

void          HW_SDMMC_Response54Get (uint8_t channel, card_status_t * response);

void          HW_SDMMC_ResponseGet (uint8_t channel, card_status_t * response);

void          HW_SDMMC_BusWidthSet (uint8_t channel, uint8_t bus_width);

void          HW_SDMMC_ResetAssert (uint8_t channel);

void          HW_SDMMC_ResetRelease (uint8_t channel);

void          HW_SDMMC_InterruptMaskInfo1Set (uint8_t channel, uint32_t mask);

void          HW_SDMMC_InterruptMaskSdioInfo1Set (uint8_t channel, uint32_t mask);

void          HW_SDMMC_InterruptMaskInfo2Set (uint8_t channel, uint32_t mask);

uint32_t 	  HW_SDMMC_VersionGet (void);

uint32_t 	  HW_SDMMC_VersionGet (void);

uint32_t      HW_SDMMC_ErrorGet (uint8_t channel);

uint32_t      HW_SDMMC_Info1Get (uint8_t channel);

uint32_t      HW_SDMMC_Info2Get (uint8_t channel);

uint32_t      HW_SDMMC_SDIO_Info1Get (uint8_t channel);

void          HW_SDMMC_SDIO_ModeSet (uint8_t channel, bool enable);

void          HW_SDMMC_Info1Set (uint8_t channel, uint32_t set_value);

void          HW_SDMMC_Info2Set (uint8_t channel, uint32_t set_value);

bool          HW_SDMMC_ReadBufferReadyGet(uint8_t channel);

void          HW_SDMMC_SDIO_Info1Set (uint8_t channel, uint32_t set_value);

void          HW_SDMMC_CardDetectTimeCounterSet (uint8_t channel, uint8_t cnt);

void          HW_SDMMC_TimeoutCounterSet (uint8_t channel, uint8_t cnt);

void 		  HW_SDMMC_PowerOn(uint8_t channel);

void          HW_SDMMC_PowerOff(uint8_t channel);

uint32_t *    HW_SDMMC_DataBufferAddressGet (uint8_t channel);

elc_event_t   HW_SDMMC_DmaEventGet (uint8_t channel);

IRQn_Type     HW_SDMMC_DmaIrqGet (uint8_t channel);

IRQn_Type     HW_SDMMC_SdioIrqGet (uint8_t channel);

/*******************************************************************************************************************//**
 * @} (end addtogroup SDMMC)
 **********************************************************************************************************************/
