#include <LiquidCrystal.h>
#include <stdlib.h> // convert functions
#include <SoftwareSerial.h> // Gps Communication Library
#include <TinyGPS.h> // Gps Library
#include <SPI.h> 
#include <dht.h> // Arduino Analog Sensor Library
#include <SD.h> // Arduino SD Card Library


// TFT pin definition for the Uno
#define sd_cs  8

//temp & humi
#define dht_dpin A0

TinyGPS gps; // gps act.
SoftwareSerial nss(3, 2); // serial comm. for gps module
void gpsdump(TinyGPS &gps); 
bool feedgps();
void printFloat(double f, int digits = 2);
int sayac = 0; //gps count
dht DHT; //temp & humi
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);


void setup()
{
  
    Serial.begin(9600);
    nss.begin(9600);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("GPSLOGGER v1.1");
    lcd.setCursor(0, 2);
    lcd.print("Kerem TiTiZ");
    Serial.print("GPSLOGGER v1.1 Kerem TiTiZ");
    Serial.println("");
    delay(2000);
    Serial.print("Booting, please wait...");
    Serial.println("");
    lcd.print("Booting,OK");
    delay(2000);
    Serial.print("Temp & Humi sensors...");
    Serial.println("");
    lcd.print("Sensors,OK");
    delay(2000);
    Serial.print("Gps initializing...");
    Serial.println("");
    lcd.print("GPS,OK");
    delay(2000);
    if (!SD.begin(sd_cs)) {
    
      Serial.print("SD Card fail...");
      Serial.println("");
      lcd.print("SD Card fail");
      // don't do anything more:
     return;
    }
    Serial.print("SD Card initializing...");
    Serial.println("");
    lcd.print("SD Card,OK");
    
    delay(2000);
}

void loop()
{


 
  // gps variables
    long lat, lon;
    float flat, flon;
    unsigned long age, date, time, chars;
    int year;
    byte month, day, hour, minute, second, hundredths;
    unsigned short sentences, failed;
    unsigned long start = millis();
    String dataString ="";
    bool newdata = false;
    
  
  // int to char variables
    char lat_text[8]="";
    char long_text[8]="";
    char day_text[3]="";
    char month_text[3]="";
    char year_text[5]="";
    char hour_text[3]="";
    char minute_text[3]="";
    char second_text[3]="";
    char speed_text[4]="";
    char alt_text[5]="";
    char head_text[5]="";
    char temp_text[3]="";
    char humi_text[3]="";
    char sayac_text[8]="";
    while (millis() - start < 5000)
      {
        if (feedgps())
          newdata = true;
      }
 
   if (newdata) {
  
    gps.f_get_position(&flat, &flon);
    dtostrf(flat,0,4,lat_text);
    dtostrf(flon,0,4,long_text);
    
    feedgps();
    
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths);
    

    dtostrf(static_cast<int>(day),0,0,day_text);
    dtostrf(static_cast<int>(month),0,0,month_text);
    dtostrf(static_cast<int>(year),0,0,year_text);
    dtostrf(static_cast<int>(hour)+3,0,0,hour_text);
    dtostrf(static_cast<int>(minute),0,0,minute_text);
    dtostrf(static_cast<int>(second),0,0,second_text);

    dtostrf(sayac,0,0,sayac_text);
    
    feedgps();

    dtostrf(gps.f_altitude(),0,0,alt_text);
    dtostrf(gps.f_course(),0,0,head_text);
    dtostrf(gps.f_speed_kmph(),0,0,speed_text);

    feedgps();
    
    DHT.read11(dht_dpin);
    dtostrf(DHT.temperature,0,0,temp_text);
    dtostrf(DHT.humidity,0,0,humi_text);

   // Sd Card open file for write
   File dataFile = SD.open("gpsdata.txt", FILE_WRITE);
   
   // write variables to file
    if (dataFile) {
      //SD card prepare variables;  
       dataString = "";
       dataString += sayac;  
       dataString += "|";
       dataString += lat_text;
       dataString += "|";
       dataString += long_text;
       dataString += "|";
       dataString += day_text;
       dataString += ".";
       dataString += month_text;
       dataString += ".";
       dataString += year_text;
       dataString += "|";
       dataString += hour_text;
       dataString += ":";
       dataString += minute_text;
       dataString += ":";
       dataString += second_text;
       dataString += "|";
       dataString += speed_text;
       dataString += "|";
       dataString += alt_text;
       dataString += "|";
       dataString += head_text;
       dataString += "|";
       dataString += temp_text;
       dataString += "|";
       dataString += humi_text;
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    } 
   
    sayac = sayac + 1;

}
   
}


bool feedgps()
{
  while (nss.available())
  {
    if (gps.encode(nss.read()))
      return true;
  }
  return false;
}
