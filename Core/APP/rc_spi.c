//#include <stdio.h>
//#include <stdarg.h>
//#include <string.h>
//#include <math.h>

//#include "main.h"
//#include "cmsis_os.h"
//#include "common.h"
//#include "Hal.h"
//#include "app.h"
//#include "rc_can.h"
//#include "rc_spi.h"

//static sspi_info sampleSspi;    //spi状态记录结构体




//static int spi_tick = 20;       //定时器相关

//void getMsgtoSpiMaster(void);

//void sampleSpiInit(void)        //通过GPIO中断模拟硬件SPI控制器，每个SPI信号的变化都会触发相应的中断处理。
//{
//    GPIO_InitTypeDef GPIO_Initure;

//    sampleSspi.s_recvMax = SPI_BUF_MAX;

//    sampleSspi.s_buf = NULL;
//    sampleSspi.s_sendMax = 0;
//    sampleSspi.s_send = 0;

//    //initiate cs_pin
//    GPIO_Initure.Pin = SPI_CS_Pin;
//    GPIO_Initure.Mode = GPIO_MODE_IT_RISING_FALLING;    //cs引脚，双沿触发中断
//    GPIO_Initure.Pull = GPIO_NOPULL;
//    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(SPI_CS_PORT, &GPIO_Initure);

//    //initiate sck_pin
//    GPIO_Initure.Pin = SPI_SCK_Pin;
//    GPIO_Initure.Mode = GPIO_MODE_IT_RISING;    //sck引脚上升沿中断
//    GPIO_Initure.Pull = GPIO_NOPULL;
//    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(SPI_SCK_PORT, &GPIO_Initure);

//    //initiate mosi_pin
//    GPIO_Initure.Pin = SPI_MOSI_Pin;
//    GPIO_Initure.Mode = GPIO_MODE_INPUT;    //mosi为输入模式
//    GPIO_Initure.Pull = GPIO_NOPULL;
//    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(SPI_MOSI_PORT, &GPIO_Initure);

//    //initiate miso_pin
//    GPIO_Initure.Pin = SPI_MISO_Pin;
//    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;    //miso配置为推挽输出
//    GPIO_Initure.Pull = GPIO_PULLDOWN;
//    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(SPI_MISO_PORT,&GPIO_Initure);

//    #if 0//ISR
//    //configMAX_SYSCALL_INTERRUPT_PRIORITY = 5
//    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 6, 0);
//    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

//    HAL_NVIC_SetPriority(EXTI4_IRQn, 6, 0);
//    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
//#endif
//}


//static inline int sspiCs(void)
//{
//    if(HAL_GPIO_ReadPin(SPI_CS_PORT, SPI_CS_Pin) == GPIO_PIN_SET)
//        
//        return 1;
//    
//    return 0;
//}


//static inline int sspiSck(void)
//{
//    if(HAL_GPIO_ReadPin(SPI_SCK_PORT, SPI_SCK_Pin) == GPIO_PIN_SET)
//        
//        return 1;
//    
//    return 0;
//}


//static inline int sspiMosi(void)
//{
//    if(HAL_GPIO_ReadPin(SPI_MOSI_PORT, SPI_MOSI_Pin) == GPIO_PIN_SET)
//        
//        return 1;
//    
//    return 0;
//}


//static inline void setSspiMiso(uint8_t value)
//{
//    if(value)
//        HAL_GPIO_WritePin(SPI_MISO_PORT, SPI_MISO_Pin, GPIO_PIN_SET);
//    else
//        HAL_GPIO_WritePin(SPI_MISO_PORT, SPI_MISO_Pin, GPIO_PIN_RESET);
//}


//static inline void sspiCS_main(void)
//{
//    int value;
//    value = sspiCs();

//    if(!value){     //片选低电平（通信开始）

//        //cs pin is low
//        sampleSspi.cs_l++;
//        sampleSspi.sck = 1;
//        sampleSspi.mosi = 0;
//        sampleSspi.start = 1;   //通信开始
//        sampleSspi.miso = 0;
//        sampleSspi.s_recv = 0;  //复位接收计数器
//        sampleSspi.r_buf = spiRxBuf.buf;    //绑定接收缓冲区

//        //准备发送数据
//        if(sampleSspi.s_sendMax){
//            
//        //exist msg to sent
//            sampleSspi.miso = sampleSspi.s_buf[sampleSspi.s_send];
//        }else{  

//            getMsgtoSpiMaster();    //从队列获取待发送的数据
//        }

//        setSspiMiso(!!(sampleSspi.miso & 0x80));    //发送最高位128
//        return;
//    }

//    //cs_pin is high
//    if(sampleSspi.s_recv > 0){      //片选高电平（通信结束）
//        
//        //data is received, sent it to the queue
//        //提交接收数据到队列
//        spiRxBuf.hdr.len = sampleSspi.s_recv;
//        if(spi_queueRx)            
//            xQueueSendFromISR(spi_queueRx, &spiRxBuf, NULL);
//        memset(spiRxBuf.buf, 0, SPI_BUF_MAX);
//        sampleSspi.s_recv = 0;
//        }
//        sampleSspi.cs_h++;
//        sampleSspi.start = 0;   //通信结束
//}


//static inline void sspiSCK_main(void)
//{
//    //接收时：在SCK上升沿时读取MOSI引脚的电平，逐位拼装为完整字节。
//    //发送时：根据主设备的SCK信号，通过MISO引脚逐位发送数据。
//    int sck,mosi;

//    if(!spi_queueTx)
//        return;

//    if(!sampleSspi.start)
//        return;

//    sck = sspiSck();

//    if(!sck)
//        return;

//    sampleSspi.sck_h_debug++;

//    mosi = sspiMosi();

//    if((mosi) && (sampleSspi.sck <= 8)){

//        //collect data of mosi
//        sampleSspi.mosi |= 1<<(8-sampleSspi.sck);
//    }

//    sampleSspi.sck++;

//    //dispose and send data
//    sampleSspi.miso = sampleSspi.miso <<1;
//    setSspiMiso(!!(sampleSspi.miso & 0x80));

//    if(sampleSspi.sck < 9)
//        return;

//    //read a word
//    sampleSspi.r_buf[sampleSspi.s_recv] = sampleSspi.mosi;
//    sampleSspi.mosi = 0;
//    if(sampleSspi.s_recv < sampleSspi.s_recvMax){

//        sampleSspi.s_recv++

//    }else{
//        //buf is fully filled, send the data of buf to the queue
//        spiRxBuf.hdr.len = sampleSspi.s_recv;
//        if(spi_queueRx)
//            xQueueSendFromISR(spi_queueRx, &spiRxBuf, NULL);
//        memset(spiRxBuf.buf, 0, SPI_BUF_MAX);
//        sampleSspi.s_recv = 0;
//    }

//    //send the next word
//    sampleSspi.miso = 0;
//    sampleSspi.s_send++;
//    if(sampleSspi.s_send < sampleSspi.s_sendMax){

//        sampleSspi.miso = sampleSspi.s_buf[sampleSspi.s_send];
//    }else{
//        //send_buf is clear
//        sampleSspi.s_sendMax = 0;
//        sampleSspi.s_sendMax = 0;
//    }

//    setSspiMiso(!!(sampleSspi.miso & 0x80));

//    sampleSspi.sck = 1;
//}


//void SSPI_HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    //通过外部中断（EXTI）检测CS和SCK引脚的电平变化，实现SPI协议的位级操作。
//    switch(GPIO_Pin){

//        default:;break;

//        case SPI_CS_Pin:{   //片选状态变化
//            sspiCS_main();
//        };break;

//        case SPI_SCK_Pin:{  //时钟信号变化
//            sspiSCK_main();
//        };break;
//    }
//}

//void getMsgtoSpiMaster(void)
//{
//    if(!spi_queueTx)
//        return;

//    if(xQueueSendFromISR(spi_queueTx, &spiTxBuf, NULL) == pdPASS){
//        sampleSspi.s_sendMax = spiTxBuf.hdr.len;
//        sampleSspi.s_buf = spiTxBuf.buf;
//        sampleSspi.s_send = 0;
//        sampleSspi.miso = sampleSspi.s_buf[sampleSspi.s_send];
//    }
//}

//void SPI_Transmit(uint8_t *pData, uint16_t size)
//{
//		HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_Pin, GPIO_PIN_RESET);
//		HAL_SPI_Transmit(&hspi1, pData, size, HAL_MAX_DELAY);
//		HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_Pin, GPIO_PIN_SET);
//}

//void SPI_Receive(uint8_t *pRxData, uint16_t size)
//{
//		HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_Pin, GPIO_PIN_RESET);
//		HAL_SPI_Transmit(&hspi1, pRxData, size, HAL_MAX_DELAY);
//		HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_Pin, GPIO_PIN_SET);
//}


