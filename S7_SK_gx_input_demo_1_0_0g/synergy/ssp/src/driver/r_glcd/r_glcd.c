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
 * File Name    : r_glcd.c
 * Description  : Graphics LCD HAL driver
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_glcd.h"
#include "r_glcd_cfg.h"
#include "r_glcd_private.h"
#include "r_glcd_private_api.h"

#include "./hw/common/hw_glcd_common.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#ifndef GLCD_ERROR_RETURN
#define GLCD_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), "glcd", &module_version)
#endif

/** The macro to use for 64-byte alignment checking, calculation */
#define GLCD_ADDRESS_ALIGNMENT_64B (64)

/** This enables the GLCD to locate the foreground/background layer image, at the physical start of the left side of the
 *  display, if the offset of layer start position is a negative value, compared to the active video area.
 *  Shifting the base address of the layer image can be used in this operation, but it must be aligned to 64 byte
 * boundary,
 *  so that the layer position adjustment can be made.
 *
 */
#define OFFSET_MARGIN_MINUS_64PIX (64)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** RGB color order select */
typedef enum e_display_plane_blend
{
    DISPLAY_PLANE_BLEND_TRANSPARENT     = 1,           ///< Current graphics layer is transparent and the lower layer is
                                                       // displayed
    DISPLAY_PLANE_BLEND_NON_TRANSPARENT = 2,           ///< Current graphics layer is displayed
    DISPLAY_PLANE_BLEND_ON_LOWER_LAYER  = 3            ///< Current graphics layer is blended with the lower layer
} display_plane_blend_t;

/** RGB color order select */
typedef enum e_glcd_fading_control_initial_alpha
{
    GLCD_FADING_CONTROL_INITIAL_ALPHA_MIN = 0,         ///< Initial alpha value setting for a graphics plane is zero
    GLCD_FADING_CONTROL_INITIAL_ALPHA_MAX = 0xff       ///< Initial alpha value setting for a graphics plane is maximum
} glcd_fading_control_initial_alpha_t;

/** Function pointers to control TCON pin settings */
typedef struct st_tcon_func
{
    void (* tcon_select)(glcd_tcon_signal_select_t);
    void (* invert)(void);
} tcon_func_t;

/** The structure for the layer parameter recalculation */
typedef struct st_recalculated_param
{
    uint16_t  hpix_size;
    uint16_t  vpix_size;
    int16_t   hpix_offset;  /* Offset can be signed value */
    int16_t   vpix_offset;
    uint32_t  hread_size;
    uint32_t  base_address;
} recalculated_param_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void r_glcd_sync_signal_set (display_cfg_t const * const p_cfg);

static void r_glcd_backgrpund_screen_set (display_cfg_t const * const p_cfg);

static void r_glcd_grahics_layer_set (display_input_cfg_t const * const p_input,
                                      display_layer_t const * const     p_layer,
                                      display_frame_layer_t             frame);

static void r_glcd_output_block_set (display_cfg_t const * const p_cfg);

static void r_glcd_gamma_correction (display_cfg_t const * const p_cfg);

static void r_glcd_hsync_set (glcd_tcon_pin_t tcon, display_timing_t const * timing, uint16_t zero_offset);

static void r_glcd_vsync_set (glcd_tcon_pin_t tcon, display_timing_t const * const timing);

static void r_glcd_data_enable_set (glcd_tcon_pin_t const           tcon,
                                    display_timing_t const * const  vtiming,
                                    display_timing_t const * const  htiming,
                                    display_signal_polarity_t const polarity,
                                    uint16_t                        zero_offset);

static void      r_glcd_clock_set (display_cfg_t const * const p_cfg);

static void      r_glcd_brightness_correction (display_ctrl_t const * const       p_ctrl,
                                               display_brightness_t const * const p_brightness);

static void      r_glcd_contrast_correction (display_ctrl_t const * const     p_ctrl,
                                             display_contrast_t const * const p_contrast);

#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
static ssp_err_t r_glcd_open_param_check (display_cfg_t const * const p_cfg);
#endif

static uint16_t  r_glcd_get_bit_size (display_in_format_t const format);

static void      r_glcd_graphics_plane_format_set (display_in_format_t const format, display_frame_layer_t const frame);

static uint16_t  r_glcd_calc_output_horizontal_pixel_offset (display_cfg_t const * const p_cfg);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/** GLCD HAL module version data structure */
static const ssp_version_t module_version =
{
    .api_version_minor  = DISPLAY_API_VERSION_MINOR,
    .api_version_major  = DISPLAY_API_VERSION_MAJOR,
    .code_version_major = GLCD_CODE_VERSION_MAJOR,
    .code_version_minor = GLCD_CODE_VERSION_MINOR
};

/** GLCD HAL module API function pointer list */
const display_api_t g_display_on_glcd =
{
    .open        = R_GLCD_Open,
    .close       = R_GLCD_Close,
    .start       = R_GLCD_Start,
    .stop        = R_GLCD_Stop,
    .layerChange = R_GLCD_LayerChange,
    .clut        = R_GLCD_ClutUpdate,
    .correction  = R_GLCD_ColorCorrection,
    .statusGet   = R_GLCD_StatusGet,
    .versionGet  = R_GLCD_VersionGet
};

/** Default setting of GLCD specific configuration */
static const glcd_cfg_t default_glcd_cfg =
{
    GLCD_TCON_PIN_0,                                      /**< Use TCON pin0 for Hsync */
    GLCD_TCON_PIN_1,                                      /**< Use TCON pin1 for Vsync */
    GLCD_TCON_PIN_2,                                      /**< Use TCON pin2 for DE    */
    GLCD_CORRECTION_PROC_ORDER_BRIGHTNESS_CONTRAST2GAMMA, /**< Color correction order */
    GLCD_CLK_SRC_INTERNAL,                                /**< Select internal clock source for dot clock generation */
    GLCD_PANEL_CLK_DIVISOR_8,                             /**< Set divider ratio of panel clock generation as x1/8 */
    GLCD_DITHERING_MODE_2X2PATTERN,                       /**< Select 2x2 pattern ditehring mode */
    GLCD_DITHERING_PATTERN_11,                            /**< Set Ditehring pattern A to '11' */
    GLCD_DITHERING_PATTERN_00,                            /**< Set Ditehring pattern B to '00' */
    GLCD_DITHERING_PATTERN_10,                            /**< Set Ditehring pattern C to '10'*/
    GLCD_DITHERING_PATTERN_01                             /**< Set Ditehring pattern D to '00' */
};

/** GLCD control block */
static glcd_ctrl_t ctrl_blk =
{
    .zero_coordinate    = { 0, 0 },
    .hsize              = 0,
    .vsize              = 0,
    .resource_lock.lock = 0,
    .p_context          = NULL
};

#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
/** Panel clock divider setting */
static uint32_t panel_clock_div[GLCD_PANEL_CLKDIV_NUM] =
{
    GLCD_PANEL_CLK_DIVISOR_1,         /**< Division Ratio 1/1 */
    GLCD_PANEL_CLK_DIVISOR_2,         /**< Division Ratio 1/2 */
    GLCD_PANEL_CLK_DIVISOR_3,         /**< Division Ratio 1/3 */
    GLCD_PANEL_CLK_DIVISOR_4,         /**< Division Ratio 1/4 */
    GLCD_PANEL_CLK_DIVISOR_5,         /**< Division Ratio 1/5 */
    GLCD_PANEL_CLK_DIVISOR_6,         /**< Division Ratio 1/6 */
    GLCD_PANEL_CLK_DIVISOR_7,         /**< Division Ratio 1/7 */
    GLCD_PANEL_CLK_DIVISOR_8,         /**< Division Ratio 1/8 */
    GLCD_PANEL_CLK_DIVISOR_9,         /**< Division Ratio 1/9 */
    GLCD_PANEL_CLK_DIVISOR_12,        /**< Division Ratio 1/12 */
    GLCD_PANEL_CLK_DIVISOR_16,        /**< Division Ratio 1/16 */
    GLCD_PANEL_CLK_DIVISOR_24,        /**< Division Ratio 1/24 */
    GLCD_PANEL_CLK_DIVISOR_32,        /**< Division Ratio 1/32 */
};
#endif

/*******************************************************************************************************************//**
 * @addtogroup GLCD
 * @{
 **********************************************************************************************************************/

void lcdc_lcdc_level_0_isr (void);
void lcdc_lcdc_level_1_isr (void);
void lcdc_lcdc_level_2_isr (void);

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS                      Device was opened successfully.
 * @retval  SSP_ERR_ASSERTION                Pointer to the control block or the configuration structure is NULL.
 * @retval  SSP_ERR_INVALID_ARGUMENT         Invalid parameter in the argument.
 * @retval  SSP_ERR_HW_LOCKED                GLCDC resource is locked.
 * @retval  SSP_ERR_CLOCK_GENERATION         Dot clock cannot be generated from clock source.
 * @retval  SSP_ERR_INVALID_TIMING_SETTING   Invalid panel timing parameter.
 * @retval  SSP_ERR_INVALID_LAYER_SETTING    Invalid layer setting found.
 * @retval  SSP_ERR_INVALID_LAYER_FORMAT     Invalid format is specified.
 * @retval  SSP_ERR_INVALID_GAMMA_SETTING    Invalid gamma correction setting found.
 * @note    PCLKA  must be supplied to Graphics LCD Controller (GLCDC) and  GLCDC pins must be set in IOPORT before
 *           calling this API.
 **********************************************************************************************************************/
ssp_err_t R_GLCD_Open (display_ctrl_t * const p_ctrl, display_cfg_t const * const p_cfg)
{
    ssp_err_t err = SSP_SUCCESS;

#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
    /** Check parameters */
    SSP_ASSERT(p_ctrl);
    err = r_glcd_open_param_check(p_cfg);
    if (SSP_SUCCESS != err)
    {
        return err;
    }
#endif
    /** Lock the GLCD resource */
    if (SSP_ERR_IN_USE == R_BSP_HardwareLock(BSP_HW_LOCK_GLCD))
    {
        return SSP_ERR_HW_LOCKED;
    }

    p_ctrl->state = DISPLAY_STATE_CLOSED;

    /** Supply the peripheral clock to the GLCD module */
    HW_GLCD_PowerOn();

    /** Release GLCD from a SW reset status. */
    HW_GLCD_ReleaseSoftwareReset();

    /** Set the dot clock frequency */
    r_glcd_clock_set(p_cfg);

    /** Set the panel signal timing */
    r_glcd_sync_signal_set(p_cfg);

    /** Configure the background screen */
    r_glcd_backgrpund_screen_set(p_cfg);

    /** Store back poach position to the control block (needed to define the layer blending position later) */
    ctrl_blk.zero_cycle        = r_glcd_calc_output_horizontal_pixel_offset(p_cfg);
    ctrl_blk.zero_coordinate.x = (int16_t)(p_cfg->output.htiming.back_porch + ctrl_blk.zero_cycle);
    ctrl_blk.zero_coordinate.y = (int16_t)(p_cfg->output.vtiming.back_porch);
    ctrl_blk.hsize             = p_cfg->output.htiming.display_cyc;
    ctrl_blk.vsize             = p_cfg->output.vtiming.display_cyc;

    /** Configure the graphics plane layers */
    for (uint32_t frame = 0; frame <= DISPLAY_FRAME_LAYER_2; frame++)
    {
        r_glcd_grahics_layer_set(&p_cfg->input[frame], &p_cfg->layer[frame], (display_frame_layer_t)frame);
    }

    /** Configure the output control block */
    r_glcd_output_block_set(p_cfg);

    /** Configure the color correction setting (brightness, brightness and gamma correction) */
    r_glcd_brightness_correction(p_ctrl, &p_cfg->output.brightness);
    r_glcd_contrast_correction(p_ctrl, &p_cfg->output.contrast);
    if (p_cfg->output.p_gamma_correction)
    {
        r_glcd_gamma_correction(p_cfg);
    }

    p_ctrl->state      = DISPLAY_STATE_OPENED;           /// Change GLCD driver state
    p_ctrl->p_callback = p_cfg->p_callback;              /// Save callback function
    p_ctrl->p_context  = p_cfg->p_context;               /// Save user defined context
    ctrl_blk.p_context = p_ctrl;                         /// Save the display interface context into GLCD HAL control
                                                         ///  block

    /** Set the line number which is suppose to happen the line detect interrupt */
    HW_GLCD_GR2planeLineDetectionNumberSet((uint16_t)(p_cfg->output.vtiming.back_porch + p_cfg->output.vtiming.display_cyc));

    /** Enable the GLCD interrupts */
    HW_GLCD_VposDetectEnable();
    HW_GLCD_GR1UnderflowDetectEnable();
    HW_GLCD_GR2UnderflowDetectEnable();
    HW_GLCD_VposInterruptEnable();
    HW_GLCD_GR1UnderflowInterruptEnable();
    HW_GLCD_GR2UnderflowInterruptEnable();

#if (BSP_IRQ_CFG_GLCDC_LINE_DETECT != BSP_IRQ_DISABLED)
    NVIC_EnableIRQ(GLCDC_LINE_DETECT_IRQn);
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_1 != BSP_IRQ_DISABLED)
    NVIC_EnableIRQ(GLCDC_UNDERFLOW_1_IRQn);
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_2 != BSP_IRQ_DISABLED)
    NVIC_EnableIRQ(GLCDC_UNDERFLOW_2_IRQn);
#endif

    return err;
}  /* End of function R_GLCD_Open() */

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS                  Device was closed successfully.
 * @retval  SSP_ERR_ASSERTION            Pointer to the control block is NULL.
 * @retval  SSP_ERR_NOT_OPEN   The function call is performed when the driver state is not equal to DISPLAY_STATE_CLOSED
 * @retval  SSP_ERR_INVALID_UPDATE_TIMING   A function call is performed when the GLCD is updating register values
 *           internally
 * @note    This API can be called when the driver is not in DISPLAY_STATE_CLOSED state. It returns an error
 *           if the register update operation for the background screen generation block is being held.
 **********************************************************************************************************************/
ssp_err_t R_GLCD_Close (display_ctrl_t * const p_ctrl)
{
    ssp_err_t err = SSP_SUCCESS;

#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
    GLCD_ERROR_RETURN(DISPLAY_STATE_CLOSED != p_ctrl->state, SSP_ERR_NOT_OPEN);
#endif

    /* Return immediately if the background block register updating is performed. */
    GLCD_ERROR_RETURN(false == HW_GLCD_IsBackgroundOperationing(), SSP_ERR_INVALID_UPDATE_TIMING);

#if (BSP_IRQ_CFG_GLCDC_LINE_DETECT != BSP_IRQ_DISABLED)
    NVIC_DisableIRQ(GLCDC_LINE_DETECT_IRQn);
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_1 != BSP_IRQ_DISABLED)
    NVIC_DisableIRQ(GLCDC_UNDERFLOW_1_IRQn);
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_2 != BSP_IRQ_DISABLED)
    NVIC_DisableIRQ(GLCDC_UNDERFLOW_2_IRQn);
#endif

    /** Disable the GLCD interrupts */
    HW_GLCD_VposDetectDisable();
    HW_GLCD_GR1UnderflowDetectDisable();
    HW_GLCD_GR2UnderflowDetectDisable();
    HW_GLCD_VposInterruptDisable();
    HW_GLCD_GR1UnderflowInterruptDisable();
    HW_GLCD_GR2UnderflowInterruptDisable();

    /** Reset the GLCD hardware */
    HW_GLCD_SoftwareReset();

    /** Halt the peripheral clock to the GLCD module */
    HW_GLCD_PowerOff();

    p_ctrl->state = DISPLAY_STATE_CLOSED;

    /** Unlock the GLCD resource */
    R_BSP_HardwareUnlock(BSP_HW_LOCK_GLCD);

    return err;
}  /* End of function R_GLCD_Close() */

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS                  Device was started successfully.
 * @retval  SSP_ERR_ASSERTION            Pointer to the control block is NULL.
 * @retval  SSP_ERR_INVALID_MODE         Function call is performed when the driver state is not DISPLAY_STATE_OPENED.
 * @note    This API can be called when the driver is not in DISPLAY_STATE_OPENED status.
 **********************************************************************************************************************/
ssp_err_t R_GLCD_Start (display_ctrl_t * const p_ctrl)
{
    ssp_err_t err = SSP_SUCCESS;

#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
    GLCD_ERROR_RETURN(DISPLAY_STATE_OPENED == p_ctrl->state, SSP_ERR_INVALID_MODE);
#endif

    /** Start to output the vertical and horizontal synchronization signals and screen data. */
    HW_GLCD_BackgroundOperationEnable();

    p_ctrl->state = DISPLAY_STATE_DISPLAYING;

    return err;
}  /* End of function R_GLCD_Start() */

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS                   Device was stopped successfully
 * @retval  SSP_ERR_ASSERTION             Pointer to the control block is NULL
 * @retval  SSP_ERR_INVALID_MODE          Function call is performed when the driver state is not 
 *                                        DISPLAY_STATE_DISPLAYING.
 * @retval  SSP_ERR_INVALID_UPDATE_TIMING The function call is performed while the GLCD is updating register values
 *                                          internally.
 * @note    This API can be called when the driver is in the DISPLAY_STATE_DISPLAYING state. It returns an
 *          error if the register update operation for the background screen generation blocks, the graphics data I/F 
 *          blocks, or the output control block is being held.
 **********************************************************************************************************************/
ssp_err_t R_GLCD_Stop (display_ctrl_t * const p_ctrl)
{
    ssp_err_t err = SSP_SUCCESS;

#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
    GLCD_ERROR_RETURN((DISPLAY_STATE_DISPLAYING == p_ctrl->state), SSP_ERR_INVALID_MODE);
#endif

    /* Return immediately if the register is being updated */
    GLCD_ERROR_RETURN(false == HW_GLCD_IsGRplaneUpdating(DISPLAY_FRAME_LAYER_1), SSP_ERR_INVALID_UPDATE_TIMING);
    GLCD_ERROR_RETURN(false == HW_GLCD_IsGRplaneUpdating(DISPLAY_FRAME_LAYER_2), SSP_ERR_INVALID_UPDATE_TIMING);
    GLCD_ERROR_RETURN(false == HW_GLCD_IsOutputControlBlockUpdating(), SSP_ERR_INVALID_UPDATE_TIMING);
    GLCD_ERROR_RETURN(false == HW_GLCD_IsInternalRegisterReflecting(), SSP_ERR_INVALID_UPDATE_TIMING);

    /** Stop outputting the vertical and horizontal synchronization signals and screen data. */
    HW_GLCD_BackgroundOperationDisable();

    p_ctrl->state = DISPLAY_STATE_OPENED;

    return err;
}  /* End of function R_GLCD_Stop() */

/*******************************************************************************************************************//**
 * @retval  SSP_ERR_INVALID_MODE             A function call is performed when the driver state is not
 *                                           DISPLAY_STATE_DISPLAYING.
 * @retval  SSP_ERR_INVALID_ARGUMENT         An invalid parameter is found in the argument.
 * @retval  SSP_ERR_INVALID_UPDATE_TIMING    A function call is performed while the GLCD is updating register values
 *                                            internally.
 * @note    This API can be called when the driver is in DISPLAY_STATE_DISPLAYING state. It returns an error
 *           if the register update operation for the background screen generation blocks or the graphics data I/F block
 *           is being held.
 **********************************************************************************************************************/
ssp_err_t R_GLCD_LayerChange (display_ctrl_t const * const        p_ctrl,
                              display_runtime_cfg_t const * const p_cfg,
                              display_frame_layer_t               frame)
{
    SSP_PARAMETER_NOT_USED(p_ctrl);

#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
    GLCD_ERROR_RETURN((DISPLAY_STATE_DISPLAYING == p_ctrl->state), SSP_ERR_INVALID_MODE);
    GLCD_ERROR_RETURN((frame == DISPLAY_FRAME_LAYER_1) || (frame == DISPLAY_FRAME_LAYER_2), SSP_ERR_INVALID_ARGUMENT);
#endif

    /* Return immediately if the register updating is in progress */
    GLCD_ERROR_RETURN(false == HW_GLCD_IsGRplaneUpdating(frame), SSP_ERR_INVALID_UPDATE_TIMING);
    GLCD_ERROR_RETURN(false == HW_GLCD_IsInternalRegisterReflecting(), SSP_ERR_INVALID_UPDATE_TIMING);

    /** Configure the graphics plane layers */
    r_glcd_grahics_layer_set(&p_cfg->input, &p_cfg->layer, frame);

    /** Reflect the graphics module register value to the GLCD internal operations (at the timing of the next Vsync
     * assertion) */
    HW_GLCD_GRplaneUpdateEnable(frame);

    return SSP_SUCCESS;
}  /* End of function R_GLCD_LayerChange() */

/*******************************************************************************************************************//**
 * @retval  SSP_ERR_ASSERTION            Pointer to the control block is NULL.
 * @retval  SSP_ERR_INVALID_MODE Function call is performed when the driver state is not DISPLAY_STATE_DISPLAYING
 *                                        DISPLAY_STATE_DISPLAYING.
 * @retval  SSP_ERR_INVALID_UPDATE_TIMING A function call is performed while the GLCDC is updating registers
 * internally.
 * @note    This API can be called when the driver is in the DISPLAY_STATE_DISPLAYING state. It returns an error
 *           if the register update operation for the background screen generation blocks or the output control block is
 *           being held.
 **********************************************************************************************************************/
ssp_err_t R_GLCD_ColorCorrection (display_ctrl_t const * const p_ctrl, display_correction_t const * const p_correction)
{
#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_correction);
    GLCD_ERROR_RETURN((DISPLAY_STATE_DISPLAYING == p_ctrl->state), SSP_ERR_INVALID_MODE);
#endif

    /* Return immediately if the register updatings are performed */
    GLCD_ERROR_RETURN(false == HW_GLCD_IsOutputControlBlockUpdating(), SSP_ERR_INVALID_UPDATE_TIMING);
    GLCD_ERROR_RETURN(false == HW_GLCD_IsInternalRegisterReflecting(), SSP_ERR_INVALID_UPDATE_TIMING);

    /** Configure the brightness and contrast correction register setting. */
    r_glcd_brightness_correction(p_ctrl, &p_correction->brightness);
    r_glcd_contrast_correction(p_ctrl, &p_correction->contrast);

    /** Update the Output block register setting. */
    HW_GLCD_OutputControlBlockUpdate();

    return SSP_SUCCESS;
}  /* End of function R_GLCD_ColorCorrection() */

/*******************************************************************************************************************//**
 * @retval  SSP_ERR_ASSERTION            Pointer to the control block or CLUT source data is NULL.
 * @retval  SSP_ERR_INVALID_CLUT_ACCESS  Illegal CLUT entry or size is specified.
 * @note    This API can be called any time.
 **********************************************************************************************************************/
ssp_err_t R_GLCD_ClutUpdate (display_ctrl_t const * const     p_ctrl,
                             display_clut_cfg_t const * const p_clut_cfg,
                             display_frame_layer_t            frame)
{
    SSP_PARAMETER_NOT_USED(p_ctrl);

#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_clut_cfg);
    GLCD_ERROR_RETURN((GLCD_CLUT_ENTRY_SIZE > p_clut_cfg->start), SSP_ERR_INVALID_CLUT_ACCESS);
    GLCD_ERROR_RETURN((GLCD_CLUT_ENTRY_SIZE >= p_clut_cfg->size), SSP_ERR_INVALID_CLUT_ACCESS);
    GLCD_ERROR_RETURN((DISPLAY_FRAME_LAYER_2 >= frame),  SSP_ERR_INVALID_CLUT_ACCESS);
#endif
    glcd_clut_plane_t current_plane;
    uint32_t          * pbase = p_clut_cfg->p_base;

    /** Check the CLUT table current used */
    if (HW_GLCD_IsCLUTplane1Selected(frame))
    {
        current_plane = GLCD_CLUT_PLANE_1;
    }
    else
    {
        current_plane = GLCD_CLUT_PLANE_0;
    }

    for (uint32_t i = p_clut_cfg->start; i < p_clut_cfg->size; i++)
    {
        /** Copy the new CLUT data on the source memory to the CLUT SRAM in the GLCD module */
        if (GLCD_CLUT_PLANE_0 == current_plane)
        {
            HW_GLCD_CLUT1Set(frame, i, *pbase++);
        }
        else
        {
            HW_GLCD_CLUT0Set(frame, i, *pbase++);
        }
    }

    /** Make the GLCD module read the new CLUT table data from the next frame */
    if (GLCD_CLUT_PLANE_0 == current_plane)
    {
        HW_GLCD_CLUTplane1Select(frame);
    }
    else
    {
        HW_GLCD_CLUTplane0Select(frame);
    }

    return SSP_SUCCESS;
}  /* End of function R_GLCD_ClutUpdate() */

/*******************************************************************************************************************//**
 * @retval  SSP_SUCCESS  Got status successfully.
 * @retval  SSP_ASSERT   Null pointer is given.
 * @note    The GLCD hardware starts the fading processing at the first Vsync after the previous LayerChange() call is
 * held.
 *          Due to this behavior of the hardware, this API may not return DISPLAY_FADE_STATUS_FADING_UNDERWAY as the fading
 *           status, if it is called before the first Vsync after LayerChange() is called. In this case, the API returns
 *           DISPLAY_FADE_STATUS_UNCERTAIN, instead of DISPLAY_FADE_STATUS_NOT_UNDERWAY.
 **********************************************************************************************************************/
ssp_err_t R_GLCD_StatusGet  (display_ctrl_t const * const p_ctrl, display_status_t * const p_status)
{
#if (GLCD_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_status);
#endif
    /** Return the GLCD HAL driver state */
    p_status->state = p_ctrl->state;

    /** Return the fading status for the layers */
    for (uint32_t frame = 0; frame <= DISPLAY_FRAME_LAYER_2; frame++)
    {
        if (HW_GLCD_IsGRplaneUpdating(frame))
        {
            /* Fading status is uncertain (the reason is described above). */
            p_status->fade_status[frame] = DISPLAY_FADE_STATUS_UNCERTAIN;
        }
        else
        {
            if (HW_GLCD_GRplaneIsFadeInOutInProgress(frame))
            {
                p_status->fade_status[frame] = DISPLAY_FADE_STATUS_FADING_UNDERWAY;
            }
            else
            {
                p_status->fade_status[frame] = DISPLAY_FADE_STATUS_NOT_UNDERWAY;
            }
        }
    }

    return SSP_SUCCESS;
}  /* End of function R_GLCD_StatusGet() */

/*******************************************************************************************************************//**
 * @retval  p_version. The version number.
 * @note    This function is re-entrant.
 **********************************************************************************************************************/
ssp_err_t R_GLCD_VersionGet (ssp_version_t * p_version)
{
    *p_version = module_version;
    return SSP_SUCCESS;
}  /* End of function R_GLCD_VersionGet() */

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
#if (GLCD_CFG_PARAM_CHECKING_ENABLE)

/*******************************************************************************************************************//**
 * @brief   This is the parameter checking subroutine for the R_GLCD_Open API.
 * @param[in]     p_cfg   Pointer to the configuration structure for display interface
 * @retval  SSP_SUCCESS                      No parameter error found
 * @retval  SSP_ERR_INVALID_TIMING_SETTING   Invalid panel timing parameter
 * @retval  SSP_ERR_INVALID_ARGUMENT         Invalid setting for signal attributes
 **********************************************************************************************************************/
static ssp_err_t r_glcd_open_param_check_sync_signal (display_cfg_t const * const p_cfg)
{
    GLCD_ERROR_RETURN((BG_PLANE_H_CYC_MIN <= p_cfg->output.htiming.total_cyc)
                      && (BG_PLANE_H_CYC_MAX >= p_cfg->output.htiming.total_cyc), SSP_ERR_INVALID_TIMING_SETTING);
    GLCD_ERROR_RETURN((BG_PLANE_V_CYC_MIN <= p_cfg->output.vtiming.total_cyc)
                      && (BG_PLANE_V_CYC_MAX >= p_cfg->output.vtiming.total_cyc), SSP_ERR_INVALID_TIMING_SETTING);
    GLCD_ERROR_RETURN((BG_PLANE_H_CYC_ACTIVE_SIZE_MIN <= p_cfg->output.htiming.display_cyc)
                      && (BG_PLANE_H_CYC_ACTIVE_SIZE_MAX >= p_cfg->output.htiming.display_cyc),
                      SSP_ERR_INVALID_TIMING_SETTING);
    GLCD_ERROR_RETURN((BG_PLANE_V_CYC_ACTIVE_SIZE_MIN <= p_cfg->output.vtiming.display_cyc)
                      && (BG_PLANE_V_CYC_ACTIVE_SIZE_MAX >= p_cfg->output.vtiming.display_cyc),
                      SSP_ERR_INVALID_TIMING_SETTING);
    GLCD_ERROR_RETURN((BG_PLANE_V_CYC_ACTIVE_POS_MIN <= p_cfg->output.vtiming.back_porch)
                      && (BG_PLANE_V_CYC_ACTIVE_POS_MAX >= p_cfg->output.vtiming.back_porch),
                      SSP_ERR_INVALID_TIMING_SETTING);
    GLCD_ERROR_RETURN((BG_PLANE_H_CYC_ACTIVE_POS_MIN <= p_cfg->output.htiming.back_porch)
                      && (BG_PLANE_H_CYC_ACTIVE_POS_MAX >= p_cfg->output.htiming.back_porch),
                      SSP_ERR_INVALID_TIMING_SETTING);
    GLCD_ERROR_RETURN((TCON_SYGNAL_ASSERT_WIDTH_MAX >= p_cfg->output.htiming.sync_width),
                      SSP_ERR_INVALID_TIMING_SETTING);
    GLCD_ERROR_RETURN((TCON_SYGNAL_ASSERT_WIDTH_MAX >= p_cfg->output.vtiming.sync_width),
                      SSP_ERR_INVALID_TIMING_SETTING);

    GLCD_ERROR_RETURN((p_cfg->output.htiming.total_cyc >=
                       (p_cfg->output.htiming.back_porch + p_cfg->output.htiming.display_cyc)),
                      SSP_ERR_INVALID_TIMING_SETTING);
    GLCD_ERROR_RETURN((p_cfg->output.vtiming.total_cyc >=
                       (p_cfg->output.vtiming.back_porch + p_cfg->output.vtiming.display_cyc)),
                      SSP_ERR_INVALID_TIMING_SETTING);
    GLCD_ERROR_RETURN((DISPLAY_SIGNAL_POLARITY_LOACTIVE == p_cfg->output.htiming.sync_polarity)
                      || (DISPLAY_SIGNAL_POLARITY_HIACTIVE == p_cfg->output.htiming.sync_polarity),
                      SSP_ERR_INVALID_ARGUMENT);
    GLCD_ERROR_RETURN((DISPLAY_SIGNAL_POLARITY_LOACTIVE == p_cfg->output.vtiming.sync_polarity)
                      || (DISPLAY_SIGNAL_POLARITY_HIACTIVE == p_cfg->output.vtiming.sync_polarity),
                      SSP_ERR_INVALID_ARGUMENT);

    /** Check DE signal setting */
    GLCD_ERROR_RETURN((DISPLAY_SIGNAL_POLARITY_LOACTIVE == p_cfg->output.data_enable_polarity)
                      || (DISPLAY_SIGNAL_POLARITY_HIACTIVE == p_cfg->output.data_enable_polarity),
                      SSP_ERR_INVALID_ARGUMENT);

    return SSP_SUCCESS;
}  /* End of function r_glcd_open_param_check_sync_signal() */

/*******************************************************************************************************************//**
 * @brief   The parameter checking subroutine for the R_GLCD_Open API.
 * @param[in]     p_cfg   Pointer to the configuration structure for display interface
 * @retval  SSP_SUCCESS                      No parameter error found
 * @retval  SSP_ERR_INVALID_LAYER_SETTING    Invalid layer setting found
 * @retval  SSP_ERR_INVALID_LAYER_FORMAT     Invalid format is specified
 * @retval  SSP_ERR_INVALID_ARGUMENT         Invalid setting for signal attributes
 **********************************************************************************************************************/
static ssp_err_t r_glcd_open_param_check_layer_setting (display_cfg_t const * const p_cfg)
{
    for (uint32_t i = 0; i <= DISPLAY_FRAME_LAYER_2; i++)
    {
        if (p_cfg->input[i].p_base)
        {
            /** Base address and memory stride have to be aligned to 64-byte boundary */
            GLCD_ERROR_RETURN(0 == ((uint32_t) (p_cfg->input[i].p_base) % GLCD_ADDRESS_ALIGNMENT_64B),
                              SSP_ERR_INVALID_ALIGNMENT);
            GLCD_ERROR_RETURN(0 ==
                              (((p_cfg->input[i].hstride
                                 * r_glcd_get_bit_size(p_cfg->input[i].format)) / 8) % GLCD_ADDRESS_ALIGNMENT_64B),
                                 SSP_ERR_INVALID_ALIGNMENT);

            /** Format check */
            GLCD_ERROR_RETURN(DISPLAY_IN_FORMAT_CLUT1 >= p_cfg->input[i].format, SSP_ERR_INVALID_LAYER_FORMAT);

            /** Check horizontal/vertical pixels is less than or equal to maximum value, also horizontal pixel must be
             * an odd value. Horizontal check is intently not performed because of supporting line repeating mode.
             */

            uint16_t vline_size = p_cfg->input[i].vsize;
            if (p_cfg->input[i].lines_repeat_enable)
            {
                vline_size = (uint16_t)(vline_size * p_cfg->input[i].lines_repeat_times);
            }

            GLCD_ERROR_RETURN((GR_PLANE_V_CYC_ACTIVE_SIZE_MAX >= vline_size), SSP_ERR_INVALID_LAYER_SETTING);

            /** Check horizontal/vertical pixels is less than or equal to the maximum value, also horizontal pixel must
             * be
             * an odd value*/
            GLCD_ERROR_RETURN(0 == ((p_cfg->input[i].hsize) % 2), SSP_ERR_INVALID_LAYER_SETTING);

            /** Check the number of data transfer times per a line (64bytes/transfer) */
            GLCD_ERROR_RETURN((GR_PLANE_TOTAL_TRANSFER_TIMES_MAX >=
                               (((p_cfg->input[i].hstride *
                                  r_glcd_get_bit_size(p_cfg->input[i].format)) / 8) / GLCD_ADDRESS_ALIGNMENT_64B)),
                                  SSP_ERR_INVALID_LAYER_SETTING);

            /** Check the timing setting for active video period against the max./min. setting value */
            GLCD_ERROR_RETURN((GR_PLANE_H_CYC_ACTIVE_POS_MIN <=
                               (p_cfg->output.htiming.back_porch + p_cfg->layer[i].coordinate.x))
                              && (GR_PLANE_H_CYC_ACTIVE_POS_MAX >=
                                  (p_cfg->output.htiming.back_porch + p_cfg->layer[i].coordinate.x)),
                              SSP_ERR_INVALID_LAYER_SETTING);
            GLCD_ERROR_RETURN((GR_PLANE_V_CYC_ACTIVE_POS_MIN <=
                               (p_cfg->output.vtiming.back_porch + p_cfg->layer[i].coordinate.y))
                              && (GR_PLANE_V_CYC_ACTIVE_POS_MAX >=
                                  (p_cfg->output.vtiming.back_porch + p_cfg->layer[i].coordinate.y)),
                              SSP_ERR_INVALID_LAYER_SETTING);
        }
    }

    return SSP_SUCCESS;
}  /* End of function r_glcd_open_param_check_layer_setting() */

/*******************************************************************************************************************//**
 * @brief   The parameter checking subroutine for R_GLCD_Open API.
 * @param[in]     p_cfg   Pointer to the configuration structure for display interface
 * @retval  SSP_SUCCESS                      No parameter error found
 * @retval  SSP_ERR_INVALID_GAMMA_SETTING    Invalid gamma correction setting found
 **********************************************************************************************************************/
static ssp_err_t r_glcd_open_param_check_correction_setting (display_cfg_t const * const p_cfg)
{
    if (p_cfg->output.p_gamma_correction)
    {
        if (p_cfg->output.p_gamma_correction->b.enable)
        {
            uint16_t previous_threshold = 0;
            for (int32_t i = 0; i < DISPLAY_GAMMA_CURVE_ELEMENT_NUM; i++)
            {
                /** Each of the gamma correction threshold values must be less than GLCD_GAMMA_THRESHOLD_MAX */
                GLCD_ERROR_RETURN((GLCD_GAMMA_THRESHOLD_MAX >= p_cfg->output.p_gamma_correction->b.threshold[i]),
                                  SSP_ERR_INVALID_GAMMA_SETTING);

                /** The Gamma correction threshold[N] must be less than threshold[N+1] */
                GLCD_ERROR_RETURN(previous_threshold <= p_cfg->output.p_gamma_correction->b.threshold[i],
                                  SSP_ERR_INVALID_GAMMA_SETTING);
                previous_threshold = p_cfg->output.p_gamma_correction->b.threshold[i];

                /** Each of gamma correction gain values must be less than GLCD_GAMMA_GAIN_MAX */
                GLCD_ERROR_RETURN((GLCD_GAMMA_GAIN_MAX >= p_cfg->output.p_gamma_correction->b.gain[i]),
                                  SSP_ERR_INVALID_GAMMA_SETTING);
            }
        }

        if (p_cfg->output.p_gamma_correction->g.enable)
        {
            uint16_t previous_threshold = 0;
            for (int32_t i = 0; i < DISPLAY_GAMMA_CURVE_ELEMENT_NUM; i++)
            {
                /** Each of gamma correction threshold value must be less than GLCD_GAMMA_THRESHOLD_MAX */
                GLCD_ERROR_RETURN((GLCD_GAMMA_THRESHOLD_MAX >= p_cfg->output.p_gamma_correction->g.threshold[i]),
                                  SSP_ERR_INVALID_GAMMA_SETTING);

                /** Gamma correction threshold[N] must be less than threshold[N+1] */
                GLCD_ERROR_RETURN(previous_threshold <= p_cfg->output.p_gamma_correction->g.threshold[i],
                                  SSP_ERR_INVALID_GAMMA_SETTING);
                previous_threshold = p_cfg->output.p_gamma_correction->g.threshold[i];

                /** Each of gamma correction gain value must be less than GLCD_GAMMA_GAIN_MAX */
                GLCD_ERROR_RETURN((GLCD_GAMMA_GAIN_MAX >= p_cfg->output.p_gamma_correction->g.gain[i]),
                                  SSP_ERR_INVALID_GAMMA_SETTING);
            }
        }

        if (p_cfg->output.p_gamma_correction->r.enable)
        {
            uint16_t previous_threshold = 0;
            for (int32_t i = 0; i < DISPLAY_GAMMA_CURVE_ELEMENT_NUM; i++)
            {
                /** Each of gamma correction threshold value must be less than GLCD_GAMMA_THRESHOLD_MAX */
                GLCD_ERROR_RETURN((GLCD_GAMMA_THRESHOLD_MAX >= p_cfg->output.p_gamma_correction->r.threshold[i]),
                                  SSP_ERR_INVALID_GAMMA_SETTING);

                /** Gamma correction threshold[N] must be less than threshold[N+1] */
                GLCD_ERROR_RETURN(previous_threshold <= p_cfg->output.p_gamma_correction->r.threshold[i],
                                  SSP_ERR_INVALID_GAMMA_SETTING);
                previous_threshold = p_cfg->output.p_gamma_correction->r.threshold[i];

                /** Each of gamma correction gain value must be less than GLCD_GAMMA_GAIN_MAX */
                GLCD_ERROR_RETURN((GLCD_GAMMA_GAIN_MAX >= p_cfg->output.p_gamma_correction->r.gain[i]),
                                  SSP_ERR_INVALID_GAMMA_SETTING);
            }
        }
    }

    return SSP_SUCCESS;
}  /* End of function r_glcd_open_param_check_correction_setting() */

/*******************************************************************************************************************//**
 * @brief   The parameter checking subroutine for R_GLCD_Open API.
 * @param[in]     p_cfg   Pointer to the configuration structure for display interface
 * @retval  SSP_SUCCESS                      No parameter error found
 * @retval  SSP_ERR_CLOCK_GENERATION         Clock ratio setting is invalid
 * @retval  SSP_ERR_INVALID_ARGUMENT         Clock source selection is invalid
 **********************************************************************************************************************/
static ssp_err_t r_glcd_open_param_check_hardware_setting (glcd_cfg_t const * const p_cfg)
{
    uint32_t i;

    GLCD_ERROR_RETURN(0 != p_cfg->clock_div_ratio, SSP_ERR_CLOCK_GENERATION);
    GLCD_ERROR_RETURN((GLCD_CLK_SRC_INTERNAL == p_cfg->clksrc)
                      || (GLCD_CLK_SRC_EXTERNAL == p_cfg->clksrc), SSP_ERR_INVALID_ARGUMENT);

    for (i = 0; i < GLCD_PANEL_CLKDIV_NUM; i++)
    {
        if (panel_clock_div[i] == p_cfg->clock_div_ratio)
        {
            break;
        }
    }

    if (GLCD_PANEL_CLKDIV_NUM == i)
    {
        return SSP_ERR_CLOCK_GENERATION;    /* Expected dot clock cannot be created by dividing clock source */
    }

    GLCD_ERROR_RETURN(GLCD_PANEL_CLK_DIVISOR_32 >= p_cfg->clock_div_ratio, SSP_ERR_INVALID_ARGUMENT);

    return SSP_SUCCESS;
}  /* End of function r_glcd_open_param_check_correction_setting() */

/*******************************************************************************************************************//**
 * @brief   The parameter checking subroutine for R_GLCD_Open API.
 * @param[in] p_cfg   Pointer to the configuration structure for display interface
 * @retval  SSP_SUCCESS                      No parameter error found
 * @retval  SSP_ERR_ASSERTION                Pointer to the control block is NULL
 * @retval  SSP_ERR_INVALID_ARGUMENT         Clock source selection is invalid
 * @retval  SSP_ERR_CLOCK_GENERATION         Clock ratio setting is invalid
 * @retval  SSP_ERR_INVALID_TIMING_SETTING   Invalid panel timing parameter
 * @retval  SSP_ERR_INVALID_LAYER_SETTING    Invalid layer setting found
 * @retval  SSP_ERR_INVALID_LAYER_FORMAT     Invalid format is specified
 * @retval  SSP_ERR_INVALID_GAMMA_SETTING    Invalid gamma correction setting found
 **********************************************************************************************************************/
static ssp_err_t r_glcd_open_param_check (display_cfg_t const * const p_cfg)
{
    ssp_err_t error;

    SSP_ASSERT(p_cfg);

    /** Sync signal parameter check */
    error = r_glcd_open_param_check_sync_signal(p_cfg);
    GLCD_ERROR_RETURN(SSP_SUCCESS == error, error);

    /** Layer setting parameter check */
    error = r_glcd_open_param_check_layer_setting(p_cfg);
    GLCD_ERROR_RETURN(SSP_SUCCESS == error, error);

    /** Check endian setting */
    GLCD_ERROR_RETURN((DISPLAY_ENDIAN_LITTLE == p_cfg->output.endian)
                      || (DISPLAY_ENDIAN_BIG    == p_cfg->output.endian), SSP_ERR_INVALID_ARGUMENT);

    /** Color correction setting parameter check */
    error = r_glcd_open_param_check_correction_setting(p_cfg);
    GLCD_ERROR_RETURN(SSP_SUCCESS == error, error);

    /** GLCD hardware dependent setting parameter check */
    if (p_cfg->p_extend)
    {
        error = r_glcd_open_param_check_hardware_setting((glcd_cfg_t *) p_cfg->p_extend);
        GLCD_ERROR_RETURN(SSP_SUCCESS == error, error);
    }

    return SSP_SUCCESS;
}  /* End of function r_glcd_open_param_check() */
#endif /* if (GLCD_CFG_PARAM_CHECKING_ENABLE) */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure dot clock setting
 * @param[in]     p_cfg   Pointer to the configuration structure for display interface
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_clock_set (display_cfg_t const * const p_cfg)
{
    glcd_cfg_t * pextend;

    if (p_cfg->p_extend)
    {
        pextend = (glcd_cfg_t *) p_cfg->p_extend;
    }
    else
    {
        pextend = (glcd_cfg_t *) &default_glcd_cfg;
    }

    /** Selects input source for dot clock */
    if (GLCD_CLK_SRC_INTERNAL == pextend->clksrc)
    {
        HW_GLCD_DOTCLK_InternalClockSelect();
    }
    else
    {
        HW_GLCD_DOTCLK_ExternalClockSelect();
    }

    /** Sets division ratio */
    HW_GLCD_DOTCLK_DividerSet(pextend->clock_div_ratio);

    /** Selects pixel clock output */
    if (DISPLAY_OUT_FORMAT_8BITS_SERIAL != p_cfg->output.format)
    {
        HW_GLCD_DOTCLK_ParallelRGBSelect();
    }
    else
    {
        HW_GLCD_DOTCLK_SerialRGBSelect();
    }

    HW_GLCD_DOTCLK_OutputEnable();
}  /* End of function r_glcd_clock_set() */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the sync signal setting (TCON block setting)
 * @param[in]     p_cfg   Pointer to the configuration structure for display interface
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_sync_signal_set (display_cfg_t const * const p_cfg)
{
    glcd_cfg_t      * pextend = (glcd_cfg_t *) p_cfg->p_extend;
    glcd_tcon_pin_t tcon_hsync;
    glcd_tcon_pin_t tcon_vsync;
    glcd_tcon_pin_t tcon_de;

    if (NULL == p_cfg->p_extend)
    {
        /** Applied default configuration if GLCD HAL configuration is NULL */
        pextend = (glcd_cfg_t *) &default_glcd_cfg;
    }

    if (DISPLAY_SIGNAL_SYNC_EDGE_RISING == p_cfg->output.sync_edge)
    {
        HW_GLCD_LCDOUT_PhaseSyncToRisingEdge();
        HW_GLCD_TCON_PhaseSyncToRisingEdge();
    }
    else
    {
        HW_GLCD_LCDOUT_PhaseSyncToFallingEdge();
        HW_GLCD_TCON_PhaseSyncToFallingEdge();
    }

    HW_GLCD_HsyncDelaySet(0);
    HW_GLCD_HsyncDelaySet(0);

    if (pextend)
    {
        tcon_hsync = pextend->tcon_hsync;
        tcon_vsync = pextend->tcon_vsync;
        tcon_de    = pextend->tcon_de;
    }
    else
    {
        tcon_hsync = GLCD_TCON_PIN_0;
        tcon_vsync = GLCD_TCON_PIN_1;
        tcon_de    = GLCD_TCON_PIN_2;
    }

    uint16_t zero_offset = r_glcd_calc_output_horizontal_pixel_offset(p_cfg);

    r_glcd_hsync_set(tcon_hsync, &p_cfg->output.htiming, zero_offset);
    r_glcd_vsync_set(tcon_vsync, &p_cfg->output.vtiming);
    r_glcd_data_enable_set(tcon_de, &p_cfg->output.vtiming, &p_cfg->output.htiming, p_cfg->output.data_enable_polarity,
                           zero_offset);
}  /* End of function r_glcd_sync_signal_set() */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the horizontal signal setting
 * @param[in]     tcon        TCON pin select(GLCD_TCON_PIN_0|GLCD_TCON_PIN_1|GLCD_TCON_PIN_2|GLCD_TCON_PIN_3)
 * @param[in]     timing      Hsync signal timing
 * @param[in]     zero_offset The number of Pixel offset needed to achieve the smooth horizontal sliding functionality.
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_hsync_set (glcd_tcon_pin_t tcon, display_timing_t const * timing, uint16_t zero_offset)
{
    tcon_func_t tcon_func;

    switch (tcon)
    {
        case GLCD_TCON_PIN_1:
            tcon_func.tcon_select = HW_GLCD_TCON1_Select;
            break;
        case GLCD_TCON_PIN_2:
            tcon_func.tcon_select = HW_GLCD_TCON2_Select;
            break;
        case GLCD_TCON_PIN_3:
            tcon_func.tcon_select = HW_GLCD_TCON3_Select;
            break;
        case GLCD_TCON_PIN_0: /* Intentionally go though to the default case */
        default:
            tcon_func.tcon_select = HW_GLCD_TCON0_Select;
            break;
    }
    if (DISPLAY_SIGNAL_POLARITY_LOACTIVE == timing->sync_polarity)
    {
        tcon_func.invert = HW_GLCD_HSYNCA_Invert;
    }
    else
    {
        tcon_func.invert = HW_GLCD_HSYNCA_NotInvert;
    }

    tcon_func.tcon_select(GLCD_TCON_SIGNAL_SELECT_STHA_HS);
    tcon_func.invert();

    HW_GLCD_TCON_HSyncPostion_Set(zero_offset);
    HW_GLCD_TCON_HSyncWidth_Set(timing->sync_width);
}  /* End of function r_glcd_hsync_set () */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the vertical signal setting
 * @param[in]     tcon        TCON pin select(GLCD_TCON_PIN_0|GLCD_TCON_PIN_1|GLCD_TCON_PIN_2|GLCD_TCON_PIN_3)
 * @param[in]     timing      Vsync signal timing
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_vsync_set (glcd_tcon_pin_t tcon, display_timing_t const * timing)
{
    tcon_func_t tcon_func;

    switch (tcon)
    {
        case GLCD_TCON_PIN_0:
            tcon_func.tcon_select = HW_GLCD_TCON0_Select;
            break;
        case GLCD_TCON_PIN_2:
            tcon_func.tcon_select = HW_GLCD_TCON2_Select;
            break;
        case GLCD_TCON_PIN_3:
            tcon_func.tcon_select = HW_GLCD_TCON3_Select;
            break;
        case GLCD_TCON_PIN_1: /* Intentionally go though to the default case */
        default:
            tcon_func.tcon_select = HW_GLCD_TCON1_Select;
            break;
    }
    if (DISPLAY_SIGNAL_POLARITY_LOACTIVE == timing->sync_polarity)
    {
        tcon_func.invert = HW_GLCD_VSYNCA_Invert;
    }
    else
    {
        tcon_func.invert = HW_GLCD_VSYNCA_NotInvert;
    }

    tcon_func.tcon_select(GLCD_TCON_SIGNAL_SELECT_STVA_VS);
    tcon_func.invert();
    HW_GLCD_TCON_VSyncPostion_Set(0);
    HW_GLCD_TCON_VSyncWidth_Set(timing->sync_width);
}  /* End of function r_glcd_vsync_set () */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the data enable(DE) signal setting
 * @param[in]     tcon        TCON pin select(GLCD_TCON_PIN_0|GLCD_TCON_PIN_1|GLCD_TCON_PIN_2|GLCD_TCON_PIN_3)
 * @param[in]     vtiming     DE signal vertical timing
 * @param[in]     htiming     DE signal horizontal timing
 * @param[in]     polarity    DE signal porarity(DISPLAY_SIGNAL_POLARITY_LOACTIVE|DISPLAY_SIGNAL_POLARITY_HIACTIVE)
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_data_enable_set (glcd_tcon_pin_t const tcon, display_timing_t const * const vtiming,
                                    display_timing_t const * const htiming, display_signal_polarity_t const polarity,
                                    uint16_t zero_offset)
{
    tcon_func_t tcon_func;

    switch (tcon)
    {
        case GLCD_TCON_PIN_0:
            tcon_func.tcon_select = HW_GLCD_TCON0_Select;
            break;
        case GLCD_TCON_PIN_1:
            tcon_func.tcon_select = HW_GLCD_TCON1_Select;
            break;
        case GLCD_TCON_PIN_3:
            tcon_func.tcon_select = HW_GLCD_TCON3_Select;
            break;
        case GLCD_TCON_PIN_2: /* Intentionally go though to the default case */
        default:
            tcon_func.tcon_select = HW_GLCD_TCON2_Select;
            break;
    }
    if (DISPLAY_SIGNAL_POLARITY_LOACTIVE == polarity)
    {
        tcon_func.invert = HW_GLCD_DE_Invert;
    }
    else
    {
        tcon_func.invert = HW_GLCD_DE_NotInvert;
    }

    tcon_func.tcon_select(GLCD_TCON_SIGNAL_SELECT_DE);
    tcon_func.invert();

    HW_GLCD_TCON_HDataEnablePostion_Set((uint16_t)(htiming->back_porch + zero_offset));
    HW_GLCD_TCON_HDataEnableWidth_Set(htiming->display_cyc);
    HW_GLCD_TCON_VDataEnablePostion_Set(vtiming->back_porch);
    HW_GLCD_TCON_VDataEnableWidth_Set(vtiming->display_cyc);
}  /* End of function r_glcd_data_enable_set () */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the background screen setting
 *          - Panel timing setting
 *          - Color setting for the background screen
 * @param[in]     p_cfg   Pointer to the configuration structure for display interface
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_backgrpund_screen_set (display_cfg_t const * const p_cfg)
{
    /** Set number of total cycle for a line including Sync & Back poach, Front poach */
    HW_GLCD_BGplaneHorizontalCycleSet(p_cfg->output.htiming.total_cyc);
    HW_GLCD_BGplaneVerticalCycleSet(p_cfg->output.vtiming.total_cyc);

    HW_GLCD_BGplaneHorizontalSyncStartPositionSet(1); /* Minimum value is 1 (1cycle) */
    HW_GLCD_BGplaneVerticalSyncStartPositionSet(1);   /* Minimum value is 1 (1line) */

    /** Set the start position of Background screen */
    HW_GLCD_BGplaneHorizontalStartPositionSet((uint16_t)(p_cfg->output.htiming.back_porch +
                                              (r_glcd_calc_output_horizontal_pixel_offset(p_cfg) + 1)));
    HW_GLCD_BGplaneVerticalStartPositionSet((uint16_t)(p_cfg->output.vtiming.back_porch + 1));

    /** Set the width of Background screen */
    HW_GLCD_BGplaneHorizontalWidthSet(p_cfg->output.htiming.display_cyc);
    HW_GLCD_BGplaneVerticalWidthSet(p_cfg->output.vtiming.display_cyc);

    /** Set the Background color */
    HW_GLCD_BGplaneColorSet(p_cfg->output.bg_color);
}  /* End of function r_glcd_backgrpund_screen_set () */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the blending setting for the graphics planes
 * @param[in]     p_layer     The layer configuration
 * @param[in]     frame       The number of input frame buffer
 * @retval        void
 * @note    This function does not perform parameter check and it would be expected to be done in the caller function.
 **********************************************************************************************************************/
static void r_glcd_grahics_layer_blend_condition_set (display_layer_t const * const     p_layer,
                                                      display_frame_layer_t const       frame)
{
    /** Enable graphics data read from memory */
    if (DISPLAY_FADE_CONTROL_NONE != p_layer->fade_control)
    {
        HW_GLCD_GRplaneRectangularAlphaBlendingEnable(frame);
        HW_GLCD_GRplaneFadeInOutVariationFrameRateSet(frame, 0);

        if (DISPLAY_FADE_CONTROL_FADEIN == p_layer->fade_control)
        {
            HW_GLCD_GRplaneRectangleAlphaInitialValueSet(frame, GLCD_FADING_CONTROL_INITIAL_ALPHA_MIN);
            HW_GLCD_GRplaneFadeInOutVariationValueSet(frame, p_layer->fade_speed);
        }
        else
        {
            HW_GLCD_GRplaneRectangleAlphaInitialValueSet(frame, GLCD_FADING_CONTROL_INITIAL_ALPHA_MAX);
            HW_GLCD_GRplaneFadeInOutVariationValueSet((uint32_t)frame, (uint32_t)p_layer->fade_speed | (1 << 8));
        }
    }

    HW_GLCD_GRplaneAlphaBlendingPlaneSet(frame, DISPLAY_PLANE_BLEND_ON_LOWER_LAYER);
    HW_GLCD_GRplaneFrameEnable(frame);
}  /* End of function r_glcd_grahics_layer_blend_condition_set() */

/*******************************************************************************************************************//**
 * @brief   Subroutine to recalculate the configuration for the graphics planes. This routine recalculates the layer
 *           configuration if the layer is beyond the left/right/top/bottom end of the active video region.
 * @param[in]     p_input         The input frame buffer configuration
 * @param[in]     p_layer         The layer configuration
 * @param[in]     frame           The number of input frame buffer
 * @param[in,out] p_recalculated  Pointer to store recalculated parameter
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_graphics_layer_param_recalculation (display_input_cfg_t const * const p_input,
                                                       display_layer_t const * const     p_layer,
                                                       recalculated_param_t              * p_recalculated)
{
    uint16_t bit_size    = r_glcd_get_bit_size(p_input->format);
    int32_t  line_offset = (int32_t)((p_input->hstride * bit_size) / 8);

    p_recalculated->hpix_size   = 0;
    p_recalculated->vpix_size   = 0;
    p_recalculated->hpix_offset = 0;
    p_recalculated->vpix_offset = 0;

    if ((ctrl_blk.hsize >= p_layer->coordinate.x) && (0 <= p_layer->coordinate.x))
    {
        /** If the offset of the graphics layer is less than or equal to the right end of the display window,
         *  calculate actual pixel size to display
         */
        if ((ctrl_blk.hsize - (uint16_t)p_layer->coordinate.x) < p_input->hsize)
        {
            /** Calculate actual pixel size, the pixels to be displayed is less than the display window size */
            p_recalculated->hpix_size = (uint16_t)(ctrl_blk.hsize - (uint16_t)p_layer->coordinate.x);
        }
        else
        {
            /** Actual pixel size to display is same as the display window size */
            p_recalculated->hpix_size = p_input->hsize;
        }

        p_recalculated->hpix_offset  = p_layer->coordinate.x;
        p_recalculated->base_address = (uint32_t) p_input->p_base;
    }
    else if (0 > p_layer->coordinate.x)
    {
        /** If the offset of the graphics layer is less than the left end of the display window,
         *  calculate the actual pixel size to display
         */

        /** Calculate the actual pixel size, when the pixels to be displayed is less than display window size.
         *  If coordinate.x is a minus value, the layer image position can be adjusted not only by adjusting
         *  the horizontal offset but also by changing the base address of layer image. Due to the base address
         *  having to be aligned to 64 bytes, the horizontal offset and size has to be calculated to compensate
         *  sliding the picture horizontally. Note that this calculation is difficult.
         */
        int16_t minus_offset = (int16_t)(OFFSET_MARGIN_MINUS_64PIX / (bit_size / 8));
        p_recalculated->hpix_offset = (int16_t)(((p_layer->coordinate.x * (-1)) % minus_offset) * (-1));
        p_recalculated->hpix_size   =
            (uint16_t)((p_input->hsize + p_layer->coordinate.x) + ((p_layer->coordinate.x * (-1)) % minus_offset));

        /** Base address must be aligned to a 64-bit address */
        uint32_t offset_address = (uint32_t)(((p_layer->coordinate.x * (-1)) * bit_size) / 8);
        p_recalculated->base_address = ((uint32_t) p_input->p_base + offset_address) & (uint32_t)(~0x3F);
    }
    else
    {
        /** If graphics layer offset is beyond the display window size, set the pixel size to display to zero */
        p_recalculated->hpix_size    = 0;
        p_recalculated->base_address = (uint32_t) p_input->p_base;
    }

    /** Get data size to be read in a line */
    p_recalculated->hread_size = ((uint32_t) p_recalculated->hpix_size * bit_size) / 8;

    /** If line repeat mode is enabled, data number to be read (in a line) is multiplied by line number of image */
    if (p_input->lines_repeat_enable)
    {
        p_recalculated->hread_size *= p_input->vsize;
    }

    /** Calculate data size to be aligned to a 64-byte boundary. This is required by hardware */
    if (p_recalculated->hread_size % GLCD_ADDRESS_ALIGNMENT_64B)
    {
        p_recalculated->hread_size = (p_recalculated->hread_size & (uint32_t)(~0x3F)) + GLCD_ADDRESS_ALIGNMENT_64B;
    }

    if ((ctrl_blk.vsize >= p_layer->coordinate.y) && (0 <= p_layer->coordinate.y))
    {
        /** If graphics layer offset is less than or equal to display window size, calculate the actual pixel size to
         * display */
        if ((ctrl_blk.vsize - p_layer->coordinate.y) < p_input->vsize)
        {
            /** Actual pixel size to display is less than display window size */
            p_recalculated->vpix_size = (uint16_t)(ctrl_blk.vsize - p_layer->coordinate.y);
        }
        else
        {
            /** Actual pixel size to display is the same as display window size */
            p_recalculated->vpix_size = p_input->vsize;
        }

        p_recalculated->vpix_offset = p_layer->coordinate.y;
    }
    else if (0 > p_layer->coordinate.y)
    {
        /** If the offset of the graphics layer is less than the top end of display window,
         *  calculate actual pixel size to display
         */
        /** Calculate actual pixel size, the pixels to be displayed is less than the display window size */
        p_recalculated->vpix_size     = (uint16_t)(p_input->vsize + p_layer->coordinate.y);
        p_recalculated->base_address += (uint32_t)(line_offset * ((p_layer->coordinate.y) * (-1)));
    }
    else
    {
        p_recalculated->vpix_size = 0;
    }
}  /* End of function r_glcd_graphics_layer_param_recalculation() */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the graphics layer register settings which includes...
 *          - Blend setting of foreground or background plane on background plane
 *          - Rectangle area blending settings
 * @param[in]     p_input         The input frame buffer configuration
 * @param[in]     p_layer         The layer configuration
 * @param[in]     frame           The number of input frame buffer
 * @retval        void
 * @note    This function does not perform parameter check and it would be expected to be done in the caller function.
 **********************************************************************************************************************/
static void r_glcd_grahics_layer_set (display_input_cfg_t const * const p_input, display_layer_t const * const p_layer,
                                      display_frame_layer_t const frame)
{
    uint32_t             bit_size    = r_glcd_get_bit_size(p_input->format);
    int32_t              line_offset = (int32_t)((p_input->hstride * bit_size) / 8);
    recalculated_param_t recalculated;

    /** If the base address is NULL, just set the later transparent and disable read memory access */
    if (NULL == p_input->p_base)
    {
        HW_GLCD_GRplaneAlphaBlendingPlaneSet(frame, DISPLAY_PLANE_BLEND_TRANSPARENT);    /* Set layer transparent */
        HW_GLCD_GRplaneFrameDisable(frame);
        return;
    }

    r_glcd_graphics_plane_format_set(p_input->format, frame);

    r_glcd_graphics_layer_param_recalculation(p_input, p_layer, &recalculated);

    /** Set the base address of graphics plane */
    HW_GLCD_GRplaneBaseAddress(frame, recalculated.base_address);

    /** Set the background color on graphics plane */
    HW_GLCD_GRplaneBGColorSet(frame, p_layer->bg_color);

    /** Set the number of data transfer times per line, 64 bytes are transferred in each transfer */
    /* set (actual transfer times - 1) */
    HW_GLCD_GRplaneFrameHtransSizeSet(frame, (recalculated.hread_size >> 6) - 1);

    /** If line number descending mode is enable, change its sign */
    if (p_input->line_descending_enable)
    {
        line_offset *= (-1);
    }

    if (p_input->lines_repeat_enable)
    {
        /** Set the line offset address for accessing the graphics data on graphics plane */
        HW_GLCD_GRplaneLineNumberSet(frame, (uint32_t)(p_input->lines_repeat_times - 1));

        /** When line repeating mode, always read data on same line(s) */
        HW_GLCD_GRplaneLineOffsetSet(frame, 0);
    }
    else
    {
        /** Set the line offset address for accessing the graphics data */
        HW_GLCD_GRplaneLineNumberSet(frame, (uint32_t)(recalculated.vpix_size - 1));     /* set (vertical size - 1) */

        /** Set the line offset address for accessing the graphics data on graphics plane */
        HW_GLCD_GRplaneLineOffsetSet(frame, (uint32_t)line_offset);
    }

    /** Set the frame number of the graphics plane */
    HW_GLCD_GRplaneFrameNumberSet(frame, 0);

    /** Set the frame offset for accessing the graphics data on the graphics plane */
    HW_GLCD_GRplaneFrameOffsetAddressSet(frame, 0);

    /** Set the start position of the graphics layers */
    HW_GLCD_GRplaneAlphaBlendingHorizontalStartPositionSet(frame,
                                                          (uint32_t)(ctrl_blk.zero_coordinate.x + recalculated.hpix_offset));
    HW_GLCD_GRplaneAlphaBlendingVerticalStartPositionSet(frame,
                                                          (uint32_t)(ctrl_blk.zero_coordinate.y + recalculated.vpix_offset));

    /** Set the start position of the rectangle area in the graphics layers */
    HW_GLCD_GRplaneAlphaBlendingRectangleHorizontalStartPositionSet(frame,
                                                          (uint32_t)(ctrl_blk.zero_coordinate.x + recalculated.hpix_offset));
    HW_GLCD_GRplaneAlphaBlendingRectangleVerticalStartPositionSet(frame,
                                                          (uint32_t)(ctrl_blk.zero_coordinate.y + recalculated.vpix_offset));

    /** Set the width of the graphics layers */
    HW_GLCD_GRplaneAlphaBlendingHorizontalWidthSet(frame, recalculated.hpix_size);
    HW_GLCD_GRplaneAlphaBlendingRectangleHorizontalWidthSet(frame, recalculated.hpix_size);
    if (p_input->lines_repeat_enable)
    {
        HW_GLCD_GRplaneAlphaBlendingVerticalWidthSet(frame, (uint32_t)(recalculated.vpix_size * p_input->lines_repeat_times));
        HW_GLCD_GRplaneAlphaBlendingRectangleVerticalWidthSet(frame,
                                                            (uint32_t)(recalculated.vpix_size * p_input->lines_repeat_times));
    }
    else
    {
        HW_GLCD_GRplaneAlphaBlendingVerticalWidthSet(frame, recalculated.vpix_size);
        HW_GLCD_GRplaneAlphaBlendingRectangleVerticalWidthSet(frame, recalculated.vpix_size);
    }

    /** Set the alpha blending condition */
    r_glcd_grahics_layer_blend_condition_set(p_layer, frame);
}  /* End of function r_glcd_grahics_layer_set() */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the output control block register settings which includes...
 *          - Bit endian / color order setting
 *          - Output color setting
 *          - Color correction setting
 * @param[in]     p_cfg   Pointer to the configuration structure for display interface
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_output_block_set (display_cfg_t const * const p_cfg)
{
    glcd_cfg_t * pextend = (glcd_cfg_t *) p_cfg->p_extend;

    if (!p_cfg->p_extend)
    {
        /** Applied default configuration if the GLCD HAL configuration is NULL */
        pextend = (glcd_cfg_t *) &default_glcd_cfg;
    }

    /** selects big or little endian for output data */
    if (DISPLAY_ENDIAN_LITTLE == p_cfg->output.endian)
    {
        HW_GLCD_OutputInterfaceLittleEndianSet();
    }
    else
    {
        HW_GLCD_OutputInterfaceBigEndianSet();
    }

    /** selects the output byte order swapping */
    if (DISPLAY_COLOR_ORDER_RGB == p_cfg->output.color_order)
    {
        HW_GLCD_OutputInterfaceSwapOff();
    }
    else
    {
        HW_GLCD_OutputInterfaceSwapOn();
    }

    /** selects the output format */
    switch (p_cfg->output.format)
    {
        case DISPLAY_OUT_FORMAT_24BITS_RGB888:
            HW_GLCD_OutputInterfaceFormatSet(GLCD_OUTPUT_INTERFACE_FORMAT_RGB888);
            HW_GLCD_OutputDitherFormatSet(GLCD_DITHERING_OUTPUT_FORMAT_RGB888);
            break;
        case DISPLAY_OUT_FORMAT_18BITS_RGB666:
            HW_GLCD_OutputInterfaceFormatSet(GLCD_OUTPUT_INTERFACE_FORMAT_RGB666);
            HW_GLCD_OutputDitherFormatSet(GLCD_DITHERING_OUTPUT_FORMAT_RGB666);
            break;
        case DISPLAY_OUT_FORMAT_16BITS_RGB565:
            HW_GLCD_OutputInterfaceFormatSet(GLCD_OUTPUT_INTERFACE_FORMAT_RGB565);
            HW_GLCD_OutputDitherFormatSet(GLCD_DITHERING_OUTPUT_FORMAT_RGB565);
            break;
        case DISPLAY_OUT_FORMAT_8BITS_SERIAL:        /** In case of serial RGB, set as RGB888 format */
            HW_GLCD_OutputInterfaceFormatSet(GLCD_OUTPUT_INTERFACE_FORMAT_SERIAL_RGB);
            HW_GLCD_OutputDitherFormatSet(GLCD_DITHERING_OUTPUT_FORMAT_RGB888);
            break;
        default:
            HW_GLCD_OutputInterfaceFormatSet(GLCD_OUTPUT_INTERFACE_FORMAT_RGB888);
            HW_GLCD_OutputDitherFormatSet(GLCD_DITHERING_OUTPUT_FORMAT_RGB888);
            break;
    }

    /** sets the pixel clock (the GLCD internal signal) frequency in case that the output format is 8-bit serial RGB */
    if (DISPLAY_OUT_FORMAT_8BITS_SERIAL != p_cfg->output.format)
    {
        HW_GLCD_OutputInterfaceClockFreqNoDivision();
    }
    else
    {
        HW_GLCD_OutputInterfaceClockFreqQuaterDivision();
    }

    /** sets the Brightness/contrast and Gamma Correction processing order */
    if (GLCD_CORRECTION_PROC_ORDER_BRIGHTNESS_CONTRAST2GAMMA == pextend->correction_proc_order)
    {
        HW_GLCD_CorrectionProcOrderBrightnessContrastFirst();
    }
    else
    {
        HW_GLCD_CorrectionProcOrderGammaFirst();
    }

    /** Set the dithering mode */
    if (p_cfg->output.dithering_on)
    {
        if (GLCD_DITHERING_MODE_2X2PATTERN == pextend->dithering_mode)
        {
            HW_GLCD_DitheringModeSet(GLCD_DITHERING_MODE_2X2PATTERN);
            HW_GLCD_OutputDitherPatternASet(pextend->dithering_pattern_A);
            HW_GLCD_OutputDitherPatternBSet(pextend->dithering_pattern_B);
            HW_GLCD_OutputDitherPatternCSet(pextend->dithering_pattern_C);
            HW_GLCD_OutputDitherPatternDSet(pextend->dithering_pattern_D);
        }
        else
        {
            HW_GLCD_DitheringModeSet(GLCD_DITHERING_MODE_ROUND_OFF);
        }
    }
    else
    {
        HW_GLCD_DitheringModeSet(GLCD_DITHERING_MODE_TRUNCATE);
    }
}  /* End of function r_glcd_output_block_set() */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the brightness register settings. Pixel color output comes to be the value
 *           shown below processed by the brightness control block.
 *          - Gout = Gin + p_cfg->output.brightness.g - 512 (output.brightness.g must be 10 bits value; up to 512)
 *          - Bout = Bin + p_cfg->output.brightness.b - 512 (output.brightness.b must be 10 bits value; up to 512)
 *          - Rout = Rin + p_cfg->output.brightness.r - 512 (output.brightness.r must be 10 bits value; up to 512)
 * @param[in]  p_ctrl       Pointer to the control block for Display Interface
 * @param[in]  p_brightness Pointer to brightness configuration structure
 * @retval     void
 **********************************************************************************************************************/
static void r_glcd_brightness_correction (display_ctrl_t const * const       p_ctrl,
                                          display_brightness_t const * const p_brightness)
{
    uint16_t r;
    uint16_t g;
    uint16_t b;

    if (p_brightness->enable)
    {
        g = p_brightness->g;
        b = p_brightness->b;
        r = p_brightness->r;

        /** Sets brightness correction register for each color in a pixel. */
        HW_GLCD_OutputBrightnessCorrectionGreen(g);
        HW_GLCD_OutputBrightnessCorrectionBlue(b);
        HW_GLCD_OutputBrightnessCorrectionRed(r);
    }
    else
    {
        if (DISPLAY_STATE_CLOSED == p_ctrl->state)
        {
            /** If brightness setting in configuration is 'off', apply default value */
            HW_GLCD_OutputBrightnessCorrectionGreen(GLCD_BRIGHTNESS_DEFAULT);
            HW_GLCD_OutputBrightnessCorrectionBlue(GLCD_BRIGHTNESS_DEFAULT);
            HW_GLCD_OutputBrightnessCorrectionRed(GLCD_BRIGHTNESS_DEFAULT);
        }
    }
}  /* End of function r_glcd_brightness_correction() */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the contrast register settings. Pixel color output becomes the value
 *           shown below, processed by the contrast control block. Contrast can be changed between x0.000 to x1.992
 *           (0x0:x0.000 / 0x80:x1.000 / 0xFF:x1.992).
 *          - Gout = (Gin + p_contrast->g)/128
 *          - Bout = (Bin + p_contrast->b)/128
 *          - Rout = (Rin + p_contrast->r)/128
 * @param[in]  p_ctrl     Pointer to the control block for the Display Interface
 * @param[in]  p_contrast Pointer to the contrast configuration structure
 * @retval     void
 **********************************************************************************************************************/
static void r_glcd_contrast_correction (display_ctrl_t const * const     p_ctrl,
                                        display_contrast_t const * const p_contrast)
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    if (p_contrast->enable)
    {
        g = p_contrast->g;
        b = p_contrast->b;
        r = p_contrast->r;

        /** Sets the contrast correction register for each color in a pixel. */
        HW_GLCD_OutputContrastCorrectionGreen(g);
        HW_GLCD_OutputContrastCorrectionBlue(b);
        HW_GLCD_OutputContrastCorrectionRed(r);
    }
    else
    {
        if (DISPLAY_STATE_CLOSED == p_ctrl->state)
        {
            /** If the contrast setting in the configuration is set to 'off', apply default value */
            HW_GLCD_OutputContrastCorrectionGreen(GLCD_CONTRAST_DEFAULT);
            HW_GLCD_OutputContrastCorrectionBlue(GLCD_CONTRAST_DEFAULT);
            HW_GLCD_OutputContrastCorrectionRed(GLCD_CONTRAST_DEFAULT);
        }
    }
}  /* End of function r_glcd_contrast_correction() */

/*******************************************************************************************************************//**
 * @brief   Subroutine to configure the gamma correction register setting.
 * @param[in]     p_cfg   Pointer to the configuration structure for the display interface
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_gamma_correction (display_cfg_t const * const p_cfg)
{
    uint8_t i;
    if (p_cfg->output.p_gamma_correction->g.enable)
    {
        for (i = 0; i < (DISPLAY_GAMMA_CURVE_ELEMENT_NUM / 2); i++)
        {
            HW_GLCD_GammaCorrectionGreenGainUpper16BitsSet(i, p_cfg->output.p_gamma_correction->g.gain[i * 2]);
            HW_GLCD_GammaCorrectionGreenGainLower16BitsSet(i, p_cfg->output.p_gamma_correction->g.gain[(i * 2) + 1]);
        }

        for (i = 0; i < (DISPLAY_GAMMA_CURVE_ELEMENT_NUM / 3); i++)
        {
            HW_GLCD_GammaCorrectionGreenThresholdHigherBitsSet(i,
                                                               p_cfg->output.p_gamma_correction->g.threshold[(i *
                                                                                                              3) + 1]);
            HW_GLCD_GammaCorrectionGreenThresholdMiddleBitsSet(i,
                                                               p_cfg->output.p_gamma_correction->g.threshold[(i *
                                                                                                              3) + 2]);
            HW_GLCD_GammaCorrectionGreenThresholdLowerBitsSet(i,
                                                              p_cfg->output.p_gamma_correction->g.threshold[(i *
                                                                                                             3) + 3]);
        }

        HW_GLCD_GammaCorrectionGreenOn();
        HW_GLCD_GammaUpdateGreen();
    }
    else
    {
        HW_GLCD_GammaCorrectionGreenOff();
        HW_GLCD_GammaUpdateGreen();
    }

    if (p_cfg->output.p_gamma_correction->b.enable)
    {
        for (i = 0; i < (DISPLAY_GAMMA_CURVE_ELEMENT_NUM / 2); i++)
        {
            HW_GLCD_GammaCorrectionBlueGainUpper16BitsSet(i, p_cfg->output.p_gamma_correction->b.gain[i * 2]);
            HW_GLCD_GammaCorrectionBlueGainLower16BitsSet(i, p_cfg->output.p_gamma_correction->b.gain[(i * 2) + 1]);
        }

        for (i = 0; i < (DISPLAY_GAMMA_CURVE_ELEMENT_NUM / 3); i++)
        {
            HW_GLCD_GammaCorrectionBlueThresholdHigherBitsSet(i,
                                                              p_cfg->output.p_gamma_correction->b.threshold[(i *
                                                                                                             3) + 1]);
            HW_GLCD_GammaCorrectionBlueThresholdMiddleBitsSet(i,
                                                              p_cfg->output.p_gamma_correction->b.threshold[(i *
                                                                                                             3) + 2]);
            HW_GLCD_GammaCorrectionBlueThresholdLowerBitsSet(i,
                                                             p_cfg->output.p_gamma_correction->b.threshold[(i *
                                                                                                            3) + 3]);
        }

        HW_GLCD_GammaCorrectionBlueOn();
        HW_GLCD_GammaUpdateBlue();
    }
    else
    {
        HW_GLCD_GammaCorrectionBlueOff();
        HW_GLCD_GammaUpdateBlue();
    }

    if (p_cfg->output.p_gamma_correction->r.enable)
    {
        for (i = 0; i < (DISPLAY_GAMMA_CURVE_ELEMENT_NUM / 2); i++)
        {
            HW_GLCD_GammaCorrectionRedGainUpper16BitsSet(i, p_cfg->output.p_gamma_correction->r.gain[i * 2]);
            HW_GLCD_GammaCorrectionRedGainLower16BitsSet(i, p_cfg->output.p_gamma_correction->r.gain[(i * 2) + 1]);
        }

        for (i = 0; i < (DISPLAY_GAMMA_CURVE_ELEMENT_NUM / 3); i++)
        {
            HW_GLCD_GammaCorrectionRedThresholdHigherBitsSet(i,
                                                             p_cfg->output.p_gamma_correction->r.threshold[(i *
                                                                                                            3) + 1]);
            HW_GLCD_GammaCorrectionRedThresholdMiddleBitsSet(i,
                                                             p_cfg->output.p_gamma_correction->r.threshold[(i *
                                                                                                            3) + 2]);
            HW_GLCD_GammaCorrectionRedThresholdLowerBitsSet(i,
                                                            p_cfg->output.p_gamma_correction->r.threshold[(i * 3) + 3]);
        }

        HW_GLCD_GammaCorrectionRedOn();
        HW_GLCD_GammaUpdateRed();
    }
    else
    {
        HW_GLCD_GammaCorrectionRedOff();
        HW_GLCD_GammaUpdateRed();
    }
}  /* End of function r_glcd_gamma_correction() */

/*******************************************************************************************************************//**
 * @brief   Subroutine to get the bit size of the specified format.
 * @param[in]     format   Color format (specify display_in_format_t type enumeration value)
 * @retval        Bit size
 **********************************************************************************************************************/
static uint16_t r_glcd_get_bit_size (display_in_format_t const format)
{
    uint16_t bit_size = 0;

    /** Get bit size and set color format  */
    switch (format)
    {
        case DISPLAY_IN_FORMAT_32BITS_ARGB8888:         ///< ARGB8888, 32bits
        case DISPLAY_IN_FORMAT_32BITS_RGB888:           ///< RGB888,   32bits
            bit_size = 32;
            break;
        case DISPLAY_IN_FORMAT_16BITS_RGB565:           ///< RGB565,   16bits
        case DISPLAY_IN_FORMAT_16BITS_ARGB1555:         ///< ARGB1555, 16bits
        case DISPLAY_IN_FORMAT_16BITS_ARGB4444:         ///< ARGB4444, 16bits
            bit_size = 16;
            break;
        case DISPLAY_IN_FORMAT_CLUT8:                   ///< CLUT8
            bit_size = 8;
            break;
        case DISPLAY_IN_FORMAT_CLUT4:                   ///< CLUT4
            bit_size = 4;
            break;
        case DISPLAY_IN_FORMAT_CLUT1:                   ///< CLUT1
            bit_size = 1;
            break;
        default:
            break;
    }

    return bit_size;
}  /* End of function r_glcd_get_bit_size () */

/*******************************************************************************************************************//**
 * @brief   Subroutine to set the color format of graphics plane to the GLCD register.
 * @param[in]     format   Color format (specify display_in_format_t type enumeration value)
 * @param[in]     frame    The number of input frame buffer
 * @retval        void
 **********************************************************************************************************************/
static void r_glcd_graphics_plane_format_set (display_in_format_t const format, display_frame_layer_t const frame)
{
    switch (format)
    {
        case DISPLAY_IN_FORMAT_32BITS_ARGB8888:         ///< ARGB8888, 32bits
            HW_GLCD_GRplaneFormatSet(frame, GLCD_INPUT_INTERFACE_FORMAT_ARGB8888);
            break;
        case DISPLAY_IN_FORMAT_32BITS_RGB888:           ///< RGB888, 32bits
            HW_GLCD_GRplaneFormatSet(frame, GLCD_INPUT_INTERFACE_FORMAT_RGB888);
            break;
        case DISPLAY_IN_FORMAT_16BITS_RGB565:           ///< RGB565,   16bits
            HW_GLCD_GRplaneFormatSet(frame, GLCD_INPUT_INTERFACE_FORMAT_RGB565);
            break;
        case DISPLAY_IN_FORMAT_16BITS_ARGB1555:         ///< ARGB1555, 16bits
            HW_GLCD_GRplaneFormatSet(frame, GLCD_INPUT_INTERFACE_FORMAT_ARGB1555);
            break;
        case DISPLAY_IN_FORMAT_16BITS_ARGB4444:         ///< ARGB4444, 16bits
            HW_GLCD_GRplaneFormatSet(frame, GLCD_INPUT_INTERFACE_FORMAT_ARGB4444);
            break;
        case DISPLAY_IN_FORMAT_CLUT8:                   ///< CLUT8
            HW_GLCD_GRplaneFormatSet(frame, GLCD_INPUT_INTERFACE_FORMAT_CLUT8);
            break;
        case DISPLAY_IN_FORMAT_CLUT4:                   ///< CLUT4
            HW_GLCD_GRplaneFormatSet(frame, GLCD_INPUT_INTERFACE_FORMAT_CLUT4);
            break;
        case DISPLAY_IN_FORMAT_CLUT1:                   ///< CLUT1
            HW_GLCD_GRplaneFormatSet(frame, GLCD_INPUT_INTERFACE_FORMAT_CLUT1);
            break;
        default:
            break;
    }
}  /* End of function r_glcd_graphics_plane_format_set () */

/*******************************************************************************************************************//**
 * @brief   Subroutine to decide the number of horizontal offset pixels based on pixel format for graphics screens.
 *          The purpose of this function is for the driver's supporting a screen transition to slide image in the
 *          horizontal direction with 1 pixel unit. The GLCD hardware supports horizontal slide by shifting the start
 *          address of the screen but the address must be aligned to 64bytes due to the hardware specification, so that
 *          the driver cannot achieve a smooth sliding by simple changing the start address. The driver can achieve
 *          a smooth sliding by 2 of technique, one is the address shift and second is giving a pixel offset in the
 *          blanking period. This function provides the number of pixel offset to achieve the functionality.
 * @param[in]     p_cfg    Pointer to the display configuration structure
 * @retval        The number of offset pixels
 **********************************************************************************************************************/
static uint16_t r_glcd_calc_output_horizontal_pixel_offset (display_cfg_t const * const p_cfg)
{
    uint16_t bit_size0 = r_glcd_get_bit_size(p_cfg->input[0].format);
    uint16_t bit_size1 = r_glcd_get_bit_size(p_cfg->input[1].format);
    if ((32 == bit_size0) || (32 == bit_size1))
    {
        /* If 32bpp case, pixel offset needed to achieve smooth sliding is (64bytes / 4) */
        return GLCD_ADDRESS_ALIGNMENT_64B / 4;
    }
    else if ((16 == bit_size0) || (16 == bit_size1))
    {
        /* If 32bpp case, pixel offset needed to achieve smooth sliding is (64bytes / 2) */
        return GLCD_ADDRESS_ALIGNMENT_64B / 2;
    }
    else
    {
        /* If 8bpp case, pixel offset needed to achieve smooth sliding is 64bytes
         * The color formats less than 8bpp needs more offset but it is not practical so return this value
         * in case of such formats */
        return GLCD_ADDRESS_ALIGNMENT_64B;
    }
}  /* End of function r_glcd_calc_output_horizontal_pixel_offset () */

#if (BSP_IRQ_CFG_GLCDC_LINE_DETECT != BSP_IRQ_DISABLED)

/*******************************************************************************************************************//**
 * @brief   The line detection interrupt service routine.
 *           This ISR is called when the number of the display line reaches the designated number of lines. If a
 *           callback function is registered in R_GLCD_Open(), it is called from this ISR and the
 *           DISPLAY_EVENT_LINE_DETECTION event code is set as its argument.
 * @param[in]     none
 * @retval        none
 **********************************************************************************************************************/
void lcdc_lcdc_level_0_isr (void)
{
    SF_CONTEXT_SAVE

    display_callback_args_t args;
    display_ctrl_t          * pctrl = (display_ctrl_t *) ctrl_blk.p_context;

    /** Call back callback function if it is registered */
    if (NULL != pctrl->p_callback)
    {
        args.event     = DISPLAY_EVENT_LINE_DETECTION;
        args.p_context = pctrl->p_context;
        pctrl->p_callback(&args);
    }

    /** Clear interrupt flag in the register of the GLCD module */
    HW_GLCD_VposInterruptStatusClear();

    /** Clear interrupt flag in the register of the NVIC module */
    R_BSP_IrqStatusClear(GLCDC_LINE_DETECT_IRQn);

    SF_CONTEXT_RESTORE
}  /* End of function lcdc_lcdc_level_0_isr() */
#endif /* if (BSP_IRQ_CFG_GLCDC_LINE_DETECT != BSP_IRQ_DISABLED) */

#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_1 != BSP_IRQ_DISABLED)

/*******************************************************************************************************************//**
 * @brief   The graphics plane 1 underflow detection interrupt service routine.
 *           This ISR is called when the underflow occurs in the graphics plane 1 control block. If a callback function
 *           is registered in R_GLCD_Open(), it is called back from this ISR and the DISPLAY_EVENT_GR1_UNDERFLOW event
 *           code is set as its argument.
 * @param[in]     none
 * @retval        none
 **********************************************************************************************************************/
void lcdc_lcdc_level_1_isr (void)
{
    SF_CONTEXT_SAVE

    display_callback_args_t args;
    display_ctrl_t          * pctrl = (display_ctrl_t *) ctrl_blk.p_context;

    /** Call back callback function if it is registered */
    if (NULL != pctrl->p_callback)
    {
        args.event     = DISPLAY_EVENT_GR1_UNDERFLOW;
        args.p_context = pctrl->p_context;
        pctrl->p_callback(&args);
    }

    /** Clear interrupt flag in the register of the GLCD module */
    HW_GLCD_GR1UnderflowInterruptStatusClear();

    /** Clear interrupt flag in the register of the NVIC module */
    R_BSP_IrqStatusClear(GLCDC_UNDERFLOW_1_IRQn);

    SF_CONTEXT_RESTORE
}  /* End of function lcdc_lcdc_level_1_isr() */
#endif /* if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_1 != BSP_IRQ_DISABLED) */

#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_2 != BSP_IRQ_DISABLED)

/*******************************************************************************************************************//**
 * @brief   The graphics plane 2 underflow detection interrupt service routine.
 *           This ISR is called when the underflow occurs in the graphics plane 2 control block. If a callback function
 *           is registered in R_GLCD_Open(), it is called from this ISR and the DISPLAY_EVENT_GR2_UNDERFLOW event
 *           code is set as its argument.
 * @param[in]     none
 * @retval        none
 **********************************************************************************************************************/
void lcdc_lcdc_level_2_isr (void)
{
    SF_CONTEXT_SAVE

    display_callback_args_t args;
    display_ctrl_t          * pctrl = (display_ctrl_t *) ctrl_blk.p_context;

    /** Call the callback function if it is registered */
    if (NULL != pctrl->p_callback)
    {
        args.event     = DISPLAY_EVENT_GR2_UNDERFLOW;
        args.p_context = pctrl->p_context;
        pctrl->p_callback(&args);
    }

    /** Clear interrupt flag in the register of the GLCD module */
    HW_GLCD_GR2UnderflowInterruptStatusClear();

    /** Clear interrupt flag in the register of the NVIC module */
    R_BSP_IrqStatusClear(GLCDC_UNDERFLOW_2_IRQn);

    SF_CONTEXT_RESTORE
}  /* End of function lcdc_lcdc_level_2_isr() */
#endif /* if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_2 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @} (end addtogroup GLCD)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @} (end addtogroup GLCD_Library)
 **********************************************************************************************************************/
