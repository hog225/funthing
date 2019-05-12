#include "HCSR04.h"

Distance_Sensor::Distance_Sensor(int t_pin, int e_pin)
{
	this->t_pin = t_pin;
	this->e_pin = e_pin;
	pinMode(t_pin, OUTPUT);
	pinMode(e_pin, INPUT);
}

void Distance_Sensor::HC_SR04_generate_signal()
{
	digitalWrite(this->t_pin, LOW);  // Added this line
	sleep_micro(2); // Added this line
	digitalWrite(this->t_pin, HIGH);
	sleep_micro(10); // Added this line
	digitalWrite(this->t_pin, LOW);
}

long Distance_Sensor::HC_SR04_decode_signal()
{
	long duration,distance;
	duration = pulseIn(this->e_pin, HIGH);
	distance = (duration/2) / 29.1;
	return distance;
}

bool Distance_Sensor::HC_SR04_check_distance(unsigned char id, long distance)
{

	if(distance <= RESTRIC_DISTANCE)
	{
	    Serial.printf("distance[%d] = ",id);
	    Serial.println(distance);
		return true;
	}
	else
	{
		return false;
	}
}
