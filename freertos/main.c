#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

QueueHandle_t xKeyQueue;

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void UART_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

void UART_SendChar(char c)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

void UART_SendString(const char *str)
{
    while(*str) {
        UART_SendChar(*str++);
    }
}

// ===== SPI1 回环测试：PA5=SCK, PA6=MISO, PA7=MOSI =====
// 杜邦线短接 PA7 和 PA6，主机发什么收到什么 = SPI收发OK

void My_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    // PA5(SCK) PA7(MOSI)：复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA6(MISO)：浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;          // 主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;             // 模式0：空闲低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;              // 软件管理片选（回环不需要CS）
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // 72M/16=4.5MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI_SendByte(uint8_t byte)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  // 等发送缓冲空
    SPI_I2S_SendData(SPI1, byte);                                    // 发
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); // 等收到数据
    return SPI_I2S_ReceiveData(SPI1);                                // 收（回环=自己发的）
}

void vSpiTask(void *parameter)
{
    uint8_t test[5] = {0xAA, 0x55, 0xFF, 0x00, 0x12};
    char buf[48];
    uint8_t rx;
    int i;

    while(1){
        UART_SendString("SPI loop test...\r\n");
        for(i=0; i<5; i++){
            rx = SPI_SendByte(test[i]);
            sprintf(buf, "sent %02X got %02X %s\r\n", test[i], rx,
                    (rx==test[i]) ? "OK" : "FAIL");
            UART_SendString(buf);
        }
        vTaskDelay(1000);
    }
}

//- LED 任务：PC13 每 200ms 翻转一次
//- 按键任务：PB0 按下（下降沿）发队列消息
//- 串口任务：等队列消息，收到输出 "KEY OK"，超时 500ms 输出 "timeout"
//- 队列深度 10，存 uint8_t
//- LED 和串口任务优先级 1，按键任务优先级 2

void vLEDTask(void *parameter)
{
	while(1){
		GPIOC->ODR^=GPIO_Pin_13;
		vTaskDelay(200);
	}
}

void vKeyTask(void *parameter)
{
	uint8_t a=1;
	while(1){
		
		uint8_t KeyState=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
		if(KeyState==0&&a==1){
		uint8_t msg=1;
		xQueueSend(xKeyQueue,&msg,0);
		
		}
		a=KeyState;
		vTaskDelay(100);
	}
}

void vUartTask(void *parameter)
{
	uint8_t msg;
	
	while(1){
		if(xQueueReceive(xKeyQueue,&msg,500)==pdPASS){
			UART_SendString("KEY OK\r\n");
	}else
		{UART_SendString("timeout\r\n");
		}
	}
}
void vApplicationIdleHook(void)
{
    // 什么都不做
}

int main(void)
{
	LED_Init();
	KEY_Init();
	UART_Init();
	My_SPI_Init();
	xKeyQueue=xQueueCreate(10,sizeof(uint8_t));

	xTaskCreate(vLEDTask,"LED",128,NULL,1,NULL);
	xTaskCreate(vKeyTask,"KEY",128,NULL,2,NULL);
	xTaskCreate(vUartTask,"UART",256,NULL,1,NULL);
	xTaskCreate(vSpiTask,"SPI",256,NULL,1,NULL);
	vTaskStartScheduler();
	
	while(1);
}
















