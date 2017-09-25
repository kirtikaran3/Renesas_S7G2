/* generated thread source file - do not edit */
#include "input_thread.h"

TX_THREAD input_thread;
void input_thread_create(void);
static void input_thread_func(ULONG thread_input);
static uint8_t input_thread_stack[512] BSP_PLACE_IN_SECTION(".stack.input_thread");;
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw5_irq
#if defined(__ICCARM__)
#define g_sw5_irq_callback_WEAK_ATTRIBUTE
#pragma weak g_sw5_irq_callback                            = g_sw5_irq_callback_internal
#elif defined(__GNUC__)
#define g_sw5_irq_callback_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("g_sw5_irq_callback_internal")))
#endif
void g_sw5_irq_callback(external_irq_callback_args_t * p_args) g_sw5_irq_callback_WEAK_ATTRIBUTE;
#endif
static external_irq_ctrl_t g_sw5_irq_ctrl;
static const external_irq_cfg_t g_sw5_irq_cfg =
{ .channel = 10, .trigger = EXTERNAL_IRQ_TRIG_FALLING, .filter_enable = true, .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true, .p_callback = g_sw5_irq_callback, .p_context = &g_sw5_irq, .p_extend = NULL };
/* Instance structure to use this module. */
const external_irq_instance_t g_sw5_irq =
{ .p_ctrl = &g_sw5_irq_ctrl, .p_cfg = &g_sw5_irq_cfg, .p_api = &g_external_irq_on_icu };

#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw5_irq
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void g_sw5_irq_callback(external_irq_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void g_sw5_irq_callback_internal(external_irq_callback_args_t * p_args);
void g_sw5_irq_callback_internal(external_irq_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw4_irq
#if defined(__ICCARM__)
#define g_sw4_irq_callback_WEAK_ATTRIBUTE
#pragma weak g_sw4_irq_callback                            = g_sw4_irq_callback_internal
#elif defined(__GNUC__)
#define g_sw4_irq_callback_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("g_sw4_irq_callback_internal")))
#endif
void g_sw4_irq_callback(external_irq_callback_args_t * p_args) g_sw4_irq_callback_WEAK_ATTRIBUTE;
#endif
static external_irq_ctrl_t g_sw4_irq_ctrl;
static const external_irq_cfg_t g_sw4_irq_cfg =
{ .channel = 11, .trigger = EXTERNAL_IRQ_TRIG_FALLING, .filter_enable = true, .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true, .p_callback = g_sw4_irq_callback, .p_context = &g_sw4_irq, .p_extend = NULL };
/* Instance structure to use this module. */
const external_irq_instance_t g_sw4_irq =
{ .p_ctrl = &g_sw4_irq_ctrl, .p_cfg = &g_sw4_irq_cfg, .p_api = &g_external_irq_on_icu };

#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw4_irq
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void g_sw4_irq_callback(external_irq_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void g_sw4_irq_callback_internal(external_irq_callback_args_t * p_args);
void g_sw4_irq_callback_internal(external_irq_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if ADC_CALLBACK_USED_g_temp_adc
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(adc_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
adc_ctrl_t g_temp_adc_ctrl;
const adc_cfg_t g_temp_adc_cfg =
{ .unit = 0, .mode = ADC_MODE_CONTINUOUS_SCAN, .resolution = ADC_RESOLUTION_12_BIT, .alignment = ADC_ALIGNMENT_RIGHT,
  .add_average_count = ADC_ADD_OFF, .clearing = ADC_CLEAR_AFTER_READ_ON, .trigger = ADC_TRIGGER_SOFTWARE,
  .trigger_group_b = ADC_TRIGGER_SYNC_ELC, .p_callback = NULL, .p_context = &g_temp_adc, };
const adc_channel_cfg_t g_temp_adc_channel_cfg =
{ .scan_mask = (uint32_t) (
        ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) ADC_MASK_TEMPERATURE)
                | (0)),
  /** Group B channel mask is right shifted by 32 at the end to form the proper mask */
  .scan_mask_group_b = (uint32_t) (
          (((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) ADC_MASK_TEMPERATURE) | (0)) >> 32),
  .priority_group_a = ADC_GROUP_A_PRIORITY_OFF, .add_mask = (uint32_t) (
          (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0)),
  .sample_hold_mask = (uint32_t) ((0) | (0) | (0)), .sample_hold_states = 24, };
/* Instance structure to use this module. */
const adc_instance_t g_temp_adc =
        { .p_ctrl = &g_temp_adc_ctrl, .p_cfg = &g_temp_adc_cfg, .p_channel_cfg = &g_temp_adc_channel_cfg, .p_api =
                  &g_adc_on_adc };
#if ADC_CALLBACK_USED_g_temp_adc
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL_internal(adc_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(adc_callback_args_t * p_args);
void NULL_internal(adc_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
void input_thread_create(void)
{
    tx_thread_create (&input_thread, (CHAR *) "Input Thread", input_thread_func, (ULONG) NULL, &input_thread_stack, 512,
                      7, 7, 1, TX_DONT_START);
}

static void input_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each kernel object and module instance. */

    /* Enter user code for this thread. */
    input_thread_entry ();
}
