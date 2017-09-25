/* generated thread header file - do not edit */
#ifndef DEV_EXAMPLE_THREAD_H_
#define DEV_EXAMPLE_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void dev_example_thread_entry(void);
#include "sf_el_nx_comms.h"
#include "sf_comms_api.h"
#include "r_fmi.h"
#include "r_fmi_api.h"
#include "r_icu.h"
#include "r_external_irq_api.h"
#include "sf_external_irq.h"
#include "r_lpm.h"
#include "r_lpm_api.h"
#include "r_wdt.h"
#include "r_wdt_api.h"
#include "sf_thread_monitor.h"
#include "r_agt.h"
#include "r_timer_api.h"
#include "r_rtc.h"
#include "r_rtc_api.h"
#include "r_crc.h"
#include "r_crc_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_qspi.h"
#include "r_qspi_api.h"
#include "r_adc.h"
#include "r_adc_api.h"
#include "r_sdmmc.h"
#include "r_sdmmc_api.h"
#include "sf_block_media_sdmmc.h"
#include "sf_block_media_api.h"
#include "sf_el_fx.h"
#include "fx_api.h"
#include "r_sci_i2c.h"
#include "r_i2c_api.h"
#include "r_flash_hp.h"
#include "r_flash_api.h"
#include "sf_el_ux_comms.h"
#include "sf_comms_api.h"
#include "sf_console.h"
#include "sf_console_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
extern const sf_comms_instance_t g_sf_comms_telnet;
/** FMI on FMI Instance. */
extern const fmi_instance_t g_fmi;
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq1;
#ifdef NULL
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq1 (0)
#else
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq1 (1)
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq1
void NULL(external_irq_callback_args_t * p_args);
#endif
/** SF External IRQ on SF External IRQ Instance. */
extern const sf_external_irq_instance_t g_sf_external_irq;
/** lpm Instance */
extern const lpm_instance_t g_lpm;
/** WDT on WDT Instance. */
extern const wdt_instance_t g_wdt;
#ifdef NULL
#define WATCHDOG_ON_WDT_CALLBACK_USED (0)
#else
#define WATCHDOG_ON_WDT_CALLBACK_USED (1)
#endif
#if WATCHDOG_ON_WDT_CALLBACK_USED
void NULL(wdt_callback_args_t * p_args);
#endif
extern const sf_thread_monitor_instance_t g_sf_thread_monitor;
/** AGT Timer Instance */
extern const timer_instance_t g_timer_agt;
#ifdef AGT_timer_INTERRUPT
#define TIMER_ON_AGT_CALLBACK_USED_g_timer_agt (0)
#else
#define TIMER_ON_AGT_CALLBACK_USED_g_timer_agt (1)
#endif
#if TIMER_ON_AGT_CALLBACK_USED_g_timer_agt
void AGT_timer_INTERRUPT(timer_callback_args_t * p_args);
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq;
#ifdef switch1_press_IRQ
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq (0)
#else
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq (1)
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq
void switch1_press_IRQ(external_irq_callback_args_t * p_args);
#endif
/** RTC on RTC Instance. */
extern const rtc_instance_t g_rtc;
#ifdef rtc_callback
#define RTC_CALLBACK_USED_g_rtc (0)
#else
#define RTC_CALLBACK_USED_g_rtc (1)
#endif
#if RTC_CALLBACK_USED_g_rtc
void rtc_callback(rtc_callback_args_t * p_args);
#endif
extern const crc_instance_t g_crc;
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer_dac;
#ifdef timer_callback_dac
#define TIMER_ON_GPT_CALLBACK_USED_g_timer_dac (0)
#else
#define TIMER_ON_GPT_CALLBACK_USED_g_timer_dac (1)
#endif
#if TIMER_ON_GPT_CALLBACK_USED_g_timer_dac
void timer_callback_dac(timer_callback_args_t * p_args);
#endif
extern const qspi_instance_t g_qspi;
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc;
#ifdef Adc_callback
#define ADC_CALLBACK_USED_g_adc (0)
#else
#define ADC_CALLBACK_USED_g_adc (1)
#endif
#if ADC_CALLBACK_USED_g_adc
void Adc_callback(adc_callback_args_t * p_args);
#endif
/** SDMMC on SDMMC Instance. */
extern const sdmmc_instance_t g_sdmmc;
#ifdef NULL
#define SDMMC_ON_SDMMC_CALLBACK_USED_g_sdmmc (0)
#else
#define SDMMC_ON_SDMMC_CALLBACK_USED_g_sdmmc (1)
#endif
#if SDMMC_ON_SDMMC_CALLBACK_USED_g_sdmmc
void NULL(sdmmc_callback_args_t * p_args);
#endif
/** Block Media on SDMMC Instance */
extern sf_block_media_instance_t g_sf_block_media_sdmmc;
extern sf_el_fx_t g_sf_el_fx_cfg;
extern FX_MEDIA g_fx_media;

/** Block Media Format Type */
typedef struct st_sf_block_media_format
{
    FX_MEDIA * p_fx_media;
    sf_el_fx_t * p_driver_info;
    uint8_t * p_memory;
    uint32_t memory_size;
    int8_t * volume_name;
    uint32_t number_of_fats;
    uint32_t directory_entries;
    uint32_t hidden_sectors;
    uint32_t total_sectors;
    uint32_t bytes_per_sector;
    uint32_t sectors_per_cluster;
} sf_block_media_format_t;
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer_hal;
#ifdef Timer0_Interrupt
#define TIMER_ON_GPT_CALLBACK_USED_g_timer_hal (0)
#else
#define TIMER_ON_GPT_CALLBACK_USED_g_timer_hal (1)
#endif
#if TIMER_ON_GPT_CALLBACK_USED_g_timer_hal
void Timer0_Interrupt(timer_callback_args_t * p_args);
#endif
extern const i2c_cfg_t g_i2c_sci_cfg;
/** I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c_sci;
#ifdef NULL
#define I2C_ON_SCI_CALLBACK_USED_g_i2c_sci (0)
#else
#define I2C_ON_SCI_CALLBACK_USED_g_i2c_sci (1)
#endif
#if I2C_ON_SCI_CALLBACK_USED_g_i2c_sci
void NULL(i2c_callback_args_t * p_args);
#endif
/* Flash on Flash HP Instance */
extern const flash_instance_t g_flash_hp;
#ifdef NULL
#define FLASH_CALLBACK_USED (0)
#else
#define FLASH_CALLBACK_USED (1)
#endif
#if FLASH_CALLBACK_USED
void NULL(flash_callback_args_t * p_args);
#endif
extern const sf_comms_instance_t g_sf_comms;
extern const sf_console_instance_t g_sf_console;
extern const sf_console_menu_t g_sf_console_root_menu;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* DEV_EXAMPLE_THREAD_H_ */
