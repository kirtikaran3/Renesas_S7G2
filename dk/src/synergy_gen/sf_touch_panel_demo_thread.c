/* generated thread source file - do not edit */
#include "sf_touch_panel_demo_thread.h"

TX_THREAD sf_touch_panel_demo_thread;
void sf_touch_panel_demo_thread_create(void);
static void sf_touch_panel_demo_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t sf_touch_panel_demo_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.sf_touch_panel_demo_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_touch_irq
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(external_irq_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
static external_irq_ctrl_t g_external_touch_irq_ctrl;
static const external_irq_cfg_t g_external_touch_irq_cfg =
{ .channel = 7, .trigger = EXTERNAL_IRQ_TRIG_FALLING, .filter_enable = true, .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_1,
  .autostart = true, .p_callback = NULL, .p_context = &g_external_touch_irq, .p_extend = NULL };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_touch_irq =
{ .p_ctrl = &g_external_touch_irq_ctrl, .p_cfg = &g_external_touch_irq_cfg, .p_api = &g_external_irq_on_icu };

#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_touch_irq
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(external_irq_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(external_irq_callback_args_t * p_args);
void NULL_internal(external_irq_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
sf_external_irq_ctrl_t g_sf_external_touch_irq_ctrl;
const sf_external_irq_cfg_t g_sf_external_touch_irq_cfg =
{ .event = SF_EXTERNAL_IRQ_EVENT_SEMAPHORE_PUT, .p_lower_lvl_irq = &g_external_touch_irq, };
/* Instance structure to use this module. */
const sf_external_irq_instance_t g_sf_external_touch_irq =
{ .p_ctrl = &g_sf_external_touch_irq_ctrl, .p_cfg = &g_sf_external_touch_irq_cfg, .p_api =
          &g_sf_external_irq_on_sf_external_irq };
#if I2C_ON_SCI_CALLBACK_USED_g_i2c_touch_panel
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(i2c_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
i2c_ctrl_t g_i2c_touch_panel_ctrl;
const i2c_cfg_t g_i2c_touch_panel_cfg =
{ .channel = 7, .rate = I2C_RATE_STANDARD, .slave = 0x48, .addr_mode = I2C_ADDR_MODE_7BIT, .sda_delay = 300,
  .p_callback = NULL, .p_context = (void *) &g_i2c_touch_panel, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_touch_panel =
{ .p_ctrl = &g_i2c_touch_panel_ctrl, .p_cfg = &g_i2c_touch_panel_cfg, .p_api = &g_i2c_master_on_sci };

#if I2C_ON_SCI_CALLBACK_USED_g_i2c_touch_panel
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
static sf_touch_panel_i2c_ctrl_t g_sf_touch_panel_i2c_extend;
sf_touch_panel_ctrl_t g_sf_touch_panel_i2c_ctrl;
const sf_touch_panel_i2c_cfg_t g_sf_touch_panel_i2c_cfg_extend =
{ .p_lower_lvl_i2c = &g_i2c_touch_panel, .p_lower_lvl_ctrl = &g_sf_touch_panel_i2c_extend, .p_lower_lvl_irq =
          &g_sf_external_touch_irq,
  .pin = IOPORT_PORT_07_PIN_11, .p_chip = &g_sf_touch_panel_i2c_chip_sx8654, };
const sf_touch_panel_cfg_t g_sf_touch_panel_i2c_cfg =
{ .hsize_pixels = 800, .vsize_pixels = 480, .priority = 8, .update_hz = 10, .p_message = &g_sf_message, .p_extend =
          &g_sf_touch_panel_i2c_cfg_extend,
  .event_class_instance = 0 };
/* Instance structure to use this module. */
const sf_touch_panel_instance_t g_sf_touch_panel_i2c =
{ .p_ctrl = &g_sf_touch_panel_i2c_ctrl, .p_cfg = &g_sf_touch_panel_i2c_cfg, .p_api =
          &g_sf_touch_panel_on_sf_touch_panel_i2c };
void sf_touch_panel_demo_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&sf_touch_panel_demo_thread, (CHAR *) "Touch panel framework demo thread",
                      sf_touch_panel_demo_thread_func, (ULONG) NULL, &sf_touch_panel_demo_thread_stack, 1024, 5, 5, 1,
                      TX_DONT_START);
}

static void sf_touch_panel_demo_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */
    ssp_err_t ssp_err_g_sf_touch_panel_i2c;
    ssp_err_g_sf_touch_panel_i2c = g_sf_touch_panel_i2c.p_api->open (g_sf_touch_panel_i2c.p_ctrl,
                                                                     g_sf_touch_panel_i2c.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_touch_panel_i2c)
    {
        while (1)
            ;
    }

    /* Enter user code for this thread. */
    sf_touch_panel_demo_thread_entry ();
}
