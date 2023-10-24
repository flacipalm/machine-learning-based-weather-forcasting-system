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
    if (radio.available()){
        char txt[64] = "";
        radio.read(&txt, sizeof(txt));
        String parse_str = String(txt);
        Serial.println(txt);
        for (int i=0; i<2; i++){
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
            }
          }
          else{
            if(parse_str.length() > 0){
              float humi = parse_str.toFloat();
              Serial.print("Humidity = ");
              Serial.println(humi);              
            }
          }
        }
      }
}