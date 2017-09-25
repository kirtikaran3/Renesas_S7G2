/***********************************************************************************************************************
* File Name    : blinky_thread_entry.c
* Description  : This is a very simple example application that blinks all the LEDs on a board.
***********************************************************************************************************************/

#include "blinky_thread.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define OUTPUT_BUFFER_SIZE 1024
#define INPUT_BUFFER_SIZE 1024

char outputBuffer[OUTPUT_BUFFER_SIZE];
char inputBuffer[INPUT_BUFFER_SIZE];

volatile int inputIndex;

volatile bool tranComp;
volatile bool rxcComp;

int _printf(const char *format,...);

int _printf(const char *format,...){

    char buffer[OUTPUT_BUFFER_SIZE];
    va_list arg;
    int done=0;
    va_start(arg,format);

    done =  vsnprintf(buffer,OUTPUT_BUFFER_SIZE,format,arg);

    tranComp = false;
    g_uart.p_api->write(g_uart.p_ctrl,buffer,sizeof(buffer));

    while(!tranComp);

    va_end(arg);

    return done;
}

void user_uart_callback(uart_callback_args_t * p_args){

    switch(p_args->event){

        case UART_EVENT_TX_COMPLETE:

            tranComp = true;
            break;
        case UART_EVENT_RX_CHAR:

            inputBuffer[inputIndex] = p_args->data;
            if(inputBuffer[inputIndex] =='\n' || inputBuffer[inputIndex]=='\r'){

                inputBuffer[inputIndex]=NULL;
                rxcComp=true;

            }
            inputIndex++;


           if(inputIndex >= INPUT_BUFFER_SIZE){

               rxcComp = true;
               inputIndex = 0;

           }
          break;

        default:
           break;
    }



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
    /* Define the units to be used with the threadx sleep function */
	const uint32_t threadx_tick_rate_Hz = 100;
    /* Set the blink frequency (must be <= threadx_tick_rate_Hz */
    const uint32_t freq_in_hz = 2;
    /* Calculate the delay in terms of the threadx tick rate */
    const uint32_t delay = threadx_tick_rate_Hz/freq_in_hz;
    /* LED type structure */
    bsp_leds_t leds;
    /* LED state variable */
    ioport_level_t level = IOPORT_LEVEL_HIGH;

    /* Get LED information for this board */
    R_BSP_LedsGet(&leds);

    inputIndex = 0;
    g_uart.p_api->open(g_uart.p_ctrl,g_uart.p_cfg);

    _printf("\033[2J");
    _printf("\033[H");
    _printf("\033[3J");

    _printf("Hello World\n");



    /* If this board has no leds then trap here */
    if (0 == leds.led_count)
    {
        while(1);   // There are no leds on this board
    }

    while (1)
    {
        /* Determine the next state of the LEDs */
        if(IOPORT_LEVEL_LOW == level)
        {
            level = IOPORT_LEVEL_HIGH;
        }
        else
        {
            level = IOPORT_LEVEL_LOW;
        }

        /* Update all board LEDs */
        for(uint32_t i = 0; i < leds.led_count; i++)
        {
            g_ioport.p_api->pinWrite(leds.p_leds[i], level);
        }

        /* Delay */

         _printf("\r\n Input:");
         rxcComp = false;
         g_uart.p_api->read(g_uart.p_ctrl,NULL,NULL);
         while(!rxcComp);
         inputIndex = 0;
         _printf("%s",inputBuffer);

        tx_thread_sleep (delay);
    }
}
