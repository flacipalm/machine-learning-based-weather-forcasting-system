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
#define OLED_RESET     4 // -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display(OLED_RESET);

String tempStr;
String humiStr;
String pressureStr;
String if_rainStr;    
String levelStr;
String UVStr;
String PM1Str;
String PM2_5Str;
String PM10Str;

void setup(){
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    // initialize the OLED object
    // if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //   Serial.println(F("SSD1306 allocation failed"));
    //   for(;;); // Don't proceed, loop forever
    // }
}

void loop(){
    while (!radio.available()){}
    char txt[64] = "";
    radio.read(&txt, sizeof(txt));
    String parse_str = String(txt);
    // Serial.println(txt);
    for (int i=0; i<5; i++){
      commapos = parse_str.indexOf(',');
      if(commapos != -1){
        if(i == 0){
          // tempStr = parse_str.substring(0,commapos);
          float temp = parse_str.substring(0,commapos).toFloat();
          tempStr = String(temp);
          // Serial.print("Temperature = ");
          // Serial.println(temp);
          Serial.print("Start");
          Serial.print("\t");
          Serial.print(temp);
          Serial.print("\t");
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        if(i == 1){
          // humiStr = parse_str.substring(0,commapos);
          float humi = parse_str.substring(0,commapos).toFloat();
          humiStr = String(humi);
          // Serial.print("Humidity = ");
          // Serial.println(humi);
          Serial.print(humi);
          Serial.print("\t");
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        if(i == 2){
          // pressureStr = parse_str.substring(0,commapos);
          float pressure = parse_str.substring(0,commapos).toFloat();
          pressureStr = String(pressure);          
          // Serial.print("Pressure: ");
          // Serial.print(pressure, 0); //whole number only.
          // Serial.println(" Pa");
          Serial.print(pressure, 0);
          Serial.print("\t");
          parse_str = parse_str.substring(commapos + 1, parse_str.length());
        }
        if(i == 3){
          // if_rainStr = parse_str.substring(0,commapos);
          int if_rain = parse_str.substring(0,commapos).toFloat();
          if_rainStr = String(if_rain);
          // Serial.print("If rain: ");
          // Serial.println(if_rain);
          Serial.print(if_rain);
          Serial.print("\t");
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
          // levelStr = parse_str;
          float Level = parse_str.toFloat();
          levelStr = String(Level);
          // Serial.print("Wind level: ");
          // Serial.println(Level);     
          Serial.print(Level);
          Serial.print("\t");
        }
      }
    }

    display.clearDisplay(); // clear previous display
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    // display.print("Temperature = ");
    // display.println(tempStr);
    // display.print("Humidity = ");
    // display.println(humiStr);

    display.print(tempStr);
    display.print(" degC  ");
    display.print(humiStr);
    display.println("%");

    // display.print("Pressure = ");
    display.print(pressureStr);
    display.println(" Pa ");
    
    if (if_rainStr.toInt() == 0){
      display.println("No Rain");
    }
    else{
      display.println("Rainy");
    }
    // display.print("Rain: ");
    // display.println(if_rainStr);
    display.print("Wind: ");
    display.print(levelStr);
    display.println(" m/s");
    display.display();


    while (!radio.available()){}
    char txt2[64] = "";
    radio.read(&txt2, sizeof(txt2));
    String parse_str2 = String(txt2);
    // Serial.println(txt2);
    for (int j=5; j<9; j++){
      commapos = parse_str2.indexOf(',');
      if(commapos != -1){
        if(j == 5){
          // UVStr = parse_str2.substring(0,commapos);
          
          int UV = parse_str2.substring(0,commapos).toFloat();
          UVStr = String(UV);
          // Serial.print("UV index: ");
          // Serial.println(UV);
          Serial.print(UV);
          Serial.print("\t");
          parse_str2 = parse_str2.substring(commapos + 1, parse_str2.length());
        }
        if(j == 6){
          // PM1Str = parse_str2.substring(0,commapos);
          int PM1 = parse_str2.substring(0,commapos).toFloat();
          PM1Str = String(PM1);
          // Serial.print("PM1.0 = ");
          // Serial.print(PM1);
          // Serial.println("ug/m3");
          Serial.print(PM1);
          Serial.print("\t");
          parse_str2 = parse_str2.substring(commapos + 1, parse_str2.length());
        }
        if(j == 7){
          // PM2_5Str = parse_str2.substring(0,commapos);
          int PM2_5 = parse_str2.substring(0,commapos).toFloat();
          PM2_5Str = String(PM2_5);
          // Serial.print("PM2.5 = ");
          // Serial.print(PM2_5);
          // Serial.println("ug/m3");
          Serial.print(PM2_5);
          Serial.print("\t");
          parse_str2 = parse_str2.substring(commapos + 1, parse_str2.length());
        }
      }
      else{
        if(parse_str2.length() > 0){
          // PM10Str = parse_str2.substring(0,commapos);
          int PM10 = parse_str2.toInt();
          PM10Str = String(PM10);
          // Serial.print("PM10 = ");
          // Serial.print(PM10);
          // Serial.println("ug/m3");
          // Serial.println();
          Serial.println(PM10);      
        }
      }
    }

    delay(1000); // wait previous content to display for one more seconds.

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("UV index: ");
    display.println(UVStr);
    display.print("PM1.0 = ");
    display.print(PM1Str);
    display.println("ug/m^3");
    display.print("PM2.5 = ");
    display.print(PM2_5Str);
    display.println("ug/m^3");
    display.print("PM10 = ");
    display.print(PM10Str);
    display.println("ug/m^3");
    display.display();
      
}