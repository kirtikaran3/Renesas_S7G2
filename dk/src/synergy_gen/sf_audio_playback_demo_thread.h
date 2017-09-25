/* generated thread header file - do not edit */
#ifndef SF_AUDIO_PLAYBACK_DEMO_THREAD_H_
#define SF_AUDIO_PLAYBACK_DEMO_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void sf_audio_playback_demo_thread_entry(void);
#include "sf_audio_playback.h"
#ifdef __cplusplus
extern "C"
{
#endif
extern sf_audio_playback_common_ctrl_t g_sf_audio_playback_common;
extern const sf_audio_playback_common_cfg_t g_sf_audio_playback_common_cfg;
extern const sf_audio_playback_instance_t g_sf_audio_playback;
/** Pointer to Audio Playback API */
extern const sf_audio_playback_api_t * g_sf_audio_playback_api;
#ifdef audio_playback_complete_callback
#define SF_AUDIO_PLAYBACK_ON_SF_AUDIO_PLAYBACK_CALLBACK_USED_g_sf_audio_playback (0)
#else
#define SF_AUDIO_PLAYBACK_ON_SF_AUDIO_PLAYBACK_CALLBACK_USED_g_sf_audio_playback (1)
#endif
#if SF_AUDIO_PLAYBACK_ON_SF_AUDIO_PLAYBACK_CALLBACK_USED_g_sf_audio_playback
void audio_playback_complete_callback(sf_message_callback_args_t * p_args);
#endif
extern TX_SEMAPHORE audio_semaphore;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SF_AUDIO_PLAYBACK_DEMO_THREAD_H_ */
