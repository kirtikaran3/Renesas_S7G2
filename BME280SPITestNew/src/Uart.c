#include "hal_data.h"

#include <stdbool.h>
#include "Uart.h"

// Buffers
char uartBuffer[UART_BUFFER_SIZE];

// Flags
volatile bool transmitComplete;
volatile bool receiveComplete;

int _write(int file, char *buffer, int count);
int _read(int file, char *buffer, int count);

int _write(int file, char *buffer, int count)
{
    ssp_err_t error;
    SSP_PARAMETER_NOT_USED(file);
    SSP_PARAMETER_NOT_USED(count);

    // Start Transmission
    transmitComplete = false;
    error = g_uart.p_api->write (g_uart.p_ctrl, (uint8_t const *) (buffer), 1);

    // Return 0 if there was an error
    if (error != SSP_SUCCESS)
    {
        return 0;
    }

    // Wait for interrupt to trigger signaling
    // a character has been received
    while (!transmitComplete)
    {
    }

    // Return the character count (1)
    return 1;
}

// Read a single byte from the UART device
int _read(int file, char *buffer, int count)
{
    ssp_err_t error;

    SSP_PARAMETER_NOT_USED(file);
    SSP_PARAMETER_NOT_USED(count);

    // Start Reception
    receiveComplete = false;
    error = g_uart.p_api->read (g_uart.p_ctrl, (uint8_t const *) (buffer), 1);

    if (error != SSP_SUCCESS)
    {
        return 0;
    }

    while (!receiveComplete)
    {
    }

    return 1;
}

ssp_err_t UartInitialize()
{
    return SSP_SUCCESS;
}

ssp_err_t UartOpen()
{
    ssp_err_t error;

    // Open UART
    error = g_uart.p_api->open (g_uart.p_ctrl, g_uart.p_cfg);
    if (error != SSP_SUCCESS)
        return error;

    // Disable Output Buffering
    setvbuf ( stdout, NULL, _IONBF, UART_BUFFER_SIZE);

    return SSP_SUCCESS;
}

ssp_err_t UartClose()
{
    ssp_err_t error;

    // Close UART
    error = g_uart.p_api->close (g_uart.p_ctrl);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}

// Callback Function for UART interrupts
void UartCallback(uart_callback_args_t * p_args)
{
    // Get Event Type
    switch (p_args->event)
    {
        // Transmission Complete
        case UART_EVENT_TX_COMPLETE:
            transmitComplete = true;
        break;
        case UART_EVENT_RX_CHAR:
        case UART_EVENT_RX_COMPLETE:
            receiveComplete = true;
        break;
        default:
        break;
    }
}

