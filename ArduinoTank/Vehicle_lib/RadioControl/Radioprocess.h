
#ifndef __RADIOPROCESS_H_
#define __RADIOPROCESS_H_

#include "Arduino.h"
#include "Motor.h"
#include <LinkedList.h>

//Phone bluetooth App
#define S_STOP 	0
#define S_GO		13
#define S_BACK 	14
#define S_LEFT		15
#define S_RIGHT	16
#define S_LTURN	64
#define S_RTURN	63

#define L_1		66
#define R_1		67
#define L_2		68
#define R_2		69

#define OVRIDE_TIME 200
#define OVRIDE_COUNT 10000
#define USE_SOFTWARE_SERIAL


//#define USE_SOFTWARE_SERIAL
#ifdef USE_SOFTWARE_SERIAL
#include "SoftwareSerial.h"
#endif

class BlueTooth
{
	public :

	unsigned long overide_start_time;
	int old_di;

	unsigned int loop_count;
	int start_val;

#ifdef USE_SOFTWARE_SERIAL
	BlueTooth(SoftwareSerial &uart);
#else /* HardwareSerial */
	BlueTooth(HardwareSerial &uart);
#endif

	uint32_t recv_str(unsigned char *buffer, int buffer_size,int timeout, char eof);
	uint32_t recvFromPhone(unsigned char *buffer);
	int returnDataForMotor(bool *restric, unsigned char input);
	int overrideDataTrigger( bool on);
	bool modifyRecvData(unsigned char *direction_code, LinkedList<int> *_list);
	String printDirection(unsigned char input);

	//BT_GetDataByte




#ifdef USE_SOFTWARE_SERIAL
    SoftwareSerial *m_puart;
#else
    HardwareSerial *m_puart;
#endif
};


#endif
