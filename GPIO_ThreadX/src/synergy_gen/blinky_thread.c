/* generated thread source file - do not edit */
#include "blinky_thread.h"

TX_THREAD blinky_thread;
void blinky_thread_create(void);
static void blinky_thread_func(ULONG thread_input);
static uint8_t blinky_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.blinky_thread");;
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq_11
#if defined(__ICCARM__)
#define g_external_irq11_callback_WEAK_ATTRIBUTE
#pragma weak g_external_irq11_callback                            = g_external_irq11_callback_internal
#elif defined(__GNUC__)
#define g_external_irq11_callback_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("g_external_irq11_callback_internal")))
#endif
void g_external_irq11_callback(external_irq_callback_args_t * p_args) g_external_irq11_callback_WEAK_ATTRIBUTE;
#endif
static external_irq_ctrl_t g_external_irq_11_ctrl;
static const external_irq_cfg_t g_external_irq_11_cfg =
{ .channel = 11, .trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE, .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64, .autostart = true, .p_callback = g_external_irq11_callback, .p_context =
          &g_external_irq_11,
  .p_extend = NULL };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq_11 =
{ .p_ctrl = &g_external_irq_11_ctrl, .p_cfg = &g_external_irq_11_cfg, .p_api = &g_external_irq_on_icu };

#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq_11
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void g_external_irq11_callback(external_irq_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void g_external_irq11_callback_internal(external_irq_callback_args_t * p_args);
void g_external_irq11_callback_internal(external_irq_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq
#if defined(__ICCARM__)
#define g_external_irq10_callback_WEAK_ATTRIBUTE
#pragma weak g_external_irq10_callback                            = g_external_irq10_callback_internal
#elif defined(__GNUC__)
#define g_external_irq10_callback_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("g_external_irq10_callback_internal")))
#endif
void g_external_irq10_callback(external_irq_callback_args_t * p_args) g_external_irq10_callback_WEAK_ATTRIBUTE;
#endif
static external_irq_ctrl_t g_external_irq_ctrl;
static const external_irq_cfg_t g_external_irq_cfg =
{ .channel = 10, .trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE, .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64, .autostart = true, .p_callback = g_external_irq10_callback, .p_context =
          &g_external_irq,
  .p_extend = NULL };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq =
{ .p_ctrl = &g_external_irq_ctrl, .p_cfg = &g_external_irq_cfg, .p_api = &g_external_irq_on_icu };

#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void g_external_irq10_callback(external_irq_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void g_external_irq10_callback_internal(external_irq_callback_args_t * p_args);
void g_external_irq10_callback_internal(external_irq_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
sf_external_irq_ctrl_t g_sf_external_irq_ctrl;
const sf_external_irq_cfg_t g_sf_external_irq_cfg =
{ .event = SF_EXTERNAL_IRQ_EVENT_SEMAPHORE_PUT, .p_lower_lvl_irq = &g_external_irq, };
/* Instance structure to use this module. */
const sf_external_irq_instance_t g_sf_external_irq =
{ .p_ctrl = &g_sf_external_irq_ctrl, .p_cfg = &g_sf_external_irq_cfg, .p_api = &g_sf_external_irq_on_sf_external_irq };
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
