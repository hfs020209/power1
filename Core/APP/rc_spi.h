#ifndef __RC_SPI_H__
#define __RC_SPI_H__



#define SPI_CS_Pin  GPIO_PIN_6
#define SPI_CS_PORT GPIOB

#define SPI_SCK_Pin GPIO_PIN_3
#define SPI_SCK_PORT    GPIOB

#define SPI_MOSI_PORT   GPIOB
#define SPI_MISO_PORT   GPIOB

#define SPI_MISO_Pin    GPIO_PIN_4
#define SPI_MOSI_Pin    GPIO_PIN_5

typedef struct{

    //调试与状态记录：通过结构体sspi_info记录通信状态（如CS高低次数、SCK计数等）。
    //片选触发统计
    uint32_t cs_l;
    uint32_t cs_h;

    //spi时序控制
    uint32_t sck;
    uint32_t start;
    uint8_t mosi;   //存储接收到的位
    uint8_t miso;   //存储待发送的位

    //发送缓冲区管理
    uint32_t s_sendMax; //需发送的总字节数
    uint32_t s_send;    //当前的发送位置
    uint8_t *s_buf;     //发送缓冲区的指针

    //接收缓冲区管理
    uint32_t s_recvMax; //缓冲区的最大容量
    uint32_t s_recv;    //当前接收字节数
    uint8_t *r_buf;     //接收缓冲区指针
    
    //debug
    uint32_t sck_h_debug;
    uint32_t sck_l_debug;
    uint32_t sck_l_debug2;
}sspi_info;


typedef struct{
    uint16_t type;
    uint16_t len;
}queueMsgHdrInfo;// this

typedef struct{
    queueMsgHdrInfo hdr;
    uint8_t buf[SPI_BUF_MAX];
}queueRxMsgInfo;  //this three struct should be transmitted to serve.h

typedef struct{
    queueMsgHdrInfo hdr;
    uint8_t buf[SPI_BUF_TX_MAX];
}queueTxMsgInfo;

#define SPI_BUF_MAX  (300) //too
#define SPI_BUF_TX_MAX  (50)



void SPI_Transmit(uint8_t *pData, uint16_t size);
void SPI_Receive(uint8_t *pRxData, uint16_t size);


#endif



//核心目标：通过GPIO模拟SPI从设备通信
//通信流程：
//主设备          从设备（本代码）
//CLK  →         (SCK引脚检测)
//CS   →         (片选触发)
//MOSI →         (接收数据)
//MISO ←         (发送数据)
//关键实现：
//通过GPIO中断响应SPI时序
//实现基本的全双工通信机制
//使用FreeRTOS队列进行消息缓冲















