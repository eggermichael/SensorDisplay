/*********************************************************************
// SENSOR DISPLAY NODE

Copyright 2018 Michael Egger

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the 
"Software"), to deal in the Software without restriction, including 
without limitation the rights to use, copy, modify, merge, publish, 
distribute, sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to 
the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

Notes:
 THIS IS A WORK IN PROGRESS

Summary:
 The goal of this program was to have a sensor and a display to
 show the values.
 I am using the SDS011 for measuring dust.
 My arduino is a counterfeit D1 with a modified ESP8266 chip.
 The GPIOs might be different with your board please check the 
 comments  below (NodemCu, D_1).

Wiring:
 No breadboard required. Simply connect the following pins:
  SDS011       arduino
   TXD -------- D12/MISO/D6 (D_1)
   RXD -------- D13/SCK/D6 (D_1)
   5V  -------- 5V (D_1)
   GND -------- GND (D_1)
  OLED_DISPLAY arduino
   SDA -------- D14/SDA (D_1)
   SLC -------- D15/SCL (D_1)
   VCC -------- 3V (D_1)
   GND -------- GND (D_1)

Usage:
 Wire up as shown above and enjoy the display of your PM10 and PM2.5 particle measurements.

// SENSOR DISPLAY NODE
*********************************************************************/


// STRING CONVERSION
#include <string.h>

// WEB SERVER
//#include <ESP8266WiFi.h>
//#include <WiFiClient.h> 


// SDS011 SENSOR
#include <SDS011.h>

//DISPLAY
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  //edit .h according to display version
#define OLED_RESET LED_BUILTIN   //for NodemCu: D4=2 // for D_1: LED_BUILTIN


// SDS011 SENSOR
SDS011 my_sds;
float p10,p25;
int error;

//DISPLAY
Adafruit_SSD1306 display(OLED_RESET);



void setup() {
  Serial.begin(115200);
  my_sds.begin(D13,D12);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  delay(1000);
  // TEST DISPLAY
  Serial.println("Testing Display...\n");
  display.clearDisplay();
  display.display();
  delay(50);
  displayTest("Testing...\n123456789.123456789:123456789|12...\n.\n.\n.\n.\n.");
  Serial.println("Display Test Complete!\n");
}


void loop() {
  Serial.print("-");
  Serial.println("SDS011 starting round");
  for( int i = 0; i < 8 ; i++){
    error = my_sds.read(&p25,&p10);
    if (! error) {
      Serial.println("P2.5: "+String(p25));
      Serial.println("P10:  "+String(p10));
      displayAndClear("PM 10.0:\n " + String(p10) + " ug/m3\n\nPM 2.50:\n " + String(p25) + " ug/m3",1);
    }  else{
       Serial.println("Error fetching SDS011 data");
       displayAndClear("P 10.0\n error\nP 2.50\n error",1);
    }
    delay(1000);
  }
  Serial.println("SDS011 round finished");
  //delay(3000);
}

/// This function is used to display text without
/// the need for clearDisplay (wich causes flickering)
void displayAndClear(String string, int textsize){
  Serial.println("Displaying: " + string);
  // writes text
  display.setCursor(0,0);
  display.setTextSize(textsize);
  display.setTextColor(WHITE);
  display.print(string);
  // refresh display
  display.display();
  // clear buffer without redraw
  display.setCursor(0,0);
  display.setTextColor(BLACK);
  display.print(string);
}


/// Just writes one character at a time from message. 
/// Three runs, increases fontsize from 1 to 2
void displayTest(String text){
  int ml = text.length();
  for(int ts = 1; ts < 3; ts++){
    for (int i = 0; i < ml; i++){
      displayAndClear(text.substring(0,i+1),ts);
      delay(10);
    }
  }
}
