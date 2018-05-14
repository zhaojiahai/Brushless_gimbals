#include "pid.h"
#include "ahrs.h"
#include "config.h"
#include <stdio.h>
//@HackOS: Angle.x,Angle.y,Angle.z===>pitch roll yaw

pid_group_t pid=
{
//	P   I  D    POWER
	0.03 , 0, 0.20, 0,			//@HackOS: ROLL
	10, 0, 0.1, 0,			//@HackOS: PITCH
	10, 0, 0.1, 0			//@HackOS: YAW
};
float expect_roll=0.0F;	//@HackOS: Roll期望值
float pid_roll_current;		//@HackOS: Roll轴上次PID输出值
float pid_roll_last;	//@HackOS: 上次PID值

float pid_roll_out;



//@HackOS: PID效果测试
void PID_Test(void)
{
	static float real=-50;			//@HackOS: PID效果测试
	static int i=0;
	static float last_err=0;
	
	float error=expect_roll-real;
	
	float kp=pid.roll.kp*error;
	
	float kd=pid.roll.kd*(error-last_err);
	
	last_err=error;
	
	pid_roll_out=kp+kd;
	
	real+=0.1*pid_roll_out;
	printf("%d,%f\r\n",i++,real);
}

void PID_Roll(void)
{
	static float last_err=0;
	static float iTerm=0;
	float kp,ki,kd,error;
	
	error=expect_roll-Angle.y;
	
	kp=pid.roll.kp*error;
	
	iTerm+=error;
	
	ki=pid.roll.ki*iTerm;
	
	kd=pid.roll.kd*(error-last_err);
	
	last_err=error;
	
	pid_roll_current=kp+ki+kd;
	
	
	pid_roll_out=pid_roll_current;
//	pid_roll_out=pid_roll_current-pid_roll_last;
//	
//	pid_roll_last=pid_roll_current;
	
//	printf("%f,%f,%f\r\n",Angle.y,error,pid_roll_out);
	
}

void PID_Show(void)
{
	printf("Roll :\tP:\t%f\tI:\t%f\tD:\t%f\r\n",pid.roll.kp,pid.roll.ki,pid.roll.kd);
	printf("Pitch:\tP:\t%f\tI:\t%f\tD:\t%f\r\n",pid.pitch.kp,pid.pitch.ki,pid.pitch.kd);
	printf("Yaw  :\tP:\t%f\tI:\t%f\tD:\t%f\r\n",pid.yaw.kp,pid.yaw.ki,pid.yaw.kd);
}

