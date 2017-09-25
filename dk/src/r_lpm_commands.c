
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
 * File Name    : r_lpm_commands.c
 * Description  : This file is used to exercise the APIs of LPM HAL module.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           TODO  1.00    Initial Release
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/
#include <stdint.h>
#include "dev_example_thread.h"
#include "dev_example_common.h"

/***********************************************************************************************************************
Macro definitions
 ***********************************************************************************************************************/

/** Command Menu */
#define LPM_INIT                             "init"
#define LPM_MSTPCRSET                        "mstpcrSet"
#define LPM_MSTPCRGET                        "mstpcrGet"
#define LPM_MODULESTOP                       "moduleStop"
#define LPM_MODULESTART                      "moduleStart"
#define LPM_OPERATINGPOWERMODESET            "operatingPowerModeSet"
#define LPM_SNOOZEENABLE                     "snoozeEnable"
#define LPM_SNOOZEDISABLE                    "snoozeDisable"
#define LPM_LOWPOWERCFG                      "lowPowerCfg"
#define LPM_WUPENSET                         "wupenSet"
#define LPM_WUPENGET                         "wupenGet"
#define LPM_DEEPSTANDBYCANCELREQUESTENABLE   "deepStandbyCancelRequestEnable"
#define LPM_DEEPSTANDBYCANCELREQUESTDISBALE  "deepStandbyCancelRequestDisable"
#define LPM_ENTERLOWPOWERMODE                "enterLowPowerMode"
#define LPM_VERSIONGET                       "versionGet"


/**  HELP MENU   */

#define HELP_LPM_INIT                            "Open the LPM driver module Initialized the LPM"
#define HELP_LPM_MSTPCRSET                       "Set value of MSTPCRs\r\n\tSynopsis:- mstpcrSet modA# "\
		                                                "modB# modC# modD#"
#define HELP_LPM_MSTPCRGET                       "Get the values of all the Module Stop Control Registers"
#define HELP_LPM_MODULESTOP                      "Stop a module"
#define HELP_LPM_MODULESTART                     "Run a module"
#define HELP_LPM_OPERATINGPOWERMODESET           "Set power mode\r\n\tSynopsis:- operatingPowerModeSet "\
		                                                "mode#(0-3) subosc#(0-1)"
#define HELP_LPM_SNOOZEENABLE                    "Config & enable snooze\r\n\tSynopsis:- "\
		                                                "snoozeEnable rxd# dtc# request# trigger#"
#define HELP_LPM_SNOOZEDISABLE                   "Disable snooze mode"
#define HELP_LPM_LOWPOWERCFG                     "Config low power mode\r\n\tSynopsis:- lowPowerCfg mode#"\
		                                                " enable# power# port#"
#define HELP_LPM_WUPENSET                        "Set value of WakeUp Interrupt Enable Register\r\n\t"\
		                                                "Synopsis:- wupenSet value#"
#define HELP_LPM_WUPENGET                        "Get the value of WakeUp Interrupt Enable Register"
#define HELP_LPM_DEEPSTANDBYCANCELREQUESTENABLE  "Enable Deep Standby\r\n\tSynopsis:- deepStandbyCancelRequestEnable"\
		                                                " pin# edge#"
#define HELP_LPM_DEEPSTANDBYCANCELREQUESTDISBALE "Disable Deep Standby\r\n\tSynopsis:- "\
		                                                "deepStandbyCancelRequestDisable pin#"
#define HELP_LPM_ENTERLOWPOWERMODE               "Enter low power mode"
#define HELP_LPM_VERSIONGET                      "Get the driver version based on compile time macros"

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/

void hal_lpm_init(sf_console_cb_args_t *p_args);
void hal_lpm_mstpcrSet(sf_console_cb_args_t *p_args);
void hal_lpm_mstpcrGet(sf_console_cb_args_t *p_args);
void hal_lpm_moduleStop(sf_console_cb_args_t *p_args);
void hal_lpm_moduleStart(sf_console_cb_args_t *p_args);
void hal_lpm_operatingPowerModeSet(sf_console_cb_args_t *p_args);
void hal_lpm_snoozeEnable(sf_console_cb_args_t *p_args);
void hal_lpm_snoozeDisable(sf_console_cb_args_t *p_args);
void hal_lpm_lowPowerCfg(sf_console_cb_args_t *p_args);
void hal_lpm_wupenSet(sf_console_cb_args_t *p_args);
void hal_lpm_wupenGet(sf_console_cb_args_t *p_args);
void hal_lpm_deepStandbyCancelRequestEnable(sf_console_cb_args_t *p_args);
void hal_lpm_deepStandbyCancelRequestDisable(sf_console_cb_args_t *p_args);
void hal_lpm_enterLowPowerMode(sf_console_cb_args_t *p_args);
void hal_lpm_versionGet(sf_console_cb_args_t *p_args);

/* Global variables */
/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/

/* lpm Commands Deceleration  */
const sf_console_command_t g_hal_lpm_commands[] =
{
 {
    .command   =  (uint8_t*)LPM_INIT,
    .help      =  (uint8_t*)HELP_LPM_INIT,
    .callback  =  hal_lpm_init,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_MSTPCRSET,
    .help      =  (uint8_t*)HELP_LPM_MSTPCRSET,
    .callback  =  hal_lpm_mstpcrSet,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_MSTPCRGET,
    .help      =  (uint8_t*)HELP_LPM_MSTPCRGET,
    .callback  =  hal_lpm_mstpcrGet,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_MODULESTOP,
    .help      =  (uint8_t*)HELP_LPM_MODULESTOP,
    .callback  =  hal_lpm_moduleStop,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_MODULESTART,
    .help      =  (uint8_t*)HELP_LPM_MODULESTART,
    .callback  =  hal_lpm_moduleStart,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_OPERATINGPOWERMODESET,
    .help      =  (uint8_t*)HELP_LPM_OPERATINGPOWERMODESET,
    .callback  =  hal_lpm_operatingPowerModeSet,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_SNOOZEENABLE,
    .help      =  (uint8_t*)HELP_LPM_SNOOZEENABLE,
    .callback  =  hal_lpm_snoozeEnable,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_SNOOZEDISABLE,
    .help      =  (uint8_t*)HELP_LPM_SNOOZEDISABLE,
    .callback  =  hal_lpm_snoozeDisable,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_LOWPOWERCFG,
    .help      =  (uint8_t*)HELP_LPM_LOWPOWERCFG,
    .callback  =  hal_lpm_lowPowerCfg,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_WUPENSET,
    .help      =  (uint8_t*)HELP_LPM_WUPENSET,
    .callback  =  hal_lpm_wupenSet,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_WUPENGET,
    .help      =  (uint8_t*)HELP_LPM_WUPENGET,
    .callback  =  hal_lpm_wupenGet,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_DEEPSTANDBYCANCELREQUESTENABLE,
    .help      =  (uint8_t*)HELP_LPM_DEEPSTANDBYCANCELREQUESTENABLE,
    .callback  =  hal_lpm_deepStandbyCancelRequestEnable,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_DEEPSTANDBYCANCELREQUESTDISBALE,
    .help      =  (uint8_t*)HELP_LPM_DEEPSTANDBYCANCELREQUESTDISBALE,
    .callback  =  hal_lpm_deepStandbyCancelRequestDisable,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_ENTERLOWPOWERMODE,
    .help      =  (uint8_t*)HELP_LPM_ENTERLOWPOWERMODE,
    .callback  =  hal_lpm_enterLowPowerMode,
    .context   =  NULL
 },
 {
    .command   =  (uint8_t*)LPM_VERSIONGET,
    .help      =  (uint8_t*)HELP_LPM_VERSIONGET,
    .callback  =  hal_lpm_versionGet,
    .context   =  NULL
 },
};//end//



sf_console_menu_t g_lpm_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .num_commands = sizeof(g_hal_lpm_commands)/sizeof(g_hal_lpm_commands[0]),
 .menu_name = (uint8_t*)"r_lpm",
 .command_list = g_hal_lpm_commands
};


uint32_t snooze_request[25]={
                                LPM_SNOOZE_REQUEST_RXD0_FALLING,
                                LPM_SNOOZE_REQUEST_IRQ0,
                                LPM_SNOOZE_REQUEST_IRQ1,
                                LPM_SNOOZE_REQUEST_IRQ2,
                                LPM_SNOOZE_REQUEST_IRQ3,
                                LPM_SNOOZE_REQUEST_IRQ4,
                                LPM_SNOOZE_REQUEST_IRQ5,
                                LPM_SNOOZE_REQUEST_IRQ6,
                                LPM_SNOOZE_REQUEST_IRQ7,
                                LPM_SNOOZE_REQUEST_IRQ8,
                                LPM_SNOOZE_REQUEST_IRQ9,
                                LPM_SNOOZE_REQUEST_IRQ10,
                                LPM_SNOOZE_REQUEST_IRQ11,
                                LPM_SNOOZE_REQUEST_IRQ12,
                                LPM_SNOOZE_REQUEST_IRQ13,
                                LPM_SNOOZE_REQUEST_IRQ14,
                                LPM_SNOOZE_REQUEST_IRQ15,
                                LPM_SNOOZE_REQUEST_KR,
                                LPM_SNOOZE_REQUEST_COMPARATOR_OC0,
                                LPM_SNOOZE_REQUEST_COMPARATOR_LP,
                                LPM_SNOOZE_REQUEST_RTC_ALARM,
                                LPM_SNOOZE_REQUEST_RTC_PERIOD,
                                LPM_SNOOZE_REQUEST_AGT1_UNDERFLOW,
                                LPM_SNOOZE_REQUEST_AGT1_COMPARE_A,
                                LPM_SNOOZE_REQUEST_AGT1_COMPARE_B
                            };

uint32_t deep_standby[24]=
                            {
                                LPM_DEEP_STANDBY_IRQ0_DS,
                                LPM_DEEP_STANDBY_IRQ1_DS,
                                LPM_DEEP_STANDBY_IRQ2_DS,
                                LPM_DEEP_STANDBY_IRQ3_DS,
                                LPM_DEEP_STANDBY_IRQ4_DS,
                                LPM_DEEP_STANDBY_IRQ5_DS,
                                LPM_DEEP_STANDBY_IRQ6_DS,
                                LPM_DEEP_STANDBY_IRQ7_DS,

                                LPM_DEEP_STANDBY_IRQ8_DS,
                                LPM_DEEP_STANDBY_IRQ9_DS,
                                LPM_DEEP_STANDBY_IRQ10_DS,
                                LPM_DEEP_STANDBY_IRQ11_DS,
                                LPM_DEEP_STANDBY_IRQ12_DS,
                                LPM_DEEP_STANDBY_IRQ13_DS,
                                LPM_DEEP_STANDBY_IRQ14_DS,
                                LPM_DEEP_STANDBY_IRQ15_DS,

                                LPM_DEEP_STANDBY_LVD1,
                                LPM_DEEP_STANDBY_LVD2,
                                LPM_DEEP_STANDBY_RTC_INTERVAL,
                                LPM_DEEP_STANDBY_RTC_ALARM,
                                LPM_DEEP_STANDBY_NMI,

                                LPM_DEEP_STANDBY_USBFS,
                                LPM_DEEP_STANDBY_USBHS,
                                LPM_DEEP_STANDBY_AGT1,
                            };

/***********************************************************************************************************************
Function
 ***********************************************************************************************************************/


/**********************************************************************************************************************
 * @brief Initialize the mcu operating power mode
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_init(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_lpm.p_api->init(g_lpm.p_cfg);
    ssp_error_code_to_string(ssp_err);
}


/**********************************************************************************************************************
 * @brief Set the value of all the Module Stop Control Registers
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_mstpcrSet(sf_console_cb_args_t *p_args)
{
    uint32_t mstpcra_value = 0,mstpcrb_value = 0, mstpcrc_value = 0, mstpcrd_value = 0;

    /** Extracting data for MSTPCRA, MSTPCRB, MSTPCRC and MSTPCRD register from argument*/
    if(!get_arg_as_uint("modA", p_args->p_remaining_string, &mstpcra_value, 16))
    {
        print_to_console("Invalid value for MSTRCRA\r\n");
        return;
    }
    if(!get_arg_as_uint("modB", p_args->p_remaining_string, &mstpcrb_value, 16))
    {
        print_to_console("Invalid value for MSTRCRB\r\n");
        return;
    }
    if(!get_arg_as_uint("modC", p_args->p_remaining_string, &mstpcrc_value, 16))
    {
        print_to_console("Invalid value for MSTRCRC\r\n");
        return;
    }
    if(!get_arg_as_uint("modD", p_args->p_remaining_string, &mstpcrd_value, 16))
    {
        print_to_console("Invalid value for MSTRCRD\r\n");
        return;
    }

    /** Restricting user to stop SRAM module */
    if(mstpcra_value & (0x000000e3))
    {
        print_to_console("Stopping SRAM will cause to reset device\r\n");
        mstpcra_value = mstpcra_value & 0xffffff1c;
    }

    /** Restricting user to stop USBFS module. Preventing console blocking*/
    if(mstpcrb_value & (0x00000800))
    {
        print_to_console("NOT valid\r\nStopping USBFS will affect console\r\n");
        mstpcrb_value = mstpcrb_value & 0xfffff7ff;
    }

    ssp_err_t ssp_err = g_lpm.p_api->mstpcrSet(mstpcra_value, mstpcrb_value, mstpcrc_value, mstpcrd_value);
    ssp_error_code_to_string(ssp_err);
}

/**********************************************************************************************************************
 * @brief Get the values of all the Module Stop Control Registers
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 *********************************************************************************************************************/
void hal_lpm_mstpcrGet(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    char   result[32];
    uint32_t mstpcra_value = 0, mstpcrb_value = 0, mstpcrc_value = 0, mstpcrd_value = 0;

    ssp_err_t ssp_err = g_lpm.p_api->mstpcrGet(&mstpcra_value, &mstpcrb_value, &mstpcrc_value, &mstpcrd_value);
    ssp_error_code_to_string(ssp_err);

    /** Printing the values of MSTPCR registers if mstpcrGet() returns success */
    if( SSP_SUCCESS == ssp_err )
    {
         sprintf (result,"mstpcra_value  =  0x%x\r\n", (unsigned int)mstpcra_value );
         print_to_console(result);
         sprintf (result,"mstpcrb_value  =  0x%x\r\n", (unsigned int)mstpcrb_value );
         print_to_console(result);
         sprintf (result,"mstpcrc_value  =  0x%x\r\n", (unsigned int)mstpcrc_value );
         print_to_console(result);
         sprintf (result,"mstpcrd_value  =  0x%x\r\n", (unsigned int)mstpcrd_value );
         print_to_console(result);
    }
}


/**********************************************************************************************************************
 * @brief Stop a module. In this case we allow user to stop only 6 modules i.e. QSPI,SCI7,CAC,CRC,AGT! and AGT0
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 *********************************************************************************************************************/
void hal_lpm_moduleStop(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    lpm_mstp_t module;
    uint8_t input;

    print_to_console("select which module to stop\r\n");
    print_to_console("1 QSPI\n\r2 SCI7\r\n3 CAC\r\n4 CRC\r\n5 AGT1\r\n6 AGT0\r\n");
    g_sf_console.p_api->read(g_sf_console.p_ctrl,&input,1,TX_WAIT_FOREVER);
    print_to_console("\r\n");

    /** Limiting user to stop only below six modules */
    switch((uint32_t)(input - '0'))
    {
        case 1:
            {
                module = LPM_MSTP_QSPI;
                break;
            }
        case 2:
            {
                module = LPM_MSTP_SCI7;
                break;
            }
        case 3:
            {
                module = LPM_MSTP_CAC;
                break;
            }
        case 4:
            {
                module = LPM_MSTP_CRC;
                break;
            }
        case 5:
            {
                module = LPM_MSTP_AGT1;
                break;
            }
        case 6:
            {
                module = LPM_MSTP_AGT0;
                break;
            }
        default:
            {
                print_to_console("Entered option is wrong\r\n");
                return;
            }
    }

    ssp_err_t ssp_err = g_lpm.p_api->moduleStop(module);
    ssp_error_code_to_string(ssp_err);
}

/**********************************************************************************************************************
 * @brief Run a module. In this case we allow user to start only 6 modules i.e. QSPI,SCI7,CAC,CRC,AGT! and AGT0
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_moduleStart(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    lpm_mstp_t module;
    uint8_t input;

    print_to_console("select which module to start\r\n");
    print_to_console("1 QSPI\n\r2 SCI7\r\n3 CAC\r\n4 CRC\r\n5 AGT1\r\n6 AGT0\r\n");
    g_sf_console.p_api->read(g_sf_console.p_ctrl,&input,1,TX_WAIT_FOREVER);
    print_to_console("\r\n");

    /** Limiting user to start only below six modules */
    switch((uint32_t)(input - '0'))
    {
        case 1:
            {
                module = LPM_MSTP_QSPI;
                break;
            }
        case 2:
            {
                module = LPM_MSTP_SCI7;
                break;
            }
        case 3:
            {
                module = LPM_MSTP_CAC;
                break;
            }
        case 4:
            {
                module = LPM_MSTP_CRC;
                break;
            }
        case 5:
            {
                module = LPM_MSTP_AGT1;
                break;
            }
        case 6:
            {
                module = LPM_MSTP_AGT0;
                break;
            }
        default:
            {
                print_to_console("Entered option is wrong\r\n");
                return;
            }
    }

    ssp_err_t ssp_err = g_lpm.p_api->moduleStart(module);
    ssp_error_code_to_string(ssp_err);
}


/**********************************************************************************************************************
 * @brief Set the operating power mode
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_operatingPowerModeSet(sf_console_cb_args_t *p_args)
{
    int32_t power_mode,subosc;

    /** Extracting values of power mode and subosc from argument*/
    if(!get_arg_as_uint("mode", p_args->p_remaining_string, (uint32_t*)&power_mode,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("subosc", p_args->p_remaining_string, (uint32_t*)&subosc,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validating power_mode and subosc value */
    if((0 > power_mode) || (power_mode >= 4) || (0 > subosc) ||  (subosc > 1))
    {
        print_to_console("Invalid Arguments\r\n");
    }
    else
    {
        ssp_err_t ssp_err = g_lpm.p_api->operatingPowerModeSet(power_mode, subosc);
        ssp_error_code_to_string(ssp_err);
    }
}

/**********************************************************************************************************************
 * @brief Configure and enable snooze mode
 *
 * NOTE: this function must be called before entering Software Standby mode, otherwise snooze mode will not be entered
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_snoozeEnable(sf_console_cb_args_t *p_args)
{
    int32_t rxd,dtc,request,trigger;

    /** Extracting values of RXD0, DTC, Snooze Request and triggers from argument */
    if(!get_arg_as_uint("rxd", p_args->p_remaining_string, (uint32_t*)&rxd,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("dtc", p_args->p_remaining_string, (uint32_t*)&dtc,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("request", p_args->p_remaining_string, (uint32_t*)&request,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("trigger", p_args->p_remaining_string, (uint32_t*)&trigger,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }


    /** validating RXD0, DTC, Snooze Request and triggers value */
    if((0 > rxd) || (rxd >= 2) || (0 > dtc) ||  (dtc >= 2) || (0 > request) || \
            (request >= 25) || (0 > trigger) ||  (trigger >= 8))
    {
        print_to_console("Invalid Arguments\r\n");
    }
    else
    {
        /**If using RXD0 falling edge as snooze request, any event other than RXD0 falling edge must not be enabled*/
        if(rxd)
        {
            request = LPM_SNOOZE_REQUEST_RXD0_FALLING;
            print_to_console("When rxd0 falling edge is selected only rxd snooze request is considered \r\n");
        }
        else
        {
            request = (int32_t)snooze_request[request];
        }
        ssp_err_t ssp_err = g_lpm.p_api->snoozeEnable(rxd,dtc,request,trigger);
        ssp_error_code_to_string(ssp_err);
    }
}


/**********************************************************************************************************************
 * @brief Disable snooze mode
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_snoozeDisable(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err = g_lpm.p_api->snoozeDisable();
    ssp_error_code_to_string(ssp_err);
}

/**********************************************************************************************************************
 * @brief Configure a low power mode
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_lowPowerCfg(sf_console_cb_args_t *p_args)
{
    int32_t power_mode,output_port_enable,power_supply,io_port_state;

    /** Extracting values of Power mode, output port enable, Power supply & IOPORT state from argument */
    if(!get_arg_as_uint("mode", p_args->p_remaining_string, (uint32_t*)&power_mode,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("enable", p_args->p_remaining_string, (uint32_t*)&output_port_enable,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("power", p_args->p_remaining_string, (uint32_t*)&power_supply,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("port", p_args->p_remaining_string, (uint32_t*)&io_port_state,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validating Power mode, output port enable, Power supply & IOPORT state value */
    if((0 > power_mode) || (power_mode >= 4) || (0 > output_port_enable) || \
            (output_port_enable >= 2) || (0 > power_supply) ||  (power_supply >= 3) || \
            (0 > io_port_state) ||  (io_port_state >= 2))
    {
        print_to_console("Invalid Arguments\r\n");
    }
    else
    {
        ssp_err_t ssp_err = g_lpm.p_api->lowPowerCfg(power_mode,output_port_enable,power_supply,io_port_state);
        ssp_error_code_to_string(ssp_err);
    }
}

/**********************************************************************************************************************
 * @brief Set the value of the Wake Up Interrupt Enable Register WUPEN
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_wupenSet(sf_console_cb_args_t *p_args)
{
    uint32_t wupen_value;

    /** Extracting values of WUPEN register from argument */
    if(!get_arg_as_uint("value", p_args->p_remaining_string, &wupen_value, 16))
    {
        print_to_console("Invalid value for WUPEN\r\n");
        return;
    }

    ssp_err_t ssp_err = g_lpm.p_api->wupenSet(wupen_value);
    ssp_error_code_to_string(ssp_err);
}


/**********************************************************************************************************************
 * @brief Get the value of the Wake Up Interrupt Enable Register WUPEN
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_wupenGet(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    char   result[32];
    uint32_t wupen_value;

    ssp_err_t ssp_err = g_lpm.p_api->wupenGet(&wupen_value);
    ssp_error_code_to_string(ssp_err);

    /** Printing the values of WUPEN registers if wupenGet() returns success */
    if( SSP_SUCCESS == ssp_err )
    {
         sprintf (result,"wupen_value  =  0x%x\r\n", (int)wupen_value );
         print_to_console(result);
    }
}

/**********************************************************************************************************************
 * @brief Enable a pin or signal that will cancel Deep Software Standby mode
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_deepStandbyCancelRequestEnable(sf_console_cb_args_t *p_args)
{
    int32_t pin_signal,rising_falling;

    /** Extracting values of pin signal and edge from argument */
    if(!get_arg_as_uint("pin", p_args->p_remaining_string, (uint32_t*)&pin_signal,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("edge", p_args->p_remaining_string, (uint32_t*)&rising_falling,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validating pin signal and edge value */
    if((0 > pin_signal) || (pin_signal >= 24) || (0 > rising_falling) ||  (rising_falling >= 2))
    {
        print_to_console("Invalid Arguments\r\n");
    }
    else
    {
        ssp_err_t ssp_err = g_lpm.p_api->deepStandbyCancelRequestEnable(deep_standby[pin_signal],rising_falling);
        ssp_error_code_to_string(ssp_err);
    }
}

/**********************************************************************************************************************
 * @brief Disable a pin or signal that will cancel Deep Software Standby mode
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_deepStandbyCancelRequestDisable(sf_console_cb_args_t *p_args)
{
    int32_t pin_signal;

    /** Extracting values of pin signal from argument*/
    if(!get_arg_as_uint("pin", p_args->p_remaining_string, (uint32_t*)&pin_signal,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** validating pin signal value */
    if((0 > pin_signal) || (pin_signal >= 24))
    {
        print_to_console("Invalid Arguments\r\n");
    }
    else
    {
        ssp_err_t ssp_err = g_lpm.p_api->deepStandbyCancelRequestDisable(deep_standby[pin_signal]);
        ssp_error_code_to_string(ssp_err);
    }
}


/**********************************************************************************************************************
 * @brief Enter low power mode (sleep/standby/deep standby) using WFI macro
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/

void hal_lpm_enterLowPowerMode(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);

    ssp_err_t ssp_err = g_lpm.p_api->lowPowerModeEnter();
    ssp_error_code_to_string(ssp_err);
}

/**********************************************************************************************************************
 * @brief Get the driver version based on compile time macros
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/
void hal_lpm_versionGet(sf_console_cb_args_t *p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    char   version[32];
    ssp_version_t version_details;

    ssp_err = g_lpm.p_api->versionGet(&version_details);
    ssp_error_code_to_string(ssp_err);

    /** Print the version value only if versionGet() results success*/
    if(SSP_SUCCESS == ssp_err )
    {
        sprintf (version,"Code version  =  %d.%d\r\n", version_details.code_version_major,
                    version_details.code_version_minor);
        print_to_console(version);

        sprintf (version,"\r\nAPI version  =  %d.%d\r\n", version_details.api_version_major,
                    version_details.api_version_minor);
        print_to_console(version);
    }
}


