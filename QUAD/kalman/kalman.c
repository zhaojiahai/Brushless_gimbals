#include "kalman.h"

static double q; //process noise covariance
static double r; //measurement noise covariance
static double x; //value
static double p; //estimation error covariance
static double k; //kalman gain

void kalman_Init(double process_noise, double sensor_noise, double estimated_error, double intial_value)
{
	/* The variables are x for the filtered value, q for the process noise,
		r for the sensor noise, p for the estimated error and k for the Kalman Gain.
		The state of the filter is defined by the values of these variables.

		The initial values for p is not very important since it is adjusted
		during the process. It must be just high enough to narrow down.
		The initial value for the readout is also not very important, since
		it is updated during the process.
		But tweaking the values for the process noise and sensor noise
		is essential to get clear readouts.

		For large noise reduction, you can try to start from: (see http://interactive-matter.eu/blog/2009/12/18/filtering-sensor-data-with-a-kalman-filter/ )
		q = 0.125
		r = 32
		p = 1023 //"large enough to narrow down"
		e.g.
		myVar = Kalman(0.125,32,1023,0);
	*/
	q = process_noise;
	r = sensor_noise;
	p = estimated_error;
	x = intial_value; //x will hold the iterated filtered value
}

double kalman_getFilteredValue(double measurement)
{
	/* Updates and gets the current measurement value */
	//prediction update
	//omit x = x
	p = p + q;

	//measurement update
	k = p / (p + r);
	x = x + k * (measurement - x);
	p = (1 - k) * p;

	return x;
}

void setParameters(double process_noise, double sensor_noise, double estimated_error)
{
	q = process_noise;
	r = sensor_noise;
	p = estimated_error;
}

double getProcessNoise()
{
	return q;
}

double getSensorNoise()
{
	return r;
}

double getEstimatedError()
{
	return p;
}
