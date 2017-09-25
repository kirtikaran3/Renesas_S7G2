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



/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 * Only references two other modules including the BSP, IOPORT.
 *
 **********************************************************************************************************************/
/* HAL-only entry function */

#include "hal_data.h"
#include <stdio.h>
#include <string.h>

// Buffer Size
#define UART_BUFFER_SIZE 8

// Buffers
char outputBuffer[UART_BUFFER_SIZE];

// Flags
volatile bool transmitComplete=false;
volatile bool transmitCompleteS=false;



int _write(int file, uint8_t *buffer, int count);
int _write(int file, uint8_t *buffer, int count){

int bytesTransmitted = 0;

for (int i = 0; i < count; i++){
// Start Transmission
    transmitComplete = false;
    g_uart0.p_api->write (g_uart0.p_ctrl, (uint8_t const *) (buffer + i), 1);

        while (!transmitComplete){
        }

       bytesTransmitted++;

    }

return bytesTransmitted;

}

// Callback Function for UART interrupts
void user_uart_callback(uart_callback_args_t * p_args){
// Get Event Type
switch (p_args->event){
// Transmission Complete
    case UART_EVENT_TX_COMPLETE:
            transmitComplete = true;
            break;
    default:
            break;
    }
}
void user_spi_callback(spi_callback_args_t * p_args){
// Get Event Type
switch (p_args->event){
// Transmission Complete
    case SPI_EVENT_TRANSFER_COMPLETE:
        transmitCompleteS = true;
        break;

     default:
         break;
    }
}

void hal_entry(void){
uint8_t deviceAddress =  0x00000076;

uint8_t initialMydata[8];
memset(initialMydata,'\0',sizeof(initialMydata));

ssp_err_t error;

// Open UART
g_uart0.p_api->open (g_uart0.p_ctrl, g_uart0.p_cfg);

// Disable Output Buffering
//setvbuf ( stdout, NULL, _IONBF, UART_BUFFER_SIZE);

// Open SPI

g_spi0.p_api->open(g_spi0.p_ctrl,g_spi0.p_cfg);

// Use TTY100 commands to clear screen and reset screen pointer
printf ("\033[2J"); // Clear Screen
printf ("\033[H"); // Return Home
printf ("\033[3J"); // Clear Back Buffer

// Print Header
printf ("SPI Flash\r\n\r\n");

// Read Initial Flash Data
/*error = g_spi0.p_api->write (g_spi0.p_ctrl, &deviceAddress, 1, SPI_BIT_WIDTH_8_BITS);

    if(error != SSP_SUCCESS){

            printf("Failed to Write\n");
    }

    else{
            printf("Writing successful\n");
            printf ("Initial Flash Data: '%x'\r\n", initialMydata[0]);

        }
while(transmitCompleteS == false );
*/
//    for(int nIndex=0;nIndex<8;nIndex++)
//    {
//    error = g_spi0.p_api->read(g_spi0.p_ctrl,&initialMydata[nIndex],1,SPI_BIT_WIDTH_8_BITS);
error = g_spi0.p_api->read(g_spi0.p_ctrl,initialMydata,8,SPI_BIT_WIDTH_8_BITS);

        if(error != SSP_SUCCESS){

            printf("error is %d",error);
            printf("Failed to Read\n");
        }
        sleep(100);
        /*else{
                printf("Reading successful\n");
                printf ("Initial Flash Data: '%x'\r\n", initialMydata[0]);
                printf ("Initial Flash Data: '%x'\r\n", initialMydata[1]);
                printf ("Initial Flash Data: '%x'\r\n", initialMydata[2]);
                printf ("Initial Flash Data: '%x'\r\n", initialMydata[3]);
                               printf ("Initial Flash Data: '%x'\r\n", initialMydata[4]);
                               printf ("Initial Flash Data: '%x'\r\n", initialMydata[5]);
                               printf ("Initial Flash Data: '%x'\r\n", initialMydata[6]);
                                              printf ("Initial Flash Data: '%x'\r\n", initialMydata[7]);
                                              //printf ("Initial Flash Data: '%x'\r\n", initialMydata[2]);
        }*/
//    }
// Close Flash
g_spi0.p_api->close (g_spi0.p_ctrl);



}

