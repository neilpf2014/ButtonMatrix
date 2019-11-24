#include <Arduino.h>
#include <ButtonMatrix.h>

#define NUM_BTNS 20 // number of buttons to be read
// all of the ESP32 button GPIO's 
// This needs to match the carrier PCB
#define OUTPIN_1 23
#define OUTPIN_2 22
#define OUTPIN_3 21
#define OUTPIN_4 19
#define OUTPIN_5 18
#define INPIN_1 34
#define INPIN_2 35
#define INPIN_3 32
#define INPIN_4 33
#define INPIN_7 25 // # 21 - 25 on board
#define INPIN_8 26 // # 26 - 30 on board
#define INPIN_5 36 // # 31 - 35 on board
#define INPIN_6 39 // # 35 - 40 on board

#define BTN_DELAY 2500 // lockout delay in ms

uint8_t OutPins[5] = {OUTPIN_1, OUTPIN_2, OUTPIN_3, OUTPIN_4, OUTPIN_5};
uint8_t InPins[4] = {INPIN_1, INPIN_2, INPIN_3, INPIN_4};

// used for debounce of btn array
uint8_t BtnTemp[NUM_BTNS];

// holds time in ms of last button press
uint64_t BtnState[NUM_BTNS];

// running count of recorded button presses
uint32_t BtnRecord[NUM_BTNS];

uint32_t CurrOutPin = 0;
uint32_t CurrInPin = 0;
uint32_t NumBtnsPressed = 0;

uint64_t now;
uint64_t PasTime = 0;

// Btn scan freq in mill
uint64_t Period1 = 5;// in mill

// use LED for debugging
uint8_t LedState;
uint64_t LedOnTime = 0;
uint64_t LedOnPer = 500;

ButtonMatrix Buttons(InPins, OutPins);

// each time this is called will step though array of buttons
// scans inputs then incr the output pins

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
	Serial.begin(115200);
	Serial.println("Started");
}

void loop() {
   NumBtnsPressed = Buttons.update();
   if (NumBtnsPressed > 0){
      uint8_t* prBtns = nullptr;
      prBtns = Buttons.GetCurPressedButtons(prBtns);
      for(size_t i=0; i<sizeof(prBtns);i++){
         Serial.print("btn #" + i);
         Serial.print(" State ");
         Serial.println(prBtns[i]);
      }
      delete prBtns;
  }
}