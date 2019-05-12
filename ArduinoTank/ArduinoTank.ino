//The setup function is called once at startup of the sketch
#include "Motor.h"
#include "Radioprocess.h"
#include "ThreadFunc.h"
#include <LinkedList.h>
#include <String.h>

#define HUE 	1
#define CYON	2
#define TURNON	3
// arduino mega sram 8000 byte
// arduino uno sram 2000byte

int freeRam(void);
Distance_Sensor HC1(TRIG_PIN1, ECHO_PIN1);
Distance_Sensor HC2(TRIG_PIN2, ECHO_PIN2);
//wifi WF(Serial2);

void setup()
{
	Serial.begin(9600);
	Serial.print("Rmain Memory : ");
	Serial.println(freeRam());

	main_thread_list->add_thread(new ControlTankThread(3));
	main_thread_list->add_thread(new HCSR04Thread(1,HC1));
	main_thread_list->add_thread(new HCSR04Thread(2,HC2));
	main_thread_list->add_thread(new LEDThread(4,TURNON,0));
	main_thread_list->add_thread(new ControlThread(90));
	//main_thread_list->add_thread(new WIFIThread(4,WF));
	Serial.print("Rmain Memory : ");
	Serial.println(freeRam());
}



int freeRam(void) {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);

}

