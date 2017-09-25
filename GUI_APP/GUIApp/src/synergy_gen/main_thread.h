/* generated thread header file - do not edit */
#ifndef MAIN_THREAD_H_
#define MAIN_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void main_thread_entry(void);
#include "r_sci_spi.h"
#include "r_spi_api.h"
#include "r_icu.h"
#include "r_external_irq_api.h"
#include "sf_external_irq.h"
#include "r_riic.h"
#include "r_i2c_api.h"
#include "sf_touch_panel_i2c.h"
#include "sf_touch_panel_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
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
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_touch_irq;
#ifdef NULL
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_touch_irq (0)
#else
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_touch_irq (1)
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_touch_irq
void NULL(external_irq_callback_args_t * p_args);
#endif
/** SF External IRQ on SF External IRQ Instance. */
extern const sf_external_irq_instance_t g_sf_external_irq;
extern const i2c_cfg_t g_i2c_cfg;
/** I2C on RIIC Instance. */
extern const i2c_master_instance_t g_i2c;
#ifdef NULL
#define I2C_CALLBACK_USED_g_i2c (0)
#else
#define I2C_CALLBACK_USED_g_i2c (1)
#endif
#if I2C_CALLBACK_USED_g_i2c
void NULL(i2c_callback_args_t * p_args);
#endif
/** SF Touch Panel on SF Touch Panel I2C Instance. */
extern const sf_touch_panel_instance_t g_sf_touch_panel_i2c;
/** Touch Chip Driver Instance to be attached to SF Touch Panel */
extern const sf_touch_panel_i2c_chip_t g_sf_touch_panel_i2c_chip_sx8654;
/** Messaging Framework Instance */
extern const sf_message_instance_t g_sf_message0;
extern TX_SEMAPHORE g_main_semaphore_lcdc;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* MAIN_THREAD_H_ */
