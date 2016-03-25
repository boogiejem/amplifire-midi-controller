

// MIDI controller for Amplifire V1.02 March 25 2016
// This programme is written for the Arduino series of micro controllers and has been tested on the Uno amd Nano boards
// Programme to enable A/B preset switching for Atomic Amplifire via MIDI
// and act as a wah pedal with an expression pedal connected to Arduino pin A0
// 4 foot switches are supported of the momentary close type and connecting pins 2,3,4,5 to ground
// a 5th footswitch toggles between a high and a low volume will be the next feature to add
// a pot on pin A0 to input a voltage between 0 and 5V

#include <RBD_Timer.h> // timer library required by the button library
#include <RBD_Button.h> // button library

const int WAH_INPUT = 0; // the pin (A0) to read the pot value from
const int THRESHOLD = 3; // sets the noise threshold for the analogue input

RBD::Button FS1(2); // FS1 on pin 2 with internal pull up - preset increment footswitch 1
RBD::Button FS2(3); // FS2 on pin 3 with internal pull up
RBD::Button FS3(4); // FS3 on pin 4 with internal pull up
RBD::Button FS4(5); // FS4 on pin 5 with internal pull up
RBD::Button FS5(6); // FS5 on pin 6 with internal pull up

byte FS1Presets[] = {33,117}; // an array of preset values for FS1
byte FS2Presets[] = {125,113}; // an array of preset values for FS2
byte FS3Presets[] = {116,36}; // an array of preset values for FS3
byte FS4Presets[] = {48,49}; // an array of preset values for FS4
byte FS5Volume[] = {200, 255}; // an array to hold the high and low volume settings

int FS1Index = 0; // a preset index for FS1
int FS2Index = 0; // a preset index for FS2
int FS3Index = 0; // a preset index for FS3
int FS4Index = 0; // a preset index for FS4
int FS5Index = 0; // an index for high and low volumes

void setup() {
  Serial.begin(31250); // set bit rate 9600 for serial monitor and 31250 for midi
}

void loop() {
  if (FS1.onPressed()){ // if FS1 is pressed do this 
    // Serial.println("FS1 pressed"); // test code
    // Serial.println(FS1Presets[FS1Index]); // test code
    progChange(FS1Presets[FS1Index]); // send the programme change message for the patch in FS1Presets
    FS1Index ++; // increment the preset index
    FS2Index = 0; // reset FS2 index
    FS3Index = 0; // reset FS3 index
    FS4Index = 0; // reset FS4 index
    if (FS1Index > 1)  { FS1Index = 0; } // count preset index to 2 and then reset to 0
  }
  if (FS2.onPressed()){
    // Serial.println("FS2 pressed"); // test code
    // Serial.println(FS2Presets[FS2Index]); // test code
    progChange(FS2Presets[FS2Index]);
    FS2Index ++; // increment the preset index
    FS1Index = 0; // reset FS1 index
    FS3Index = 0; // reset FS3 index
    FS4Index = 0; // reset FS4 index
    if (FS2Index > 1)  { FS2Index = 0; } // loop through 2 presets 
  }
  if (FS3.onPressed()){
    //Serial.println("FS3 pressed"); // test code
    //Serial.println(FS3Presets[FS3Index]); // test code
    progChange(FS3Presets[FS3Index]); // send the programme change message 
    FS3Index ++; // increment the preset index
    FS1Index = 0; // reset FS1 index
    FS2Index = 0; // reset FS2 index
    FS4Index = 0; // reset FS4 index
    if (FS3Index > 1)  { FS3Index = 0; }
  }
  if (FS4.onPressed()){
    // Serial.println("FS3 pressed"); // test code
    // Serial.println(FS3Presets[FS3Index]); // test code
    progChange(FS4Presets[FS4Index]);
    FS4Index ++; // increment the preset index
    FS1Index = 0; // reset FS1 index
    FS2Index = 0; // reset FS2 index
    FS3Index = 0; // reset FS3 index
    if (FS4Index > 1)  { FS4Index = 0; }
  }
// start of the wah pedal read code for a pot on analogue input A0
  static int oldWahValue = 0;
  static int oldMappedWahValue = 0;
  int wahValue = analogRead(WAH_INPUT); // read the voltage from the pedal
  if(abs(wahValue - oldWahValue) < THRESHOLD)return; // return if the wah value has changed by less than the threshold
  oldWahValue = wahValue;
  int mappedWahValue = map(wahValue, 0, 1023, 0, 255);
  if(mappedWahValue == oldMappedWahValue) return;
  oldMappedWahValue = mappedWahValue;
  // Serial.println(mappedWahValue); // trouble shooting test code
  changeControl(0x15, mappedWahValue); // 0x15 = Amplifire wah cc message
}

void progChange(int progNum) {
  Serial.write(0xC0); // prog change command on midi chan 1
  Serial.write(progNum); // send the patch number
}

void changeControl(int message, int value) {
  Serial.write(0xB0); // change control command on midi channel 1
  Serial.write(message); 
  Serial.write(value);
}


