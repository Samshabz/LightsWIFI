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


void setup() {
  pinMode(LPIN, OUTPUT);
  pinMode(BPIN, INPUT);
  FastLED.addLeds<WS2812B, LPIN, GRB>(leds, Num);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000);
  FastLED.clear();
  FastLED.show();
}

void loop() {


  buttonState = digitalRead(BPIN);

  if (buttonState == HIGH) {
    hold++;
    delay(200);

  }

  if (hold==1){

      for (int j = 0; j < Num; j++) {
        leds[j] = CRGB(255, 255, 255);
        FastLED.show();
      }
  }
      

    if (hold==2){

      for (int j = 0; j < Num; j++) {
        leds[j] = CRGB(0, 255, 0);
        FastLED.show();
      }
    }

    if (hold==3){

      for (int j = 0; j < Num; j++) {
        leds[j] = CRGB(0, 0, 255);
        FastLED.show();
      }
    }
   if (hold>3) {hold=1;}


}
