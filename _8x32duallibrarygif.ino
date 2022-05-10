#include "FastLED.h"

#include "array.h"

// Ho w many leds in your strip?
#define NUM_LEDS_A 256

//Data pins
#define DATA_PIN_A 5

int indexcount = 0;

int indexnum = 0;
int indexnum2 = 0;

int globalbrightness=16;

// Define the array of leds
CRGB leds_a[NUM_LEDS_A];

int framecounter = 0;

CRGB colorbuffer[3] = {CRGB::Red, CRGB::Green, CRGB::Blue};






/*------------------------NEOPIXEL------------------*/
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 5

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

String letters[40] = {"a", "b", "c", "d", "e", "f","g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

int redrando=random(0,255);
int greenrando=random(0,255);
int bluerando=random(0,255);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)};

int x    = matrix.width();
int pass = 0;
/*-------------END NEOPIXEL-------------*/

















void setup() {
  // put your setup code here, to run once:
  //Initialize LED strips
  FastLED.addLeds<WS2812B, DATA_PIN_A, GRB>(leds_a, NUM_LEDS_A);
  //  Serial.begin(9600);
  FastLED.setBrightness(globalbrightness);


  //neopixel stuff
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(globalbrightness);

  matrix.setTextColor(colors[0]);
  
}

void loop() {

//   drawclaws();
  
  if(((framecounter%380)/190)==0)
  {
    drawgifinfo();
    FastLED.show();
  } else  
  {
    //FastLED.clear ();
    messagedraw1();
    matrix.show();
  }
  
   

    
  protoshader();
  
  framecounter++;
}

void drawclaws(){

  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 8; j++) {


      if (((i) % 2) == 0) {
        indexnum = (i * 8 + j);
        indexnum2 = indexnum;
//        leds_a[indexnum] = CRGB::Red;
      }
      if (((i) % 2) == 1) {
        indexnum = (i * 8 + j);
        indexnum2 = (i * 8 + (7 - j));
//        leds_a[indexnum] = CRGB::Yellow;                
      }

      if(((framecounter/8)%8)==0){
        leds_a[indexnum2] = CRGB(crabimage[255-indexnum][2], crabimage[255-indexnum][1], crabimage[255-indexnum][0]);
      } else if(((framecounter/8)%8)==(1||3)) {
        leds_a[indexnum2] = CRGB(crabimage2[255-indexnum][2], crabimage2[255-indexnum][1], crabimage2[255-indexnum][0]);
      } else if(((framecounter/8)%8 )==2) {
        leds_a[indexnum2] = CRGB(crabimage3[255-indexnum][2], crabimage3[255-indexnum][1], crabimage3[255-indexnum][0]);
      }
      

      
    }
  }
  delay(5);
}

void drawgifinfo(){

  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 8; j++) {


      if (((i) % 2) == 0) {
        indexnum = (i * 8 + j);
        indexnum2 = indexnum;
//        leds_a[indexnum] = CRGB::Red;
      }
      if (((i) % 2) == 1) {
        indexnum = (i * 8 + j);
        indexnum2 = (i * 8 + (7 - j));
//        leds_a[indexnum] = CRGB::Yellow;                
      }
        int gifframeindex=framecounter%numframes;
          leds_a[indexnum2] = CRGB(animatedgifmatrixfull[gifframeindex][255-indexnum][0], animatedgifmatrixfull[gifframeindex][255-indexnum][1], animatedgifmatrixfull[gifframeindex][255-indexnum][2]);      
    }
  }

  delay(25);
}


void messagedraw1() {
  
  Serial.println("Arduino Run"); 

  //  
  matrix.setTextColor(matrix.Color(redrando,greenrando,bluerando));
  matrix.setCursor(-x*2-120, 1);
  matrix.print(F("<3 <3 <3 "));

  matrix.setTextColor(matrix.Color(255,255,255)); 
  matrix.setCursor(x+61, 0);
  matrix.print(F("to crab, or not to crab?"));  

  if( ++x < 100 ) {
    x = matrix.width();

    if(++pass >= 8) pass = 0;
//    matrix.setTextColor(colors[pass]);

     redrando=int(random(0,2))*255;
     greenrando=int(random(0,2))*255;
     bluerando=int(random(0,2))*255;

  }

  
  matrix.show();
  delay(15);
  matrix.fillScreen(0);    //Turn off all the LEDs
}


void protoshader(){
      for(int i=0;i<256;i++){
            if((leds_a[i].red==255)&&(leds_a[i].green==0)&&(leds_a[i].blue==0)){  
                  leds_a[i]=CRGB(255,255,255);
            }
      }
      
}
