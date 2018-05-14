#include "commander.h"
#include "sys.h"
#include "uart4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ahrs.h"
#include "delay.h"
#include "taskrun.h"
#include "pid.h"
void Commander_Task(void)
{
	uint8_t index = UART4_RX_BUF[0];
	char temp[20]="";
	switch(index)
	{
		case 'b':
			printf("\r\nSystem Reboot\r\n");
			delay_ms(5);
			SYSTEM_Reset();
		break;
		
		case 'g':
			MPU6050_GetGyro_Offset();
		break;
		
		case 'a':
			MPU6050_GetAcc_Offset();
		break;
		
		case 'd':
			send_anodt=!send_anodt;
		break;
		
		case 'l':
			log_attitude=!log_attitude;
		break;
		
		case 'R':	//@HackOS: ROLL_PID参数设置,格式: "R10.3 0.21 0.15" 必须保证每个参数加小数点为4位,用空格隔开
			memset(temp,0,20);
			strncpy(temp,(const char *)&UART4_RX_BUF[1],4);
			pid.roll.kp=atof(temp);
			memset(temp,0,20);
			strncpy(temp,(const char *)&UART4_RX_BUF[6],4);
			pid.roll.ki=atof(temp);
			memset(temp,0,20);
			strncpy(temp,(const char *)&UART4_RX_BUF[11],5);
			pid.roll.kd=atof(temp);
			memset(temp,0,20);
			strncpy(temp,(const char *)&UART4_RX_BUF[17],3);
			pid.roll.power=(float)atoi(temp);
			printf("ROLL P:%f\tI:%f\tD:%f\tPower:%f\r\n",pid.roll.kp,pid.roll.ki,pid.roll.kd,pid.roll.power);
		break;
		
		case 'S':
			PID_Show();
		break;
			
		case 't':	//@HackOS: PID测试任务
			pid_test=!pid_test;
		break;
		
		default:
			printf("unknown command\r\n");
		break;
	}
	UART4_RX_STA=0;
}
	
