#include "SD.h"
#include "TMRpcm.h"
#include "SPI.h"
//#include "LowPower.h"
#include <stdio.h>

// Analog Pins
#define sw0  A0             // 1st switch attached at pin A0
#define sw1  A1             // 2nd switch attached at pin A1
#define sw2  A2             // 3rd switch attached at pin A2
#define sw3  A4             // 4th switch attached at pin A3
#define sw4  A5             // 5th switch attached at pin A5

// Digital Pins
#define wakeUpPin 2         // Switch interrupt pin D2, for low power wakeup
#define SD_ChipSelectPin 4  // SD card chip select pin D4
#define speakerOut 9        // Speaker output pin D9
#define SD_MOSI 11          // SD card MOSI SPI output pin D11
#define SD_MISO 12          // SD card MISO SPI output pin D12
#define SD_SCK 13           // SD card SCK SPI output pin D13


// Variable to interface with the speaker & SD card
TMRpcm audioV;

// TODO: Remove
int counter = 6;

// Indicates the state of the system
//    There are 6 states from beginning to end. 0-->5, 
//    This mirrors the amount of switches flipped
int stateIndicator = 0, savedState, randomIndex;

// Probability of output:     A,    B,    C,    X,    F},
int outputProbability[5][5]={{80,   10,   7,    3,    0},
                             {30,   50,   15,   5,    0},
                             {5,    70,   22,   3,    0},
                             {0,    10,   85,   3,    2},
                             {0,    0,    0,    0,    100}
                            };
// Switch Inputs
int InputArray[] = {0, 0, 0, 0, 0};                            

// Arrays containing the filenames for each output sound effect
//    They are split into 5 categories, depending on rarity of occurrence.
//    The final category is for when all switches are flipped.
char *soundsA[] = {"a1.wav", "a2.wav", "a3.wav", "a4.wav", "a5.wav", "a6.wav", "a7.wav"};
char *soundsB[] = {"b1.wav", "b2.wav", "b3.wav", "b4.wav", "b5.wav"};
char *soundsC[] = {"c1.wav", "c3.wav"};
char *soundsX[] = {"x3.wav", "x2.wav", "x1.wav"};
char *soundsF[] = {"f0.wav", "f1.wav", "f2.wav", "f3.wav", "f4.wav", "f5.wav"};
int sizeSounds[] = {7, 5, 2, 3, 6};

void setup(){
  
//  Pin & Serial Setup
  Serial.begin(9600);
  pinMode(wakeUpPin, INPUT);
  pinMode(sw0, INPUT);
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT); 
  pinMode(sw3, INPUT);
  pinMode(sw4, INPUT);

//  SD Setup
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }
  else{
    Serial.println("SD OK");
  }

//  Audio library setup
  audioV.speakerPin = speakerOut;
  audioV.quality(1);
  audioV.setVolume(5);

//  Finite State Machine Setup
  savedState = stateIndicator;
}

void loop(){  
  
// Attach interrupt
//  attachInterrupt(digitalPinToInterrupt(wakeUpPin), readSwitchStates, RISING);
  
// Enter Sleep Mode
//  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 

// Detach interrupt on interrupt
//  detachInterrupt(digitalPinToInterrupt(wakeUpPin)); 
  readSwitchStates();

//   Check State
  if (savedState < stateIndicator){
//    Update LEDs    
//    Create sounds on advancement
    Serial.println(stateIndicator);
    playRandomSound(stateIndicator);
  }
  
// Perform Operations

// Update State
  savedState = stateIndicator;

  
//  debugBySerial();
}

void playRandomSound(int state){

  randomIndex = checkProbability(random(1, 101), state, 0);
//  randomIndex = state - 1;
  Serial.print("randomIndex = ");
  Serial.println(randomIndex);
 
  if(randomIndex == 4){
    audioV.play(soundsF[random(0, sizeSounds[4])]);
  }
  else if(randomIndex == 3){
    audioV.play(soundsX[random(0, sizeSounds[3])]);
  }
  else if(randomIndex == 2){
    audioV.play(soundsC[random(0, sizeSounds[2])]);
  }
  else if(randomIndex == 1){
    audioV.play(soundsB[random(0, sizeSounds[1])]);
  }
  else{
    audioV.play(soundsA[random(0, sizeSounds[0])]);
  }
  
}

// Subtractive & Recursive function to implement a probability distribution on a number between 1-100
int checkProbability(int randomStop, int distIndex, int probIndex){
  
  int comparison = randomStop - outputProbability[distIndex - 1][probIndex];
  
  char str[32];
  sprintf(str, "Orig: %3d Comp: %3d Index: %3d", randomStop, comparison, probIndex, distIndex);
  Serial.println(str);
  
  if(comparison >= 1){
    return checkProbability(comparison, distIndex, probIndex + 1);
  }
  else{
    return probIndex;
  }
}

// Interrupt service routine
//   Reads the switches and counts the amount in the on state.
void readSwitchStates(){

  InputArray[0] = digitalRead(sw0);
  InputArray[1] = digitalRead(sw1);
  InputArray[2] = digitalRead(sw2);
  InputArray[3] = digitalRead(sw3);
  InputArray[4] = digitalRead(sw4);

//  InputArray[0] = analogRead(sw0);
//  InputArray[1] = analogRead(sw1);
//  InputArray[2] = analogRead(sw2);
//  InputArray[3] = analogRead(sw3);
//  InputArray[4] = analogRead(sw4);
  
//  char str[32];
//  sprintf(str, "%4d   %4d   %4d   %4d   %4d", InputArray[0], InputArray[1], InputArray[2], InputArray[3], InputArray[4]);
//  Serial.println(str);

  stateIndicator = InputArray[0] + InputArray[1] + InputArray[2] + InputArray[3] + InputArray[4];
}

void debugBySerial(){
  // Use the Serial connection to test outputs or simulate inputs
  
  if(Serial.available()){    
    switch(Serial.read()){
      case '0': audioV.play("f0.wav"); Serial.println("f0-cool"); break;
      case '1': audioV.play("f1.wav"); Serial.println("f1-cool"); break;
      case '2': audioV.play("f2.wav"); Serial.println("f2-cool"); break;
      case '3': audioV.play("f3.wav"); Serial.println("f3-cool"); break;
      case '4': audioV.play("f4.wav"); Serial.println("f4-cool"); break;
      case '5': audioV.play("f5.wav"); Serial.println("f5-cool"); break;
      case 'z': audioV.play("z.wav"); Serial.println("zelda-cool"); break;
      case 'l': audioV.play(soundsA[counter]); Serial.println(counter--); break;
      case 'P': audioV.play("temple"); break;
      case 't': audioV.play("catfish"); break;
      case 'p': audioV.pause(); break;
      case '?': if(audioV.isPlaying()){ Serial.println("A wav file is being played");} break;
      case 'S': audioV.stopPlayback(); break;
      case '[': stateIndicator--; break;
      case ']': stateIndicator++; break;
      case '=': audioV.volume(1); break;
      case '-': audioV.volume(0); break;
      default: break;
    }
  }
}
