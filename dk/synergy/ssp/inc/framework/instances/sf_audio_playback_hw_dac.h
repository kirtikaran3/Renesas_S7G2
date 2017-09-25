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
* File Name    : sf_audio_playback_hw_dac.h
* Description  : DAC instance of audio playback abstraction header file.
***********************************************************************************************************************/


#ifndef SF_AUDIO_PLAYBACK_HW_DAC_H
#define SF_AUDIO_PLAYBACK_HW_DAC_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_audio_playback_hw_api.h"
#include "r_timer_api.h"
#include "r_dac_api.h"
#include "r_transfer_api.h"

/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @defgroup SF_AUDIO_PLAYBACK_DAC DAC Audio Playback Framework
 * @brief RTOS-integrated DAC implementation of Audio Playback Interface.
 *
 * The Audio Playback Framework DAC implementation uses a timer to generate events at the sampling frequency,
 * and uses these events to transfer PCM samples to the DAC.
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SF_AUDIO_PLAYBACK_HW_DAC_CODE_VERSION_MAJOR   (1)
#define SF_AUDIO_PLAYBACK_HW_DAC_CODE_VERSION_MINOR   (2)


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/** Hardware dependent control block for DAC audio driver. */
typedef struct st_sf_audio_playback_hw_dac_ctrl
{
    dac_instance_t      const * p_lower_lvl_dac;       ///< DAC API used to access DAC hardware.
    timer_instance_t    const * p_lower_lvl_timer;     ///< Timer API used to generate sampling frequency.
    transfer_instance_t const * p_lower_lvl_transfer;  ///< Transfer API used to transfer data each sampling frequency.
} sf_audio_playback_hw_dac_ctrl_t;

/** Hardware dependent configuration for DAC audio driver. */
typedef struct st_sf_audio_playback_hw_cfg_dac
{
    dac_instance_t      const * p_lower_lvl_dac;       ///< DAC API used to access DAC hardware.
    timer_instance_t    const * p_lower_lvl_timer;     ///< Timer API used to generate sampling frequency.
    transfer_instance_t const * p_lower_lvl_transfer;  ///< Transfer API used to transfer data each sampling frequency.

    /** Pointer to memory allocated for hardware control block.  Initialization not required. */
    sf_audio_playback_hw_dac_ctrl_t * p_ctrl;
} sf_audio_playback_hw_dac_cfg_t;

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

extern sf_audio_playback_hw_api_t g_sf_audio_playback_hw_on_sf_audio_playback_hw_dac;

#endif // SF_AUDIO_PLAYBACK_HW_DAC_H

/*******************************************************************************************************************//**
 * @} (end defgroup SF_AUDIO_PLAYBACK_DAC)
***********************************************************************************************************************/


