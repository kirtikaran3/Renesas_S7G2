/* generated thread header file - do not edit */
#ifndef SF_ADC_DEMO_THREAD_H_
#define SF_ADC_DEMO_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void sf_adc_demo_thread_entry(void);
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_adc.h"
#include "r_adc_api.h"
#include "sf_adc_periodic.h"
#ifdef __cplusplus
extern "C"
{
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer_frame;
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer_frame;
#ifdef NULL
#define TIMER_ON_GPT_CALLBACK_USED_g_timer_frame (0)
#else
#define TIMER_ON_GPT_CALLBACK_USED_g_timer_frame (1)
#endif
#if TIMER_ON_GPT_CALLBACK_USED_g_timer_frame
void NULL(timer_callback_args_t * p_args);
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc_frame;
#ifdef sf_adc_periodic_cb
#define ADC_CALLBACK_USED_g_adc_frame (0)
#else
#define ADC_CALLBACK_USED_g_adc_frame (1)
#endif
#if ADC_CALLBACK_USED_g_adc_frame
void sf_adc_periodic_cb(adc_callback_args_t * p_args);
#endif
/** ADC Periodic on ADC Periodic instance */
extern sf_adc_periodic_instance_t g_sf_adc_periodic;
/** Buffer where the sampled data will be stored for application usage */
extern uint16_t g_user_buffer[32];
#ifdef g_adc_framework_user_callback
#define ADC_PERIODIC_ON_ADC_PERIODIC_CALLBACK_USED_g_sf_adc_periodic (0)
#else
#define ADC_PERIODIC_ON_ADC_PERIODIC_CALLBACK_USED_g_sf_adc_periodic (1)
#endif
#if ADC_PERIODIC_ON_ADC_PERIODIC_CALLBACK_USED_g_sf_adc_periodic
/** Declaration of user callback function. This function MUST be defined in the user application.*/
void g_adc_framework_user_callback(sf_adc_periodic_callback_args_t * p_args);
#endif
extern TX_SEMAPHORE adc_semaphore;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SF_ADC_DEMO_THREAD_H_ */
