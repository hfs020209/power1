#include "eepromrw_app.h"
#include "m95m01.h"

//#define TEST_EEPROM

#define BIT0        0x01U

/********************************************************
  * 函数功能: 写使能状态读取测试
  * 输入参数: voidc
  * 返 回 值: void
  * 说    明: 用于设备启动测试EEPROM是否正常
********************************************************/
static int32_t WriteEnableReadStatus(void)
{

  uint8_t rx;
  int32_t readstatus = EEPROM_STATUS_COMPLETE;
	
	sEE_WriteEnable();
	
	rx = sEE_ReadStatusRegister();
	
	if((rx & EEPROM_WEL_FLAG)==0x02)
		return readstatus;
	else
		return EEPROM_STATUS_ERROR;
			
}

/********************************************************
  * 函数功能: 写禁用状态读取测试
  * 输入参数: void
  * 返 回 值: void
  * 说    明: 用于设备启动测试EEPROM是否正常
********************************************************/
static int32_t WriteDisableReadStatus(void)
{
  uint8_t rx;
  int32_t readstatus = EEPROM_STATUS_COMPLETE;

	sEE_WriteDisable();
	
	rx = sEE_ReadStatusRegister();
	
	if((rx & EEPROM_WEL_FLAG)==0x00)
		return readstatus;
	else
		return EEPROM_STATUS_ERROR;
}

/********************************************************
  * 函数功能: EEPROM 初始化
  * 输入参数: void
  * 返 回 值: void
  * 说    明: 无
********************************************************/
void rc_eeprom_app_init(void)
{
	uint8_t status = 0;
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
  }
	
		
	status = WriteDisableReadStatus();
  HAL_Delay(5);
  status |= WriteEnableReadStatus();
	if(status != EEPROM_STATUS_COMPLETE)
	{
    Error_Handler();
  }
	UserLog("SPI EEPROM READ STATUS SUCCES");
	
}

/********************************************************
  * 函数功能: EEPROM 任务
  * 输入参数: void
  * 返 回 值: void
  * 说    明: 目前为测试程序
********************************************************/
void rc_eeprom_app_poll(void)
{
#ifdef TEST_EEPROM
	uint8_t tx256[] = "abcdefghijklmnopqrstuvwxtadd0000";
	static uint8_t rx32[32];
	static uint32_t add_t=0;
	
	memset(rx32,0,32);
	tx256[28]='\0';
	tx256[29]=add_t>>16;
	tx256[30]=add_t>>8;
	tx256[31]=add_t;
	
	EEPROM_Write(tx256,add_t,32);
	EEPROM_Read(rx32,add_t,32);
	
	uint16_t len = strlen((char *)&rx32[0]);

	Log( "%s",rx32);

	Log( "--0x%02X",rx32[30]);
	
	Log( "%02X",rx32[31]);
	
	Log("\r\n");

	add_t+=32;
	
	if(add_t>=1048576)
		add_t=0;
#endif
	//HAL_Delay(1000);
}





