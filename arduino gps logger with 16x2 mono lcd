#include <LiquidCrystal.h>
#include <stdlib.h> // convert functions
#include <SoftwareSerial.h> // Gps Communication Library
#include <TinyGPS.h> // Gps Library
#include <SPI.h> 
#include <dht.h> // Arduino Analog Sensor Library
#include <SD.h> // Arduino SD Card Library

#define sd_cs  8 //Sd card Pin

#define dht_dpin A0 //temp & humi pin
dht DHT; //temp & humi

SoftwareSerial nss(3, 2); // serial comm. for gps module

LiquidCrystal lcd(10, 9, 7, 6, 5, 4);

TinyGPS gps; // gps act.
void gpsdump(TinyGPS &gps); 
bool feedgps();

int sayac = 0; //gps count

void setup()
{

  Serial.begin(9600);
  nss.begin(9600);
  lcd.begin(16, 2);
  lcd.print("GPSLOGGER v1.2..");
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("by Kerem TiTiZ");
  Serial.print("GPSLOGGER v1.1 Kerem TiTiZ");
  Serial.println("");
  delay(2000);
  Serial.print("Booting, please wait...");
  Serial.println("");
  lcd.setCursor(0,1);
  lcd.print("Booting, OK....");
  delay(2000);
  Serial.print("Temp & Humi sensors...");
  Serial.println("");
  lcd.setCursor(0,1);
  lcd.print("Sensors init, OK");
  if (!SD.begin(sd_cs)) {
    Serial.print("SD Card fail!!");
    Serial.println("");
    lcd.setCursor(0,1);
    lcd.print("SD Card fail");
    // don't do anything more:
    return;
  }
  Serial.print("SD Card initializing...");
  Serial.println("");
  lcd.setCursor(0,1); 
  lcd.print("SD Card, OK     ");
  delay(2000);
  Serial.print("Gps initializing...");
  Serial.println("");
  lcd.setCursor(0,1);
  lcd.print("GPS init, OK    ");
  delay(2000);
  lcd.clear();  
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
  String lcdDataString1 ="";
  String lcdDataString2 ="";
  String lcdDataString3 ="";
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


      lcdDataString1 = "";
      lcdDataString1 += lat_text;
      lcdDataString1 +="N";
      lcdDataString1 +=" ";
      lcdDataString1 += temp_text;
      lcdDataString1 +="C ";
      lcdDataString1 += humi_text;
      lcdDataString1 +="% ";
      
      
      
      
      lcdDataString2 = "";      
      lcdDataString2 += long_text;
      lcdDataString2 +="E";
      lcdDataString2 +=" ";
      lcdDataString2 +="S";
      lcdDataString2 += speed_text;
      lcdDataString2 +=" A";
      lcdDataString2 += alt_text;
     
     
      lcdDataString3 ="";
      lcdDataString3 += day_text;
      lcdDataString3 +=".";
      lcdDataString3 += month_text;
      lcdDataString3 +=".";
      lcdDataString3 += year_text;
      lcdDataString3 +=" ";
      lcdDataString3 += hour_text;
      lcdDataString3 +=":";
      lcdDataString3 += minute_text;

      lcd.clear();
      lcd.print(lcdDataString1);
      lcd.setCursor(0,1);
     // lcd.print(lcdDataString3);
     // delay(1000);
     // lcd.setCursor(0,1);
      lcd.print(lcdDataString2);
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
