#include "wifi.h"


wifi::wifi(HardwareSerial &uart, uint32_t _baud) : ESP8266(uart,_baud)
{

}

char wifi::tcpSingleConnect()
{
	char res = 0;
    Serial.begin(9600);
    Serial.print(F("setup begin\r\n"));

    Serial.print(F("FW Version:"));
    Serial.println(getVersion().c_str());

    if (setOprToStationSoftAP()) {
        Serial.print(F("to station + softap ok\r\n"));
    } else {
        Serial.print(F("to station + softap err\r\n"));
    }

    if (joinAP(SSID, PASSWORD)) {
        Serial.print(F("Join AP success\r\n"));
        Serial.print(F("IP:"));
        Serial.println(getLocalIP().c_str());
        res = 1;
    } else {
        Serial.print(F("Join AP failure\r\n"));
        res = 0;
    }

    if (disableMUX()) {
        Serial.print(F("single ok\r\n"));
    } else {
        Serial.print(F("single err\r\n"));
    }

    Serial.print("setup end\r\n");
    return res;
}
