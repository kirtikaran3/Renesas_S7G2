/* generated thread header file - do not edit */
#ifndef GUI_THREAD_H_
#define GUI_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void gui_thread_entry(void);
#include "r_sci_spi.h"
#include "r_spi_api.h"
#include "r_glcd.h"
#include "r_display_api.h"
#include "sf_el_gx.h"
#include "r_jpeg_decode.h"
#include "sf_jpeg_decode.h"
#ifdef __cplusplus
extern "C"
{
#endif
extern const spi_cfg_t g_lcd_spi_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_lcd_spi;
#ifdef g_lcd_spi_callback
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_lcd_spi (0)
#else
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_lcd_spi (1)
#endif
#if SPI_ON_SCI_SPI_CALLBACK_USED_g_lcd_spi
void g_lcd_spi_callback(spi_callback_args_t * p_args);
#endif
/* Display on GLCD Instance. */
extern const display_instance_t g_display;
extern display_runtime_cfg_t g_display_runtime_cfg_fg;
extern display_runtime_cfg_t g_display_runtime_cfg_bg;
#if (false)
extern display_clut_cfg_t g_display_clut_cfg_glcd;
#endif
#ifdef NULL
#define DISPLAY_ON_GLCD_CALLBACK_USED (0)
#else
#define DISPLAY_ON_GLCD_CALLBACK_USED (1)
#endif
#if DISPLAY_ON_GLCD_CALLBACK_USED
void NULL(display_callback_args_t * p_args);
#endif
#if (true)
#ifdef __GNUC__
extern uint8_t g_display_fb_background[2][163840];
#else /* __ICCARM__ */
extern uint8_t g_display_fb_background[2][163840];
#endif
#endif
#if (false)
#ifdef __GNUC__
extern uint8_t g_display_fb_foreground[2][768000];
#else /* __ICCARM__ */
extern uint8_t g_display_fb_foreground[2][768000];
#endif
#endif
#ifndef NULL
extern void NULL(sf_el_gx_callback_args_t * p_arg);
#endif
extern sf_el_gx_instance_t g_sf_el_gx;
extern const jpeg_decode_instance_t g_jpeg_decode;
#ifdef NULL
#define JPEG_DECODE_CALLBACK_USED_g_jpeg_decode (0)
#else
#define JPEG_DECODE_CALLBACK_USED_g_jpeg_decode (1)
#endif
#if JPEG_DECODE_CALLBACK_USED_g_jpeg_decode
void NULL(jpeg_decode_callback_args_t * p_args);
#endif
extern const sf_jpeg_decode_instance_t g_sf_jpeg_decode;
extern TX_SEMAPHORE g_lcd_semaphore;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GUI_THREAD_H_ */
