
#include "hal_data.h"
#include <stdio.h>
#include <string.h>

// Buffer Size
#define UART_BUFFER_SIZE 8

// Buffers
uint8_t outputBuffer[UART_BUFFER_SIZE];

// Flags
volatile bool transmitComplete;
volatile bool receiveComplete;

int _write(int file, uint8_t *buffer, int count);
int _read(int file, uint8_t *buffer, int count);

int _write(int file, uint8_t *buffer, int count){
    ssp_err_t error;
    SSP_PARAMETER_NOT_USED(file);
    SSP_PARAMETER_NOT_USED(count);

    // Start Transmission
    transmitComplete = false;
    error = g_uart0.p_api->write (g_uart0.p_ctrl, (uint8_t const *) (buffer), 1);

    // Return 0 if there was an error
    if (error != SSP_SUCCESS) {
        return 0;
    }

    // Wait for interrupt to trigger signaling
    // a character has been received
    while (!transmitComplete) {
    }

    // Return the character count (1)
    return 1;
}

// Read a single byte from the UART device
int _read(int file, uint8_t *buffer, int count){
    ssp_err_t error;

    SSP_PARAMETER_NOT_USED(file);
    SSP_PARAMETER_NOT_USED(count);

    // Start Reception
    receiveComplete = false;
    error = g_uart0.p_api->read (g_uart0.p_ctrl, (uint8_t const *) (buffer), 1);

    if (error != SSP_SUCCESS){
        return 0;
    }

    while (!receiveComplete) {
    }

    return 1;
}
// Callback Function for UART interrupts
void UartCallback(uart_callback_args_t * p_args){
    // Get Event Type
    switch (p_args->event) {
        // Transmission Complete
        case UART_EVENT_TX_COMPLETE:
            transmitComplete = true;
        break;
        default:
        break;
    }
}

void hal_entry(void){

    uint8_t deviceAddress =  0x77;

    ssp_err_t error;

    uint8_t initialData[8]="00000000";

    // Open UART
    g_uart0.p_api->open (g_uart0.p_ctrl, g_uart0.p_cfg);

    // Disable Output Buffering
    setvbuf ( stdout, NULL, _IONBF, UART_BUFFER_SIZE);


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
    else {

        printf("Reading successful\n");
        printf ("Initial Data: '%d'\r\n", initialData);
    }

        // Close Flash
    g_spi0.p_api->close (g_spi0.p_ctrl);

}
