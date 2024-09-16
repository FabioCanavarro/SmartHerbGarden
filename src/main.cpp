#include <Arduino.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#define DHTPIN D3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

const int AirValue = 561;   //replace the value with value when placed in air using calibration code 
const int WaterValue = 310; //replace the value with value when placed in water using calibration code 
int soilMoistureValue = 0;
int soilmoisturepercent=0;

int read_hygrometer(){
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
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
  // Wait a few seconds between measurements.
delay(3500);
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
}
