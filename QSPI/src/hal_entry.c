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
char outputBuffer[UART_BUFFER_SIZE];

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
        g_uart.p_api->write (g_uart.p_ctrl, (uint8_t const *) (buffer + i), 1);
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
    uint8_t *deviceAddress = (uint8_t *) 0x60000000;

    bool isWriting;
    uint8_t writeData[] = "The quick brown fox jumps over the lazy dog.";
    uint8_t readData[sizeof(writeData)];
    uint8_t initialData[1024];

    // Open UART
    g_uart.p_api->open (g_uart.p_ctrl, g_uart.p_cfg);

    // Disable Output Buffering
    setvbuf ( stdout, NULL, _IONBF, UART_BUFFER_SIZE);

    // Open QSPI
    //g_qspi0.p_api->open (g_qspi0.p_ctrl, g_qspi0.p_cfg);
    g_qspi0.p_api->open(g_qspi0.p_ctrl,g_qspi0.p_cfg);

    // Use TTY100 commands to clear screen and reset screen pointer
    printf ("\033[2J"); // Clear Screen
    printf ("\033[H"); // Return Home
    printf ("\033[3J"); // Clear Back Buffer

    // Print Header
    printf ("Lesson 010: QSPI Flash\r\n\r\n");

    // Read Initial Flash Data
    g_qspi0.p_api->read (g_qspi0.p_ctrl, deviceAddress, (uint8_t *) initialData, sizeof(initialData));
    printf ("Initial Flash Data: '%s'\r\n", initialData);

    // Erasing Flash
    printf ("Erasing Flash\r\n");
    g_qspi0.p_api->sectorErase (g_qspi0.p_ctrl, deviceAddress);

    // Wait for flash to be idle (not writing)
    printf ("Waiting for Flash to be idle...");
    while (true)
    {
        g_qspi0.p_api->statusGet (g_qspi0.p_ctrl, &isWriting);
        if (isWriting)
            continue;
        break;
    }
    printf ("OK!\r\n\r\n");

    // Write data to flash
    printf ("Writing '%s' to Flash.\r\n\r\n", writeData);
    g_qspi0.p_api->pageProgram (g_qspi0.p_ctrl, deviceAddress, writeData, sizeof(writeData));

    // Wait for flash to be idle (not writing)
    printf ("Waiting for Flash to be idle...");
    while (true)
    {
        g_qspi0.p_api->statusGet (g_qspi0.p_ctrl, &isWriting);
        if (isWriting)
            continue;
        break;
    }
    printf ("OK!\r\n\r\n");

    // Read Flash Data
    g_qspi0.p_api->read (g_qspi0.p_ctrl, deviceAddress, (uint8_t *) readData, sizeof(readData));
    printf ("Flash Data: '%s'\r\n", readData);

    // Close Flash
    g_qspi0.p_api->close (g_qspi0.p_ctrl);

    // Endless Loop
    while (true)
    {
    }

}
