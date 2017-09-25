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
 * File Name    : sf_audio_playback_hw_api.h
 * Description  : Audio Playback Driver Shared Interface definition
 **********************************************************************************************************************/

#ifndef SF_AUDIO_PLAYBACK_HW_API_H
#define SF_AUDIO_PLAYBACK_HW_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "bsp_api.h"

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup SF_AUDIO_PLAYBACK_HW_API Audio Playback Framework Interface
 * @brief RTOS-integrated Audio Playback Framework Interface.
 *
 * @section SF_AUDIO_PLAYBACK_HW_API_SUMMARY Summary
 * Audio playback driver to play buffers of audio data.
 *
 * Implemented by:
 * @ref SF_AUDIO_PLAYBACK_DAC
 *
 * Audio Framework Interface description: @ref FrameworkAudioModule
 *
 * @{
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_AUDIO_PLAYBACK_HW_API_VERSION_MAJOR (1)
#define SF_AUDIO_PLAYBACK_HW_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Audio data type. */
typedef struct st_sf_audio_playback_hw_data_type
{
    uint8_t  scale_bits_max;       ///< Maximum data resolution in bits
    bool     is_signed;            ///< Set to 1 for signed samples, or 0 for unsigned samples
} sf_audio_playback_data_type_t;

/** Callback function parameter data */
typedef struct st_sf_audio_playback_hw_callback_args
{
    /** Placeholder for user data.  Set in sf_audio_playback_hw_api_t::open function in ::sf_audio_playback_hw_cfg_t. */
    void           * p_context;
} sf_audio_playback_hw_callback_args_t;

/** Audio playback driver configuration. */
typedef struct st_sf_audio_playback_hw_cfg
{
    /** Callback called when play is complete.  Set to NULL for no callback. */
    void (* p_callback)(sf_audio_playback_hw_callback_args_t * p_args);

    /** Placeholder for user data.  Passed to the user callback in ::sf_audio_playback_hw_callback_args_t. */
    void       * p_context;
    void const * p_extend;                       ///< Hardware dependent configuration
} sf_audio_playback_hw_cfg_t;

/** Interface control block */
typedef struct st_sf_audio_playback_hw_ctrl
{
    /** Callback called when play is complete. */
    void (* p_callback)(sf_audio_playback_hw_callback_args_t * p_args);

    /** Placeholder for user data.  Passed to the user callback in ::sf_audio_playback_hw_callback_args_t. */
    void      * p_context;
    void      * p_extend;                       ///< Hardware dependent control block
} sf_audio_playback_hw_ctrl_t;

/** Audio playback API definition. */
typedef struct st_sf_audio_playback_hw_api
{
    /** Open a device channel for read/write and control.
     * @par Implemented as
     * - SF_AUDIO_PLAYBACK_HW_DAC_Open()
     *
     * @param[in,out] p_ctrl    Pointer to memory allocated for control block.
     * @param[in]     p_cfg     Pointer to the hardware configurations.
     */
    ssp_err_t (* open)(sf_audio_playback_hw_ctrl_t      * const p_ctrl,
                       sf_audio_playback_hw_cfg_t const * const p_cfg);

    /** Start audio playback hardware.
     * @par Implemented as
     * - SF_AUDIO_PLAYBACK_HW_DAC_Start()
     *
     * @param[in] p_ctrl        Pointer to control block initialized in sf_audio_playback_hw_api_t::open.
     */
    ssp_err_t (* start)(sf_audio_playback_hw_ctrl_t * const p_ctrl);

    /** Stop audio playback hardware.
     * @par Implemented as
     * - SF_AUDIO_PLAYBACK_HW_DAC_Stop()
     *
     * @param[in] p_ctrl        Pointer to control block initialized in sf_audio_playback_hw_api_t::open.
     */
    ssp_err_t (* stop)(sf_audio_playback_hw_ctrl_t * const p_ctrl);

    /** Play audio buffer.
     * @par Implemented as
     * - SF_AUDIO_PLAYBACK_HW_DAC_Play()
     *
     * @param[in] p_ctrl        Pointer to control block initialized in sf_audio_playback_hw_api_t::open.
     * @param[in] p_buffer      Pointer to buffer with PCM samples to play.  Data must be scaled for audio
     *                          playback hardware.
     * @param[in] length        Length of data in p_buffer.
     */
    ssp_err_t (* play)(sf_audio_playback_hw_ctrl_t * const p_ctrl,
                       int16_t         const * const p_buffer,
                       uint32_t                      length);

    /** Stores expected data type in provided pointer p_data_type.
     * @par Implemented as
     * - SF_AUDIO_PLAYBACK_HW_DAC_DataTypeGet()
     *
     * @param[in]  p_ctrl        Pointer to control block initialized in sf_audio_playback_hw_api_t::open.
     * @param[out] p_data_type   Pointer to audio sample data type required by hardware.
     */
    ssp_err_t (* dataTypeGet)(sf_audio_playback_hw_ctrl_t      * const p_ctrl,
                              sf_audio_playback_data_type_t    * const p_data_type);

    /** Close the audio driver.
     * @par Implemented as
     * - SF_AUDIO_PLAYBACK_HW_DAC_Close()
     *
     * @param[in] p_ctrl        Pointer to control block initialized in sf_audio_playback_hw_api_t::open.
     */
    ssp_err_t (* close)(sf_audio_playback_hw_ctrl_t * const p_ctrl);

    /** Return the version of the driver.
     * @par Implemented as
     * - SF_AUDIO_PLAYBACK_HW_DAC_VersionGet()
     *
     * @param[out]  p_version   Pointer to variable that will be populated with version information.
     */
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);
} sf_audio_playback_hw_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sf_audio_playback_hw_instance
{
    sf_audio_playback_hw_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    sf_audio_playback_hw_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    sf_audio_playback_hw_api_t const * p_api;     ///< Pointer to the API structure for this instance
} sf_audio_playback_hw_instance_t;

/**********************************************************************************************************************
 * Public Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @} (end defgroup SF_AUDIO_PLAYBACK_HW_API)
 **********************************************************************************************************************/
#endif /* SF_AUDIO_PLAYBACK_HW_API_H */
