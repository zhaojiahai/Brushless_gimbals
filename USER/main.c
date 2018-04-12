#include "sys.h"
#include "uart4.h"
#include "mpu6050.h"
#include "moto.h"
#include <stdlib.h>
#include "ahrs.h"
#include "tim3.h"
#include "taskrun.h"
#include "delay.h"
#include "config.h"

uint8_t SYS_INIT_OK=0;

int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	SYS_INIT_OK=0;									//初始化完成
	UART4_Init(500000);								//串口初始化为115200
	
	TIM3_Int_Init(1000-1,72-1);
	AHRS_Data_Init();								//姿态解算数据初始化
	if(!MPU_Init())									//MPU6050初始化
	{
		printf("\r\nMPU6050 init success...\r\n");
	}
	else
	{
		printf("\r\nMPU6050 init failed...\r\n");
	}
//	MotorInit();	//电机初始化
//	MotorPwmFlash(0,0,0,0);
	GetConfig();
	printf("config: \r\n");
	printf("gyro--->\tX: %d\tY: %d\tZ: %d\r\nAcc --->\tX: %d\tY: %d\tZ: %d\r\n",
		mpu6050.Gyro_Offset.x,mpu6050.Gyro_Offset.y,mpu6050.Gyro_Offset.z,
		mpu6050.Acc_Offset.x,mpu6050.Acc_Offset.y,mpu6050.Acc_Offset.z
	);
	SYS_INIT_OK=1;									//初始化完成
	
	
	
	while(1)
	{
		if(tim3_int)
		{
			tim3_int=0;
			TaskRun();
		}
	}
} 

