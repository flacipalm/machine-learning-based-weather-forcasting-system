// Set up for DHT11
#include "DHT.h"
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define DHTPIN 7
#define DHTTYPE DHT11

RF24 radio(9,8); // CE, CSN
const byte address[10] = "ADDRESS01";
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  // TEMP & HUM
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  //char comma[] = ",";
  String tran_str = "";

  tran_str = String(temp) + "," + String(humi);
  char tran_char[tran_str.length() + 1];
  tran_str.toCharArray(tran_char, tran_str.length() + 1);
  radio.write(&tran_char, sizeof(tran_char));

  // char tempBuffer[32];
  // char humiBuffer[32];
  // sprintf(tempBuffer, "%f", temp);
  // sprintf(humiBuffer, "%f", humi);
  // char tempStr[16];

  // snprintf(tempStr, sizeof(tempStr), "%.2f", temp);
  // Serial.print("Temperature = ");
  // Serial.print(tempStr);
  // Serial.print("\n");
  // // Serial.println(temp);
  // Serial.print("Temperature = ");
  // Serial.println(temp);
  // Serial.print("Humidity = ");
  // Serial.println(humi);

  //const char txt0[] = "Temperature = ";
  //radio.write(&txt0, sizeof(txt0));

  // radio.write(&tempStr, sizeof(tempStr));
  // radio.write(&comma, sizeof(comma));
  
  //const char txt1[] = "Humidity = ";
  //radio.write(&txt1, sizeof(txt1));
  //radio.write(&humiBuffer, sizeof(humiBuffer));

  delay(4000); //延时设置需在一秒以上 delay() 单位为ms
}


// void loop(){
//     const char txt[] = "Hello World";
//     radio.write(&txt, sizeof(txt));
//     delay(1000);
// }