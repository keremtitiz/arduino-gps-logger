#include <SoftwareSerial.h>
SoftwareSerial GPS = SoftwareSerial(4,3);
void setup()
{
  GPS.begin(9600);
  Serial.begin(9600); 

}

void loop() {
  Serial.write(GPS.read());
  delay(100);
}
