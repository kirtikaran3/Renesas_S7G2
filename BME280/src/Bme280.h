#ifndef BME280_H_
#define BME280_H_

#define BME280_CHIP_ID                      0x60
#define BME280_CHIP_ID_ADDRESS              0xD0
#define BME280_CALIBRATION_LOWER_ADDRESS    0x88
#define BME280_CALIBRATION_UPPER_ADDRESS    0xE1
#define BME280_CONFIG_ADDRESS               0xF5
#define BME280_HUMIDITY_CONTROL_ADDRESS     0xF2
#define BME280_MEASUREMENT_CONTROL_ADDRESS  0xF4
#define BME280_TEMPERATURE_ADDRESS          0xFA
#define BME280_PRESSURE_ADDRESS             0xF7
#define BME280_HUMIDITY_ADDRESS             0xFD

// Structure to hold the BME calibration factors
typedef struct bme280CalibrationFactors
{
    uint16_t T1;
    int16_t T2;
    int16_t T3;
    uint16_t P1;
    int16_t P2;
    int16_t P3;
    int16_t P4;
    int16_t P5;
    int16_t P6;
    int16_t P7;
    int16_t P8;
    int16_t P9;
    uint8_t H1;
    int16_t H2;
    uint8_t H3;
    int16_t H4;
    int16_t H5;
    int8_t H6;

} Bme280CalibrationFactors;

ssp_err_t Bme280Initialize(uint8_t const deviceAddress);
ssp_err_t Bme280GetChipId(uint8_t const deviceAddress, uint8_t * const chipId);
ssp_err_t Bme280Open(uint8_t const deviceAddress);
ssp_err_t Bme280Close(void);
ssp_err_t Bme280GetCalibrationFactors(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors);
ssp_err_t Bme280GetTemperatureFine(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors, int32_t * const temperatureFine);
ssp_err_t Bme280GetTemperature(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors, float * const temperature);
ssp_err_t Bme280GetPressure(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors, float * const pressure);
ssp_err_t Bme280GetHumidity(uint8_t const deviceAddress, Bme280CalibrationFactors * const calibrationFactors, float * const humidity);

#endif /* BME280_H_ */
