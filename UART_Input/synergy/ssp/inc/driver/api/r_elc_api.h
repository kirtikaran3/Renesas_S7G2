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
 * File Name    : r_elc_api.h
 * Description  : ELC Interface
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup ELC_API ELC Interface
 * @brief Interface for the Event Link Controller.
 *
 * Related SSP architecture topics:
 *  - What is an SSP Interface? @ref ssp-interfaces
 *  - What is a SSP Layer? @ref ssp-predefined-layers
 *  - How to use SSP Interfaces and Modules? @ref using-ssp-modules
 *
 * Event Link Controller Interface description: @ref HALELCInterface
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_ELC_API_H
#define DRV_ELC_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Register definitions, common services and error codes. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ELC_API_VERSION_MAJOR (1)
#define ELC_API_VERSION_MINOR (0)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Possible peripherals to be linked to event signals */
typedef enum e_elc_peripheral
{
    ELC_PERIPHERAL_GPT_A        = (0),  ///< General purpose timer event A
    ELC_PERIPHERAL_GPT_B        = (1),  ///< General purpose timer event B
    ELC_PERIPHERAL_GPT_C        = (2),  ///< General purpose timer event C
    ELC_PERIPHERAL_GPT_D        = (3),  ///< General purpose timer event D
    ELC_PERIPHERAL_GPT_E        = (4),  ///< General purpose timer event E
    ELC_PERIPHERAL_GPT_F        = (5),  ///< General purpose timer event F
    ELC_PERIPHERAL_GPT_G        = (6),  ///< General purpose timer event G
    ELC_PERIPHERAL_GPT_H        = (7),  ///< General purpose timer event H
    ELC_PERIPHERAL_ADC0         = (8),  ///< ADC Unit 0 
    ELC_PERIPHERAL_ADC0_B       = (9),  ///< ADC Unit 0 Group B
    ELC_PERIPHERAL_ADC1         = (10), ///< ADC Unit 1 
    ELC_PERIPHERAL_ADC1_B       = (11), ///< ADC Unit 1 Group B
    ELC_PERIPHERAL_DAC0         = (12), ///< DAC Channel 0
    ELC_PERIPHERAL_DAC1         = (13), ///< DAC Channel 1
    ELC_PERIPHERAL_IOPORT1      = (14), ///< GPIO Port Group 1
    ELC_PERIPHERAL_IOPORT2      = (15), ///< GPIO Port Group 2
    ELC_PERIPHERAL_IOPORT3      = (16), ///< GPIO Port Group 3
    ELC_PERIPHERAL_IOPORT4      = (17), ///< GPIO Port Group 4
    ELC_PERIPHERAL_CTSU         = (18), ///< Capacitive Touch Sensing Unit
} elc_peripheral_t;

/** Sources of event signals to be linked to other peripherals or the CPU
 * @note This list may change based on device. This list is for S7G2.
 * */
typedef enum e_elc_event
{
    ELC_EVENT_ICU_IRQ0                      = (0x1),      ///< IRQ0
    ELC_EVENT_ICU_IRQ1                      = (0x2),      ///< IRQ1
    ELC_EVENT_ICU_IRQ2                      = (0x3),      ///< IRQ2
    ELC_EVENT_ICU_IRQ3                      = (0x4),      ///< IRQ3
    ELC_EVENT_ICU_IRQ4                      = (0x5),      ///< IRQ4
    ELC_EVENT_ICU_IRQ5                      = (0x6),      ///< IRQ5
    ELC_EVENT_ICU_IRQ6                      = (0x7),      ///< IRQ6
    ELC_EVENT_ICU_IRQ7                      = (0x8),      ///< IRQ7
    ELC_EVENT_ICU_IRQ8                      = (0x9),      ///< IRQ8
    ELC_EVENT_ICU_IRQ9                      = (0xA),      ///< IRQ9
    ELC_EVENT_ICU_IRQ10                     = (0xB),      ///< IRQ10
    ELC_EVENT_ICU_IRQ11                     = (0xC),      ///< IRQ11
    ELC_EVENT_ICU_IRQ12                     = (0xD),      ///< IRQ12
    ELC_EVENT_ICU_IRQ13                     = (0xE),      ///< IRQ13
    ELC_EVENT_ICU_IRQ14                     = (0xF),      ///< IRQ14
    ELC_EVENT_ICU_IRQ15                     = (0x10),     ///< IRQ15
    ELC_EVENT_DMAC0_INT                     = (0x20),     ///< DMAC grouped
    ELC_EVENT_DMAC1_INT                     = (0x21),     ///< DMAC grouped
    ELC_EVENT_DMAC2_INT                     = (0x22),     ///< DMAC grouped
    ELC_EVENT_DMAC3_INT                     = (0x23),     ///< DMAC grouped
    ELC_EVENT_DMAC4_INT                     = (0x24),     ///< DMAC grouped
    ELC_EVENT_DMAC5_INT                     = (0x25),     ///< DMAC grouped
    ELC_EVENT_DMAC6_INT                     = (0x26),     ///< DMAC grouped
    ELC_EVENT_DMAC7_INT                     = (0x27),     ///< DMAC grouped
    ELC_EVENT_DTC_COMPLETE                  = (0x29),     ///< DTC Complete
    ELC_EVENT_DTC_END                       = (0x2A),     ///< DTC End
    ELC_EVENT_ICU_SNOOZE_CANCEL             = (0x2D),     ///< Snooze cancel
    ELC_EVENT_FCU_FIFERR                    = (0x30),     ///< Flash error
    ELC_EVENT_FCU_FRDYI                     = (0x31),     ///< Flash ready
    ELC_EVENT_LVD_LVD1                      = (0x38),     ///< Voltage-monitoring 1 
    ELC_EVENT_LVD_LVD2                      = (0x39),     ///< Voltage-monitoring 2 
    ELC_EVENT_VBATT_LVD                     = (0x3A),     ///< VBATT LVD
    ELC_EVENT_CGC_MOSC_STOP                 = (0x3B),     ///< Main oscillator stop
    ELC_EVENT_LPM_SNOOZE_REQUEST            = (0x3C),     ///< Snooze entry request
    ELC_EVENT_AGT0_INT                      = (0x40),     ///< AGT interrupt
    ELC_EVENT_AGT0_COMPARE_A                = (0x41),     ///< The values of AGT and AGTCMA matched
    ELC_EVENT_AGT0_COMPARE_B                = (0x42),     ///< The values of AGT and AGTCMB matched
    ELC_EVENT_AGT1_INT                      = (0x43),     ///< AGT interrupt
    ELC_EVENT_AGT1_COMPARE_A                = (0x44),     ///< The values of AGT and AGTCMA matched
    ELC_EVENT_AGT1_COMPARE_B                = (0x45),     ///< The values of AGT and AGTCMB matched
    ELC_EVENT_IWDT_UNDERFLOW                = (0x46),     ///< IWDT Underflow
    ELC_EVENT_WDT_UNDERFLOW                 = (0x47),     ///< WDT Underflow
    ELC_EVENT_RTC_ALARM                     = (0x48),     ///< RTC alarm
    ELC_EVENT_RTC_PERIOD                    = (0x49),     ///< RTC periodic 
    ELC_EVENT_RTC_CARRY                     = (0x4A),     ///< RTC carry
    ELC_EVENT_ADC0_SCAN_END                 = (0x4B),     ///< A/D scan end interrupt
    ELC_EVENT_ADC0_SCAN_END_B               = (0x4C),     ///< A/D scan end interrupt for group B
    ELC_EVENT_ADC0_WINDOW_A                 = (0x4D),     ///< ADC Window A
    ELC_EVENT_ADC0_WINDOW_B                 = (0x4E),     ///< ADC Window B
    ELC_EVENT_ADC0_COMPARE_MATCH            = (0x4F),     ///< Compare match
    ELC_EVENT_ADC0_COMPARE_MISMATCH         = (0x50),     ///< Compare mismatch
    ELC_EVENT_ADC1_SCAN_END                 = (0x51),     ///< A/D scan end interrupt
    ELC_EVENT_ADC1_SCAN_END_B               = (0x52),     ///< A/D scan end interrupt for group B
    ELC_EVENT_ADC1_WINDOW_A                 = (0x53),     ///< ADC Window A
    ELC_EVENT_ADC1_WINDOW_B                 = (0x54),     ///< ADC Window B
    ELC_EVENT_ADC1_COMPARE_MATCH            = (0x55),     ///< Compare match
    ELC_EVENT_ADC1_COMPARE_MISMATCH         = (0x56),     ///< Compare mismatch
    ELC_EVENT_COMP_HS_0                     = (0x57),     ///< High Speed Comparator 
    ELC_EVENT_COMP_HS_1                     = (0x58),     ///< High Speed Comparator 
    ELC_EVENT_COMP_HS_2                     = (0x59),     ///< High Speed Comparator 
    ELC_EVENT_COMP_HS_3                     = (0x5A),     ///< High Speed Comparator 
    ELC_EVENT_COMP_HS_4                     = (0x5B),     ///< High Speed Comparator 
    ELC_EVENT_COMP_HS_5                     = (0x5C),     ///< High Speed Comparator 
    ELC_EVENT_COMP_LP_0                     = (0x5D),     ///< Low power comparator
    ELC_EVENT_COMP_LP_1                     = (0x5E),     ///< Low power comparator
    ELC_EVENT_USBFS_FIFO_0                  = (0x5F),     ///< USBFS transfer request
    ELC_EVENT_USBFS_FIFO_1                  = (0x60),     ///< USBFS transfer request
    ELC_EVENT_USBFS_INT                     = (0x61),     ///< USBFSI - Status collection 1
    ELC_EVENT_USBFS_RESUME                  = (0x62),     ///< USBFSR - Status collection 2
    ELC_EVENT_IIC0_RXI                      = (0x63),     ///< Receive data full
    ELC_EVENT_IIC0_TXI                      = (0x64),     ///< Transmit data empty
    ELC_EVENT_IIC0_TEI                      = (0x65),     ///< Transmit end
    ELC_EVENT_IIC0_ERI                      = (0x66),     ///< Transfer error/event generation
    ELC_EVENT_IIC0_WUI                      = (0x67),     ///< Slave address match wake up
    ELC_EVENT_IIC1_RXI                      = (0x68),     ///< Receive data full
    ELC_EVENT_IIC1_TXI                      = (0x69),     ///< Transmit data empty
    ELC_EVENT_IIC1_TEI                      = (0x6A),     ///< Transmit end
    ELC_EVENT_IIC1_ERI                      = (0x6B),     ///< Transfer error/event generation
    ELC_EVENT_IIC2_RXI                      = (0x6D),     ///< Receive data full
    ELC_EVENT_IIC2_TXI                      = (0x6E),     ///< Transmit data empty
    ELC_EVENT_IIC2_TEI                      = (0x6F),     ///< Transmit end
    ELC_EVENT_IIC2_ERI                      = (0x70),     ///< Transfer error/event generation
    ELC_EVENT_SSI0_TXI                      = (0x72),     ///< Transmit data empty
    ELC_EVENT_SSI0_RXI                      = (0x73),     ///< Receive data full
    ELC_EVENT_SSI0_INT                      = (0x75),     ///< Tx/RX, Tx/Rx overflow, idle
    ELC_EVENT_SSI1_TXI_RXI                  = (0x78),     ///< Transmit data empty and receive data full
    ELC_EVENT_SSI1_INT                      = (0x79),     ///< Tx/RX, Tx/Rx overflow, idle
    ELC_EVENT_SRC_INPUT_FIFO_EMPTY          = (0x7A),     ///< Input FIFO empty
    ELC_EVENT_SRC_OUTPUT_FIFO_FULL          = (0x7B),     ///< Output FIFO full
    ELC_EVENT_SRC_OUTPUT_FIFO_OVERFLOW      = (0x7C),     ///< Output FIFO overflow
    ELC_EVENT_SRC_OUTPUT_FIFO_UNDERFLOW     = (0x7D),     ///< Output FIFO underflow
    ELC_EVENT_SRC_CONVERSION_END            = (0x7E),     ///< Conversion end
    ELC_EVENT_PDC_RECEIVE_DATA_READY        = (0x7F),     ///< Receive data ready
    ELC_EVENT_PDC_FRAME_END                 = (0x80),     ///< Frame end
    ELC_EVENT_PDC_INT                       = (0x81),     ///< Grouped error
    ELC_EVENT_CTSU_WRITE                    = (0x82),     ///< Write end
    ELC_EVENT_CTSU_READ                     = (0x83),     ///< Read request
    ELC_EVENT_CTSU_END                      = (0x84),     ///< Measurement end
    ELC_EVENT_KEY_INT                       = (0x85),     ///< Key interrupt
    ELC_EVENT_DOC_INT                       = (0x86),     ///< Result
    ELC_EVENT_CAC_FREQUENCY_ERROR           = (0x87),     ///< Frequency error
    ELC_EVENT_CAC_MEASUREMENT_END           = (0x88),     ///< Measurement end
    ELC_EVENT_CAC_OVERFLOW                  = (0x89),     ///< Overflow
    ELC_EVENT_CAN0_ERROR                    = (0x8A),     ///< Grouped error
    ELC_EVENT_CAN0_FIFO_RX                  = (0x8B),     ///< FIFO message received
    ELC_EVENT_CAN0_FIFO_TX                  = (0x8C),     ///< FIFO message transmission completed
    ELC_EVENT_CAN0_MAILBOX_RX               = (0x8D),     ///< Mailbox message received
    ELC_EVENT_CAN0_MAILBOX_TX               = (0x8E),     ///< Mailbox transmission completed
    ELC_EVENT_CAN1_ERROR                    = (0x8F),     ///< Grouped error
    ELC_EVENT_CAN1_FIFO_RX                  = (0x90),     ///< FIFO message received
    ELC_EVENT_CAN1_FIFO_TX                  = (0x91),     ///< FIFO message transmission completed
    ELC_EVENT_CAN1_MAILBOX_RX               = (0x92),     ///< Mailbox message received
    ELC_EVENT_CAN1_MAILBOX_TX               = (0x93),     ///< Mailbox transmission completed
    ELC_EVENT_IOPORT_EVENT_1                = (0x94),     ///< IOPORT state/change detection
    ELC_EVENT_IOPORT_EVENT_2                = (0x95),     ///< IOPORT state/change detection
    ELC_EVENT_IOPORT_EVENT_3                = (0x96),     ///< IOPORT state/change detection
    ELC_EVENT_IOPORT_EVENT_4                = (0x97),     ///< IOPORT state/change detection
    ELC_EVENT_ELC_SOFTWARE_EVENT_0          = (0x98),     ///< ELC software event 0
    ELC_EVENT_ELC_SOFTWARE_EVENT_1          = (0x99),     ///< ELC software event 1
    ELC_EVENT_POEG0_EVENT                   = (0x9A),     ///< Port output enable
    ELC_EVENT_POEG1_EVENT                   = (0x9B),     ///< Port output enable
    ELC_EVENT_POEG2_EVENT                   = (0x9C),     ///< Port output enable
    ELC_EVENT_POEG3_EVENT                   = (0x9D),     ///< Port output enable
    ELC_EVENT_GPT0_CAPTURE_COMPARE_A        = (0xB0),     ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT0_CAPTURE_COMPARE_B        = (0xB1),     ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT0_COMPARE_C                = (0xB2),     ///< GTCCRC compare match
    ELC_EVENT_GPT0_COMPARE_D                = (0xB3),     ///< GTCCRD compare match
    ELC_EVENT_GPT0_COMPARE_E                = (0xB4),     ///< GTCCRE compare match
    ELC_EVENT_GPT0_COMPARE_F                = (0xB5),     ///< GTCCRF compare match
    ELC_EVENT_GPT0_COUNTER_OVERFLOW         = (0xB6),     ///< GTCNT overflow
    ELC_EVENT_GPT0_COUNTER_UNDERFLOW        = (0xB7),     ///< GTCNT underflow
    ELC_EVENT_GPT0_AD_TRIG_A                = (0xB8),     ///< GTADTRA compare match
    ELC_EVENT_GPT0_AD_TRIG_B                = (0xB9),     ///< GTADTRB compare match
    ELC_EVENT_GPT1_CAPTURE_COMPARE_A        = (0xBA),     ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT1_CAPTURE_COMPARE_B        = (0xBB),     ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT1_COMPARE_C                = (0xBC),     ///< GTCCRC compare match
    ELC_EVENT_GPT1_COMPARE_D                = (0xBD),     ///< GTCCRD compare match
    ELC_EVENT_GPT1_COMPARE_E                = (0xBE),     ///< GTCCRE compare match
    ELC_EVENT_GPT1_COMPARE_F                = (0xBF),     ///< GTCCRF compare match
    ELC_EVENT_GPT1_COUNTER_OVERFLOW         = (0xC0),     ///< GTCNT overflow
    ELC_EVENT_GPT1_COUNTER_UNDERFLOW        = (0xC1),     ///< GTCNT underflow
    ELC_EVENT_GPT1_AD_TRIG_A                = (0xC2),     ///< GTADTRA compare match
    ELC_EVENT_GPT1_AD_TRIG_B                = (0xC3),     ///< GTADTRB compare match
    ELC_EVENT_GPT2_CAPTURE_COMPARE_A        = (0xC4),     ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT2_CAPTURE_COMPARE_B        = (0xC5),     ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT2_COMPARE_C                = (0xC6),     ///< GTCCRC compare match
    ELC_EVENT_GPT2_COMPARE_D                = (0xC7),     ///< GTCCRD compare match
    ELC_EVENT_GPT2_COMPARE_E                = (0xC8),     ///< GTCCRE compare match
    ELC_EVENT_GPT2_COMPARE_F                = (0xC9),     ///< GTCCRF compare match
    ELC_EVENT_GPT2_COUNTER_OVERFLOW         = (0xCA),     ///< GTCNT overflow
    ELC_EVENT_GPT2_COUNTER_UNDERFLOW        = (0xCB),     ///< GTCNT underflow
    ELC_EVENT_GPT2_AD_TRIG_A                = (0xCC),     ///< GTADTRA compare match
    ELC_EVENT_GPT2_AD_TRIG_B                = (0xCD),     ///< GTADTRB compare match
    ELC_EVENT_GPT3_CAPTURE_COMPARE_A        = (0xCE),     ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT3_CAPTURE_COMPARE_B        = (0xCF),     ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT3_COMPARE_C                = (0xD0),     ///< GTCCRC compare match
    ELC_EVENT_GPT3_COMPARE_D                = (0xD1),     ///< GTCCRD compare match
    ELC_EVENT_GPT3_COMPARE_E                = (0xD2),     ///< GTCCRE compare match
    ELC_EVENT_GPT3_COMPARE_F                = (0xD3),     ///< GTCCRF compare match
    ELC_EVENT_GPT3_COUNTER_OVERFLOW         = (0xD4),     ///< GTCNT overflow
    ELC_EVENT_GPT3_COUNTER_UNDERFLOW        = (0xD5),     ///< GTCNT underflow
    ELC_EVENT_GPT3_AD_TRIG_A                = (0xD6),     ///< GTADTRA compare match
    ELC_EVENT_GPT3_AD_TRIG_B                = (0xD7),     ///< GTADTRB compare match
    ELC_EVENT_GPT4_CAPTURE_COMPARE_A        = (0xD8),     ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT4_CAPTURE_COMPARE_B        = (0xD9),     ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT4_COMPARE_C                = (0xDA),     ///< GTCCRC compare match
    ELC_EVENT_GPT4_COMPARE_D                = (0xDB),     ///< GTCCRD compare match
    ELC_EVENT_GPT4_COMPARE_E                = (0xDC),     ///< GTCCRE compare match
    ELC_EVENT_GPT4_COMPARE_F                = (0xDD),     ///< GTCCRF compare match
    ELC_EVENT_GPT4_COUNTER_OVERFLOW         = (0xDE),     ///< GTCNT overflow
    ELC_EVENT_GPT4_COUNTER_UNDERFLOW        = (0xDF),     ///< GTCNT underflow
    ELC_EVENT_GPT4_AD_TRIG_A                = (0xE0),     ///< GTADTRA compare match
    ELC_EVENT_GPT4_AD_TRIG_B                = (0xE1),     ///< GTADTRB compare match
    ELC_EVENT_GPT5_CAPTURE_COMPARE_A        = (0xE2),     ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT5_CAPTURE_COMPARE_B        = (0xE3),     ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT5_COMPARE_C                = (0xE4),     ///< GTCCRC compare match
    ELC_EVENT_GPT5_COMPARE_D                = (0xE5),     ///< GTCCRD compare match
    ELC_EVENT_GPT5_COMPARE_E                = (0xE6),     ///< GTCCRE compare match
    ELC_EVENT_GPT5_COMPARE_F                = (0xE7),     ///< GTCCRF compare match
    ELC_EVENT_GPT5_COUNTER_OVERFLOW         = (0xE8),     ///< GTCNT overflow
    ELC_EVENT_GPT5_COUNTER_UNDERFLOW        = (0xE9),     ///< GTCNT underflow
    ELC_EVENT_GPT5_AD_TRIG_A                = (0xEA),     ///< GTADTRA compare match
    ELC_EVENT_GPT5_AD_TRIG_B                = (0xEB),     ///< GTADTRB compare match
    ELC_EVENT_GPT6_CAPTURE_COMPARE_A        = (0xEC),     ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT6_CAPTURE_COMPARE_B        = (0xED),     ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT6_COMPARE_C                = (0xEE),     ///< GTCCRC compare match
    ELC_EVENT_GPT6_COMPARE_D                = (0xEF),     ///< GTCCRD compare match
    ELC_EVENT_GPT6_COMPARE_E                = (0xF0),     ///< GTCCRE compare match
    ELC_EVENT_GPT6_COMPARE_F                = (0xF1),     ///< GTCCRF compare match
    ELC_EVENT_GPT6_COUNTER_OVERFLOW         = (0xF2),     ///< GTCNT overflow
    ELC_EVENT_GPT6_COUNTER_UNDERFLOW        = (0xF3),     ///< GTCNT underflow
    ELC_EVENT_GPT6_AD_TRIG_A                = (0xF4),     ///< GTADTRA compare match
    ELC_EVENT_GPT6_AD_TRIG_B                = (0xF5),     ///< GTADTRB compare match
    ELC_EVENT_GPT7_CAPTURE_COMPARE_A        = (0xF6),     ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT7_CAPTURE_COMPARE_B        = (0xF7),     ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT7_COMPARE_C                = (0xF8),     ///< GTCCRC compare match
    ELC_EVENT_GPT7_COMPARE_D                = (0xF9),     ///< GTCCRD compare match
    ELC_EVENT_GPT7_COMPARE_E                = (0xFA),     ///< GTCCRE compare match
    ELC_EVENT_GPT7_COMPARE_F                = (0xFB),     ///< GTCCRF compare match
    ELC_EVENT_GPT7_COUNTER_OVERFLOW         = (0xFC),     ///< GTCNT overflow
    ELC_EVENT_GPT7_COUNTER_UNDERFLOW        = (0xFD),     ///< GTCNT underflow
    ELC_EVENT_GPT7_AD_TRIG_A                = (0xFE),     ///< GTADTRA compare match
    ELC_EVENT_GPT7_AD_TRIG_B                = (0xFF),     ///< GTADTRB compare match
    ELC_EVENT_GPT8_CAPTURE_COMPARE_A        = (0x100),    ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT8_CAPTURE_COMPARE_B        = (0x101),    ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT8_COMPARE_C                = (0x102),    ///< GTCCRC compare match
    ELC_EVENT_GPT8_COMPARE_D                = (0x103),    ///< GTCCRD compare match
    ELC_EVENT_GPT8_COMPARE_E                = (0x104),    ///< GTCCRE compare match
    ELC_EVENT_GPT8_COMPARE_F                = (0x105),    ///< GTCCRF compare match
    ELC_EVENT_GPT8_COUNTER_OVERFLOW         = (0x106),    ///< GTCNT overflow
    ELC_EVENT_GPT8_COUNTER_UNDERFLOW        = (0x107),    ///< GTCNT underflow
    ELC_EVENT_GPT8_AD_TRIG_A                = (0x108),    ///< GTADTRA compare match
    ELC_EVENT_GPT8_AD_TRIG_B                = (0x109),    ///< GTADTRB compare match
    ELC_EVENT_GPT9_CAPTURE_COMPARE_A        = (0x10A),    ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT9_CAPTURE_COMPARE_B        = (0x10B),    ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT9_COMPARE_C                = (0x10C),    ///< GTCCRC compare match
    ELC_EVENT_GPT9_COMPARE_D                = (0x10D),    ///< GTCCRD compare match
    ELC_EVENT_GPT9_COMPARE_E                = (0x10E),    ///< GTCCRE compare match
    ELC_EVENT_GPT9_COMPARE_F                = (0x10F),    ///< GTCCRF compare match
    ELC_EVENT_GPT9_COUNTER_OVERFLOW         = (0x110),    ///< GTCNT overflow
    ELC_EVENT_GPT9_COUNTER_UNDERFLOW        = (0x111),    ///< GTCNT underflow
    ELC_EVENT_GPT9_AD_TRIG_A                = (0x112),    ///< GTADTRA compare match
    ELC_EVENT_GPT9_AD_TRIG_B                = (0x113),    ///< GTADTRB compare match
    ELC_EVENT_GPT10_CAPTURE_COMPARE_A       = (0x114),    ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT10_CAPTURE_COMPARE_B       = (0x115),    ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT10_COMPARE_C               = (0x116),    ///< GTCCRC compare match
    ELC_EVENT_GPT10_COMPARE_D               = (0x117),    ///< GTCCRD compare match
    ELC_EVENT_GPT10_COMPARE_E               = (0x118),    ///< GTCCRE compare match
    ELC_EVENT_GPT10_COMPARE_F               = (0x119),    ///< GTCCRF compare match
    ELC_EVENT_GPT10_COUNTER_OVERFLOW        = (0x11A),    ///< GTCNT overflow
    ELC_EVENT_GPT10_COUNTER_UNDERFLOW       = (0x11B),    ///< GTCNT underflow
    ELC_EVENT_GPT10_AD_TRIG_A               = (0x11C),    ///< GTADTRA compare match
    ELC_EVENT_GPT10_AD_TRIG_B               = (0x11D),    ///< GTADTRB compare match
    ELC_EVENT_GPT11_CAPTURE_COMPARE_A       = (0x11E),    ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT11_CAPTURE_COMPARE_B       = (0x11F),    ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT11_COMPARE_C               = (0x120),    ///< GTCCRC compare match
    ELC_EVENT_GPT11_COMPARE_D               = (0x121),    ///< GTCCRD compare match
    ELC_EVENT_GPT11_COMPARE_E               = (0x122),    ///< GTCCRE compare match
    ELC_EVENT_GPT11_COMPARE_F               = (0x123),    ///< GTCCRF compare match
    ELC_EVENT_GPT11_COUNTER_OVERFLOW        = (0x124),    ///< GTCNT overflow
    ELC_EVENT_GPT11_COUNTER_UNDERFLOW       = (0x125),    ///< GTCNT underflow
    ELC_EVENT_GPT11_AD_TRIG_A               = (0x126),    ///< GTADTRA compare match
    ELC_EVENT_GPT11_AD_TRIG_B               = (0x127),    ///< GTADTRB compare match
    ELC_EVENT_GPT12_CAPTURE_COMPARE_A       = (0x128),    ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT12_CAPTURE_COMPARE_B       = (0x129),    ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT12_COMPARE_C               = (0x12A),    ///< GTCCRC compare match
    ELC_EVENT_GPT12_COMPARE_D               = (0x12B),    ///< GTCCRD compare match
    ELC_EVENT_GPT12_COMPARE_E               = (0x12C),    ///< GTCCRE compare match
    ELC_EVENT_GPT12_COMPARE_F               = (0x12D),    ///< GTCCRF compare match
    ELC_EVENT_GPT12_COUNTER_OVERFLOW        = (0x12E),    ///< GTCNT overflow
    ELC_EVENT_GPT12_COUNTER_UNDERFLOW       = (0x12F),    ///< GTCNT underflow
    ELC_EVENT_GPT12_AD_TRIG_A               = (0x130),    ///< GTADTRA compare match
    ELC_EVENT_GPT12_AD_TRIG_B               = (0x131),    ///< GTADTRB compare match
    ELC_EVENT_GPT13_CAPTURE_COMPARE_A       = (0x132),    ///< GTCCRA input capture/compare match
    ELC_EVENT_GPT13_CAPTURE_COMPARE_B       = (0x133),    ///< GTCCRB input capture/compare match
    ELC_EVENT_GPT13_COMPARE_C               = (0x134),    ///< GTCCRC compare match
    ELC_EVENT_GPT13_COMPARE_D               = (0x135),    ///< GTCCRD compare match
    ELC_EVENT_GPT13_COMPARE_E               = (0x136),    ///< GTCCRE compare match
    ELC_EVENT_GPT13_COMPARE_F               = (0x137),    ///< GTCCRF compare match
    ELC_EVENT_GPT13_COUNTER_OVERFLOW        = (0x138),    ///< GTCNT overflow
    ELC_EVENT_GPT13_COUNTER_UNDERFLOW       = (0x139),    ///< GTCNT underflow
    ELC_EVENT_GPT13_AD_TRIG_A               = (0x13A),    ///< GTADTRA compare match
    ELC_EVENT_GPT13_AD_TRIG_B               = (0x13B),    ///< GTADTRB compare match
    ELC_EVENT_OPS_UVW_EDGE                  = (0x150),    ///< UVW edge event
    ELC_EVENT_EPTPC_IPLS                    = (0x160),    ///< EPTPC IPLS
    ELC_EVENT_EPTPC_MINT                    = (0x161),    ///< EPTPC MINT
    ELC_EVENT_EPTPC_PINT                    = (0x162),    ///< EDMACPTP PINT
    ELC_EVENT_EDMAC0_EINT                   = (0x163),    ///< EDMAC0 EINT
    ELC_EVENT_EDMAC1_EINT                   = (0x164),    ///< EDMAC1 EINT
    ELC_EVENT_EPTPC_TIMER0_RISE             = (0x165),    ///< Pulse output timer 0 rising edge detection
    ELC_EVENT_EPTPC_TIMER1_RISE             = (0x166),    ///< Pulse output timer 1 rising edge detection
    ELC_EVENT_EPTPC_TIMER2_RISE             = (0x167),    ///< Pulse output timer 2 rising edge detection
    ELC_EVENT_EPTPC_TIMER3_RISE             = (0x168),    ///< Pulse output timer 3 rising edge detection
    ELC_EVENT_EPTPC_TIMER4_RISE             = (0x169),    ///< Pulse output timer 4 rising edge detection
    ELC_EVENT_EPTPC_TIMER5_RISE             = (0x16A),    ///< Pulse output timer 5 rising edge detection
    ELC_EVENT_EPTPC_TIMER0_FALL             = (0x16B),    ///< Pulse output timer 0 falling edge detection
    ELC_EVENT_EPTPC_TIMER1_FALL             = (0x16C),    ///< Pulse output timer 1 falling edge detection
    ELC_EVENT_EPTPC_TIMER2_FALL             = (0x16D),    ///< Pulse output timer 2 falling edge detection
    ELC_EVENT_EPTPC_TIMER3_FALL             = (0x16E),    ///< Pulse output timer 3 falling edge detection
    ELC_EVENT_EPTPC_TIMER4_FALL             = (0x16F),    ///< Pulse output timer 4 falling edge detection
    ELC_EVENT_EPTPC_TIMER5_FALL             = (0x170),    ///< Pulse output timer 5 falling edge detection
    ELC_EVENT_USBHS_FIFO_0                  = (0x171),    ///< USBHS transfer request
    ELC_EVENT_USBHS_FIFO_1                  = (0x172),    ///< USBHS transfer request
    ELC_EVENT_USBHS_USB_INT_RESUME          = (0x173),    ///< USBHS grouped
    ELC_EVENT_SCI0_RXI                      = (0x174),    ///< Receive data full interrupt
    ELC_EVENT_SCI0_TXI                      = (0x175),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI0_TEI                      = (0x176),    ///< Transmit end interrupt
    ELC_EVENT_SCI0_ERI                      = (0x177),    ///< Receive error interrupt
    ELC_EVENT_SCI0_AM                       = (0x178),    ///< Address match event
    ELC_EVENT_SCI0_RXI_OR_ERI               = (0x179),    ///< Recieve data full or error
    ELC_EVENT_SCI1_RXI                      = (0x17A),    ///< Receive data full interrupt
    ELC_EVENT_SCI1_TXI                      = (0x17B),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI1_TEI                      = (0x17C),    ///< Transmit end interrupt
    ELC_EVENT_SCI1_ERI                      = (0x17D),    ///< Receive error interrupt
    ELC_EVENT_SCI1_AM                       = (0x17E),    ///< Address match event
    ELC_EVENT_SCI2_RXI                      = (0x180),    ///< Receive data full interrupt
    ELC_EVENT_SCI2_TXI                      = (0x181),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI2_TEI                      = (0x182),    ///< Transmit end interrupt
    ELC_EVENT_SCI2_ERI                      = (0x183),    ///< Receive error interrupt
    ELC_EVENT_SCI2_AM                       = (0x184),    ///< Address match event
    ELC_EVENT_SCI3_RXI                      = (0x186),    ///< Receive data full interrupt
    ELC_EVENT_SCI3_TXI                      = (0x187),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI3_TEI                      = (0x188),    ///< Transmit end interrupt
    ELC_EVENT_SCI3_ERI                      = (0x189),    ///< Receive error interrupt
    ELC_EVENT_SCI3_AM                       = (0x18A),    ///< Address match event
    ELC_EVENT_SCI4_RXI                      = (0x18C),    ///< Receive data full interrupt
    ELC_EVENT_SCI4_TXI                      = (0x18D),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI4_TEI                      = (0x18E),    ///< Transmit end interrupt
    ELC_EVENT_SCI4_ERI                      = (0x18F),    ///< Receive error interrupt
    ELC_EVENT_SCI4_AM                       = (0x190),    ///< Address match event
    ELC_EVENT_SCI5_RXI                      = (0x192),    ///< Receive data full interrupt
    ELC_EVENT_SCI5_TXI                      = (0x193),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI5_TEI                      = (0x194),    ///< Transmit end interrupt
    ELC_EVENT_SCI5_ERI                      = (0x195),    ///< Receive error interrupt
    ELC_EVENT_SCI5_AM                       = (0x196),    ///< Address match event
    ELC_EVENT_SCI6_RXI                      = (0x198),    ///< Receive data full interrupt
    ELC_EVENT_SCI6_TXI                      = (0x199),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI6_TEI                      = (0x19A),    ///< Transmit end interrupt
    ELC_EVENT_SCI6_ERI                      = (0x19B),    ///< Receive error interrupt
    ELC_EVENT_SCI6_AM                       = (0x19C),    ///< Address match event
    ELC_EVENT_SCI7_RXI                      = (0x19E),    ///< Receive data full interrupt
    ELC_EVENT_SCI7_TXI                      = (0x19F),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI7_TEI                      = (0x1A0),    ///< Transmit end interrupt
    ELC_EVENT_SCI7_ERI                      = (0x1A1),    ///< Receive error interrupt
    ELC_EVENT_SCI7_AM                       = (0x1A2),    ///< Address match event
    ELC_EVENT_SCI8_RXI                      = (0x1A4),    ///< Receive data full interrupt
    ELC_EVENT_SCI8_TXI                      = (0x1A5),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI8_TEI                      = (0x1A6),    ///< Transmit end interrupt
    ELC_EVENT_SCI8_ERI                      = (0x1A7),    ///< Receive error interrupt
    ELC_EVENT_SCI8_AM                       = (0x1A8),    ///< Address match event
    ELC_EVENT_SCI9_RXI                      = (0x1AA),    ///< Receive data full interrupt
    ELC_EVENT_SCI9_TXI                      = (0x1AB),    ///< Transmit data empty interrupt
    ELC_EVENT_SCI9_TEI                      = (0x1AC),    ///< Transmit end interrupt
    ELC_EVENT_SCI9_ERI                      = (0x1AD),    ///< Receive error interrupt
    ELC_EVENT_SCI9_AM                       = (0x1AE),    ///< Address match event
    ELC_EVENT_SPI0_RXI                      = (0x1BC),    ///< Receive buffer full interrupt
    ELC_EVENT_SPI0_TXI                      = (0x1BD),    ///< Transmit buffer empty interrupt
    ELC_EVENT_SPI0_IDLE                     = (0x1BE),    ///< Idle interrupt
    ELC_EVENT_SPI0_ERI                      = (0x1BF),    ///< Error interrupt
    ELC_EVENT_SPI0_TEI                      = (0x1C0),    ///< Transmission completed event
    ELC_EVENT_SPI1_RXI                      = (0x1C1),    ///< Receive buffer full interrupt
    ELC_EVENT_SPI1_TXI                      = (0x1C2),    ///< Transmit buffer empty interrupt
    ELC_EVENT_SPI1_IDLE                     = (0x1C3),    ///< Idle interrupt
    ELC_EVENT_SPI1_ERI                      = (0x1C4),    ///< Error interrupt
    ELC_EVENT_SPI1_TEI                      = (0x1C5),    ///< Transmission completed event
    ELC_EVENT_QSPI_INT                      = (0x1C6),    ///< QSPI access error
    ELC_EVENT_SDHIMMC0_ACCS                 = (0x1C7),    ///< SD/MMC card access
    ELC_EVENT_SDHIMMC0_SDIO                 = (0x1C8),    ///< SD/MMC access
    ELC_EVENT_SDHIMMC0_CARD                 = (0x1C9),    ///< SD/MMC card detect
    ELC_EVENT_SDHIMMC0_DMA_REQ              = (0x1CA),    ///< SD/MMC DMA request
    ELC_EVENT_SDHIMMC1_ACCS                 = (0x1CB),    ///< SD/MMC card access
    ELC_EVENT_SDHIMMC1_SDIO                 = (0x1CC),    ///< SD/MMC access
    ELC_EVENT_SDHIMMC1_CARD                 = (0x1CD),    ///< SD/MMC card detect
    ELC_EVENT_SDHIMMC1_DMA_REQ              = (0x1CE),    ///< SD/MMC DMA request
    ELC_EVENT_DIVIDER_INT                   = (0x1CF),    ///< Divider 
    ELC_EVENT_SCE_PROC_BUSY                 = (0x1E1),    ///< SCE_PROC_BUSY
    ELC_EVENT_SCE_ROMOK                     = (0x1E2),    ///< SCE_ROMOK
    ELC_EVENT_SCE_LONG_PLG                  = (0x1E3),    ///< SCE_LONG_PLG
    ELC_EVENT_SCE_TEST_BUSY                 = (0x1E4),    ///< SCE_TEST_BUSY
    ELC_EVENT_SCE_WRRDY_0                   = (0x1E5),    ///< SCE_WRRDY_0
    ELC_EVENT_SCE_WRRDY_1                   = (0x1E6),    ///< SCE_WRRDY_1
    ELC_EVENT_SCE_WRRDY_4                   = (0x1E7),    ///< SCE_WRRDY_4
    ELC_EVENT_SCE_RDRDY_0                   = (0x1E8),    ///< SCE_RDRDY_0
    ELC_EVENT_SCE_RDRDY_1                   = (0x1E9),    ///< SCE_RDRDY_1
    ELC_EVENT_SCE_INTEGRATE_WRRDY           = (0x1EA),    ///< SCE_INTEGRATE_WRRDY
    ELC_EVENT_SCE_INTEGRATE_RDRDY           = (0x1EB),    ///< SCE_INTEGRATE_RDRDY
    ELC_EVENT_GLCDC_LINE_DETECT             = (0x1FA),    ///< Line detect
    ELC_EVENT_GLCDC_UNDERFLOW_1             = (0x1FB),    ///< Layer 1 buffer underflow
    ELC_EVENT_GLCDC_UNDERFLOW_2             = (0x1FC),    ///< Layer 2 buffer underflow
    ELC_EVENT_DRW_INT                       = (0x1FD),    ///< 2D Display list complete
    ELC_EVENT_JPEG_JEDI                     = (0x1FE),    ///< Encode/decode 
    ELC_EVENT_JPEG_JDTI                     = (0x1FF),    ///< Data transfer 
} elc_event_t;

/** Individual event link */
typedef struct st_elc_link
{
    elc_peripheral_t  peripheral;     ///< Peripheral to receive the signal
    elc_event_t       event;          ///< Signal that gets sent to the Peripheral
} elc_link_t;

/** Main configuration structure for the Event Link Controller */
typedef struct st_elc_cfg
{
    bool                autostart;   ///< Start operation and enable interrupts during open().
    uint32_t            link_count;  ///< Number of event links
    elc_link_t const  * link_list;   ///< Event links
} elc_cfg_t;

/** Software event number */
typedef enum e_elc_software_event
{
    ELC_SOFTWARE_EVENT_0,       ///< Software event 0
    ELC_SOFTWARE_EVENT_1,       ///< Software event 1
} elc_software_event_t;

/** ELC driver structure. General ELC functions implemented at the HAL layer follow this API. */
typedef struct st_elc_api
{
    /** Initialize all links in the Event Link Controller.
     * @par Implemented as
     * - R_ELC_Init()
     *
     * @param[in]   p_cfg   Pointer to configuration structure.
     **/
    ssp_err_t (* init)(elc_cfg_t const * const p_cfg);

    /** Generate a software event in the Event Link Controller.
     * @par Implemented as
     * - R_ELC_SoftwareEventGenerate()
     *
     * @param[in]   eventNum           Software event number to be generated.
     **/
    ssp_err_t (* softwareEventGenerate)(elc_software_event_t event_num);

    /** Create a single event link.
     * @par Implemented as
     * - R_ELC_LinkSet()
     *
     * @param[in]   peripheral The peripheral block that will receive the event signal.
     * @param[in]   signal     The event signal.
     **/
    ssp_err_t (* linkSet)(elc_peripheral_t peripheral, elc_event_t signal);

    /** Break an event link.
     * @par Implemented as
     * - R_ELC_LinkBreak()
     *
     * @param[in]   peripheral   The peripheral that should no longer be linked.
     **/
    ssp_err_t (* linkBreak)(elc_peripheral_t peripheral);

    /** Enable the operation of the Event Link Controller.
     * @par Implemented as
     * - R_ELC_Enable()
     **/
    ssp_err_t (* enable)(void);

    /** Disable the operation of the Event Link Controller.
     * @par Implemented as
     * - R_ELC_Disable()
     **/
    ssp_err_t (* disable)(void);

    /** Get the driver version based on compile time macros.
     * @par Implemented as
     * - R_ELC_VersionGet()
     *
     * @param[out]  p_version is value returned.
     **/
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);
} elc_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_elc_instance
{
    elc_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    elc_api_t const * p_api;     ///< Pointer to the API structure for this instance
} elc_instance_t;

#endif /* DRV_ELC_API_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup ELC_API)
 **********************************************************************************************************************/
