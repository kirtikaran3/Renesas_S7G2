/* generated thread source file - do not edit */
#include "sf_audio_playback_demo_thread.h"

TX_THREAD sf_audio_playback_demo_thread;
void sf_audio_playback_demo_thread_create(void);
static void sf_audio_playback_demo_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t sf_audio_playback_demo_thread_stack[10240] BSP_PLACE_IN_SECTION(".stack.sf_audio_playback_demo_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#if SF_AUDIO_PLAYBACK_ON_SF_AUDIO_PLAYBACK_CALLBACK_USED_g_sf_audio_playback
#if defined(__ICCARM__)
#define audio_playback_complete_callback_WEAK_ATTRIBUTE
#pragma weak audio_playback_complete_callback                            = audio_playback_complete_callback_internal
#elif defined(__GNUC__)
#define audio_playback_complete_callback_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("audio_playback_complete_callback_internal")))
#endif
void audio_playback_complete_callback(sf_message_callback_args_t * p_args) audio_playback_complete_callback_WEAK_ATTRIBUTE;
#endif
sf_audio_playback_ctrl_t g_sf_audio_playback_ctrl;
const sf_audio_playback_cfg_t g_sf_audio_playback_cfg =
{ .p_common_cfg = &g_sf_audio_playback_common_cfg, .p_common_ctrl = &g_sf_audio_playback_common, .class_instance = 0,
  .p_callback = audio_playback_complete_callback, };
const sf_audio_playback_instance_t g_sf_audio_playback =
{ .p_api = &g_sf_audio_playback_on_sf_audio_playback, .p_ctrl = &g_sf_audio_playback_ctrl, .p_cfg =
          &g_sf_audio_playback_cfg, };

#if SF_AUDIO_PLAYBACK_ON_SF_AUDIO_PLAYBACK_CALLBACK_USED_g_sf_audio_playback
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void audio_playback_complete_callback(sf_message_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void audio_playback_complete_callback_internal(sf_message_callback_args_t * p_args)
{
    /** Do nothing. */
}
#endif
TX_SEMAPHORE audio_semaphore;
void sf_audio_playback_demo_thread_create(void)
{
    /* Initialize each kernel object. */
    tx_semaphore_create (&audio_semaphore, (CHAR *) "Audio Semaphore", 2);

    tx_thread_create (&sf_audio_playback_demo_thread, (CHAR *) "Audio Playback demo thread",
                      sf_audio_playback_demo_thread_func, (ULONG) NULL, &sf_audio_playback_demo_thread_stack, 10240, 2,
                      2, 1, TX_AUTO_START);
}

static void sf_audio_playback_demo_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */
    ssp_err_t ssp_err_g_sf_audio_playback;
    ssp_err_g_sf_audio_playback = g_sf_audio_playback.p_api->open (g_sf_audio_playback.p_ctrl,
                                                                   g_sf_audio_playback.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_audio_playback)
    {
        while (1)
            ;
    }

    /* Enter user code for this thread. */
    sf_audio_playback_demo_thread_entry ();
}
