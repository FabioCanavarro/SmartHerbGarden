#include <Arduino.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>
/* initiating the pins*/
// receiving pin
const int DHTPIN = D3;     
const int HYGROGENATOR_PIN = D6; 
const int DB18S20 = D4;

// output pins
const int UAH = D5;
const int water_pump = D8;
const int fertilizer_pump = D7;
const int growlight = D0;




// needed config for HYGROGENATOR
const int needed_air_temp= 26; //adjustable
const int needed_soil_temp = 29; //adjustable
const int needed_humidity_percentage = 60; //adjustable
const int needed_moisture_percentage = 70; //adjustable


int currentsecond;
int tempsecond = 1800;//some delay before spraying
const int duration_before_fertlizer_in_hours=1 ;

LiquidCrystal_I2C lcd(0x27, 16, 2);
/*
*SCL = D1
*SDA = D2
*VCC = 5V
*GND = GND
*/



DHT dht(DHTPIN, DHT22);
const int AirValue = 561;   //replace the value with value when placed in air using calibration code 
const int WaterValue = 310; //replace the value with value when placed in water using calibration code 
int soilMoistureValue = 0;
int soilmoisturepercent=0;

int read_hygrometer(){
  soilMoistureValue = analogRead(HYGROGENATOR_PIN);  //put Sensor insert into soil
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
  pinMode(growlight,HIGH);
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
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Humidity: "); 
lcd.print(hum);
lcd.print("%");
lcd.setCursor(0, 1);
lcd.print("Temp: "); 
lcd.print(tempC);
lcd.print(" *C ");
delay(3500);
int moisturelevel = read_hygrometer();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("moisture: ");
lcd.print(moisturelevel);

/*
    *TODO: soil temp
*/


if (((currentsecond - tempsecond) /3600) >= duration_before_fertlizer_in_hours)
{
  /*
    this is where the code for spraying it will be
    turn on spray
  */
  lcd.clear();
  lcd.print("Spraying fertilizer.");
  delay(1000);

  lcd.clear();
  lcd.print("Spraying fertilizer..");
  delay(1000);

  lcd.clear();
  lcd.print("Spraying fertilizer...");
  tempsecond = currentsecond;
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
  lcd.clear();
  lcd.print("Spraying water.");
  delay(1000);

  lcd.clear();
  lcd.print("Spraying water..");  
  delay(1000);

  lcd.clear();
  lcd.print("Spraying water...");
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

  delay(3000);
  float tempC = dht.readTemperature();
}
else if (tempC > needed_air_temp){
  /*
    turn off growlight
    turn on ultrasonic atomizer humidifier for 3 sec
  */
  delay(3000);
  float tempC = dht.readTemperature();
}
/*
    soil temp rebound *TODO: havent been make
*/



if (hum < needed_humidity_percentage){
  /*
  turn on ultrasonic atomizer humidifier
  */
  delay(3000);
  float hum = dht.readHumidity();
}
else if (hum > needed_humidity_percentage){
  /*
  turn off ultrasonic atomizer humidifier
  turn on grow light
  */
  delay(3000);
  float hum = dht.readHumidity();
}
}