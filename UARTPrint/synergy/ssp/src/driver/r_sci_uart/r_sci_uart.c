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
 * File Name    : r_sci_uart.c
 * Description  : UART on SCI HAL driver
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_sci_common_cfg.h"
#include "r_sci_uart.h"
#include "r_sci_uart_private_api.h"
#include "r_cgc.h"
#include "../r_sci_common/r_sci_private.h"

#if (SCI_CFG_ASYNC_INCLUDED)

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#ifndef SCI_UART_ERROR_RETURN
#define SCI_UART_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), "sci_uart", &module_version)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
static ssp_err_t r_sci_uart_open_param_check  (uart_ctrl_t const * const p_ctrl, uart_cfg_t const * const p_cfg);

static ssp_err_t r_sci_read_write_param_check (uart_ctrl_t const * const p_ctrl,
                                               uint8_t const * const     addr,
                                               transfer_instance_t const * const p_transfer,
                                               uint32_t const            bytes);
#endif /* #if (SCI_UART_CFG_PARAM_CHECKING_ENABLE) */

static ssp_err_t    r_sci_uart_config_set        (uart_cfg_t const * const p_cfg);

static ssp_err_t    r_sci_uart_transfer_open     (uart_cfg_t const * const p_cfg);

static ssp_err_t    r_sci_uart_baud_set          (uint32_t const channel, sci_clk_src_t clk_src, uint32_t baudrate);

static void         r_sci_uart_fifo_reset  (uart_cfg_t const * const p_cfg);

static void         r_sci_uart_fifo_enable (uart_cfg_t const * const p_cfg);

#if (SCI_UART_CFG_EXTERNAL_RTS_OPERATION)
static void         r_sci_uart_external_rts_operation_enable (uart_cfg_t const * const p_cfg);
#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** SCI control block array for channel */
extern sci_ctrl_t           g_sci_ctrl_blk[SCI_PHY_CH_MAX];

/** Baud rate divisor information(UART mode) */
static const baud_setting_t async_baud[NUM_DIVISORS_ASYNC] =
{
    {   6,  0,  0,  1,  0 }, /* divisor, BGDM, ABCS, ABCSE, n */
    {   8,  1,  1,  0,  0 },
    {  16,  0,  1,  0,  0 },
    {  24,  0,  0,  1,  1 },
    {  32,  0,  0,  0,  0 },
    {  64,  0,  1,  0,  1 },
    {  96,  0,  0,  1,  2 },
    { 128,  0,  0,  0,  1 },
    { 256,  0,  1,  0,  2 },
    { 384,  0,  0,  1,  3 },
    { 512,  0,  0,  0,  2 },
    { 1024, 0,  1,  0,  3 },
    { 2048, 0,  0,  0,  3 }
};

/** SCI UART HAL module version data structure */
static const ssp_version_t module_version =
{
    .api_version_minor  = UART_API_VERSION_MINOR,
    .api_version_major  = UART_API_VERSION_MAJOR,
    .code_version_major = SCI_UART_CODE_VERSION_MAJOR,
    .code_version_minor = SCI_UART_CODE_VERSION_MINOR
};

/** UART on SCI HAL API mapping for UART interface */
const uart_api_t  g_uart_on_sci =
{
    .open       = R_SCI_UartOpen,
    .close      = R_SCI_UartClose,
#if (SCI_UART_CFG_TX_ENABLE)
    .write      = R_SCI_UartWrite,
#else
    .write      = NULL,
#endif
#if (SCI_UART_CFG_RX_ENABLE)
    .read       = R_SCI_UartRead,
#else
    .read       = NULL,
#endif
    .infoGet    = R_SCI_UartInfoGet,
    .baudSet    = R_SCI_UartBaudSet,
    .versionGet = R_SCI_UartVersionGet
};

/*******************************************************************************************************************//**
 * @addtogroup UARTonSCI
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS                  Channel opened successfully.
 * @retval  SSP_ERR_IN_USE               Channel already in use.
 * @retval  SSP_ERR_ASSERTION            Pointer to UART control block or configuration structure is NULL.
 * @retval  SSP_ERR_HW_LOCKED            Channel is locked.
 * @retval  SSP_ERR_INVALID_MODE         Channel is used for non-UART mode or illegal mode is set.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Invalid parameter setting found in the configuration structure.
 * @note This function is reentrant.
 **********************************************************************************************************************/
ssp_err_t R_SCI_UartOpen (uart_ctrl_t * const p_ctrl, uart_cfg_t const * const p_cfg)
{
    ssp_err_t         err = SSP_SUCCESS;
    uart_on_sci_cfg_t * pextend;

#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
    /** Check parameters. */
    err = r_sci_uart_open_param_check(p_ctrl, p_cfg);        /** check arguments */
    SCI_UART_ERROR_RETURN((SSP_SUCCESS == err), err);
#endif

    pextend = (uart_on_sci_cfg_t *) p_cfg->p_extend;

    /** lock specified SCI channel */
    SCI_UART_ERROR_RETURN((SSP_SUCCESS == r_sci_hardware_lock(p_cfg->channel)), SSP_ERR_HW_LOCKED);

    p_ctrl->p_transfer_rx = p_cfg->p_transfer_rx;
    p_ctrl->p_transfer_tx = p_cfg->p_transfer_tx;

    err = r_sci_uart_transfer_open(p_cfg);
    SCI_UART_ERROR_RETURN(SSP_SUCCESS == err, err);

    HW_SCI_PowerOn(p_cfg->channel);                             /** applies power to channel */
    HW_SCI_InterruptEnable(p_cfg->channel, SCI_ALL_INT, false); /** disables interrupt */
    HW_SCI_ReceiverDisable(p_cfg->channel);                     /** disables receiver */
    HW_SCI_TransmitterDisable(p_cfg->channel);                  /** enables transmitter */

    r_sci_uart_fifo_reset(p_cfg);                          /** configure FIFO related registers */

    if ((pextend) && (pextend->rx_edge_start))
    {
        HW_SCI_StartBitFallingEdgeSet(p_cfg->channel);     /** starts reception when RXD has falling edge */
    }
    else
    {
        HW_SCI_StartBitLowLevelSet(p_cfg->channel);        /** starts reception when RXD becomes low level */
    }

    if ((pextend) && (pextend->noisecancel_en))
    {
        HW_SCI_NoiseFilterSet(p_cfg->channel, NOISE_CANCEL_LVL1);          /** enables the noise cancellation, the effect level should
                                                            *   be fixed to the minimum */
    }
    else
    {
        HW_SCI_NoiseFilterClear(p_cfg->channel);           /** disables the noise cancellation */
    }

    err = r_sci_uart_config_set(p_cfg);                    /** configure UART related registers */
    if (err != SSP_SUCCESS)
    {
#if (SCI_UART_CFG_RX_ENABLE)
        if (NULL != p_ctrl->p_transfer_rx)
        {
            p_ctrl->p_transfer_rx->p_api->close(p_ctrl->p_transfer_rx->p_ctrl);
        }
#endif
#if (SCI_UART_CFG_TX_ENABLE)
        if (NULL != p_ctrl->p_transfer_tx)
        {
            p_ctrl->p_transfer_tx->p_api->close(p_ctrl->p_transfer_tx->p_ctrl);
        }
#endif
        HW_SCI_StartBitLowLevelSet(p_cfg->channel);        /** set default setting */
        HW_SCI_PowerOff(p_cfg->channel);                   /** removes power to channel */
        r_sci_hardware_unlock(p_cfg->channel);
        return err;
    }

    r_sci_uart_fifo_enable(p_cfg);                         /** configure FIFO related registers */

#if (SCI_UART_CFG_RX_ENABLE)
    HW_SCI_RXIeventSelect(p_cfg->channel);             /** selects RXI when detecting a reception data ready */
#endif

    p_ctrl->channel                          = p_cfg->channel;
    p_ctrl->p_context                        = p_cfg->p_context;  /** saves AMS UART device context inside SCI HAL
                                                                   * driver */
    p_ctrl->p_callback                       = p_cfg->p_callback; /** registers callback function from higher layer */
    p_ctrl->p_tx_src                         = NULL;
    p_ctrl->tx_src_bytes                     = 0;

    g_sci_ctrl_blk[p_cfg->channel].mode      = SCI_MODE_ASYNC;
    g_sci_ctrl_blk[p_cfg->channel].tx_busy   = false;
    g_sci_ctrl_blk[p_cfg->channel].p_context = p_ctrl;

#if (SCI_UART_CFG_RX_ENABLE)
    HW_SCI_ReceiverEnable(p_cfg->channel);                      /** enables receiver */
    HW_SCI_InterruptEnable(p_cfg->channel, SCI_RX_INT, true);   /** enables receive interrupt */
#endif

    /** Transmitter and its interrupt are enabled in R_SCI_UartWrite() */

#if (SCI_UART_CFG_EXTERNAL_RTS_OPERATION)
    r_sci_uart_external_rts_operation_enable(p_cfg);            /** configure FIFO related registers */
#endif

    return SSP_SUCCESS;
}  /* End of function R_SCI_UartOpen() */

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS              Channel successfully closed.
 * @retval  SSP_ERR_ASSERTION        Pointer to UART control block is NULL.
 * @note This function is reentrant.
 **********************************************************************************************************************/
ssp_err_t R_SCI_UartClose (uart_ctrl_t * const p_ctrl)
{
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
#endif

    /** Close transfer interface. */
    if (g_sci_ctrl_blk[p_ctrl->channel].mode == SCI_MODE_ASYNC)
    {
#if (SCI_UART_CFG_RX_ENABLE)
        if (NULL != p_ctrl->p_transfer_rx)
        {
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
            SSP_ASSERT(NULL != p_ctrl->p_transfer_rx->p_api);
            SSP_ASSERT(NULL != p_ctrl->p_transfer_rx->p_ctrl);
#endif
            p_ctrl->p_transfer_rx->p_api->close(p_ctrl->p_transfer_rx->p_ctrl);
        }
#endif
#if (SCI_UART_CFG_TX_ENABLE)
        if (NULL != p_ctrl->p_transfer_tx)
        {
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
            SSP_ASSERT(NULL != p_ctrl->p_transfer_tx->p_api);
            SSP_ASSERT(NULL != p_ctrl->p_transfer_tx->p_ctrl);
#endif
            p_ctrl->p_transfer_tx->p_api->close(p_ctrl->p_transfer_tx->p_ctrl);
        }
#endif
    }

    /** clears control block parameters */
    p_ctrl->p_callback   = NULL;

    /** disables the associated interrupts<br>
     *  disables receiver<br>
     *  disables transmitter<br>
     *  removes power to the SCI channel<br>
     *  unlocks specified SCI channel<br>
     *  clears device context<br>
     */
    r_sci_close_common(p_ctrl->channel);

    return SSP_SUCCESS;
}  /* End of function R_SCI_UartClose() */

#if (SCI_UART_CFG_RX_ENABLE)

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS                  Data reception successfully ends.
 * @retval  SSP_ERR_HW_LOCKED            Channel is locked.
 * @retval  SSP_ERR_ASSERTION            Pointer to UART control block is NULL.
 * @retval  SSP_ERR_INVALID_MODE         Channel is used for non-UART mode.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Destination address or data size is invalid against data length.
 * @note This function is reentrant. This API is only valid when SCI_UART_CFG_RX_ENABLE is enabled.
 *       If 9-bit data length is specified at R_SCI_UartOpen call, dest must be aligned 16-bit boundary.
 **********************************************************************************************************************/
ssp_err_t R_SCI_UartRead (uart_ctrl_t * const p_ctrl, uint8_t const * const p_dest, uint32_t const bytes)
{
    ssp_err_t err        = SSP_SUCCESS;
    uint32_t  data_bytes = 1;

#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
    /** checks arguments */
    err = r_sci_read_write_param_check(p_ctrl, p_dest, p_ctrl->p_transfer_rx, bytes);
    SCI_UART_ERROR_RETURN((SSP_SUCCESS == err), err);
#endif

    if (0 == bytes)
    {
        /** Nothing to do. */
        return SSP_SUCCESS;
    }

    /** checks data byte length */
    if (HW_SCI_IsDataLength9bits(p_ctrl->channel))
    {
        data_bytes = 2;               /* data length per data entry is 2byte if 9bits data length */
    }

    if (g_sci_ctrl_blk[p_ctrl->channel].mode == SCI_MODE_ASYNC)
    {
        /** Read from SCI channel. */
        if (NULL != p_ctrl->p_transfer_rx)
        {
            uint32_t size = bytes / data_bytes;
            if(0xFFFF < size)
            {
                size = 0xFFFF;
            }
            /* Cast to a value acceptable by the transfer interface. */
            uint8_t const * p_src = (uint8_t const *) HW_SCI_ReadAddrGet(p_ctrl->channel, data_bytes);
            err = p_ctrl->p_transfer_rx->p_api->reset(p_ctrl->p_transfer_rx->p_ctrl, p_src, (void *) p_dest, (uint16_t)size);
            SCI_UART_ERROR_RETURN(SSP_SUCCESS == err, err);
        }
        else
        {
            /** Do nothing in non-transfer case.  Bytes will come in through the callback. */
        }
    }

    return err;
}  /* End of function R_SCI_UartRead() */
#endif /* if (SCI_UART_CFG_RX_ENABLE) */

#if (SCI_UART_CFG_TX_ENABLE)

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS                  Data transmission finished successfully.
 * @retval  SSP_ERR_ASSERTION            Pointer to UART control block is NULL.
 * @retval  SSP_ERR_INVALID_MODE         Channel is used for non-UART mode or illegal mode is set in handle.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Source address or data size is invalid against data length.
 * @retval  SSP_ERR_HW_LOCKED            Could not lock hardware.
 * @note This function is reentrant.
 *       If 9-bit data length is specified at R_SCI_UartOpen call, the source must be aligned on a 16-bit boundary.
 **********************************************************************************************************************/
ssp_err_t R_SCI_UartWrite (uart_ctrl_t * const p_ctrl, uint8_t const * const p_src, uint32_t const bytes)
{
    ssp_err_t err        = SSP_SUCCESS;
    uint32_t  data_bytes = 1;
    uint32_t  fifo_rest;

#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
    /** checks arguments */
    err = r_sci_read_write_param_check(p_ctrl, p_src, p_ctrl->p_transfer_tx, bytes);
    if (SSP_SUCCESS != err)
    {
        return err;
    }
    SCI_UART_ERROR_RETURN(NULL == p_ctrl->p_tx_src, SSP_ERR_IN_USE);
#endif

    if (SCI_MODE_ASYNC == g_sci_ctrl_blk[p_ctrl->channel].mode)
    {
        /** disables TIE prior to disabling TE because disabling TE during TIE=1 generates
         *   TXI interrupt which is not expected here. */
        HW_SCI_InterruptEnable(p_ctrl->channel, SCI_TX_INT, false);

        HW_SCI_TransmitterDisable(p_ctrl->channel);                 /** disables first prepare for any case */
        if (!g_sci_ctrl_blk[p_ctrl->channel].tx_busy)
        {
            g_sci_ctrl_blk[p_ctrl->channel].tx_busy = true;     /** sets transmit status as ON TRANSACTION */
        }

        /** checks the rest size in FIFO */
        fifo_rest = SCI_FIFO_STAGE_NUM - HW_SCI_FIFO_WriteCount(p_ctrl->channel);
        SSP_PARAMETER_NOT_USED(fifo_rest);
        if (HW_SCI_IsDataLength9bits(p_ctrl->channel))
        {
            data_bytes = 2;
        }

        if ((p_ctrl->p_transfer_tx) && (bytes > 1))
        {
            /** Configure a transfer to transfer all but the last byte.  The last byte is sent in the ISR to enable the
             * transmit end ISR, which does not work if the last byte is sent with DMAC or DTC. */

            /** Store last character to transmit from ISR. */
            p_ctrl->p_tx_src = &p_src[bytes - data_bytes];
            p_ctrl->tx_src_bytes = data_bytes;

            /* Cast the register to a value acceptable to the transfer interface. */
            uint8_t * p_dest = (uint8_t *) HW_SCI_WriteAddrGet(p_ctrl->channel, data_bytes);
            uint32_t num_transfers = (bytes / data_bytes) - 1;
            if(0xFFFF < num_transfers)
            {
                num_transfers = 0xFFFF;
            }
            uint8_t const * p_transfer_src = p_src;
            err = p_ctrl->p_transfer_tx->p_api->reset(p_ctrl->p_transfer_tx->p_ctrl, p_transfer_src, p_dest, (uint16_t)num_transfers);
            SCI_UART_ERROR_RETURN(SSP_SUCCESS == err, err);
        }
        else
        {
            /** All bytes will be transmitted from ISR. */
            p_ctrl->p_tx_src = p_src;
            p_ctrl->tx_src_bytes = bytes;
        }

        HW_SCI_TransmitterEnable(p_ctrl->channel);          /** enables transmitter */
        HW_SCI_InterruptEnable(p_ctrl->channel, SCI_TX_INT, true);
        err = SSP_SUCCESS;
    }

    return err;
}  /* End of function R_SCI_UartWrite() */
#endif /* if (SCI_UART_CFG_TX_ENABLE) */

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS                  Baud rate was successfully changed.
 * @retval  SSP_ERR_ASSERTION            Pointer to UART control block is NULL.
 * @retval  SSP_ERR_INVALID_ARGUMENT     Illegal baud rate value is specified.
 * @retval  SSP_ERR_HW_LOCKED            Could not lock hardware.
 * @note This function is reentrant. Clock source cannot be changed by this API, need to open again if it is needed.
 **********************************************************************************************************************/
ssp_err_t R_SCI_UartBaudSet (uart_ctrl_t const * p_ctrl, uint32_t const baudrate)
{
    ssp_err_t     err = SSP_SUCCESS;
    sci_clk_src_t clk_src;

#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
#endif

    /** disables interrupts */
    HW_SCI_InterruptEnable(p_ctrl->channel, SCI_ALL_INT, false);

    /** disables transmitter. This API does not resume transmission but terminate it */
    HW_SCI_TransmitterDisable(p_ctrl->channel);

    /** disables receiver */
    HW_SCI_ReceiverDisable(p_ctrl->channel);

    /** sets baud-rate related registers */
    if (HW_SCI_IsBaudRateInternalClkSelected(p_ctrl->channel))
    {
        clk_src = SCI_CLK_SRC_INT;
    }
    else
    {
        if (HW_SCI_IsBaudRateGenClkDivideBy8Selected(p_ctrl->channel))
        {
            clk_src = SCI_CLK_SRC_EXT8X;
        }
        else
        {
            clk_src = SCI_CLK_SRC_EXT16X;
        }
    }

    err = r_sci_uart_baud_set(p_ctrl->channel, clk_src, baudrate);

    /** enables receiver */
    HW_SCI_ReceiverEnable(p_ctrl->channel);

    /** enables interrupts */
    HW_SCI_InterruptEnable(p_ctrl->channel, SCI_RX_INT, true);

    return err;
}  /* End of function R_SCI_UartBaudSet() */

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS                  Baud rate was successfully changed.
 * @retval  SSP_ERR_ASSERTION            Pointer to UART control block is NULL.
 * @note This function is reentrant.
 **********************************************************************************************************************/
ssp_err_t R_SCI_UartInfoGet (uart_ctrl_t * const p_ctrl, uart_info_t * const p_info)
{
    ssp_err_t     err = SSP_SUCCESS;

#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_info);
#endif

    p_info->read_bytes_max = 0;
    p_info->write_bytes_max = 0;

    if (NULL != p_ctrl->p_transfer_rx)
    {
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
        SSP_ASSERT(p_ctrl->p_transfer_rx->p_api);
        SSP_ASSERT(p_ctrl->p_transfer_rx->p_api->infoGet);
#endif
        transfer_properties_t properties;
        err = p_ctrl->p_transfer_rx->p_api->infoGet(p_ctrl->p_transfer_rx->p_ctrl, &properties);
        SCI_UART_ERROR_RETURN(SSP_SUCCESS == err, err);

        p_info->read_bytes_max = properties.transfer_length_max;
    }
    else
    {
        /* No limit to number of bytes to read. */
        p_info->read_bytes_max = 0xFFFFFFFF;
    }

    if (NULL != p_ctrl->p_transfer_tx)
    {
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
        SSP_ASSERT(p_ctrl->p_transfer_tx->p_api);
        SSP_ASSERT(p_ctrl->p_transfer_tx->p_api->infoGet);
#endif
        transfer_properties_t properties;
        err = p_ctrl->p_transfer_tx->p_api->infoGet(p_ctrl->p_transfer_tx->p_ctrl, &properties);
        SCI_UART_ERROR_RETURN(SSP_SUCCESS == err, err);

        p_info->write_bytes_max = properties.transfer_length_max;
    }
    else
    {
        /* No limit to number of bytes to write. */
        p_info->write_bytes_max = 0xFFFFFFFF;
    }

    return err;
}  /* End of function R_SCI_UartInfoGet() */

/*******************************************************************************************************************//**
 * @retval   Version number
 * @note This function is reentrant.
 **********************************************************************************************************************/
ssp_err_t R_SCI_UartVersionGet (ssp_version_t * p_version)
{
    *p_version = module_version;
    return SSP_SUCCESS;
} /* End of function R_SCI_UartVersionGet() */

/*******************************************************************************************************************//**
 * @} (end addtogroup UARTonSCI)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)

/*******************************************************************************************************************//**
 * Parameter error check function for open processing
 * @param[in] p_ctrl   Pointer to the control block for the channel
 * @param[in] p_cfg    Pointer to the configuration structure specific to UART mode
 * @retval  SSP_SUCCESS                  No parameter error found
 * @retval  SSP_ERR_IN_USE               Channel already in use
 * @retval  SSP_ERR_ASSERTION            Pointer to UART control block or configuration structure is NULL
 * @retval  SSP_ERR_INVALID_ARGUMENT     Invalid parameter setting found in the configuration structure
 **********************************************************************************************************************/
static ssp_err_t r_sci_uart_open_param_check (uart_ctrl_t const * const p_ctrl, uart_cfg_t const * const p_cfg)
{
	ssp_err_t err = SSP_SUCCESS;

    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_cfg);
    /* Check channel number. */
    err = r_sci_channel_param_check(p_cfg->channel);
    SCI_UART_ERROR_RETURN((SSP_SUCCESS == err), err);

    SCI_UART_ERROR_RETURN((SCI_MODE_OFF == g_sci_ctrl_blk[p_cfg->channel].mode), SSP_ERR_IN_USE);

    SCI_UART_ERROR_RETURN((((UART_DATA_BITS_7  == p_cfg->data_bits) || (UART_DATA_BITS_8  == p_cfg->data_bits))
                           || (UART_DATA_BITS_9  == p_cfg->data_bits)), SSP_ERR_INVALID_ARGUMENT);

    SCI_UART_ERROR_RETURN(((UART_STOP_BITS_1 == p_cfg->stop_bits) || (UART_STOP_BITS_2 == p_cfg->stop_bits))
                          , SSP_ERR_INVALID_ARGUMENT);

    SCI_UART_ERROR_RETURN((((UART_PARITY_OFF  == p_cfg->parity) || (UART_PARITY_EVEN == p_cfg->parity))
                           || (UART_PARITY_ODD  == p_cfg->parity)), SSP_ERR_INVALID_ARGUMENT);

    if (p_cfg->p_extend)
    {
        if (SCI_CLK_SRC_INT == ((uart_on_sci_cfg_t *) (p_cfg->p_extend))->clk_src)
        {
            SCI_UART_ERROR_RETURN((0 != p_cfg->baud_rate), SSP_ERR_INVALID_ARGUMENT);
        }
        else if ((SCI_CLK_SRC_EXT8X  == ((uart_on_sci_cfg_t *) (p_cfg->p_extend))->clk_src)
                 || (SCI_CLK_SRC_EXT16X == ((uart_on_sci_cfg_t *) (p_cfg->p_extend))->clk_src)
                 )
        {
            /* No error */
        }
        else
        {
            return SSP_ERR_INVALID_ARGUMENT;
        }
    }

    return err;
}  /* End of function r_sci_uart_open_param_check() */

/*******************************************************************************************************************//**
 * Parameter error check function for read/write processing
 * @param[in] p_ctrl Pointer to the control block for the channel
 * @param[in] addr   Pointer to the buffer
 * @param[in] size   Data size
 * @retval  SSP_SUCCESS              No parameter error found
 * @retval  SSP_ERR_ASSERTION        Pointer to UART control block or configuration structure is NULL
 * @retval  SSP_ERR_INVALID_MODE     Channel is used for non-UART mode or illegal mode is set
 * @retval  SSP_ERR_INVALID_ARGUMENT Address is not aligned to 2-byte boundary or size is the odd number when the data
 * length is 9-bit
 * @note This function is reentrant.
 **********************************************************************************************************************/
static ssp_err_t r_sci_read_write_param_check (uart_ctrl_t const * const p_ctrl,
                                               uint8_t const * const     addr,
                                               transfer_instance_t const * const p_transfer,
                                               uint32_t const            bytes)
{
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(addr);
    SCI_UART_ERROR_RETURN((g_sci_ctrl_blk[p_ctrl->channel].mode != SCI_MODE_OFF), SSP_ERR_INVALID_MODE);
    SCI_UART_ERROR_RETURN((g_sci_ctrl_blk[p_ctrl->channel].mode < SCI_MODE_MAX), SSP_ERR_INVALID_MODE);

    if (HW_SCI_IsDataLength9bits(p_ctrl->channel))
    {
        /* Do not allow odd buffer address if data length is 9bits. */
        SCI_UART_ERROR_RETURN((0 == ((uint32_t) addr % 2)), SSP_ERR_INVALID_ARGUMENT);

        /* Do not allow odd number of data size if data length is 9bits. */
        SCI_UART_ERROR_RETURN((0 == (bytes % 2)), SSP_ERR_INVALID_ARGUMENT);
    }

    if (NULL != p_transfer)
    {
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
        SSP_ASSERT(NULL != p_ctrl->p_transfer_rx->p_api);
        SSP_ASSERT(NULL != p_ctrl->p_transfer_rx->p_ctrl);
#endif
    }

    return SSP_SUCCESS;
}  /* End of function r_sci_read_write_param_check() */
#endif /* if (SCI_UART_CFG_PARAM_CHECKING_ENABLE) */

/*******************************************************************************************************************//**
 * Configures UART related transfer driverss (if enabled).
 * @param[in]     p_cfg   Pointer to UART specific configuration structure
 * @retval        none
 **********************************************************************************************************************/
static ssp_err_t    r_sci_uart_transfer_open     (uart_cfg_t const * const p_cfg)
{
#if (SCI_UART_CFG_RX_ENABLE)
    if (NULL != p_cfg->p_transfer_rx)
    {
        /** Configure the RX transfer, if enabled. */
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
        SSP_ASSERT(NULL != p_cfg->p_transfer_rx->p_api);
        SSP_ASSERT(NULL != p_cfg->p_transfer_rx->p_ctrl);
        SSP_ASSERT(NULL != p_cfg->p_transfer_rx->p_cfg);
        SSP_ASSERT(NULL != p_cfg->p_transfer_rx->p_cfg->p_info);
#endif
        transfer_info_t * p_info = p_cfg->p_transfer_rx->p_cfg->p_info;
        p_info->mode = TRANSFER_MODE_NORMAL;
        p_info->dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
        p_info->src_addr_mode = TRANSFER_ADDR_MODE_FIXED;
        p_info->irq = TRANSFER_IRQ_END;
        if (UART_DATA_BITS_9 == p_cfg->data_bits)
        {
            p_info->size = TRANSFER_SIZE_2_BYTE;
        }
        else
        {
            p_info->size = TRANSFER_SIZE_1_BYTE;
        }
        /* Check to make sure the interrupt is enabled. */
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
        IRQn_Type irq = r_sci_select_irqn(p_cfg->channel, SCI_RX_INT);
        SSP_ASSERT(BSP_MAX_NUM_IRQn != irq);
#endif
        transfer_cfg_t cfg = *(p_cfg->p_transfer_rx->p_cfg);
        cfg.activation_source = r_sci_rxi_event_lookup(p_cfg->channel);
        cfg.auto_enable = false;
        ssp_err_t err = p_cfg->p_transfer_rx->p_api->open(p_cfg->p_transfer_rx->p_ctrl, &cfg);
        SCI_UART_ERROR_RETURN(SSP_SUCCESS == err, err);
    }
#endif
#if (SCI_UART_CFG_TX_ENABLE)
    if (NULL != p_cfg->p_transfer_tx)
    {
        /** Configure the TX transfer, if enabled. */
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
        SSP_ASSERT(NULL != p_cfg->p_transfer_tx->p_api);
        SSP_ASSERT(NULL != p_cfg->p_transfer_tx->p_ctrl);
        SSP_ASSERT(NULL != p_cfg->p_transfer_tx->p_cfg);
        SSP_ASSERT(NULL != p_cfg->p_transfer_tx->p_cfg->p_info);
#endif
        transfer_info_t * p_info = p_cfg->p_transfer_tx->p_cfg->p_info;
        p_info->mode = TRANSFER_MODE_NORMAL;
        p_info->dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;
        p_info->src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
        p_info->irq = TRANSFER_IRQ_END;
        if (UART_DATA_BITS_9 == p_cfg->data_bits)
        {
            p_info->size = TRANSFER_SIZE_2_BYTE;
        }
        else
        {
            p_info->size = TRANSFER_SIZE_1_BYTE;
        }
#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
        /* Check to make sure the interrupt is enabled. */
        IRQn_Type irq = r_sci_select_irqn(p_cfg->channel, SCI_TX_INT);
        SSP_ASSERT(BSP_MAX_NUM_IRQn != irq);
#endif
        transfer_cfg_t cfg = *(p_cfg->p_transfer_tx->p_cfg);
        cfg.activation_source = r_sci_txi_event_lookup(p_cfg->channel);
        cfg.auto_enable = false;
        ssp_err_t err = p_cfg->p_transfer_tx->p_api->open(p_cfg->p_transfer_tx->p_ctrl, &cfg);
        SCI_UART_ERROR_RETURN(SSP_SUCCESS == err, err);
    }
#endif
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configures UART related registers
 * @param[in]     p_cfg   Pointer to UART specific configuration structure
 * @retval        none
 **********************************************************************************************************************/
static ssp_err_t r_sci_uart_config_set (uart_cfg_t const * const p_cfg)
{
    ssp_err_t         err;
    uart_on_sci_cfg_t * pextend = (uart_on_sci_cfg_t *) p_cfg->p_extend;
    sci_clk_src_t     clk_src;

    HW_SCI_AsyncModeSet(p_cfg->channel);               /* applies ASYNC mode */

    HW_SCI_ClockPhaseDelayDisable(p_cfg->channel);     /* disables SCKn clock delay */

    HW_SCI_ClockPorarityNormalSet(p_cfg->channel);     /* sets SCKn clock polarity normal */

    if (UART_PARITY_OFF != p_cfg->parity)
    {
        HW_SCI_ParityBitEnable(p_cfg->channel);        /* enables parity */
    }
    else
    {
        HW_SCI_ParityBitDisable(p_cfg->channel);       /* disables parity */
    }

    if (UART_PARITY_ODD == p_cfg->parity)
    {
        HW_SCI_ParityOddSelect(p_cfg->channel);        /* selects odd parity */
    }
    else
    {
        HW_SCI_ParityEvenSelect(p_cfg->channel);       /* selects even parity */
    }

    if (UART_DATA_BITS_7 == p_cfg->data_bits)
    {
        HW_SCI_DataBits7bitsSelect(p_cfg->channel);    /* selects 7-bit data length */
    }
    else if (UART_DATA_BITS_9 == p_cfg->data_bits)
    {
        HW_SCI_DataBits9bitsSelect(p_cfg->channel);    /* selects 9-bit data length */
    }
    else
    {
        HW_SCI_DataBits8bitsSelect(p_cfg->channel);    /* selects 8-bit data length */
    }

    if (UART_STOP_BITS_2 == p_cfg->stop_bits)
    {
        HW_SCI_StopBits2bitsSelect(p_cfg->channel);    /* selects 2-bit stop bit length */
    }
    else
    {
        HW_SCI_StopBits1bitSelect(p_cfg->channel);     /* selects 1-bit stop bit length */
    }

    if (p_cfg->ctsrts_en)
    {
        HW_SCI_CtsInEnable(p_cfg->channel);            /* enables CTS hardware flow control on RTSn#/CTSn# pin */
    }
    else
    {
        HW_SCI_RtsOutEnable(p_cfg->channel);           /* enables RTS hardware flow control on RTSn#/CTSn# pin */
    }

    /** sets baud rate */
    if ((pextend)
        && (((SCI_CLK_SRC_INT    == pextend->clk_src)
             ||   (SCI_CLK_SRC_EXT8X  == pextend->clk_src))
            ||   (SCI_CLK_SRC_EXT16X == pextend->clk_src))
        )
    {
        clk_src = pextend->clk_src;
    }
    else
    {
        clk_src = SCI_CLK_SRC_INT;
    }

    err = r_sci_uart_baud_set(p_cfg->channel, clk_src, p_cfg->baud_rate);
    if (err == SSP_ERR_INVALID_ARGUMENT)
    {
        HW_SCI_BaudClkOutputDisable(p_cfg->channel);       /** set default setting */
        return err;
    }

    if ((pextend) && (pextend->baudclk_out))
    {
        HW_SCI_BaudClkOutputEnable(p_cfg->channel);        /** enables Baud rate clock output */
    }
    else
    {
        HW_SCI_BaudClkOutputDisable(p_cfg->channel);       /** disables Baud rate clock output */
    }

    return SSP_SUCCESS;
}  /* End of function r_sci_uart_config_set() */

/*******************************************************************************************************************//**
 * This function enables external RTS(using a GPIO) operation
 * @param[in] p_cfg    Pointer to UART configuration structure
 * @retval  SSP_SUCCESS                  Circular buffer for a channel initialized successfully
 * @note This function is reentrant.
 **********************************************************************************************************************/
#if (SCI_UART_CFG_EXTERNAL_RTS_OPERATION)
static void r_sci_uart_external_rts_operation_enable (uart_cfg_t const * const p_cfg)
{
    uart_on_sci_cfg_t * pextend = (uart_on_sci_cfg_t *) p_cfg->p_extend;

    if ((p_cfg->ctsrts_en) && (pextend->p_extpin_ctrl))
    {
        g_sci_ctrl_blk[p_cfg->channel].p_extpin_ctrl = pextend->p_extpin_ctrl;
        g_sci_ctrl_blk[p_cfg->channel].p_extpin_ctrl(p_cfg->channel, 0);   /** user definition function call to control
                                                                            * GPIO */
    }
}  /* End of function r_sci_uart_external_rts_operation_enable () */
#endif /* if (SCI_UART_CFG_EXTERNAL_RTS_OPERATION) */

/*******************************************************************************************************************//**
 * Resets FIFO related registers
 * @param[in]     p_cfg   Pointer to UART specific configuration structure
 * @retval        none
 **********************************************************************************************************************/
static void r_sci_uart_fifo_reset (uart_cfg_t const * const p_cfg)
{
    HW_SCI_FifoDisable(p_cfg->channel);                    /** disables FIFO mode */

#if (SCI_UART_CFG_RX_ENABLE)
    HW_SCI_ReceiveFifoReset(p_cfg->channel);                              /** resets receive FIFO mode */
    if (NULL != p_cfg->p_transfer_rx)
    {
        /** Set receive trigger number to 0 to facilitate transfer. */
        HW_SCI_RxTriggerNumberSet(p_cfg->channel, 0);
    }
    else
    {
        /** Set receive trigger number as half of FIFO stage */
        HW_SCI_RxTriggerNumberSet(p_cfg->channel, (SCI_FIFO_STAGE_NUM >> 1));
    }
    HW_SCI_RTSTriggerNumberSet(p_cfg->channel, (SCI_FIFO_STAGE_NUM - 1)); /* sets RTS trigger number */
#endif
#if (SCI_UART_CFG_TX_ENABLE)
    HW_SCI_TransmitFifoReset(p_cfg->channel);                            /** resets transmit FIFO mode */
    if (NULL != p_cfg->p_transfer_tx)
    {
        /** Set transfer trigger number to 0 to facilitate transfer. */
        HW_SCI_TxTriggerNumberSet(p_cfg->channel, 0);
    }
    else
    {
        /** Set FIFO trigger to trigger when one byte is remaining. */
        HW_SCI_TxTriggerNumberSet(p_cfg->channel, (SCI_FIFO_STAGE_NUM - 1));
    }
#endif
}  /* End of function r_sci_uart_fifo_reset() */

/*******************************************************************************************************************//**
 * Enable FIFO
 * @param[in]     p_cfg   Pointer to UART specific configuration structure
 * @retval        none
 **********************************************************************************************************************/
static void r_sci_uart_fifo_enable (uart_cfg_t const * const p_cfg)
{
#if (SCI_UART_CFG_RX_ENABLE)
    while (0 != HW_SCI_ReceiveFifoResetStatusRead(p_cfg->channel))
    {
        /* FIFO reset status is automatically cleared after 1 peripheral clock for SCI */
    }
#endif
#if (SCI_UART_CFG_TX_ENABLE)
    while (0 != HW_SCI_TransmitFifoResetStatusRead(p_cfg->channel))
    {
        /* FIFO reset status is automatically cleared after 1 peripheral clock for SCI */
    }
#endif

    HW_SCI_FifoEnable(p_cfg->channel);                 /** enables FIFO mode */
}  /* End of function r_sci_uart_fifo_enable() */

/*******************************************************************************************************************//**
 * Changes baud rate. It evaluates and determines the best possible settings set to the baud rate related registers.
 * @param[in] channel      Channel number of SCI module
 * @param[in] clk_src      A clock source for SCI module (SCI_CLK_SRC_INT|SCI_CLK_SRC_EXT8X|SCI_CLK_SRC_EXT16X)
 * @param[in] baudrate     Baud rate[bps] e.g. 19200, 57600, 115200, etc.
 * @retval  SSP_SUCCESS                  Baud rate is set successfully
 * @retval  SSP_ERR_INVALID_ARGUMENT     Baud rate is '0' or cannot set properly
 * @note   The transmitter and receiver (TE and RE bits in SCR) must be disabled prior to calling this function.
 **********************************************************************************************************************/
static ssp_err_t  r_sci_uart_baud_set (uint32_t const channel, sci_clk_src_t clk_src, uint32_t baudrate)
{
    uint32_t       i   = 8;
    uint32_t       hit = 0;
    uint32_t       brr = 255;
    uint32_t       temp_brr;
    int32_t        bit_err;
    int32_t        temp_bit_err = 10000;
    uint32_t       divisor;
    baud_setting_t * pbaudinfo;

#if (SCI_UART_CFG_PARAM_CHECKING_ENABLE)
    SCI_UART_ERROR_RETURN((0 != baudrate), SSP_ERR_INVALID_ARGUMENT);
#endif

    /* selects proper table based upon mode */
    pbaudinfo = (baud_setting_t *) async_baud;

    if (SCI_CLK_SRC_INT == clk_src)
    {
        /* FIND BEST_BRR_VALUE
         *  In table async_baud", divisor value is associated with BGDM, ABCS, ABCSE and N values, so once best divisor
         * value is found,
         *  baud rate related register setting values are decided. The formula to calculate BRR is as follows and it
         * must be 255 or less.
         *  BRR = (PCLKA/(div_coefficient * baud)) - 1
         */
        for (i = 0; i < NUM_DIVISORS_ASYNC; i++)
        {
            uint32_t freq_hz;
            SCI_UART_ERROR_RETURN((SSP_SUCCESS == (g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKA, &freq_hz))),
                                  SSP_ERR_INVALID_ARGUMENT);

            temp_brr = freq_hz / (pbaudinfo[i].div_coefficient * baudrate);
            if (0 < temp_brr)
            {
                temp_brr -= 1;

                /* Calculate the bit rate error and have a best selection. The formula is as follows.
                 *  bit error[%] = {(PCLK / (baud * div_coefficient * (BRR + 1)) - 1} x 100
                 *  calculates bit error[%] to two decimal places
                 */
                divisor   = (temp_brr + 1) * (uint32_t) pbaudinfo[i].div_coefficient;
                divisor  *= baudrate;
                divisor >>= 9;  /* This prevents overflow beyond 32-bit size.  */
                bit_err   = (int32_t) ((((freq_hz >> 9) * 10000L) / divisor) - 10000L);
                if (temp_brr < 256)
                {
                    if (bit_err < temp_bit_err)
                    {
                        hit          = i;
                        brr          = temp_brr;
                        temp_bit_err = bit_err;
                    }
                }
            }
        }

        SCI_UART_ERROR_RETURN((10000 != temp_bit_err), SSP_ERR_INVALID_ARGUMENT);

        HW_SCI_BaudRateGenInternalClkSelect(channel);

        SCI_UART_ERROR_RETURN(0xFF >= brr, SSP_ERR_ASSERTION);

        HW_SCI_UartBitRateSet(channel, (uint8_t)brr, &pbaudinfo[hit]);
    }
    else
    {
        HW_SCI_BitRateDefaultSet(channel);

        HW_SCI_BaudRateGenExternalClkSelect(channel);

        if (SCI_CLK_SRC_EXT8X == clk_src)
        {
            HW_SCI_BaudRateGenExternalClkDivideBy8(channel);
        }
        else
        {
            HW_SCI_BaudRateGenExternalClkDivideBy16(channel);
        }
    }

    return SSP_SUCCESS;
}  /* End of function r_sci_uart_baud_set() */

/*******************************************************************************************************************//**
 * TXI interrupt processing for UART mode. TXI interrupt happens when the data in the data register or FIFO register has
 *  to be transferred to the data shift register, and the next writing data to it is available. This function is
 *  repeatedly called from TXI ISR and continues data transfer until the transmit circular buffer becoming empty.
 * Finally
 *  this function disables TXI interrupt and enables TEI interrupt(@see r_sci_uart_tei()).
 * @param[in] channel    Channel number of SCI module
 * @retval    none
 **********************************************************************************************************************/
void r_sci_uart_txi_common (uint32_t const channel)
{
    uart_ctrl_t * p_ctrl    = (uart_ctrl_t *) g_sci_ctrl_blk[channel].p_context;
    uint32_t fifo_remaining = HW_SCI_FIFO_WriteCount(channel);
    int32_t fifo_open = SCI_FIFO_STAGE_NUM - (int32_t)fifo_remaining;
    uint16_t data;

    /** checks data byte length */
    uint32_t data_bytes = 1;
    if (HW_SCI_IsDataLength9bits(p_ctrl->channel))
    {
        data_bytes = 2;
    }

    if (NULL == p_ctrl->p_transfer_tx)
    {

        for (uint32_t cnt = 0; cnt < p_ctrl->tx_src_bytes; cnt += data_bytes)
        {
            if (fifo_open <= 0)
            {
                /* FIFO full. */
                break;
            }
            if (1 == data_bytes)
            {
                data = (uint16_t) *(p_ctrl->p_tx_src);
            }
            else
            {
                data = (uint16_t) *((uint16_t *) p_ctrl->p_tx_src);    /* 9-bit data length needs 2 bytes */
            }

            /** Write to the hardware FIFO. */
            HW_SCI_WriteFIFO(p_ctrl->channel, data);
            fifo_open--;
            p_ctrl->tx_src_bytes -= data_bytes;
            p_ctrl->p_tx_src += data_bytes;
        }
        HW_SCI_TDFEClear(p_ctrl->channel);
    }
    else
    {
        if (0 == fifo_remaining)
        {
            HW_SCI_WriteFIFO(p_ctrl->channel, *(p_ctrl->p_tx_src));
            p_ctrl->tx_src_bytes -= data_bytes;
            p_ctrl->p_tx_src += data_bytes;
        }
    }
    if (0 == p_ctrl->tx_src_bytes)
    {
        HW_SCI_InterruptEnable(channel, SCI_TE_INT, true);
        HW_SCI_InterruptEnable(channel, SCI_TX_INT, false);
        p_ctrl->p_tx_src = NULL;
    }
}  /* End of function r_sci_uart_txi_common() */

/*******************************************************************************************************************//**
 * RXI interrupt processing for UART mode. RXI interrupt happens when data arrives to the data register or the FIFO
 *  register.  This function calls callback function when it meets conditions below.
 *  - The number of data which has been read reaches to the number specified in R_SCI_UartRead()
 *(UART_EVENT_RX_COMPLETE)
 *  This function also calls the callback function for RTS pin control if it is registered in R_SCI_UartOpen(). This is
 *  special functionality to expand SCI hardware capability and make RTS/CTS hardware flow control possible. If macro
 *  'SCI_UART_CFG_EXTERNAL_RTS_OPERATION' is set, it is called at the beginning in this function,
 *  data done, it is called again (just before leaving this function). SCI UART module does not control any GPIOs but
 *  this callback function let user know the timing of RTS signal assert or negate but user have to control the GPIO pin
 *  which is used for RTS pin.
 * @param[in] channel    Channel number of SCI module
 * @retval    none
 **********************************************************************************************************************/
void r_sci_uart_rxi_common (uint32_t const channel)
{
    uint32_t             data             = 0;
    uart_callback_args_t args;
    uart_ctrl_t          * pctrl          = (uart_ctrl_t *) g_sci_ctrl_blk[channel].p_context;
    uint32_t             read_cnt;

#if (SCI_UART_CFG_EXTERNAL_RTS_OPERATION)
    if (g_sci_ctrl_blk[channel].p_extpin_ctrl)
    {
        g_sci_ctrl_blk[channel].p_extpin_ctrl(channel, 1);         /** user definition function call to control GPIO */
    }
#endif
    read_cnt = HW_SCI_FIFO_ReadCount(channel);

    /** checks data byte length */
    uint32_t data_bytes = 1;
    if (HW_SCI_IsDataLength9bits(pctrl->channel))
    {
        data_bytes = 2;
    }

    if (NULL != pctrl->p_transfer_rx)
    {
        /* Do callback if available */
        if (NULL != pctrl->p_callback)
        {
            args.channel        = channel;
            args.data           = data;
            args.p_context      = pctrl->p_context;
            args.event = UART_EVENT_RX_COMPLETE;
            pctrl->p_callback(&args);
        }
    }
    else
    {
        while (read_cnt--)
        {
            /* Read data */
            data = HW_SCI_ReadFIFO(channel);

            /* Do callback if available */
            if (NULL != pctrl->p_callback)
            {
                args.channel        = channel;
                args.data           = (uint8_t) data;
                args.p_context      = pctrl->p_context;
                args.event = UART_EVENT_RX_CHAR;
                pctrl->p_callback(&args);
                if (2 == data_bytes)
                {
                    args.data       = (uint8_t) (data >> 8);
                    pctrl->p_callback(&args);
                }
            }
        }
    }

#if (SCI_UART_CFG_EXTERNAL_RTS_OPERATION)
    if (g_sci_ctrl_blk[channel].p_extpin_ctrl)
    {
        g_sci_ctrl_blk[channel].p_extpin_ctrl(channel, 0);         /** user definition function call to control GPIO */
    }
#endif
}  /* End of function r_sci_uart_rxi_common () */

/*******************************************************************************************************************//**
 * TEI interrupt processing for UART mode. TEI interrupt happens at the timing of data transmit completion. The user
 * callback function is called with UART_EVENT_TX_COMPLETE event code (if it is registered in R_SCI_UartOpen()).
 * @param[in] channel    Channel number of SCI module
 * @retval    none
 **********************************************************************************************************************/
void r_sci_uart_tei (uint32_t const channel)
{
    uart_callback_args_t args;
    uart_ctrl_t          * pctrl = (uart_ctrl_t *) g_sci_ctrl_blk[channel].p_context;

    /* Receiving TEI(transmit end interrupt) means the completion of
     * transmission, so call call-back function here
     */
    HW_SCI_InterruptEnable(channel, SCI_TE_INT, false);
    if (NULL != pctrl->p_callback)
    {
        args.channel   = channel;
        args.data      = 0;
        args.event     = UART_EVENT_TX_COMPLETE;
        args.p_context = pctrl->p_context;
        pctrl->p_callback(&args);
    }

    /* Transmission ended */
    g_sci_ctrl_blk[channel].tx_busy = false;
}  /* End of function r_sci_uart_tei () */

/*******************************************************************************************************************//**
 * ERI interrupt processing for UART mode. When ERI interrupt happens, the user callback function is called if it is
 *  registered in R_SCI_UartOpen() with the event code which happen at the time.
 * @param[in] channel    Channel number of SCI module
 * @retval    none
 **********************************************************************************************************************/
void r_sci_uart_eri_common (uint32_t const channel)
{
    uint32_t             data;
    uart_callback_args_t args;
    uart_ctrl_t          * pctrl = (uart_ctrl_t *) g_sci_ctrl_blk[channel].p_context;

    /* Read data */
    data = HW_SCI_ReadFIFO(channel);

    HW_SCI_RDFClear(channel);

    /* Error check */
    if (HW_SCI_OverRunErrorCheck(channel))
    {
        args.event = UART_EVENT_ERR_OVERFLOW;
    }
    else if (HW_SCI_FramingErrorCheck(channel))
    {
        if (HW_SCI_BreakDetectionCheck(channel))
        {
            args.event = UART_EVENT_BREAK_DETECT;
        }
        else
        {
            args.event = UART_EVENT_ERR_FRAMING;
        }
    }
    else
    {
        args.event = UART_EVENT_ERR_PARITY;
    }

    /* Do callback if available */
    if (NULL != pctrl->p_callback)
    {
        args.channel   = channel;
        args.data      = data;
        args.p_context = pctrl->p_context;
        pctrl->p_callback(&args);
    }
}  /* End of function r_sci_uart_eri_common () */
#endif /* if (SCI_CFG_ASYNC_INCLUDED) */
