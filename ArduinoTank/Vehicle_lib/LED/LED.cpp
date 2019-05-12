#include "LED.h"

LED::LED()
{
	int i;
	FastLED.addLeds<WS2812B, DATA_PIN,GRB>(leds, NUM_LEDS);
	this->hueMiddle =0;
	this->hueEdge =0;
	this->hueStep =10;
	this->maxHue =255;
	this->sync =false;
	this->s = 0;
	this->onoff_flag = 0;
	for(i=0; i< NUM_LEDS; i++)
	{
		leds[i] = CRGB::AliceBlue;
	}

}

void LED::play_CYON()
{
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red
		leds[i] = CRGB::Green;
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		leds[i] = CRGB::Black;
		// Wait a little bit before we loop around and do it again
		delay(30);
	}

	// Now go in the other direction.
	for(int i = NUM_LEDS-1; i >= 0; i--) {
		// Set the i'th led to red
		leds[i] = CRGB::Green;
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		leds[i] = CRGB::Black;
		// Wait a little bit before we loop around and do it again
		delay(30);
	}
}


void LED::Turn_On(CRGB color)
{
	//if(this->onoff_flag == 0)
	//{
		for(int i = 0; i < NUM_LEDS; i++) {
			leds[i] = color;
		}
		FastLED.show();
	//}
	//this->onoff_flag = 1;
}

void LED::Turn_Off()
{

	for(int i = 0; i < NUM_LEDS; i++) {
		leds[i] = CRGB::Black;
	}
	FastLED.show();
	this->onoff_flag = 0;
}
void LED::play_HUE()
{



		//pause a second
		delay(1);

		//push the hue of the middle part of the bias light forward
		/*for(int s=0; s< hueStep; s++)
		{
			hueMiddle++;
			if(hueMiddle >= maxHue)
			{
			  sync = true;
			  break;
			}
			updateStrip(hueMiddle, hueEdge);
		}*/

		//have the edge of the bias light catch up to the hue in the middle
		//for(int s=0; s< hueStep; s++)
		{
			hueEdge++;
			if(hueEdge >= maxHue)
			{
			  sync = true;
			}
		   updateStrip(hueMiddle, hueEdge);
		}

		// Reached the end of the hue cycle, wrap to the begining.
		// We do it this way because if the hueMiddle wraps around, but the
		// hueEdge remains before the end, then a full rainbow is creted
		// when updateStrip smooths the pixels over the strip
		if(sync)
		{
		   hueMiddle = 0;
		   hueEdge = 0;
		   sync = false;
		}


}
void LED::play_Bullet()
{
	unsigned char bright[3] = {0xff, 0xaf,0x2f};
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red
		leds[i] = CRGB::Green;
		// Show the leds
		FastLED.setBrightness(bright[i]);
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		//leds[i] = CRGB::Black;
		// Wait a little bit before we loop around and do it again
		delay(1000);
	}
	for(int i=0; i< NUM_LEDS; i++)
	{
		leds[i] = CRGB::Black;
		FastLED.show();
	}
}

CRGB LED::Get_Color(unsigned char color)
{
	CRGB col = 0x0;
	if (color == 0)
		col = CRGB::Green;
	else if (color == 1)
		col = CRGB::Blue;
	else if (color == 2)
		col = CRGB::Red;
	else if (color == 3)
		col = CRGB::Aqua;
	else if (color == 4)
		col = CRGB::DarkSlateGray;
	else if (color == 5)
		col = CRGB::DeepPink;
	else if (color == 6)
		col = CRGB::DarkOliveGreen;
	else if (color == 7)
		col = CRGB::Gold;
	else if (color == 8)
		col = CRGB::Ivory;
	else if (color == 9)
		col = CRGB::IndianRed;
	else if (color == 10)
		col = CRGB::Lime;
	else if (color == 11)
		col = CRGB::LimeGreen;
	else if (color == 12)
		col = CRGB::Maroon;
	else if (color == 13)
		col = CRGB::MediumBlue;
	else if (color == 14)
		col = CRGB::MintCream;
	else if (color == 15)
		col = CRGB::MistyRose;
	else if (color == 16)
		col = CRGB::OldLace;
	else if (color == 17)
		col = CRGB::PaleGreen;
	else if (color == 18)
		col = CRGB::Orchid;
	else if (color == 19)
		col = CRGB::PaleVioletRed;
	else if (color == 20)
		col = CRGB::Silver;
	else if (color == 21)
		col = CRGB::SaddleBrown;



	return col;
}

void LED::updateStrip(byte hueMiddle, byte hueEdge)
{
    float mid = NUM_LEDS/2;
    int hueDiff =  hueEdge - hueMiddle;
    for(int i = 0; i < NUM_LEDS; i++)
    {
        //get the hue for the pixel
        CRGB pixel;
        //float h = abs(i-mid)/mid; //0 at mid, 1 at edge
        float h =1.0;
        h *= hueDiff; //0 at mid, hueDiff at edge
        h += hueMiddle; //hueMiddle at mid, hueEdge at edge

        //set the pixel
        hsv2rgb_spectrum(CHSV((byte)round(h), 255, 255), pixel);
        leds[i] = pixel;
    }
    delay(1);
    LEDS.show();
}


