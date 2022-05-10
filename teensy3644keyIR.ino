/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>
#include "IRbuttons.h"

#define POWER 0xFF629D
#define A 0x00FF22DD
#define B 0x00FF02FD
#define C 0x00FFC23D
#define UP 0x00FF9867
#define DOWN 0x00FF38C7
#define LEFT 0x00FF30CF
#define RIGHT 0x00FF7A85
#define SELECT 0x00FF18E7






int RECV_PIN = 22;

int recbuffer = 0;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  IRremotedebugger();
  delay(100);
}


void IRremotedebugger(){
  if (irrecv.decode(&results)) {
    
    recbuffer=results.value;
    Serial.println(recbuffer, HEX);

    switch(recbuffer){
      case POWER:
        Serial.println("IR_BUTTON_R0C0");
        break;
      default:
        break;  
    }


    irrecv.resume(); // Receive the next value
  }
}
