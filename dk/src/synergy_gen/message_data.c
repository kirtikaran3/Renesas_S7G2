/* generated messaging source file - do not edit */
#include "sf_message.h"
#ifndef SF_MESSAGE_CFG_QUEUE_SIZE
#define SF_MESSAGE_CFG_QUEUE_SIZE (16)
#endif
TX_QUEUE sf_touch_panel_demo_thread_message_queue;
static uint8_t queue_memory_sf_touch_panel_demo_thread_message_queue[SF_MESSAGE_CFG_QUEUE_SIZE];
TX_QUEUE sf_audio_playback_demo_thread_message_queue;
static uint8_t queue_memory_sf_audio_playback_demo_thread_message_queue[SF_MESSAGE_CFG_QUEUE_SIZE];
static sf_message_subscriber_t sf_touch_panel_demo_thread_message_queue_0_255 =
{ .p_queue = &sf_touch_panel_demo_thread_message_queue, .instance_range =
{ .start = 0, .end = 255 } };
static sf_message_subscriber_t sf_audio_playback_demo_thread_message_queue_0_255 =
{ .p_queue = &sf_audio_playback_demo_thread_message_queue, .instance_range =
{ .start = 0, .end = 255 } };
static sf_message_subscriber_t * gp_group_SF_MESSAGE_EVENT_CLASS_AUDIO[] =
{ &sf_audio_playback_demo_thread_message_queue_0_255, };
static sf_message_subscriber_list_t g_list_SF_MESSAGE_EVENT_CLASS_AUDIO =
{ .event_class = (uint16_t) SF_MESSAGE_EVENT_CLASS_AUDIO, .number_of_nodes = 1, .pp_subscriber_group =
          gp_group_SF_MESSAGE_EVENT_CLASS_AUDIO };
static sf_message_subscriber_t * gp_group_SF_MESSAGE_EVENT_CLASS_TOUCH[] =
{ &sf_touch_panel_demo_thread_message_queue_0_255, };
static sf_message_subscriber_list_t g_list_SF_MESSAGE_EVENT_CLASS_TOUCH =
{ .event_class = (uint16_t) SF_MESSAGE_EVENT_CLASS_TOUCH, .number_of_nodes = 1, .pp_subscriber_group =
          gp_group_SF_MESSAGE_EVENT_CLASS_TOUCH };
sf_message_subscriber_list_t * p_subscriber_lists[] =
{ &g_list_SF_MESSAGE_EVENT_CLASS_AUDIO, &g_list_SF_MESSAGE_EVENT_CLASS_TOUCH, NULL };
void g_message_init(void);
void g_message_init(void)
{
    tx_queue_create (&sf_touch_panel_demo_thread_message_queue,
                     (CHAR *) "Touch panel framework demo thread Message Queue", 1,
                     &queue_memory_sf_touch_panel_demo_thread_message_queue,
                     sizeof(queue_memory_sf_touch_panel_demo_thread_message_queue));
    tx_queue_create (&sf_audio_playback_demo_thread_message_queue, (CHAR *) "Audio Playback demo thread Message Queue",
                     1, &queue_memory_sf_audio_playback_demo_thread_message_queue,
                     sizeof(queue_memory_sf_audio_playback_demo_thread_message_queue));
}
