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
* File Name    : hw_sci_common.h
* @brief    SCI LLD definitions, common portion
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "bsp_api.h"
/* Public interface header file for this module. */
#include "r_sci_common.h"
#ifdef BSP_MCU_GROUP_S7G2
#include "..\..\..\src\driver\r_sci_common\hw\target\s7g2\hw_sci_common_s7g2.h"
#endif

#ifdef BSP_MCU_GROUP_S3A7
#include "..\..\..\src\driver\r_sci_common\hw\target\s3a7\hw_sci_common_s3a7.h"
#endif
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** SCI SCR register bit masks */
#define SCI_SCR_TE_MASK         (0x20U)     ///< transmitter enable
#define SCI_SCR_RE_MASK         (0x10U)     ///< receiver enable
#define SCI_SCR_TE_RE_MASK      (0x30U)     ///< transmitter & receiver enable
#define SCI_SCR_RIE_VALUE_MASK  (0x01U)     ///< RIE: 1 bit
#define SCI_SCR_TIE_VALUE_MASK  (0x01U)     ///< TIE: 1 bit
#define SCI_SCR_TEIE_VALUE_MASK (0x01U)     ///< TEIE: 1 bit
#define SCI_SCR_CKE_VALUE_MASK  (0x03U)     ///< CKE: 2 bits

/** SCI SEMR register bit masks */
#define SCI_SEMR_BGDM_VALUE_MASK    (0x01U)     ///< BGDM: 1 bit
#define SCI_SEMR_ABCS_VALUE_MASK    (0x01U)     ///< ABCS: 1 bit
#define SCI_SEMR_ABCSE_VALUE_MASK   (0x01U)     ///< ABCSE: 1 bit

/** SCI SMR register bit masks */
#define SCI_SMR_CKS_VALUE_MASK      (0x03U)     ///< CKS: 2 bits

/** SCI SSR register receiver error bit masks */
#define SCI_SSR_ORER_MASK   (0x20U)     ///< overflow error
#define SCI_SSR_FER_MASK    (0x10U)     ///< framing error
#define SCI_SSR_PER_MASK    (0x08U)     ///< parity err
#define SCI_RCVR_ERR_MASK   (SCI_SSR_ORER_MASK | SCI_SSR_FER_MASK | SCI_SSR_PER_MASK)

/** SCI register access or macro  */
#define SCI_REG(channel)    ((R_SCI0_Type *)((uint32_t)R_SCI0 + ((uint32_t)R_SCI1 - (uint32_t)R_SCI0) * (channel)))

/** MSTP register access or macro  */
#define MODULE_STOP         ((R_MSTP_Type *)R_MSTP)              ///< MSTPCRB
#define MSTP_BIT(channel)   ((uint32_t)(1 << (31 - (channel))))  ///< MSTPCRB bit field access

/** To stop compiler warning */
#define SCI_NOT_USED(x)     ((void)(x))

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Private Functions
***********************************************************************************************************************/
static IRQn_Type r_sci_select_irqn (uint32_t const channel, sci_int_t sci_int);
__STATIC_INLINE elc_event_t r_sci_rxi_event_lookup (uint32_t const channel);
__STATIC_INLINE elc_event_t r_sci_txi_event_lookup (uint32_t const channel);

/*******************************************************************************************************************//**
* Enable/Disable interrupt for SCI reception in interrupt controller
* @param[in] channel  SCI channel
* @param[in] sci_int  SCI interrupt type (SCI_TX_INT|SCI_TE_INT|SCI_RE_INT|SCI_ALL_INT)
* @param[in] enable   True(Enable)|False(Disable)
* @retval    void
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_InterruptEnable (uint32_t const channel, sci_int_t sci_int, bool const enable)
{
    void (*func_irq)(IRQn_Type IRQn);
    uint8_t  value;
    volatile uint8_t dummy;

    /* Function select */
    if(enable)
    {
        func_irq = NVIC_EnableIRQ;
        value    = 1;
    }
    else
    {
        func_irq = NVIC_DisableIRQ;
        value    = 0;
    }

    /* Register set */
    switch (sci_int)
    {
    case SCI_ALL_INT:
        func_irq (r_sci_select_irqn (channel, SCI_TX_INT));
        func_irq (r_sci_select_irqn (channel, SCI_TE_INT));
        func_irq (r_sci_select_irqn (channel, SCI_RX_INT));
        func_irq (r_sci_select_irqn (channel, SCI_ERR_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
    	SCI_REG(channel)->SCR_b.TIE = (uint8_t)(SCI_SCR_TIE_VALUE_MASK & value);
        SCI_REG(channel)->SCR_b.TEIE = (uint8_t)(SCI_SCR_TEIE_VALUE_MASK & value);
        SCI_REG(channel)->SCR_b.RIE = (uint8_t)(SCI_SCR_RIE_VALUE_MASK & value);
        SCI_NOT_USED(dummy);
    break;
    case SCI_TE_INT:
        func_irq (r_sci_select_irqn (channel, SCI_TE_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_REG(channel)->SCR_b.TEIE = (uint8_t)(SCI_SCR_TEIE_VALUE_MASK & value);
        SCI_NOT_USED(dummy);
    break;
    case SCI_TX_INT:
        func_irq (r_sci_select_irqn (channel, SCI_TX_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_REG(channel)->SCR_b.TIE = (uint8_t)(SCI_SCR_TIE_VALUE_MASK & value);
        SCI_NOT_USED(dummy);
    break;
    case SCI_RX_INT:
        func_irq (r_sci_select_irqn (channel, SCI_RX_INT));
    	func_irq (r_sci_select_irqn (channel, SCI_ERR_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
    	SCI_REG(channel)->SCR_b.RIE = (uint8_t)(SCI_SCR_RIE_VALUE_MASK & value);
        SCI_NOT_USED(dummy);
    break;
    default:
    break;
    }
}  /* End of function HW_SCI_InterruptEnable() */

/*******************************************************************************************************************//**
* Clears the Pending IRQ for SCI in the NVIC
* @param[in] channel  SCI channel
* @param[in] sci_int  SCI interrupt type (SCI_TX_INT|SCI_TE_INT|SCI_RE_INT|SCI_ALL_INT)
* @retval    void
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_PendingIRQClear(uint32_t const channel, sci_int_t sci_int)
{
    void (*func_irq)(IRQn_Type IRQn);
    volatile uint8_t dummy;

    /* Function select */
    func_irq = NVIC_ClearPendingIRQ;

    /* Register set */
    switch (sci_int)
    {
    case SCI_ALL_INT:
        func_irq (r_sci_select_irqn (channel, SCI_TX_INT));
        func_irq (r_sci_select_irqn (channel, SCI_TE_INT));
        func_irq (r_sci_select_irqn (channel, SCI_RX_INT));
        func_irq (r_sci_select_irqn (channel, SCI_ERR_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_NOT_USED(dummy);
    break;
    case SCI_TX_INT:
        func_irq (r_sci_select_irqn (channel, SCI_TX_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_NOT_USED(dummy);
    break;
    case SCI_TE_INT:
        func_irq (r_sci_select_irqn (channel, SCI_TE_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_NOT_USED(dummy);
    break;
    case SCI_RX_INT:
        func_irq (r_sci_select_irqn (channel, SCI_RX_INT));
        func_irq (r_sci_select_irqn (channel, SCI_ERR_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_NOT_USED(dummy);
    break;
    default:
    break;
    }
}  /* End of function HW_SCI_PendingIRQClear() */

/*******************************************************************************************************************//**
* Clears the IRQ Status for SCI channel
* @param[in] channel  SCI channel
* @param[in] sci_int  SCI interrupt type (SCI_TX_INT|SCI_TE_INT|SCI_RE_INT|SCI_ALL_INT)
* @retval    void
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_IrqStatusClear(uint32_t const channel, sci_int_t sci_int)
{
    void (*func_irq)(IRQn_Type IRQn);
    volatile uint8_t dummy;

    /* Function select */
    func_irq = R_BSP_IrqStatusClear;

    /* Register set */
    switch (sci_int)
    {
    case SCI_ALL_INT:
        func_irq (r_sci_select_irqn (channel, SCI_TX_INT));
        func_irq (r_sci_select_irqn (channel, SCI_TE_INT));
        func_irq (r_sci_select_irqn (channel, SCI_RX_INT));
        func_irq (r_sci_select_irqn (channel, SCI_ERR_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_NOT_USED(dummy);
    break;
    case SCI_TX_INT:
        func_irq (r_sci_select_irqn (channel, SCI_TX_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_NOT_USED(dummy);
    break;
    case SCI_TE_INT:
        func_irq (r_sci_select_irqn (channel, SCI_TE_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_NOT_USED(dummy);
    break;
    case SCI_RX_INT:
        func_irq (r_sci_select_irqn (channel, SCI_RX_INT));
        func_irq (r_sci_select_irqn (channel, SCI_ERR_INT));
        /* Give time for value to be updated */
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        dummy = SCI_REG(channel)->SCR;
        SCI_NOT_USED(dummy);
    break;
    default:
    break;
    }
}  /* End of function HW_SCI_IrqStatusClear() */

/*******************************************************************************************************************//**
* Clears the SCI Serial Status Register, IRQ Status and the Pending IRQ for SCI channel
* @param[in] channel  SCI channel
* @param[in] sci_int  SCI interrupt type (SCI_TX_INT|SCI_TE_INT|SCI_RE_INT|SCI_ALL_INT)
* @retval    void
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_IRQClear(uint32_t const channel, sci_int_t sci_int)
{
	/* Clear RDRF and TDRE interrupt bits in the SCI Serial Status Register */
    SCI_REG(channel)->SSR_b.RDRF = 0;
    SCI_REG(channel)->SSR_b.TDRE = 0;

    /* Clear IRQ Status */
	HW_SCI_IrqStatusClear(channel, sci_int);

	/* Clear Pending IRQ */
	HW_SCI_PendingIRQClear(channel, sci_int);
}  /* End of function HW_SCI_IRQClear() */

/*******************************************************************************************************************//**
 * Clears the transmit end interrupt.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ClearTxEnd(uint32_t const channel)
{
    /* Clear Serial Status Register Transmit End flag (TEND bit) */
    //SCI_REG(channel)->SSR_b.TEND = 0;

    /* Clear ARM NVIC pending flag */
    HW_SCI_IrqStatusClear(channel, SCI_TE_INT);
}  /* End of function HW_SCI_SIIC_ClearTxEnd() */

/*******************************************************************************************************************//**
 * Clears all interrupts.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ClearInts(uint32_t const channel)
{
    /* Clear all ICU IR flags and the all the SCI register interrupt bits */
    HW_SCI_InterruptEnable(channel, SCI_ALL_INT, false);
    /* Clear ARM All NVIC pending flags */
    HW_SCI_PendingIRQClear(channel, SCI_ALL_INT);
}  /* End of function HW_SCI_SIIC_ClearInts() */

/*******************************************************************************************************************//**
* @brief    Enables transmission for the specified SCI channel
* @param[in] channel  SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_TransmitterEnable (uint32_t const channel)
{
    SCI_REG(channel)->SCR |= SCI_SCR_TE_MASK;
}  /* End of function HW_SCI_TransmitterEnable() */

/*******************************************************************************************************************//**
* Disables transmission for the specified SCI channel
* @param[in] channel  SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_TransmitterDisable (uint32_t const channel)
{
    SCI_REG(channel)->SCR &= (uint8_t)(~SCI_SCR_TE_MASK);
}  /* End of function HW_SCI_TransmitterDisable() */

/*******************************************************************************************************************//**
* Enables reception for the specified SCI channel
* @param[in] channel  SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ReceiverEnable (uint32_t const channel)
{
    SCI_REG(channel)->SCR |= SCI_SCR_RE_MASK;
}  /* End of function HW_SCI_ReceiverEnable() */

/*******************************************************************************************************************//**
* Disables reception for the specified SCI channel
* @param[in] hdl handle to SCI control block of each channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ReceiverDisable (uint32_t const channel)
{
    SCI_REG(channel)->SCR &= (uint8_t)(~SCI_SCR_RE_MASK);
}  /* End of function HW_SCI_ReceiverDisable() */

/*******************************************************************************************************************//**
* Enables reception and transmission for the specified SCI channel
* @param[in] channel  SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_TransmitterReceiverEnable (uint32_t const channel)
{
    SCI_REG(channel)->SCR |= SCI_SCR_TE_RE_MASK;
}  /* End of function HW_SCI_ReceiverEnable() */

/*******************************************************************************************************************//**
* Disables reception  and transmission for the specified SCI channel
* @param[in] channel  SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_TransmitterReceiverDisable (uint32_t const channel)
{
    SCI_REG(channel)->SCR &= (uint8_t)(~SCI_SCR_TE_RE_MASK);
}  /* End of function HW_SCI_TransmitterReceiverDisable() */

/*******************************************************************************************************************//**
* Disable FIFO mode
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_FifoDisable (uint32_t const channel)
{
    SCI_REG(channel)->FCR_b.FM = 0;    /* FCRL.FM (select non-FIFO mode) */
}  /* End of function HW_SCI_FifoDisable() */

/*******************************************************************************************************************//**
* This function writes data to transmit data register.
* @param[in] channel  SCI channel
* @param[in] data     Data to be sent
* @retval    void
* @note      All the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_Write (uint32_t const channel, uint8_t const data)
{
    while (0 == SCI_REG(channel)->SSR_b.TDRE)
    {
        /* Wait until TDRE is cleared */
    }

    /* Write 1byte data to data register */
    SCI_REG(channel)->TDR = data;
}  /* End of function HW_SCI_Write() */

/*******************************************************************************************************************//**
* This function reads data from receive data register
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_SCI_Read (uint32_t const channel)
{
    return SCI_REG(channel)->RDR;
}  /* End of function HW_SCI_Read() */

/*******************************************************************************************************************//**
* This function provides power to the channel.
* @param[in] channel    SCI Channel number
* @retval    none
* @note      All the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_PowerOn (uint32_t const channel)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
    MODULE_STOP->MSTPCRB &= (uint32_t)(~MSTP_BIT(channel));
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
}  /* End of function HW_SCI_PowerControl() */

/*******************************************************************************************************************//**
* This function removes power from the channel.
* @param[in] channel    SCI Channel number
* @retval    none
* @note      All the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_PowerOff (uint32_t const channel)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
    MODULE_STOP->MSTPCRB |= MSTP_BIT(channel);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
}  /* End of function HW_SCI_PowerControl() */


/*******************************************************************************************************************//**
* This function gives delay time for the clock signal output through the SCKn pin
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
*            Do not call this function from other than simple SPI mode and clock synchronous mode.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ClockPhaseDelayEnable (uint32_t const channel)
{
        /* Set the SPMR.CKPH bit to 1
         * (Clock Phase Select 1: Clock is delayed)
         */
        SCI_REG(channel)->SPMR_b.CKPH  = 1;
}  /* End of function HW_SCI_ClockPhaseDelayEnable() */

/*******************************************************************************************************************//**
* This function gives no delay for the clock signal output through the SCKn pin
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ClockPhaseDelayDisable (uint32_t const channel)
{
        /* Set the SPMR.CKPH bit to 1
         * (Clock Phase Select 1: Clock is delayed)
         */
        SCI_REG(channel)->SPMR_b.CKPH  = 0;
}  /* End of function HW_SCI_ClockPhaseDelayDisable() */

/*******************************************************************************************************************//**
* This function selects inverted polarity of the clock signal output through the SCKn pin.
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
*            Do not call this function from other than simple SPI mode and clock synchronous mode.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ClockPorarityInvert (uint32_t const channel)
{
        /* Set the SPMR.CKPOL bit to 1
         * (Clock Polarity Select 1: Clock polarity is inverted)
         */
        SCI_REG(channel)->SPMR_b.CKPOL  = 1;
}  /* End of function HW_SCI_ClockPorarityInvert() */

/*******************************************************************************************************************//**
* This function selects normal polarity of the clock signal output through the SCKn pin.
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ClockPorarityNormalSet (uint32_t const channel)
{
        /* Set the SPMR.CKPOL bit to 0
         * (Clock Polarity Select 0: Clock polarity is not inverted)
         */
        SCI_REG(channel)->SPMR_b.CKPOL  = 0;
}  /* End of function HW_SCI_ClockPorarityNormalSet() */

/*******************************************************************************************************************//**
* Selects internal clock for baud rate generator
* @param[in] channel    SCI Channel number
* @retval    void
* @note      the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BaudRateGenInternalClkSelect (uint32_t const channel)
{
    SCI_REG(channel)->SCR_b.CKE    = 0;             /* Internal clock */
}  /* End of function HW_SCI_BaudRateGenInternalClkSelect() */

/*******************************************************************************************************************//**
* Selects external clock for baud rate generator
* @param[in] channel    SCI Channel number
* @retval    void
* @note      the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BaudRateGenExternalClkSelect (uint32_t const channel)
{
    /* Use external clock for baud rate */
    SCI_REG(channel)->SCR_b.CKE    = 0x2;           /* External clock */
}  /* End of function HW_SCI_BaudRateGenExternalClkSelect() */

/*******************************************************************************************************************//**
* Checks if Internal clock is selected or not
* @param[in] channel    SCI Channel number
* @retval    true   Internal clock is selected
* @retval    false  External clock is selected
* @note      the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_IsBaudRateInternalClkSelected (uint32_t const channel)
{
    return (SCI_REG(channel)->SCR_b.CKE == 0x0);
}  /* End of function HW_SCI_IsBaudRateInternalClkSelected() */

/*******************************************************************************************************************//**
* Selects 8 base clock cycles for 1-bit period
* @param[in] channel    SCI Channel number
* @retval    void
* @note      the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BaudRateGenExternalClkDivideBy8 (uint32_t const channel)
{
    SCI_REG(channel)->SEMR_b.ABCS = 1;           /* set baud rate as (external clock / 8) */
}  /* End of function HW_SCI_BaudRateGenExternalClkDivideBy8() */

/*******************************************************************************************************************//**
* Selects 16 base clock cycles for 1-bit period
* @param[in] channel    SCI Channel number
* @retval    void
* @note      the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BaudRateGenExternalClkDivideBy16 (uint32_t const channel)
{
    SCI_REG(channel)->SEMR_b.ABCS = 0;           /* set baud rate as (external clock / 16) */
}  /* End of function HW_SCI_BaudRateGenExternalClkDivideBy16() */

/*******************************************************************************************************************//**
* Checks if 8 base clock cycles for 1-bit period is selected or not
* @param[in] channel    SCI Channel number
* @retval    true   8 clocks is selected
* @retval    false  16 clocks is selected
* @note      the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_IsBaudRateGenClkDivideBy8Selected (uint32_t const channel)
{
    return (SCI_REG(channel)->SEMR_b.ABCS);
}  /* End of function HW_SCI_IsBaudRateGenClkDivideBy8Selected() */

/*******************************************************************************************************************//**
* Sets baud rate generator related registers as configured
* @param[in] channel    SCI Channel number
* @param[in] brr        BRR register setting  value
* @param[in] pbaudinfo  Baud rate information to be configured
* @retval    void
* @note      the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_UartBitRateSet (uint32_t const channel, const uint8_t brr, baud_setting_t* const pbaudinfo)
{
    SCI_REG(channel)->BRR          = brr;
    SCI_REG(channel)->SEMR_b.BGDM  = (uint8_t)(SCI_SEMR_BGDM_VALUE_MASK & pbaudinfo->bgdm);
    SCI_REG(channel)->SEMR_b.ABCS  = (uint8_t)(SCI_SEMR_ABCS_VALUE_MASK & pbaudinfo->abcs);
    SCI_REG(channel)->SEMR_b.ABCSE = (uint8_t)(SCI_SEMR_ABCSE_VALUE_MASK & pbaudinfo->abcse);
    SCI_REG(channel)->SMR_b.CKS    = (uint8_t)(SCI_SMR_CKS_VALUE_MASK & pbaudinfo->cks);
}  /* End of function HW_SCI_UartBitRateSet() */

/*******************************************************************************************************************//**
* Sets baud rate generator related registers as default
* @param[in] channel    SCI Channel number
* @param[in] brr        BRR register setting  value
* @param[in] pbaudinfo  Baud rate information to be configured
* @retval    void
* @note      the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BitRateDefaultSet (uint32_t const channel)
{
    /* Use external clock for baud rate */
    SCI_REG(channel)->BRR          = 0xFF;
    SCI_REG(channel)->SEMR_b.BGDM  = 0;
    SCI_REG(channel)->SEMR_b.ABCSE = 0;
    SCI_REG(channel)->SMR_b.CKS    = 0;
}  /* End of function HW_SCI_BitRateDefaultSet() */

/*******************************************************************************************************************//**
* Enables to output baud rate clock
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BaudClkOutputEnable (uint32_t const channel)
{
    SCI_REG(channel)->SCR_b.CKE |= (SCI_SCR_CKE_VALUE_MASK & 0x01);    /* enable to output baud clock on SCK pin */
}  /* End of function HW_SCI_BaudClkOutputEnable() */

/*******************************************************************************************************************//**
* Disables to output baud rate clock
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BaudClkOutputDisable (uint32_t const channel)
{
    SCI_REG(channel)->SCR_b.CKE &= (SCI_SCR_CKE_VALUE_MASK & (uint8_t)~(uint8_t)0x01);   /* disable to output baud clock on SCK pin */
}  /* End of function HW_SCI_BaudClkOutputDisable() */

/*****************************************************************************************************************//**
 * @brief     Sets BRR for baud rate generator register
 * @param[in] channel    SCI Channel number
 * @param[in] brr        BRR register setting  value
 *            clk_div    Clock divisor (n value)
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BitRateBRRSet (uint32_t const channel, const uint8_t brr, const uint8_t clk_div)
{
    /* Setting the BRR bits for the baud rate register */
    SCI_REG(channel)->BRR       = brr;
    SCI_REG(channel)->SMR_b.CKS = (uint8_t)(SCI_SMR_CKS_VALUE_MASK & clk_div);
}  /* End of function HW_SCI_BitRateBRRSet() */

/*******************************************************************************************************************//**
* Sets Noise cancel filter
* @param[in] channel   SCI Channel number
* @param[in] level  Noise cancel level
* @retval    void
* @note      Channel number and argument check is omitted, must be checked by SCI HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_NoiseFilterSet (uint32_t const channel, noise_cancel_lvl_t level)
{
    SCI_REG(channel)->SEMR_b.NFEN = 1;      /* enable noise filter */
    SCI_REG(channel)->SNFR = level;
}  /* End of function HW_SCI_NoiseFilterSet() */

/*******************************************************************************************************************//**
* Clears Noise cancel filter
* @param[in] channel   SCI Channel number
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_NoiseFilterClear (uint32_t const channel)
{
    SCI_REG(channel)->SEMR_b.NFEN = 0;          /* disable noise filter */
    SCI_REG(channel)->SNFR = 0x0;               /* clock divided by 1 (default) */
}  /* End of function HW_SCI_NoiseFilterClear() */

/*******************************************************************************************************************//**
* Checks if overrun error happen or not
* @param[in] channel    SCI Channel number
* @retval    true  : Overrun error happens
* @retval    false : Overrun error does not happen
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_OverRunErrorCheck (uint32_t const channel)
{
    return (1 == SCI_REG(channel)->SSR_b.ORER);
}  /* End of function HW_SCI_OverRunErrorCheck() */

/*******************************************************************************************************************//**
* Checks if framing error happen or not
* @param[in] channel    SCI Channel number
* @retval    true  : Framing error happens
* @retval    false : Framing error does not happen
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_FramingErrorCheck (uint32_t const channel)
{
    return (1 == SCI_REG(channel)->SSR_b.FER);
}  /* End of function HW_SCI_FramingErrorCheck() */

/*******************************************************************************************************************//**
* Checks if Break signal is found or not
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it. This function is only valid
*            when called in case of HW_SCI_FramingErrorCheck() returns true just before calling this function. If level
*            of RxD pin is low when framing error happens, that means receiving break signal.
***********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_BreakDetectionCheck (uint32_t const channel)
{
  	return (0 == SCI_REG(channel)->SPTR_b.RXDMON);
}  /* End of function HW_SCI_BreakDetectionCheck() */

/*******************************************************************************************************************//**
* Checks if parity error happen or not
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_ParityErrorCheck (uint32_t const channel)
{
	return (1 == SCI_REG(channel)->SSR_b.PER);
}  /* End of function HW_SCI_ParityErrorCheck() */

/*******************************************************************************************************************//**
* Clears error status
* @param[in] channel    SCI Channel number
* @note     Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ErrorConditionClear (uint32_t const channel)
{
    SCI_REG(channel)->SSR &= (uint8_t)(~SCI_RCVR_ERR_MASK);
}  /* End of function HW_SCI_ErrorConditionClear() */

/*******************************************************************************************************************//**
* Sets start bit detection when RXDn becomes low level.
* @param[in] channel   SCI channel
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_StartBitLowLevelSet (uint32_t const channel)
{
    SCI_REG(channel)->SEMR_b.RXDESEL = 0;   /* detect start bit when RXDn becomes low level */
}  /* End of function HW_SCI_StartBitLowLevelSet() */

/*****************************************************************************************************************//**
 * @brief     This function selects transfer direction (MSB first or LSB first)
 * @param[in] channel   SCI channel
 * @retval    void
 * @note      Channel number is not checked in this function, caller function must check it.
 ********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_TransferDirection (uint32_t const channel, bool const direction)
{
    /* Set the SCMR.SDIR to 0 or 1 (0: transfer with LSB first; 1: transfer with MSB first) */
    if (direction)
    {
        SCI_REG(channel)->SCMR_b.SDIR = 1;
    }
    else
    {
        SCI_REG(channel)->SCMR_b.SDIR = 0;
    }
}  /* End of function HW_SCI_TransferDirection() */

/*******************************************************************************************************************//**
 * Function for enabling/disabling the data inversion in the Smart card mode register (SCMR).
 * @param   channel The hardware channel to access
 * @param   enable  true or false
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_DataInversionEnable(uint32_t const channel, bool const enable)
{
    /* enable/disable bit */
    SCI_REG(channel)->SCMR_b.SINV = enable;
}  /* End of function HW_SCI_SIIC_DataDirectionEnable() */

/*******************************************************************************************************************//**
 * Function for enabling/disabling the data inversion in the Smart card mode register (SCMR).
 * @param   channel The hardware channel to access
 * @param   enable  true or false
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_DataLengthSet(uint32_t const channel)
{
    /* enable/disable bit */
    SCI_REG(channel)->SCMR_b.CHR1 = 1;

    SCI_REG(channel)->SMR_b.CHR = 0;


}  /* End of function HW_SCI_SIIC_DataDirectionEnable() */


/*******************************************************************************************************************//**
 * Function for enabling/disabling bit rate modulation function in serial extended mode register (SEMR).
 * @param   channel The hardware channel to access
 * @param   enable  true or false
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BitRateModulationEnable(uint32_t const channel, bool const enable)
{
    /* enable/disable bit */
    SCI_REG(channel)->SEMR_b.BRME = enable;
}  /* End of function HW_SCI_BitRateModulationEnable() */

/*******************************************************************************************************************//**
 * Function for enabling/disabling smart card function in the Smart card mode register (SCMR).
 * @param   channel The hardware channel to access
 * @param   enable  true or false
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SmartCardEnable(uint32_t const channel, bool const enable)
{
    /* enable/disable bit */
    SCI_REG(channel)->SCMR_b.SMIF = enable;
}  /* End of function HW_SCI_SIIC_SmartCardEnable() */

/*****************************************************************************************************************//**
 * @brief     This function enables/disables FIFO mode (FM bit) in FCR register
 * @param[in] channel   SCI channel
 * @param     enable  true or false
 * @retval    void
 * @note      Channel number is not checked in this function, caller function must check it.
 ********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_FIFOModeEnable(uint32_t const channel, bool const enable)
{
    /* Set the FCR.FM bit to 0 or 1 (0: FIFO off 1: FIFO on) */
    SCI_REG(channel)->FCR_b.FM = enable;
}  /* End of function HW_SCI_FIFOModeEnable */

/*******************************************************************************************************************//**
 * Function for clearing the SPI mode register (SPMR).
 * @param   channel The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SPIModeClear(uint32_t const channel)
{
    /* Clear register */
    SCI_REG(channel)->SPMR = 0;

}  /* End of function HW_SCI_SIIC_SPIModeClear() */

/*******************************************************************************************************************//**
 * Function for setting the communication mode to be be clock synchronous mode in the SMR register.
 * @param   channel The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ClockSynModeSet(uint32_t const channel)
{
    /* Clear register */
    SCI_REG(channel)->SMR_b.CM = 1;

}  /* End of function HW_SCI_ClockSynModeSet() */

/*******************************************************************************************************************//**
 * Function for setting the communication mode to be be clock synchronous mode in the SMR register.
 * @param   channel The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ClockSynModeClear(uint32_t const channel)
{
    /* Clear register */
    SCI_REG(channel)->SMR_b.CM = 0;

}  /* End of function HW_SCI_ClockSynModeSet() */

/*******************************************************************************************************************//**
 * Function for clearing the serial mode register (SMR).
 * @param   channel The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SerialModeClear(uint32_t const channel)
{
    /*  Clear register */
    SCI_REG(channel)->SMR = 0;
}  /* End of function HW_SCI_SIIC_SerialModeClear() */

/*******************************************************************************************************************//**
 * Function for clearing the smart card mode register (SCMR).
 * @param   channel The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SmartCardModeClear(uint32_t const channel)
{
    /* Clear register */
    SCI_REG(channel)->SCMR = 0;
}  /* End of function HW_SCI_SIIC_SmartCardModeClear() */

/*******************************************************************************************************************//**
 * Function for clearing the serial control register (SCR).
 * @param   channel The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SerialControlClear(uint32_t const channel)
{
    /* Clear register */
    SCI_REG(channel)->SCR = 0;
}  /* End of function HW_SCI_SIIC_SerialControlClear() */

/*******************************************************************************************************************//**
 * Function for clearing the Serial Status register (SSR).
 * @param   channel The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SerialStatusClear(uint32_t const channel)
{
    /* Clear SSR register */
    SCI_REG(channel)->SSR = 0;
}  /* End of function HW_SCI_SerialStatusClear() */

/* End of file */
