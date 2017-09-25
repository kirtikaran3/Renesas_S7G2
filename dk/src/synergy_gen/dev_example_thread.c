/* generated thread source file - do not edit */
#include "dev_example_thread.h"

TX_THREAD dev_example_thread;
void dev_example_thread_create(void);
static void dev_example_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t dev_example_thread_stack[10240] BSP_PLACE_IN_SECTION(".stack.dev_example_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
sf_comms_ctrl_t g_sf_comms_telnet_ctrl;
sf_el_nx_comms_on_comms_ctrl_t g_sf_comms_telnet_ctrl_extend;
extern VOID nx_ether_driver_eth1(NX_IP_DRIVER*);
sf_el_nx_comms_on_comms_cfg_t g_sf_comms_telnet_cfg_extend =
{ .p_ctrl = &g_sf_comms_telnet_ctrl_extend, .ip_address = IP_ADDRESS (10, 75, 70, 212), .subnet_mask = IP_ADDRESS (255,
                                                                                                                   255,
                                                                                                                   255,
                                                                                                                   0),
  .driver = nx_ether_driver_eth1, };
const sf_comms_cfg_t g_sf_comms_telnet_cfg =
{ .p_extend = &g_sf_comms_telnet_cfg_extend, };
/* Instance structure to use this module. */
const sf_comms_instance_t g_sf_comms_telnet =
{ .p_ctrl = &g_sf_comms_telnet_ctrl, .p_cfg = &g_sf_comms_telnet_cfg, .p_api = &g_sf_el_nx_comms_on_sf_comms };
/* Instance structure to use this module. */
const fmi_instance_t g_fmi =
{ .p_api = &g_fmi_on_fmi };
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq1
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(external_irq_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
static external_irq_ctrl_t g_external_irq1_ctrl;
static const external_irq_cfg_t g_external_irq1_cfg =
{ .channel = 14, .trigger = EXTERNAL_IRQ_TRIG_FALLING, .filter_enable = false, .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true, .p_callback = NULL, .p_context = &g_external_irq1, .p_extend = NULL };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq1 =
{ .p_ctrl = &g_external_irq1_ctrl, .p_cfg = &g_external_irq1_cfg, .p_api = &g_external_irq_on_icu };

#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq1
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(external_irq_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(external_irq_callback_args_t * p_args);
void NULL_internal(external_irq_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
sf_external_irq_ctrl_t g_sf_external_irq_ctrl;
const sf_external_irq_cfg_t g_sf_external_irq_cfg =
{ .event = SF_EXTERNAL_IRQ_EVENT_SEMAPHORE_PUT, .p_lower_lvl_irq = &g_external_irq1, };
/* Instance structure to use this module. */
const sf_external_irq_instance_t g_sf_external_irq =
{ .p_ctrl = &g_sf_external_irq_ctrl, .p_cfg = &g_sf_external_irq_cfg, .p_api = &g_sf_external_irq_on_sf_external_irq };
const lpm_cfg_t g_lpm_cfg =
{ .operating_power = LPM_OPERATING_POWER_HIGH_SPEED_MODE, .sub_oscillator = LPM_SUBOSC_OTHER, .code_flash =
          LPM_CODE_FLASH_OPERATES };
const lpm_instance_t g_lpm =
{ .p_api = &g_lpm_on_lpm, .p_cfg = &g_lpm_cfg };
#if WATCHDOG_ON_WDT_CALLBACK_USED
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(wdt_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif

static wdt_ctrl_t g_wdt_ctrl;

static const wdt_cfg_t g_wdt_cfg =
{ .start_mode = WDT_START_MODE_REGISTER, .autostart = false, .timeout = WDT_TIMEOUT_16384, .clock_division =
          WDT_CLOCK_DIVISION_8192,
  .window_start = WDT_WINDOW_START_100, .window_end = WDT_WINDOW_END_0, .reset_control = WDT_RESET_CONTROL_RESET,
  .stop_control = WDT_STOP_CONTROL_DISABLE, .p_callback = NULL, };

/* Instance structure to use this module. */
const wdt_instance_t g_wdt =
{ .p_ctrl = &g_wdt_ctrl, .p_cfg = &g_wdt_cfg, .p_api = &g_wdt_on_wdt };

#if WATCHDOG_ON_WDT_CALLBACK_USED
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function
 *             with the prototype below.
 *               - void NULL(wdt_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(wdt_callback_args_t * p_args);
void NULL_internal(wdt_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
static sf_thread_monitor_ctrl_t g_sf_thread_monitor_ctrl;
static sf_thread_monitor_cfg_t g_sf_thread_monitor_cfg =
{ .p_lower_lvl_wdt = &g_wdt, .priority = 1, .profiling_mode_enabled = false };
/* Instance structure to use this module. */
const sf_thread_monitor_instance_t g_sf_thread_monitor =
{ .p_ctrl = &g_sf_thread_monitor_ctrl, .p_cfg = &g_sf_thread_monitor_cfg, .p_api =
          &g_sf_thread_monitor_on_sf_thread_monitor };
#if TIMER_ON_AGT_CALLBACK_USED_g_timer_agt
#if defined(__ICCARM__)
#define AGT_timer_INTERRUPT_WEAK_ATTRIBUTE
#pragma weak AGT_timer_INTERRUPT                            = AGT_timer_INTERRUPT_internal
#elif defined(__GNUC__)
#define AGT_timer_INTERRUPT_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("AGT_timer_INTERRUPT_internal")))
#endif
void AGT_timer_INTERRUPT(timer_callback_args_t * p_args) AGT_timer_INTERRUPT_WEAK_ATTRIBUTE;
#endif

static timer_ctrl_t g_timer_agt_ctrl;
static const timer_on_agt_cfg_t g_timer_agt_extend =
        { .count_source = AGT_CLOCK_LOCO, .agto_output_enabled = false, .agtio_output_enabled = false,
          .output_inverted = false, };
static const timer_cfg_t g_timer_agt_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period = 1, .unit = TIMER_UNIT_PERIOD_SEC, .channel = 0, .autostart = false,
  .p_callback = AGT_timer_INTERRUPT, .p_context = &g_timer_agt, .p_extend = &g_timer_agt_extend };
/* Instance structure to use this module. */
const timer_instance_t g_timer_agt =
{ .p_ctrl = &g_timer_agt_ctrl, .p_cfg = &g_timer_agt_cfg, .p_api = &g_timer_on_agt };

#if TIMER_ON_AGT_CALLBACK_USED_g_timer_agt
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void AGT_timer_INTERRUPT(timer_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void AGT_timer_INTERRUPT_internal(timer_callback_args_t * p_args);
void AGT_timer_INTERRUPT_internal(timer_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq
#if defined(__ICCARM__)
#define switch1_press_IRQ_WEAK_ATTRIBUTE
#pragma weak switch1_press_IRQ                            = switch1_press_IRQ_internal
#elif defined(__GNUC__)
#define switch1_press_IRQ_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("switch1_press_IRQ_internal")))
#endif
void switch1_press_IRQ(external_irq_callback_args_t * p_args) switch1_press_IRQ_WEAK_ATTRIBUTE;
#endif
static external_irq_ctrl_t g_external_irq_ctrl;
static const external_irq_cfg_t g_external_irq_cfg =
{ .channel = 11, .trigger = EXTERNAL_IRQ_TRIG_FALLING, .filter_enable = false, .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true, .p_callback = switch1_press_IRQ, .p_context = &g_external_irq, .p_extend = NULL };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq =
{ .p_ctrl = &g_external_irq_ctrl, .p_cfg = &g_external_irq_cfg, .p_api = &g_external_irq_on_icu };

#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_external_irq
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void switch1_press_IRQ(external_irq_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void switch1_press_IRQ_internal(external_irq_callback_args_t * p_args);
void switch1_press_IRQ_internal(external_irq_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if RTC_CALLBACK_USED_g_rtc
#if defined(__ICCARM__)
#define rtc_callback_WEAK_ATTRIBUTE
#pragma weak rtc_callback                            = rtc_callback_internal
#elif defined(__GNUC__)
#define rtc_callback_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("rtc_callback_internal")))
#endif
void rtc_callback(rtc_callback_args_t * p_args) rtc_callback_WEAK_ATTRIBUTE;
#endif
rtc_ctrl_t g_rtc_ctrl;
const rtc_cfg_t g_rtc_cfg =
{ .clock_source = RTC_CLOCK_SOURCE_LOCO, .error_adjustment_value = 0,
  .error_adjustment_type = RTC_ERROR_ADJUSTMENT_NONE, .p_callback = rtc_callback, .p_context = &g_rtc, };
/* Instance structure to use this module. */
const rtc_instance_t g_rtc =
{ .p_ctrl = &g_rtc_ctrl, .p_cfg = &g_rtc_cfg, .p_api = &g_rtc_on_rtc };

#if RTC_CALLBACK_USED_g_rtc
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void rtc_callback(rtc_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void rtc_callback_internal(rtc_callback_args_t * p_args);
void rtc_callback_internal(rtc_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
static crc_ctrl_t g_crc_ctrl;
const crc_cfg_t g_crc_cfg =
{ .polynomial = CRC_POLYNOMIAL_CRC_8, .bit_order = CRC_BIT_ORDER_LMS_MSB, };
/* Instance structure to use this module. */
const crc_instance_t g_crc =
{ .p_ctrl = &g_crc_ctrl, .p_cfg = &g_crc_cfg, .p_api = &g_crc_on_crc };
#if TIMER_ON_GPT_CALLBACK_USED_g_timer_dac
#if defined(__ICCARM__)
#define timer_callback_dac_WEAK_ATTRIBUTE
#pragma weak timer_callback_dac                            = timer_callback_dac_internal
#elif defined(__GNUC__)
#define timer_callback_dac_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("timer_callback_dac_internal")))
#endif
void timer_callback_dac(timer_callback_args_t * p_args) timer_callback_dac_WEAK_ATTRIBUTE;
#endif
static timer_ctrl_t g_timer_dac_ctrl;
static const timer_on_gpt_cfg_t g_timer_dac_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW } };
static const timer_cfg_t g_timer_dac_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period = 8000, .unit = TIMER_UNIT_FREQUENCY_HZ, .duty_cycle = 50, .duty_cycle_unit =
          TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 2, .autostart = false, .p_callback = timer_callback_dac, .p_context = &g_timer_dac, .p_extend =
          &g_timer_dac_extend };
/* Instance structure to use this module. */
const timer_instance_t g_timer_dac =
{ .p_ctrl = &g_timer_dac_ctrl, .p_cfg = &g_timer_dac_cfg, .p_api = &g_timer_on_gpt };

#if TIMER_ON_GPT_CALLBACK_USED_g_timer_dac
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void timer_callback_dac(timer_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void timer_callback_dac_internal(timer_callback_args_t * p_args);
void timer_callback_dac_internal(timer_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
static qspi_ctrl_t g_qspi_ctrl;
const qspi_cfg_t g_qspi_cfg =
{ .p_extend = NULL, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
const qspi_instance_t g_qspi =
{ .p_ctrl = &g_qspi_ctrl, .p_cfg = &g_qspi_cfg, .p_api = &g_qspi_on_qspi, };
#if ADC_CALLBACK_USED_g_adc
#if defined(__ICCARM__)
#define Adc_callback_WEAK_ATTRIBUTE
#pragma weak Adc_callback                            = Adc_callback_internal
#elif defined(__GNUC__)
#define Adc_callback_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("Adc_callback_internal")))
#endif
void Adc_callback(adc_callback_args_t * p_args) Adc_callback_WEAK_ATTRIBUTE;
#endif
adc_ctrl_t g_adc_ctrl;
const adc_cfg_t g_adc_cfg =
{ .unit = 0, .mode = ADC_MODE_SINGLE_SCAN, .resolution = ADC_RESOLUTION_12_BIT, .alignment = ADC_ALIGNMENT_RIGHT,
  .add_average_count = ADC_ADD_OFF, .clearing = ADC_CLEAR_AFTER_READ_ON, .trigger = ADC_TRIGGER_SOFTWARE,
  .trigger_group_b = ADC_TRIGGER_SYNC_ELC, .p_callback = Adc_callback, .p_context = &g_adc, };
const adc_channel_cfg_t g_adc_channel_cfg =
{ .scan_mask = (uint32_t) (
        ((uint64_t) ADC_MASK_CHANNEL_0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                | (0)),
  /** Group B channel mask is right shifted by 32 at the end to form the proper mask */
  .scan_mask_group_b = (uint32_t) (
          (((uint64_t) ADC_MASK_CHANNEL_0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                  | (0)) >> 32),
  .priority_group_a = ADC_GROUP_A_PRIORITY_OFF, .add_mask = (uint32_t) (
          (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0)
                  | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0)),
  .sample_hold_mask = (uint32_t) ((0) | (0) | (0)), .sample_hold_states = 24, };
/* Instance structure to use this module. */
const adc_instance_t g_adc =
{ .p_ctrl = &g_adc_ctrl, .p_cfg = &g_adc_cfg, .p_channel_cfg = &g_adc_channel_cfg, .p_api = &g_adc_on_adc };
#if ADC_CALLBACK_USED_g_adc
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void Adc_callback(adc_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void Adc_callback_internal(adc_callback_args_t * p_args);
void Adc_callback_internal(adc_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
sdmmc_ctrl_t g_sdmmc_ctrl;
sdmmc_cfg_t g_sdmmc_cfg =
{ .hw.media_type = SDMMC_MEDIA_TYPE_EMBEDDED, .hw.bus_width = SDMMC_BUS_WIDTH_4_BITS, .hw.channel = 0, .p_callback =
          NULL,
  .p_extend = NULL,
#define SYNERGY_NOT_DEFINED (1)                        
#if (SYNERGY_NOT_DEFINED == SYNERGY_NOT_DEFINED)
  .p_lower_lvl_transfer = NULL,
#else
        .p_lower_lvl_transfer = &SYNERGY_NOT_DEFINED,
#endif            
    };
/* Instance structure to use this module. */
const sdmmc_instance_t g_sdmmc =
{ .p_ctrl = &g_sdmmc_ctrl, .p_cfg = &g_sdmmc_cfg, .p_api = &g_sdmmc_on_sdmmc };
static const sf_block_media_on_sdmmc_cfg_t g_sf_block_media_sdmmc_block_media_cfg =
{ .p_lower_lvl_sdmmc = &g_sdmmc, };
static sf_block_media_on_sdmmc_ctrl_t g_sf_block_media_sdmmc_on_sdmmc_ctrl;
static sf_block_media_ctrl_t g_sf_block_media_sdmmc_ctrl =
{ .p_extend = &g_sf_block_media_sdmmc_on_sdmmc_ctrl };
static sf_block_media_cfg_t g_sf_block_media_sdmmc_cfg =
{ .block_size = 512, .p_extend = &g_sf_block_media_sdmmc_block_media_cfg };

sf_block_media_instance_t g_sf_block_media_sdmmc =
{ .p_ctrl = &g_sf_block_media_sdmmc_ctrl, .p_cfg = &g_sf_block_media_sdmmc_cfg, .p_api = &g_sf_block_media_on_sdmmc };
sf_el_fx_t g_sf_el_fx_cfg =
{ .p_lower_lvl_block_media = &g_sf_block_media_sdmmc };
#define FX_FORMAT_MEDIA_ENABLE (0)
#define FILESYSTEM_ON_SDMMC (1)
ssp_err_t SF_EL_FX_MediaFormat(sf_block_media_format_t * p_sf_block_media_format);
FX_MEDIA g_fx_media;
uint8_t g_media_memory_g_fx_media[512];
#if TIMER_ON_GPT_CALLBACK_USED_g_timer_hal
#if defined(__ICCARM__)
#define Timer0_Interrupt_WEAK_ATTRIBUTE
#pragma weak Timer0_Interrupt                            = Timer0_Interrupt_internal
#elif defined(__GNUC__)
#define Timer0_Interrupt_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("Timer0_Interrupt_internal")))
#endif
void Timer0_Interrupt(timer_callback_args_t * p_args) Timer0_Interrupt_WEAK_ATTRIBUTE;
#endif
static timer_ctrl_t g_timer_hal_ctrl;
static const timer_on_gpt_cfg_t g_timer_hal_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW } };
static const timer_cfg_t g_timer_hal_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period = 1, .unit = TIMER_UNIT_PERIOD_SEC, .duty_cycle = 50, .duty_cycle_unit =
          TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 0, .autostart = false, .p_callback = Timer0_Interrupt, .p_context = &g_timer_hal, .p_extend =
          &g_timer_hal_extend };
/* Instance structure to use this module. */
const timer_instance_t g_timer_hal =
{ .p_ctrl = &g_timer_hal_ctrl, .p_cfg = &g_timer_hal_cfg, .p_api = &g_timer_on_gpt };

#if TIMER_ON_GPT_CALLBACK_USED_g_timer_hal
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void Timer0_Interrupt(timer_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void Timer0_Interrupt_internal(timer_callback_args_t * p_args);
void Timer0_Interrupt_internal(timer_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if I2C_ON_SCI_CALLBACK_USED_g_i2c_sci
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(i2c_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
i2c_ctrl_t g_i2c_sci_ctrl;
const i2c_cfg_t g_i2c_sci_cfg =
{ .channel = 7, .rate = I2C_RATE_STANDARD, .slave = 0x27, .addr_mode = I2C_ADDR_MODE_7BIT, .sda_delay = 300,
  .p_callback = NULL, .p_context = (void *) &g_i2c_sci, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_sci =
{ .p_ctrl = &g_i2c_sci_ctrl, .p_cfg = &g_i2c_sci_cfg, .p_api = &g_i2c_master_on_sci };

#if I2C_ON_SCI_CALLBACK_USED_g_i2c_sci
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(i2c_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(i2c_callback_args_t * p_args);
void NULL_internal(i2c_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if FLASH_CALLBACK_USED
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(flash_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
flash_ctrl_t g_flash_hp_ctrl;
const flash_cfg_t g_flash_hp_cfg =
{ .data_flash_bgo = false, .p_callback = NULL, .p_context = &g_flash_hp };
/* Instance structure to use this module. */
const flash_instance_t g_flash_hp =
{ .p_ctrl = &g_flash_hp_ctrl, .p_cfg = &g_flash_hp_cfg, .p_api = &g_flash_on_flash_hp };

#if FLASH_CALLBACK_USED
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(flash_callback_args_t * p_args)
 *
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(flash_callback_args_t * p_args)
{
    /** Do nothing. */
}
#endif
sf_comms_ctrl_t g_sf_comms_ctrl;
sf_el_ux_comms_on_comms_ctrl_t g_sf_comms_ctrl_extend;
sf_el_ux_comms_on_comms_cfg_t g_sf_comms_cfg_extend =
{ .p_ctrl = &g_sf_comms_ctrl_extend, };
const sf_comms_cfg_t g_sf_comms_cfg =
{ .p_extend = &g_sf_comms_cfg_extend };
/* Instance structure to use this module. */
const sf_comms_instance_t g_sf_comms =
{ .p_ctrl = &g_sf_comms_ctrl, .p_cfg = &g_sf_comms_cfg, .p_api = &g_sf_el_ux_comms_on_sf_comms };
sf_console_ctrl_t g_sf_console_ctrl;
const sf_console_cfg_t g_sf_console_cfg =
{ .p_comms = &g_sf_comms, .p_initial_menu = &g_sf_console_root_menu, .echo = true, .autostart = false, };
/* Instance structure to use this module. */
const sf_console_instance_t g_sf_console =
{ .p_ctrl = &g_sf_console_ctrl, .p_cfg = &g_sf_console_cfg, .p_api = &g_sf_console_on_sf_console };

#if FX_FORMAT_MEDIA_ENABLE == 1
ssp_err_t SF_EL_FX_MediaFormat (sf_block_media_format_t * p_sf_block_media_format)
{
    ssp_err_t ret_val = SSP_SUCCESS;
    uint32_t fx_ret_val;
    sf_block_media_instance_t * p_block_media;
    uint32_t sector_size;
    uint32_t sector_count;

    p_block_media = (sf_block_media_instance_t *) p_sf_block_media_format->p_driver_info->p_lower_lvl_block_media;

#if FILESYSTEM_ON_SDMMC == 1
    ret_val = p_block_media->p_api->open(p_block_media->p_ctrl, p_block_media->p_cfg);

    /* Get actual sector size from media. */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = p_block_media->p_api->ioctl(p_block_media->p_ctrl, SSP_COMMAND_GET_SECTOR_SIZE, &sector_size);
    }

    /* Get actual sector count from media. */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val =
        p_block_media->p_api->ioctl(p_block_media->p_ctrl, SSP_COMMAND_GET_SECTOR_COUNT, &sector_count);
    }

    /* Close driver.  */
    if (SSP_SUCCESS == ret_val)
    {
        ret_val = p_block_media->p_api->close(p_block_media->p_ctrl);
    }
#else
    sector_count = p_sf_block_media_format->total_sectors;
    sector_size = p_sf_block_media_format->bytes_per_sector;
#endif

    /* If media operations were successful and memory size is as large as sector size, format media. */
    if ((SSP_SUCCESS == ret_val) && (!(p_sf_block_media_format->memory_size < sector_size)))
    {
        /* Format media.  */
        fx_ret_val = fx_media_format(p_sf_block_media_format->p_fx_media,       // Pointer to FileX media control block.
                SF_EL_FX_BlockDriver,// Driver entry
                p_sf_block_media_format->p_driver_info,// Pointer to Block Media Driver
                p_sf_block_media_format->p_memory,// Media buffer pointer
                p_sf_block_media_format->memory_size,// Media buffer size
                p_sf_block_media_format->volume_name,// Volume Name
                p_sf_block_media_format->number_of_fats,// Number of FATs
                p_sf_block_media_format->directory_entries,// Directory Entries
                p_sf_block_media_format->hidden_sectors,// Hidden sectors
                sector_count,// Total sectors
                sector_size,// Sector size
                p_sf_block_media_format->sectors_per_cluster,// Sectors per cluster
                1,// Heads 
                1);// Sectors per track 

        if (FX_SUCCESS != fx_ret_val)
        {
            ret_val = SSP_ERR_MEDIA_FORMAT_FAILED;
        }
    }
    else
    {
        ret_val = SSP_ERR_MEDIA_FORMAT_FAILED;
    }
    return ret_val;

}
#endif
void dev_example_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&dev_example_thread, (CHAR *) "Developer Example Thread", dev_example_thread_func, (ULONG) NULL,
                      &dev_example_thread_stack, 10240, 10, 10, 1, TX_AUTO_START);
}

static void dev_example_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */
    ssp_err_t ssp_err_g_sf_comms_telnet;
    ssp_err_g_sf_comms_telnet = g_sf_comms_telnet.p_api->open (g_sf_comms_telnet.p_ctrl, g_sf_comms_telnet.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_comms_telnet)
    {
        while (1)
            ;
    }
    ssp_err_t ssp_err_g_sf_thread_monitor;
    ssp_err_g_sf_thread_monitor = g_sf_thread_monitor.p_api->open (g_sf_thread_monitor.p_ctrl,
                                                                   g_sf_thread_monitor.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_thread_monitor)
    {
        while (1)
            ;
    }
    ssp_err_t error;
    uint32_t fx_ret_val;

    fx_system_initialize ();

#if FX_FORMAT_MEDIA_ENABLE == 1
    sf_block_media_format_t g_fx_media_format =
    {
        .p_fx_media = &g_fx_media,
        .p_driver_info = &g_sf_el_fx_cfg,
        .p_memory = g_media_memory_g_fx_media,
        .memory_size = sizeof(g_media_memory_g_fx_media),
        .volume_name = (int8_t*)"Volume 1",
        .number_of_fats = 1,
        .directory_entries = 256,
        .hidden_sectors = 0,
        .total_sectors = 3751936,
        .bytes_per_sector = 512,
        .sectors_per_cluster = 1
    };

    error = SF_EL_FX_MediaFormat(&g_fx_media_format);

    if(error != SSP_SUCCESS)
    {
        error = SSP_ERR_MEDIA_FORMAT_FAILED;
    }

#endif

    fx_ret_val = fx_media_open (&g_fx_media, (int8_t *) "g_fx_media", SF_EL_FX_BlockDriver, &g_sf_el_fx_cfg,
                                g_media_memory_g_fx_media, sizeof(g_media_memory_g_fx_media));
    if (fx_ret_val != FX_SUCCESS)
    {
        error = SSP_ERR_INTERNAL;
    }
    ssp_err_t ssp_err_g_sf_console;
    ssp_err_g_sf_console = g_sf_console.p_api->open (g_sf_console.p_ctrl, g_sf_console.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_console)
    {
        while (1)
            ;
    }

    /* Enter user code for this thread. */
    dev_example_thread_entry ();
}
