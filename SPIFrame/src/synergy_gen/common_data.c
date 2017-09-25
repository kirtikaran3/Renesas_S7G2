/* generated common source file - do not edit */
#include "common_data.h"
static TX_MUTEX sf_bus_mutex_g_sf_spi_bus0;
static TX_EVENT_FLAGS_GROUP sf_bus_eventflag_g_sf_spi_bus0;
static sf_spi_ctrl_t * p_sf_curr_ctrl_g_sf_spi_bus0;

#define SF_SPI_LOWER_LEVEL_MODULE_sci

#if  defined(BSP_MCU_GROUP_S124)

#if BSP_CFG_HOCO_FREQUENCY == 0
#define BSP_CFG_HOCO_FREQUENCY_HZ 24000000
#elif BSP_CFG_HOCO_FREQUENCY == 2
#define BSP_CFG_HOCO_FREQUENCY_HZ 32000000
#elif BSP_CFG_HOCO_FREQUENCY == 4
#define BSP_CFG_HOCO_FREQUENCY_HZ 48000000
#elif BSP_CFG_HOCO_FREQUENCY == 5
#define BSP_CFG_HOCO_FREQUENCY_HZ 64000000
#endif

#define PCLKB_FREQ_HZ (BSP_CFG_HOCO_FREQUENCY_HZ / (0x01 << BSP_CFG_PCKB_DIV))

#ifdef SF_SPI_LOWER_LEVEL_MODULE_sci
#define SF_SPI_FREQ_HZ_MAX (PCLKB_FREQ_HZ / 4)
#else
#define SF_SPI_FREQ_HZ_MAX (PCLKB_FREQ_HZ / 2)
#endif

#else 

#define PCLKA_FREQ_HZ (((BSP_CFG_XTAL_HZ/(BSP_CFG_PLL_DIV + 1)) * BSP_CFG_PLL_MUL) / (0x01 << BSP_CFG_PCKA_DIV))

#ifdef SF_SPI_LOWER_LEVEL_MODULE_sci

#define SF_SPI_FREQ_HZ_MAX (PCLKA_FREQ_HZ / 8)
#else

uint32_t pclk_freqhz = (uint32_t)PCLKA_FREQ_HZ;
#define GREATER_THAN_60MHZ (pclk_freqhz > 60000000) ? 1 : 0		

#if GREATER_THAN_60MHZ
#define SF_SPI_FREQ_HZ_MAX (PCLKA_FREQ_HZ / 4)
#else
#define SF_SPI_FREQ_HZ_MAX (PCLKA_FREQ_HZ / 2)
#endif			
#endif
#endif

sf_spi_bus_t g_sf_spi_bus0 =
{ .p_bus_name = (uint8_t *) "g_sf_spi_bus0", .channel = 0, .freq_hz_max = SF_SPI_FREQ_HZ_MAX, .freq_hz_min = 0,
  .p_lock_mutex = &sf_bus_mutex_g_sf_spi_bus0, .p_sync_eventflag = &sf_bus_eventflag_g_sf_spi_bus0, .pp_curr_ctrl =
          (sf_spi_ctrl_t **) &p_sf_curr_ctrl_g_sf_spi_bus0,
  .p_lower_lvl_api = (spi_api_t *) &g_spi_on_sci, .device_count = 0, };
void g_common_init(void)
{
}
