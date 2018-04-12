#include "config.h"
#include "stmflash.h"
#include <stdio.h>
#include "mpu6050.h"
#include "delay.h"
config_t config;

void GetConfig(void)
{
	//@HackOS: 从flash读取配置信息
	STMFLASH_Read(CONFIG_SAVE_ADDR,(uint16_t *)&config,sizeof(config)/2);
	
	//@HackOS: 恢复加速度校准值
	mpu6050.Acc_Offset.x=config.Acc_Offset.x;
	mpu6050.Acc_Offset.y=config.Acc_Offset.y;
	mpu6050.Acc_Offset.z=config.Acc_Offset.z;
	
	//@HackOS: 恢复陀螺仪校准值
	mpu6050.Gyro_Offset.x=config.Gyro_Offset.x;
	mpu6050.Gyro_Offset.y=config.Gyro_Offset.y;
	mpu6050.Gyro_Offset.z=config.Gyro_Offset.z;
	
}

void SetConfig(void)
{
	STMFLASH_Write(CONFIG_SAVE_ADDR,(uint16_t *)&config,sizeof(config)/2);
}


void MPU6050_GetGyro_Offset(void)
{
	uint8_t i=100;
	double temp_x=0;
	double temp_y=0;
	double temp_z=0;
	printf("\r\nStart GetOffset---->gyro\r\n");
	while(i--)
	{
		MPU_Get_Gyroscope(&(mpu6050.Gyro_I16.x),&(mpu6050.Gyro_I16.y),&(mpu6050.Gyro_I16.z));
		temp_x+=mpu6050.Gyro_I16.x;
		temp_y+=mpu6050.Gyro_I16.y;
		temp_z+=mpu6050.Gyro_I16.z;
		printf(".");
		delay_ms(5);
	}
	mpu6050.Gyro_Offset.x=(int16_t)(temp_x/100.0f);
	mpu6050.Gyro_Offset.y=(int16_t)(temp_y/100.0f);
	mpu6050.Gyro_Offset.z=(int16_t)(temp_z/100.0f);
	printf("\r\nEnd GetOffset---->gyro\r\n");
	printf("Gyro_Offset.x --> %d\r\n",mpu6050.Gyro_Offset.x);
	printf("Gyro_Offset.y --> %d\r\n",mpu6050.Gyro_Offset.y);
	printf("Gyro_Offset.z --> %d\r\n",mpu6050.Gyro_Offset.z);
	config.Gyro_Offset.x=mpu6050.Gyro_Offset.x;
	config.Gyro_Offset.y=mpu6050.Gyro_Offset.y;
	config.Gyro_Offset.z=mpu6050.Gyro_Offset.z;
	
	STMFLASH_Write(CONFIG_SAVE_ADDR,(uint16_t *)&config,sizeof(config)/2);
	
}
void MPU6050_GetAcc_Offset(void)
{
	uint8_t i=100;
	double temp_x=0;
	double temp_y=0;
	double temp_z=0;
	printf("\r\nStart GetOffset---->Acc\r\n");
	printf("\r\n\r\nStart X\r\n\r\n");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("dont move\r\n");
	while(i--)
	{
		MPU_Get_Accelerometer(&(mpu6050.Acc_I16.x),&(mpu6050.Acc_I16.y),&(mpu6050.Acc_I16.z));
		temp_x+=mpu6050.Acc_I16.x;
		printf("X");
		delay_ms(5);
	}
	printf("\r\n\r\nStart Y\r\n\r\n");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("dont move\r\n");
	i=100;
	while(i--)
	{
		MPU_Get_Accelerometer(&(mpu6050.Acc_I16.x),&(mpu6050.Acc_I16.y),&(mpu6050.Acc_I16.z));
		temp_y+=mpu6050.Acc_I16.y;
		printf("Y");
		delay_ms(5);
	}
	printf("\r\n\r\nStart Z\r\n\r\n");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("dont move\r\n");
	i=100;
	while(i--)
	{
		MPU_Get_Accelerometer(&(mpu6050.Acc_I16.x),&(mpu6050.Acc_I16.y),&(mpu6050.Acc_I16.z));
		temp_z+=mpu6050.Acc_I16.z;
		printf("Z");
		delay_ms(5);
	}
	
	
	mpu6050.Acc_Offset.x=(int16_t)(temp_x/100.0f)-4096;
	mpu6050.Acc_Offset.y=(int16_t)(temp_y/100.0f)-4096;
	mpu6050.Acc_Offset.z=(int16_t)(temp_z/100.0f)-4096;
	printf("\r\nEnd GetOffset---->Acc\r\n");
	printf("Acc_Offset.x --> %d\r\n",mpu6050.Acc_Offset.x);
	printf("Acc_Offset.y --> %d\r\n",mpu6050.Acc_Offset.y);
	printf("Acc_Offset.z --> %d\r\n",mpu6050.Acc_Offset.z);
	config.Acc_Offset.x=mpu6050.Acc_Offset.x;
	config.Acc_Offset.y=mpu6050.Acc_Offset.y;
	config.Acc_Offset.z=mpu6050.Acc_Offset.z;
	
	STMFLASH_Write(CONFIG_SAVE_ADDR,(uint16_t *)&config,sizeof(config)/2);
}


