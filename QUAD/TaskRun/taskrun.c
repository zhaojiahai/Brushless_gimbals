#include "taskrun.h"
#include "stm32f10x.h"
#include "ANO_DT.h"
#include "mpu6050.h"
#include "ahrs.h"
#include <stdio.h>
#include <math.h>
/**
  ******************************************************************************
  * @fuction    
  * @author  赵加海
  * @date     16-02-2017
  * @param   
  * @retval   
  * @brief  1000HZ任务函数,由Systick调用
  * @CopyRight Sunic-Ocean
  ******************************************************************************
**/
extern uint8_t SYS_INIT_OK;
uint8_t send_anodt=0;		//发送到匿名上位机
uint8_t log_attitude=0;		//通过串口记录姿态信息
void TaskRun(void)
{
	static uint32_t i=0;
	static u16 ms1 = 0,ms2 = 0,ms5 = 0,ms10 = 0,ms100 = 0,ms1000 = 0;	//中断次数计数器
	if(!SYS_INIT_OK)				//未初始化		
		return;
	//每次中断都执行,0.5ms
	ms1++;
	ms2++;
	ms5++;
	ms10++;
	ms100++;
	ms1000++;
	//@HackOS: 1000HZ
	if(ms1==1)				//每两次中断执行一次,1ms
	{
		ms1=0;
		Prepare_Data();				//获取数据并滤波
		IMU_Update();				//姿态解算
		if(send_anodt)
			ANO_DT_Data_Exchange();		//上传至上位机
	}
	//@HackOS: 500HZ
	if(ms2==2)				//每四次中断执行一次,2ms
	{
		ms2=0;
	}
	//@HackOS: 200HZ
	if(ms5==5)
	{
		ms5=0;					//每十次中断执行一次,5ms
	}
	//@HackOS: 100HZ
	if(ms10==10)
	{
		ms10=0;					//每二十次中断执行一次,10ms
		if(log_attitude)
		{
			i++;
			printf("%5d,%-5.2f,%-5.2f,%-5.2f\r\n",i,Angle.x,Angle.y,Angle.z);
		}
	}
	if(ms100==10)
	{
		ms100=0;
	}
	if(ms1000==1000)
	{
		ms1000=0;
	}
		
}
