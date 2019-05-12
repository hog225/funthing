
#ifndef __WIFI_H_
#define __WIFI_H_

#include "ESP8266.h"
#define SSID            ""
#define PASSWORD        ""
#define HOST_NAME       ""
#define D_NAME          ""
#define HOST_PORT  	(9898)



class wifi : public ESP8266
{
	public:
		wifi(HardwareSerial &uart = Serial, uint32_t _baud = 9600);
		char tcpSingleConnect();

};

#endif
