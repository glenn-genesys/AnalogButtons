AnalogButtons

Author: Glenn Burgess July 2014
Includes code by Marc Alexander, 7 September 2011

A library class for analog keypads for Arduino and other micro-controllers.
That is, keypads which encode multiple button presses into a single analog voltage value. The library handles keypads with keys for up, down, left, right, select and back.

For example, the Freetronics LCD & Keypad Shield (http://www.freetronics.com/products/lcd-keypad-shield)

Example usage:
	import "AnalogButtons.h"
	
	// Create an instance of a Freetronics keypad object -- make it global
	AnalogButtons keypad(BUTTON_ADC_PIN, AnalogButtons::FREETRONICS);
	
	// Determine the voltage levels of a new or custom keypad
	keypad.calibrate();
	
	// Configure a new keypad with the given voltage levels
	AnalogButtons button(BUTTON_ADC_PIN, 0, 98, 252, 407, 637);  
	
	// Button testing loop
	bool prevPressed = false;
	void loop() {
	   button.read();
	
	   if (button.buttonWasJustPressed() && !prevPressed) {
	     //show text label for the button pressed
	      displayButton(button.getButton());
	      Serial.println(" pressed");
	      prevPressed = true;
	   }
	
	   if (button.buttonWasJustReleased() && prevPressed) {
	     //show text label for the button released
	      displayButton(button.getButton());
	      Serial.println(" released");
	      prevPressed = false;
	   }
	}
	
	
	// Wait for a key release then act on result
	while (keypad->buttonWasJustPressed()) keypad->read();

	switch (keypad->getButtonWas()) {
	case BUTTON_UP: 	return up();
	case BUTTON_DOWN: 	return down();
	case BUTTON_LEFT: 	return left();
	case BUTTON_RIGHT: 	return right();
	case BUTTON_SELECT: return select();
	case BUTTON_NONE:   return NULL;
	}

  
