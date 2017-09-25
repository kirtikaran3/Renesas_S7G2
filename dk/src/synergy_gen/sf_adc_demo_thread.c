/* generated thread source file - do not edit */
#include "sf_adc_demo_thread.h"

TX_THREAD sf_adc_demo_thread;
void sf_adc_demo_thread_create(void);
static void sf_adc_demo_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t sf_adc_demo_thread_stack[2048] BSP_PLACE_IN_SECTION(".stack.sf_adc_demo_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#ifdef NULL
#define DTC_CALLBACK_USED_g_transfer_frame (0)
#else
#define DTC_CALLBACK_USED_g_transfer_frame (1)
#endif
#if DTC_CALLBACK_USED_g_transfer_frame
void NULL(transfer_callback_args_t * p_args);
#endif
transfer_ctrl_t g_transfer_frame_ctrl;
transfer_info_t g_transfer_frame_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .repeat_area = TRANSFER_REPEAT_AREA_SOURCE, .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .size =
          TRANSFER_SIZE_2_BYTE,
  .mode = TRANSFER_MODE_BLOCK, .p_dest = (void *) NULL, .p_src = (void const *) NULL, .num_blocks = 1, .length = 1, };
const transfer_cfg_t g_transfer_frame_cfg =
{ .p_info = &g_transfer_frame_info, .activation_source = ELC_EVENT_ELC_SOFTWARE_EVENT_0, .auto_enable = false,
  .p_callback = NULL, .p_context = &g_transfer_frame, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer_frame =
{ .p_ctrl = &g_transfer_frame_ctrl, .p_cfg = &g_transfer_frame_cfg, .p_api = &g_transfer_on_dtc };
#if TIMER_ON_GPT_CALLBACK_USED_g_timer_frame
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(timer_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
static timer_ctrl_t g_timer_frame_ctrl;
static const timer_on_gpt_cfg_t g_timer_frame_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW } };
static const timer_cfg_t g_timer_frame_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period = 10, .unit = TIMER_UNIT_PERIOD_MSEC, .duty_cycle = 50, .duty_cycle_unit =
          TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 0, .autostart = false, .p_callback = NULL, .p_context = &g_timer_frame, .p_extend = &g_timer_frame_extend };
/* Instance structure to use this module. */
const timer_instance_t g_timer_frame =
{ .p_ctrl = &g_timer_frame_ctrl, .p_cfg = &g_timer_frame_cfg, .p_api = &g_timer_on_gpt };

#if TIMER_ON_GPT_CALLBACK_USED_g_timer_frame
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
#if ADC_CALLBACK_USED_g_adc_frame
#if defined(__ICCARM__)
#define sf_adc_periodic_cb_WEAK_ATTRIBUTE
#pragma weak sf_adc_periodic_cb                            = sf_adc_periodic_cb_internal
#elif defined(__GNUC__)
#define sf_adc_periodic_cb_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("sf_adc_periodic_cb_internal")))
#endif
void sf_adc_periodic_cb(adc_callback_args_t * p_args) sf_adc_periodic_cb_WEAK_ATTRIBUTE;
#endif
adc_ctrl_t g_adc_frame_ctrl;
const adc_cfg_t g_adc_frame_cfg =
{ .unit = 0, .mode = ADC_MODE_SINGLE_SCAN, .resolution = ADC_RESOLUTION_8_BIT, .alignment = ADC_ALIGNMENT_RIGHT,
  .add_average_count = ADC_ADD_OFF, .clearing = ADC_CLEAR_AFTER_READ_ON, .trigger = ADC_TRIGGER_SYNC_ELC,
  .trigger_group_b = ADC_TRIGGER_SYNC_ELC, .p_callback = sf_adc_periodic_cb, .p_context = &g_adc_frame, };
const adc_channel_cfg_t g_adc_frame_channel_cfg =
{ .scan_mask = (uint32_t) (
        ((uint64_t) ADC_MASK_CHANNEL_0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | (0)),
  /** Group B channel mask is right shifted by 32 at the end to form the proper mask */
  .scan_mask_group_b = (uint32_t) (
          (((uint64_t) ADC_MASK_CHANNEL_0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | (0)) >> 32),
  .priority_group_a = ADC_GROUP_A_PRIORITY_OFF, .add_mask = (uint32_t) (
          (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0)
                  | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0)),
  .sample_hold_mask = (uint32_t) ((0) | (0) | (0)), .sample_hold_states = 24, };
/* Instance structure to use this module. */
const adc_instance_t g_adc_frame =
{ .p_ctrl = &g_adc_frame_ctrl, .p_cfg = &g_adc_frame_cfg, .p_channel_cfg = &g_adc_frame_channel_cfg, .p_api =
          &g_adc_on_adc };
#if ADC_CALLBACK_USED_g_adc_frame
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void sf_adc_periodic_cb(adc_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void sf_adc_periodic_cb_internal(adc_callback_args_t * p_args);
void sf_adc_periodic_cb_internal(adc_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if ADC_PERIODIC_ON_ADC_PERIODIC_CALLBACK_USED_g_sf_adc_periodic
void g_adc_framework_user_callback(sf_adc_periodic_callback_args_t * p_args);
#endif
uint16_t g_user_buffer[32];
sf_adc_periodic_ctrl_t g_sf_adc_periodic_ctrl;
sf_adc_periodic_cfg_t g_sf_adc_periodic_cfg =
{ .p_lower_lvl_adc = &g_adc_frame, .p_lower_lvl_timer = &g_timer_frame, .p_lower_lvl_transfer = &g_transfer_frame,
  .p_data_buffer = g_user_buffer, .data_buffer_length = 32, .sample_count = 1, .scan_trigger =
          ELC_EVENT_GPT0_COUNTER_OVERFLOW,
  .p_callback = g_adc_framework_user_callback, .p_extend = NULL, };

sf_adc_periodic_instance_t g_sf_adc_periodic =
{ .p_ctrl = &g_sf_adc_periodic_ctrl, .p_cfg = &g_sf_adc_periodic_cfg, .p_api = &g_sf_adc_periodic_on_sf_adc_periodic, };
TX_SEMAPHORE adc_semaphore;
void sf_adc_demo_thread_create(void)
{
    /* Initialize each kernel object. */
    tx_semaphore_create (&adc_semaphore, (CHAR *) "adc_semaphore", 2);

    tx_thread_create (&sf_adc_demo_thread, (CHAR *) "ADC framework demo thread", sf_adc_demo_thread_func, (ULONG) NULL,
                      &sf_adc_demo_thread_stack, 2048, 12, 12, 1, TX_DONT_START);
}

static void sf_adc_demo_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */
    ssp_err_t ssp_err_g_sf_adc_periodic;
    ssp_err_g_sf_adc_periodic = g_sf_adc_periodic.p_api->open (g_sf_adc_periodic.p_ctrl, g_sf_adc_periodic.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_adc_periodic)
    {
        while (1)
            ;
    }

    /* Enter user code for this thread. */
    sf_adc_demo_thread_entry ();
}
