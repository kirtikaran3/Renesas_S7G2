/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_cgc_api.h"
#include "r_spi_api.h"
#include "sf_spi.h"
#include "sf_spi_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
extern sf_spi_bus_t g_sf_spi_bus0;
extern const spi_api_t g_spi_on_sci;
void g_common_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* COMMON_DATA_H_ */
