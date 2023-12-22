#include <FastLED.h>


#define LPIN 2
#define Num 60
int p=1;
int h=0;
CRGB leds[Num];

//button



void setup() {
  pinMode(LPIN, OUTPUT);
  FastLED.addLeds<WS2812B, LPIN, GRB>(leds, Num);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000);
  FastLED.clear();
  FastLED.show();
}

void loop() {



      for (int j = 1; j < Num && j>=0; j=j+p) {
        
       
        if (j==59 || j==0){

          p*=-1;
        }

         leds[j] = CHSV (3*(h%255) , 255, 255);
         h++;

       
        FastLED.show();
        delay(30);
      }
}
