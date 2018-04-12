#ifndef __KALMAN_H_
#define __KALMAN_H_

void kalman_Init(double process_noise, double sensor_noise, double estimated_error, double intial_value);
double kalman_getFilteredValue(double measurement);
void setParameters(double process_noise, double sensor_noise, double estimated_error);
double getProcessNoise(void);
double getSensorNoise(void);
double getEstimatedError(void);

#endif

