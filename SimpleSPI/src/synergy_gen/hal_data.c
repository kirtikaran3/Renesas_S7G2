/* generated HAL source file - do not edit */
#include "hal_data.h"
#if spi_on_rspi_CALLBACK_USED_g_spi0
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(spi_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
spi_ctrl_t g_spi0_ctrl;

/** RSPI extended configuration for RSPI HAL driver */
const spi_on_rspi_cfg_t g_spi0_ext_cfg_t =
{ .rspi_clksyn = RSPI_OPERATION_SPI, .rspi_comm = RSPI_COMMUNICATION_FULL_DUPLEX, .ssl_polarity.rspi_ssl0 =
          RSPI_SSLP_LOW,
  .ssl_polarity.rspi_ssl1 = RSPI_SSLP_LOW, .ssl_polarity.rspi_ssl2 = RSPI_SSLP_LOW, .ssl_polarity.rspi_ssl3 =
          RSPI_SSLP_LOW,
  .loopback.rspi_loopback1 = RSPI_LOOPBACK1_NORMAL_DATA, .loopback.rspi_loopback2 = RSPI_LOOPBACK2_NORMAL_DATA,
  .mosi_idle.rspi_mosi_idle_fixed_val = RSPI_MOSI_IDLE_FIXED_VAL_LOW, .mosi_idle.rspi_mosi_idle_val_fixing =
          RSPI_MOSI_IDLE_VAL_FIXING_DISABLE,
  .parity.rspi_parity = RSPI_PARITY_STATE_DISABLE, .parity.rspi_parity_mode = RSPI_PARITY_MODE_ODD, .ssl_select =
          RSPI_SSL_SELECT_SSL0,
  .ssl_level_keep = RSPI_SSL_LEVEL_KEEP_NOT, .clock_delay.rspi_clock_delay_count = RSPI_CLOCK_DELAY_COUNT_1,
  .clock_delay.rspi_clock_delay_state = RSPI_CLOCK_DELAY_STATE_DISABLE, .ssl_neg_delay.rspi_ssl_neg_delay_count =
          RSPI_SSL_NEGATION_DELAY_1,
  .ssl_neg_delay.rspi_ssl_neg_delay_state = RSPI_SSL_NEGATION_DELAY_DISABLE,
  .access_delay.rspi_next_access_delay_count = RSPI_NEXT_ACCESS_DELAY_COUNT_1,
  .access_delay.rspi_next_access_delay_state = RSPI_NEXT_ACCESS_DELAY_STATE_DISABLE, };

const spi_cfg_t g_spi0_cfg =
{ .channel = 0, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST, .bitrate = 500000,
#define SYNERGY_NOT_DEFINED (1)            
#if (SYNERGY_NOT_DEFINED == SYNERGY_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
  .p_transfer_tx = &SYNERGY_NOT_DEFINED,
#endif
#if (SYNERGY_NOT_DEFINED == SYNERGY_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
  .p_transfer_rx = &SYNERGY_NOT_DEFINED,
#endif
#undef SYNERGY_NOT_DEFINED
  .p_callback = NULL,
  .p_context = (void *) &g_spi0, .p_extend = (void *) &g_spi0_ext_cfg_t, };
/* Instance structure to use this module. */
const spi_instance_t g_spi0 =
{ .p_ctrl = &g_spi0_ctrl, .p_cfg = &g_spi0_cfg, .p_api = &g_spi_on_rspi };

#if spi_on_rspi_CALLBACK_USED_g_spi0
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(spi_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(spi_callback_args_t * p_args);
void NULL_internal(spi_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#ifdef NULL
#define DTC_CALLBACK_USED_g_transfer1 (0)
#else
#define DTC_CALLBACK_USED_g_transfer1 (1)
#endif
#if DTC_CALLBACK_USED_g_transfer1
void NULL(transfer_callback_args_t * p_args);
#endif
transfer_ctrl_t g_transfer1_ctrl;
transfer_info_t g_transfer1_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .repeat_area = TRANSFER_REPEAT_AREA_DESTINATION, .irq =
          TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_FIXED, .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL, .p_dest = (void *) NULL, .p_src = (void const *) NULL, .num_blocks = 0, .length = 0, };
const transfer_cfg_t g_transfer1_cfg =
{ .p_info = &g_transfer1_info, .activation_source = ELC_EVENT_SCI0_RXI, .auto_enable = false, .p_callback = NULL,
  .p_context = &g_transfer1, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer1 =
{ .p_ctrl = &g_transfer1_ctrl, .p_cfg = &g_transfer1_cfg, .p_api = &g_transfer_on_dtc };
#ifdef NULL
#define DTC_CALLBACK_USED_g_transfer0 (0)
#else
#define DTC_CALLBACK_USED_g_transfer0 (1)
#endif
#if DTC_CALLBACK_USED_g_transfer0
void NULL(transfer_callback_args_t * p_args);
#endif
transfer_ctrl_t g_transfer0_ctrl;
transfer_info_t g_transfer0_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED, .repeat_area = TRANSFER_REPEAT_AREA_SOURCE, .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .size =
          TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL, .p_dest = (void *) NULL, .p_src = (void const *) NULL, .num_blocks = 0, .length = 0, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info, .activation_source = ELC_EVENT_SCI0_TXI, .auto_enable = false, .p_callback = NULL,
  .p_context = &g_transfer0, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dtc };
#if UART_ON_SCI_UART_CALLBACK_USED_g_uart0
#if defined(__ICCARM__)
#define UartCallback_WEAK_ATTRIBUTE
#pragma weak UartCallback  = UartCallback_internal
#elif defined(__GNUC__)
#define UartCallback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("UartCallback_internal")))
#endif
void UartCallback(uart_callback_args_t * p_args) UartCallback_WEAK_ATTRIBUTE;
#endif

uart_ctrl_t g_uart0_ctrl;

/** UART extended configuration for UARTonSCI HAL driver */
const uart_on_sci_cfg_t g_uart0_cfg_extend =
{ .clk_src = SCI_CLK_SRC_INT, .baudclk_out = false, .rx_edge_start = true, .noisecancel_en = false, .p_extpin_ctrl =
          (void *) NULL };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 0, .baud_rate = 9600, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits =
          UART_STOP_BITS_1,
  .ctsrts_en = false, .p_callback = UartCallback, .p_context = &g_uart0, .p_extend = &g_uart0_cfg_extend,
#define SYNERGY_NOT_DEFINED (1)                        
#if (SYNERGY_NOT_DEFINED == g_transfer0)
  .p_transfer_tx = NULL,
#else
  .p_transfer_tx = &g_transfer0,
#endif            
#if (SYNERGY_NOT_DEFINED == g_transfer1)
  .p_transfer_rx = NULL,
#else
  .p_transfer_rx = &g_transfer1,
#endif   
#undef SYNERGY_NOT_DEFINED            
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_sci };

#if UART_ON_SCI_UART_CALLBACK_USED_g_uart0
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void UartCallback(uart_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void UartCallback_internal(uart_callback_args_t * p_args);
void UartCallback_internal(uart_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
const cgc_instance_t g_cgc =
{ .p_api = &g_cgc_on_cgc, .p_cfg = NULL };
const elc_instance_t g_elc =
{ .p_api = &g_elc_on_elc, .p_cfg = NULL };
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_cfg = NULL };
void g_hal_init(void)
{
    g_common_init ();
}
