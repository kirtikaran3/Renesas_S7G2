#include "hal_data.h"

#include <stdbool.h>
#include "I2C.h"

ssp_err_t I2CInitialize(void)
{
    return SSP_SUCCESS;
}

ssp_err_t I2COpen(void)
{
    ssp_err_t error;

    // Open I2C
    error = g_spi0.p_api->open (g_spi0.p_ctrl, g_spi0.p_cfg);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}

ssp_err_t I2CClose(void)
{
    ssp_err_t error;

    // Close I2C
    error = g_spi0.p_api->close (g_spi0.p_ctrl);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}

ssp_err_t I2CWrite(uint8_t slaveAddress, uint8_t * const data, uint32_t const length, bool const restart)
{
    // Set the slave address to the device we want to talk to.
    //g_spi0.p_ctrl->current_slave = slaveAddress;


    // Write data to the slave
    return g_spi0.p_api->write (g_spi0.p_ctrl, slaveAddress, 1, SPI_BIT_WIDTH_8_BITS);
}

ssp_err_t I2CWriteByte(uint8_t slaveAddress, uint8_t const data, bool const restart)
{
    // Write single byte to slave
    return I2CWrite (slaveAddress, (uint8_t * const ) &data, 1, restart);
}

ssp_err_t I2CRead(uint8_t slaveAddress, uint8_t * const data, uint32_t const length, bool const restart)
{
    // Set the slave address to the device we want to talk to.
   // g_spi0.p_ctrl->current_slave = slaveAddress;

    // Read data from the slave
    return g_spi0.p_api->read (g_spi0.p_ctrl, data, sizeof(data), SPI_BIT_WIDTH_8_BITS);
}

ssp_err_t I2CReadByte(uint8_t slaveAddress, uint8_t * const data, bool const restart)
{
    // Read single byte from slave
    return I2CRead (slaveAddress, data, 1, restart);
}

