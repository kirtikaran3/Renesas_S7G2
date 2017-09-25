/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
 * display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
 * purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
 * SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
 * NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
 * INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
 * be subject to different terms.
 **********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : sf_audio_playback_demo_thread_entry.c
* Description  : This is the thread which runs in the background and gives back the console for the user to control
*                audio from CLI
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           TODO  1.00    Initial Release
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include "sf_audio_playback_demo_thread.h"
#include "sf_console_api.h"

/***********************************************************************************************************************
Extern global variables
***********************************************************************************************************************/

volatile uint32_t play  = 0;
volatile uint32_t pause = 0;
extern void *param;

extern int demo(CHAR * );
/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief  This function is called as soon as audio_thread is created. This thread which runs in the background and
 * gives back the console for the user to control audio from CLI
 *
 *  @retval VOID
***********************************************************************************************************************/

void sf_audio_playback_demo_thread_entry(void)
{
    while (1)
    {
        /** waits till play command is given in CLI*/
        if(play == 1)
        {
            /**Decodes the audio file which is in ogg vorbis and start playing  audio */
           demo(param);
        }
        tx_thread_sleep (1);
    }
}
