#include <SPI.h>
#include <RF24.h>
#include <nRF24L01>

RF24 radio(9,8); // CE, CSN
const byte address[10] = "ADDRESS01";

void setup(){
    serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}

void loop(){
    const char txt[] = "Hello World";
    radio.write(&txt, sizeof(txt));
    delay(1000);
}