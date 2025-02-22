#include "adc_app.h"



/* --------------------------------------------------------------------------
 *  ADC采集数组定义
 * --------------------------------------------------------------------------
 */
volatile uint16_t ADC_convert_result[ADC_MAX] = {0};
/* --------------------------------------------------------------------------
 *  ADC采集完成标志位
 * --------------------------------------------------------------------------
 */
volatile uint8_t	ADC_convertend_flag=0;
/* --------------------------------------------------------------------------
 *  温度
 * --------------------------------------------------------------------------
 */
volatile double	dev_temp=0;
volatile uint16_t dev_voltage[ADC_MAX-1] = {0};

/********************************************************
  * 函数功能: 温度计算
  * 输入参数: AD值
  * 返 回 值: 温度值
  * 说    明: 无
********************************************************/
double rc_temp_adc_result(uint32_t temp_adc)
{
	double temp=0;
	temp = (double)((temp_adc*3.3/4096)-0.5)/0.01;
	return temp;
}

/********************************************************
  * 函数功能: 电压计算
  * 输入参数: voltage_adc：AD原始值
									voltage： 返回计算值
  * 返 回 值: 电压值
  * 说    明: 无
********************************************************/
void rc_voltage_adc_result(uint16_t* voltage_adc, uint16_t* voltage)
{
	voltage[0]=(uint32_t)(voltage_adc[0]*3.3*81.43586419*1.029411765/4096) - 1.838235294 * 10;
	voltage[1]=(uint32_t)(voltage_adc[1]*3.3*81.43586419*1.014492754/4096) - 1.81884058 * 10;
	voltage[2]=(uint32_t)(voltage_adc[2]*3.3*81.43586419*1.02489019/4096) - 1.827232796 * 10;
	voltage[3]=(uint32_t)(voltage_adc[3]*3.3*454.1771331*1.008827238/4096) - 3.581336696 * 10;
	voltage[4]=(uint32_t)(voltage_adc[4]*3.3*454.1771331*1.015228426/4096) - 3.45177665 * 10;
}

/********************************************************
  * 函数功能: ADC任务
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 无
********************************************************/
void rc_sys_adc_app_poll(void)
{
	static uint32_t adc_timer = 0;
	if(rc_sys_timeHasPast(adc_timer, ADC_TIMEOUT)){
		
		adc_timer = rc_sys_timeGet();
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_convert_result, 6);
	}
	
	if(ADC_convertend_flag ==1)
	{
		
		ADC_convertend_flag=0;
		rc_voltage_adc_result((uint16_t*)&ADC_convert_result, (uint16_t*)&dev_voltage);
		dev_temp = rc_temp_adc_result((ADC_convert_result[5]));
		
//		debug_log("adc:%d,%d,%d,%d,%d,  temp=%0.2f",dev_voltage[0],dev_voltage[1],dev_voltage[2],dev_voltage[3],dev_voltage[4],dev_temp);
	}
}
/********************************************************
  * 函数功能: adc DMA转换完成回调
  * 输入参数: 指向ADC_HandleTypeDef结构
  * 返 回 值: 无
  * 说    明: 重定向CAN的FIFO0缓冲器中断接收
********************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == hadc1.Instance)
	{
		ADC_convertend_flag=1;
	}
}
