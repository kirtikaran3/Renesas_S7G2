#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <string.h>

// Buffer Size
#define UART_BUFFER_SIZE 1024

ssp_err_t UartInitialize(void);
ssp_err_t UartOpen(void);
ssp_err_t UartClose(void);

#endif /* UART_H_ */
