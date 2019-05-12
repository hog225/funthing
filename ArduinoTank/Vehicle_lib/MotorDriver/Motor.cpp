#include "Motor.h"

L298N::L298N()
{
	m_in1 =2;
	m_in2 =3;
	m_As = 9;

	m_in3 =4;
	m_in4=5;
	m_Bs=10;

	m_Aspeed = 0;
	m_Bspeed=0;
	Serial.begin(9600);
}

void L298N::M_Control(int output, int direction, int _speed)
{
	unsigned char pin1=0;
	unsigned char pin2=0;
	unsigned char speed_pin=0;
	if (_speed > 0xFF)
		_speed = 0xFF;

	if (output == OUTA)
	{
		pin1 =      m_in1;
		pin2 =      m_in2;
		speed_pin = m_As;
	}
	else
	{
		pin1 =      m_in3;
		pin2 =      m_in4;
		speed_pin = m_Bs;
	}

	switch (direction){
		case FORWARD:// forward
			analogWrite(speed_pin, _speed);//Sets speed variable via PWM
			digitalWrite(pin1, LOW);
			digitalWrite(pin2, HIGH);
			break;
		case BACKWARD:// backward
			analogWrite(speed_pin, _speed);//Sets speed variable via PWM
			digitalWrite(pin1, HIGH);
			digitalWrite(pin2, LOW);
			break;
		case STOP:// stop
			analogWrite(speed_pin, _speed);//Sets speed variable via PWM
			digitalWrite(pin1, LOW);
			digitalWrite(pin2, HIGH);
			break;


	}

}

void L298N::tankGO(int _speed)
{
	M_Control(OUTA,FORWARD,_speed);
	M_Control(OUTB,FORWARD,_speed);
}

void L298N::tankLEFT(int _speed)
{
	M_Control(OUTB,FORWARD,_speed);
	M_Control(OUTA,FORWARD,0);
}

void L298N::tankRIGHT(int _speed)
{
	M_Control(OUTB,FORWARD,0);
	M_Control(OUTA,FORWARD,_speed);
}

void L298N::tankBACK(int _speed)
{
	M_Control(OUTA,BACKWARD,_speed);
	M_Control(OUTB,BACKWARD,_speed);
}

void L298N::tankTURN(int _speed)
{
	M_Control(OUTB,FORWARD,_speed);
	M_Control(OUTA,BACKWARD,_speed);
}


void L298N::tankWriteMotorInfo(int direction,int speed)
{
	switch (direction){
		case GO:
			tankGO(speed);
			break;
		case RIGHT:
			tankRIGHT(speed);
			break;
		case LEFT:
			tankLEFT(speed);
			break;
		case BACK:
			tankBACK(speed);
			break;
		default :
			tankBACK(0);
			break;
	}
}

void L298N::tankWriteIndiMotorInfo(int fb_di, int rl_di, int m_speed)
{
	int speed_buf[2] = {50,100};
	int speed1=0;
	int speed2=0;
	bool val=0;
	if(rl_di > 0)
	{
		if(fb_di > 0)
		{
			val = rl_di - LEFT;
			speed1 = speed_buf[val]; //Left motor
			speed2 = speed_buf[!val];	//right motot
			Serial.printf("diag %d/%d ,speed1 = %d, speed2 = %d\n",fb_di,rl_di, speed1, speed2);
		}
		else
		{
			val = rl_di - LEFT;
			speed1 = (speed_buf[val] - 50)*2; //Left motor
			speed2 = (speed_buf[!val] - 50)*2;	//right motot
			Serial.printf("Right/Left %d/%d ,speed1 = %d, speed2 = %d\n",fb_di,rl_di, speed1, speed2);
		}

	}
	else
	{

		speed1 = m_speed;
		speed2 = m_speed;
		Serial.printf("Forward/Backward %d/%d ,speed1 = %d, speed2 = %d\n",fb_di,rl_di, speed1, speed2);
	}

	M_Control(OUTB,fb_di,speed1);
	M_Control(OUTA,fb_di,speed2);
}

void L298N::tankWriteTurnMotorInfo(int di1, int di2, int m_speed)
{
	Serial.printf("Turn %d,%d \n",di1,di2);
	M_Control(OUTB,di1,m_speed);
	M_Control(OUTA,di2,m_speed);
}


M_PARA L298N::GetMotorPara()
{
	String str;
	char motor_out;
	char direction;
	char speed[5];
	char string[64];
	M_PARA motor_para;

	int index;
	str = Serial.readString();
	Serial.println(str);

	motor_out = str.charAt(0);
	index = str.indexOf(';',0);
	direction = str.charAt(index+1);
	index = str.indexOf(';',index+1);
	str.toCharArray(speed,5,index+1);

	motor_para._motor_out 	= motor_out - '0';
	motor_para._direction 	= direction - '0';
	motor_para._speed 		= atoi(speed);
	sprintf(string,"M_out:direction:speed=%d:%d:%d\n"\
			,motor_para._motor_out,motor_para._direction ,motor_para._speed);
	Serial.print(string);

	return motor_para;
}


