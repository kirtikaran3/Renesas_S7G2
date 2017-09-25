/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
 * display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
 * purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
 * SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
 * NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
 * INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
 * be subject to different terms.
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : dev_example_cli_thread_entry.c
 * Description  : TODO
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           TODO       1.00    Initial Release.
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @defgroup DeveloperExample Application
 *
 * @brief Developer example demonstrating the HAL APIs for S7G2 Internal hp flash
 *
 * To use this program:
 *   1. Build, download, debug, and run.
 *   2. Connect a USB micro cable to connector J2 on the S7G2 DK version 2.1
 *   3. Connect to the terminal on your PC at 460800 baud.
 *   4. Press any key to start the demo.
 *   5. Press '?' then 'Enter' to view the help menu.
 *
 * @{
 **********************************************************************************************************************/
#include "dev_example_thread.h"
#include "dev_example_common.h"

/***********************************************************************************************************************
Macro definitions
 ***********************************************************************************************************************/
/* This App version */
#define MAJOR_NUMBER             ('1')
#define MINOR_NUMBER             ('1')
#define PATCH_NUMBER             ('0')

#define HAL_SCI_I2C_COMMAND     "r_sci_i2c"
#define HAL_FLASH_COMMAND       "r_flash_hp"
#define HAL_ADC_COMMAND         "r_adc"
#define HAL_TIMER_COMMAND       "r_gpt"
#define HAL_DAC_COMMAND         "r_dac"
#define HAL_RTC_COMMAND         "r_rtc"
#define HAL_CRC_COMMAND         "r_crc"
#define HAL_QSPI_FLASH_COMMAND  "r_qspi"
#define HAL_WDT_COMMAND   		"r_wdt"
#define HAL_EXTIRQ_COMMAND      "r_icu"
#define HAL_LPM_COMMAND         "r_lpm"
#define HAL_IOPORT_COMMAND      "r_ioport"


#define SF_ADC_COMMAND          "sf_adc_periodic"
#define SF_AUDIO_COMMAND        "sf_audio_playback"
#define SF_I2C_COMMAND          "sf_i2c"
#define HAL_TIMER_AGT_COMMAND   "r_agt"
#define SF_TM_COMMAND           "sf_thread_monitor"
#define SF_COMMS_COMMAND        "sf_el_ux_comms"
#define SF_FX_COMMAND           "sf_el_fx"
#define SF_TOUCH_COMMAND        "sf_touch_panel_i2c"
#define SF_EL_NX_COMMS_COMMANDS "sf_el_nx_comms"

#define SF_EXTIRQ_COMMAND       "sf_external_irq"
#define HAL_FMI_COMMAND         "r_fmi"
#define HAL_HELP_COMMAND        "help"

#define STR_NOT_SUPPORTED           "Not supported in this version"

#define HAL_SCI_I2C_HELP_STRING     "Exercise SCI_I2C HAL APIs"
#define HAL_FLASH_HELP_STRING       "Exercise Flash HAL APIs"
#define HAL_QSPI_FLASH_HELP_STRING  "Exercise QSPI Flash HAL APIs"
#define HAL_ADC_HELP_STRING         "Exercise ADC HAL APIs "
#define HAL_TIMER_GPT_HELP_STRING   "Exercise Timer GPT HAL APIs"
#define HAL_CRC_HELP_STRING         "Exercise CRC HAL APIs"
#define HAL_DAC_HELP_STRING         "Exercise DAC HAL APIs"
#define HAL_RTC_HELP_STRING         "Exercise RTC HAL APIs"
#define SF_AUDIO_HELP_STRING        "Exercise Audio framework APIs"
#define SF_ADC_HELP_STRING          "Exercise ADC framework APIs"
#define SF_I2C_HELP_STRING          "Runs a I2c Framework Application"
#define SF_TM_HELP_STRING           "Runs a Thread Monitor Framework Application"
#define HAL_QSPI_FLASH_HELP_STRING  "Exercise QSPI Flash HAL APIs"
#define HAL_TIMER_AGT_HELP_STRING   "Exercise Timer AGT HAL"
#define SF_COMMS_HELP_STRING        "Exercise COMMS framework APIs"
#define HAL_WDT_HELP_STRING         "Exercise WDT HAL APIs"
#define HAL_EXTIRQ_HELP_STRING      "Exercise EXTIRQ API's "
#define SF_FX_HELP_STRING           "Exercise the FileX framework API's"
#define HAL_LPM_HELP_STRING         "Exercise LPM HAL API's "
#define SF_TOUCH_HELP_STRING        "Run I2c Touch framework Application"
#define SF_EXTIRQ_HELP_STRING       "Exercise the External IRQ framework API's"
#define HAL_FMI_HELP_STRING         "Exercise the FMI hal API's"
#define HAL_IOPORT_HELP_STRING      "Exercise IOPORT HAL API's"
#define HELP_COMMAND_HELP_STRING    "Prints the help information for Developer Example"
#define SF_EL_NX_COMMS_HELP_STRING  "Runs a COMMS framework on NETX Application"


/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/

void ssp_error_code_to_string(ssp_err_t error_code);
void dev_example_thread_entry(void);
void dev_example_help(sf_console_cb_args_t *p_args);

/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/
extern sf_console_menu_t g_flash_hal_menu;
extern sf_console_menu_t g_sci_i2c_hal_menu;
extern sf_console_menu_t g_adc_hal_menu;
extern sf_console_menu_t g_timer_hal_menu;
extern sf_console_menu_t g_dac_hal_menu;
extern sf_console_menu_t g_audio_menu;
extern sf_console_menu_t g_qspi_flash_hal_menu;
extern sf_console_menu_t g_sf_adc_menu;
extern sf_console_menu_t g_crc_hal_menu;
extern sf_console_menu_t g_sf_i2c_menu;
extern sf_console_menu_t g_rtc_hal_menu;
extern sf_console_menu_t g_wdt_hal_menu;
extern sf_console_menu_t g_sf_comms_menu;
extern sf_console_menu_t g_timer_agt_hal_menu;
extern sf_console_menu_t g_sf_tread_monitor_menu;
extern sf_console_menu_t g_sf_fx_menu;
extern sf_console_menu_t g_extirq_hal_menu;
extern sf_console_menu_t g_ioport_hal_menu;
extern sf_console_menu_t g_sf_external_IRQ_menu;
extern sf_console_menu_t g_hal_fmi_menu;
extern sf_console_menu_t g_sf_touch_menu;
extern sf_console_menu_t g_lpm_hal_menu;
extern sf_console_menu_t g_sf_el_nx_comms_menu;

const char HELP_MESSAGE[] =
        "Developer Example application allow the user to run commands that exercise the "
        "corresponding HAL APIs or Framework applications.\r\n\r\n"
        "1. To invoke a command, type the command name and press Enter/Return key.\r\n\r\n"
        "2. Entering any command in the main menu, would take the user to its sub menu. "
        "This is indicated by change in prompt. \r\n\r\n"
        "3. To return to main menu press tilde(~) key followed by Enter/Return key. \r\n\r\n"
        "4. Commands and arguments are NOT case sensitive i.e. invoking hal_crc command using hal_crc, "
        "HAL_CRC or hal_CRC will give the same results. \r\n\r\n"
        "5. Commands that take arguments are depicted as <command> <arg_name># .. in the "
        "help string.Here '#' should be replaced with appropriate values as dictated by the command help.\r\n";

/** Root console menu commands */
const sf_console_command_t g_console_commands[] =
{
 {
  .command  = (uint8_t*)HAL_HELP_COMMAND,
  .help     = (uint8_t*)HELP_COMMAND_HELP_STRING,
  .callback = dev_example_help,
  .context  = NULL
 },
 {
  .command  = (uint8_t*)HAL_FLASH_COMMAND,
  .help     = (uint8_t*)HAL_FLASH_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_flash_hal_menu
 },

 {
  .command  = (uint8_t*)HAL_SCI_I2C_COMMAND,
  .help     = (uint8_t*)HAL_SCI_I2C_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_sci_i2c_hal_menu
 },

 {
  .command  = (uint8_t*)HAL_TIMER_COMMAND,
  .help     = (uint8_t*)HAL_TIMER_GPT_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_timer_hal_menu
 },

 {
  .command  = (uint8_t*)HAL_ADC_COMMAND,
  .help     = (uint8_t*)HAL_ADC_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_adc_hal_menu
 },

 {
  .command  = (uint8_t*)HAL_QSPI_FLASH_COMMAND,
  .help     = (uint8_t*)HAL_QSPI_FLASH_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_qspi_flash_hal_menu
 },

 {
  .command  = (uint8_t*)HAL_CRC_COMMAND,
  .help     = (uint8_t*)HAL_CRC_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_crc_hal_menu
 },

 {
  .command  = (uint8_t*)HAL_DAC_COMMAND,
  .help     = (uint8_t*)HAL_DAC_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_dac_hal_menu
 },
 {
  .command  = (uint8_t*)HAL_TIMER_AGT_COMMAND,
  .help     = (uint8_t*)HAL_TIMER_AGT_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_timer_agt_hal_menu
 },

 {
  .command  = (uint8_t*)HAL_WDT_COMMAND,
  .help     = (uint8_t*)HAL_WDT_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_wdt_hal_menu
 },
{
  .command  = (uint8_t*)HAL_RTC_COMMAND,
  .help     = (uint8_t*)HAL_RTC_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_rtc_hal_menu
},
{
  .command  = (uint8_t*)HAL_EXTIRQ_COMMAND,
  .help     = (uint8_t*)HAL_EXTIRQ_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_extirq_hal_menu
},
{
  .command  = (uint8_t*)HAL_FMI_COMMAND,
  .help     = (uint8_t*)HAL_FMI_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_hal_fmi_menu
},
{
 .command  = (uint8_t*)HAL_LPM_COMMAND,
 .help     = (uint8_t*)HAL_LPM_HELP_STRING,
 .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
 .context  = &g_lpm_hal_menu
},
{
 .command  = (uint8_t*)HAL_IOPORT_COMMAND,
 .help     = (uint8_t*)HAL_IOPORT_HELP_STRING,
 .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
 .context  = &g_ioport_hal_menu
},
{
  .command  = (uint8_t*)SF_AUDIO_COMMAND,
  .help     = (uint8_t*)SF_AUDIO_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_audio_menu
},
{
  .command  = (uint8_t*)SF_I2C_COMMAND,
  .help     = (uint8_t*)SF_I2C_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_sf_i2c_menu
},

 {
  .command  = (uint8_t*)SF_ADC_COMMAND,
  .help     = (uint8_t*)SF_ADC_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_sf_adc_menu
 },
 {
  .command  = (uint8_t*)SF_TM_COMMAND,
  .help     = (uint8_t*)SF_TM_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_sf_tread_monitor_menu
 },
 {
  .command  = (uint8_t*)SF_FX_COMMAND,
  .help     = (uint8_t*)SF_FX_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_sf_fx_menu
 },
{
  .command  = (uint8_t*)SF_COMMS_COMMAND,
  .help     = (uint8_t*)SF_COMMS_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_sf_comms_menu
},
{
 .command  = (uint8_t*)SF_TOUCH_COMMAND,
 .help     = (uint8_t*)SF_TOUCH_HELP_STRING,
 .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
 .context  = &g_sf_touch_menu
},
{
  .command  = (uint8_t*)SF_EXTIRQ_COMMAND,
  .help     = (uint8_t*)SF_EXTIRQ_HELP_STRING,
  .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
  .context  = &g_sf_external_IRQ_menu
},
{
 .command  = (uint8_t*)SF_EL_NX_COMMS_COMMANDS,
 .help     = (uint8_t*)SF_EL_NX_COMMS_HELP_STRING,
 .callback = SF_CONSOLE_CALLBACK_NEXT_FUNCTION,
 .context  = &g_sf_el_nx_comms_menu
},
};


/** Root console menu */
const sf_console_menu_t g_sf_console_root_menu =
{
 .menu_name = (uint8_t*)"synergy",
 .num_commands = (sizeof(g_console_commands)) / (sizeof(g_console_commands[0])),
 .command_list = g_console_commands
};

bsp_leds_t ledpresent;


void dev_example_thread_entry(void)
{

    char        str[32];
    ioport_level_t level;

#ifdef BSP_BOARD_S7G2_DK
    /** Enable audio. */
    g_ioport_on_ioport.pinWrite(IOPORT_PORT_06_PIN_08, IOPORT_LEVEL_HIGH);
    g_ioport_on_ioport.pinDirectionSet(IOPORT_PORT_06_PIN_08, IOPORT_DIRECTION_OUTPUT);
    g_ioport_on_ioport.pinWrite(IOPORT_PORT_09_PIN_02, IOPORT_LEVEL_HIGH);
    g_ioport_on_ioport.pinDirectionSet(IOPORT_PORT_09_PIN_02, IOPORT_DIRECTION_OUTPUT);
    g_ioport_on_ioport.pinCfg(IOPORT_PORT_00_PIN_14, IOPORT_CFG_ANALOG_ENABLE);
#endif
    /** Start demo by pressing any character (this gives time for user to connect PC console) */
    uint8_t ch[2] = {'\0'};

    R_BSP_LedsGet(&ledpresent);  // Get LED list from BSP

    /** Glowing LED2 just after bootup which will help user understand system is ready to use and even to make
     *      out when reset is pressed */
    g_ioport.p_api->pinRead(ledpresent.p_leds[2], &level);  // Read current level
    g_ioport.p_api->pinWrite(ledpresent.p_leds[2], !level); // Write opposite level


    g_sf_console.p_api->read(g_sf_console.p_ctrl, ch, 1, TX_WAIT_FOREVER);

    /** Print splash to console. */
    print_to_console("\r\n********************************************************");

    print_to_console("\r\n*         Synergy Developer Example applications       *");

    sprintf (str,    "\r\n*                     Version %c.%c.%c                    *",
            MAJOR_NUMBER, MINOR_NUMBER, PATCH_NUMBER);
    print_to_console(str);
    print_to_console("\r\n*              Hit ? to show command list              *");

    print_to_console("\r\n********************************************************\r\n");

    while (1) {
        /** Prompt based on current menu */
        g_sf_console.p_api->prompt(g_sf_console.p_ctrl, NULL, TX_WAIT_FOREVER);

        tx_thread_sleep(10);
    }
}


/*******************************************************************************************************************//**
 *  @brief  Print error message with respect to error code.
 *
 *  @param[in] error_code - ssp_err_t number
 *  @retval VOID
 ***********************************************************************************************************************/

void ssp_error_code_to_string(ssp_err_t error_code)
{
    const char* ssp_common_error_codes[]=
    {
     "SSP_SUCCESS",
     "SSP_ERR_ASSERTION",
     "SSP_ERR_INVALID_POINTER",
     "SSP_ERR_INVALID_ARGUMENT",
     "SSP_ERR_INVALID_CHANNEL",
     "SSP_ERR_INVALID_MODE",
     "SSP_ERR_UNSUPPORTED ",
     "SSP_ERR_NOT_OPEN",
     "SSP_ERR_IN_USE",
     "SSP_ERR_OUT_OF_MEMORY",
     "SSP_ERR_HW_LOCKED",
     "SSP_ERR_IRQ_BSP_DISABLED",
     "SSP_ERR_OVERFLOW",
     "SSP_ERR_UNDERFLOW",
     "SSP_ERR_ALREADY_OPEN",
     "SSP_ERR_APPROXIMATION",
     "SSP_ERR_CLAMPED",
     "SSP_ERR_INVALID_RATE",
     "SSP_ERR_ABORTED",
     "SSP_ERR_NOT_ENABLED",
     "SSP_ERR_TIMEOUT",
     "SSP_ERR_INVALID_BLOCKS",
     "SSP_ERR_INVALID_ADDRESS",
     "SSP_ERR_INVALID_SIZE",
     "SSP_ERR_WRITE_FAILED",
     "SSP_ERR_ERASE_FAILED",
     "SSP_ERR_INVALID_CALL",
     "SSP_ERR_INVALID_HW_CONDITION"
    };

    const uint32_t ssp_flash_error_code_offset = 500;
    const char* ssp_flash_error_codes[]=
    {
     "SSP_ERR_PE_FAILURE",
     "SSP_ERR_CMD_LOCKED",
     "SSP_ERR_FCLK"
    };

    char error_string[32] = {0};
    uint32_t error_code_index = error_code;

    if(error_code <= SSP_ERR_INVALID_HW_CONDITION)
    {
        sprintf(error_string, "%s\r\n",ssp_common_error_codes[error_code_index]);
    }
    else
    {
        error_code_index = error_code - ssp_flash_error_code_offset;

        if(error_code_index <= SSP_ERR_FCLK )
        {
            sprintf(error_string, "%s\r\n",ssp_flash_error_codes[error_code_index]);
        }
        else
        {
            sprintf(error_string,"%d\r\n", error_code);
        }
    }

    print_to_console(error_string);
}

/*******************************************************************************************************************//**
 * @brief  Brief instructions regarding Developer Example
 *
 * @example help
 *  @retval VOID
 ***********************************************************************************************************************/

void dev_example_help(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    g_sf_comms.p_api->write(g_sf_comms.p_ctrl,(uint8_t*)HELP_MESSAGE,sizeof(HELP_MESSAGE),TX_WAIT_FOREVER);
}
