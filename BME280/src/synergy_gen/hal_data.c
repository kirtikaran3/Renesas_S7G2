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
#if I2C_ON_SCI_CALLBACK_USED_g_i2c
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(i2c_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
i2c_ctrl_t g_i2c_ctrl;
const i2c_cfg_t g_i2c_cfg =
{ .channel = 4, .rate = I2C_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_ADDR_MODE_7BIT, .p_callback = NULL,
  .p_context = (void *) &g_i2c, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c =
{ .p_ctrl = &g_i2c_ctrl, .p_cfg = &g_i2c_cfg, .p_api = &g_i2c_master_on_sci };

#if I2C_ON_SCI_CALLBACK_USED_g_i2c
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(i2c_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(i2c_callback_args_t * p_args);
void NULL_internal(i2c_callback_args_t * p_args)
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
#if UART_ON_SCI_UART_CALLBACK_USED_g_uart
#if defined(__ICCARM__)
#define UartCallback_WEAK_ATTRIBUTE
#pragma weak UartCallback  = UartCallback_internal
#elif defined(__GNUC__)
#define UartCallback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("UartCallback_internal")))
#endif
void UartCallback(uart_callback_args_t * p_args) UartCallback_WEAK_ATTRIBUTE;
#endif

uart_ctrl_t g_uart_ctrl;

/** UART extended configuration for UARTonSCI HAL driver */
const uart_on_sci_cfg_t g_uart_cfg_extend =
{ .clk_src = SCI_CLK_SRC_INT, .baudclk_out = false, .rx_edge_start = true, .noisecancel_en = false, .p_extpin_ctrl =
          (void *) NULL };

/** UART interface configuration */
const uart_cfg_t g_uart_cfg =
{ .channel = 0, .baud_rate = 9600, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits =
          UART_STOP_BITS_1,
  .ctsrts_en = false, .p_callback = UartCallback, .p_context = &g_uart, .p_extend = &g_uart_cfg_extend,
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
const uart_instance_t g_uart =
{ .p_ctrl = &g_uart_ctrl, .p_cfg = &g_uart_cfg, .p_api = &g_uart_on_sci };

#if UART_ON_SCI_UART_CALLBACK_USED_g_uart
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
