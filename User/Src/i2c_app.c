#include "i2c_app.h"

//#define TEST_I2C_EEPROM
#define ReadAddr   0xAF
#define WriteAddr  0xAE

uint16_t PageSize   = 128;        
uint16_t PageNum    = 512;        
uint64_t MemorySize = 65536;               /*65536 FOR 64k */

static uint8_t i2c_flag = 0;		//��ȡ��ɱ�־λ

#define IIC_MEM_READ(R_Addr,R_buf,R_size)	HAL_I2C_Mem_Read_DMA(&hi2c2,ReadAddr,R_Addr,I2C_MEMADD_SIZE_16BIT,R_buf, R_size); //�ж�DMA

/*
*********************************************************************************************************
*	�� �� ��: E2ROM_ReadBytes
*	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
*	��    ��:  usAddress : ��ʼ��ַ
*			 usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t Eeprom_I2C_Read(uint8_t *pReadBuf, uint16_t usAddress, uint16_t usSize)
{
	uint8_t IIC_status=0; 
	
	IIC_status = HAL_I2C_IsDeviceReady(&hi2c2, ReadAddr, 10, 0xFFFF);
	if (IIC_status != HAL_OK)
	{
		err_log("");
		goto cmd_fail;
	}
	
	i2c_flag =1;
	IIC_status = IIC_MEM_READ(usAddress,pReadBuf,usSize); 
	if (IIC_status != HAL_OK)
	{
		i2c_flag =0;
		err_log("");
		goto cmd_fail;
	}

return 1;

cmd_fail:
	debug_log("��eeprom����");
	switch( IIC_status )
	{
		case HAL_OK:break;
		case HAL_ERROR:err_log("HAL_ERROR");return 0;
		case HAL_BUSY:err_log("HAL_BUSY ");return 0;
		case HAL_TIMEOUT:err_log("HAL_TIMEOUT");return 0;
		default:return 0;
	}

return 0;

}

/********************************************************
  * ��������: I2C DMA��ȡ��ɻص�
* �������: *hi2c:i2c2
  * �� �� ֵ: ��
  * ˵    ��: ��
********************************************************/
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c->Instance == I2C2)
	{
		i2c_flag=0;
	}
}

/********************************************************
  * ��������: EEPROM����
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ��
********************************************************/
void rc_sys_i2c_app_poll(void)
{
#ifdef TEST_I2C_EEPROM
	static uint8_t rx512[32];
	static uint16_t add_t=0;
	
	//memset(rx512,0,32);
	
	Eeprom_I2C_Read(&rx512[0],add_t,32);
	
	while(i2c_flag!=0);
	
	Log( "addr:0x%04X---",add_t);

	for(int i =0;i<32; i++)
	{
		Log( ",%02X",rx512[i]);
	}
	Log("\r\n");

	if((uint32_t)(add_t+=32)>=65536)
		add_t=0;
	else
		add_t+=32;
#endif
	//HAL_Delay(1000);
}
