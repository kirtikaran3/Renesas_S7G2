/* generated messaging header file - do not edit */
#ifndef SF_MESSAGE_PAYLOADS_H_
#define SF_MESSAGE_PAYLOADS_H_
#include "sf_audio_playback_api.h"
#include "sf_touch_panel_api.h"
typedef union u_sf_message_payload
{
    sf_audio_playback_data_t sf_audio_playback_data;
    sf_touch_panel_payload_t sf_touch_panel_payload;
} sf_message_payload_t;
#endif /* SF_MESSAGE_PAYLOADS_H_ */
