/*
/mMo/12,12|
/mlC/p|
/mlC/r|
/mKey/hello|
*/

#include "Mouse.h"
#include "Keyboard.h"
#include "HID.h"

int x,y = -1;

void setup() {
  Serial.begin(9600);

  // Sends a clean report to the host. This is important on any Arduino type.
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  if (Serial.available())
  {
    String mouseString = Serial.readStringUntil('|');
    mouseString.trim();
    if (mouseString.charAt(0) == '/')
    {
      // send like this - "/mMo/5,5"
      if (mouseString.substring(1, 4).equals(F("mMo")) == true)
      {
            //-------------------------------   
            int slashIdx = mouseString.indexOf('/', 1);
            if (slashIdx > 0)
            {
              int commaIdx = mouseString.indexOf(',', slashIdx + 1);
              if (commaIdx > 0)
              {
                x = mouseString.substring(slashIdx + 1, commaIdx).toInt();
                y = mouseString.substring(commaIdx + 1).toInt();
                
              }
              else
              {
                x = mouseString.substring(slashIdx + 1).toInt();
                y = 0;
              }
              Mouse.move(x, y, 0);
              //Serial.print(x);Serial.print(':');Serial.println(y);
            }
            //-------------------------------         
            Serial.println(F("move"));
      }
      else if (mouseString.substring(1, 4).equals(F("mlC")) == true)
      {
            //-------------------------------   
            int slashIdx = mouseString.indexOf('/', 1);
            if (slashIdx > 0)
            {
              char action = mouseString.substring(slashIdx + 1)[0];
              if ((action == 'p') && (!Mouse.isPressed(MOUSE_LEFT)))
              {
                Mouse.press(MOUSE_LEFT);
                Serial.println(F("Pre"));
              }
              else if ((action == 'r') && (Mouse.isPressed(MOUSE_LEFT)))
              {
                Mouse.release(MOUSE_LEFT);
                Serial.println(F("Rls"));
              }
            }
            else
            {
              Mouse.press();
              delay(400);
              Mouse.release();
              Serial.println(F("Click"));
            }
            //-------------------------------         
      }
      else if (mouseString.substring(1, 4).equals(F("mrC")) == true)
      {
            //-------------------------------   
            int slashIdx = mouseString.indexOf('/', 1);
            if (slashIdx > 0)
            {
              char action = mouseString.substring(slashIdx + 1)[0];
              if ((action == 'p') && (!Mouse.isPressed(MOUSE_RIGHT)))
              {
                Mouse.press(MOUSE_RIGHT);
                Serial.println(F("Pre"));
              }
              else if ((action == 'r') && (Mouse.isPressed(MOUSE_RIGHT)))
              {
                Mouse.release(MOUSE_RIGHT);
                Serial.println(F("Rls"));
              }
            }
            else
            {
              Mouse.press(MOUSE_RIGHT);
              delay(400);
              Mouse.release(MOUSE_RIGHT);
              Serial.println(F("Click"));
            }
      }
      else if (mouseString.substring(1, 5).equals(F("mKey")) == true)
      {
            //-------------------------------   
            int slashIdx = mouseString.indexOf('/', 1);
            if (slashIdx > 0)
            {
              
              Keyboard.print(mouseString.substring(slashIdx + 1));
            }
            //-------------------------------         
            
            Serial.println(F("Key"));
      }      
      
    }
  }
}
