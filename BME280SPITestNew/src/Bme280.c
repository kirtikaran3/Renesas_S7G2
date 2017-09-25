#include "hal_data.h"

#include <stdbool.h>
#include <stdio.h>
#include "Bme280.h"
#include "I2C.h"

ssp_err_t Bme280Initialize(uint8_t const deviceAddress)
{
    SSP_PARAMETER_NOT_USED(deviceAddress);

    return SSP_SUCCESS;
}

ssp_err_t Bme280GetChipId(uint8_t const deviceAddress, uint8_t * const chipId)
{
    ssp_err_t error;

    error = I2CWriteByte (deviceAddress, BME280_CHIP_ID_ADDRESS, true);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    error = I2CReadByte (deviceAddress, chipId, false);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    return SSP_SUCCESS;
}

ssp_err_t Bme280Open(uint8_t const deviceAddress)
{
    ssp_err_t error;
    uint8_t chipId;

    error = Bme280GetChipId (deviceAddress, &chipId);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    if (chipId != BME280_CHIP_ID)
    {
        return -1;
    }

    // Set Config
    const uint8_t config[] =
    { BME280_CONFIG_ADDRESS, 0b00010000 };
    error = I2CWrite (deviceAddress, (uint8_t * const ) config, sizeof(config), false);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    const uint8_t ctrl_hum[] =
    { BME280_HUMIDITY_CONTROL_ADDRESS, 0b00000101 };
    error = I2CWrite (deviceAddress, (uint8_t * const ) ctrl_hum, sizeof(ctrl_hum), false);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    const uint8_t ctrl_meas[] =
    { BME280_MEASUREMENT_CONTROL_ADDRESS, 0b10110111 };
    error = I2CWrite (deviceAddress, (uint8_t * const ) ctrl_meas, sizeof(ctrl_hum), false);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    return SSP_SUCCESS;
}

ssp_err_t Bme280Close(void)
{
    return SSP_SUCCESS;
}

ssp_err_t Bme280GetCalibrationFactors(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors)
{
    // Calibration factors are split among two different
    // memory chunks.
    // The first chunk starts at 0x88
    uint8_t calibrationDataLower[25];

    // The second chunk starts at 0xE1
    uint8_t calibrationDataUpper[7];

    ssp_err_t error;

    // Send lower calibration memory address
    error = I2CWriteByte (deviceAddress, BME280_CALIBRATION_LOWER_ADDRESS, true);
    if (error != SSP_SUCCESS)
        return error;

    // Read lower data chunk
    error = I2CRead (deviceAddress, calibrationDataLower, sizeof(calibrationDataLower), false);
    if (error != SSP_SUCCESS)
        return error;

    // Send upper calibration memory address
    error = I2CWriteByte (deviceAddress, BME280_CALIBRATION_UPPER_ADDRESS, true);
    if (error != SSP_SUCCESS)
        return error;

    // Read upper data chunk
    error = I2CRead (deviceAddress, calibrationDataUpper, sizeof(calibrationDataUpper), false);
    if (error != SSP_SUCCESS)
        return error;

    // Move data from the data buffers to the calibration factors and cast to the correct data type.

    // Temperature Cal Factors
    calibrationFactors->T1 =
            (uint16_t) ((uint16_t) (calibrationDataLower[1] << 8) | (uint16_t) calibrationDataLower[0]);
    calibrationFactors->T2 = (int16_t) ((int16_t) (calibrationDataLower[3] << 8) | (int16_t) calibrationDataLower[2]);
    calibrationFactors->T3 = (int16_t) ((int16_t) (calibrationDataLower[5] << 8) | (int16_t) calibrationDataLower[4]);

    // Pressure Cal Factors
    calibrationFactors->P1 = (uint16_t) (calibrationDataLower[7] << 8 | calibrationDataLower[6]);
    calibrationFactors->P2 = (int16_t) (calibrationDataLower[9] << 8 | calibrationDataLower[8]);
    calibrationFactors->P3 = (int16_t) (calibrationDataLower[11] << 8 | calibrationDataLower[10]);
    calibrationFactors->P4 = (int16_t) (calibrationDataLower[13] << 8 | calibrationDataLower[12]);
    calibrationFactors->P5 = (int16_t) (calibrationDataLower[15] << 8 | calibrationDataLower[14]);
    calibrationFactors->P6 = (int16_t) (calibrationDataLower[17] << 8 | calibrationDataLower[16]);
    calibrationFactors->P7 = (int16_t) (calibrationDataLower[19] << 8 | calibrationDataLower[18]);
    calibrationFactors->P8 = (int16_t) (calibrationDataLower[21] << 8 | calibrationDataLower[20]);
    calibrationFactors->P9 = (int16_t) (calibrationDataLower[23] << 8 | calibrationDataLower[22]);

    // Humidity Cal Factors
    calibrationFactors->H1 = (uint8_t) (calibrationDataLower[24]);
    calibrationFactors->H2 = (int16_t) (calibrationDataUpper[1] << 8 | calibrationDataUpper[0]);
    calibrationFactors->H3 = (uint8_t) (calibrationDataUpper[2]);

    // H4 and H5 are split weirdly. Which Bosch just used one additional byte of memory to make this easier
    calibrationFactors->H4 = (int16_t) (calibrationDataUpper[3] << 4 | (calibrationDataUpper[4] & 0x0F));
    calibrationFactors->H5 = (int16_t) (calibrationDataUpper[5] << 4 | calibrationDataUpper[4] >> 4);
    calibrationFactors->H6 = (int8_t) (calibrationDataUpper[6]);

    return SSP_SUCCESS;
}

ssp_err_t Bme280GetTemperature(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors,
        float * const temperature)
{
    ssp_err_t error;

    // Variables below are named so I could copy and paste example code from datasheet
    int32_t T;
    int32_t t_fine;

    // Get fine temperature
    error = Bme280GetTemperatureFine (deviceAddress, calibrationFactors, &t_fine);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    T = (t_fine * 5 + 128) >> 8;

    // Store temperature (in degrees C)
    *temperature = (float) T / 100.0f;

    return SSP_SUCCESS;
}

ssp_err_t Bme280GetTemperatureFine(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors,
        int32_t * const temperatureFine)
{
    ssp_err_t error;
    uint8_t data[3];

    // Variables below are named so I could copy and paste example code from datasheet
    int32_t adc_T;
    int32_t var1, var2;

    uint16_t dig_T1 = calibrationFactors->T1;
    int16_t dig_T2 = calibrationFactors->T2;
    int16_t dig_T3 = calibrationFactors->T3;

    // Send Temperature Measurement Address
    error = I2CWriteByte (deviceAddress, BME280_TEMPERATURE_ADDRESS, true);
    if (error != SSP_SUCCESS)
        return error;

    // Read Temperature ADC Value
    error = I2CRead (deviceAddress, data, sizeof(data), false);
    if (error != SSP_SUCCESS)
        return error;

    // Convert ADC value to temperature
    adc_T = (int32_t) (((data[0] << 16) | (data[1] << 8) | (data[2] << 0)) >> 4);

    var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1))) * ((int32_t) dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) dig_T1)) * ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3))
            >> 14;

    // Store Result
    *temperatureFine = (int32_t) (var1 + var2);

    return SSP_SUCCESS;
}

ssp_err_t Bme280GetPressure(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors,
        float * const pressure)
{
    ssp_err_t error;
    uint8_t data[3];

    // Variables below are named so I could copy and paste example code from datasheet
    int32_t adc_P;
    int64_t var1, var2, p;
    int32_t t_fine;

    uint16_t dig_P1 = calibrationFactors->P1;
    int16_t dig_P2 = calibrationFactors->P2;
    int16_t dig_P3 = calibrationFactors->P3;
    int16_t dig_P4 = calibrationFactors->P4;
    int16_t dig_P5 = calibrationFactors->P5;
    int16_t dig_P6 = calibrationFactors->P6;
    int16_t dig_P7 = calibrationFactors->P7;
    int16_t dig_P8 = calibrationFactors->P8;
    int16_t dig_P9 = calibrationFactors->P9;

    // Get Fine Temperature
    error = Bme280GetTemperatureFine (deviceAddress, calibrationFactors, &t_fine);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    // Send Pressure Measurement Address
    error = I2CWriteByte (deviceAddress, BME280_PRESSURE_ADDRESS, true);
    if (error != SSP_SUCCESS)
        return error;

    // Read Pressure ADC Value
    error = I2CRead (deviceAddress, data, sizeof(data), false);
    if (error != SSP_SUCCESS)
        return error;

    // Convert ADC value to pressure
    adc_P = (int32_t) (((data[0] << 16) | (data[1] << 8) | (data[2] << 0)) >> 4);

    var1 = ((int64_t) t_fine) - 128000;
    var2 = var1 * var1 * (int64_t) dig_P6;
    var2 = var2 + ((var1 * (int64_t) dig_P5) << 17);
    var2 = var2 + (((int64_t) dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t) dig_P3) >> 8) + ((var1 * (int64_t) dig_P2) << 12);
    var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) dig_P1) >> 33;
    if (var1 == 0)
    {
        return -1; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t) dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t) dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t) dig_P7) << 4);

    // Store Result
    *pressure = ((float) p) / (256.0f * 100.0f);

    return SSP_SUCCESS;
}

ssp_err_t Bme280GetHumidity(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors,
        float * const humidity)
{
    ssp_err_t error;
    uint8_t data[2];

    // Variables below are named so I could copy and paste example code from datasheet
    int16_t adc_H;
    uint32_t h;
    int32_t t_fine;
    int32_t v_x1_u32;

    uint8_t dig_H1 = calibrationFactors->H1;
    int16_t dig_H2 = calibrationFactors->H2;
    uint8_t dig_H3 = calibrationFactors->H3;
    int16_t dig_H4 = calibrationFactors->H4;
    int16_t dig_H5 = calibrationFactors->H5;
    int8_t dig_H6 = calibrationFactors->H6;

    // Get Fine Temperature
    error = Bme280GetTemperatureFine (deviceAddress, calibrationFactors, &t_fine);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    // Send Humidity Measurement Address
    error = I2CWriteByte (deviceAddress, BME280_HUMIDITY_ADDRESS, true);
    if (error != SSP_SUCCESS)
        return error;

    // Read Humidity ADC Value
    error = I2CRead (deviceAddress, data, sizeof(data), false);
    if (error != SSP_SUCCESS)
        return error;

    // Convert ADC value to humidity
    adc_H = (int16_t) ((data[0] << 8) | (data[1] << 0));

    /* calculate x1*/
    v_x1_u32 = (t_fine - ((int32_t) 76800));

    /* calculate x1*/
    v_x1_u32 = (((((adc_H << 14) - (((int32_t) dig_H4) << 20) - (((int32_t) dig_H5) * v_x1_u32)) + ((int32_t) 16384))
            >> 15)
            * (((((((v_x1_u32 * ((int32_t) dig_H6)) >> 10)
                    * (((v_x1_u32 * ((int32_t) dig_H3)) >> 11) + ((int32_t) 32768))) >> 10) + ((int32_t) 2097152))
                    * ((int32_t) dig_H2) + 8192) >> 14));
    v_x1_u32 = (v_x1_u32 - (((((v_x1_u32 >> 15) * (v_x1_u32 >> 15)) >> 7) * ((int32_t) dig_H1)) >> 4));
    v_x1_u32 = (v_x1_u32 < 0 ? 0 : v_x1_u32);
    v_x1_u32 = (v_x1_u32 > 419430400 ? 419430400 : v_x1_u32);
    h = (uint32_t) (v_x1_u32 >> 12);

    // Store Result
    *humidity = ((float) h) / 1024.0f;

    return SSP_SUCCESS;
}
