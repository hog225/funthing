#include "Radioprocess.h"
#include "LED.h"

extern int _resume_id;
extern int _turn_off;
extern LED ledstrip;
extern unsigned char _color;

#ifdef USE_SOFTWARE_SERIAL
BlueTooth::BlueTooth(SoftwareSerial &uart): m_puart(&uart)
{
	m_puart->begin(9600);
	overide_start_time = 0;
	old_di = 0;
	loop_count = 0;
	start_val = 0;
}
#else
BlueTooth::BlueTooth(HardwareSerial &uart): m_puart(&uart)
{
	m_puart->begin(9600);
	overide_start_time = 0;
	old_di = 0;
	loop_count = 0;
	start_val = 0;


}
#endif


uint32_t BlueTooth::recv_str(unsigned char *buffer, int buffer_size,int timeout, char eof)
{
    char a;
    unsigned long start;
    uint32_t i = 0;

    if (buffer == NULL) {
        return 0;
    }

	start = millis();
	while (millis() - start < timeout) {
		while(m_puart->available() > 0 && i < buffer_size) {
			a = m_puart->read();
			buffer[i++] = a;
		}
		if (buffer[i] == eof || i == buffer_size) {
			//rx_empty();

			return i;
		}
	}
    return i;
}

uint32_t BlueTooth::recvFromPhone(unsigned char *buffer)
{
	return recv_str(buffer, 3, 15,'U');
}

bool BlueTooth::modifyRecvData(unsigned char *direction_code, LinkedList<int> *_list)
{

	unsigned char buffer[6];
	int direction=0;
	int i;
	int listsize=0;
	int tmpdirection=0;
	memset(buffer,0,sizeof(buffer));
	if (m_puart->available())
		recvFromPhone(buffer);

	if (buffer[2] == 'D')
	{
		direction = atoi((const char*)buffer);
		if(((direction >= S_GO) && (direction <=S_RIGHT)) \
			|| ((direction >= S_RTURN) && (direction <=S_LTURN)) \
			|| ((direction >= L_1) && (direction <=R_2)))
		{
			*direction_code = direction;
			Serial.print(printDirection(direction));
			Serial.println("  -- Pushed  --  ");
			if (_list->size() < 2)
				_list->add(direction);
			else
			{
				Serial.println("_list size error");
				return false;
			}

			return true;
		}
		else
		{
			Serial.print("direction range error ");
			Serial.println(direction);

			return false;
		}

	}

	else if (buffer[2] == 'U')
	{
		direction = atoi((const char*)buffer);
		if(((direction >= S_GO) && (direction <=S_RIGHT)) \
				|| ((direction >= S_RTURN) && (direction <=S_LTURN)) \
				|| ((direction >= L_1) && (direction <=R_2)) )
		{
			*direction_code = direction;
			listsize = _list->size();
			if (listsize != 0)
			{
				for (i=0; i<listsize; i++)
				{
					if(_list->get(i) == direction)
					{
						_list->remove(i);
						break;
					}
				}
				if (i == listsize)
				{
					Serial.println("invalid release value \n");
					return false;
				}
				else
				{
					Serial.print(printDirection(direction));
					Serial.println("  -- Released  --  ");
					if ((direction >= L_1) && (direction <=R_2))
					{
						if(direction == L_1)
						{
							Serial.println("Resume LED Thread");
							_turn_off = 0;
							_resume_id = 4;//led

							_color++;
							if (_color == MAX_COL_DIP)
								_color = 0;

						}
						else if(direction == R_1)
						{
							Serial.println("Stop LED Thread");
							_turn_off = 1;
						}
					}
					*direction_code = 0;// STOP
					return true;
				}
			}
			else
			{
				Serial.println("_list size error \n");
				return false;
			}

			return true;
		}
		else
		{
			Serial.print("direction range error ");
			Serial.println(direction);
			return false;
		}

	}

	if (_list->size() > 1)
	{
		tmpdirection =  _list->get(0);
		_list->set(0,_list->get(1));
		_list->set(1,tmpdirection);
		*direction_code = _list->get(0);
		return true;
	}
	*direction_code = 0xFF;


}

int BlueTooth::returnDataForMotor(bool* restric,unsigned char input)
{
	switch (input){
		case S_GO:
			if((restric[0] == true))
				return STOP;
			else
				return GO;
		case S_BACK:
			if((restric[1] == true))
				return STOP;
			else
				return BACK;
		case S_LEFT:
			return LEFT;
		case S_RIGHT:
			return RIGHT;
		case S_LTURN:
			return L_TURN;
		case S_RTURN:
			return R_TURN;
		case S_STOP:
			return STOP;
		case L_1:
			return LEFT_LED;
		case L_2:
			return RIGHT_LED;
		case R_1:
			return FRONT_LED;
		case R_2:
			return BACK_LED;
		default:
			return STOP;

	}
}

String BlueTooth::printDirection(unsigned char input)
{

	switch (input){
		case S_GO:
		case GO:
			return "GO";
		case S_BACK:
		case BACK:
			return "BACK";
		case S_LEFT:
		case LEFT:
			return "LEFT";
		case S_RIGHT:
		case RIGHT:
			return "RIGHT";
		case S_STOP:
			return "STOP";
		default:
			return "STOP";

	}
}



int BlueTooth::overrideDataTrigger(bool on)
{
	if(on != 0)
	{
		overide_start_time = millis();
		return 1;
	}
	return 0;

}
