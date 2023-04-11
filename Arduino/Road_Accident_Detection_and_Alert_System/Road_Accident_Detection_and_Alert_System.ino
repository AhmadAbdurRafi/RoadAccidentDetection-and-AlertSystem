#include<SoftwareSerial.h>
#include <TinyGPS.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(4,5,6,7,8,9);
SoftwareSerial Serial1(2,3); //rx=2, tx=3

TinyGPS gps;

#define x A0
#define y A1
#define z A2

float initX, initY, initZ;
float latitude, longitude;
float thresholdVal = 100;
bool flag = false;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);  
  initX = analogRead(x);
  initY = analogRead(y);
  initZ = analogRead(z);
  
  lcd.print("Accident");
  lcd.setCursor(0,1);
  lcd.print("Detection System");
  lcd.clear();  
}
void loop()
{
    float valueX = analogRead(x); 
    float valueY = analogRead(y);
    float valueZ = analogRead(z); 

    float difX = abs(valueX-initX);
    float difY = abs(valueY-initY);
    float difZ = abs(valueZ-initZ);
    if(difX>=thresholdVal || difY>=thresholdVal || difZ >=thresholdVal){
      Serial.println(valueX);
      Serial.println(valueY);
      Serial.println(valueZ);
      if(!flag){
        get_gps();
        flag = true;
      }
      Serial.println(latitude);
      Serial.println(longitude);
      Serial.println("Accident_occured");
      lcd.print("Accident");
      lcd.setCursor(0,1);
      lcd.print("Detected!!");
      delay(500);
      lcd.clear();
    }
    else{
      Serial.println(valueX);
      Serial.println(valueY);
      Serial.println(valueZ);
      delay(1000);
    }
}
 void get_gps()
{ 
    bool newData = false;
    unsigned long chars;
    unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 500;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      //Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }

  if (newData)      //If newData is true
  {

    unsigned long age;
    gps.f_get_position(&latitude, &longitude, &age);   

  }

 }
 void Send()
{ 
   Serial1.write("AT");
   Serial1.write("AT+CMGF=1");
   Serial1.write("AT+CMGS=");
   Serial1.print("+8801878124795");    //mobile no. for SMS alert
   Serial1.println("");
   delay(500);
   Serial1.println(longitude);
   Serial1.print(latitude);
   delay(500);
   String strlongitude = String(longitude,2);
   String strlatitude = String(latitude,2);
   String msg  = "Accident occured here: http://maps.google.com/maps?&z=15&mrt=yp&t=k&q="+strlongitude+strlatitude;
   Serial.print(msg);
   delay(2000);
}
