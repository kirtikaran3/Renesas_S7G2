/* generated thread header file - do not edit */
#ifndef BLINKY_THREAD_H_
#define BLINKY_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void blinky_thread_entry(void);
#include "r_icu.h"
#include "r_external_irq_api.h"
#include "sf_external_irq.h"
#ifdef __cplusplus
extern "C"
{
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq_11;
#ifdef g_external_irq11_callback
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq_11 (0)
#else
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq_11 (1)
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq_11
void g_external_irq11_callback(external_irq_callback_args_t * p_args);
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq;
#ifdef g_external_irq10_callback
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq (0)
#else
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq (1)
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq
void g_external_irq10_callback(external_irq_callback_args_t * p_args);
#endif
/** SF External IRQ on SF External IRQ Instance. */
extern const sf_external_irq_instance_t g_sf_external_irq;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* BLINKY_THREAD_H_ */
