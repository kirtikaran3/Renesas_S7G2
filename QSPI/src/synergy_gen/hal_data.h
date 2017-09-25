/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "r_qspi.h"
#include "r_qspi_api.h"
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
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer1;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart;
#ifdef NULL
#else
extern void NULL(uint32_t channel, uint32_t level);
#endif
#ifdef user_uart_callback
#define UART_ON_SCI_UART_CALLBACK_USED_g_uart (0)
#else
#define UART_ON_SCI_UART_CALLBACK_USED_g_uart (1)
#endif
#if UART_ON_SCI_UART_CALLBACK_USED_g_uart
void user_uart_callback(uart_callback_args_t * p_args);
#endif
extern const qspi_instance_t g_qspi0;
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
