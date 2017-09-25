/* generated thread header file - do not edit */
#ifndef INPUT_THREAD_H_
#define INPUT_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void input_thread_entry(void);
#include "r_icu.h"
#include "r_external_irq_api.h"
#include "r_adc.h"
#include "r_adc_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_sw5_irq;
#ifdef g_sw5_irq_callback
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw5_irq (0)
#else
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw5_irq (1)
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw5_irq
void g_sw5_irq_callback(external_irq_callback_args_t * p_args);
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_sw4_irq;
#ifdef g_sw4_irq_callback
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw4_irq (0)
#else
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw4_irq (1)
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_sw4_irq
void g_sw4_irq_callback(external_irq_callback_args_t * p_args);
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_temp_adc;
#ifdef NULL
#define ADC_CALLBACK_USED_g_temp_adc (0)
#else
#define ADC_CALLBACK_USED_g_temp_adc (1)
#endif
#if ADC_CALLBACK_USED_g_temp_adc
void NULL(adc_callback_args_t * p_args);
#endif
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* INPUT_THREAD_H_ */
