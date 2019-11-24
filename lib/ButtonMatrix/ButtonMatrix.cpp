#include "ButtonMatrix.h"

// read all of the inpins each time this is called

void ButtonMatrix::CheckButton()
{
	// Step to next button
	digitalWrite(OutPins[CurrOutPin],HIGH);
	delayMicroseconds(5);
    for(size_t CurrInPin = 0; CurrInPin< inPinSz; CurrInPin++){
	    if (digitalRead(InPins[CurrInPin]) == HIGH)
		    if(BtnTemp[(CurrOutPin)+(CurrInPin*outPinSz)] < 255)
			    BtnTemp[(CurrOutPin)+(CurrInPin*outPinSz)]++;
	    if (digitalRead(InPins[CurrInPin]) == LOW)
		    BtnTemp[(CurrOutPin)+(CurrInPin*outPinSz)] = 0;
    }
	digitalWrite(OutPins[CurrOutPin],LOW);
	CurrOutPin++;
	if(CurrOutPin >= outPinSz){
		CurrOutPin = 0;
	}
}

// Constructor pass the input and output ESP32 GPIOs

ButtonMatrix::ButtonMatrix(uint8_t* _inputPins, uint8_t* _outputPins)
{
    InPins = _inputPins;
    OutPins = _outputPins;
    inPinSz = sizeof(InPins);
    outPinSz = sizeof(OutPins);
    numBtns = inPinSz * outPinSz;
    BtnTemp = new uint8_t[numBtns];
    pressedBtns = new uint8_t[numBtns];
    BtnRunningTotal = new uint32_t[numBtns];
    BtnState = new uint64_t[numBtns];
    
   for (size_t i = 0; i < outPinSz; i++)
		pinMode(OutPins[i], OUTPUT);
	for (size_t i = 0; i < inPinSz; i++)
		pinMode(InPins[i], INPUT_PULLDOWN);
}

// update; call each loop

int ButtonMatrix::update(void){
   int NumBtnsPressed = 0;
   now = millis();
   // Button check 
   if(now > (pastTime + ReadDelay)){
      pastTime = now;
      CurrOutPin = 0;
      // Scan all buttons once
      for (size_t i = 0; i < outPinSz; i++)
         CheckButton();
      for (size_t i = 0; i < numBtns; i++){
         if (BtnTemp[i] > 1){
            if ((BtnState[i] == 0)||(now > (BtnState[i] + lockoutDelay)))
            	   BtnState[i] = millis(); // store curr mils of current button pressed
               pressedBtns[i] = 1;
            }
         else{
            pressedBtns[i] = 0;
            if((BtnState[i] > 0) && (now > (BtnState[i] + lockoutDelay))){
			   // debug code
			      Serial.print("button ");  
               Serial.print(i + 1);
               Serial.println(" Cycled !");
			      // ***********
		         BtnRunningTotal[i]++;
               BtnState[i] = 0;
            }
         }
	   }
   }
   return NumBtnsPressed;
}

// Get copy of currently pressed buttons array

uint8_t* ButtonMatrix::GetCurPressedButtons(uint8_t* PBs){
   uint32_t AL = sizeof(pressedBtns);
   memcpy(PBs, pressedBtns, AL);
   return PBs;
}

// Get copy of button press running total array

uint32_t* ButtonMatrix::GetButtonsTotal(uint32_t* TBs){
   uint32_t AL = sizeof(BtnRunningTotal);
   memcpy(TBs, BtnRunningTotal, AL);
   return TBs;
}

// Reset running total array

void ButtonMatrix::ResetTotal(void){
   for(size_t i=0; i<(sizeof(BtnRunningTotal) / sizeof(BtnRunningTotal[0])); i++){
      BtnRunningTotal[i] = 0;
   }
}


