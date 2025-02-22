#include "m95m01.h"

#define SPI1_DMA

HAL_StatusTypeDef SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
#ifdef SPI1_DMA
	return HAL_SPI_Transmit_DMA(hspi,pData, Size);
#else
	return HAL_SPI_Transmit(hspi,pData, Size, Timeout);
#endif
}

HAL_StatusTypeDef SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
#ifdef SPI1_DMA
	return HAL_SPI_Receive_DMA(hspi,pData, Size);
#else
	return HAL_SPI_Receive(hspi,pData, Size, Timeout);
#endif
}

EepromOperations EEPROM_WritePage(uint8_t* TxData, uint32_t WriteAddr, uint16_t NumByteToWrite) 
{
  HAL_StatusTypeDef spiTransmitStatus;
 
  sEE_WriteEnable();
 
  /* We gonna send commands in one packet of 3 bytes */
  uint8_t header[4];
 
  header[0] = EEPROM_WRITE;   // Send "Write to Memory" instruction
  header[1] = WriteAddr >> 16; // Send 16-bit address
  header[2] = WriteAddr >> 8;
  header[3] = WriteAddr;
  // Select the EEPROM: Chip Select low
  EE_NSS_LOW;
 
  EEPROM_SendInstruction(header, 4);
 
	spiTransmitStatus = SPI_Transmit(&hspi1, TxData, NumByteToWrite, 500);
	
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
  }
	
  // Deselect the EEPROM: Chip Select high
  EE_NSS_HIGH;
	
	//HAL_Delay(5); 
  // Wait the end of EEPROM writing
  EEPROM_WaitStandbyState();
 
  // Disable the write access to the EEPROM
  sEE_WriteDisable();
 
  if (spiTransmitStatus == HAL_ERROR) {
    return EEPROM_STATUS_ERROR;
  } else {
    return EEPROM_STATUS_COMPLETE;
  }
}
 
/**
  * @brief  Writes block of data to the EEPROM. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  *
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM.
  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
  */
EepromOperations EEPROM_Write(uint8_t* TxData, uint32_t WriteAddr, uint16_t NumByteToWrite) 
{
  uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
  uint32_t sEE_DataNum = 0;
 
  EepromOperations pageWriteStatus = EEPROM_STATUS_PENDING;
 
  Addr = WriteAddr % EEPROM_PAGESIZE;
  count = EEPROM_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
  NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;
 
  if (Addr == 0) { /* WriteAddr is EEPROM_PAGESIZE aligned  */
    if (NumOfPage == 0) { /* NumByteToWrite < EEPROM_PAGESIZE */
			sEE_DataNum = NumByteToWrite;
      pageWriteStatus = EEPROM_WritePage(TxData, WriteAddr, sEE_DataNum);
 
      if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
        return pageWriteStatus;
      }
    } else { /* NumByteToWrite > EEPROM_PAGESIZE */
      while (NumOfPage--) {
        sEE_DataNum = EEPROM_PAGESIZE;
        pageWriteStatus = EEPROM_WritePage(TxData, WriteAddr, sEE_DataNum);
 
        if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
          return pageWriteStatus;
        }
 
        WriteAddr +=  EEPROM_PAGESIZE;
        TxData += EEPROM_PAGESIZE;
      }
 
      sEE_DataNum = NumOfSingle;
      pageWriteStatus = EEPROM_WritePage(TxData, WriteAddr, sEE_DataNum);
 
      if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
        return pageWriteStatus;
      }
    }
  } else { /* WriteAddr is not EEPROM_PAGESIZE aligned  */
    if (NumOfPage == 0) { /* NumByteToWrite < EEPROM_PAGESIZE */
      if (NumOfSingle > count) { /* (NumByteToWrite + WriteAddr) > EEPROM_PAGESIZE */
        temp = NumOfSingle - count;
        sEE_DataNum = count;
        pageWriteStatus = EEPROM_WritePage(TxData, WriteAddr, sEE_DataNum);
 
        if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
          return pageWriteStatus;
        }
 
        WriteAddr +=  count;
        TxData += count;
 
        sEE_DataNum = temp;
        pageWriteStatus = EEPROM_WritePage(TxData, WriteAddr, sEE_DataNum);
      } else {
        sEE_DataNum = NumByteToWrite;
        pageWriteStatus = EEPROM_WritePage(TxData, WriteAddr, sEE_DataNum);
      }
 
      if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
				return pageWriteStatus;
      }
    } else { /* NumByteToWrite > EEPROM_PAGESIZE */
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
      NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;
 
      sEE_DataNum = count;
 
      pageWriteStatus = EEPROM_WritePage(TxData, WriteAddr, sEE_DataNum);
 
      if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
        return pageWriteStatus;
      }
 
      WriteAddr +=  count;
      TxData += count;
 
      while (NumOfPage--) {
        sEE_DataNum = EEPROM_PAGESIZE;
 
        pageWriteStatus = EEPROM_WritePage(TxData, WriteAddr, sEE_DataNum);
 
        if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
          return pageWriteStatus;
        }
 
        WriteAddr +=  EEPROM_PAGESIZE;
        TxData += EEPROM_PAGESIZE;
      }
 
      if (NumOfSingle != 0) {
        sEE_DataNum = NumOfSingle;
 
        pageWriteStatus = EEPROM_WritePage(TxData, WriteAddr, sEE_DataNum);
 
        if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
          return pageWriteStatus;
        }
      }
    }
  }
 
  return EEPROM_STATUS_COMPLETE;
}
 
/**
  * @brief  Reads a block of data from the EEPROM.
  *
  * @param  pBuffer: pointer to the buffer that receives the data read from the EEPROM.
  * @param  ReadAddr: EEPROM's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the EEPROM.
  * @retval None
  */
EepromOperations EEPROM_Read(uint8_t* RxData, uint32_t ReadAddr, uint16_t NumByteToRead) 
{
  /* We gonna send all commands in one packet of 3 bytes */
  uint8_t header[4];
 
  header[0] = EEPROM_READ;    // Send "Read from Memory" instruction
  header[1] = ReadAddr >> 16;  // Send 16-bit address
  header[2] = ReadAddr >> 8;
  header[3] = ReadAddr;
  // Select the EEPROM: Chip Select low
  EE_NSS_LOW;
 
  /* Send WriteAddr address byte to read from */
  EEPROM_SendInstruction(header, 4);
	
  SPI_Receive(&hspi1, (uint8_t*)RxData, NumByteToRead, 500);
 
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
  }
 
  // Deselect the EEPROM: Chip Select high
  EE_NSS_HIGH;
 
  return EEPROM_STATUS_COMPLETE;
}

/**
  * @brief  Enables the write access to the EEPROM.
  *
  * @param  None
  * @retval None
  */
void sEE_WriteEnable(void) 
{
  // Select the EEPROM: Chip Select low
  EE_NSS_LOW;
 
  uint8_t command[1] = { EEPROM_WREN };
  /* Send "Write Enable" instruction */
  EEPROM_SendInstruction(command, 1);
 
  // Deselect the EEPROM: Chip Select high
  EE_NSS_HIGH;
}
 
/**
  * @brief  Disables the write access to the EEPROM.
  *
  * @param  None
  * @retval None
  */
void sEE_WriteDisable(void) 
{
  // Select the EEPROM: Chip Select low
  EE_NSS_LOW;
 
  uint8_t command[1] = { EEPROM_WRDI };
 
  /* Send "Write Disable" instruction */
  EEPROM_SendInstruction(command, 1);
 
  // Deselect the EEPROM: Chip Select high
  EE_NSS_HIGH;
}
 
/**
  * @brief  Write new value in EEPROM Status Register.
  *
  * @param  regval : new value of register
  * @retval None
  */
void sEE_WriteStatusRegister(uint8_t regval)
{
  uint8_t command[2];
 
  command[0] = EEPROM_WRSR;
  command[1] = regval;
 
  // Enable the write access to the EEPROM
  sEE_WriteEnable();
 
  // Select the EEPROM: Chip Select low
  EE_NSS_LOW;
 
  // Send "Write Status Register" instruction
  // and Regval in one packet
  EEPROM_SendInstruction(command, 2);
 
  // Deselect the EEPROM: Chip Select high
  EE_NSS_HIGH;
 
  sEE_WriteDisable();
}

/**
  * @brief  Write new value in EEPROM Status Register.
  *
  * @param  regval : new value of register
  * @retval None
  */
uint8_t sEE_ReadStatusRegister(void)
{
  uint8_t sEEstatus;
  uint8_t command[1] = { EEPROM_RDSR };
 
  // Select the EEPROM: Chip Select low
  EE_NSS_LOW;
 
  // Send "Read Status Register" instruction
  EEPROM_SendInstruction(command, 1);
 
  // Loop as long as the memory is busy with a write cycle
	SPI_Receive(&hspi1, &sEEstatus, 1, 500);
	
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
	}
  // Deselect the EEPROM: Chip Select high
  EE_NSS_HIGH;
 
  return sEEstatus;
}
 
/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the EEPROM's
  *         status register and loop until write operation has completed.
  *
  * @param  None
  * @retval None
  */
uint8_t EEPROM_WaitStandbyState(void)
{
  uint8_t sEEstatus[1] = { 0x00 };
  uint8_t command[1] = { EEPROM_RDSR };
 
  // Select the EEPROM: Chip Select low
  EE_NSS_LOW;
 
  // Send "Read Status Register" instruction
  EEPROM_SendInstruction(command, 1);
 
  // Loop as long as the memory is busy with a write cycle
  do {
		SPI_Receive(&hspi1, (uint8_t*)sEEstatus, 1, 500);
		
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
    }
  } while ((sEEstatus[0] & EEPROM_WIP_FLAG) == SET); // Write in progress
 
  // Deselect the EEPROM: Chip Select high
  EE_NSS_HIGH;
 
  return 0;
}
 
/**
 * @brief Low level function to send header data to EEPROM
 *
 * @param instruction array of bytes to send
 * @param size        data size in bytes
 */
void EEPROM_SendInstruction(uint8_t *instruction, uint16_t size)
{
  SPI_Transmit(&hspi1, instruction, size, 500);
	
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
  }
}


