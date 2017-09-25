/* generated thread header file - do not edit */
#ifndef BLINKY_THREAD_H_
#define BLINKY_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void blinky_thread_entry(void);
#include "r_sci_uart.h"
#include "r_uart_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
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
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* BLINKY_THREAD_H_ */
