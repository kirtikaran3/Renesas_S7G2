/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : sf_message_port_ref.h
 * Description  : SSP message framework static configuration
 ***********************************************************************************************************************/

#ifndef SF_MESSAGE_PORT_H
#define SF_MESSAGE_PORT_H

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** --- Messaging framework event class code definition --- */
/*  You can port the following enumeration to match to your system. */
typedef enum
{
    SF_MESSAGE_EVENT_CLASS_NONE = 0,

    /** Standard classes. */
    SF_MESSAGE_EVENT_CLASS_PM_REQ,
    SF_MESSAGE_EVENT_CLASS_PM_EXE,
    SF_MESSAGE_EVENT_CLASS_INPUT,
    SF_MESSAGE_EVENT_CLASS_TIME,
    SF_MESSAGE_EVENT_CLASS_AUDIO,
    SF_MESSAGE_EVENT_CLASS_TOUCH,
    SF_MESSAGE_EVENT_CLASS_ERROR_LOG,
    SF_MESSAGE_EVENT_CLASS_DATA,

    /** Add user classes here. */
    SF_MESSAGE_EVENT_CLASS_USER,
} sf_message_event_class_t;

/** --- Messaging framework event code definition --- */
/*  You can port the following enumeration to match to your system. */
typedef enum
{
    SF_MESSAGE_EVENT_NONE = 0,

    /** Generic action */
    SF_MESSAGE_EVENT_START,
    SF_MESSAGE_EVENT_STOP,
    SF_MESSAGE_EVENT_WRITE,
    SF_MESSAGE_EVENT_READ,

    /** Power management action */
    SF_MESSAGE_EVENT_POWER_UP,
    SF_MESSAGE_EVENT_POWER_DOWN,

    /** Input action */
    SF_MESSAGE_EVENT_DOWN,
    SF_MESSAGE_EVENT_UP,
    SF_MESSAGE_EVENT_MOVE,

    /** Synergy touch framework events */
    SF_MESSAGE_EVENT_NEW_DATA,

    /** Synergy audio framework events */
    SF_MESSAGE_EVENT_AUDIO_START,
    SF_MESSAGE_EVENT_AUDIO_PAUSE,
    SF_MESSAGE_EVENT_AUDIO_RESUME,
    SF_MESSAGE_EVENT_AUDIO_STOP,
    SF_MESSAGE_EVENT_AUDIO_VOLUME_SET,

    /** Add user events here. */
    SF_MESSAGE_EVENT_USER,
} sf_message_event_t;

#endif /* SF_MESSAGE_PORT_H */
