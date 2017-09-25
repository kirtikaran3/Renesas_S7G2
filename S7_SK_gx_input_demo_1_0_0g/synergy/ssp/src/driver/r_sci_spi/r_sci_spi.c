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

/*********************************************************************************************************************
 * File Name    : r_sci_spi.c
 * Description  : This module contains API functions and HLD layer functions for SCI simple SPI module.
 *                The API functions includes an open() function to initialize and power on the SPI bus,
 *                a close() function to power down and shut off the channel, read(), Write() and Write/read()
 *                function to access and transfer data to and from the SPI bus.
 *                HDL layer functions includes functions like r_sci_sspi_baud_set() to set the baud rate,
 *                r_sci_sspi_write_read_common for write and read data from the SPI bus and r_sci_sspi_tx_rx_common()
 *                function as the transmit and reception handler and r_sci_sspi_spei_isr_common.
 ********************************************************************************************************************/




/*********************************************************************************************************************
 * Includes
 ********************************************************************************************************************/
#include "bsp_api.h"
#include "r_sci_common_cfg.h"
#include "r_sci_common.h"
#include "r_spi_api.h"
#include "r_sci_spi.h"
#include "r_sci_spi_private_api.h"
#include "../r_sci_common/r_sci_private.h"
#include "r_cgc.h"
#include "r_cgc_api.h"

#if (SCI_CFG_SSPI_INCLUDED)
/*********************************************************************************************************************
 * Macro definitions
 ********************************************************************************************************************/
/** Macro for error logger. */
#ifndef SCI_SPI_ERROR_RETURN
#define SCI_SPI_ERROR_RETURN(a, err)  SSP_ERROR_RETURN((a), (err), "sci_spi", NULL)
#endif
/* Bit setting for the SPI Mode Register */
#define SSPI_SPMR_SSN_PIN_ENABLE_SET (0x01)   // Enables the SSN pin function -- SSE bit
#define SSPI_SPMR_CTS_ENABLE_SET     (0x02)   // Enables the CTS function -- CTSE bit
#define SSPI_SPMR_SLAVE_MODE         (0x04)   // Select the slave operation mode -- MSS bit
#define SSPI_SPMR_MODE_FAULT_ENABLE  (0x10)   // Enable Mode fault error detection -- MFF bit
#define SSPI_SPMR_CKPOL_INVERTED     (0x40)   // Clock polarity is inverted -- CKPOL bit
#define SSPI_SPMR_CKPH_DELAYED       (0x80)   // Clock is delayed -- CKPH bit
/* Default SPMR setting, master mode, mode fault disabled, clock polarity inverted and clock is delayed */
#define SSPI_SPMR_DEF                (SSPI_SPMR_SSN_PIN_ENABLE | SSPI_SPMR_CKPOL_INVERTED | SSPI_SPMR_CKPH_DELAYED)

#define SSPI_NUM_DIVISORS_SYNC       (4)   // Number of synchronous divisors
#define SSPI_BRR_MAX                 (255) // Maximum Bit Rate Register (BRR)
#define SSPI_BRR_MIN                 (0)   // Minimum Bit Rate Register (BRR)

#define SSPI_BYTE_DATA               0x01  // Bit width is a byte
#define SSPI_WORD_DATA               0x02  // Bit width is 2 bytes
#define SSPI_LONG_DATA               0x04  // Bit width is 4 bytes

/* Fixed data that is transmitted during receive-only operations. Change as needed. */
#define SSPI_DUMMY_TXDATA (0xFF)

/*********************************************************************************************************************
 * Typedef definitions
 ********************************************************************************************************************/
/** SSPI on  SCI device Control block */
typedef enum e_sspi_operation
{
    SSPI_OPERATION_DO_TX    = 0x1,    /**< perform SPI transmission operation */
    SSPI_OPERATION_DO_RX    = 0x2,    /**< perform SPI reception operation */
    SSPI_OPERATION_DO_TX_RX = 0x3     /**< perform SPI Transmission and reception operation */
} sspi_operation_t;

typedef struct sspi_tcb_s
{
    void              * p_src;
    void              * p_dest;
    uint16_t          tx_count;
    uint16_t          rx_count;
    uint32_t          xfr_length;
    uint8_t           bytes_per_transfer;  /* Source buffer bytes per transfer: 1, 2, or 4. */
    bool              do_rx_now;           /* State flag for valid read data available. */
    bool              do_tx;               /* State flag for transmit operation. */
    sspi_operation_t  transfer_mode;       /* Transmit only, receive only, or transmit-receive. */
    uint8_t           rx_data;             /* temporary storage for transfer data */
    bsp_lock_t        resource_lock_tx_rx; /**< Resource lock for transmission/reception */
} sspi_tcb_t;

/*******************************************************************************************************************
 * Private global variables
 ******************************************************************************************************************/
/* Array of channel handles. One for each physical SCI channel on the device. */
static struct st_spi_ctrl g_sspi_handles[SCI_PHY_CH_MAX];

/* SSPI control block for channel */
static sspi_tcb_t  g_sspi_tcb[SCI_PHY_CH_MAX] =
{ { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };

/** SCI control block array for channel  */
extern sci_ctrl_t g_sci_ctrl_blk[SCI_PHY_CH_MAX];

const spi_api_t   g_spi_on_sci =
{
    .open      = R_SCI_SSPI_Open,
    .read      = R_SCI_SSPI_Read,
    .write     = R_SCI_SSPI_Write,
    .writeRead = R_SCI_SSPI_WriteRead,
    .close     = R_SCI_SSPI_Close,
    .versionGet= R_SCI_SSPI_VersionGet
};

/** Baud rate divisor information(SPI mode) */
static const baud_setting_t sync_baud[SSPI_NUM_DIVISORS_SYNC] =
{
    {   4, 1, 1, 1, 0 }, /* divisor, BGDM, ABCS, ABCSE, n */
    {  16, 1, 1, 1, 1 },
    {  64, 1, 1, 1, 2 },
    { 256, 1, 1, 1, 3 }
};

/*********************************************************************************************************************
 * Private function declarations
 ********************************************************************************************************************/
/* Common routine used by SSPI API write or read functions. */
static ssp_err_t r_sci_sspi_write_read_common (spi_ctrl_t        * const p_ctrl,
                                               void const                * p_src,
                                               void const                * p_dest,
                                               uint32_t const            length,
                                               spi_bit_width_t const     bit_width,
                                               sspi_operation_t          tx_rx_mode);

/* Sets the SCI bit rate registers (BRR) for a given frequency. */
static ssp_err_t r_sci_sspi_baud_set (uint32_t const   channel,
                                      uint32_t         bitrate,
                                      sci_mode_t const mode);

/*******************************************************************************************************************
 * Functions
 ******************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup SSPI
 * @{
 **********************************************************************************************************************/

/***************************************************************************************************************//**
 * @brief   Initialize a channel for SPI communication mode.
 * Implements spi_api_t::open
 *          This function performs the following tasks:
 *          Performs parameter checking and processes error conditions.
 *          Applies power to the SPI channel.
 *          Disables interrupts.
 *          Initializes the associated registers with default value and the user-configurable options.
 *          Provides the channel handle for use with other API functions.
 *          Updates user-configurable file if necessary.
 * @retval     SSP_SUCCESS                 Channel initialized successfully.
 * @retval     SSP_ERR_INVALID_ARGUMENT    Channel number invalid.
 * @retval     SSP_ERR_IN_USE              Channel currently in operation; Close channel first.
 * @retval     SSP_ERR_INVALID_POINTER     The p_cfg pointer or p_ctrl pointer is NULL.
 * @retval     SSP_ERR_INVALID_ARGUMENT    An element of the r_spi_cfg_t structure contains an invalid value.
 * @retval     SSP_ERR_HW_LOCKED           The lock could not be acquired. The channel is busy.
 * @note  This function is reentrant.
 * @note  The bit-rate argument in p_cfg ranges from 2500 to 7.5m for Simple SPI at PCLK=120 MHz. For RSPI, BRDV is
 *        fixed at 0 to get the maximum bit rate. The range is 10.0 mbps to 30.0 mbps at PCLK=120.0 MHz
 *****************************************************************************************************************/
ssp_err_t R_SCI_SSPI_Open (spi_ctrl_t                       * p_ctrl,
                           spi_cfg_t                  const * const p_cfg)
{
    uint32_t         channel      = 0;
    uint8_t          temp;
    ssp_err_t        result = SSP_SUCCESS;

#if SCI_SPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_ctrl);

    /* Check channel number. */
    result = r_sci_channel_param_check(p_cfg->channel);
    SCI_SPI_ERROR_RETURN((SSP_SUCCESS == result), result);
#endif /* if SCI_SPI_CFG_PARAM_CHECKING_ENABLE */

    channel = p_cfg->channel;

    /* Check mode is not off, someone is using it, exist */
    if (SCI_MODE_OFF != g_sci_ctrl_blk[channel].mode)
    {
        return SSP_ERR_IN_USE;
    }

    /* Check to see if the peripheral has already been initialized. */
    if (true == g_sspi_handles[channel].channel_opened)
    {
        /* This channel has already been initialized. */
        return SSP_ERR_IN_USE;
    }

    /* Attempt to acquire lock for this SSPI channel. Prevents reentrancy conflict. */
    if (SSP_SUCCESS != r_sci_hardware_lock(channel))
    {
        return SSP_ERR_HW_LOCKED;
    }

    /* Turn on power */
    HW_SCI_PowerOn(channel);           /** Applies power to channel */

    /* Disable interrupts in ICU. */
    HW_SCI_InterruptEnable(channel, SCI_ALL_INT, false);   /** Disables interrupt */

    /* Don't use FIFO mode -- set FCRL_b.FM = 0*/
    HW_SCI_FifoDisable(channel);

    /* Select SPI mode - set SCMR.SMIF=0, SIMR1.IICM=0, SMR.CM=1, SPMR.SSE=1 */
    HW_SCI_SPIModeSelect(channel);

    /* Set baud rate in SCI channel for the SPI channel */
    if (SSP_SUCCESS != r_sci_sspi_baud_set(channel, p_cfg->bitrate, SCI_MODE_SSPI))
    {
        /* If setting failed, unlock channel */
        r_sci_hardware_unlock(channel);
        SSP_ASSERT(false);  /* Could not calculate settings for the requested baud rate. */
    }

    /* Sets the CKE bit in the SCR register */
    HW_SCI_BaudRateGenInternalClkSelect(channel);

    /* Process the user configuration to update the local configuration image */
    /* Program the registers based on the user configuration file */
    /* Update SPI Mode register -- SPMR selection */
    temp = 0;

    /* Set MSS -- master/slave mode */
    if (SPI_MODE_SLAVE == p_cfg->operating_mode)
    {
        temp |= SSPI_SPMR_SLAVE_MODE;
    }

    /* Set MFF -- Mode fault error detection */
    if (SPI_MODE_FAULT_ERROR_ENABLE == p_cfg->mode_fault)
    {
        temp |= SSPI_SPMR_MODE_FAULT_ENABLE;
    }

    /* Set CKPOL - clock polarity */
    if (SPI_CLK_POLARITY_HIGH == p_cfg->clk_polarity)
    {
        temp |= SSPI_SPMR_CKPOL_INVERTED;
    }

    /* Set CKPH - clock phase ODD-no clock delay, EVEN--clock delay*/
    if (SPI_CLK_PHASE_EDGE_EVEN == p_cfg->clk_phase)
    {
        temp |= SSPI_SPMR_CKPH_DELAYED;
    }

    /* Set the hardware LLD with the temp value */
    HW_SCI_SPIModeSet(channel, temp);

    /* Set MSB/LSB based on the user configuration file */
    if (SPI_BIT_ORDER_MSB_FIRST == p_cfg->bit_order)
    {
        HW_SCI_TransferDirection(channel, true);
    }

    /* Peripheral Initialized */
    /* Set control block for SCI channel to SPI mode operation */
    g_sci_ctrl_blk[channel].mode           = SCI_MODE_SSPI;
    g_sspi_handles[channel].channel_opened = true;
    g_sspi_handles[channel].p_callback     = p_cfg->p_callback;
    g_sspi_handles[channel].p_context      = p_cfg->p_context;
    g_sspi_handles[channel].channel        = (uint8_t)channel;

    /* Update info to the handle */
    p_ctrl->channel        = (uint8_t)channel;
    p_ctrl->channel_opened = g_sspi_handles[channel].channel_opened;

    return result;
}

/* End of function R_SCI_SSPI_Open(). */

/*************************************************************************************************************//**
 * @brief   Receive data from an SPI device.
 * Implements spi_api_t::read
 *          The function performs the following tasks:
 *          - Performs parameter checking and processes error conditions.
 *          - Disable Interrupts.
 *          - Set-up data bit width per user request.
 *          - Enable transmitter.
 *          - Enable receiver.
 *          - Enable interrupts.
 *          - Start data transmission with dummy data via transmit buffer empty interrupt.
 *          - Copy data from source buffer to the SPI data register for transmission.
 *          - Receive data from receive buffer full interrupt occurs and copy data to the buffer of destination.
 *          - Complete data reception via receive buffer full interrupt and transmitting dummy data.
 *          - Disable transmitter.
 *          - Disable receiver.
 *          - Disable interrupts.
 * @retval     SSP_SUCCESS               Read operation successfully completed.
 * @retval     SSP_ERR_INVALID_ARGUMENT  Channel number invalid.
 * @retval     SSP_ERR_INVALID_POINTER   A required pointer argument is NULL.
 * @retval     SSP_ERR_HW_LOCKED         The lock could not be acquired. The channel is busy.
 * @retval     SSP_ERR_CH_NOT_OPEN       The channel has not been opened. Open channel first.
 * @note  This function is reentrant.
 ***************************************************************************************************************/
ssp_err_t  R_SCI_SSPI_Read (spi_ctrl_t          * const p_ctrl,
                            void const                  * p_dest,
                            uint32_t const              length,
                            spi_bit_width_t const       bit_width)
{
    ssp_err_t result;

#if SCI_SPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_ctrl);

    /* Check bit_width parameter, in simple SPI, only 8 bits operation is allowed */
    SSP_ASSERT(SPI_BIT_WIDTH_8_BITS == bit_width);

    /* Check the data length, should not be 0 */
    SSP_ASSERT(0 != length);

    /* Check channel number. */
    if (SCI_PHY_CH_MAX < p_ctrl->channel)
    {
        /* Invalid channel. */
        return SSP_ERR_INVALID_CHANNEL;
    }
#endif /* if SCI_SPI_CFG_PARAM_CHECKING_ENABLE */

    result = r_sci_sspi_write_read_common(p_ctrl, NULL, p_dest, length, bit_width, SSPI_OPERATION_DO_RX);

    return result;
}

/* End of function R_SCI_SSPI_Read(). */

/*************************************************************************************************************//**
 * @brief   Transmit data to a SPI  device.
 * Implements spi_api_t::write
 *          - The function performs the following tasks:
 *          - Performs parameter checking and processes error conditions.
 *          - Disable Interrupts.
 *          - Setup data bit width per user request.
 *          - Enable transmitter.
 *          - Enable receiver.
 *          - Enable interrupts.
 *          - Start data transmission with data via transmit buffer empty interrupt.
 *          - Copy data from source buffer to the SPI data register for transmission.
 *          - Receive data from receive buffer full interrupt occurs and do nothing with the received data.
 *          - Complete data transmission via receive buffer full interrupt.
 *          - Disable transmitter.
 *          - Disable receiver.
 *          - Disable interrupts.
 * @retval     SSP_SUCCESS               Write operation successfully completed.
 * @retval     SSP_ERR_INVALID_ARGUMENT  Channel number invalid.
 * @retval     SSP_ERR_INVALID_POINTER   A required pointer argument is NULL.
 * @retval     SSP_ERR_HW_LOCKED 2       The lock could not be acquired. The channel is busy.
 * @retval     SSP_ERR_CH_NOT_OPEN       The channel has not been opened. Open the channel first
 * @note  This function is reentrant.
 ***************************************************************************************************************/
ssp_err_t   R_SCI_SSPI_Write (spi_ctrl_t          * const p_ctrl,
                              void const                  * p_src,
                              uint32_t const              length,
                              spi_bit_width_t const       bit_width)
{
    ssp_err_t result;

#if SCI_SPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_ctrl);

    /* Check bit_width parameter, in simple SPI, only 8 bits operation is allowed */
    SSP_ASSERT(SPI_BIT_WIDTH_8_BITS == bit_width);

    /* Check the data length, should not be 0 */
    SSP_ASSERT(0 != length);

    /* Check channel number. */
    if (SCI_PHY_CH_MAX < p_ctrl->channel)
    {
        /* Invalid channel. */
        return SSP_ERR_INVALID_CHANNEL;
    }
#endif /* if SCI_SPI_CFG_PARAM_CHECKING_ENABLE */

    result = r_sci_sspi_write_read_common(p_ctrl, p_src, NULL, length, bit_width, SSPI_OPERATION_DO_TX);

    return result;
}

/* End of function R_SCI_SSPI_Write(). */

/*************************************************************************************************************//**
 * @brief   Simultaneously transmit data to SPI device while receiving data from SPI device
 *          (full duplex).
 * Implements spi_api_t::writeRead
 *          The function performs the following tasks:
 *          - Performs parameter checking and processes error conditions.
 *          - Disable Interrupts.
 *          Setup data bit width per user request.
 *          Enable transmitter.
 *          Enable receiver.
 *          Enable interrupts.
 *          Start data transmission using transmit buffer empty interrupt.
 *          Copy data from source buffer to the SPI data register for transmission.
 *          Receive data from receive buffer full interrupt occurs and copy data to the buffer of destination.
 *          Complete data transmission and reception via receive buffer full interrupt.
 *          Disable transmitter.
 *          Disable receiver.
 *          Disable interrupts.
 * @retval     SSP_SUCCESS               Write operation successfully completed.
 * @retval     SSP_ERR_INVALID_ARGUMENT  Channel number invalid.
 * @retval     SSP_ERR_INVALID_POINTER   A required pointer argument is NULL.
 * @retval     SSP_ERR_HW_LOCKED         The lock could not be acquired. The channel is busy.
 * @retval     SSP_ERR_CH_NOT_OPEN       The channel has not been opened. Open the channel first.
 * @note  This function is reentrant.
 ***************************************************************************************************************/
ssp_err_t  R_SCI_SSPI_WriteRead (spi_ctrl_t     * const p_ctrl,
                                 void const             * p_src,
                                 void const             * p_dest,
                                 uint32_t const         length,
                                 spi_bit_width_t const  bit_width)
{
    ssp_err_t result;

#if SCI_SPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_ctrl);

    /* Check bit_width parameter, in simple SPI, only 8 bits operation is allowed */
    SSP_ASSERT(SPI_BIT_WIDTH_8_BITS == bit_width);

    /* Check the data length, should not be 0 */
    SSP_ASSERT(0 != length);

    /* Check channel number. */
    if (SCI_PHY_CH_MAX < p_ctrl->channel)
    {
        /* Invalid channel. */
        return SSP_ERR_INVALID_CHANNEL;
    }
#endif /* if SCI_SPI_CFG_PARAM_CHECKING_ENABLE */

    result = r_sci_sspi_write_read_common(p_ctrl, p_src, p_dest, length, bit_width, SSPI_OPERATION_DO_TX_RX);

    return result;
}

/* End of function R_SCI_SSPI_WriteRead(). */

/***************************************************************************************************************
 * @brief   Initiates writ or read process. Common routine used by RSPI API write or read functions.
 * @retval     SSP_SUCCESS                 Operation successfully completed.
 * @retval     SSP_ERR_HW_LOCKED           The lock could not be acquired. The channel is busy.
 * @retval     SSP_ERR_CH_NOT_OPEN         The channel has not been opened. Perform R_SCI_SSPI_Open() first.
 * @retval     SSP_ERR_INVALID_ARGUMENT    An element of the p_cfg structure contains an invalid value.
 * @note  This function is reentrant.
 ***************************************************************************************************************/
static ssp_err_t  r_sci_sspi_write_read_common (spi_ctrl_t        * const p_ctrl,
                                                void const                * p_src,
                                                void const                * p_dest,
                                                uint32_t const            length,
                                                spi_bit_width_t const     bit_width,
                                                sspi_operation_t          tx_rx_mode)
{
    uint8_t   channel = p_ctrl->channel;
    ssp_err_t result;

    if (!g_sspi_handles[channel].channel_opened)
    {
        return SSP_ERR_NOT_OPEN;
    }

    /* Attempt to acquire lock for this transfer operation. Prevents re-entrance conflict. */
    if (SSP_SUCCESS != R_BSP_SoftwareLock(&g_sspi_tcb[channel].resource_lock_tx_rx))
    {
        return SSP_ERR_HW_LOCKED;
    }

    /* Disable interrupts in ICU. */
    HW_SCI_InterruptEnable(channel, SCI_ALL_INT, false);   /**< disables interrupts */

    /* setup the control block */
    g_sspi_tcb[channel].xfr_length         = length;
    g_sspi_tcb[channel].tx_count           = 0;
    g_sspi_tcb[channel].rx_count           = 0;
    g_sspi_tcb[channel].bytes_per_transfer = bit_width;
    g_sspi_tcb[channel].p_src              = (void *) p_src;
    g_sspi_tcb[channel].p_dest             = (void *) p_dest;
    g_sspi_tcb[channel].transfer_mode      = tx_rx_mode;

    result                                 = SSP_SUCCESS;

    if (tx_rx_mode & SSPI_OPERATION_DO_TX)
    {
        g_sspi_tcb[channel].do_tx = true;
    }
    else
    {
        g_sspi_tcb[channel].do_tx = false;
    }

    g_sspi_tcb[channel].do_rx_now = false;  // Initialize receive state flag.

    /* Clear error sources: mode fault error bit should be cleared in the SPMR register */
    HW_SCI_SPIModeFaultClear(channel);

    /* clear interrupt status for Transmit(TXI), Receive(RXI), Error(ERI)*/
    sci_irq_status_clear(channel);

    /* Enable interrupts in ICU. */
    HW_SCI_InterruptEnable(channel, SCI_RX_INT, true);   /**< enables interrupts */
    HW_SCI_InterruptEnable(channel, SCI_TX_INT, true);   /**< enables interrupts */

    /** enables transmitter and receiver at the same time */
    HW_SCI_TransmitterReceiverEnable(channel);

    return result;
}

/* End of function r_sci_sspi_write_read_common(). */

/*************************************************************************************************************//**
 * @brief   Handle the closing of a channel by the following task.
 * Implements spi_api_t::close
 *          Power off the channel.
 *          Disables all the associated interrupts.
 *          Update channel status.
 * @retval     SSP_SUCCESS              Channel successfully closed.
 * @retval     SSP_ERR_INVALID_POINTER  A required pointer argument is NULL.
 * @retval     SSP_ERR_CH_NOT_OPEN      The channel has not been opened. Open the channel first.
 * @note  This function is reentrant.
 ***************************************************************************************************************/
ssp_err_t  R_SCI_SSPI_Close (spi_ctrl_t * const p_ctrl)
{
    uint8_t channel;

#if SCI_SPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_ctrl);

    /* Check channel number. */
    if (SCI_PHY_CH_MAX < p_ctrl->channel)
    {
        /* Invalid channel. */
        return SSP_ERR_INVALID_CHANNEL;
    }
#endif /* if SCI_SPI_CFG_PARAM_CHECKING_ENABLE */

    channel = p_ctrl->channel;

    /* Check to see if the channel is currently initialized. */
    if (false == g_sspi_handles[channel].channel_opened)
    {
        /* This channel is not open so need not be closed. */
        return SSP_ERR_NOT_OPEN;
    }

    /* Disable interrupts in SCI and ICU. */
    HW_SCI_InterruptEnable(channel, SCI_ALL_INT, false);   /**< disables interrupts */

    g_sspi_handles[channel].channel_opened = false;

    /** Clear SCI device context */
    g_sci_ctrl_blk[channel].mode        = SCI_MODE_OFF;
    g_sci_ctrl_blk[channel].tx_busy     = false;
    g_sci_ctrl_blk[channel].p_context   = NULL;
    g_sci_ctrl_blk[channel].p_extpin_ctrl = NULL;

    /* Turn off power */
    HW_SCI_PowerOff(channel);           /**< remove power to channel */

    /* Release the software lock. */
    R_BSP_SoftwareUnlock(&g_sspi_tcb[channel].resource_lock_tx_rx);

    /* Release lock for this channel. */
    r_sci_hardware_unlock(channel);

    return SSP_SUCCESS;
}

/* End of function R_SCI_SSPI_Close(). */

/*****************************************************************************************************************//**
 * @brief   Get the version information of the underlying driver.
 * Implements spi_api_t::versionGet
 * @retval   SSP_SUCCESS  Channel successfully closed.
 * @note     This function is reentrant.
 ********************************************************************************************************************/
ssp_err_t R_SCI_SSPI_VersionGet (ssp_version_t * p_version)
{
    p_version->code_version_major = SCI_SSPI_CODE_VERSION_MAJOR;
    p_version->code_version_minor = SCI_SSPI_CODE_VERSION_MINOR;
    p_version->api_version_major  = SPI_API_VERSION_MAJOR;
    p_version->api_version_minor  = SPI_API_VERSION_MINOR;

    return SSP_SUCCESS;
}

/* End of function R_SCI_SSPI_VersionGet() */

/*******************************************************************************************************************//**
 * @} (end addtogroup SSPI)
 **********************************************************************************************************************/

/***************************************************************************************************************
 * @brief   This function is the common ISR handler for SPTI and SPRI interrupts.
 *
 * @param[in]  channel  Channel number to use.
 * @retval  void
 ***************************************************************************************************************/
void r_sci_sspi_tx_rx_common (uint32_t channel, sci_int_t which_irq)
{
    void          * p_src   = g_sspi_tcb[channel].p_src;
    void          * p_dest  = g_sspi_tcb[channel].p_dest;
    uint8_t       data_size = g_sspi_tcb[channel].bytes_per_transfer;
    uint16_t      tx_count;
    uint16_t      rx_count;
    uint32_t      rx_data;
    spi_callback_args_t sspi_cb_data;

    if (SCI_TX_INT == which_irq)
    {
        g_sspi_tcb[channel].rx_data = HW_SCI_Read(channel);     /* Read rx-data register into temp buffer. */

        /* If master mode then disable further TX interrupts on first transmit.
         * If slave mode then we do two transmits to fill the double buffer, then disable TX interrupts.
         * The receive interrupt will handle any remaining data. */
        if ((HW_SCI_SPIMasterCheck(channel)) || (g_sspi_tcb[channel].tx_count > 0))
        {
            /* Disable Tx interrupts. */
            HW_SCI_InterruptEnable(channel, SCI_TX_INT, false);   /**< Disables interrupts */
        }
    }
    else
    {
        /* In case of data receiving */
        g_sspi_tcb[channel].rx_data = HW_SCI_Read(channel); /* Read rx-data register into temporary buffer. */
        g_sspi_tcb[channel].rx_count++;
    }

    tx_count = g_sspi_tcb[channel].tx_count;
    rx_count = g_sspi_tcb[channel].rx_count;
    rx_data  = g_sspi_tcb[channel].rx_data;

    /* Service the hardware first to keep it busy. */
    /* Feed the TX. */
    if (tx_count < g_sspi_tcb[channel].xfr_length)   /* Don't write transmit buffer more than length. */
    {
        if (g_sspi_tcb[channel].do_tx)
        {
            /* Transmit the data. TX data register accessed in byte. */
            if (SSPI_BYTE_DATA == data_size)
            {
                HW_SCI_Write(channel, ((uint8_t *) p_src)[tx_count]);
            }
        }
        else /* Must be RX only mode, so transmit dummy data for clocking.*/
        {
            /* TX data register accessed in bytes. */
            HW_SCI_Write(channel, SSPI_DUMMY_TXDATA);
        }

        g_sspi_tcb[channel].tx_count++;
    }

    /* Store the received data in user buffer.
     * Receive data not valid until after first transmission is complete. */
    if (g_sspi_tcb[channel].do_rx_now)
    {
        if (SSPI_BYTE_DATA == data_size)
        {
            ((uint8_t *) p_dest)[rx_count - 1] = (uint8_t)rx_data;
        }
    }

    /* Check for last data.  */
    if (rx_count == g_sspi_tcb[channel].xfr_length)
    {
        /* Last data was transferred. Disable interrupts and disable receiver */
        /** Disables receiver and transmitter*/
        HW_SCI_TransmitterReceiverDisable(channel);

        /* Disable interrupts in SCI and ICU. */
        HW_SCI_InterruptEnable(channel, SCI_ALL_INT, false);   /**< disables interrupts */

        /* Transfer complete. Call the user callback function passing pointer to the result structure. */
        if ((NULL != g_sspi_handles[channel].p_callback))
        {
            sspi_cb_data.channel = channel;
            sspi_cb_data.event   = SPI_EVENT_TRANSFER_COMPLETE;
            sspi_cb_data.p_context = g_sspi_handles[channel].p_context;
            g_sspi_handles[channel].p_callback((spi_callback_args_t *) &(sspi_cb_data));

            /* Disable interrupts in ICU. */
            HW_SCI_InterruptEnable(channel, SCI_RX_INT, false);   /**< disables interrupts */
            HW_SCI_InterruptEnable(channel, SCI_TX_INT, false);   /**< disables interrupts */

            /** Disables transmitter and receiver at the same time */
            HW_SCI_TransmitterReceiverDisable(channel);
        }

        /* Transfer is done, release the lock for this operation. */
        R_BSP_SoftwareUnlock(&g_sspi_tcb[channel].resource_lock_tx_rx);
    }

    if (SCI_TX_INT == which_irq)
    {
        if (g_sspi_tcb[channel].transfer_mode & SSPI_OPERATION_DO_RX)
        {
            /* Count was incremented in the call to rspi_tx_rx_common. */
            if ((HW_SCI_SPIMasterCheck(channel)) || (g_sspi_tcb[channel].tx_count > 1))
            {
                g_sspi_tcb[channel].do_rx_now = true; /* Enables saving of receive data on next receive interrupt. */
            }
        }
    }
} /* End r_sci_sspi_tx_rx_common() */

/***************************************************************************************************************
 * @brief   This function is common ISR handler for  RSPI SPEI-error interrupts.
 *
 * @param[in]  channel  Channel number to use.
 * @retval  void
 ***************************************************************************************************************/
void r_sci_sspi_spei_isr_common (uint32_t channel)
{
    spi_callback_args_t sspi_cb_data;

    /* Get the error status and return back through the caller function */
    if (NULL != g_sspi_handles[channel].p_callback)
    {
        sspi_cb_data.channel = channel;

        if (HW_SCI_OverRunErrorCheck(channel))
        {
            sspi_cb_data.event = SPI_EVENT_ERR_OVERRUN;
        }
        else if (HW_SCI_ParityErrorCheck(channel))
        {
            sspi_cb_data.event = SPI_EVENT_ERR_PARITY;
        }
        else if (HW_SCI_SPIModeFaultCheck(channel))
        {
            sspi_cb_data.event = SPI_EVENT_ERR_MODE_FAULT;
        }
        else if (HW_SCI_FramingErrorCheck(channel))
        {
            sspi_cb_data.event = SPI_EVENT_ERR_FRAMING;
        }

        g_sspi_handles[channel].p_callback((spi_callback_args_t *) &(sspi_cb_data));
    }

    /* Error condition occurs, release the software lock for this operation. */
    R_BSP_SoftwareUnlock(&g_sspi_tcb[channel].resource_lock_tx_rx);

    /* Release the hardware lock for this channel. */
    r_sci_hardware_unlock(channel);
} /* End r_sci_sspi_spei_isr_common() */

/*********************************************************************************************************************
 * @brief   This function changes baud rate. It evaluates and determines the best possible settings for the baud rate
 * registers.
 * @param[in] channel      Channel number of SCI module
 * @param[in] clk_src      Clock source for SCI module (SCI_CLK_INT|SCI_CLK_EXT|SCI_CLK_EXT8X|SCI_CLK_EXT16X)
 * @param[in] bitrate      bitrate[bps] e.g. 250,000; 500,00; 2,500,000(max), etc.
 * @param[in] mode         SCI operational modes (SCI_MODE_ASYNC|SCI_MODE_SSPI|SCI_MODE_SI2C)
 * @retval  SSP_SUCCESS                  Baud rate is set successfully
 * @retval  SSP_ERR_INVALID_ARGUMENT     Baud rate is '0' or cannot set properly
 * @note    The application must pause for 1 bit time after the BRR register is loaded
 *          before transmitting/receiving to allow time for the clock to settle.
 ********************************************************************************************************************/
ssp_err_t r_sci_sspi_baud_set (uint32_t const   channel,
                               uint32_t         bitrate,
                               sci_mode_t const mode)
{
    uint32_t         i             = 0;
    uint32_t         brr           = SSPI_BRR_MAX;
    uint32_t         temp_brr      = SSPI_BRR_MIN;
    baud_setting_t   * p_baudinfo;
    volatile uint8_t clock_divisor = 0;
    ssp_err_t        result;
    uint32_t         clock_mhz;

    result = g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKA, &clock_mhz);
    if (SSP_SUCCESS != result)
    {
        return SSP_ERR_ASSERTION;
    }

    if (0 == bitrate)
    {
        return SSP_ERR_ASSERTION;
    }

    if (SCI_MODE_SSPI != mode)
    {
        return SSP_ERR_ASSERTION;
    }

    if (SCI_PHY_CH_MAX  < channel)
    {
        return SSP_ERR_ASSERTION;
    }

    p_baudinfo = (baud_setting_t *) sync_baud;

    /* FIND BEST_BRR_VALUE
     *  In table sync_baud", divisor value is associated with
     *  the divisor coefficient and the n values (clock divisor) , so once best divisor value is found,
     *  baud rate related register setting values are decided.
     *  The formula to calculate BRR is as follows and it must be 255 or less.
     *  BRR = (PCLKA/(div_coefficient * baud)) - 1
     */
    result = SSP_ERR_ASSERTION;
    for (i = 0; i < SSPI_NUM_DIVISORS_SYNC; i++)
    {
        temp_brr = clock_mhz / ((uint32_t)p_baudinfo[i].div_coefficient * bitrate);
        if ((SSPI_BRR_MIN < temp_brr) && (temp_brr < (SSPI_BRR_MAX + 1)))
        {
            brr           = temp_brr - 1;
            clock_divisor = (uint8_t)i;
            /* Now set the bit rate register in the hardware */
            HW_SCI_BitRateBRRSet(channel, (uint8_t)brr, clock_divisor);
            result = SSP_SUCCESS;
            break;
        }
    }

    return result;
}
/* End of function r_sci_sspi_baud_set() */

#endif /* SCI_CFG_SSPI_INCLUDED */


/* End of file S_SCI_SSPI.c*/
