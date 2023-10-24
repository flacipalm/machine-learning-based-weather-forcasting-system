#include <SPI.h>
#include <RF24.h>
#include <nRF24L01>

RF24 radio(9,8); // CE, CSN
const byte address[10] = "ADDRESS01";

void setup(){
    serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}

void loop(){
    if (radio.available()){
        char txt[32] = "";
        radio.read(&txt, sizeof(txt));
        serial.println(txt);
    }
}