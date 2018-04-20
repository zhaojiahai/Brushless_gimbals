#include "pid.h"
#include "ahrs.h"
#include "config.h"
//@HackOS: Angle.x,Angle.y,Angle.z===>pitch roll yaw

pid_group_t pid=
{
//	P   I  D    damp
	10, 0, 0.1, 0,			//@HackOS: ROLL
	10, 0, 0.1, 0,			//@HackOS: PITCH
	10, 0, 0.1, 0			//@HackOS: YAW
};
float expect_roll=0;	//@HackOS: Roll期望值
float pid_roll_out;		//@HackOS: Roll轴PID输出值
void PID_Roll(void)
{
	static float last_err=0;
	
	float error=expect_roll-Angle.y;
	
	float kp=pid.roll.kp*error;
	
	float kd=pid.roll.kd*(error-last_err);
	
	last_err=error;
	
	pid_roll_out=kp+kd;
	
}

