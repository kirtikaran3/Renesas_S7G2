/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 *
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : hal_entry.c
* Description  : This is a very simple example application that blinks all LEDs on a board.
***********************************************************************************************************************/
/* HAL-only entry function */

#include "hal_data.h"
#include <stdio.h>
#include <string.h>

// Buffer Size
#define UART_BUFFER_SIZE 1024

// Buffers
uint8_t outputBuffer[UART_BUFFER_SIZE];

// Flags
volatile bool transmitComplete;

int _write(int file, char *buffer, int count);

int _write(int file, char *buffer, int count)
{
    // As far as I know, there isn't a way to retrieve how many
    // bytes were send on using the uart->write function if it does not return
    // SSP_SUCCESS (unless we want to use the tx interrupt function and a global counter
    // so, we will send each character one by one instead.
    int bytesTransmitted = 0;

    for (int i = 0; i < count; i++)
    {
        // Start Transmission
        transmitComplete = false;
        g_uart0.p_api->write (g_uart0.p_ctrl, (uint8_t const *) (buffer + i), 1);
        while (!transmitComplete)
        {
        }

        bytesTransmitted++;
    }

    return bytesTransmitted;
}
// Callback Function for UART interrupts
void user_uart_callback(uart_callback_args_t * p_args)
{
    // Get Event Type
    switch (p_args->event)
    {
        // Transmission Complete
        case UART_EVENT_TX_COMPLETE:
            transmitComplete = true;
        break;
        default:
        break;
    }
}

void hal_entry(void)
{
    uint8_t deviceAddress =  0x76;

    ssp_err_t error;

    uint8_t initialData[1024];


    // Open UART
    g_uart0.p_api->open (g_uart0.p_ctrl, g_uart0.p_cfg);

    // Disable Output Buffering
    setvbuf ( stdout, NULL, _IONBF, UART_BUFFER_SIZE);

    // Open QSPI
    //g_qspi0.p_api->open (g_qspi0.p_ctrl, g_qspi0.p_cfg);

    // Use TTY100 commands to clear screen and reset screen pointer
    printf ("\033[2J"); // Clear Screen
    printf ("\033[H"); // Return Home
    printf ("\033[3J"); // Clear Back Buffer

    printf("In Begin %d",initialData);
    // Print Header
    printf ("SPI Mode\r\n\r\n");

    printf("Opening SPI\n");
     error=g_spi0.p_api->open(g_spi0.p_ctrl,g_spi0.p_cfg);

     if(error != SSP_SUCCESS){

         printf("Failed to open\n");

     }
     else{
         printf("SPI open\n");
         printf("Reading SPI\n");
     }


     g_spi0.p_api->write (g_spi0.p_ctrl, &deviceAddress,1,SPI_BIT_WIDTH_8_BITS);

        if(error != SSP_SUCCESS){

               printf("Failed to write\n");

           }
        else{

            printf("writing successful\n");
            printf ("Initial Data: '%d'\r\n", initialData);
        }

    // Read Initial Flash Data
    g_spi0.p_api->read (g_spi0.p_ctrl, initialData, sizeof(initialData),SPI_BIT_WIDTH_8_BITS);

    if(error != SSP_SUCCESS){

           printf("Failed to Read\n");

       }
    else{

        printf("Reading successful\n");
        printf ("Initial Data: '%d'\r\n", initialData);
    }

        // Close Flash
    g_spi0.p_api->close (g_spi0.p_ctrl);

    // Endless Loop
    while (true)
    {
    }

}
