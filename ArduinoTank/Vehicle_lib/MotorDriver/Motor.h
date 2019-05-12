
#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "Arduino.h"
#define OUTA	1
#define OUTB	2

#define STOP		0
#define FORWARD  	1
#define BACKWARD	2

#define GO			1
#define BACK		2
#define LEFT		3
#define RIGHT		4
#define L_TURN		5
#define R_TURN		6

struct Motor_para{
	int _motor_out;
	int _direction;
	int _speed;
};
typedef struct Motor_para M_PARA;

class L298N
{
	public :
		unsigned char m_in1;
		unsigned char m_in2;
		unsigned char m_As;
		unsigned char m_Aspeed;

		unsigned char m_in3;
		unsigned char m_in4;
		unsigned char m_Bs;
		unsigned char m_Bspeed;



		L298N();
		void M_Control(int output, int direction, int _speed);
		void tankGO(int _speed);
		void tankLEFT(int _speed);
		void tankRIGHT(int _speed);
		void tankBACK(int _speed);
		void tankTURN(int _speed);
		void tankWriteMotorInfo(int direction,int speed);
		void tankWriteIndiMotorInfo(int fb_di, int rl_di, int m_speed);
		void tankWriteTurnMotorInfo(int di1, int di2, int m_speed);
		M_PARA GetMotorPara(void);
};


#endif
