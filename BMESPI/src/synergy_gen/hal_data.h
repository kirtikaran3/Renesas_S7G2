/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
extern const spi_cfg_t g_spi0_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi0;
#ifdef user_spi_callback
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0 (0)
#else
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0 (1)
#endif
#if SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0
void user_spi_callback(spi_callback_args_t * p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart0;
#ifdef NULL
#else
extern void NULL(uint32_t channel, uint32_t level);
#endif
#ifdef user_uart_callback
#define UART_ON_SCI_UART_CALLBACK_USED_g_uart0 (0)
#else
#define UART_ON_SCI_UART_CALLBACK_USED_g_uart0 (1)
#endif
#if UART_ON_SCI_UART_CALLBACK_USED_g_uart0
void user_uart_callback(uart_callback_args_t * p_args);
#endif
/** ELC Instance */
extern const elc_instance_t g_elc;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
void hal_entry(void);
void g_hal_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* HAL_DATA_H_ */
