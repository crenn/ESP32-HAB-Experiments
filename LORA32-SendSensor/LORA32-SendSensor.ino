#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <U8x8lib.h>
//#include "images.h"

//U8x8 display library configuration
//See U8g2/U8x8 documentation for available display write functions

#include <U8x8lib.h>
#define OLED_RESET  16
#define OLED_SCL    15
#define OLED_SDA    4
//For Heltec Wifi LoRa 32, TTGO LoRa and TTGO LoRa32 V1 use:
U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ OLED_RESET, /*scl*/ OLED_SCL, /*sda*/ OLED_SDA);


#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

unsigned long delayTime;

#define SCK 5 // GPIO5 - SX1278's SCK
#define MISO 19 // GPIO19 - SX1278's MISO
#define MOSI 27 // GPIO27 - SX1278's MOSI
#define SS 18 // GPIO18 - SX1278's CS
#define RST 14 // GPIO14 - SX1278's RESET
#define DI0 26 // GPIO26 - SX1278's IRQ (interrupt request)
#define BAND 915E6

unsigned int counter = 0;
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
  Serial.println ("LoRa Sensor Sender Test");
  
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

  if (Wire.begin(OLED_SDA, OLED_SCL, 100000))
    Serial.println("I2C Initialised");
  else
    Serial.println("I2C Uninitialised");

  unsigned status;
  status = bme.begin(0x76);

  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
  }
  
  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
    
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

  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();

  // send packet
  LoRa.beginPacket ();
  LoRa.print("Temperature = ");
  LoRa.print(temperature);
  LoRa.print(" *C");
  LoRa.print("\r\nHumidity = ");
  LoRa.print(humidity);
  LoRa.println(" %");
  LoRa.endPacket ();

  counter ++;
  digitalWrite (2, HIGH); // turn the LED on (HIGH is the voltage level)
  delay (1000); // wait for a second
  digitalWrite (2, LOW); // turn the LED off by making the voltage LOW
  delay (1000); // wait for a second
}
