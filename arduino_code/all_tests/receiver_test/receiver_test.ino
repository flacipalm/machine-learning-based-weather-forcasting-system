#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

RF24 radio(9,8); // CE, CSN
const byte address[10] = "ADDRESS01";
int commapos;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup(){
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    String tempStr;
    String humiStr;
    String pressureStr;
    String if_rainStr;    
    String levelStr;
    String UVStr;
    String PM1Str;
    String PM2_5Str;
    String PM10Str;

    // initialize the OLED object
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
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
          tempStr = parse_str.substring(0,commapos);

          float temp = parse_str.substring(0,commapos).toFloat();
          Serial.print("Temperature = ");
          Serial.println(temp);
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        if(i == 1){
          humiStr = parse_str.substring(0,commapos);

          float humi = parse_str.substring(0,commapos).toFloat();
          Serial.print("Humidity = ");
          Serial.println(humi);
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        if(i == 2){
          pressureStr = parse_str.substring(0,commapos);
          float pressure = parse_str.substring(0,commapos).toFloat();
          Serial.print("Pressure: ");
          Serial.print(pressure, 0); //whole number only.
          Serial.println(" Pa");
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        if(i == 3){
          if_rainStr = parse_str.substring(0,commapos);
          int if_rain = parse_str.substring(0,commapos).toFloat();
          Serial.print("If rain: ");
          Serial.println(if_rain);
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
      }
      else{
        if(parse_str.length() > 0){
          levelStr = parse_str;
          float Level = parse_str.toFloat();
          Serial.print("Wind level: ");
          Serial.println(Level);         
        }
      }
    }

    display.clearDisplay(); // clear previous display
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Temperature = " + tempStr);
    display.println("Humidity = " + humiStr)
    display.println("If rain: "+ if_rainStr);
    display.println("Wind level: "+ levelStr);
    display.display()

    while (!radio.available()){}
    char txt2[64] = "";
    radio.read(&txt2, sizeof(txt2));
    String parse_str2 = String(txt2);
    Serial.println(txt2);
    for (int j=5; j<9; j++){
      commapos = parse_str2.indexOf(',');
      if(commapos != -1){
        if(j == 5){
          UVStr = parse_str2.substring(0,commapos);

          int UV = parse_str2.substring(0,commapos).toFloat();
          Serial.print("UV index: ");
          Serial.println(UV);
          parse_str2 = parse_str2.substring(commapos + 1, parse_str2.length());
        }
        if(j == 6){
          PM1Str = parse_str2.substring(0,commapos);

          int PM1 = parse_str2.substring(0,commapos).toFloat();
          Serial.print("PM1.0 = ");
          Serial.print(PM1);
          Serial.println("ug/m3");
          parse_str2 = parse_str2.substring(commapos + 1, parse_str2.length());
        }
        if(j == 7){
          PM2_5Str = parse_str2.substring(0,commapos);

          int PM2_5 = parse_str2.substring(0,commapos).toFloat();
          Serial.print("PM2.5 = ");
          Serial.print(PM2_5);
          Serial.println("ug/m3");
          parse_str2 = parse_str2.substring(commapos + 1, parse_str2.length());
        }
      }
      else{
        if(parse_str2.length() > 0){
          PM10Str = parse_str2.substring(0,commapos);
          
          int PM10 = parse_str2.toInt();
          Serial.print("PM10 = ");
          Serial.print(PM10);
          Serial.println("ug/m3");
          Serial.println();             
        }
      }
    }
    delay(1000); // wait previous content to display for one more seconds.

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("UV index: " + UVStr);
    display.println("PM1.0 = " + PM1Str)
    display.println("PM2.5 =  "+ PM2_5Str);
    display.println("PM10 = : "+ PM10Str);
    display.display()
}