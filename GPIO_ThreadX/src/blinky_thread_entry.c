/***********************************************************************************************************************
* File Name    : blinky_thread_entry.c
* Description  : This is a very simple example application that blinks all the LEDs on a board.
***********************************************************************************************************************/

#include "blinky_thread.h"
#include <stdbool.h>

#define BUTTON_PRESSED IOPORT_LEVEL_LOW
#define LED_ON 1
#define LED_OFF 0

void g_external_irq11_callback(external_irq_callback_args_t * p_args)
{
    ioport_level_t p_pin_value;
    g_ioport.p_api->pinRead(IOPORT_PORT_00_PIN_06, &p_pin_value);
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, p_pin_value == BUTTON_PRESSED ? LED_ON : LED_OFF);
}

void g_external_irq10_callback(external_irq_callback_args_t * p_args)
{
    ioport_level_t p_pin_value;
    g_ioport.p_api->pinRead(IOPORT_PORT_00_PIN_05, &p_pin_value);
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, p_pin_value == BUTTON_PRESSED ? LED_ON : LED_OFF);
}




/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the the threadx sleep function.
 * Only references two other modules including the BSP, IOPORT.
 *
 **********************************************************************************************************************/
void blinky_thread_entry(void)
{


    g_external_irq.p_api->open(g_external_irq.p_ctrl, g_external_irq.p_cfg);
    g_external_irq_11.p_api->open(g_external_irq_11.p_ctrl, g_external_irq_11.p_cfg);

       // Set LEDs to their initial state
       g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, LED_OFF);
       g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, LED_OFF);
       g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, LED_OFF);

       while(true){}
    /* If this board has no leds then trap here */
    }
