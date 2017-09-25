/* generated thread header file - do not edit */
#ifndef SF_I2C_PCAL2_THREAD_H_
#define SF_I2C_PCAL2_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void sf_i2c_pcal2_thread_entry(void);
#include "r_sci_i2c.h"
#include "r_i2c_api.h"
#include "sf_i2c.h"
#include "sf_i2c_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
extern const i2c_cfg_t g_i2c_breakoutboard_cfg;
/** I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c_breakoutboard;
#ifdef NULL
#define I2C_ON_SCI_CALLBACK_USED_g_i2c_breakoutboard (0)
#else
#define I2C_ON_SCI_CALLBACK_USED_g_i2c_breakoutboard (1)
#endif
#if I2C_ON_SCI_CALLBACK_USED_g_i2c_breakoutboard
void NULL(i2c_callback_args_t * p_args);
#endif
/* SF I2C on SF I2C Instance. */
extern const sf_i2c_instance_t g_sf_i2c_device_breakoutboard;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SF_I2C_PCAL2_THREAD_H_ */
