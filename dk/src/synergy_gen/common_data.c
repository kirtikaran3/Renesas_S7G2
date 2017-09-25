/* generated common source file - do not edit */
#include "common_data.h"
static TX_MUTEX sf_bus_mutex_g_sf_i2c_bus;
static TX_EVENT_FLAGS_GROUP sf_bus_eventflag_g_sf_i2c_bus;
static sf_i2c_ctrl_t * sf_curr_ctrl_g_sf_i2c_bus;
sf_i2c_bus_t g_sf_i2c_bus =
{ .p_bus_name = (uint8_t *) "g_sf_i2c_bus", .channel = 7, .p_lock_mutex = &sf_bus_mutex_g_sf_i2c_bus,
  .p_sync_eventflag = &sf_bus_eventflag_g_sf_i2c_bus, .pp_curr_ctrl = (sf_i2c_ctrl_t **) &sf_curr_ctrl_g_sf_i2c_bus,
  .p_lower_lvl_api = (i2c_api_master_t *) &g_i2c_master_on_sci, .device_count = 0, };
dac_ctrl_t g_dac_ctrl;
const dac_cfg_t g_dac_cfg =
{ .channel = 0, .ad_da_synchronized = false, .data_format = DAC_DATA_FORMAT_FLUSH_RIGHT, .output_amplifier_enabled =
          false,
  .p_extend = NULL };
/* Instance structure to use this module. */
const dac_instance_t g_dac =
{ .p_ctrl = &g_dac_ctrl, .p_cfg = &g_dac_cfg, .p_api = &g_dac_on_dac };
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
{ .mode = TIMER_MODE_PERIODIC, .period = 44100, .unit = TIMER_UNIT_FREQUENCY_HZ, .duty_cycle = 50, .duty_cycle_unit =
          TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 1, .autostart = false, .p_callback = NULL, .p_context = &g_timer, .p_extend = &g_timer_extend };
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
#define DTC_CALLBACK_USED_g_transfer (0)
#else
#define DTC_CALLBACK_USED_g_transfer (1)
#endif
#if DTC_CALLBACK_USED_g_transfer
void NULL(transfer_callback_args_t * p_args);
#endif
transfer_ctrl_t g_transfer_ctrl;
transfer_info_t g_transfer_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED, .repeat_area = TRANSFER_REPEAT_AREA_SOURCE, .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .size =
          TRANSFER_SIZE_2_BYTE,
  .mode = TRANSFER_MODE_NORMAL, .p_dest = (void *) &R_DAC->DADRn[0], .p_src = (void const *) NULL, .num_blocks = 0,
  .length = 0, };
const transfer_cfg_t g_transfer_cfg =
{ .p_info = &g_transfer_info, .activation_source = ELC_EVENT_GPT1_COUNTER_OVERFLOW, .auto_enable = false, .p_callback =
          NULL,
  .p_context = &g_transfer, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer =
{ .p_ctrl = &g_transfer_ctrl, .p_cfg = &g_transfer_cfg, .p_api = &g_transfer_on_dtc };
static sf_audio_playback_hw_ctrl_t g_sf_audio_playback_hw_ctrl;
static sf_audio_playback_hw_dac_ctrl_t g_sf_audio_playback_hw_extend;
static const sf_audio_playback_hw_dac_cfg_t g_sf_audio_playback_hw_cfg_extend =
{ .p_lower_lvl_timer = &g_timer, .p_lower_lvl_dac = &g_dac, .p_lower_lvl_transfer = &g_transfer, .p_ctrl =
          &g_sf_audio_playback_hw_extend, };
static const sf_audio_playback_hw_cfg_t g_sf_audio_playback_hw_cfg =
{ .p_extend = &g_sf_audio_playback_hw_cfg_extend };
const sf_audio_playback_hw_instance_t g_sf_audio_playback_hw =
{ .p_api = &g_sf_audio_playback_hw_on_sf_audio_playback_hw_dac, .p_ctrl = &g_sf_audio_playback_hw_ctrl, .p_cfg =
          &g_sf_audio_playback_hw_cfg, };
extern sf_message_subscriber_list_t * p_subscriber_lists[];
sf_message_ctrl_t g_sf_message_ctrl;
static uint8_t g_sf_message_work_buffer[2048];
/* Configures the messaging framework */
sf_message_cfg_t g_sf_message_cfg =
{ .p_work_memory_start = &g_sf_message_work_buffer, .work_memory_size_bytes = 2048, .buffer_size =
          sizeof(sf_message_payload_t),
  .pp_subscriber_lists = p_subscriber_lists, .p_block_pool_name = (uint8_t *) "sf_msg_blk_pool" };
/* Instance structure to use this module. */
const sf_message_instance_t g_sf_message =
{ .p_ctrl = &g_sf_message_ctrl, .p_cfg = &g_sf_message_cfg, .p_api = &g_sf_message_on_sf_message };
extern TX_QUEUE sf_audio_playback_demo_thread_message_queue;
extern const sf_message_instance_t g_sf_message;
sf_audio_playback_common_ctrl_t g_sf_audio_playback_common;
const sf_audio_playback_common_cfg_t g_sf_audio_playback_common_cfg =
{ .p_lower_lvl_hw = &g_sf_audio_playback_hw, .priority = 3, .p_message = &g_sf_message, .p_queue =
          &sf_audio_playback_demo_thread_message_queue, };
void g_common_init(void)
{
    ssp_err_t err_g_sf_message;

    /* Initializes Messaging Framework Queues */
    g_message_init ();

    /* Opens the messaging framework */
    err_g_sf_message = g_sf_message.p_api->open (g_sf_message.p_ctrl, g_sf_message.p_cfg);
    if (SSP_SUCCESS != err_g_sf_message)
    {
        /* Error returns, check the cause. */
        while (1)
            ;
    }
}
