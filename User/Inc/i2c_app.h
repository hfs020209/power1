#ifndef __I2C_APP_H__
#define __I2C_APP_H__



#include "sys_app.h"

uint8_t Eeprom_I2C_Write(uint8_t *pWriteBuf, uint16_t usAddress, uint16_t usSize);
uint8_t Eeprom_I2C_Read(uint8_t *pReadBuf, uint16_t usAddress, uint16_t usSize);

void rc_sys_i2c_app_poll(void);

#endif
