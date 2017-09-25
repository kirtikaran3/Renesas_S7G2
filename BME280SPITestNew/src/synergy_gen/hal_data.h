/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer;
#ifdef NULL
#define TIMER_ON_GPT_CALLBACK_USED_g_timer (0)
#else
#define TIMER_ON_GPT_CALLBACK_USED_g_timer (1)
#endif
#if TIMER_ON_GPT_CALLBACK_USED_g_timer
void NULL(timer_callback_args_t * p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart;
#ifdef NULL
#else
extern void NULL(uint32_t channel, uint32_t level);
#endif
#ifdef UartCallback
#define UART_ON_SCI_UART_CALLBACK_USED_g_uart (0)
#else
#define UART_ON_SCI_UART_CALLBACK_USED_g_uart (1)
#endif
#if UART_ON_SCI_UART_CALLBACK_USED_g_uart
void UartCallback(uart_callback_args_t * p_args);
#endif
extern const spi_cfg_t g_spi0_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi0;
#ifdef NULL
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0 (0)
#else
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0 (1)
#endif
#if SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0
void NULL(spi_callback_args_t * p_args);
#endif
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
/** ELC Instance */
extern const elc_instance_t g_elc;
void hal_entry(void);
void g_hal_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* HAL_DATA_H_ */
