#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

// Variable to interface with the speaker & SD card
TMRpcm audioV;

// Indicates the state of the system
//    There are 6 states from beginning to end. 0-->5, 
//    This mirrors the amount of switches flipped
int stateIndicator = 0;
int stateProbability[][] = {{0    ,0,    0,    0,    0},
                            {0.8  ,0.1,  0.07, 0.03, 0},
                            {0.6  ,0.15, 0.15, 0.1,  0},
                            {0.3  ,0.5,  0.15, 0.05, 0},
                            {0.05 ,0.7,  0.22, 0.03, 0},
                            {0    ,0.1,  0.85, 0.03, 0.02},
                            {0    ,0,    0,    0,    1}
                            };

// Arrays containing the filenames for each sound effect
//    They are split into 5 categories, depending on rarity of occurrence.
//    The final category is for when all switches are flipped.
char *soundsA[] = {"a1.wav", "a2.wav", "a3.wav", "a4.wav", "a5.wav", "a6.wav", "a7.wav"};
char *soundsB[] = {"b1.wav", "b2.wav", "b3.wav", "b4.wav", "b5.wav"};
char *soundsC[] = {"c1.wav", "c3.wav"};
char *soundsX[] = {"x3.wav", "x2.wav", "x1.wav"};
char *soundsF[] = {"f0.wav", "f1.wav", "f2.wav", "f3.wav", "f4.wav", "f5.wav"};
              
void setup(){

  Serial.begin(9600);

//  SD Setup
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }
  else{
    Serial.println("SD OK");
  }

//  Audio library setup
  audioV.speakerPin = 9;
  audioV.quality(1);
  audioV.setVolume(5);
}

void loop(){  
  if(Serial.available()){    
    switch(Serial.read()){
      case '0': audioV.play("f0.wav"); Serial.println("f0-cool"); break;
      case '1': audioV.play("f1.wav"); Serial.println("f1-cool"); break;
      case '2': audioV.play("f2.wav"); Serial.println("f2-cool"); break;
      case '3': audioV.play("f3.wav"); Serial.println("f3-cool"); break;
      case '4': audioV.play("f4.wav"); Serial.println("f4-cool"); break;
      case '5': audioV.play("f5.wav"); Serial.println("f5-cool"); break;
      case 'z': audioV.play("z.wav"); Serial.println("zelda-cool"); break;
      case 'l': audioV.play(arr[counter]); Serial.println(counter--); break;
      case 'P': audioV.play("temple"); break;
      case 't': audioV.play("catfish"); break;
      case 'p': audioV.pause(); break;
      case '?': if(audioV.isPlaying()){ Serial.println("A wav file is being played");} break;
      case 'S': audioV.stopPlayback(); break;
      case '=': audioV.volume(1); break;
      case '-': audioV.volume(0); break;
      default: break;
    }
  }
  
}
