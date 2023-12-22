#include <FastLED.h>
#define OUT_PIN 2
 #define Num 60
 CRGB leds[Num];


void setup() {
  
  FastLED.addLeds<WS2812B, OUT_PIN, GRB>(leds, Num);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();
  FastLED.setBrightness(255);
  FastLED.setTemperature(0xFF8029);

}

void loop() {
  

}
