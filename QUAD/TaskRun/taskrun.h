#ifndef __TASK_RUN_
#define __TASK_RUN_
#include "stm32f10x.h"
extern void TaskRun(void);
extern uint8_t send_anodt;		//发送到匿名上位机
extern uint8_t log_attitude;	//通过串口记录姿态信息
#endif
