/*
 * AnalogButtons.h
 *
 *  Created on: May 4, 2014
 *      Author: glenn
 */

#ifndef ANALOGBUTTONS_H_
#define ANALOGBUTTONS_H_

#include "Arduino.h"

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif

// #define SHOW_VOLTAGE

// AnalogButtons(A0, 0, 145, 333, 505, 741)    Freetronics   
// AnalogButtons(A0, 0, 98, 252, 407, 637)     Other one

// ADC readings expected for the 5 buttons on the ADC input -- could be parameterised
/* #define RIGHT_10BIT_ADC           0  // right
#define UP_10BIT_ADC            145  // up
#define DOWN_10BIT_ADC          333  // down
#define LEFT_10BIT_ADC          505  // left
#define SELECT_10BIT_ADC        741  // right
*/
#define BUTTONHYSTERESIS         15  // hysteresis for valid button sensing window
//return values for ReadButtons()
#define BUTTON_NONE               0  //
#define BUTTON_RIGHT              1  //
#define BUTTON_UP                 2  //
#define BUTTON_DOWN               3  //
#define BUTTON_LEFT               4  //
#define BUTTON_SELECT             5  //

class AnalogButtons {
private:
	byte ADC_PIN;
	byte buttonJustPressed;     //this will be true after a ReadButtons() call if triggered
	byte buttonJustReleased;    //this will be true after a ReadButtons() call if triggered
	byte buttonWas;             //used by ReadButtons() for detection of button events

  int RIGHT_10BIT_ADC;
  int UP_10BIT_ADC;
  int DOWN_10BIT_ADC;
  int LEFT_10BIT_ADC;
  int SELECT_10BIT_ADC;

public:
#ifdef SHOW_VOLTAGE
	unsigned int voltageWas; // Useful for debugging
#endif

	enum Model {FREETRONICS, OTHER};

	AnalogButtons(byte pin, int rv, int uv, int dv, int lv, int sv);
	AnalogButtons(byte pin, Model m);
	virtual ~AnalogButtons();

	void init(int rv, int uv, int dv, int lv, int sv);

	// Non-blocking test of the current button state
	AnalogButtons read();

	float getButtonVoltage();
	void calibrate();
	
	byte getButtonWas();
	bool buttonWasJustPressed();
	bool buttonWasJustReleased();
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ANALOGBUTTONS_H_ */
