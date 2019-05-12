
#ifndef __LED_H_
#define __LED_H_

#include "Arduino.h"
#include "FastLED.h"
#include "mthread.h"


#define NUM_LEDS	5

#define MAX_COL_DIP 20
#define DATA_PIN	A2

#define LEFT_LED 7
#define RIGHT_LED 8
#define FRONT_LED 9
#define BACK_LED 10


class LED
{
	public :

		LED();

		byte hueMiddle;
		byte hueEdge;
		byte hueStep;
		byte s;
		byte onoff_flag;
		int maxHue ; //NB: hue may not be in the range 0-255, if we use a differnt hsv2rgb function.
		bool sync ;

		CRGB leds[NUM_LEDS];
		CRGB Get_Color(unsigned char color);
		void play_CYON();
		void play_HUE();
		void play_Bullet();
		void Turn_On(CRGB color);
		void Turn_Off();
		void updateStrip(byte hueMiddle, byte hueEdge);

};

#endif
