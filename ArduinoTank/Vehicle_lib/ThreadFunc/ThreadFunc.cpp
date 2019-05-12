#include "ThreadFunc.h"


L298N M;
SoftwareSerial mySerial(8, 11); /* RX, TX */
BlueTooth BT(mySerial);
LED ledstrip;
unsigned char direction;
LinkedList<int> list;
bool control_restric_flag[2];

int _stop_id =0;
int _resume_id = 0;
int _turn_off = 0;
unsigned char _color;
unsigned char _old_color;


/**********************************WIFI**********************************/
WIFIThread::WIFIThread(unsigned char id, wifi &_WIFI) : WIFI(&_WIFI)
{
	this->thread_id = id;
	if(_WIFI.tcpSingleConnect())
		Serial.println("Success");
}



bool WIFIThread::loop()
{
    // Die if requested:
    if(kill_flag)
        return false;

    return true;
}
/************************************************************************/

/**********************************HCSR04**********************************/
HCSR04Thread::HCSR04Thread(unsigned char id,Distance_Sensor &_HC): HC(&_HC)
{
	this->thread_id = id;
}


bool HCSR04Thread::loop()
{
    // Die if requested:
    if(kill_flag)
        return false;

    HC->HC_SR04_generate_signal();
    long distance = HC->HC_SR04_decode_signal();
    control_restric_flag[(this->thread_id - 1)] = HC->HC_SR04_check_distance(this->thread_id, distance);
    //Serial.print("distance = ");
    //Serial.println(distance);
    sleep_milli(500);
    //count++;
    return true;
}
/************************************************************************/
/**********************************ControlTank**********************************/

ControlTankThread::ControlTankThread(int id)
{
    this->thread_id = id;
}

bool ControlTankThread::loop()
{

    // Die if requested:
    if(kill_flag)
        return false;
    //Serial.println(count);
	if(BT.modifyRecvData(&direction, &list))
	{
		if(list.size()>0)// direction 이 0 이면 일로 안들어감
		{
			int di = BT.returnDataForMotor(control_restric_flag,list.get(0));
			if ((di <= RIGHT) && (di > STOP))
			{
				if (BT.old_di - di != 0)
				{
					if(di <= 2)
						M.tankWriteIndiMotorInfo(di,BT.old_di,100);
					else
						M.tankWriteIndiMotorInfo(BT.old_di,di,100);

					BT.old_di = di;
				}
				else
				{
					if(di <= 2)
						M.tankWriteIndiMotorInfo(di,0,100);
					else
						M.tankWriteIndiMotorInfo(0,di,100);
				}
			}
			else if ((di <= R_TURN) && (di >= L_TURN))
			{
				if(di == L_TURN)
					M.tankWriteTurnMotorInfo(BACKWARD, FORWARD,100);
				else if(di == R_TURN)
					M.tankWriteTurnMotorInfo(FORWARD, BACKWARD,100);
				else{
					BT.old_di = 0;
					M.tankWriteMotorInfo(0,0);
				}
			}
			else if((di <= BACK_LED) && (di >= LEFT_LED))
			{
				//_resume_id = 4;
				//Serial.println("pushed  ");
			}
		}
		else
		{
			BT.old_di = 0;
			M.tankWriteMotorInfo(0,0);
		}

	}

    return true;
}

/************************************************************************/
/**********************************LED**********************************/
LEDThread::LEDThread(unsigned char id, int _mode,bool initial_val)
{
	this->thread_id = id;
	this->mode = _mode;
	pinMode(A4,OUTPUT);
	pinMode(A5,OUTPUT);
	if (initial_val == 0)
		this->pause();
}


bool LEDThread::loop()
{
    // Die if requested:
    if(kill_flag)
        return false;

    if(_turn_off == 1)
    {
    	ledstrip.Turn_Off();
    	digitalWrite(A4,LOW);
    	digitalWrite(A5,LOW);
    	_stop_id = 4;//led
    	return true;
    }

    if(this->mode == 0)
    	ledstrip.play_Bullet();
    else if (this->mode == 1)
    	ledstrip.play_HUE();
    else if(this->mode == 2)
    	ledstrip.play_CYON();
    else if (this->mode == 3)
    {
    	CRGB col = 0x0;
    	col = ledstrip.Get_Color(_color);
    	if(_old_color != _color)
    	{
    		_old_color = _color;
    		ledstrip.Turn_On(col);
    		digitalWrite(A4, HIGH);
    	    digitalWrite(A5, HIGH);
    	}


    }


    //count++;
    return true;
}


/************************************************************************/
/**********************************ControlThread**********************************/
ControlThread::ControlThread(int id)
{
	this->thread_id = id;

}

bool ControlThread::loop()
{
	if(kill_flag)
		return false;

	if (_stop_id)
	{
		main_thread_list->StopThread(_stop_id);
		_stop_id = 0;
	}
	else if(_resume_id)
	{

		main_thread_list->ResumeThread(_resume_id);
		_resume_id = 0;
	}
	return true;
}

