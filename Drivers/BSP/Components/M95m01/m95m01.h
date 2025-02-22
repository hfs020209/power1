#ifndef __M95M01_H__
#define __M95M01_H__

#ifdef __cplusplus
 extern "C" {
#endif   

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "main.h"

/* M95512 SPI EEPROM defines */
#define EEPROM_WREN  0x06  /*!< Write Enable */
#define EEPROM_WRDI  0x04  /*!< Write Disable */
#define EEPROM_RDSR  0x05  /*!< Read Status Register */
#define EEPROM_WRSR  0x01  /*!< Write Status Register */
#define EEPROM_READ  0x03  /*!< Read from Memory Array */
#define EEPROM_WRITE 0x02  /*!< Write to Memory Array */
 
#define EEPROM_WIP_FLAG     0x01  /*!< Write In Progress (WIP) flag */
#define EEPROM_WEL_FLAG			0x02	/*!< WriteEnable In Progress (WIP) flag */

#define EEPROM_PAGESIZE     256    /*!< Pagesize according to documentation */
 
#define EE_NSS_LOW    HAL_GPIO_WritePin(EEP_CS_GPIO_Port, EEP_CS_Pin, GPIO_PIN_RESET);
#define EE_NSS_HIGH   HAL_GPIO_WritePin(EEP_CS_GPIO_Port, EEP_CS_Pin, GPIO_PIN_SET);
/**
 * @brief EEPROM Operations statuses
 */
typedef enum {
    EEPROM_STATUS_PENDING,
    EEPROM_STATUS_COMPLETE,
    EEPROM_STATUS_ERROR
} EepromOperations;
 
EepromOperations EEPROM_Write(uint8_t* TxData, uint32_t WriteAddr, uint16_t NumByteToWrite);
EepromOperations EEPROM_WritePage(uint8_t* TxData, uint32_t WriteAddr, uint16_t NumByteToWrite);
EepromOperations EEPROM_Read(uint8_t* RxData, uint32_t ReadAddr, uint16_t NumByteToRead);
uint8_t EEPROM_WaitStandbyState(void);
 
/* Low layer functions */
void sEE_WriteEnable(void);
void sEE_WriteDisable(void);
void sEE_WriteStatusRegister(uint8_t regval);
uint8_t sEE_ReadStatusRegister(void);
 
void EEPROM_SendInstruction(uint8_t *instruction, uint16_t size);


#ifdef __cplusplus
  }
#endif
#endif /* __M95XX_H */


