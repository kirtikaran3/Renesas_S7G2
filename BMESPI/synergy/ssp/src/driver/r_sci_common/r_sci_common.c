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
* File Name    : r_sci_common.c
* Description  : SCI HAL driver
***********************************************************************************************************************/


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_sci_private.h"
#include "r_sci_common.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef SCI_COMMON_ERROR_RETURN
#define SCI_COMMON_ERROR_RETURN(a, err)       SSP_ERROR_RETURN((a), (err), "sci_common", NULL)
#endif

#define SCI_MAX_CH_S3A7 4
#define SCI_MAX_CH_S7G2 9
#define SCI_MAX_CH_S1   1
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/
static void      rxi_handler (uint32_t const channel);
static void      txi_handler (uint32_t const channel);
static void      tei_handler (uint32_t const channel);
static void      eri_handler (uint32_t const channel);

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/
/** SCI control block array for channel */
sci_ctrl_t g_sci_ctrl_blk[SCI_PHY_CH_MAX];

/***********************************************************************************************************************
Functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
* Common subroutine for closing channel
* @param[in] channel    Channel number of SCI module
***********************************************************************************************************************/
void r_sci_close_common (uint32_t channel)
{
    /** disables interrupt */
    HW_SCI_InterruptEnable (channel, SCI_ALL_INT, false);

    HW_SCI_ReceiverDisable (channel);               ///< disables receiver

    HW_SCI_TransmitterDisable (channel);            ///< disables transmitter

    HW_SCI_BaudClkOutputDisable (channel);          ///< set default setting

    HW_SCI_StartBitLowLevelSet (channel);           ///< set default setting

    /** sets default value to Noise Filter related register */
    HW_SCI_NoiseFilterClear (channel);

    /** removes power to channel */
    HW_SCI_PowerOff (channel);

    /** unlocks specified SCI channel */
    r_sci_hardware_unlock (channel);

    /** Clear device context */
    g_sci_ctrl_blk[channel].mode        = SCI_MODE_OFF;
    g_sci_ctrl_blk[channel].tx_busy     = false;
    g_sci_ctrl_blk[channel].p_context   = NULL;
    g_sci_ctrl_blk[channel].p_extpin_ctrl = NULL;

}  /* End of function r_sci_close_common() */

/***********************************************************************************************************************
Private Functions
***********************************************************************************************************************/
/*******************************************************************************************************************//**
* Lock specified SCI channel
* @param[in] channel    Channel number of SCI module
* @retval    none
***********************************************************************************************************************/
ssp_err_t r_sci_hardware_lock (uint32_t channel)
{
    /* Channel 9 is separated because, for S124 there is no channels between 1 and 9 so it should not calculate incorrect enum value for channel9.
     * Similarly for S3A7 channels are 0,1,2,3,4 and 9 */
    if(channel == 9)
    {
        if (SSP_ERR_IN_USE == R_BSP_HardwareLock ((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_SCI9)))
        {
            return SSP_ERR_HW_LOCKED;
        }
    }
    else
    {
        if (SSP_ERR_IN_USE == R_BSP_HardwareLock ((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_SCI0 + channel)))
        {
            return SSP_ERR_HW_LOCKED;
        }
    }
    return SSP_SUCCESS;
}  /* End of function r_sci_hardware_lock() */

/*******************************************************************************************************************//**
* Unlock specified SCI channel
* @param[in] channel    Channel number of SCI module
* @retval    none
***********************************************************************************************************************/
void r_sci_hardware_unlock (uint32_t channel)
{
    /* All SCI channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    if(channel == 9)
    {
        /*For S124 there is no channels between 1 and 9 so it should not calculate incorrect enum value for channel-9.
         * Similarly for S3A7 channels are 0,1,2,3,4 and 9 */
        R_BSP_HardwareUnlock ((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_SCI9));
    }
    else
    {
        R_BSP_HardwareUnlock ((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_SCI0 + channel));
    }
}  /* End of function r_sci_hardware_unlock() */

/*******************************************************************************************************************//**
* Common channel check for all SCI modules.
* @param[in] channel    Channel number of SCI module
* @retval    none
***********************************************************************************************************************/
ssp_err_t r_sci_channel_param_check (uint32_t channel)
{
    /* SCI has channels as follows,
	   S7 : 0 1 2 3 4 5 6 7 8 9
	   S3 : 0 1 2 3 4 - - - - 9
       S1 : 0 1 - - - - - - - 9
       Access to channel 6 - 8 in S3, channel 2 - 8 in S1 should be rejected.
     */
#if   (BSP_CFG_MCU_PART_SERIES == 7)
	SCI_COMMON_ERROR_RETURN((channel <= SCI_MAX_CH_S7G2), SSP_ERR_INVALID_CHANNEL);
#elif (BSP_CFG_MCU_PART_SERIES == 3)
	SCI_COMMON_ERROR_RETURN(((SCI_MAX_CH_S3A7 >= channel) || (9 == channel)), SSP_ERR_INVALID_CHANNEL);
#elif (BSP_CFG_MCU_PART_SERIES == 1)
	SCI_COMMON_ERROR_RETURN(((SCI_MAX_CH_S1 >= channel) || (9 == channel)), SSP_ERR_INVALID_CHANNEL);
#endif
	return SSP_SUCCESS;
}  /* End of function r_sci_channel_param_check() */

/*******************************************************************************************************************//**
* Data transmit end interrupt(TXI or TEI) handler for SCI
* @param[in] channel    Channel number of SCI module
* @retval    none
***********************************************************************************************************************/
static void txi_handler (uint32_t const channel)
{
    if (g_sci_ctrl_blk[channel].mode == SCI_MODE_ASYNC)
    {
#if (SCI_CFG_ASYNC_INCLUDED)
        /* ASYNC mode */
        r_sci_uart_txi_common (channel);
#endif
    }
    else if (g_sci_ctrl_blk[channel].mode == SCI_MODE_SPI)
    {
        /* SCI SPI mode */
#if (SCI_CFG_SPI_INCLUDED)
        r_sci_spi_tx_rx_common(channel, SCI_TX_INT);
#endif
    }
    else if (g_sci_ctrl_blk[channel].mode == SCI_MODE_SIIC)
    {
        /* SIIC mode */
#if (SCI_CFG_SIIC_INCLUDED)
    	r_sci_siic_txi_master(channel);
#endif
    }
    else
    {
         /* do other modes */
    }
}  /* End of function txi_handler () */

/*******************************************************************************************************************//**
* TEI interrupt handler for SCI
* @param[in] channel    Channel number of SCI module
* @retval    none
***********************************************************************************************************************/
static void tei_handler (uint32_t const channel)
{
    if (g_sci_ctrl_blk[channel].mode == SCI_MODE_ASYNC)
    {
#if (SCI_CFG_ASYNC_INCLUDED)
        r_sci_uart_tei (channel);
    #endif
    }
    else if (g_sci_ctrl_blk[channel].mode == SCI_MODE_SIIC)
    {
#if (SCI_CFG_SIIC_INCLUDED)
    	r_sci_siic_tei_master(channel);
#endif
    }
    else if (g_sci_ctrl_blk[channel].mode == SCI_MODE_SPI)
    {
#if (SCI_CFG_SPI_INCLUDED)
    	r_sci_spi_tx_rx_common(channel, SCI_TE_INT);
#endif
    }
    else
    {
        /* do other modes */
    }
}  /* End of function tei_handler () */

/*******************************************************************************************************************//**
* Data Reception interrupt(RXI) handler for SCI
* @param[in] channel    Channel number of SCI module
* @retval    none
***********************************************************************************************************************/
static void rxi_handler (uint32_t const channel)
{
    if (g_sci_ctrl_blk[channel].mode == SCI_MODE_ASYNC)
    {
#if (SCI_CFG_ASYNC_INCLUDED)
        /* ASYNC mode */
        r_sci_uart_rxi_common (channel);
        HW_SCI_RDFClear(channel);
#endif
    }
    else if (g_sci_ctrl_blk[channel].mode == SCI_MODE_SPI)
    {
#if (SCI_CFG_SPI_INCLUDED)
        r_sci_spi_tx_rx_common(channel, SCI_RX_INT);
#endif
    }
    else if (g_sci_ctrl_blk[channel].mode == SCI_MODE_SIIC)
    {
#if (SCI_CFG_SIIC_INCLUDED)
    	r_sci_siic_rxi_master(channel);
#endif
    }
    else
    {
        /* do other modes */
    }
}  /* End of function rxi_handler () */

/*******************************************************************************************************************//**
* Error interrupt handler for SCI
* @param[in] channel    Channel number of SCI module
* @retval    none
***********************************************************************************************************************/
static void eri_handler (uint32_t const channel)
{
    if (g_sci_ctrl_blk[channel].mode == SCI_MODE_ASYNC)
    {
#if (SCI_CFG_ASYNC_INCLUDED)
        /* ASYNC mode */
        r_sci_uart_eri_common (channel);
#endif
    }
    else if (g_sci_ctrl_blk[channel].mode == SCI_MODE_SPI)
    {
#if (SCI_CFG_SPI_INCLUDED)
        /* SCI SPI, SYNC mode */
        r_sci_spi_spei_isr_common(channel);

#endif
    }
    else if (g_sci_ctrl_blk[channel].mode == SCI_MODE_SIIC)
    {
#if (SCI_CFG_SIIC_INCLUDED)
        /* SIIC, SYNC mode */
    	r_sci_siic_err_master(channel);
#endif
    }
    else
    {
         /* do other modes */
    }

    /* Clear error condition */
    HW_SCI_ErrorConditionClear (channel);

}  /* End of function eri_handler () */

/*******************************************************************************************************************//**
* TXI interrupt routines for every SCI channel
* @param  none
* @retval none
***********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_SCI0_TXI
#if (BSP_IRQ_CFG_SCI0_TXI != BSP_IRQ_DISABLED)
void sci0_txi_isr (void);
void sci0_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (0);
    R_BSP_IrqStatusClear (SCI0_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI0_TXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci0_txi_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI1_TXI
#if (BSP_IRQ_CFG_SCI1_TXI != BSP_IRQ_DISABLED)
void sci1_txi_isr (void);
void sci1_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (1);
    R_BSP_IrqStatusClear (SCI1_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI1_TXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci1_txi_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI2_TXI
#if (BSP_IRQ_CFG_SCI2_TXI != BSP_IRQ_DISABLED)
void sci2_txi_isr (void);
void sci2_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (2);
    R_BSP_IrqStatusClear (SCI2_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI2_TXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci2_txi_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI3_TXI
#if (BSP_IRQ_CFG_SCI3_TXI != BSP_IRQ_DISABLED)
void sci3_txi_isr (void);
void sci3_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (3);
    R_BSP_IrqStatusClear (SCI3_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI3_TXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci3_txi_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI4_TXI
#if (BSP_IRQ_CFG_SCI4_TXI != BSP_IRQ_DISABLED)
void sci4_txi_isr (void);
void sci4_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (4);
    R_BSP_IrqStatusClear (SCI4_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI4_TXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci4_txi_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI5_TXI
#if (BSP_IRQ_CFG_SCI5_TXI != BSP_IRQ_DISABLED)
void sci5_txi_isr (void);
void sci5_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (5);
    R_BSP_IrqStatusClear (SCI5_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI5_TXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci5_txi_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI6_TXI
#if (BSP_IRQ_CFG_SCI6_TXI != BSP_IRQ_DISABLED)
void sci6_txi_isr (void);
void sci6_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (6);
    R_BSP_IrqStatusClear (SCI6_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI6_TXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci6_txi_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI7_TXI
#if (BSP_IRQ_CFG_SCI7_TXI != BSP_IRQ_DISABLED)
void sci7_txi_isr (void);
void sci7_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (7);
    R_BSP_IrqStatusClear (SCI7_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI7_TXI_IRQn]; /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci7_txi_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI8_TXI
#if (BSP_IRQ_CFG_SCI8_TXI != BSP_IRQ_DISABLED)
void sci8_txi_isr (void);
void sci8_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (8);
    R_BSP_IrqStatusClear (SCI8_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI8_TXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci8_txi_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI9_TXI
#if (BSP_IRQ_CFG_SCI9_TXI != BSP_IRQ_DISABLED)
void sci9_txi_isr (void);
void sci9_txi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    txi_handler (9);
    R_BSP_IrqStatusClear (SCI9_TXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI9_TXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci9_txi_isr () */
#endif
#endif


/*******************************************************************************************************************//**
* RXI interrupt routines for SCI channels. The RXI ISR is Receive Interrupt service routine. This function is executed
*  when TXI interrupt happen. RXI interrupt is generated when received data is stored in the RDR (in case of FIFO
*  disabled) or when the number of data stored in FRDRL register becomes the value preset to FCRHL.RTRG or above (in
*  case of FIFO enabled).
* @param  none
* @retval none
***********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_SCI0_RXI
#if (BSP_IRQ_CFG_SCI0_RXI != BSP_IRQ_DISABLED)
void sci0_rxi_isr (void);
void sci0_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (0);
    R_BSP_IrqStatusClear (SCI0_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI0_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci0_rxi_isr */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI1_RXI
#if (BSP_IRQ_CFG_SCI1_RXI != BSP_IRQ_DISABLED)
void sci1_rxi_isr (void);
void sci1_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (1);
    R_BSP_IrqStatusClear (SCI1_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI1_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci1_rxi_isr */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI2_RXI
#if (BSP_IRQ_CFG_SCI2_RXI != BSP_IRQ_DISABLED)
void sci2_rxi_isr (void);
void sci2_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (2);
    R_BSP_IrqStatusClear (SCI2_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI2_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci2_rxi_isr */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI3_RXI
#if (BSP_IRQ_CFG_SCI3_RXI != BSP_IRQ_DISABLED)
void sci3_rxi_isr (void);
void sci3_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (3);
    R_BSP_IrqStatusClear (SCI3_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI3_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci3_rxi_isr */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI4_RXI
#if (BSP_IRQ_CFG_SCI4_RXI != BSP_IRQ_DISABLED)
void sci4_rxi_isr (void);
void sci4_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (4);
    R_BSP_IrqStatusClear (SCI4_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI4_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci4_rxi_isr */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI5_RXI
#if (BSP_IRQ_CFG_SCI5_RXI != BSP_IRQ_DISABLED)
void sci5_rxi_isr (void);
void sci5_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (5);
    R_BSP_IrqStatusClear (SCI5_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI5_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci5_rxi_isr */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI6_RXI
#if (BSP_IRQ_CFG_SCI6_RXI != BSP_IRQ_DISABLED)
void sci6_rxi_isr (void);
void sci6_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (6);
    R_BSP_IrqStatusClear (SCI6_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI6_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci6_rxi_isr */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI7_RXI
#if (BSP_IRQ_CFG_SCI7_RXI != BSP_IRQ_DISABLED)
void sci7_rxi_isr (void);
void sci7_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (7);
    R_BSP_IrqStatusClear (SCI7_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI7_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci7_rxi_isr */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI8_RXI
#if (BSP_IRQ_CFG_SCI8_RXI != BSP_IRQ_DISABLED)
void sci8_rxi_isr (void);
void sci8_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (8);
    R_BSP_IrqStatusClear (SCI8_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI8_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci8_rxi_isr */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI9_RXI
#if (BSP_IRQ_CFG_SCI9_RXI != BSP_IRQ_DISABLED)
void sci9_rxi_isr (void);
void sci9_rxi_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    rxi_handler (9);
    R_BSP_IrqStatusClear (SCI9_RXI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI9_RXI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
} /* end of function sci9_rxi_isr */
#endif
#endif

/*******************************************************************************************************************//**
* ERI interrupt routines for every SCI channel
* @param  none
* @retval none
***********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_SCI0_ERI
#if (BSP_IRQ_CFG_SCI0_ERI != BSP_IRQ_DISABLED)
void sci0_eri_isr (void);
void sci0_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (0);
    R_BSP_IrqStatusClear (SCI0_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI0_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci0_eri_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI1_ERI
#if (BSP_IRQ_CFG_SCI1_ERI != BSP_IRQ_DISABLED)
void sci1_eri_isr (void);
void sci1_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (1);
    R_BSP_IrqStatusClear (SCI1_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI1_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci1_eri_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI2_ERI
#if (BSP_IRQ_CFG_SCI2_ERI != BSP_IRQ_DISABLED)
void sci2_eri_isr (void);
void sci2_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (2);
    R_BSP_IrqStatusClear (SCI2_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI2_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci2_eri_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI3_ERI
#if (BSP_IRQ_CFG_SCI3_ERI != BSP_IRQ_DISABLED)
void sci3_eri_isr (void);
void sci3_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (3);
    R_BSP_IrqStatusClear (SCI3_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI3_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci3_eri_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI4_ERI
#if (BSP_IRQ_CFG_SCI4_ERI != BSP_IRQ_DISABLED)
void sci4_eri_isr (void);
void sci4_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (4);
    R_BSP_IrqStatusClear (SCI4_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI4_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci4_eri_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI5_ERI
#if (BSP_IRQ_CFG_SCI5_ERI != BSP_IRQ_DISABLED)
void sci5_eri_isr (void);
void sci5_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (5);
    R_BSP_IrqStatusClear (SCI5_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI5_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci5_eri_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI6_ERI
#if (BSP_IRQ_CFG_SCI6_ERI != BSP_IRQ_DISABLED)
void sci6_eri_isr (void);
void sci6_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (6);
    R_BSP_IrqStatusClear (SCI6_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI6_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci6_eri_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI7_ERI
#if (BSP_IRQ_CFG_SCI7_ERI != BSP_IRQ_DISABLED)
void sci7_eri_isr (void);
void sci7_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (7);
    R_BSP_IrqStatusClear (SCI7_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI7_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci7_eri_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI8_ERI
#if (BSP_IRQ_CFG_SCI8_ERI != BSP_IRQ_DISABLED)
void sci8_eri_isr (void);
void sci8_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (8);
    R_BSP_IrqStatusClear (SCI8_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI8_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci8_eri_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI9_ERI
#if (BSP_IRQ_CFG_SCI9_ERI != BSP_IRQ_DISABLED)
void sci9_eri_isr (void);
void sci9_eri_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    eri_handler (9);
    R_BSP_IrqStatusClear (SCI9_ERI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI9_ERI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci9_eri_isr() */
#endif
#endif

/*******************************************************************************************************************//**
* TEI interrupt routines for SCI channels.
* @param  none
* @retval none
***********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_SCI0_TEI
#if (BSP_IRQ_CFG_SCI0_TEI != BSP_IRQ_DISABLED)
void sci0_tei_isr (void);
void sci0_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (0);
    R_BSP_IrqStatusClear (SCI0_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI0_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci0_tei_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI1_TEI
#if (BSP_IRQ_CFG_SCI1_TEI != BSP_IRQ_DISABLED)
void sci1_tei_isr (void);
void sci1_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (1);
    R_BSP_IrqStatusClear (SCI1_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI1_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci1_tei_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI2_TEI
#if (BSP_IRQ_CFG_SCI2_TEI != BSP_IRQ_DISABLED)
void sci2_tei_isr (void);
void sci2_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (2);
    R_BSP_IrqStatusClear (SCI2_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI2_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci2_tei_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI3_TEI
#if (BSP_IRQ_CFG_SCI3_TEI != BSP_IRQ_DISABLED)
void sci3_tei_isr (void);
void sci3_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (3);
    R_BSP_IrqStatusClear (SCI3_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI3_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci3_tei_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI4_TEI
#if (BSP_IRQ_CFG_SCI4_TEI != BSP_IRQ_DISABLED)
void sci4_tei_isr (void);
void sci4_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (4);
    R_BSP_IrqStatusClear (SCI4_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI4_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci4_tei_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI5_TEI
#if (BSP_IRQ_CFG_SCI5_TEI != BSP_IRQ_DISABLED)
void sci5_tei_isr (void);
void sci5_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (5);
    R_BSP_IrqStatusClear (SCI5_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI5_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci5_tei_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI6_TEI
#if (BSP_IRQ_CFG_SCI6_TEI != BSP_IRQ_DISABLED)
void sci6_tei_isr (void);
void sci6_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (6);
    R_BSP_IrqStatusClear (SCI6_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI6_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci6_tei_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI7_TEI
#if (BSP_IRQ_CFG_SCI7_TEI != BSP_IRQ_DISABLED)
void sci7_tei_isr (void);
void sci7_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (7);
    R_BSP_IrqStatusClear (SCI7_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI7_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci7_tei_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI8_TEI
#if (BSP_IRQ_CFG_SCI8_TEI != BSP_IRQ_DISABLED)
void sci8_tei_isr (void);
void sci8_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (8);
    R_BSP_IrqStatusClear (SCI8_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI8_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci8_tei_isr() */
#endif
#endif

#ifdef BSP_IRQ_CFG_SCI9_TEI
#if (BSP_IRQ_CFG_SCI9_TEI != BSP_IRQ_DISABLED)
void sci9_tei_isr (void);
void sci9_tei_isr (void)
{
    volatile uint32_t dummy;
    SF_CONTEXT_SAVE
    tei_handler (9);
    R_BSP_IrqStatusClear (SCI9_TEI_IRQn);
    dummy = R_ICU->IELSRn[(uint32_t)SCI9_TEI_IRQn];  /* Give time for value to be updated */
    SCI_NOT_USED(dummy);
    SF_CONTEXT_RESTORE
}  /* End of function sci9_tei_isr() */
#endif
#endif
/*******************************************************************************************************************//**
* This function clears the Transmit(TXI), Receive(RXI), and Error(ERI) IRQ status for the specified channel.
* @param[in] channel    Channel number of SCI module
* @retval void
***********************************************************************************************************************/
void sci_irq_status_clear (uint32_t const channel)
{
    switch (channel)
    {
#ifdef BSP_IRQ_CFG_SCI0_TXI
        case 0:
#if (BSP_IRQ_CFG_SCI0_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI0_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI0_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI0_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI0_ERI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI0_ERI_IRQn);
#endif
        break;
#endif

#ifdef BSP_IRQ_CFG_SCI1_TXI
        case 1:
#if (BSP_IRQ_CFG_SCI1_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI1_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI1_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI1_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI1_ERI != BSP_IRQ_DISABLED)
           R_BSP_IrqStatusClear (SCI1_ERI_IRQn);
#endif
        break;
#endif

#ifdef BSP_IRQ_CFG_SCI2_TXI
        case 2:
#if (BSP_IRQ_CFG_SCI2_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI2_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI2_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI2_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI2_ERI != BSP_IRQ_DISABLED)
           R_BSP_IrqStatusClear (SCI2_ERI_IRQn);
#endif
        break;
#endif

#ifdef BSP_IRQ_CFG_SCI3_TXI
        case 3:
#if (BSP_IRQ_CFG_SCI3_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI3_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI3_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI3_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI3_ERI != BSP_IRQ_DISABLED)
           R_BSP_IrqStatusClear (SCI3_ERI_IRQn);
#endif
        break;
#endif

#ifdef BSP_IRQ_CFG_SCI4_TXI
        case 4:
#if (BSP_IRQ_CFG_SCI4_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI4_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI4_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI4_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI4_ERI != BSP_IRQ_DISABLED)
           R_BSP_IrqStatusClear (SCI4_ERI_IRQn);
#endif
        break;
#endif

#ifdef BSP_IRQ_CFG_SCI5_TXI
        case 5:
#if (BSP_IRQ_CFG_SCI5_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI5_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI5_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI5_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI5_ERI != BSP_IRQ_DISABLED)
           R_BSP_IrqStatusClear (SCI5_ERI_IRQn);
#endif
        break;
#endif

#ifdef BSP_IRQ_CFG_SCI6_TXI
        case 6:
#if (BSP_IRQ_CFG_SCI6_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI6_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI6_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI6_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI6_ERI != BSP_IRQ_DISABLED)
           R_BSP_IrqStatusClear (SCI6_ERI_IRQn);
#endif
        break;
#endif

#ifdef BSP_IRQ_CFG_SCI7_TXI
        case 7:
#if (BSP_IRQ_CFG_SCI7_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI7_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI7_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI7_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI7_ERI != BSP_IRQ_DISABLED)
           R_BSP_IrqStatusClear (SCI7_ERI_IRQn);
#endif
        break;
#endif

#ifdef BSP_IRQ_CFG_SCI8_TXI
        case 8:
#if (BSP_IRQ_CFG_SCI8_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI8_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI8_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI8_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI8_ERI != BSP_IRQ_DISABLED)
           R_BSP_IrqStatusClear (SCI8_ERI_IRQn);
#endif
        break;
#endif

#ifdef BSP_IRQ_CFG_SCI9_TXI
        case 9:
#if (BSP_IRQ_CFG_SCI9_TXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI9_TXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI9_RXI != BSP_IRQ_DISABLED)
            R_BSP_IrqStatusClear (SCI9_RXI_IRQn);
#endif
#if (BSP_IRQ_CFG_SCI9_ERI != BSP_IRQ_DISABLED)
           R_BSP_IrqStatusClear (SCI9_ERI_IRQn);
#endif
        break;
#endif
        default:
        break;
    }

}

