#ifndef I2C_H_
#define I2C_H_

ssp_err_t I2CInitialize(void);
ssp_err_t I2COpen(void);
ssp_err_t I2CClose(void);
ssp_err_t I2CWrite(uint8_t slaveAddress, uint8_t * const data, uint32_t const length, bool const restart);
ssp_err_t I2CWriteByte(uint8_t slaveAddress, uint8_t const data, bool const restart);
ssp_err_t I2CRead(uint8_t slaveAddress, uint8_t * const data, uint32_t const length, bool const restart);
ssp_err_t I2CReadByte(uint8_t slaveAddress, uint8_t * const data, bool const restart);

#endif /* I2C_H_ */
