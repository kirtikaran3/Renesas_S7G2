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
 * File Name    :   r_ioport_commands.c
 * Description  :   IOPorts hal commands implementation. Here subset of IOPort commands are shown how to implement
 *
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

#include "dev_example_thread.h"
#include "dev_example_common.h"

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/

void ioport_hal_pinread(sf_console_cb_args_t * p_args);
void ioport_hal_pinwrite(sf_console_cb_args_t * p_args);
void ioport_hal_pindirectionset(sf_console_cb_args_t * p_args);
void ioport_hal_portread(sf_console_cb_args_t * p_args);
void ioport_hal_pincfg(sf_console_cb_args_t * p_args);
void ioport_hal_pineventinputread(sf_console_cb_args_t * p_args);
void ioport_hal_pineventoutputwrite(sf_console_cb_args_t * p_args);
void ioport_hal_portdirectionset(sf_console_cb_args_t * p_args);
void ioport_hal_porteventinputread(sf_console_cb_args_t * p_args);
void ioport_hal_porteventoutputwrite(sf_console_cb_args_t * p_args);
void ioport_hal_portwrite(sf_console_cb_args_t * p_args);
void ioport_hal_ethernetmodeconfig(sf_console_cb_args_t * p_args);
void ioport_hal_versionget(sf_console_cb_args_t * p_args);


/***********************************************************************************************************************
Typedef definitions
 ***********************************************************************************************************************/
#define GREEN_LED_PIN_7     IOPORT_PORT_08_PIN_07
#define RED_LED_PIN_8       IOPORT_PORT_08_PIN_08
#define GREEN_LED_PIN_9     IOPORT_PORT_08_PIN_09
#define RED_LED_PIN_10      IOPORT_PORT_08_PIN_10
#define LED1                0
#define LED2                1
#define LED3                2
#define LED4                3


/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/


const uint32_t port_no[] =
{
 IOPORT_PORT_00,
 IOPORT_PORT_01,
 IOPORT_PORT_02,
 IOPORT_PORT_03,
 IOPORT_PORT_04,
 IOPORT_PORT_05,
 IOPORT_PORT_06,
 IOPORT_PORT_07,
 IOPORT_PORT_08,
 IOPORT_PORT_09,
 IOPORT_PORT_10,
 IOPORT_PORT_11

};
uint16_t pin_no[] =
{
 GREEN_LED_PIN_7,
 RED_LED_PIN_8,
 GREEN_LED_PIN_9,
 RED_LED_PIN_10
};


/** IOPORT HAL Module console menu commands */
const sf_console_command_t g_ioport_hal_commands[] =

{

 {
  (uint8_t*)"pinRead",
  (uint8_t*)"Reads value of pin Input: port(0-11) pin(0-15)"
  "\r\n\tSynopsis:- pinRead port# pin#"
  "\r\n\tExample:- pinRead port8 pin7",
  ioport_hal_pinread,
  NULL
 },

 {
  (uint8_t*)"pinWrite",
  (uint8_t*)"Writes to pin Input: pin(0-3) level(0-1)"
  "\r\n\tSynopsis:- pinWrite pin# level#"
  "\r\n\tExample:- pinWrite pin1 level1",
  ioport_hal_pinwrite,
  NULL
 },

 {
  (uint8_t*)"pinDirectionSet",
  (uint8_t*)"Sets direction of pin Input: pin(0-3) dir(0-1)"
  "\r\n\tSynopsis:- pinDirectionSet pin# dir#"
  "\r\n\tExample:- pinDirectionSet pin1 dir0",
  ioport_hal_pindirectionset,
  NULL
 },

 {
  (uint8_t*)"portRead",
  (uint8_t*)"Reads from a port Input: port(0-11)"
  "\r\n\tSynopsis:- portRead port#"
  "\r\n\tExample:- portRead port2",
  ioport_hal_portread,
  NULL
 },

 {
  (uint8_t*)"pinCfg",
  (uint8_t*)"Configure the pin Input: pin(0-3)"
  "\r\n\tSynopsis:- pinCfg pin# cfg#"
  "\r\n\tExample:- pinCfg pin1 cfg4",
  ioport_hal_pincfg,
  NULL
 },

 {
  (uint8_t*)"pinEventInputRead",
  (uint8_t*)"Read the input data of pin Input: pin(0-3)"
  "\r\n\tSynopsis:- pinEventInputRead pin#"
  "\r\n\tExample:- pinEventInputRead pin1",
  ioport_hal_pineventinputread,
  NULL
 },

 {
  (uint8_t*)"pinEventOutputWrite",
  (uint8_t*)"Write pin event Input: pin(0-3) val(0-1)"
  "\r\n\tSynopsis:- pinEventOutputWrite pin# val#"
  "\r\n\tExample:- pinEventOutputWrite pin1 val0",
  ioport_hal_pineventoutputwrite,
  NULL
 },

 {
  (uint8_t*)"portDirectionSet",
  (uint8_t*)"Sets direction of port Input: port(8) dir(0-1)"
  "\r\n\tSynopsis:- portDirectionSet port# dir#"
  "\r\n\tExample:- portDirectionSet port8 dir1",
  ioport_hal_portdirectionset,
  NULL
 },

 {
  (uint8_t*)"portEventInputRead",
  (uint8_t*)"Read the input data of port Input: port(0-11)"
  "\r\n\tSynopsis:- portEventInputRead port#"
  "\r\n\tExample:- portEventInputRead port1",
  ioport_hal_porteventinputread,
  NULL
 },

 {
  (uint8_t*)"portEventOutputWrite",
  (uint8_t*)"Write port event Input: port(8)"
  "\r\n\tSynopsis:- portEventOutputWrite port# data#"
  "\r\n\tExample:- portEventOutputWrite port8 data1",
  ioport_hal_porteventoutputwrite,
  NULL
 },

 {
  (uint8_t*)"portWrite",
  (uint8_t*)"Writes to a port Input: port(8)"
  "\r\n\tSynopsis:- portWrite port# data#"
  "\r\n\tExample:- portWrite port8 data1",
  ioport_hal_portwrite,
  NULL
 },

 {
  (uint8_t*)"ethernetModeConfig",
  (uint8_t*)"Configure the PHY mode of the Ethernet channels Input:channel(0-1) mode(0-1)"
  "\r\n\tSynopsis:- ethernetModeConfig channel# mode#",
  ioport_hal_ethernetmodeconfig,
  NULL
 },

 {
  (uint8_t*)"versionGet",
  (uint8_t*)"Give the version of API"
  "\r\n\tExample:- versionGet",
  ioport_hal_versionget,
  NULL
 },

};


/** IOPORT HAL Module console menu **/

const sf_console_menu_t g_ioport_hal_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"r_ioport",
 .num_commands = (sizeof(g_ioport_hal_commands)) / (sizeof(g_ioport_hal_commands[0])),
 .command_list = g_ioport_hal_commands
};


/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Reads level on the pin.
 *
 * @example pinRead port# pin#
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_pinread(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ioport_port_pin_t pin;
    ioport_level_t level;
    int32_t input_pin, port;
    uint32_t i;
    uint32_t flag = 0;
    char buffer[32];
    /** Not defined pins in SSP are listed down and checked weather these pin selected to read */
    uint32_t invalid_pins[] = {
                               12,13,520,521,522,523,526,527,1806,1807,2062,2063,
                               2824,2825,2826,2827,2828,2829,2830,2831
    };

    if(!get_arg_as_uint("port", p_args->p_remaining_string, (uint32_t*)&port,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("pin", p_args->p_remaining_string, (uint32_t*)&input_pin,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    /** Validating the entered port and pin no */
    if(((port < 12) && (port >= 0)) && ((input_pin < 16) && (input_pin >= 0)))
    {
        /** Converting port and pin into given port and pin number */
        port = (int16_t)port_no[port];
        pin = port + input_pin;

        /** Checking wether the given pin is valid or not */
        for(i = 0 ; i < 20 ; i++)
        {
            if(pin == invalid_pins[i])
            {
                flag = 1;
                break;
            }
        }

        /** If input is valid then perform API else return error */
        if(flag == 0)
        {
            ssp_err = g_ioport.p_api->pinRead(pin,(&level));
            if(ssp_err == SSP_SUCCESS)
            {
                sprintf (buffer,"IOPORT_LEVEL_%s\r\n",((level == 0) ? "LOW" : "HIGH"));
                print_to_console(buffer);
            }
            else
            {
                ssp_error_code_to_string(ssp_err);
            }

        }
        else
        {
            print_to_console("Selected pin is not Defined\r\n");
        }
    }
    else
    {
        print_to_console("Invalid Arguments give valid port and pin\r\n");
    }
}


/*******************************************************************************************************************//**
 * @brief    Write's given level to pins.
 *           Since writing to few pins can change the configuration here we writes on four LED pins.
 *
 *  @example pinWrite pin# level#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_pinwrite(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ioport_level_t level;
    ioport_port_pin_t pin;
    int32_t input_pin;
    char buffer[32];

    if(!get_arg_as_uint("pin", p_args->p_remaining_string, (uint32_t*)&input_pin,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("level", p_args->p_remaining_string, (uint32_t*)&level,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** Validating the entered input */
    if(((input_pin < 4) && (input_pin >= 0)) && ((level == 1) || (level == 0)))
    {
        /** Assigning the input value to the LED pins on port 8 */
        pin = pin_no[input_pin];
        ssp_err = g_ioport.p_api->pinWrite(pin,level);
        if(ssp_err == SSP_SUCCESS)
        {
            sprintf (buffer,"New IOPORT_LEVEL_%s\r\n",((level == 0) ? "LOW" : "HIGH"));
            print_to_console(buffer);
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }
    }
    else
    {
        print_to_console("Invalid Arguments give valid pin_no and level\r\n");
    }

}


/*******************************************************************************************************************//**
 *  @brief   Set the direction for pin as input(0) or output(1)
 *
 *  @example pinDirecrionSet pin# dir#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_pindirectionset(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ioport_direction_t direction;
    int32_t input_pin;
    ioport_port_pin_t pin;
    char buffer[32];

    if(!get_arg_as_uint("pin", p_args->p_remaining_string, (uint32_t*)&input_pin,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    if(!get_arg_as_uint("dir", p_args->p_remaining_string, (uint32_t*)&direction,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** Validating the entered input */
    if(((input_pin < 4) && (input_pin >= 0)) && ((direction == 1) || (direction == 0)))
    {
        /** Assigning the input value to the LED pins on port 8 */
        pin = pin_no[input_pin];
        ssp_err = g_ioport.p_api->pinDirectionSet(pin,direction);
        if(ssp_err == SSP_SUCCESS)
        {
            sprintf (buffer,"New IOPORT_DIRECTION_%s\r\n",(direction == 0) ? "INPUT" : "OUTPUT");
            print_to_console(buffer);
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }

    }
    else
    {
        print_to_console("Invalid Arguments give valid pin_no and direction\r\n");
    }

}


/*******************************************************************************************************************//**
 *  @brief   Reads the port value and prints to console.
 *
 *  @example portRead port#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_portread(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ioport_port_t port;
    ioport_size_t size;
    int32_t temp;
    char buffer[32];

    if(!get_arg_as_uint("port", p_args->p_remaining_string, (uint32_t*)&temp,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    /** Validating the entered input  */
    if((temp < 12) && (temp >= 0))
    {
        port = (uint16_t )port_no[temp];
        ssp_err = g_ioport.p_api->portRead(port,&size);
        if(ssp_err == SSP_SUCCESS)
        {
            sprintf (buffer,"Port value =  0x%x\r\n",size);
            print_to_console(buffer);
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }
    }
    else
    {
        print_to_console("Invalid Arguments give valid Port no\r\n");
    }

}



/*******************************************************************************************************************//**
 *  @brief   Configures the pin which is given as input.
 *
 *  @example pinCfg pin# cfg#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_pincfg(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    int32_t input_pin;
    uint32_t cfg;
    ioport_port_pin_t pin;

    if(!get_arg_as_uint("pin", p_args->p_remaining_string, (uint32_t*)&input_pin,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("cfg", p_args->p_remaining_string, (uint32_t*)&cfg,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    /** Validating the entered input */
    if((input_pin < 4) && (input_pin >= 0) && (cfg < 0xFFFFFFFF))
    {
        pin = pin_no[input_pin];
        ssp_err = g_ioport.p_api->pinCfg(pin,cfg);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        print_to_console("Invalid Arguments give valid pin_no and configuration value\r\n");
    }

}


/*******************************************************************************************************************//**
 *  @brief   Read the event input data of the specified pin and return the level.
 *
 *  @example pinEventInputRead pin#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_pineventinputread(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ioport_port_pin_t pin;
    int32_t input_pin;
    ioport_level_t p_pin_event;
    char buffer[32];

    if(!get_arg_as_uint("pin", p_args->p_remaining_string, (uint32_t*)&input_pin,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    /** Validating the entered input */
    if((input_pin < 4) && (input_pin >= 0))
    {
        pin = pin_no[input_pin];
        ssp_err = g_ioport.p_api->pinEventInputRead(pin,&p_pin_event);
        if(ssp_err == SSP_SUCCESS)
        {
            sprintf (buffer,"IOPORT_LEVEL_%s\r\n",((p_pin_event == 0) ? "LOW" : "HIGH"));
            print_to_console(buffer);
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }
    }
    else
    {
        print_to_console("Invalid Arguments give valid pin_no\r\n");
    }
}

/*******************************************************************************************************************//**
 *  @brief   Write pin event Data
 *
 *  @example pinEventOutputWrite pin# val#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/


void ioport_hal_pineventoutputwrite(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    int32_t input_pin;
    ioport_level_t pin_value;
    ioport_port_pin_t pin;

    if(!get_arg_as_uint("pin", p_args->p_remaining_string, (uint32_t*)&input_pin,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("val", p_args->p_remaining_string, (uint32_t*)&pin_value,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    /** Validating the entered input */
    if(((input_pin < 4) && (input_pin >= 0)) && ((pin_value == 1) || (pin_value == 0)))
    {
        pin = pin_no[input_pin];
        ssp_err = g_ioport.p_api->pinEventOutputWrite(pin,pin_value);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        print_to_console("Invalid Arguments give valid pin_no and pin_value\r\n");
    }
}

/*******************************************************************************************************************//**
 *  @brief   Set the direction for one or more pins on a port
 *
 *  @example portDirectionSet port# dir#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_portdirectionset(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    int32_t temp;
    ioport_port_t port;
    ioport_size_t direction_values, mask_value;

    if(!get_arg_as_uint("port", p_args->p_remaining_string, (uint32_t*)&temp,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    port = port_no[temp];

    if(!get_arg_as_uint("dir", p_args->p_remaining_string, (uint32_t*)&direction_values,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** Validating the entered input */
    if((port == IOPORT_PORT_08) && ((direction_values == 1) || (direction_values == 0)))
    {
        mask_value = 0x0780;
        ssp_err = g_ioport.p_api->portDirectionSet(port,direction_values,mask_value);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        print_to_console("Invalid Arguments give valid Port and direction\r\n");
    }
}


/*******************************************************************************************************************//**
 *  @brief   Read captured event data on a port
 *
 *  @example portEventInputRead port#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_porteventinputread(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ioport_size_t p_event_data;
    int32_t temp;
    ioport_port_t port;
    char buffer[32];

    if(!get_arg_as_uint("port", p_args->p_remaining_string, (uint32_t*)&temp,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    /** Validating the entered input */
    if((temp < 12) && (temp >= 0))
    {
        port = port_no[temp];
        ssp_err = g_ioport.p_api->portEventInputRead(port,&p_event_data);
        if(ssp_err == SSP_SUCCESS)
        {
            sprintf (buffer,"Port event data = 0x%x\r\n",p_event_data);
            print_to_console(buffer);
        }
        else
        {
            ssp_error_code_to_string(ssp_err);
        }
    }
    else
    {
        print_to_console("Invalid Arguments give valid Port\r\n");
    }
}


/*******************************************************************************************************************//**
 *  @brief   Write event output data for port
 *
 *  @example portEventOutputWrite port# data#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_porteventoutputwrite(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ioport_size_t p_event_data, mask_value;
    int32_t temp,data;
    ioport_port_t port;

    if(!get_arg_as_uint("port", p_args->p_remaining_string, (uint32_t*)&temp,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    port = port_no[temp];

    if(!get_arg_as_uint("data", p_args->p_remaining_string, (uint32_t*)&data,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    /** Validating the entered input */
    if((port == IOPORT_PORT_08) && (data >= 0) && (data < 65536))
    {
        p_event_data = (uint16_t )data;
        mask_value = 0x0780;
        ssp_err = g_ioport.p_api->portEventOutputWrite(port,p_event_data,mask_value);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        print_to_console("Invalid Arguments give valid Port and data\r\n");
    }

}


/*******************************************************************************************************************//**
 *  @brief   Writes to multiple pins on port
 *
 *  @example portWrite port# data#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_portwrite(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ioport_size_t value, mask_value;
    int32_t temp,data;
    ioport_port_t port;

    if(!get_arg_as_uint("port", p_args->p_remaining_string, (uint32_t*)&temp,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    if(!get_arg_as_uint("data", p_args->p_remaining_string, (uint32_t*)&data,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }
    /** Validating the entered input */
    if((temp == 8) && (data >= 0) && (data < 65536))
    {
        port = port_no[temp];
        value = (uint16_t )data;
        mask_value = 0x0780;
        ssp_err = g_ioport.p_api->portWrite(port,value,mask_value);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        print_to_console("Invalid Arguments give valid Port and data\r\n");
    }

}


/*******************************************************************************************************************//**
 *  @brief   Configure the PHY mode of the Ethernet channels.
 *
 *  @example ethernetmodeconfig channel# mode#
 *  @param[in] p_args Structure of data providing the context in which the callback was triggered.
 *  @retval VOID
 ***********************************************************************************************************************/


void ioport_hal_ethernetmodeconfig(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    ioport_ethernet_channel_t channel;
    ioport_ethernet_mode_t mode;
    int32_t temp;
    bool invalid_arguments = false;

    if(!get_arg_as_uint("channel", p_args->p_remaining_string, (uint32_t*)&temp,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** Validating the entered input */
    if(temp == 0)
    {
        channel = IOPORT_ETHERNET_CHANNEL_0;
    }
    else if(temp == 1)
    {
        channel = IOPORT_ETHERNET_CHANNEL_1;
    }
    else
    {
        invalid_arguments = true;
    }


    if(!get_arg_as_uint("mode", p_args->p_remaining_string, (uint32_t*)&temp,DEC))
    {
        print_to_console("Invalid Arguments\r\n");
        return;
    }

    /** Validating the entered input */
    if(temp == 0)
    {
        mode = IOPORT_ETHERNET_MODE_MII;
    }
    else if(temp == 1)
    {
        mode = IOPORT_ETHERNET_MODE_RMII;
    }
    else
    {
        invalid_arguments = true;
    }
    if(!invalid_arguments)
    {
        ssp_err = g_ioport.p_api->pinEthernetModeCfg(channel,mode);
        ssp_error_code_to_string(ssp_err);
    }
    else
    {
        print_to_console("Invalid Arguments give valid channel and mode\r\n");
    }
}


/*******************************************************************************************************************//**
 * @brief Get the version of IOPort HAL API's
 *
 * @example versionGet
 * @param[in] p_args Structure of data providing the context in which the callback was triggered.
 * @retval VOID
 ***********************************************************************************************************************/

void ioport_hal_versionget(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    ssp_err_t ssp_err;
    char   version[32];
    ssp_version_t version_details;

    /** Get the version of IOPORT */
    ssp_err = g_ioport.p_api->versionGet(&version_details);
    if(ssp_err == SSP_SUCCESS)
    {
        sprintf (version,"Code version  =  %d.%d\r\n",version_details.code_version_major,
                 version_details.code_version_minor);
        print_to_console(version);
        sprintf (version,"\r\nAPI version  =  %d.%d\r\n", version_details.api_version_major,
                version_details.api_version_minor);
        print_to_console(version);
    }

    else
    {
        ssp_error_code_to_string(ssp_err);
    }

}



