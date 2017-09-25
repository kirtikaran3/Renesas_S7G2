/* generated thread header file - do not edit */
#ifndef SF_TOUCH_PANEL_DEMO_THREAD_H_
#define SF_TOUCH_PANEL_DEMO_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void sf_touch_panel_demo_thread_entry(void);
#include "r_icu.h"
#include "r_external_irq_api.h"
#include "sf_external_irq.h"
#include "r_sci_i2c.h"
#include "r_i2c_api.h"
#include "sf_touch_panel_i2c.h"
#include "sf_touch_panel_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_touch_irq;
#ifdef NULL
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_touch_irq (0)
#else
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_touch_irq (1)
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_touch_irq
void NULL(external_irq_callback_args_t * p_args);
#endif
/** SF External IRQ on SF External IRQ Instance. */
extern const sf_external_irq_instance_t g_sf_external_touch_irq;
extern const i2c_cfg_t g_i2c_touch_panel_cfg;
/** I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c_touch_panel;
#ifdef NULL
#define I2C_ON_SCI_CALLBACK_USED_g_i2c_touch_panel (0)
#else
#define I2C_ON_SCI_CALLBACK_USED_g_i2c_touch_panel (1)
#endif
#if I2C_ON_SCI_CALLBACK_USED_g_i2c_touch_panel
void NULL(i2c_callback_args_t * p_args);
#endif
/** SF Touch Panel on SF Touch Panel I2C Instance. */
extern const sf_touch_panel_instance_t g_sf_touch_panel_i2c;
/** Touch Chip Driver Instance to be attached to SF Touch Panel */
extern const sf_touch_panel_i2c_chip_t g_sf_touch_panel_i2c_chip_sx8654;
/** Messaging Framework Instance */
extern const sf_message_instance_t g_sf_message;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SF_TOUCH_PANEL_DEMO_THREAD_H_ */
