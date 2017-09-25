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
 * File Name    : r_uart_api.h
 * Description  : UART Shared Interface definition
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup UART_API UART Interface
 * @brief Interface for UART communications.
 *
 * @section UART_INTERFACE_SUMMARY Summary
 * The UART interface provides common APIs for UART HAL drivers. The UART interface supports the following features:
 * - Full-duplex UART communication
 * - Generic UART parameter setting
 * - Interrupt driven transmit/receive processing
 * - Callback function with returned event code
 * - Runtime baud-rate change
 * - Hardware resource locking during a transaction
 * - CTS/RTS hardware flow control support (with an associated IOPORT pin)
 * - Circular buffer support
 * - Runtime Transmit/Receive circular buffer flushing
 *
 * Implemented as:
 * - @ref UARTonSCI
 *
 * * Related SSP architecture topics:
 *  - What is an SSP Interface? @ref ssp-interfaces
 *  - What is a SSP Layer? @ref ssp-predefined-layers
 *  - How to use SSP Interfaces and Modules? @ref using-ssp-modules
 *
 * UART Interface description: @ref HALUARTInterface
 * @{
 **********************************************************************************************************************/

#ifndef R_UART_API_H
#define R_UART_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "bsp_api.h"
#include "r_transfer_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define UART_API_VERSION_MAJOR (1)
#define UART_API_VERSION_MINOR (0)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** UART Event codes */
typedef enum e_sf_event
{
    UART_EVENT_RX_COMPLETE        = (1 << 0),         ///< Receive complete event
    UART_EVENT_TX_COMPLETE        = (1 << 1),         ///< Transmit complete event
    UART_EVENT_ERR_PARITY         = (1 << 2),         ///< Parity error event
    UART_EVENT_ERR_FRAMING        = (1 << 3),         ///< Mode fault error event
    UART_EVENT_BREAK_DETECT       = (1 << 4),         ///< Break detect error event
    UART_EVENT_ERR_OVERFLOW       = (1 << 5),         ///< FIFO Overflow error event
    UART_EVENT_ERR_RXBUF_OVERFLOW = (1 << 6),         ///< Receive buffer overflow error event
    UART_EVENT_RX_CHAR            = (1 << 7),         ///< Character received
} uart_event_t;

/** UART Data bit length definition */
typedef enum e_uart_data_bits
{
    UART_DATA_BITS_8,                       ///< Data bits 8-bit
    UART_DATA_BITS_7,                       ///< Data bits 7-bit
    UART_DATA_BITS_9                        ///< Data bits 9-bit
} uart_data_bits_t;

/** UART Parity definition */
typedef enum e_uart_parity
{
    UART_PARITY_OFF,                        ///< No parity
    UART_PARITY_ODD,                        ///< Odd parity
    UART_PARITY_EVEN                        ///< Even parity
} uart_parity_t;

/** UART Stop bits definition */
typedef enum e_uart_stop_bits
{
    UART_STOP_BITS_1,                       ///< Stop bit 1-bit
    UART_STOP_BITS_2                        ///< Stop bits 2-bit
} uart_stop_bits_t;

/** UART driver specific information */
typedef struct st_uart_info
{
    /** Maximum bytes that can be written at this time.  Only applies if uart_cfg_t::p_transfer_tx is not NULL. */
    uint32_t      write_bytes_max;

    /** Maximum bytes that are available to read at one time.  Only applies if uart_cfg_t::p_transfer_rx is not NULL. */
    uint32_t      read_bytes_max;
} uart_info_t;

/** UART Callback parameter definition */
typedef struct st_uart_callback_arg
{
    uint32_t      channel;                  ///< Device channel number
    uart_event_t  event;                    ///< Event code
    uint32_t      data;                     ///< General purpose data storage
    void const    * p_context;              ///< Context provided to user during callback
} uart_callback_args_t;

/** UART Configuration */
typedef struct st_uart_cfg
{
    /* UART generic configuration */
    uint32_t          channel;              ///< Select a channel corresponding to the channel number of the hardware.
    uint32_t          baud_rate;            ///< Baud rate, i.e. 9600, 19200, 115200
    uart_data_bits_t  data_bits;            ///< Data bit length (8 or 7 or 9)
    uart_parity_t     parity;               ///< Parity type (none or odd or even)
    uart_stop_bits_t  stop_bits;            ///< Stop bit length (1 or 2)
    bool              ctsrts_en;            ///< CTS/RTS hardware flow control enable

    /** Optional transfer instance used to receive multiple bytes without interrupts.  Set to NULL if unused.
     * If NULL, the number of bytes allowed in the read API is limited to one byte at a time. */
    transfer_instance_t const * p_transfer_rx;

    /** Optional transfer instance used to send multiple bytes without interrupts.  Set to NULL if unused.
     * If NULL, the number of bytes allowed in the write APIs is limited to one byte at a time. */
    transfer_instance_t const * p_transfer_tx;

    /* Configuration for UART Event processing */
    void (* p_callback)(uart_callback_args_t * p_args); ///< Pointer to callback function
    void const * p_context;                             ///< User defined context passed into callback function

    /* Pointer to UART peripheral specific configuration */
    void const * p_extend;                  ///< UART hardware dependent configuration
} uart_cfg_t;

/** UART Control block */
typedef struct st_uart_ctrl
{
    /* Parameters to control UART peripheral device */
    uint32_t  channel;                      ///< Channel number

    /** Optional transfer instance used to send or receive multiple bytes without interrupts. */
    transfer_instance_t const * p_transfer_rx;

    /** Optional transfer instance used to send or receive multiple bytes without interrupts. */
    transfer_instance_t const * p_transfer_tx;

    /** Source buffer pointer used to fill hardware FIFO from transmit ISR. */
    uint8_t const * p_tx_src;

    /** Size of source buffer pointer used to fill hardware FIFO from transmit ISR. */
    uint32_t tx_src_bytes;

    /* Parameters to process UART Event */
    void (* p_callback)(uart_callback_args_t * p_args); ///< Pointer to callback function
    void const * p_context;                             ///< Pointer to the higher level device context
                                                        //   (e.g. sf_uart_comms_ctrl_t type data )
} uart_ctrl_t;

/** Shared Interface definition for UART */
typedef struct st_uart_api
{
    /** Open  UART device.
     * @par Implemented as
     * - R_SCI_UartOpen()
     *
     * @param[in,out]  p_ctrl     Pointer to the UART control block Must be declared by user. Value set here.
     * @param[in]      uart_cfg_t Pointer to UART configuration structure. All elements of this structure must be set by
     *                            user.
     */
	ssp_err_t (* open)(uart_ctrl_t * const      p_ctrl,
                       uart_cfg_t const * const p_cfg);

    /** Read from UART device.  Read bytes will be received in the callback function provided by
     * uart_cfg_t::p_callback if p_transfer_rx is NULL.  If uart_cfg_t::p_transfer_rx is not NULL, the read
     * bytes will be written directly to the read input buffer.
     * @par Implemented as
     * - R_SCI_UartRead()
     *
     * @param[in]   p_ctrl     Pointer to the UART control block for the channel.
     * @param[in]   p_dest     Destination address to read data from.
     * @param[in]   bytes      Read data length.  Only applicable if uart_cfg_t::p_transfer_rx is not NULL.
     *                         Otherwise all read bytes will be provided through the callback set in
     *                         uart_cfg_t::p_callback.
     */
    ssp_err_t (* read)(uart_ctrl_t * const   p_ctrl,
                       uint8_t const * const p_dest,
                       uint32_t const        bytes);

    /** Write to UART device.
     * @par Implemented as
     * - R_SCI_UartWrite()
     *
     * @param[in]   p_ctrl     Pointer to the UART control block.
     * @param[in]   p_src      Source address  to write data to.
     * @param[in]   bytes      Write data length.
     */
    ssp_err_t (* write)(uart_ctrl_t * const p_ctrl,
                        uint8_t const * const     p_src,
                        uint32_t const            bytes);

    /** Change baud rate.
     * @par Implemented as
     * - R_SCI_UartBaudSet()
     *
     * @param[in]   p_ctrl     Pointer to the UART control block.
     * @param[in]   baudrate   Baud rate in bps.
     */
    ssp_err_t (* baudSet)(uart_ctrl_t const * const p_ctrl,
                          uint32_t const            baudrate);

    /** Get the driver specific information.
     * @par Implemented as
     * - R_SCI_UartInfoGet()
     *
     * @param[in]   p_ctrl     Pointer to the UART control block.
     * @param[in]   baudrate   Baud rate in bps.
     */
    ssp_err_t (* infoGet)(uart_ctrl_t * const p_ctrl,
                          uart_info_t * const p_info);

    /** Close UART device.
     * @par Implemented as
     * - R_SCI_UartClose()
     *
     * @param[in]   p_ctrl     Pointer to the UART control block.
     */
    ssp_err_t (* close)(uart_ctrl_t * const p_ctrl);

    /** Get version.
     * @par Implemented as
     * - R_SCI_UartVersionGet()
     *
     * @param[in]   p_version  Pointer to the memory to store the version information.
     */
    ssp_err_t (* versionGet)(ssp_version_t * p_version);
} uart_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_uart_instance
{
    uart_ctrl_t       * p_ctrl;    ///< Pointer to the control structure for this instance
    uart_cfg_t  const * p_cfg;     ///< Pointer to the configuration structure for this instance
    uart_api_t  const * p_api;     ///< Pointer to the API structure for this instance
} uart_instance_t;


/** @} (end defgroup UART_API) */
#endif /* R_UART_API_H */
