/* generated HAL source file - do not edit */
#include "hal_data.h"
#if TIMER_ON_GPT_CALLBACK_USED_g_timer
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(timer_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
static timer_ctrl_t g_timer_ctrl;
static const timer_on_gpt_cfg_t g_timer_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW } };
static const timer_cfg_t g_timer_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period = 0, .unit = TIMER_UNIT_PERIOD_MSEC, .duty_cycle = 50, .duty_cycle_unit =
          TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 0, .autostart = true, .p_callback = NULL, .p_context = &g_timer, .p_extend = &g_timer_extend };
/* Instance structure to use this module. */
const timer_instance_t g_timer =
{ .p_ctrl = &g_timer_ctrl, .p_cfg = &g_timer_cfg, .p_api = &g_timer_on_gpt };

#if TIMER_ON_GPT_CALLBACK_USED_g_timer
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(timer_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(timer_callback_args_t * p_args);
void NULL_internal(timer_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#ifdef NULL
#define DTC_CALLBACK_USED_g_transfer3 (0)
#else
#define DTC_CALLBACK_USED_g_transfer3 (1)
#endif
#if DTC_CALLBACK_USED_g_transfer3
void NULL(transfer_callback_args_t * p_args);
#endif
transfer_ctrl_t g_transfer3_ctrl;
transfer_info_t g_transfer3_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .repeat_area = TRANSFER_REPEAT_AREA_DESTINATION, .irq =
          TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_FIXED, .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL, .p_dest = (void *) NULL, .p_src = (void const *) NULL, .num_blocks = 0, .length = 0, };
const transfer_cfg_t g_transfer3_cfg =
{ .p_info = &g_transfer3_info, .activation_source = ELC_EVENT_SCI8_RXI, .auto_enable = false, .p_callback = NULL,
  .p_context = &g_transfer3, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer3 =
{ .p_ctrl = &g_transfer3_ctrl, .p_cfg = &g_transfer3_cfg, .p_api = &g_transfer_on_dtc };
#ifdef NULL
#define DTC_CALLBACK_USED_g_transfer2 (0)
#else
#define DTC_CALLBACK_USED_g_transfer2 (1)
#endif
#if DTC_CALLBACK_USED_g_transfer2
void NULL(transfer_callback_args_t * p_args);
#endif
transfer_ctrl_t g_transfer2_ctrl;
transfer_info_t g_transfer2_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED, .repeat_area = TRANSFER_REPEAT_AREA_SOURCE, .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .size =
          TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL, .p_dest = (void *) NULL, .p_src = (void const *) NULL, .num_blocks = 0, .length = 0, };
const transfer_cfg_t g_transfer2_cfg =
{ .p_info = &g_transfer2_info, .activation_source = ELC_EVENT_SCI8_TXI, .auto_enable = false, .p_callback = NULL,
  .p_context = &g_transfer2, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer2 =
{ .p_ctrl = &g_transfer2_ctrl, .p_cfg = &g_transfer2_cfg, .p_api = &g_transfer_on_dtc };
#if SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(spi_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
spi_ctrl_t g_spi0_ctrl;
const spi_cfg_t g_spi0_cfg =
{ .channel = 8, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST, .bitrate = 100000,
#define SYNERGY_NOT_DEFINED (1)             
#if (SYNERGY_NOT_DEFINED == g_transfer2)
  .p_transfer_tx = NULL,
#else
  .p_transfer_tx = &g_transfer2,
#endif
#if (SYNERGY_NOT_DEFINED == g_transfer3)
  .p_transfer_rx = NULL,
#else
  .p_transfer_rx = &g_transfer3,
#endif
#undef SYNERGY_NOT_DEFINED	
  .p_callback = NULL,
  .p_context = (void *) &g_spi0, };
/* Instance structure to use this module. */
const spi_instance_t g_spi0 =
{ .p_ctrl = &g_spi0_ctrl, .p_cfg = &g_spi0_cfg, .p_api = &g_spi_on_sci };

#if SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0
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
#if UART_ON_SCI_UART_CALLBACK_USED_g_uart0
#if defined(__ICCARM__)
#define user_uart_callback_WEAK_ATTRIBUTE
#pragma weak user_uart_callback  = user_uart_callback_internal
#elif defined(__GNUC__)
#define user_uart_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("user_uart_callback_internal")))
#endif
void user_uart_callback(uart_callback_args_t * p_args) user_uart_callback_WEAK_ATTRIBUTE;
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
  .ctsrts_en = false, .p_callback = user_uart_callback, .p_context = &g_uart0, .p_extend = &g_uart0_cfg_extend,
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
    };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_sci };

#if UART_ON_SCI_UART_CALLBACK_USED_g_uart0
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void user_uart_callback(uart_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void user_uart_callback_internal(uart_callback_args_t * p_args);
void user_uart_callback_internal(uart_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
const elc_instance_t g_elc =
{ .p_api = &g_elc_on_elc, .p_cfg = NULL };
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_cfg = NULL };
const cgc_instance_t g_cgc =
{ .p_api = &g_cgc_on_cgc, .p_cfg = NULL };
void g_hal_init(void)
{
    g_common_init ();
}
