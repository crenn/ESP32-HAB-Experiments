#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <U8x8lib.h>
//#include "images.h"

#define SCK 5 // GPIO5 - SX1278's SCK
#define MISO 19 // GPIO19 - SX1278's MISO
#define MOSI 27 // GPIO27 - SX1278's MOSI
#define SS 18 // GPIO18 - SX1278's CS
#define RST 14 // GPIO14 - SX1278's RESET
#define DI0 26 // GPIO26 - SX1278's IRQ (interrupt request)
#define BAND 915E6

unsigned int counter = 0;

//For Heltec Wifi LoRa 32, TTGO LoRa and TTGO LoRa32 V1 use:
U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ 16, /*scl*/ 15, /*sda*/ 4);
String rssi = "RSSI -";
String packSize = "-";
String packet;


void setup () {
  pinMode (16, OUTPUT);
  pinMode (2, OUTPUT);
  
  digitalWrite (16, LOW); // set GPIO16 low to reset OLED
  delay (50);
  digitalWrite (16, HIGH); // while OLED is running, GPIO16 must go high
  
  Serial.begin (115200);
  while (! Serial);
  Serial.println ();
  Serial.println ("LoRa Sender Test");
  
  SPI.begin (SCK, MISO, MOSI, SS);
  LoRa.setPins (SS, RST, DI0);
  if (! LoRa.begin (BAND)) {
    Serial.println ("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
// LoRa.receive ();
  Serial.println ("init ok");
  display.begin();
  display.setFont(u8x8_font_victoriamedium8_r);   //Optional, more fonts are available.
  display.clear ();
  //display.flipScreenVertically ();
  //display.setFont (ArialMT_Plain_10);
  delay (1500);
}

void loop () {
  display.clear ();
  //display.setTextAlignment (TEXT_ALIGN_LEFT);
  display.setFont(u8x8_font_victoriamedium8_r);   //Optional, more fonts are available.

  display.setCursor(0, 0);
  display.println("Sending packet:");
  display.print(String (counter));
  display.display ();

  // send packet
  LoRa.beginPacket ();
  LoRa.print ("hello");
  LoRa.print (counter);
  LoRa.endPacket ();

  counter ++;
  digitalWrite (2, HIGH); // turn the LED on (HIGH is the voltage level)
  delay (1000); // wait for a second
  digitalWrite (2, LOW); // turn the LED off by making the voltage LOW
  delay (1000); // wait for a second
}
