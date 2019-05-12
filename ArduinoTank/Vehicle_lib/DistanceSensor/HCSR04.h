
#ifndef __HCSR04_H_
#define __HCSR04_H_

#include "Arduino.h"
#include "mthread.h"

#define		TRIG_PIN1				12
#define 		ECHO_PIN1				13

#define		TRIG_PIN2				7
#define 		ECHO_PIN2				6

#define 		RESTRIC_DISTANCE		10



class Distance_Sensor : public Thread
{
	public :
		Distance_Sensor(int t_pin, int e_pin);
		int t_pin,e_pin;
		void HC_SR04_generate_signal();
		long HC_SR04_decode_signal();
		bool HC_SR04_check_distance(unsigned char id, long distance);


};

#endif
