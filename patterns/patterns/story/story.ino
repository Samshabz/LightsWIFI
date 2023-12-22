#include <FastLED.h>
#define OUT_PIN 2
#define Num 60


CRGB leds[Num];

void setup() {
  FastLED.addLeds<WS2812B, OUT_PIN, GRB>(leds, Num);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();


}

void loop() {
  int j = Num / 2;
  for (int i = 0; i <= Num / 2; i++) {

    FastLED.clear();
    leds[i] = CRGB(0, 255, 0);
    FastLED.show();
    delay(100);

  }
  for (int i = Num; i >= (2 * Num / 3); i--) {

    FastLED.clear();
    leds[i] = CRGB(255, 0, 0);
    leds[j] = CRGB(0, 255, 0);
    j--;
    FastLED.show();
    delay(250);

  }

  int k = ((2 * Num / 3));
  int q = Num / 6;

  for (int i = 0; i <= Num / 2; i++) {

    FastLED.clear();
    if (q == k) {
      FastLED.clear();
      for (int y = 0; y < Num; y++) {
        leds[y] = CRGB(255, 27, 5);

        i = 800;
        FastLED.show();
        
      }
      delay(5000);
    }

   
    leds[i] = CRGB(255, 0, 0);
    leds[q] = CRGB(0, 255, 0);
    leds[k] = CRGB(255, 0, 0);
    q++;
    k--;
    delay(300);
    FastLED.show();

  }





}
