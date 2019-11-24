// ButtonMatrix.h

#ifndef _BUTTONMATRIX_h
#define _BUTTONMATRIX_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <functional>

class ButtonMatrix{
 protected:
	uint8_t* InPins;
	uint8_t* OutPins;
	uint32_t numBtns;
	uint8_t* BtnTemp;  // Temp for debounce, don't return
	uint8_t* pressedBtns; // array of currently pressed buttons
	

	uint64_t* BtnState; // store mills since last button "on" press
	uint32_t* BtnRunningTotal;
	uint32_t inPinSz;
	uint32_t outPinSz;
	
	uint32_t CurrOutPin = 0;
	
	// non blocking timer
	unsigned long now; //current time in mils
	unsigned long pastTime;
	unsigned long lockoutDelay = 2500;
	const long ReadDelay = 20;

	void CheckButton();
	// void callback(char* topic, uint8_t* payload, unsigned int length);
	// void CBbinMsg(char* topic, uint8_t* payload, unsigned int length);
	// std::function<void(char*, uint8_t*, unsigned int)> callback;

 public:
	
	// constuctor 
	ButtonMatrix(uint8_t* _inputPins, uint8_t* _outputPins);


	// need to call in main loop
	int update(void);
	uint8_t* GetCurPressedButtons(uint8_t* PBs);
	uint32_t* GetButtonsTotal(uint32_t* TBs);
	void ResetTotal(void);

};


#endif