#include "commander.h"
#include "sys.h"
#include "uart4.h"
#include <stdio.h>
#include "ahrs.h"
#include "delay.h"
#include "taskrun.h"
void Commander_Task(void)
{
	uint8_t index = UART4_RX_BUF[0];
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
		
		default:
		break;
	}
	UART4_RX_STA=0;
}
	
