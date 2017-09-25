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
* File Name    : hw_sci_i2c_private.h
* Description  : SCI HAL driver for Simple I2C interface hardware definitions and declarations
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup SIIC
 * @{
***********************************************************************************************************************/
#ifndef HW_SCI_SIIC_PRIVATE_H
#define HW_SCI_SIIC_PRIVATE_H

/**********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "bsp_api.h"
#include "r_sci_i2c.h"
#include "r_sci_common_cfg.h"

#if (SCI_CFG_SIIC_INCLUDED)
/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#if defined(__GNUC__) && defined(__ARM_EABI__)
#define INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define INLINE_ATTRIBUTE
#endif

#define    SCI_SIIC_SIMR3_STARTREQ    (0x51)
#define    SCI_SIIC_SIMR3_RESTARTREQ  (0x52)
#define    SCI_SIIC_SIMR3_STOPREQ     (0x54)
#define    SCI_SIIC_SIMR3_CLEARREQ    (0x00)
#define    SCI_SIIC_SIMR3_HALTPREQ    (0xF0)

/** SCI register access or macro  */
#define SCI_REG(channel)    ((R_SCI0_Type *)((uint32_t)R_SCI0 + ((uint32_t)R_SCI1 - (uint32_t)R_SCI0) * (channel)))

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/** SCI SIIC Simple Mode select */
typedef enum e_sci_siic_mode
{
    SCI_SIIC_MODE_ASYNCHRONOUS = 0,  /**< SCI SIIC asynchronous mode, multiprocessor, clock synchronous mode */
    SCI_SIIC_MODE_I2C,               /**< SCI simple I2C mode */
    SCI_SIIC_MODE_SMART_CARD,        /**< SCI smart card mode */
    SCI_SIIC_MODE_PROHIBITED,        /**< SCI prohibited mode */
} sci_siic_mode_t;

/** SCI SIIC Output pin state for SDA/SCL */
typedef enum e_sci_siic_sda_scl_output
{
    SCI_SIIC_OUTPUT_NORMAL = 0,      /**< SCI SIIC SDA/SCL pins operate in serial data mode mode */
    SCI_SIIC_OUTPUT_COMMAND,         /**< SCI SIIC SDA/SCL pins operate in command (start, restart, stop) mode */
    SCI_SIIC_OUTPUT_LOW,             /**< SCI SIIC SDA/SCL pins set to low level output */
    SCI_SIIC_OUTPUT_OFF,             /**< SCI SIIC SDA/SCL pins set to high impedance mode  */
} sci_siic_sda_scl_output_t;

/** SCI SIIC Interrupt Source select */
typedef enum e_sci_siic_interrupt_source
{
    SCI_SIIC_INTERRUPT_SOURCEE_ACK_NACK = 0,            /**< SCI SIIC uses ACK/NACK interrupts */
    SCI_SIIC_INTERRUPT_SOURCE_RECEPTION_TRANSMISSION,   /**< SCI SIIC uses reception and transmission interrupts */
} sci_siic_interrupt_source_t;

/*******************************************************************************************************************//**
* This function sets the I2C mode in I2C Mode Register 1 (SIMR1).
* @param[in] channel    SCI Channel number
* @param[in] sci_siic_mode    set mode as Asynchronous/Multiprocessor/Synchronous, Simple I2C, smart card
* @note      Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_ModeSet (uint32_t const channel, sci_siic_mode_t sci_siic_mode)
{
    SCI_REG(channel)->SIMR1_b.IICM = sci_siic_mode;

}  /* End of function HW_SCI_SIICModeSet() */

/*******************************************************************************************************************//**
* This function initiates a start condition in the I2C Mode Register 3 (SIMR3).
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_SendStart (uint32_t const channel)
{
		/* The IICRSTARREQ, IICSDAS, IICSCLS bits must be set simultaneously */
	SCI_REG(channel)->SIMR3 = SCI_SIIC_SIMR3_STARTREQ;
	/* Wait for command flag to be set before return */
	while(SCI_REG(channel)->SIMR3_b.IICSTIF == 0x0);

	SCI_REG(channel)->SIMR3 = 0x00; //Clear IICSTIF, IICSCLS and IICSDAS to 0
}  /* End of function HW_SCI_SIIC_SendStart() */

/*******************************************************************************************************************//**
* This function initiates a restart condition in the I2C Mode Register 3 (SIMR3).
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_SendRestart(uint32_t const channel)
{
	/* The IICRSTARREQ, IICSDAS, IICSCLS bits must be set simultaneously */
    SCI_REG(channel)->SIMR3 = SCI_SIIC_SIMR3_RESTARTREQ;

    /* Wait for command flag to be set before return */
	while(SCI_REG(channel)->SIMR3_b.IICSTIF == 0x0);

	SCI_REG(channel)->SIMR3 = 0x00; //Clear IICSTIF, IICSCLS and IICSDAS to 0
}  /* End of function HW_SCI_SIIC_SendRestart() */

/*******************************************************************************************************************//**
* This function initiates a stop condition in the I2C Mode Register 3 (SIMR3).
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_SendStop(uint32_t const channel)
{
	/* The IICRSTARREQ, IICSDAS, IICSCLS bits must be set simultaneously */
    SCI_REG(channel)->SIMR3 = SCI_SIIC_SIMR3_STOPREQ;

	/* Wait for command flag to be set before return */
	while(SCI_REG(channel)->SIMR3_b.IICSTIF == 0x0);

	SCI_REG(channel)->SIMR3 = 0xF0; //Clear IICSTIF=0, set output pin high impedance

}  /* End of function HW_SCI_SIIC_SendStop() */

/*******************************************************************************************************************//**
* Waits for Start/Restart/Stop condition to be completed and clears IICSTIF and sets SDA and SCL to data output.
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_Wait_to_ClearLine(uint32_t const channel)
{
	/* Wait for Start/Restart/Stop condition to be completed */
	while (SCI_REG(channel)->SIMR3_b.IICSTIF == 0x0);

	/* Clear I2C mode register command issuing flag and enable SDA/SCL for serial data output mode */
    SCI_REG(channel)->SIMR3 = SCI_SIIC_SIMR3_CLEARREQ;

}  /* End of function HW_SCI_SIIC_Wait_to_ClearLine() */

/*******************************************************************************************************************//**
* Waits for Start/Restart/Stop condition to be completed and clears IICSTIF and sets SDA and SCL to data output.
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_Wait_to_ShutLine(uint32_t const channel)
{
	/* Wait for Start/Restart/Stop condition to be completed */
	while (SCI_REG(channel)->SIMR3_b.IICSTIF == 0x0);

	/* Clear I2C mode register command issuing flag and disable SDA/SCL output mode */
    SCI_REG(channel)->SIMR3 = SCI_SIIC_SIMR3_HALTPREQ;

}  /* End of function HW_SCI_SIIC_Wait_to_ShutLine() */

/*******************************************************************************************************************//**
* This function clears the command issuing flag and sets SDA/SCL to serial output in the I2C Mode Register 3 (SIMR3).
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_LineClear(uint32_t const channel)
{
    /* Clear I2C mode register command issuing flag and enable SDA/SCL for serial data output mode */
    SCI_REG(channel)->SIMR3 = SCI_SIIC_SIMR3_CLEARREQ;
}

/*******************************************************************************************************************//**
* This function clears the command issuing flag and sets SDA/SCL to high-impedance state in the I2C Mode Register 3 (SIMR3).
* @param[in] channel    SCI Channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_LineHalt(uint32_t const channel)
{
    /* Clear I2C mode register command issuing flag and sets SDA/SCL to high-impedance state */
    SCI_REG(channel)->SIMR3 = SCI_SIIC_SIMR3_HALTPREQ;
}

/*******************************************************************************************************************//**
* This function sets the SDA/SCL pin output state in the I2C Mode Register 3 (SIMR3).
* @param   channel The hardware channel to access
* @param   state  state for the SDA/SCL pin output
* @note     Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_Output (uint32_t const channel, sci_siic_sda_scl_output_t const state)
{
   SCI_REG(channel)->SIMR3_b.IICSDAS = state;    /**< set SDA output state */
   SCI_REG(channel)->SIMR3_b.IICSCLS = state;    /**< set SCL output state */

   /* Give time for value to be updated */
   volatile uint8_t dummy;
   /** read the registers*/
   dummy = SCI_REG(channel)->SIMR3_b.IICSDAS;
   dummy = SCI_REG(channel)->SIMR3_b.IICSCLS;
   dummy = SCI_REG(channel)->SIMR3_b.IICSDAS;
   dummy = SCI_REG(channel)->SIMR3_b.IICSCLS;
   /** Do this to remove compile warnings */
   SSP_PARAMETER_NOT_USED(dummy);
}  /* End of function HW_SCI_SIIC_Output() */

/*******************************************************************************************************************//**
 * This function returns true when the SDA line is being held low.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_SIIC_SDALow(uint32_t const channel)
{
    return (SCI_SIIC_OUTPUT_LOW ==  SCI_REG(channel)->SIMR3_b.IICSDAS);  /**< return true if SDA pin stays at low level */
}  /* End of function HW_SCI_SIIC_SDALow() */

/*******************************************************************************************************************//**
 * Returns true when a requested stop condition has not yet been issued.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_SIIC_StopPending(uint32_t const channel)
{
    return (1 == SCI_REG(channel)->SIMR3_b.IICSTPREQ);
}  /* End of function HW_SCI_SIIC_StopPending() */

/*******************************************************************************************************************//**
* This function sets the SDA output delay in the I2C Mode Register 1 (SIMR1).
* @param   channel The hardware channel to access
* @param   delay   add delay to the SDA output up to 31 cycles
* @note    Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_DataOutputDelay (uint32_t const channel, uint32_t const delay)
{
   SCI_REG(channel)->SIMR1_b.IICDL = (uint8_t)(delay & 0x1F);    /**< set SDA output delay */
}  /* End of function HW_SCI_SIIC_DataOutputDelay() */

/*******************************************************************************************************************//**
* This function enables/disables the ACK/NACK transmission and reception (IICACKT bit) in the I2C Mode Register 2 (SIMR2).
* @param   channel The hardware channel to access
* @param   enable  true when ACK and NACK bits are received, false, otherwise.
* @note    Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_ACKNACKEnable (uint32_t const channel, bool const enable)
{
   SCI_REG(channel)->SIMR2_b.IICACKT = enable;    /**< Set ACK/NACK transmission */
}  /* End of function HW_SCI_SIIC_ACKNACK_Enable() */

/*******************************************************************************************************************//**
* This function enables/disables the ACK/NACK transmission and reception (IICACKT bit) in the I2C Mode Register 2 (SIMR2).
* @param   channel The hardware channel to access
* @param   enable  true when ACK and NACK bits are received, false, otherwise.
* @note    Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_FIFODisable(uint32_t const channel)
{
   SCI_REG(channel)->FCR_b.FM = 0;    /**< Set ACK/NACK transmission */
}  /* End of function HW_SCI_SIIC_ACKNACK_Enable() */

/*******************************************************************************************************************//**
* This function enables/disables the clock synchronization (IICCSC bit) in the I2C Mode Register 2 (SIMR2).
* @param   channel The hardware channel to access
* @param   enable  true selects clock synchronization, false, otherwise.
* @note    Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_ClockSynchronizationEnable (uint32_t const channel, bool const enable)
{
   SCI_REG(channel)->SIMR2_b.IICCSC = enable;    /**< enable/disable clock synchronization */
}  /* End of function HW_SCI_SIIC_ClockSynchronizationEnable() */

/*******************************************************************************************************************//**
* This function selects the source on interrupt requests (IICINTM bit) in simple I2C mode in the I2C Mode Register 2 (SIMR2).
* @param   channel The hardware channel to access
* @param   enable  true selects clock synchronization, false, otherwise.
* @note    Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_InterruptSource (uint32_t const channel, sci_siic_interrupt_source_t const source)
{
   SCI_REG(channel)->SIMR2_b.IICINTM = source;    /**< set the source for interrupts */
}  /* End of function HW_SCI_SIIC_InterruptSource() */

/*******************************************************************************************************************//**
 * Sets the acknowledge bit when getting ready to signal the slave to stop transmitting.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_AckSet (uint32_t const channel)
{
   SCI_REG(channel)->SIMR2_b.IICACKT = 0;    /**< set ACK transmission data */
}

/*******************************************************************************************************************//**
* Checks if ACK has been received or not
* @param[in] channel    SCI Channel number
* @note      Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_SIIC_AckReceived (uint32_t const channel)
{
  	return (0 == SCI_REG(channel)->SISR_b.IICACKR);
}  /* End of function HW_SCI_SIIC_AckReceived() */

/*******************************************************************************************************************//**
* Checks if NACK has been received or not
* @param[in] channel    SCI Channel number
* @note      Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE bool HW_SCI_SIIC_NAckReceived (uint32_t const channel)
{
  	return (1 == SCI_REG(channel)->SISR_b.IICACKR);
}  /* End of function HW_SCI_SIIC_NAckReceived() */

/*******************************************************************************************************************//**
* Waits for the character transfer to be completed
* * @param[in] channel    SCI Channel number
* @note      Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_Wait_TXEnd (uint32_t const channel)
{
  	while (!SCI_REG(channel)->SSR_b.TEND);
}  /* End of function HW_SCI_SIIC__Wait_TXEnd() */

/*******************************************************************************************************************//**
* Waits for the TX buffer to be ready to transfer
* * @param[in] channel    SCI Channel number
* @note      Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_Wait_TXReady (uint32_t const channel)
{
  	while (!SCI_REG(channel)->SSR_b.TDRE);
}  /* End of function HW_SCI_SIIC__Wait_TXReady() */

/*******************************************************************************************************************//**
* Waits for the RX buffer to be buffer (received data ready)
* * @param[in] channel    SCI Channel number
* @note      Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_Wait_RXReady (uint32_t const channel)
{
  	while (!SCI_REG(channel)->SSR_b.RDRF);
}  /* End of function HW_SCI_SIIC__Wait_RXReady() */

/*******************************************************************************************************************//**
* Waits for an ACK to be received
* @param[in] channel    SCI Channel number
* @note      Parameter check is not held in this function.
***********************************************************************************************************************/
__STATIC_INLINE void HW_SCI_SIIC_Wait_Ack (uint32_t const channel)
{
  	while (1 == SCI_REG(channel)->SISR_b.IICACKR);
}  /* End of function HW_SCI_SIIC_Wait_Ack() */

#endif /* end of SCI_CFG_SIIC_INCLUDED */

#endif /* HW_SCI_SIIC_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup SIIC)
***********************************************************************************************************************/
