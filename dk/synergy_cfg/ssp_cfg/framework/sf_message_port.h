/* generated messaging header file - do not edit */
#ifndef SF_MESSAGE_PORT_H_
#define SF_MESSAGE_PORT_H_
typedef enum e_sf_message_event_class
{
    SF_MESSAGE_EVENT_CLASS_AUDIO, /* Audio Playback */
    SF_MESSAGE_EVENT_CLASS_TOUCH, /* Touch */
} sf_message_event_class_t;
typedef enum e_sf_message_event
{
    SF_MESSAGE_EVENT_UNUSED, /* Unused */
    SF_MESSAGE_EVENT_NEW_DATA, /* New Data */
    SF_MESSAGE_EVENT_AUDIO_START, /* Audio Playback Start */
    SF_MESSAGE_EVENT_AUDIO_STOP, /* Audio Playback Stop */
    SF_MESSAGE_EVENT_AUDIO_PAUSE, /* Audio Playback Pause */
    SF_MESSAGE_EVENT_AUDIO_RESUME, /* Audio Playback Resume */
} sf_message_event_t;
extern TX_QUEUE sf_touch_panel_demo_thread_message_queue;
extern TX_QUEUE sf_audio_playback_demo_thread_message_queue;
#endif /* SF_MESSAGE_PORT_H_ */
