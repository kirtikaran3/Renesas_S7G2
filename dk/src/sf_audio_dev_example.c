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
 * File Name    : sf_audio_dev_example.c
 * Description  : TODO
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           TODO       1.00    Initial Release.
 ***********************************************************************************************************************/

#include <dev_example_common.h>
#include "dev_example_thread.h"
#include "sf_audio_playback_demo_thread.h"
#include "fx_api.h"

/***********************************************************************************************************************
Macro definitions
 ***********************************************************************************************************************/

#define PLAY_COMMAND            "play"
#define PAUSE_COMMAND           "pause"
#define RESUME_COMMAND          "resume"
#define STOP_COMMAND            "stop"
#define DIR_COMMAND             "ls"


#define DIR_HELP_STRING         "Display the list of files"
#define PLAY_HELP_STRING        "Play the audio file: play <filename>"
#define PAUSE_HELP_STRING       "Pause audio play back"
#define RESUME_HELP_STRING      "Resume audio play back"
#define STOP_HELP_STRING        "Stop audio play back"


#define PRINT_TIMEOUT           10

/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/
typedef enum
{
    AUDIO_THREAD_STOP,
    AUDIO_THREAD_START,
    AUDIO_THREAD_PAUSE,
    AUDIO_THREAD_RESUME
}audio_thread_ops_t;



/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
/* Prototypes for the callback functions called by Audio console commands */

void dir_callback(sf_console_cb_args_t * p_args);
void play_callback(sf_console_cb_args_t * p_args);
void pause_callback(sf_console_cb_args_t * p_args);
void resume_callback(sf_console_cb_args_t * p_args);
void stop_callback(sf_console_cb_args_t * p_args);

void do_thread_ops(audio_thread_ops_t thread_op);


/***********************************************************************************************************************
Global variables
 ***********************************************************************************************************************/



static CHAR g_file_name[FX_MAX_LONG_NAME_LEN];

int demo(CHAR * filename);

void *param;

extern volatile uint32_t play;
extern volatile uint32_t pause;

const sf_console_command_t g_audio_commands[] =
{

 {
  .command  = (uint8_t*)DIR_COMMAND,
  .help     = (uint8_t*)DIR_HELP_STRING,
  .callback = dir_callback,
  .context  = NULL
 },

 {
  .command  = (uint8_t*)PLAY_COMMAND,
  .help     = (uint8_t*)PLAY_HELP_STRING,
  .callback = play_callback,
  .context  = NULL
 },

 {
  .command  = (uint8_t*)PAUSE_COMMAND,
  .help     = (uint8_t*)PAUSE_HELP_STRING,
  .callback = pause_callback,
  .context  = NULL
 },

 {
  .command  = (uint8_t*)RESUME_COMMAND,
  .help     = (uint8_t*)RESUME_HELP_STRING,
  .callback = resume_callback,
  .context  = NULL
 },

 {
  .command  = (uint8_t*)STOP_COMMAND,
  .help     = (uint8_t*)STOP_HELP_STRING,
  .callback = stop_callback,
  .context  = NULL
 },

};


const sf_console_menu_t g_audio_menu =
{
    .menu_prev = &g_sf_console_root_menu,
    .menu_name = (uint8_t*)"sf_audio_playback",
    .num_commands = (sizeof(g_audio_commands)) / (sizeof(g_audio_commands[0])),
    .command_list = g_audio_commands
};


FX_FILE g_new_file;
/*******************************************************************************************************************//**
 * @brief   Prints the file list in the storage media
 *
 * No arguments provided in CLI
 *  @example dir
 *  @retval VOID
 ***********************************************************************************************************************/
void dir_callback(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    UINT status = FX_SUCCESS;
    status =  fx_directory_first_entry_find(&g_fx_media, g_file_name);
    while (status == FX_SUCCESS)
    {
       /** Find file name, then print it. */
        print_to_console((char *)g_file_name);
        print_to_console("\r\n");

       status =  fx_directory_next_entry_find(&g_fx_media, &g_file_name[0]);
    }
}

/*******************************************************************************************************************//**
 * @brief  Audio playback function .
   @in[]   Audio file.ogg
 **********************************************************************************************************************/

void play_callback(sf_console_cb_args_t * p_args)
{

    param = (void*)p_args->p_remaining_string;

    play  = 1;
    pause = 0;

}

/*******************************************************************************************************************//**
 * @brief  Pause callback function (the Current playing Audio out line) .

 **********************************************************************************************************************/
void pause_callback(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    play  = 1;
    pause = 1;

    print_to_console("Playback paused\r\n");

}



/*******************************************************************************************************************//**
 * @brief  Resume callback function (the Resumes Audio out line ) .

 **********************************************************************************************************************/
void resume_callback(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    play  = 1;
    pause = 0;

    print_to_console("Playback resumed\r\n");

}

/*******************************************************************************************************************//**
 * @brief  Stop callback function (the Stops Audio playing ) .

 **********************************************************************************************************************/
void stop_callback(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    play  = 0;
    pause = 0;

    print_to_console("Playback stopped\r\n");

}

