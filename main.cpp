/*
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

#define SD_FAT_TYPE 3

#define SPI_SPEED SD_SCK_MHZ(4)

#if SD_FAT_TYPE == 0
SdFat sd;
File file;
#elif SD_FAT_TYPE == 1
SdFat32 sd;
File32 file;
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile file;
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile file;
#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

HardwareSerial GPS_Module(1); // use UART1

TinyGPSPlus gps;


void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  GPS_Module.begin(9600, SERIAL_8N1, 3, 2); 

  while(!Serial);
    Serial.println("Initializing");

  
  delay(5000);
  Serial.println("Test");

  GPS_Module.println("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29<CR><LF>");

  GPS_Module.println("$PMTK220,100*2F<CR><LF>");
  //GPS_Module.println("$PMTK251,115200*1F<CR><LF>");

  //GPS_Module.begin(9600, SERIAL_8N1, 9, 10); 
  
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (GPS_Module.available() > 0)
    Serial.print(char(GPS_Module.read()));
    gps.encode(GPS_Module.read());

    
    Serial.print("LAT="); Serial.print(gps.location.lat(), 6);
    Serial.print(" ");
    Serial.print("LNG="); Serial.println(gps.location.lng(), 6);
    Serial.println(gps.time.value());
    Serial.println("");
    delay(1000);
    

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

*/


// Quick hardware test for SPI card access.
#define _GLIBCXX_USE_CXX11_ABI 0

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <time.h>
#include <string>

#define SCK  8
#define MISO  9
#define MOSI  10
#define CS  4

File myFile;
SPIClass myspi = SPIClass(HSPI);

String GPS_DATA = "";
int GPS_DATA_INT = 0;
std::string GPS_String = "dsfsfsdf";

HardwareSerial GPS_Module(1); // use UART1

TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);

  

  delay(2000);

  GPS_Module.begin(9600, SERIAL_8N1, 3, 2);

  GPS_Module.println("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29<CR><LF>");

  //GPS_Module.println("$PMTK220,100*2F<CR><LF>");

  myspi.begin(8,9,10,4);
 
  Serial.print("Initializing SD card...");

  while (!SD.begin(4)) {
    Serial.println("initialization failed!");
  }
  Serial.println("initialization done.");

  myFile = SD.open("/test.txt", FILE_APPEND);

  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 3, 2, 1.");
    

    
    
    Serial.println("done.");
    //myFile.close();
  } 
  else {
    Serial.println("error opening test.txt");
  }
  
}

void loop() {
  /*
  while (GPS_Module.available() > 0)
    //while(myFile.read()!= -1)
    //Serial.write(myFile.read());
    //Serial.print(char(GPS_Module.read()));
    //GPS_DATA_INT = GPS_Module.read();
    Serial.print(char(GPS_Module.read()));
    //Serial.println(GPS_DATA_INT);
    
    //while (32 <= GPS_Module.read() <= 127)
    //{
      //GPS_DATA += char(GPS_Module.read());
    //}
    
    myFile = SD.open("/test.txt", FILE_WRITE);

    if (myFile) {
      myFile.println(char(GPS_Module.read()));

      while (myFile.available()) {
      Serial.write(myFile.read());
      }

      myFile.close();
    }
    delay(100);*/
  
    myFile = SD.open("/test.txt", FILE_APPEND);
    //while (GPS_Module.available() > 0)
      //Serial.println(GPS_Module.read());
      //GPS_String += char(GPS_Module.read());

    //Serial.println(GPS_String); 
    Serial.println("Writing to test.txt...");
    myFile.println("testing 3, 2, 1.");
    myFile.close();
    delay(1000);
  
  
}


