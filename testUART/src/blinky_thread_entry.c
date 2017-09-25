/***********************************************************************************************************************
* File Name    : blinky_thread_entry.c
* Description  : This is a very simple example application that blinks all the LEDs on a board.
***********************************************************************************************************************/

#include "blinky_thread.h"

int puts(const char *str);

int puts(const char *str){

    g_uart.p_api->write(g_uart.p_ctrl,(const uint8_t *)str,strlen(str));

    return 0;
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

    uint8_t msg[]= "Hello World\r\n";

    ssp_err_t openResult;
    ssp_err_t writeResult;

    uint8_t data[] = "Kirti";
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

        openResult =  g_uart.p_api->open(g_uart.p_ctrl,g_uart.p_cfg);
        //puts("Kirti");

        puts(&data[0]);
        writeResult =  g_uart.p_api->write(g_uart.p_ctrl,msg,sizeof(msg));

        tx_thread_sleep (delay);
    }
}
