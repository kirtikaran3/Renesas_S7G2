/* HAL-only entry function */

#include "hal_data.h"
#include <stdio.h>
#include <string.h>

// Buffer Size
#define UART_BUFFER_SIZE 8

// Buffers
char outputBuffer[UART_BUFFER_SIZE];

// Flags
volatile bool transmitComplete;



int _write(int file, char *buffer, int count);
int _write(int file, char *buffer, int count)
{

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
void user_spi_callback(spi_callback_args_t * p_args)
{
// Get Event Type
switch (p_args->event)
{
// Transmission Complete
case SPI_EVENT_TRANSFER_COMPLETE:
transmitComplete = true;
break;
default:
break;


}
}

void hal_entry(void)
{
uint8_t deviceAddress =  0x76;

uint8_t initialData[1024];

ssp_err_t error;

// Open UART
g_uart0.p_api->open (g_uart0.p_ctrl, g_uart0.p_cfg);

// Disable Output Buffering
setvbuf ( stdout, NULL, _IONBF, UART_BUFFER_SIZE);

// Open SPI

g_spi0.p_api->open(g_spi0.p_ctrl,g_spi0.p_cfg);

// Use TTY100 commands to clear screen and reset screen pointer
printf ("\033[2J"); // Clear Screen
printf ("\033[H"); // Return Home
printf ("\033[3J"); // Clear Back Buffer

// Print Header
printf ("SPI Flash\r\n\r\n");

// Read Initial Flash Data
g_spi0.p_api->write (g_spi0.p_ctrl, &deviceAddress, 1, SPI_BIT_WIDTH_8_BITS);

if(error != SSP_SUCCESS){

printf("Failed to Write\n");
}
else{
printf("Writing successful\n");
printf ("Initial Flash Data: '%d'\r\n", initialData);

}

error = g_spi0.p_api->read(g_spi0.p_ctrl,initialData,8,SPI_BIT_WIDTH_8_BITS);

if(error != SSP_SUCCESS){
    printf("error is %d",error);

printf("Failed to Read\n");
}
else{
printf("Reading successful\n");
printf ("Initial Flash Data: '%d'\r\n", initialData);

}

// Close Flash
g_spi0.p_api->close (g_spi0.p_ctrl);

// Endless Loop
while (true)
{
}

}
