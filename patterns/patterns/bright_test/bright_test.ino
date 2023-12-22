#include <FastLED.h>
//pulldown res
//leds
#define LPIN 2
#define Num 60
CRGB leds[Num];
//button
const int BPIN = 4;
int buttonState = LOW;
int hold = 1;
int verity1=LOW;
int uno=5;
int k =2000;


void setup() {
  pinMode(LPIN, OUTPUT);
  pinMode(BPIN, INPUT);
  FastLED.addLeds<WS2812B, LPIN, GRB>(leds, Num);

  FastLED.clear();
  FastLED.show();
  
  
}

void loop() {
  
    FastLED.setMaxPowerInVoltsAndMilliamps(5, k);
verity1=buttonState;

FastLED.clear();
  
  buttonState = digitalRead(BPIN);

  if (buttonState == HIGH && verity1!=buttonState) {
    hold++;
    uno+=5;

  }

FastLED.clear();
      for (int j = 0; j < 60; j++) {
        leds[j] = CRGB(255, 255, 255);
      }
        FastLED.show();
      
  
      

   if (uno>60) {uno=5;}

delay(150);
k+=100;
if (k>3100){k=2000;}
}
