/* generated thread source file - do not edit */
#include "blinky_thread.h"

TX_THREAD blinky_thread;
void blinky_thread_create(void);
static void blinky_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t blinky_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.blinky_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
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
sf_comms_ctrl_t g_sf_comms0_ctrl;
static sf_uart_comms_ctrl_t g_sf_comms0_ctrl_extend;
static const sf_uart_comms_cfg_t g_sf_comms0_cfg_extend =
{ .p_lower_lvl_uart = &g_uart0, .p_ctrl = &g_sf_comms0_ctrl_extend, };
static const sf_comms_cfg_t g_sf_comms0_cfg =
{ .p_extend = &g_sf_comms0_cfg_extend, };
/* Instance structure to use this module. */
const sf_comms_instance_t g_sf_comms0 =
{ .p_ctrl = &g_sf_comms0_ctrl, .p_cfg = &g_sf_comms0_cfg, .p_api = &g_sf_comms_on_sf_uart_comms };
void blinky_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&blinky_thread, (CHAR *) "Blinky Thread", blinky_thread_func, (ULONG) NULL, &blinky_thread_stack,
                      1024, 1, 1, 1, TX_AUTO_START);
}

static void blinky_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */
    ssp_err_t ssp_err_g_sf_comms0;
    ssp_err_g_sf_comms0 = g_sf_comms0.p_api->open (g_sf_comms0.p_ctrl, g_sf_comms0.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_comms0)
    {
        while (1)
            ;
    }

    /* Enter user code for this thread. */
    blinky_thread_entry ();
}
