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
 * File Name    : r_dac_commands.c
 * Description  : The general timer interface provides standard timer functionality including periodic mode, one-shot
mode, and free-running timer mode. After each timer cycle (overflow or underflow), an interrupt can be triggered.
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
#define NUM_OF_SINE_SAMPLES 11
#define NUM_OF_TRIANGLE_SAMPLES 21

/***********************************************************************************************************************
Global definitions
 ***********************************************************************************************************************/
extern void timer_callback_dac(timer_callback_args_t * p_args);
extern const timer_instance_t g_timer_dac;

dac_size_t triangle_samples[NUM_OF_TRIANGLE_SAMPLES] = {1,3,5,7,15,31,63,127,255,511,1023,511,255,127,63,31,15,7,5,3,1};
dac_size_t sine_samples[NUM_OF_SINE_SAMPLES] = {2048,3251,3995,3996,3253,2051,847,101,98,839};


dac_size_t *samples;
dac_size_t number_of_samples;

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/
/* Prototypes for the callback functions called by I2C console commands */

void dac_hal_open(sf_console_cb_args_t * p_args);
void dac_hal_close(sf_console_cb_args_t * p_args);
void dac_hal_write(sf_console_cb_args_t * p_args);
void dac_hal_start(sf_console_cb_args_t * p_args);
void dac_hal_stop(sf_console_cb_args_t * p_args);
void dac_hal_versionGet(sf_console_cb_args_t * p_args);



/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/


/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/
const sf_console_command_t g_dac_hal_commands[] =
{
 {
  (uint8_t*)"open",
  (uint8_t*)"Initial configuration",
  dac_hal_open,
  NULL
 },
 {
  (uint8_t*)"close",
  (uint8_t*)"Stop the D/A conversion,stop output,and close the DAC channel.",
  dac_hal_close,
  NULL
 },
 {
  (uint8_t*)"write",
  (uint8_t*)"Write data to the D/A converter\r\n\t\tSynopsis:- write WAVE#(0 - triangle wave, 1- sine wave)"
  "\r\n\t\tExample:- write WAVE1",
  dac_hal_write,
  NULL
 },
 {
  (uint8_t*)"start",
  (uint8_t*)"Start the D/A conversion output if it has not been started "
  "call write before start to choose the wave form",
  dac_hal_start,
  NULL
 },
 {
  (uint8_t*)"stop",
  (uint8_t*)"Stop the D/A conversion output if it has not been started.",
  dac_hal_stop,
  NULL
 },
 {
  (uint8_t*)"versionGet",
  (uint8_t*)"Gets the version of API",
  dac_hal_versionGet,
  NULL
 }

};


/** Dac HAL menu */
const sf_console_menu_t g_dac_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t const *)"r_dac",
 .num_commands = (sizeof(g_dac_hal_commands)) / (sizeof(g_dac_hal_commands[0])),
 .command_list = g_dac_hal_commands

};

/*******************************************************************************************************************//**
 * @brief  Configures Dac based on user input.
 * @pre    open should be called before use the dac hal module
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void dac_hal_open(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;

    ssp_err = g_dac.p_api->open(g_dac.p_ctrl,g_dac.p_cfg);
    ssp_error_code_to_string(ssp_err);
    /* The DAC is enabled for Audio framework by default so to make
     * Dac HAL open success we close it here to make open from CLI*/
    if(ssp_err != SSP_SUCCESS)
    {
        ssp_err = g_dac.p_api->close(g_dac.p_ctrl);
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Closing DA0 opened by audio thread\r\n",
                                  TX_WAIT_FOREVER);
        ssp_error_code_to_string(ssp_err);

        ssp_err = g_dac.p_api->open(g_dac.p_ctrl,g_dac.p_cfg);
        g_sf_console.p_api->write(g_sf_console.p_ctrl,
                                  (uint8_t*)"Open DA0 for DAC HAL\r\n",
                                  TX_WAIT_FOREVER);
        ssp_error_code_to_string(ssp_err);

    }
    /*we need to initialize timer for Dac*/
    ssp_err = g_timer_dac.p_api->open(g_timer_dac.p_ctrl,g_timer_dac.p_cfg);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,
                              (uint8_t*)"Open Timer For DAC\r\n",
                              TX_WAIT_FOREVER);
    ssp_error_code_to_string(ssp_err);
    /*Initializing default values of wave form*/
    samples = &triangle_samples[0];
    number_of_samples = NUM_OF_TRIANGLE_SAMPLES;

}

/*******************************************************************************************************************//**
 * @brief  Closes the Dac conversion and channel used
 * @pre    open should be called before use the dac hal module
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void dac_hal_close(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    bool audio = 0;

    if(audio)
    {
        ssp_err = g_dac.p_api->close(g_dac.p_ctrl);
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Close DAC\r\n",TX_WAIT_FOREVER);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Leaving DAC open since Audio Needs DA0"
                                  "\r\n",TX_WAIT_FOREVER);
    }
    /*we need to close timer for Dac*/
    ssp_err = g_timer_dac.p_api->close(g_timer_dac.p_ctrl);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Close Timer\r\n",TX_WAIT_FOREVER);
    ssp_error_code_to_string(ssp_err);
}

/*******************************************************************************************************************//**
 * @brief  Write the digital value to be converted to analog.
 * @pre    open should be called before use the dac hal module
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void dac_hal_write(sf_console_cb_args_t * p_args)
{

    uint32_t wave_type = 0;

    if(!get_arg_as_uint("WAVE", p_args->p_remaining_string, (uint32_t*)&wave_type,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(wave_type == 0)
    {
        samples = &triangle_samples[0];
        number_of_samples = NUM_OF_TRIANGLE_SAMPLES;
    }
    else if(wave_type == 1)
    {
        samples = &sine_samples[0];
        number_of_samples = NUM_OF_SINE_SAMPLES;
    }
    else
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t const *)"Incorrect Parameter\r\n",
                                  TX_WAIT_FOREVER);
    }
}
/*******************************************************************************************************************//**
 * @brief  Start the DAC
 * @pre    open should be called before use the dac hal module
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void dac_hal_start(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    /*we need to start the Dac*/
    ssp_err = g_dac.p_api->start(g_dac.p_ctrl);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Start DAC\r\n",TX_WAIT_FOREVER);
    ssp_error_code_to_string(ssp_err);
    /* We need to start timer for sampling*/
    ssp_err = g_timer_dac.p_api->start(g_timer_dac.p_ctrl);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Start Timer\r\n",TX_WAIT_FOREVER);
    ssp_error_code_to_string(ssp_err);

}


/*******************************************************************************************************************//**
 * @brief  Stop the DAC
 * @pre    open should be called before use the dac hal module
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void dac_hal_stop(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ssp_err = g_dac.p_api->stop(g_dac.p_ctrl);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Stop DAC\r\n",TX_WAIT_FOREVER);
    ssp_error_code_to_string(ssp_err);
    /*We also need to stop the timer*/
    ssp_err = g_timer_dac.p_api->stop(g_timer_dac.p_ctrl);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t*)"Stop Timer\r\n",TX_WAIT_FOREVER);
    ssp_error_code_to_string(ssp_err);

}

/*******************************************************************************************************************//**
 * @brief  Version of Dac HAL
 * @pre
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void dac_hal_versionGet(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    char   version[32];

    ssp_version_t version_details;
    /** calls SCI_I2C_VERSIONGET API */
    ssp_err_t ssp_err = g_dac.p_api->versionGet(&version_details);
    ssp_error_code_to_string(ssp_err);

    sprintf (version,"Code version  =  %d.%d\r\n", version_details.code_version_major,
            version_details.code_version_minor);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)version, TX_WAIT_FOREVER);

    sprintf (version,"\r\nAPI version  =  %d.%d\r\n", version_details.api_version_major,
            version_details.api_version_minor);
    g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)version, TX_WAIT_FOREVER);

}

/*******************************************************************************************************************//**
 * @brief  Timer call back for sampling Frequency(GPT2)
 * @pre    Configured as 8Khz
 *
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 ***********************************************************************************************************************/
void timer_callback_dac(timer_callback_args_t * p_args)
{
    UNUSED(p_args);
    int i;
    for(i=0;i <number_of_samples;i++)
    {
        g_dac.p_api->write(g_dac.p_ctrl,samples[i]);
    }
}

