/* generated thread source file - do not edit */
#include "blinky_thread.h"

TX_THREAD blinky_thread;
void blinky_thread_create(void);
static void blinky_thread_func(ULONG thread_input);
static uint8_t blinky_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.blinky_thread");;
#if UART_ON_SCI_UART_CALLBACK_USED_g_uart
#if defined(__ICCARM__)
#define user_uart_callback_WEAK_ATTRIBUTE
#pragma weak user_uart_callback  = user_uart_callback_internal
#elif defined(__GNUC__)
#define user_uart_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("user_uart_callback_internal")))
#endif
void user_uart_callback(uart_callback_args_t * p_args) user_uart_callback_WEAK_ATTRIBUTE;
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
  .ctsrts_en = false, .p_callback = user_uart_callback, .p_context = &g_uart, .p_extend = &g_uart_cfg_extend,
#ifdef NULL
  .p_transfer_tx = NULL,
#else
  .p_transfer_tx = &NULL,
#endif
#ifdef NULL
  .p_transfer_rx = NULL,
#else
  .p_transfer_rx = &NULL,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart =
{ .p_ctrl = &g_uart_ctrl, .p_cfg = &g_uart_cfg, .p_api = &g_uart_on_sci };

#if UART_ON_SCI_UART_CALLBACK_USED_g_uart
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
void blinky_thread_create(void)
{
    tx_thread_create (&blinky_thread, (CHAR *) "Blinky Thread", blinky_thread_func, (ULONG) NULL, &blinky_thread_stack,
                      1024, 1, 1, 1, TX_AUTO_START);
}

static void blinky_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each kernel object and module instance. */

    /* Enter user code for this thread. */
    blinky_thread_entry ();
}
