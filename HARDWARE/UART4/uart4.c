#include "sys.h"
#include "uart4.h"
#include "commander.h"
//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
//////////////////////////////////////////////////////////////////////////////////

//@HackOS: 采用发送缓冲区来发送数据,带发送中断
u8 TxBuffer[0xff];
u8 TxCounter=0;
u8 count=0; 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
_sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while((UART4->SR&0X40)==0);//循环发送,直到发送完毕   
	UART4->DR=ch;
	return ch;
}
#endif

uint8_t UART4_Put_Char(unsigned char ch)
{
	while((UART4->SR&0X40)==0);//循环发送,直到发送完毕   
	UART4->DR=ch;
	return ch;
}


//@HackOS: USART4
//串口4中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 UART4_RX_BUF[UART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART4_RX_STA = 0;     //接收状态标记

void UART4_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    //Set USART4 Tx (PC10) as AF push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //Set USART4 Rx (PC11) as input pull down
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART4, &USART_InitStructure);

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//Enable UART4 Receive interrupt
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//Enable USART4
	USART_Cmd(UART4, ENABLE);

}


void UART4_IRQHandler(void)                	//串口1中断服务程序
{
	
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();
#endif
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(UART4);	//读取接收到的数据
		
		if((UART4_RX_STA & 0x8000) == 0) //接收未完成
		{
			if(UART4_RX_STA & 0x4000) //接收到了0x0d
			{
				if(Res != 0x0a) { UART4_RX_STA = 0; } //接收错误,重新开始
				else	//接收完成了
				{
					UART4_RX_STA |= 0x8000;
					Commander_Task();
				}
			}
			else //还没收到0X0D
			{
				if(Res == 0x0d) { UART4_RX_STA |= 0x4000; }
				else
				{
					UART4_RX_BUF[UART4_RX_STA & 0X3FFF] = Res ;
					UART4_RX_STA++;
					if(UART4_RX_STA > (UART_REC_LEN - 1)) { UART4_RX_STA = 0; } //接收数据错误,重新开始接收
				}
			}
		}
	}
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();
#endif
}

void UART4_Send(uint8_t *data,uint32_t length)
{
	uint32_t i=0;
	for(i=0;i<length;i++)
	{
		UART4_Put_Char(data[i]);
	}
	
}
