#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

#define SCK 5 // GPIO5 - SX1278's SCK
#define MISO 19 // GPIO19 - SX1278's MISO
#define MOSI 27 // GPIO27 - SX1278's MOSI
#define SS 18 // GPIO18 - SX1278's CS
#define RST 14 // GPIO14 - SX1278's RESET
#define DI0 26 // GPIO26 - SX1278's IRQ (interrupt request)
#define BAND 915E6

//U8x8 display library configuration
//See U8g2/U8x8 documentation for available display write functions

#include <U8x8lib.h>
#define OLED_RESET  16
#define OLED_SCL    15
#define OLED_SDA    4
//For Heltec Wifi LoRa 32, TTGO LoRa and TTGO LoRa32 V1 use:
U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ OLED_RESET, /*scl*/ OLED_SCL, /*sda*/ OLED_SDA);
String rssi = "RSSI -";
String packSize = "-";
String packet;

#define PROJECT_FONT  u8x8_font_pxplusibmcga_r



void loraData () {
  display.clear ();
//  display.setTextAlignment (TEXT_ALIGN_LEFT);
  display.setFont (PROJECT_FONT);
  display.println(rssi);
  display.println();
  display.println("Received " + packSize + " bytes");
  display.println(packet);
  
  display.display ();
}

void cbk (int packetSize) {
  packet = "";
  packSize = String (packetSize, DEC);
  rssi = "RSSI " + String (LoRa.packetRssi (), DEC);
  while (LoRa.available() > 0)
    packet = packet + (char)LoRa.read();
  loraData ();
}

void setup () {
  pinMode (16, OUTPUT);
  digitalWrite (16, LOW); // set GPIO16 low to reset OLED
  delay (50);
  digitalWrite (16, HIGH); // while OLED is running, GPIO16 must go high,
  
  Serial.begin (115200);
  while (! Serial);
  Serial.println ();
  Serial.println ("LoRa Receiver Callback");
  SPI.begin (SCK, MISO, MOSI, SS);
  LoRa.setPins (SS, RST, DI0);
  if (! LoRa.begin (BAND)) {
    Serial.println ("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
  LoRa.receive ();
  Serial.println ("init ok");
  display.begin();
  display.setFont(PROJECT_FONT);   //Optional, more fonts are available.
  display.clear();
  display.println("Waiting for LoRa packet");
  
  delay (1500);
}

void loop () {
  int packetSize = LoRa.parsePacket ();
  if (packetSize) {cbk (packetSize); }
  delay (10);
}
