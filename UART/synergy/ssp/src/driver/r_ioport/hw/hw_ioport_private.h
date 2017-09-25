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
 * File Name    : hw_ioport_private.h
 * Description  : IOPORT private macros.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup IOPORT
 * @{
 **********************************************************************************************************************/

#ifndef HW_IOPORT_PRIVATE_H
#define HW_IOPORT_PRIVATE_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define IOPORT_PRV_PCNTR_OFFSET        0x00000020

#define IOPORT_PRV_PORT_OUTPUT_HIGH    (1)
#define IOPORT_PRV_PORT_INPUT          (1 << 1)
#define IOPORT_PRV_PORT_DIR_OUTPUT     (1 << 2)
#define IOPORT_PRV_PULL_UP_ENABLE      (1 << 4)
#define IOPORT_PRV_EVENT_RISING_EDGE   (1 << 12)
#define IOPORT_PRV_EVENT_FALLING_EDGE  (1 << 13)
#define IOPORT_PRV_EVENT_BOTH_EDGES    (3 << 12)
#define IOPORT_PRV_ANALOG_INPUT        (1 << 15)
#define IOPORT_PRV_PERIPHERAL_FUNCTION (1 << 16)
#define IOPORT_PRV_PORT_FUNC_SEL(X) (X << 24)
#define IOPORT_PRV_CLEAR_BITS_MASK     (0x1F01FCD5)     ///< Zero bits in mask must be written as zero to PFS register

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** PFS writing enable/disable. */
typedef enum e_ioport_pwpr
{
    IOPORT_PFS_WRITE_DISABLE = 0,       ///< Disable PFS write access
    IOPORT_PFS_WRITE_ENABLE  = 1        ///< Enable PFS write access
} ioport_pwpr_t;

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
void           HW_IOPORT_PinWrite (ioport_port_pin_t pin, ioport_level_t level);
void           HW_IOPORT_PortWrite (ioport_port_t port, ioport_size_t value);
void           HW_IOPORT_PortWriteWithPCNTR3 (ioport_port_t port, ioport_size_t set_value, ioport_size_t reset_value);
ioport_level_t HW_IOPORT_PinRead (ioport_port_pin_t pin);
ioport_size_t  HW_IOPORT_PortRead (ioport_port_t port);
ioport_size_t  HW_IOPORT_PortDirectionRead (ioport_port_t port);
void           HW_IOPORT_PortDirectionSet (ioport_port_t port, ioport_size_t value);
void           HW_IOPORT_PinDirectionSet (ioport_port_pin_t pin, ioport_direction_t direction);
ioport_size_t  HW_IOPORT_PortOutputDataRead (ioport_port_t port);
ioport_size_t  HW_IOPORT_PortOutputDataRead (ioport_port_t port);
ioport_size_t  HW_IOPORT_PortEventInputDataRead (ioport_port_t port);
void           HW_IOPORT_PortEventOutputDataWrite (ioport_port_t port,
                                                   ioport_size_t set_value,
                                                   ioport_size_t reset_value);
void     HW_IOPORT_EthernetModeCfg (ioport_ethernet_channel_t channel, ioport_ethernet_mode_t mode);
void     HW_IOPORT_PFSWrite (ioport_port_pin_t pin, uint32_t value);
void     HW_IOPORT_PFSSetDirection (ioport_port_pin_t pin, ioport_direction_t direction);
uint32_t HW_IOPORT_PFSRead (ioport_port_pin_t pin);
void     HW_IOPORT_PFSAccess (ioport_pwpr_t value);
void     HW_IOPORT_Init_Reference_Counter(void);

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "common/hw_ioport_common.h"
#endif /* HW_IOPORT_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup IOPORT)
 **********************************************************************************************************************/
