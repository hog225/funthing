
#ifndef __THREADFUNC_H_
#define __THREADFUNC_H_

#include "Arduino.h"
#include "mthread.h"
#include "Radioprocess.h"
#include "HCSR04.h"
#include "wifi.h"
#include "LED.h"

#define TOTAL_THREAD_COUNT 10;

class HCSR04Thread : public Thread
{
	public:
		HCSR04Thread(unsigned char id,Distance_Sensor &_HC);
		Distance_Sensor *HC;
	protected:
		bool loop();


};

class ControlTankThread : public Thread
{
	public:
		ControlTankThread(int id);
	protected:
		bool loop();

};

class WIFIThread : public Thread
{
	public:
		WIFIThread(unsigned char id, wifi &_WIFI);
		wifi *WIFI;
	protected:
		bool loop();


};

class LEDThread : public Thread
{
	public:
		int mode;
		LEDThread(unsigned char id,int _mode, bool initial_val);
	protected:
		bool loop();

};

class ControlThread : public Thread
{
	public:
		ControlThread(int id);
	protected:
		bool loop();

};

#endif
