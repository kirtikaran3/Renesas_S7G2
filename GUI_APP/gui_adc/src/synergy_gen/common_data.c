/* generated common source file - do not edit */
#include "common_data.h"
#if JPEG_DECODE_CALLBACK_USED_g_jpeg_decode0
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(jpeg_decode_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif

static jpeg_decode_ctrl_t g_jpeg_decode0_ctrl;
const jpeg_decode_cfg_t g_jpeg_decode0_cfg =
{ .input_data_format = JPEG_DECODE_DATA_FORMAT_NORMAL, .output_data_format = JPEG_DECODE_DATA_FORMAT_NORMAL,
  .pixel_format = JPEG_DECODE_PIXEL_FORMAT_RGB565, .alpha_value = 255, .p_callback = NULL, };
const jpeg_decode_instance_t g_jpeg_decode0 =
{ .p_api = (jpeg_decode_api_t const *) &g_jpeg_decode_on_jpeg_decode, .p_ctrl = &g_jpeg_decode0_ctrl, .p_cfg =
          &g_jpeg_decode0_cfg };

#if JPEG_DECODE_CALLBACK_USED_g_jpeg_decode0
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function for JPEG Decode HAL Driver. It should be overridden by defining
 *             a user callback function with the prototype below.
 *               - void NULL(jpeg_decode_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(jpeg_decode_callback_args_t * p_args)
{
    /** Do nothing. */
}
#endif
static sf_jpeg_decode_ctrl_t g_sf_jpeg_decode0_ctrl;

static const sf_jpeg_decode_cfg_t g_sf_jpeg_decode0_cfg =
{ .p_lower_lvl_jpeg_decode = (jpeg_decode_instance_t const *) &g_jpeg_decode0 };
const sf_jpeg_decode_instance_t g_sf_jpeg_decode0 =
{ .p_api = &g_sf_jpeg_decode_on_sf_jpeg_decode, .p_ctrl = &g_sf_jpeg_decode0_ctrl, .p_cfg =
          (sf_jpeg_decode_cfg_t const *) &g_sf_jpeg_decode0_cfg };
#if defined(DISPLAY_ON_GLCD_CALLBACK_USED)
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
void NULL(display_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
#endif
/** Display frame buffer */
#if (true)
#ifdef __GNUC__
uint8_t g_display_fb_background[2][((256 * 320) * DISPLAY_BITS_PER_PIXEL_INPUT0) >> 3] __attribute__ ((aligned(64), section(".bss")));
#else /* __ICCARM__ */
#pragma data_alignment = 64
#pragma location = ".bss"
uint8_t g_display_fb_background[2][((256 * 320) * DISPLAY_BITS_PER_PIXEL_INPUT0) >> 3];
#endif
#else
/** Graphics screen1 is specified not to be used when starting */
#endif
#if (false)
#ifdef __GNUC__
uint8_t g_display_fb_foreground[2][((256 * 320) * DISPLAY_BITS_PER_PIXEL_INPUT1) >> 3] __attribute__ ((aligned(64), section(".bss")));
#else /* __ICCARM__ */
#pragma data_alignment = 64
#pragma location = ".bss"
uint8_t g_display_fb_foreground[2][((256 * 320) * DISPLAY_BITS_PER_PIXEL_INPUT1) >> 3];
#endif
#else
/** Graphics screen2 is specified not to be used when starting */
#endif

#if (false)
/** Display CLUT buffer to be used for updating CLUT */
static uint32_t CLUT_buffer[256];

/** Display CLUT configuration(only used if using CLUT format) */
display_clut_cfg_t g_display_clut_cfg_glcd =
{
    .p_base = (uint32_t *)CLUT_buffer,
    .start = 0, /* User have to update this setting when using */
    .size = 256 /* User have to update this setting when using */
};
#else
/** CLUT is specified not to be used */
#endif

#if (false | false | false)
/** Display interface configuration */
static const display_gamma_correction_t g_display_gamma_cfg =
{
    .r =
    {
        .enable = false,
        .gain =
        {   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .threshold =
        {   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    .g =
    {
        .enable = false,
        .gain =
        {   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .threshold =
        {   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    .b =
    {
        .enable = false,
        .gain =
        {   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .threshold =
        {   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    }
};
#endif

/** Display device extended configuration */
static const glcd_cfg_t g_display_extend_cfg =
{ .tcon_hsync = GLCD_TCON_PIN_2, .tcon_vsync = GLCD_TCON_PIN_1, .tcon_de = GLCD_TCON_PIN_0, .correction_proc_order =
          GLCD_CORRECTION_PROC_ORDER_BRIGHTNESS_CONTRAST2GAMMA,
  .clksrc = GLCD_CLK_SRC_INTERNAL, .clock_div_ratio = GLCD_PANEL_CLK_DIVISOR_32, .dithering_mode =
          GLCD_DITHERING_MODE_TRUNCATE,
  .dithering_pattern_A = GLCD_DITHERING_PATTERN_11, .dithering_pattern_B = GLCD_DITHERING_PATTERN_11,
  .dithering_pattern_C = GLCD_DITHERING_PATTERN_11, .dithering_pattern_D = GLCD_DITHERING_PATTERN_11 };

/** Display control block instance */
display_ctrl_t g_display_ctrl;

/** Display interface configuration */
const display_cfg_t g_display_cfg =
{
/** Input1(Graphics1 screen) configuration */
.input[0] =
{
#if (true)
  .p_base = (uint32_t *)&g_display_fb_background[0],
#else
  .p_base = NULL,
#endif
  .hsize = 256,
  .vsize = 320, .hstride = 256, .format = DISPLAY_IN_FORMAT_16BITS_RGB565, .line_descending_enable = false,
  .lines_repeat_enable = false, .lines_repeat_times = 0 },

  /** Input2(Graphics2 screen) configuration */
  .input[1] =
  {
#if (false)
    .p_base = (uint32_t *)&g_display_fb_foreground[0],
#else
    .p_base = NULL,
#endif
    .hsize = 256,
    .vsize = 320, .hstride = 256, .format = DISPLAY_IN_FORMAT_16BITS_RGB565, .line_descending_enable = false,
    .lines_repeat_enable = false, .lines_repeat_times = 0 },

  /** Input1(Graphics1 screen) layer configuration */
  .layer[0] =
  { .coordinate =
  { .x = 0, .y = 0 },
    .bg_color =
    { .byte =
    { .a = 255, .r = 255, .g = 255, .b = 255 } },
    .fade_control = DISPLAY_FADE_CONTROL_NONE, .fade_speed = 0 },

  /** Input2(Graphics2 screen) layer configuration */
  .layer[1] =
  { .coordinate =
  { .x = 0, .y = 0 },
    .bg_color =
    { .byte =
    { .a = 255, .r = 255, .g = 255, .b = 255 } },
    .fade_control = DISPLAY_FADE_CONTROL_NONE, .fade_speed = 0 },

  /** Output configuration */
  .output =
  { .htiming =
  { .total_cyc = 320, .display_cyc = 240, .back_porch = 6, .sync_width = 4, .sync_polarity =
            DISPLAY_SIGNAL_POLARITY_LOACTIVE },
    .vtiming =
    { .total_cyc = 328, .display_cyc = 320, .back_porch = 4, .sync_width = 4, .sync_polarity =
              DISPLAY_SIGNAL_POLARITY_LOACTIVE },
    .format = DISPLAY_OUT_FORMAT_16BITS_RGB565, .endian = DISPLAY_ENDIAN_LITTLE, .color_order = DISPLAY_COLOR_ORDER_RGB,
    .data_enable_polarity = DISPLAY_SIGNAL_POLARITY_HIACTIVE, .sync_edge = DISPLAY_SIGNAL_SYNC_EDGE_RISING, .bg_color =
    { .byte =
    { .a = 255, .r = 0, .g = 0, .b = 0 } },
    .brightness =
    { .enable = false, .r = 512, .g = 512, .b = 512 },
    .contrast =
    { .enable = false, .r = 128, .g = 128, .b = 128 },
#if (false | false | false)
    .p_gamma_correction = (display_gamma_correction_t *)(&g_display_gamma_cfg),
#else
    .p_gamma_correction = NULL,
#endif
    .dithering_on = false },

  /** Display device callback function pointer */
  .p_callback = NULL,
  .p_context = (void *) &g_display,

  /** Display device extended configuration */
  .p_extend = (void *) (&g_display_extend_cfg) };

#if (true)
/** Display on GLCD run-time configuration(for the graphics1 screen) */
display_runtime_cfg_t g_display_runtime_cfg_bg =
{
    .input =
    {
#if (true)
        .p_base = (uint32_t *)&g_display_fb_background[0],
#else
        .p_base = NULL,
#endif
        .hsize = 256,
        .vsize = 320,
        .hstride = 256,
        .format = DISPLAY_IN_FORMAT_16BITS_RGB565,
        .line_descending_enable = false,
        .lines_repeat_enable = false,
        .lines_repeat_times = 0
    },
    .layer =
    {
        .coordinate =
        {
            .x = 0,
            .y = 0
        },
        .bg_color =
        {
            .byte =
            {
                .a = 255,
                .r = 255,
                .g = 255,
                .b = 255
            }
        },
        .fade_control = DISPLAY_FADE_CONTROL_NONE,
        .fade_speed = 0
    }
};
#endif
#if (false)
/** Display on GLCD run-time configuration(for the graphics2 screen) */
display_runtime_cfg_t g_display_runtime_cfg_fg =
{
    .input =
    {
#if (false)
        .p_base = (uint32_t *)&g_display_fb_foreground[0],
#else
        .p_base = NULL,
#endif
        .hsize = 256,
        .vsize = 320,
        .hstride = 256,
        .format = DISPLAY_IN_FORMAT_16BITS_RGB565,
        .line_descending_enable = false,
        .lines_repeat_enable = false,
        .lines_repeat_times = 0
    },
    .layer =
    {
        .coordinate =
        {
            .x = 0,
            .y = 0
        },
        .bg_color =
        {
            .byte =
            {
                .a = 255,
                .r = 255,
                .g = 255,
                .b = 255
            }
        },
        .fade_control = DISPLAY_FADE_CONTROL_NONE,
        .fade_speed = 0
    }
};
#endif

/* Instance structure to use this module. */
const display_instance_t g_display =
{ .p_ctrl = &g_display_ctrl, .p_cfg = (display_cfg_t *) &g_display_cfg, .p_api = (display_api_t *) &g_display_on_glcd };

#if defined(DISPLAY_ON_GLCD_CALLBACK_USED)
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(display_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(display_callback_args_t * p_args);
void NULL_internal(display_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
/** GUIX Canvas Buffer */
#if false
#ifdef __GNUC__
uint8_t g_sf_el_gx_canvas[sizeof(g_display_fb_background[0])] __attribute__ ((aligned(4), section(".bss")));
#else /* __ICCARM__ */
#pragma data_alignment = 4
#pragma location = ".bss"
uint8_t g_sf_el_gx_canvas[sizeof(g_display_fb_background[0])];
#endif
#endif

/** JPEG Work Buffer */
#ifdef GX_USE_SYNERGY_JPEG
#if (81920)
#ifdef __GNUC__
uint8_t g_sf_el_gx_jpegbuffer[81920] __attribute__ ((aligned(64), section(".bss")));
#else /* __ICCARM__ */
#pragma data_alignment = 64
#pragma location = ".bss"
uint8_t g_sf_el_gx_jpegbuffer[81920];
#endif
#endif
#endif

/** GUIX Port module control block instance */
static sf_el_gx_ctrl_t g_sf_el_gx_ctrl;

/** GUIX Port module configuration */
static const sf_el_gx_cfg_t g_sf_el_gx_cfg =
{ .p_display_instance = (display_instance_t *) &g_display, .p_display_runtime_cfg = &g_display_runtime_cfg_bg,
#if false
  .p_canvas = g_sf_el_gx_canvas,
#else
  .p_canvas = NULL,
#endif
  .p_framebuffer_a = g_display_fb_background[0],
  .p_framebuffer_b = g_display_fb_background[1], .p_callback = NULL,
#ifdef GX_USE_SYNERGY_JPEG
  .p_jpegbuffer = g_sf_el_gx_jpegbuffer,
  .jpegbuffer_size = 81920,
#else
  .p_jpegbuffer = NULL,
  .jpegbuffer_size = 0,
#endif
  .rotation_angle = 0 };

/** GUIX Port module instance */
sf_el_gx_instance_t g_sf_el_gx =
{ .p_api = &sf_el_gx_on_guix, .p_ctrl = &g_sf_el_gx_ctrl, .p_cfg = &g_sf_el_gx_cfg };
void g_common_init(void)
{
}
