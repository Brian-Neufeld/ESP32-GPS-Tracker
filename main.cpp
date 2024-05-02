
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

HardwareSerial SerialPort(1); // use UART1

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
  SerialPort.begin(9600, SERIAL_8N1, 9, 10); 

  while(!Serial);
    Serial.println("Initializing");

  
  delay(5000);
  Serial.println("Test");

  SerialPort.println("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29<CR><LF>");

  SerialPort.println("$PMTK220,100*2F<CR><LF>");
  //SerialPort.println("$PMTK251,115200*1F<CR><LF>");

  //SerialPort.begin(9600, SERIAL_8N1, 9, 10); 
  
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (SerialPort.available() > 0)
    Serial.print(char(SerialPort.read()));
    /*gps.encode(SerialPort.read());

    
    Serial.print("LAT="); Serial.print(gps.location.lat(), 6);
    Serial.print(" ");
    Serial.print("LNG="); Serial.println(gps.location.lng(), 6);
    Serial.println(gps.time.value());
    Serial.println("");
    delay(1000);*/
    

  /*if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }*/
}

