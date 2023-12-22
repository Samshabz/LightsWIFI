#include <FastLED.h>
#include <math.h>

//pulldown res
//leds
#define LPIN 2
#define Num 60
#define Mar 10
#define DEL 1500
#define LAG 320
CRGB leds[Num];
int p = 1;
float speed;
float b = pow((pow((Num / 2), 2) + 1), 2);
int sp = 150;
int strtsp = 200;

/*
  to solve a given value b - take your end bound of interest half it to get ; square  + 1 sqaure  - then add that value to h. This bounds curves
*/
//button
const int BPIN = 4;
int buttonState = LOW;
int hold = 0;

void setup() {
  pinMode(LPIN, OUTPUT);
  pinMode(BPIN, INPUT);
  FastLED.addLeds<WS2812B, LPIN, GRB>(leds, Num);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 3600);
  FastLED.clear();
  FastLED.show();
  FastLED.setBrightness(100);
}


void loop() {


  //original setting red blue gold

  for (int i = 0; i < 3 ; i++) {
    leds[i] = CRGB(255, 0, 0);
  }

  for (int i = Num - 3; i < Num ; i++) {
    leds[i] = CRGB(255, 0, 0);
  }

  for (int i = 3; i < (Num - 3); i++) {
    leds[i] = CRGB(255, 47, 5);
  }

  for (int i = 3; i < Mar; i++) {
    leds[i] = CRGB(0, 150, 255);
  }

  for (int i = Num - Mar;  i < Num - 3; i++ ) {
    leds[i] = CRGB(0, 150, 255);
  }

  FastLED.show();
  delay(500);
  //original

  for (int i = 1; i < Num && i >= 0; i += p) {



    //green mover
    leds[i] = CRGB(0, 255, 0);
    leds[i - p] = CRGB(0, 255, 0);

    // color reinstater

    if (i < 3 + 2 * p ) {
      leds[i - (2 * p)] = CRGB(255, 0, 0);
    }

    if (i > (Num - 4 + 2 * p)) {
      leds[i - (2 * p)] = CRGB(255, 0, 0);
    }

    if (i >= 3 + 2 * p && i < Mar + 2 * p ) {
      leds[i - (2 * p)] = CRGB(0, 150, 255);
    }
    if (i > (Num - Mar + 2 * p - 2) && i < (Num - 3 + 2 * p)) {
      leds[i - (2 * p)] = CRGB(0, 150, 255);
    }

    else if (i >= Mar + 2 * p && i <= (Num - Mar + 2 * p)) {

      leds[i - (2 * p)] = CRGB(255, 47, 5);
    }

    FastLED.show();




    speed = strtsp - (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) - sp;

    delay(speed);
    FastLED.show();


    buttonState = digitalRead(BPIN);

    //button pressed
    if (buttonState == HIGH) {
      //within margin
      if (i > (Num - Mar - 2) && i < Num - 3 && p > 0 || i < Mar && i > 2 && p < 0) {
        p *= -1;
        if (strtsp > 20){
          sp-=24; //higher sp number will be higher rate and peak (sp=peakk val ms) ie higher sp is faster

          strtsp-=30; //sp cannot exceed strt speed - lower means rate is lower but av speed higher

        }

      }
      //out margin
      else {
        //play 1 wins
        if (i > Num / 2) {
          for (int j = 0; j < Num / 2 ; j++) {
            leds[j] = CRGB(0, 0, 255);
          }
          for (int j = Num / 2; j < Num ; j++) {
            leds[j] = CRGB(255, 0, 0);
          }
        }
        //play 2 wins
        else if (i < Num / 2) {
          for (int j = 0; j < Num / 2 ; j++) {
            leds[j] = CRGB(255, 0, 0);
          }
          for (int j = Num / 2; j < Num ; j++) {
            leds[j] = CRGB(0, 0, 255);
          }

        }
        FastLED.show();
        delay(DEL);
        //buttonState = LOW;
        //i = 1;
        p = 1;
        sp = 150;
        strtsp = 200;
        break;

        //for (int i = 3; i < (Num - 3); i++)
        //leds[i] = CRGB(255, 47, 5);
        //FastLED.show();




      }
      delay(LAG);
    }
    //button not pressed - off court
    else if (i == Num - 1 || i == 0) {
      //play 1 wins
      if (i > Num / 2) {
        for (int j = 0; j < Num / 2 ; j++) {
          leds[j] = CRGB(0, 0, 255);
        }
        for (int j = Num / 2; j < Num ; j++) {
          leds[j] = CRGB(255, 0, 0);
        }
      }
      //play 2 wins
      else if (i < Num / 2) {
        for (int j = 0; j < Num / 2 ; j++) {
          leds[j] = CRGB(255, 0, 0);
        }
        for (int j = Num / 2; j < Num ; j++) {
          leds[j] = CRGB(0, 0, 255);
        }

      }

      FastLED.show();
      delay(DEL);
      //i = 1;
      p = 1;
      sp = 150;
      strtsp = 200;
      break;

    }
  }

}
