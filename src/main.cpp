#include <Arduino.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>
#include <OneWire.h>
#include <DallasTemperature.h>


//*Pins
// initiating the Pins
// receiving Pins
const int DHTPIN = D3;     //completed
const int HYGROGENATOR_PIN = D6; //completed
const int DB18S20 = D4; //completed

// output Pins
const int UAH = D5; //completed
const int water_pump = D8; //completed
const int fertilizer_pump = D7; //completed
const int growlight = D0; //completed


// needed config for HYGROGENATOR
const int needed_air_temp= 26; //adjustable
const int needed_soil_temp = 29; //adjustable
const int needed_humidity_percentage = 60; //adjustable
const int needed_moisture_percentage = 70; //adjustable
const int AirValue = 561;   //replace the value with value when placed in air using calibration code 
const int WaterValue = 310; //replace the value with value when placed in water using calibration code 

// needed config for Fertilizer
const int duration_before_fertlizer_in_hours=1 ;



// Misc
int soilMoistureValue = 0;
int soilmoisturepercent=0;
int currentsecond;
int tempsecond = 1800;//some delay before spraying




//*Setup
//LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
/*
*SCL = D1
*SDA = D2
*VCC = 5V
*GND = GND
*/

// DHT22 setup
DHT dht(DHTPIN, DHT22);

// DS18B20 setup
OneWire oneWire(DB18S20);
DallasTemperature sensors(&oneWire);


//*Functions
int readDS18B20(){
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    return tempC;
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
    return 0;
  }
  
}


int read_hygrometer(){
  soilMoistureValue = analogRead(HYGROGENATOR_PIN);  // put Sensor insert into soil
  Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

  if(soilmoisturepercent >= 100)
  {

    return 100;

  }
  else if(soilmoisturepercent <=0)
  {

    return 0;
  }

  else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
  {
    
    return soilmoisturepercent;
  
  }
  return soilmoisturepercent;
}




void setup() 
{
  pinMode(growlight,OUTPUT);
  pinMode(water_pump,OUTPUT);
  pinMode(fertilizer_pump,OUTPUT);
  pinMode(UAH,OUTPUT);


  sensors.begin();
Serial.begin(115200); 
dht.begin();
lcd.begin(16,2);
lcd.init();
lcd.backlight();
lcd.setCursor(5, 0);
lcd.print("start");
delay(1000);
}

void loop() 
{
  currentsecond= second();
  // Wait a few seconds between measurements.
  delay(3000);
  lcd.clear();
  float hum = dht.readHumidity();
  float tempC = dht.readTemperature();

    // Check if readings have failed
  if (isnan(hum) || isnan(tempC)) 
  {
      lcd.print("Failed to read");
      lcd.setCursor(0,1);
      lcd.print("from DHT sensor!");
      // return;
  }     

  // humidity
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity: "); 
  lcd.print(hum);
  lcd.print("%");

  // air temp
  lcd.setCursor(0, 1);
  lcd.print("Air Temp: "); 
  lcd.print(tempC);
  lcd.print(" *C ");
  delay(3500);


  int moisturelevel = read_hygrometer();
  int soiltempc = readDS18B20();
  lcd.clear();


  //moisture 
  lcd.setCursor(0,0);
  lcd.print("moisture: ");
  lcd.print(moisturelevel);
  lcd.print("%");

  //soil temp
  lcd.setCursor(0, 1);
  lcd.print("Soil Temp: ");
  lcd.print(soiltempc);
  lcd.print(" *C ");


if (((currentsecond - tempsecond) /3600) >= duration_before_fertlizer_in_hours)
{
  /*
    this is where the code for spraying it will be
    turn on spray
  */
  digitalWrite(fertilizer_pump,HIGH);
  lcd.clear();
  lcd.print("Spraying fertilizer.");
  delay(300);

  lcd.clear();
  lcd.print("Spraying fertilizer..");
  delay(300);

  lcd.clear();
  lcd.print("Spraying fertilizer...");
  tempsecond = currentsecond;
  digitalWrite(fertilizer_pump,LOW);
  /*
    this is where the code for spraying it will be
    turn off spray
  */
}

if (moisturelevel < needed_moisture_percentage){
  /*
    this is where the code for spraying it will be
    turn on spray
  */
  digitalWrite(water_pump,HIGH);
  lcd.clear();
  lcd.print("Spraying water.");
  delay(300);

  lcd.clear();
  lcd.print("Spraying water..");  
  delay(300);

  lcd.clear();
  lcd.print("Spraying water...");
  digitalWrite(water_pump,LOW);
  /*
    this is where the code for spraying it will be
    turn off spray
  */
  delay(250);
  int moisturelevel = read_hygrometer();
}









if (tempC < needed_air_temp){
  /*
    Growlight
  */

  delay(200);
  digitalWrite(growlight,HIGH);
  float tempC = dht.readTemperature();
  
}
else if (tempC > needed_air_temp){
  /*
    turn off growlight
    turn on ultrasonic atomizer humidifier for 3 sec
  */
  delay(200);
  digitalWrite(growlight,LOW);
  float tempC = dht.readTemperature();
  
}



/*
    soil temp rebound *TODO: havent been make
*/
if (soiltempc < needed_soil_temp){

  float soiltempc = readDS18B20();

}
else if (soiltempc > needed_soil_temp){

  delay(200);
  digitalWrite(water_pump,HIGH);
  
  float soiltempc = readDS18B20();
  delay(400);
  digitalWrite(water_pump,LOW);
  
}





if (hum < needed_humidity_percentage){
  /*
  turn on ultrasonic atomizer humidifier
  */
  delay(200);
  digitalWrite(UAH,HIGH);
  float hum = dht.readHumidity();
  

}
else if (hum > needed_humidity_percentage){
  /*
  turn off ultrasonic atomizer humidifier
  turn on grow light
  */
  delay(200);
  digitalWrite(UAH,HIGH);
  float hum = dht.readHumidity();
}
}