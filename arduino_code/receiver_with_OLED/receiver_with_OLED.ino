#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9,8); // CE, CSN
const byte address[10] = "ADDRESS01";
int commapos;

void setup(){
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}

void loop(){
    while (!radio.available()){}
    char txt[64] = "";
    radio.read(&txt, sizeof(txt));
    String parse_str = String(txt);
    Serial.println(txt);
    for (int i=0; i<5; i++){
      commapos = parse_str.indexOf(',');
      if(commapos != -1){
        if(i == 0){
          float temp = parse_str.substring(0,commapos).toFloat();
          Serial.print("Temperature = ");
          Serial.println(temp);
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        if(i == 1){
          float humi = parse_str.substring(0,commapos).toFloat();
          Serial.print("Humidity = ");
          Serial.println(humi);
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        if(i == 2){
          float pressure = parse_str.substring(0,commapos).toFloat();
          Serial.print("Pressure: ");
          Serial.print(pressure, 0); //whole number only.
          Serial.println(" Pa");
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        if(i == 3){
          int if_rain = parse_str.substring(0,commapos).toFloat();
          Serial.print("If rain: ");
          Serial.println(if_rain);
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        // if(i == 4){
        //   float Level = parse_str.substring(0,commapos).toFloat();
        //   Serial.print("Wind level: ");
        //   Serial.println(Level);
        //   parse_str = parse_str.substring(commapos + 1, parse_str.length());
        // }
        // if(i == 5){
        //   int UV = parse_str.substring(0,commapos).toFloat();
        //   Serial.print("UV index: ");
        //   Serial.println(UV);
        //   parse_str = parse_str.substring(commapos + 1, parse_str.length());
        // }
        // if(i == 6){
        //   int PM1 = parse_str.substring(0,commapos).toFloat();
        //   Serial.print("PM1.0 = ");
        //   Serial.print(PM1);
        //   Serial.println("ug/m3");
        //   parse_str = parse_str.substring(commapos + 1, parse_str.length());
        // }
        // if(i == 7){
        //   int PM2_5 = parse_str.substring(0,commapos).toFloat();
        //   Serial.print("PM2.5 = ");
        //   Serial.print(PM2_5);
        //   Serial.println("ug/m3");
        //   parse_str = parse_str.substring(commapos + 1, parse_str.length());
        // }
      }
      else{
        if(parse_str.length() > 0){
          float Level = parse_str.toFloat();
          Serial.print("Wind level: ");
          Serial.println(Level);         
        }
      }
    }

    while (!radio.available()){}
    char txt2[64] = "";
    radio.read(&txt2, sizeof(txt2));
    String parse_str2 = String(txt2);
    Serial.println(txt2);
    for (int j=5; j<9; j++){
      commapos = parse_str2.indexOf(',');
      if(commapos != -1){
        if(j == 5){
          int UV = parse_str2.substring(0,commapos).toFloat();
          Serial.print("UV index: ");
          Serial.println(UV);
          parse_str2 = parse_str2.substring(commapos + 1, parse_str2.length());
        }
        if(j == 6){
          int PM1 = parse_str2.substring(0,commapos).toFloat();
          Serial.print("PM1.0 = ");
          Serial.print(PM1);
          Serial.println("ug/m3");
          parse_str2 = parse_str2.substring(commapos + 1, parse_str2.length());
        }
        if(j == 7){
          int PM2_5 = parse_str2.substring(0,commapos).toFloat();
          Serial.print("PM2.5 = ");
          Serial.print(PM2_5);
          Serial.println("ug/m3");
          parse_str2 = parse_str2.substring(commapos + 1, parse_str2.length());
        }
      }
      else{
        if(parse_str2.length() > 0){
          int PM10 = parse_str2.toInt();
          Serial.print("PM10 = ");
          Serial.print(PM10);
          Serial.println("ug/m3");
          Serial.println();             
        }
      }
    }
      
}