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
* File Name    : hw_sci_uart_private.h
* @brief    SCI HAL driver for UART
***********************************************************************************************************************/
#ifndef HW_SCI_UART_PRIVATE_H
#define HW_SCI_UART_PRIVATE_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "bsp_api.h"
/* Public interface header file for this module. */
#include "r_sci_common_cfg.h"
#include "r_sci_uart_cfg.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SCI_REG(channel)        ((R_SCI0_Type *)((uint32_t)R_SCI0 + ((uint32_t)R_SCI1 - (uint32_t)R_SCI0) * (channel)))
#define TDR_TDAT_MASK_9BITS     (0x01FF)
#define RDR_TDAT_MASK_9BITS     (0x01FF)
#define FTDR_TDAT_MASK_8BITS    (0x00FF)
#define FTDR_TDAT_MASK_9BITS    (0x01FF)
#define FRDR_TDAT_MASK_8BITS    (0x00FF)
#define FRDR_TDAT_MASK_9BITS    (0x01FF)
#define FCR_RTRG_MASK_4BITS     (0x000FU)

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
#if (SCI_CFG_ASYNC_INCLUDED)

/*******************************************************************************************************************//**
* @brief     Enable FIFO mode
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_FifoEnable (uint32_t const channel)
{
    SCI_REG(channel)->FCR_b.FM = 1;    /* FCRL.FM (select FIFO mode) */
}  /* End of function HW_SCI_FifoEnable() */

#if (SCI_UART_CFG_RX_ENABLE == 1)
/*******************************************************************************************************************//**
* @brief     Receive FIFO reset
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ReceiveFifoReset (uint32_t const channel)
{
    SCI_REG(channel)->FCR_b.RFRST = 1;    /* FCRL.RFRST (reset Receive FIFO Data Register) */
}  /* End of function HW_SCI_ReceiveFifoReset() */

/*******************************************************************************************************************//**
* @brief     Read out receive FIFO reset status
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_SCI_ReceiveFifoResetStatusRead (uint32_t const channel)
{
    return SCI_REG(channel)->FCR_b.RFRST;
}  /* End of function HW_SCI_ReceiveFifoResetStatusRead() */

/*******************************************************************************************************************//**
* @brief     Select RXI event which happens when detecting a reception data ready
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_RXIeventSelect (uint32_t const channel)
{
    SCI_REG(channel)->FCR_b.DRES  = 0;    /* FCRL.DRES  (select RXI happen when detecting a reception data ready) */
}  /* End of function HW_SCI_RXIeventSelect() */

/*******************************************************************************************************************//**
* @brief     RXI happens when number of received data in FIFO becomes equal or greater than this value
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_RxTriggerNumberSet (uint32_t const channel, uint32_t const number)
{
    /* FCRL.RTRG  (RXI happens when number of received data in FIFO becomes equal or greater than this value) */
    SCI_REG(channel)->FCR_b.RTRG  = number & FCR_RTRG_MASK_4BITS;
}  /* End of function HW_SCI_RxTriggerNumberSet() */

/*******************************************************************************************************************//**
* @brief     RTS signal is asserted when number of receive data in FIFO comes equal or less than this value
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_RTSTriggerNumberSet (uint32_t const channel, uint32_t const number)
{
    /* FCRL.RSTRG  (RTS asserts when number of receive data comes equal or less than this value ) */
    SCI_REG(channel)->FCR_b.RSTRG  = number & 0xF;
}  /* End of function HW_SCI_RTSTriggerNumberSet() */
#endif

#if (SCI_UART_CFG_TX_ENABLE == 1)
/*******************************************************************************************************************//**
* @brief     Transmit FIFO reset
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_TransmitFifoReset (uint32_t const channel)
{
    SCI_REG(channel)->FCR_b.TFRST = 1;    /* FCRL.TFRST (reset Transmit FIFO Data Register) */
    while(SCI_REG(channel)->FCR_b.TFRST)
    {
        /* Wait until RFRST cleared */
    }
}  /* End of function HW_SCI_TransmitFifoReset() */

/*******************************************************************************************************************//**
* @brief     Read out transmit FIFO reset status
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_SCI_TransmitFifoResetStatusRead (uint32_t const channel)
{
    return SCI_REG(channel)->FCR_b.TFRST;
}  /* End of function HW_SCI_TransmitFifoReset() */

/*******************************************************************************************************************//**
* @brief     TXI happens when number of transmit data comes equal or less than this value
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_TxTriggerNumberSet (uint32_t const channel, uint32_t const number)
{
    /* FCRL.TTRG  (TXI happens when number of transmit data comes equal or less than this value ) */
    SCI_REG(channel)->FCR_b.TTRG  = number & 0xF;
}  /* End of function HW_SCI_TxTriggerNumberSet() */
#endif

/*******************************************************************************************************************//**
* @brief     This function selects Asynchronous mode/Multi-processor mode/Clock synchronous mode.
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_AsyncModeSet (uint32_t const channel)
{
    /* Set the SIMR1.IICM bit to 0
     * (Asynchronous mode, Multi-processor mode, Clock synchronous mode if SMIF = 1)
     */
    SCI_REG(channel)->SIMR1_b.IICM  = 0;
}  /* End of function HW_SCI_AsyncModeSet() */

/*******************************************************************************************************************//**
* @brief     This function enables parity bit.
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ParityBitEnable (uint32_t const channel)
{
    SCI_REG(channel)->SMR_b.PE = 1;    /* Set the SMR.PE bit (Parity Enable) */
}  /* End of function HW_SCI_ParityEnable() */

/*******************************************************************************************************************//**
* @brief     This function disables parity bit.
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ParityBitDisable (uint32_t const channel)
{
    SCI_REG(channel)->SMR_b.PE = 0;    /* Clear the SMR.PE bit (Parity Disable) */
}  /* End of function HW_SCI_ParityBitDisable() */

/*******************************************************************************************************************//**
* @brief     This function selects odd parity.
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ParityOddSelect (uint32_t const channel)
{
    SCI_REG(channel)->SMR_b.PM = 1;    /* Set the SMR.PM bit (Select Odd parity mode ) */
}  /* End of function HW_SCI_ParityOddSelect() */

/*******************************************************************************************************************//**
* @brief     This function selects even parity.
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_ParityEvenSelect (uint32_t const channel)
{
    SCI_REG(channel)->SMR_b.PM = 0;    /* Clear the SMR.PM bit (Select Even parity mode ) */
}  /* End of function HW_SCI_ParityEvenSelect() */

/*******************************************************************************************************************//**
* @brief     This function selects 7-bit data length
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_DataBits7bitsSelect (uint32_t const channel)
{
    /* Set the SMR.CM bit to 0 (0: Communications Mode = Asynchronous) */
    /* Set the SMR.CHR bit & SCMR.CHR1 bit as selected (Character Length)
     *  Character Length
     *  (CHR1,CHR)
     *  (1, 1) Transmit/receive in 7-bit data length*3
     */
    SCI_REG(channel)->SMR_b.CHR   = 1;
    SCI_REG(channel)->SCMR_b.CHR1 = 1;
}  /* End of function HW_SCI_DataBits7bitsSelect() */

/*******************************************************************************************************************//**
* @brief     This function selects 8-bit data length
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_DataBits8bitsSelect (uint32_t const channel)
{
    /* Set the SMR.CM bit to 0 (0: Communications Mode = Asynchronous) */
    /* Set the SMR.CHR bit & SCMR.CHR1 bit as selected (Character Length)
     *  Character Length
     *  (CHR1,CHR)
     *  (1, 0) Transmit/receive in 8-bit data length (initial value)
     */
    SCI_REG(channel)->SMR_b.CHR   = 0;
    SCI_REG(channel)->SCMR_b.CHR1 = 1;
}  /* End of function HW_SCI_DataBits8bitsSelect() */

/*******************************************************************************************************************//**
* @brief     This function selects 9-bit data length
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_DataBits9bitsSelect (uint32_t const channel)
{
    /* Set the SMR.CM bit to 0 (0: Communications Mode = Asynchronous) */
    /* Set the SMR.CHR bit & SCMR.CHR1 bit as selected (Character Length)
     *  Character Length
     *  (CHR1,CHR)
     *  (0, 0) Transmit/receive in 9-bit data length
     */
    SCI_REG(channel)->SMR_b.CHR   = 0;
    SCI_REG(channel)->SCMR_b.CHR1 = 0;
}  /* End of function HW_SCI_DataBits9bitsSelect() */

/*******************************************************************************************************************//**
* @brief     This function selects stop length as 2bits for an SCI channel.
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_StopBits2bitsSelect (uint32_t const channel)
{
    SCI_REG(channel)->SMR_b.STOP = 1;    /* Set the SMR.STOP bit (2 stop bit length) */
}  /* End of function HW_SCI_StopBits2bitsSelect() */

/*******************************************************************************************************************//**
* @brief     This function selects stop length as 1bit for an SCI channel.
* @param[in] channel   SCI channel
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_StopBits1bitSelect (uint32_t const channel)
{
    SCI_REG(channel)->SMR_b.STOP = 0;    /* Clear the SMR.STOP bit (1 stop bit length) */
}  /* End of function HW_SCI_StopBits1bitSelect() */

/*******************************************************************************************************************//**
* @brief     This function writes data to transmit data register.
* @param[in] channel  SCI channel
* @param[in] data     Data to be sent
* @retval    void
* @note      All the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_Write9bits (uint32_t const channel, uint16_t const data)
{
    /* TODO_SCI (KI) shall wait until TDRE is cleared here */
    while (0 == SCI_REG(channel)->SSR_b.TDRE)
    {
        /* Wait until TDRE is cleared */
    }

    /* Write 9-bit data to FTDRx register */
    SCI_REG(channel)->TDRHL = data & TDR_TDAT_MASK_9BITS;

    return;
}  /* End of function HW_SCI_Write_9bit() */

/*******************************************************************************************************************//**
* @brief     This function reads data from receive data register
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_SCI_Read9bits (uint32_t const channel)
{
    return ((SCI_REG(channel)->RDRHL) & TDR_TDAT_MASK_9BITS);
}  /* End of function HW_SCI_Read_9bit() */

/*******************************************************************************************************************//**
* @brief     This function returns the address of the write FIFO
* @param[in] channel     SCI channel
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint8_t volatile const * HW_SCI_WriteAddrGet (uint32_t const channel, uint32_t data_bytes)
{
    if (2 == data_bytes)
    {
        /* Casting for compatibility with 7 or 8 bit mode. */
        return (uint8_t volatile const *) (&(SCI_REG(channel)->FTDR));
    }
    else
    {
        /* Casting for compatibility with 7 or 8 bit mode. */
        return (uint8_t volatile const *) ((uint32_t) (&SCI_REG(channel)->FTDR) + 1);
    }
}  /* End of function HW_SCI_WriteFIFO() */

/*******************************************************************************************************************//**
* @brief     This function writes data to transmit FIFO register
* @param[in] channel     SCI channel
* @param[in] data     Data to be sent
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_WriteFIFO (uint32_t const channel, uint16_t const data)
{
    while (0 == SCI_REG(channel)->SSR_b.TDRE)
    {
        /* Wait until TDRE is cleared */
    }

    /* Write data to data register */
    SCI_REG(channel)->FTDR = data & FTDR_TDAT_MASK_8BITS;

    return;
}  /* End of function HW_SCI_WriteFIFO() */

/*******************************************************************************************************************//**
* @brief     This function writes data to transmit FIFO register, FIFO mode, 9-bit data length mode
* @param[in] channel     SCI channel
* @param[in] data     Data to be sent
* @retval    void
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_WriteFIFO_9bit (uint32_t const channel, uint16_t const data)
{
    /* TODO_UART (KI) shall wait until TDRE is cleared here */
    while (0 == SCI_REG(channel)->SSR_b.TDRE)
    {
        /* Wait until TDRE is cleared */
    }

    /* Write 9-bit data to FTDRx register */
    SCI_REG(channel)->FTDR = data & FTDR_TDAT_MASK_9BITS;

    return;
}  /* End of function HW_SCI_WriteFIFO_9bit() */

/*******************************************************************************************************************//**
* @brief     This function returns the address of the read FIFO.
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint8_t volatile const * HW_SCI_ReadAddrGet (uint32_t const channel, uint32_t data_bytes)
{
    if (2 == data_bytes)
    {
        /* Casting for compatibility with 7 or 8 bit mode. */
        return (uint8_t volatile const *) (&(SCI_REG(channel)->FRDR));
    }
    else
    {
        /* Casting for compatibility with 7 or 8 bit mode. */
        return (uint8_t volatile const *) ((uint32_t) &(SCI_REG(channel)->FRDR) + 1);
    }
}  /* End of function HW_SCI_ReadFIFO() */

/*******************************************************************************************************************//**
* @brief     This function reads data from receive FIFO register
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_SCI_ReadFIFO (uint32_t const channel)
{
    return (SCI_REG(channel)->FRDR) & FRDR_TDAT_MASK_8BITS;
}  /* End of function HW_SCI_ReadFIFO() */

/*******************************************************************************************************************//**
* @brief     This function reads data from receive FIFO register, 9-bit data lenght mode
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_SCI_ReadFIFO_9bit (uint32_t const channel)
{
    return ((SCI_REG(channel)->FRDR) & FRDR_TDAT_MASK_9BITS);
}  /* End of function HW_SCI_ReadFIFO_9bit() */

/*******************************************************************************************************************//**
* @brief     This function clears TDFE flag
* @param[in] channel    SCI Channel number
* @retval    none
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_TDFEClear (uint32_t const channel)
{
    /* Clear TDFE flag */
    SCI_REG(channel)->SSR_FIFO_b.TDFE = 0;
}  /* End of function HW_SCI_TDFEClear() */

/*******************************************************************************************************************//**
* @brief     This function clears RDF flag (also clears DR flag)
* @param[in] channel    SCI Channel number
* @retval    none
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_RDFClear (uint32_t const channel)
{
    /* Clear RDF flag */
    SCI_REG(channel)->SSR_FIFO_b.RDF = 0;

    /* Clear DR flag */
    SCI_REG(channel)->SSR_FIFO_b.DR = 0;

}  /* End of function HW_SCI_RDFClear() */

/*******************************************************************************************************************//**
* @brief     This function reads data number in transmit FIFO
* @param[in] channel    SCI Channel number
* @retval    Data number in transmit FIFO
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint32_t HW_SCI_FIFO_WriteCount (uint32_t const channel)
{
    return (uint32_t)(SCI_REG(channel)->FDR_b.T);
}  /* End of function HW_SCI_FIFO_WriteCount() */

/*******************************************************************************************************************//**
* @brief     This function reads data number in receive FIFO
* @param[in] channel    SCI Channel number
* @retval    Data number in receive FIFO
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint32_t HW_SCI_FIFO_ReadCount (uint32_t const channel)
{
    return (uint32_t)(SCI_REG(channel)->FDR_b.R);
}  /* End of function HW_SCI_FIFO_ReadCount() */

/*******************************************************************************************************************//**
* @brief     Checks whether data length is 9bits or not
* @param[in] channel   SCI channel
* @return    true if data length is 9bits
* @return    false if data length is not 9bits
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_IsDataLength9bits (uint32_t const channel)
{
	/* if CHR1 = 0, it means data length is 9-bit */
    return (0 == SCI_REG(channel)->SCMR_b.CHR1);
}  /* End of function HW_SCI_IsDataLength9bits() */

/*******************************************************************************************************************//**
* @brief     Enables CTS
* @param[in] channel   SCI channel
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_CtsInEnable (uint32_t const channel)
{
    SCI_REG(channel)->SPMR_b.CTSE = 1;      /* enables CTS (disables RTS) */
}  /* End of function HW_SCI_CtsInEnable() */

/*******************************************************************************************************************//**
* @brief     Enables RTS
* @param[in] channel   SCI channel
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_RtsOutEnable (uint32_t const channel)
{
    SCI_REG(channel)->SPMR_b.CTSE = 0;      /* enables RTS (disables CTS) */
}  /* End of function HW_SCI_RtsOutEnable() */

/*******************************************************************************************************************//**
* @brief     Sets start bit detection when RXDn has falling edge.
*            This mode stops data reception during BREAK condition.
* @param[in] channel   SCI channel
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_StartBitFallingEdgeSet (uint32_t const channel)
{
    SCI_REG(channel)->SEMR_b.RXDESEL = 1;   /* detect start bit if RXDn has falling edge */
}  /* End of function HW_SCI_StartBitFallingEdgeSet() */

/*******************************************************************************************************************//**
* @brief     Send break signal from TXD pin
* @param[in] channel   SCI channel
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BreakSend (uint32_t const channel)
{
    SCI_REG(channel)->SPTR_b.SPB2DT = 0;    /* Low level is output in TXD terminal when SPB2IO=1&TE=0 */
    SCI_REG(channel)->SPTR_b.SPB2IO = 1;    /* The value of SPB2DT is output to TXD terminal when TE=0 */
    SCI_REG(channel)->SCR_b.TEIE    = 0;    /* disable TXI interrupt */
    SCI_REG(channel)->SCR_b.TE      = 0;    /* output to low level to TXD to generate break */
}  /* End of function HW_SCI_BreakSend() */

/*******************************************************************************************************************//**
* @brief     Finish to send break signal from TXD pin
* @param[in] channel   SCI channel
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_BreakEnd (uint32_t const channel)
{
    SCI_REG(channel)->SPTR_b.SPB2DT = 1;    /* Set SPB2IO to default value(1) */
    SCI_REG(channel)->SPTR_b.SPB2IO = 0;    /* The value of SPB2DT is output to TXD terminal when TE=0 */
}  /* End of function HW_SCI_BreakEnd() */

#endif

#endif /* HW_SCI_UART_PRIVATE_H */
