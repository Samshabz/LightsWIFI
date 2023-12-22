#include <FastLED.h>
#include <math.h>
#define LPIN 2
#define Num 60
int offs;
int sat, bri, sp;

//parabolic shifter
float hgth;
float b = pow((pow((Num / 2), 2) + 1), 2);
//int strtsp = 200;
CRGB leds[Num];
void setup() {
   pinMode(LPIN, OUTPUT);
  
  FastLED.addLeds<WS2812B, LPIN, GRB>(leds, Num);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 3600);
  FastLED.clear();
  FastLED.show();
  FastLED.setBrightness(250);
}

void loop() {

  //set normal distribution of red to orange outer hue - lightens at edges

 for (int i=0; i<Num; i++){
int sp = 8;
hgth = - (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) ;
 sp = 30;
 offs=220;
sat =  (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;
sp= 74;
offs=160;
bri = (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;

leds[i]= CHSV (hgth,sat,bri);
//red to orange 0-15, alongside brightness drop 255-0

 }
 FastLED.show();
delay(180);


//set darker pixels 1
 for (int i=0; i<Num ; i++){

 if (i<2*Num/5 || i>3*Num/5){
 sp = 8;
 offs=2;
hgth = - (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2))+offs ;
 sp = 50;
 offs=200;
sat =  (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;
sp= 35;
offs=160;
bri = (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;

leds[i]= CHSV (hgth,sat,bri);
//red to orange 0-15, alongside brightness drop 255-0

 

 }
 }
 FastLED.show();
 delay(160);

//set lighter at pixels 3,4
 for (int i=0; i<Num; i+=3){
 sp = 7;
 offs=4;
hgth = - (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2))+offs ;
 sp = 75;
 offs=170;
sat =  (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;
sp= 55;
offs=160;
bri = (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;

leds[i, i+1]= CHSV (hgth,sat,bri);
//red to orange 0-15, alongside brightness drop 255-0

 }
 FastLED.show();
delay(130);

//set darker pixels 5
 for (int i=0; i<Num; i+=5){
 sp = 5;
 offs=0;
hgth = - (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2))+offs ;
 sp = 55;
 offs=190;
sat =  (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;
sp= 35;
offs=160;
bri = (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;

leds[i]= CHSV (hgth,sat,bri);
//red to orange 0-15, alongside brightness drop 255-0

 }
 FastLED.show();
delay(160);



//set even darker pixels 7, 8
 for (int i=0; i<Num; i+=7){
 sp = 7;
 offs=1;
hgth = - (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2))+offs ;
 sp = 55;
 offs=190;
sat =  (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;
sp= 35;
offs=140;
bri = (1 / (1 - sqrt(b))) * (sp) * (pow((i - sqrt(sqrt(b - 1))), 2)) + sp+offs;

leds[i, i+1]= CHSV (hgth,sat,bri);
//red to orange 0-15, alongside brightness drop 255-0

 }
 FastLED.show();
delay(150);

}
