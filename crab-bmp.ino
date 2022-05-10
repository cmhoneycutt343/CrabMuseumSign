//SD card includes
 
#include <SD.h>
#include <SPI.h>
#include <FastLED.h>

File myBMP;

/*----SD card variables-----*/
uint8_t headerdata[54];
uint8_t pixldata[1536];
uint8_t incomingByte;
int load_inc = 0;

bool readbusy = false; 

////char bmptempname[] = "CHESAP~1.bmp";
////char bmptempname[] = "FIDDLE~1.bmp";
////char bmptempname[] = "DUNGEN~1.bmp";
////char bmptempname[] = "OREGON~1.bmp";
////char bmptempname[] = "KINGCRAB.bmp";
//char bmptempname[] = "SALLYL~1.bmp";

char *crabnames[] = {"CHESAP~1.bmp","FIDDLE~1.bmp","DUNGEN~1.bmp","OREGON~1.bmp","KINGCRAB.bmp","SALLYL~1.bmp"};

int framecounter = 0;
int crabroller = 0;
/*-----end SD card Variables----*/

/*----LED Related Varibles-------*/
const int chipSelect = BUILTIN_SDCARD;

// How many leds in your strip?
#define NUM_LEDS_A 384

//Data pins
#define DATA_PIN_A 5

int indexcount = 0;

int downstripe = 0;
int indexnum= 0;

int globalbrightness=17;

// Define the array of leds
CRGB leds_a[NUM_LEDS_A];
CRGB pixelloadbuffer;


CRGB colorbuffer[3] = {CRGB::Blue, CRGB::Orange, CRGB::Yellow};
/*-------end LED------*/


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

void setup()
{ 
  /*-----intialize Pixels---------*/
  FastLED.addLeds<WS2812B, DATA_PIN_A, GRB>(leds_a, NUM_LEDS_A);

  //  Serial.begin(9600);
  FastLED.setBrightness(globalbrightness);

  //neopixel stuff
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(globalbrightness);
  matrix.setTextColor(colors[0]);

//test pixel
//  leds_a[0] = CRGB::Red;
//  FastLED.show();
  /*-----End intialize Pixels---------*/



  /*------------intialize / Read .bmp file------------*/
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
//   while (!Serial) {
//    ; // wait for serial port to connect. Needed for Leonardo only
//  }

  //load pixels of first crab drawing
  loadfile(crabnames[0]);
}

void loop()
{
    //once reading is done
    if(!readbusy){
      drawcrab();     
    }
          
    //every 30 frames.....
    if(framecounter==30){
      //load the next crab pixels
      loadfile(crabnames[crabroller%6]);

      FastLED.show();

      //reset the crab
      framecounter = 0;
      crabroller++;
    }

    //frameloop stuff
    framecounter++;
    delay(50);
}

void drawcrab(){
    //load 8x32
    for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 32; i++) {
            
            if (((i) % 2) == 0) {
              downstripe = (i * 8 + j);
              
                           
              //leds_a[indexnum] = CRGB::Red;
            }
            
            if (((i) % 2) == 1) {
              downstripe = (i * 8 + (7 - j));
              
              //leds_a[indexnum] = CRGB::Blue;                
            }  

            //color read as 'BGR'
            int packet_rowadj = i*3+3*32*j;
            
            pixelloadbuffer.b=pixldata[packet_rowadj];
            pixelloadbuffer.g=pixldata[packet_rowadj+1];
            pixelloadbuffer.r=pixldata[packet_rowadj+2];
          
            leds_a[downstripe]=pixelloadbuffer;
            
            
          }
     }


       
     /*--end load 8x32----*/

     //draw left claw
     for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 8; i++) {
            
            if (((i) % 2) == 0) {
              downstripe = (i * 8 + (j));
              
                           
              //leds_a[indexnum] = CRGB::Red;
            }
            
            if (((i) % 2) == 1) {
              downstripe = (i * 8 + (7 - j));
              
              //leds_a[indexnum] = CRGB::Blue;                
            }  

            //color read as 'BGR'
            int pixeloffset=0;
            int packet_rowadj = (i+24)*3+3*32*(j+8)+pixeloffset;

           
            
            pixelloadbuffer.b=pixldata[packet_rowadj];
            pixelloadbuffer.g=pixldata[packet_rowadj+1];
            pixelloadbuffer.r=pixldata[packet_rowadj+2];
          
            leds_a[downstripe+256+pixeloffset]=pixelloadbuffer;

            
          }
     }

      //draw right claw
     for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 8; i++) {
            
            if (((i) % 2) == 0) {
              downstripe = (i * 8 + (j));
              
                           
              //leds_a[indexnum] = CRGB::Red;
            }
            
            if (((i) % 2) == 1) {
              downstripe = (i * 8 + (7 - j));
              
              //leds_a[indexnum] = CRGB::Blue;                
            }  

            //color read as 'BGR'
            int pixeloffset=0;
            int packet_rowadj = (i)*3+3*32*(j+8)+pixeloffset;

           
            
            pixelloadbuffer.b=pixldata[packet_rowadj];
            pixelloadbuffer.g=pixldata[packet_rowadj+1];
            pixelloadbuffer.r=pixldata[packet_rowadj+2];
          
            leds_a[downstripe+320+pixeloffset]=pixelloadbuffer;

            
          }
     } 
     
}

void loadfile(char bmpname[]){
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    //return;
  }
  Serial.println("initialization done.");
  
  // open the file. 
  //myBMP = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  
  // re-open the file for reading:
  myBMP = SD.open(bmpname);
  if (myBMP) {

    load_inc = 0;
    
    // read from the file until there's nothing else in it:
    readbusy=true;
    
    while (myBMP.available()) {
     incomingByte = myBMP.read();

     Serial.println("");

     //load header and pixel arrays
     
     
     if(load_inc<54)
     {
        Serial.print("header byte:");
        Serial.println(load_inc); 
        Serial.print(incomingByte);
        

        //if last byte of header
        if(load_inc==53){
           //display some header data
           headerdata[load_inc] = incomingByte;
          Serial.print("");
          Serial.print("image width:");
          Serial.print(headerdata[18]);
          Serial.print("");
          Serial.print("image height:");
          Serial.print(headerdata[22]);             
        }
        
     } else {
        Serial.print("pixel data byte:");
        Serial.println(load_inc-54); 
        Serial.print(incomingByte);
        pixldata[load_inc-54] = incomingByte;
     }
     
     load_inc++;
    }

    //write first line of pixels
    Serial.println("//write first line of pixels");  

    //i represents columns, j represents rows
    
  


    myBMP.close();

    readbusy=false;
    
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
