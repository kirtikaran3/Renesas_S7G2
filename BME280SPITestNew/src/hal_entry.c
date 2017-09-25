/* HAL-only entry function */
#include "hal_data.h"
#include <stdio.h>

#include "Uart.h"
#include "I2C.h"
#include "Bme280.h"
#include "Timer.h"

#define USE_VT100

void hal_entry(void)
{
    ssp_err_t error;
    uint8_t const bme280Address = 0x77;
    uint8_t chipId;
    Bme280CalibrationFactors calibrationFactors;
    float temperature;
    float pressure;
    float humidity;

    /************************************
     * Init UART
     ************************************/
    error = UartInitialize ();
    if (error != SSP_SUCCESS)
    {
        while (true)
        {
        }
    }

    error = UartOpen ();
    if (error != SSP_SUCCESS)
    {
        while (true)
        {
        }
    }

    /************************************
     * Clear Console
     ************************************/
#ifdef USE_VT100
    {
        // Use TTY100 commands to clear screen and reset screen pointer
        printf ("\033[2J"); // Clear Screen
        printf ("\033[H"); // Return Home
        printf ("\033[3J"); // Clear Back Buffer
    }
#endif
    /************************************
     * Init Timer
     ************************************/
    printf ("Initializing Timer: ");
    error = TimerInitialize ();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    /************************************
     * Init I2C
     ************************************/
    printf ("Initializing I2C: ");
    error = I2CInitialize ();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("Opening I2C: ");
    error = I2COpen ();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    /************************************
     * Init BME280
     ************************************/
    printf ("Initializing BME280: ");
    error = Bme280Initialize (bme280Address);
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("Opening Bme280: ");
    error = Bme280Open (bme280Address);
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("Chip ID: ");
    error = Bme280GetChipId (bme280Address, &chipId);
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("0x%02x\r\n", chipId);

    printf ("Retrieving BME280 Calibration Factors: ");
    // Get Calibration Factors
    error = Bme280GetCalibrationFactors (bme280Address, &calibrationFactors);
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("\r\n");

#ifdef USE_VT100
    {
        // Save Cursor Position
        printf ("\0337");
    }
#endif

    while (true)
    {
        // Get Temperature
        Bme280GetTemperature (bme280Address, &calibrationFactors, &temperature);

        // Get Barometric Pressure
        Bme280GetPressure (bme280Address, &calibrationFactors, &pressure);

        // Get Humidity
        Bme280GetHumidity (bme280Address, &calibrationFactors, &humidity);

        // Print Results To Screen
        printf ("    Current Temp: %fC (%fF)\r\n", temperature, ((temperature * 9.0) / 5.0) + 32.0);
        printf ("Current Pressure: %fhPa\r\n", pressure);
        printf ("Current Humidity: %f%%\r\n", humidity);

#ifdef USE_VT100
        {
            // Restore Cursor Position
            printf ("\0338");
        }
#endif

        TimerSleepMs (500);
    }
}
