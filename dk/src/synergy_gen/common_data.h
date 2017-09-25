/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_i2c_api.h"
#include "sf_i2c.h"
#include "sf_i2c_api.h"
#include "r_dac.h"
#include "r_dac_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "sf_audio_playback_hw_dac.h"
#include "sf_message.h"
#include "sf_message_payloads.h"
#include "sf_audio_playback.h"
#ifdef __cplusplus
extern "C"
{
#endif
extern sf_i2c_bus_t g_sf_i2c_bus;
extern i2c_api_master_t const g_i2c_master_on_sci;
/** DAC on DAC Instance. */
extern const dac_instance_t g_dac;
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer;
#ifdef NULL
#define TIMER_ON_GPT_CALLBACK_USED_g_timer (0)
#else
#define TIMER_ON_GPT_CALLBACK_USED_g_timer (1)
#endif
#if TIMER_ON_GPT_CALLBACK_USED_g_timer
void NULL(timer_callback_args_t * p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer;
extern const sf_audio_playback_hw_instance_t g_sf_audio_playback_hw;
extern void g_message_init(void);

/* SF Message on SF Message Instance. */
extern const sf_message_instance_t g_sf_message;
void g_common_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* COMMON_DATA_H_ */
