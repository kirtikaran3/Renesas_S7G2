/* generated thread header file - do not edit */
#ifndef COMMS_THREAD0_H_
#define COMMS_THREAD0_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void comms_thread0_entry(void);
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"
#include "sf_spi.h"
#include "sf_spi_api.h"
#include "sf_el_ux_comms.h"
#include "sf_comms_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer1;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;
extern const spi_cfg_t g_spi0_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi0;
#ifdef user_spi_callback
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0 (0)
#else
#define SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0 (1)
#endif
#if SPI_ON_SCI_SPI_CALLBACK_USED_g_spi0
void user_spi_callback(spi_callback_args_t * p_args);
#endif
/* SF SPI on SF SPI Instance. */
extern const sf_spi_instance_t g_sf_spi_device0;
extern const sf_comms_instance_t g_sf_comms0;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* COMMS_THREAD0_H_ */
