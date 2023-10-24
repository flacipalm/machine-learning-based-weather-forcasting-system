// Set up for DHT11
#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // TEMP & HUM
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  Serial.print("Temperature = ");
  Serial.println(temp);
  Serial.print("Humidity = ");
  Serial.println(humi);
  delay(2000); //延时设置需在一秒以上 delay() 单位为ms

}
