#include <stdlib.h> // convert functions
#include <SoftwareSerial.h> // Gps Communication Library
#include <TinyGPS.h> // Gps Library
#include <TFT.h>  // Arduino LCD library
#include <SPI.h> 
#include <dht.h> // Arduino Analog Sensor Library
#include <SD.h> // Arduino SD Card Library


// TFT pin definition for the Uno
#define sd_cs  4
#define cs   10
#define dc   9
#define rst  8  

//temp & humi
#define dht_dpin A0

TinyGPS gps; // gps act.
SoftwareSerial nss(2, 3); // serial comm. for gps module
TFT TFTscreen = TFT(cs, dc, rst); //lcd
int ledPin =  7; //activity led
void gpsdump(TinyGPS &gps); 
bool feedgps();
void printFloat(double f, int digits = 2);
int sayac = 0; //gps count
dht DHT; //temp & humi



void setup()
{
  
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // software serial begin

    nss.begin(9600);
    
  //pinMode(ledPin, OUTPUT); //led check

  // TFT Start

    TFTscreen.begin();

  // TFT Gui
    
    TFTscreen.setTextSize(1);
    TFTscreen.background(0, 0, 0);
    TFTscreen.stroke(224,255,255);
 
 // begin lcd layout
 
    TFTscreen.text("GPSLOGGER v1.1 Kerem TiTiZ",2,2);
    Serial.print("GPSLOGGER v1.1 Kerem TiTiZ");
    Serial.println("");
    
    TFTscreen.rect(0, 12, 160, 106);
 
 // boot & sensors & gps loader
 
    TFTscreen.text("Booting, please wait...",4,20);
    Serial.print("Booting, please wait...");
    Serial.println("");

    delay(2000);
    
    TFTscreen.text("Temp & Humi sensors...",4,40);
    Serial.print("Temp & Humi sensors...");
    Serial.println("");
    
    delay(2000);
    
    TFTscreen.text("Gps initializing...",4,60);
    Serial.print("Gps initializing...");
    Serial.println("");
    
    delay(2000);
    
    if (!SD.begin(sd_cs)) {
    
      TFTscreen.text("SD Card fail...",4,80);
      Serial.print("SD Card fail...");
      Serial.println("");
      // don't do anything more:
     return;
    }
    TFTscreen.text("SD Card initializing...",4,80);
    Serial.print("SD Card initializing...");
    Serial.println("");
    
    delay(2000);
    
    TFTscreen.stroke(0,0,0);
    TFTscreen.text("Booting, please wait...",4,20);
    TFTscreen.text("Temp & Humi sensors...",4,40);
    TFTscreen.text("Gps initializing...",4,60);
    TFTscreen.text("SD Card fail...",4,80);
    TFTscreen.text("SD Card initializing...",4,80);
    TFTscreen.stroke(224,255,255);
    
  // main display lcd layout
  
  // legand
    TFTscreen.setTextSize(3);
    TFTscreen.text("N",137,16);
    TFTscreen.text("E",137,44);
    TFTscreen.setTextSize(2);
    TFTscreen.text("A",4,74);
    TFTscreen.text("H",100,74);
    TFTscreen.text("S",4,99);
  
  // units
   
    TFTscreen.setTextSize(1); 
    TFTscreen.text("KM",68,98);
    TFTscreen.text("H",68,108);
    TFTscreen.text("Mt",78,74);
    TFTscreen.text("o",151,72);
    
 // frame lines
   
    TFTscreen.line(0,40,160,40);
    TFTscreen.line(0,68,160,68);
    TFTscreen.line(0,94,160,94);
    TFTscreen.line(94,68,94,94);
    TFTscreen.line(84,94,84,117);
    TFTscreen.line(120,94,120,117);

  // close main display lcd layout

    TFTscreen.noStroke();
    TFTscreen.stroke(255,255,255);



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
    

    
    
    // Temp & Humi display
  
    TFTscreen.setTextSize(2);
    TFTscreen.stroke(255,0,0); 
    DHT.read11(dht_dpin);
    dtostrf(DHT.temperature,0,0,temp_text);
    TFTscreen.text(temp_text,88,99);
    TFTscreen.setTextSize(1);
    TFTscreen.text("o",112,96);
    TFTscreen.text("C",112,106);
    TFTscreen.stroke(0,0,255);
    TFTscreen.setTextSize(2);
    dtostrf(DHT.humidity,0,0,humi_text);
    TFTscreen.text(humi_text,125,98);
    TFTscreen.setTextSize(1);
    TFTscreen.text("%",150,98);
    
    while (millis() - start < 5000)
      {
        if (feedgps())
          newdata = true;
      }
 
   if (newdata) {
  
  // GPS Display
  
    TFTscreen.stroke(255,255,255);
    TFTscreen.setTextSize(3);
    gps.f_get_position(&flat, &flon);
    dtostrf(flat,0,4,lat_text);
    TFTscreen.text(lat_text,4,16); //La
    dtostrf(flon,0,4,long_text);
    TFTscreen.text(long_text,4,44); //lo
    
    feedgps();
    
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths);
    
    TFTscreen.setTextSize(1);
    dtostrf(static_cast<int>(day),0,0,day_text);
    TFTscreen.text(day_text,0,120);
    TFTscreen.text(".",10,120);
    dtostrf(static_cast<int>(month),0,0,month_text);
    TFTscreen.text(month_text,15,120);
    TFTscreen.text(".",20,120);
    dtostrf(static_cast<int>(year),0,0,year_text);
    TFTscreen.text(year_text,30,120);
    dtostrf(static_cast<int>(hour)+3,0,0,hour_text);
    TFTscreen.text(hour_text,60,120);
    TFTscreen.text(":",72,120);
    dtostrf(static_cast<int>(minute),0,0,minute_text);
    TFTscreen.text(minute_text,78,120);
    TFTscreen.text(":",88,120);
    dtostrf(static_cast<int>(second),0,0,second_text);
    TFTscreen.text(second_text,94,120);
    
    // GPS Log line counter display
    TFTscreen.text("C:",110,120);
    dtostrf(sayac,0,0,sayac_text);
    TFTscreen.text(sayac_text,124,120);
    
    feedgps();

    TFTscreen.setTextSize(2);
    dtostrf(gps.f_altitude(),0,0,alt_text);
    TFTscreen.text(alt_text,18,74); //ALT
  
    dtostrf(gps.f_course(),0,0,head_text);
    TFTscreen.text(head_text,114,74); //HEAD

    dtostrf(gps.f_speed_kmph(),0,0,speed_text);
    TFTscreen.text(speed_text,18,99); //SPEED

    feedgps();

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
   
   delay(5000); 
 
    TFTscreen.stroke(0,0,0);
    TFTscreen.setTextSize(2);
    TFTscreen.text(temp_text,88,99);
    TFTscreen.text(humi_text,125,98);

    TFTscreen.stroke(0,0,0);
    TFTscreen.setTextSize(3);
    TFTscreen.text(lat_text,4,16);
    TFTscreen.text(long_text,4,44);
    TFTscreen.setTextSize(1);
    TFTscreen.text(day_text,0,120);
    TFTscreen.text(month_text,15,120);
    TFTscreen.text(year_text,30,120);
    TFTscreen.text(hour_text,60,120);
    TFTscreen.text(minute_text,78,120);
    TFTscreen.text(second_text,94,120);
    TFTscreen.text(sayac_text,124,120);
    TFTscreen.setTextSize(2);
    TFTscreen.text(alt_text,18,74);
    TFTscreen.text(head_text,114,74);
    TFTscreen.text(speed_text,18,99);
 
 }
  
 
   
    
   
    
    

   
}

/*void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0)
  {
    TFTscreen.print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  TFTscreen.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    TFTscreen.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    TFTscreen.print(toPrint);
    remainder -= toPrint; 
  } 
}

void gpsdump(TinyGPS &gps)
{


}*/

bool feedgps()
{
  while (nss.available())
  {
    if (gps.encode(nss.read()))
      return true;
  }
  return false;
}
