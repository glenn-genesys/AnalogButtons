/*
 * AnalogButtons.cpp
 *
 *  Created on: May 4, 2014
 *      Author: glenn
 */

#include "AnalogButtons.h"

// AnalogButtons(A0, 0, 145, 333, 505, 741)    Freetronics   
// AnalogButtons(A0, 0, 98, 252, 407, 637)     Other one
/* AnalogButtons::AnalogButtons(byte pin, int rv=0, int uv=145, int dv=333, int lv=505, int sv=741):
	RIGHT_10BIT_ADC(rv), UP_10BIT_ADC(uv), DOWN_10BIT_ADC(dv), LEFT_10BIT_ADC(lv), SELECT_10BIT_ADC(sv)
 {
	ADC_PIN = pin;
	buttonJustPressed  = false;
	buttonJustReleased = false;
	buttonWas          = BUTTON_NONE;
#ifdef SHOW_VOLTAGE
	voltageWas = 0;
#endif
  } */

AnalogButtons::AnalogButtons(byte pin, Model m) {
  switch (m) {
    case FREETRONICS: { init(0, 145, 333, 505, 741); break; }
    case OTHER:       { init(0, 98, 252, 407, 637);  break; }
  }
	ADC_PIN = pin;
	buttonJustPressed  = false;
	buttonJustReleased = false;
	buttonWas          = BUTTON_NONE;
#ifdef SHOW_VOLTAGE
	voltageWas = 0;
#endif
}

AnalogButtons::~AnalogButtons() {
	// TODO Auto-generated destructor stub
}

/*
  Pins used by Freetronics LCD & Keypad Shield:

    A0: Buttons, analog input from voltage ladder
    D4: LCD bit 4
    D5: LCD bit 5
    D6: LCD bit 6
    D7: LCD bit 7
    D8: LCD RS
    D9: LCD E
    D3: LCD Backlight (high = on, also has pullup high so default is on)

  ADC voltages for the 5 buttons on analog input pin A0:

    RIGHT:  0.00V :   0 @ 8bit ;   0 @ 10 bit
    UP:     0.71V :  36 @ 8bit ; 145 @ 10 bit
    DOWN:   1.61V :  82 @ 8bit ; 329 @ 10 bit
    LEFT:   2.47V : 126 @ 8bit ; 505 @ 10 bit
    SELECT: 3.62V : 185 @ 8bit ; 741 @ 10 bit
*/

void AnalogButtons::init(int rv, int uv, int dv, int lv, int sv) {
  RIGHT_10BIT_ADC = rv;
  UP_10BIT_ADC = uv;
  DOWN_10BIT_ADC = dv;
  LEFT_10BIT_ADC = lv;
  SELECT_10BIT_ADC = sv;
}

byte AnalogButtons::getButtonWas() { return buttonWas; }
bool AnalogButtons::buttonWasJustPressed() { return buttonJustPressed; }
bool AnalogButtons::buttonWasJustReleased() { return buttonJustReleased; }

/*--------------------------------------------------------------------------------------
  read()
  A non-blocking routine to check if a button was pressed and return the value
--------------------------------------------------------------------------------------*/
AnalogButtons AnalogButtons::read()
{
   unsigned int buttonVoltage;
   byte button = BUTTON_NONE;   // return no button pressed if the below checks don't write to btn

   //read the button ADC pin voltage
   buttonVoltage = analogRead( ADC_PIN );
   //sense if the voltage falls within valid voltage windows
   if( buttonVoltage < ( RIGHT_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_RIGHT;
   }
   else if(   buttonVoltage >= ( UP_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( UP_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_UP;
   }
   else if(   buttonVoltage >= ( DOWN_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( DOWN_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_DOWN;
   }
   else if(   buttonVoltage >= ( LEFT_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( LEFT_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_LEFT;
   }
   else if(   buttonVoltage >= ( SELECT_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( SELECT_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_SELECT;
   }
   //handle button flags for just pressed and just released events
   if( ( buttonWas == BUTTON_NONE ) && ( button != BUTTON_NONE ) )
   {
      //the button was just pressed, set buttonJustPressed, this can optionally be used to trigger a once-off action for a button press event
      //it's the duty of the receiver to clear these flags if it wants to detect a new button change event
      buttonJustPressed  = true;
      buttonJustReleased = false;
   }
   if( ( buttonWas != BUTTON_NONE ) && ( button == BUTTON_NONE ) )
   {
      buttonJustPressed  = false;
      buttonJustReleased = true;
   }

   //save the latest button value, for change event detection next time round
   buttonWas = button;
#ifdef SHOW_VOLTAGE
   voltageWas = buttonVoltage;
#endif

   return *this;
}

/*--------------------------------------------------------------------------------------
  getButtonVoltage()
  Steps through button pressing to determine the correct voltage levels for each button
--------------------------------------------------------------------------------------*/
float AnalogButtons::getButtonVoltage() {
	const float averagingRatio = 0.9;

	unsigned int pinVoltage = analogRead( ADC_PIN );
	float buttonVoltage;
	unsigned int minVoltage = 1024, maxVoltage = 0;

	// Detect onset of button pressing
	while (pinVoltage > 1020)
	   pinVoltage = analogRead( ADC_PIN );

	Serial.print("First press: ");
	Serial.println(pinVoltage);
	buttonVoltage = pinVoltage;

	// Average voltages while button being pressed
	while (pinVoltage < 1020) {
	   buttonVoltage = buttonVoltage*averagingRatio + pinVoltage*(1.0 - averagingRatio);
	   minVoltage = min(minVoltage, pinVoltage);
	   maxVoltage = max(maxVoltage, pinVoltage);
	   pinVoltage = analogRead( ADC_PIN );
	}

	Serial.print(minVoltage);
	Serial.print(" < ");
	Serial.print(buttonVoltage, DEC);
	Serial.print(" < ");
	Serial.println(maxVoltage);

	return buttonVoltage;
}

/*--------------------------------------------------------------------------------------
  calibrate()
  Steps through button pressing to determine the correct voltage levels for each button
--------------------------------------------------------------------------------------*/
void AnalogButtons::calibrate()
{
   unsigned int buttonVoltage;
   byte button = BUTTON_NONE;   // return no button pressed if the below checks don't write to btn

   Serial.print(F("Press 'Right' "));
   getButtonVoltage();
   Serial.print(F("'Up' "));
   getButtonVoltage();
   Serial.print(F("'Down' "));
   getButtonVoltage();
   Serial.print(F("'Left' "));
   getButtonVoltage();
   Serial.print(F("'Select' "));
   getButtonVoltage();
}
