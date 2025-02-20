#ifndef __CHECK_H__
#define __CHECK_H__

uint32_t DWord_Sum_Check(uint32_t* src,uint32_t len);
uint16_t Word_Sum_Check(uint16_t* src,uint32_t len);
uint8_t Byte_Sum_Check(uint8_t* src,uint32_t len);
uint16_t CRC16_Chk(uint8_t *buf, uint32_t len);


#endif
