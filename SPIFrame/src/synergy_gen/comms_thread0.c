/* generated thread source file - do not edit */
#include "comms_thread0.h"

TX_THREAD comms_thread0;
void comms_thread0_create(void);
static void comms_thread0_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t comms_thread0_stack[1024] BSP_PLACE_IN_SECTION(".stack.comms_thread0") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
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
{ .p_info = &g_transfer1_info, .activation_source = ELC_EVENT_SCI8_RXI, .auto_enable = false, .p_callback = NULL,
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
{ .p_info = &g_transfer0_info, .activation_source = ELC_EVENT_SCI8_TXI, .auto_enable = false, .p_callback = NULL,
  .p_context = &g_transfer0, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dtc };
#if SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0
#if defined(__ICCARM__)
#define user_spi_callback_WEAK_ATTRIBUTE
#pragma weak user_spi_callback                            = user_spi_callback_internal
#elif defined(__GNUC__)
#define user_spi_callback_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("user_spi_callback_internal")))
#endif
void user_spi_callback(spi_callback_args_t * p_args) user_spi_callback_WEAK_ATTRIBUTE;
#endif
spi_ctrl_t g_spi0_ctrl;
const spi_cfg_t g_spi0_cfg =
{ .channel = 8, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST, .bitrate = 100000,
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
  .p_callback = user_spi_callback,
  .p_context = (void *) &g_spi0, };
/* Instance structure to use this module. */
const spi_instance_t g_spi0 =
{ .p_ctrl = &g_spi0_ctrl, .p_cfg = &g_spi0_cfg, .p_api = &g_spi_on_sci };

#if SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void user_spi_callback(spi_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void user_spi_callback_internal(spi_callback_args_t * p_args);
void user_spi_callback_internal(spi_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
sf_spi_ctrl_t g_sf_spi_device0_ctrl;
const sf_spi_cfg_t g_sf_spi_device0_cfg =
{ .p_bus = (sf_spi_bus_t *) &g_sf_spi_bus0, .chip_select = IOPORT_PORT_00_PIN_00, .chip_select_level_active =
          IOPORT_LEVEL_LOW,
  .p_lower_lvl_cfg = &g_spi0_cfg, };
/* Instance structure to use this module. */
const sf_spi_instance_t g_sf_spi_device0 =
{ .p_ctrl = &g_sf_spi_device0_ctrl, .p_cfg = &g_sf_spi_device0_cfg, .p_api = &g_sf_spi_on_sf_spi };
sf_comms_ctrl_t g_sf_comms0_ctrl;
sf_el_ux_comms_on_comms_ctrl_t g_sf_comms0_ctrl_extend;
sf_el_ux_comms_on_comms_cfg_t g_sf_comms0_cfg_extend =
{ .p_ctrl = &g_sf_comms0_ctrl_extend, };
const sf_comms_cfg_t g_sf_comms0_cfg =
{ .p_extend = &g_sf_comms0_cfg_extend };
/* Instance structure to use this module. */
const sf_comms_instance_t g_sf_comms0 =
{ .p_ctrl = &g_sf_comms0_ctrl, .p_cfg = &g_sf_comms0_cfg, .p_api = &g_sf_el_ux_comms_on_sf_comms };
void comms_thread0_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&comms_thread0, (CHAR *) "Comms Thread", comms_thread0_func, (ULONG) NULL, &comms_thread0_stack,
                      1024, 20, 20, 10, TX_AUTO_START);
}

static void comms_thread0_func(ULONG thread_input)
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
    comms_thread0_entry ();
}
