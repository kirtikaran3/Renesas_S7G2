/* generated thread header file - do not edit */
#ifndef MAIN_THREAD_H_
#define MAIN_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void main_thread_entry(void);
#include "r_adc.h"
#include "r_adc_api.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc;
#ifdef g_adc_callback
#define ADC_CALLBACK_USED_g_adc (0)
#else
#define ADC_CALLBACK_USED_g_adc (1)
#endif
#if ADC_CALLBACK_USED_g_adc
void g_adc_callback(adc_callback_args_t * p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer1;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;
extern const spi_cfg_t g_spi_lcdc_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi_lcdc;
#ifdef g_lcd_spi_callback
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_spi_lcdc (0)
#else
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_spi_lcdc (1)
#endif
#if SPI_ON_SCI_SPI_CALLBACK_USED_g_spi_lcdc
void g_lcd_spi_callback(spi_callback_args_t * p_args);
#endif
extern TX_SEMAPHORE g_main_semaphore_lcdc;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* MAIN_THREAD_H_ */
