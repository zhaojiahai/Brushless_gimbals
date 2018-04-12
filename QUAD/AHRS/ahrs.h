#ifndef __AHRS_H_
#define __AHRS_H_
#include "config.h"
#define SAMPLE_HALF_T 0.0005f	//采样周期的一半，单位：s
#define FILTER_LENGTH 20		//滑动滤波窗口长度

#define GYRO_G 	0.0610351f		//角速度变成度/S   此参数对应陀螺2000度每秒  Gyro_G=1/16.375=0.0610687
#define GYRO_GR	0.0010652f		//角速度变成弧度/S	此参数对应陀螺2000度每秒

#define IMU_KP 2.0f     		//比例
#define IMU_KI 0.005f 			//积分

extern xyz_s16_t Acc_Avg;		//滑动滤波后加速度计值
extern xyz_f_t Angle;			//数据融合计算出的角度
extern xyz_s16_t Acc_Buf[FILTER_LENGTH];	//滑动滤波数组

extern void ACC_Flitter(void);
extern void AHRS_Data_Init(void);
extern void IMU_Update(void);
extern void Prepare_Data(void);

#endif
