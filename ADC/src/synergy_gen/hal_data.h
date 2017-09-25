/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_adc.h"
#include "r_adc_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc0;
#ifdef NULL
#define ADC_CALLBACK_USED_g_adc0 (0)
#else
#define ADC_CALLBACK_USED_g_adc0 (1)
#endif
#if ADC_CALLBACK_USED_g_adc0
void NULL(adc_callback_args_t * p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer0;
#ifdef NULL
#define TIMER_ON_GPT_CALLBACK_USED_g_timer0 (0)
#else
#define TIMER_ON_GPT_CALLBACK_USED_g_timer0 (1)
#endif
#if TIMER_ON_GPT_CALLBACK_USED_g_timer0
void NULL(timer_callback_args_t * p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer1;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;
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
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
/** ELC Instance */
extern const elc_instance_t g_elc;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
void hal_entry(void);
void g_hal_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* HAL_DATA_H_ */
