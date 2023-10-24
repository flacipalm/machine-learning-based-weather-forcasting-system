#include <Wire.h>

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

// Set up for DHT11
#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT11
#define DebugSerial Serial
#define PM2_5Serial Serial //用于接收PM2.5数据
DHT dht(DHTPIN, DHTTYPE);

const unsigned char OSS = 0;  // Oversampling Setting
byte buffer[32] = {};
int count = 0;
bool bufferComplete = false;

// Calibration values
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;

// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).
long b5; 


void setup() {
  // put your setup code here, to run once:
  DebugSerial.begin(9600);
  Wire.begin();
  dht.begin();
  PM2_5Serial.begin(9600);

  bmp085Calibration();
  pinMode(13,OUTPUT);//设置数字13引脚为辒出模式
  DebugSerial.println("setup end! waiting receive sensor datas");

}

void loop() {
  // put your main code here, to run repeatedly:
  float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float atm = pressure / 101325; // "standard atmosphere"
  float altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  float temp_avg = (temperature + temp)/2;

  float water_level =analogRead(A0);//读叏模拟0口电压值 
  // if(water_level>512)//如果大于512（2.5V） 
  // { 
  //   digitalWrite(13,HIGH);//点亮led灯 
  //   delay(50);
  //   digitalWrite(13,LOW);//熄灭led灯 
  //   delay(50);
  // } 
  // else//否则 
  // { 
  //   digitalWrite(13,LOW);//熄灭led灯 
  // } 

  int sensorValue = analogRead(A2);
  float outvoltage = sensorValue * (5.0 / 1023.0);
  float Level = 6*outvoltage;

  while(bufferComplete == false)	//等待数据接收一帧完整
	{
		getPM();
  }

  int PM1 = buffer[10]*256+buffer[11];
	int PM2_5 = buffer[12]*256+buffer[13];
	int PM10 = buffer[14]*256+buffer[15];


  int uvValue;
  int uv;
  long  uv_sum=0;
  for(int i=0;i<1024;i++)
    {  
      uvValue=analogRead(A1);
      uv_sum=uvValue+uv_sum;
      delay(2);
    }   
  uv_sum = uv_sum >> 10;
  float vout = uv_sum * 4980.0 / 1024;


  // DebugSerial.print("Temperature from BMP: ");
  // DebugSerial.print(temperature, 2); //display 2 decimal places
  // DebugSerial.println("deg C");

  // DebugSerial.print("Temperature: ");
  // DebugSerial.println(temp);

  DebugSerial.print("Temperature: ");
  DebugSerial.println(temp_avg, 2);
  DebugSerial.println("deg C");

  DebugSerial.print("Humidity: ");
  DebugSerial.println(humi, 2);

  DebugSerial.print("Pressure: ");
  DebugSerial.print(pressure, 0); //whole number only.
  DebugSerial.println(" Pa");

  // DebugSerial.print("Standard Atmosphere: ");
  // DebugSerial.println(atm, 4); //display 4 decimal places

  // DebugSerial.print("Altitude: ");
  // DebugSerial.print(altitude, 2); //display 2 decimal places
  // DebugSerial.println(" M");

  DebugSerial.print("Water level = ");
  DebugSerial.println(water_level);

  DebugSerial.print("outvoltage = ");
  DebugSerial.print(outvoltage);
  DebugSerial.println("V");
  DebugSerial.print("wind speed is ");
  DebugSerial.print(Level);
  DebugSerial.println(" level now");
  //DebugSerial.println();


  DebugSerial.print("The Photocurrent value:");
  DebugSerial.print(vout);
  DebugSerial.println("mV");
  if (vout < 50) {
    uv = 0;
  }
  else if (vout < 227) {
    uv = 1;
  }
  else if (vout < 318) {
    uv = 2;
  }
  else if (vout < 408) {
    uv = 3;
  }
  else if (vout < 503) {
    uv = 4;
  }
  else if (vout < 606) {
    uv = 5;
  }
  else if (vout < 696) {
    uv = 6;
  }
  else if (vout < 795) {
    uv = 7;
  }
  else if (vout < 881) {
    uv = 8;
  }
  else if (vout < 976) {
    uv = 9;
  }
  else if (vout < 1079) {
    uv = 10;
  }
  else {
    uv = 11;
  }
  DebugSerial.print("UV Index = ");
  DebugSerial.println(uv);

  DebugSerial.print("PM1.0 = ");
	DebugSerial.print(PM1);
	DebugSerial.println("ug/m3");

	DebugSerial.print("PM2.5 = ");
	DebugSerial.print(PM2_5);
	DebugSerial.println("ug/m3");

	DebugSerial.print("PM10 = ");
	DebugSerial.print(PM10);
	DebugSerial.println("ug/m3");
	
	bufferComplete = false;

  DebugSerial.println();//line break

  //delay(2000); //wait 2 second and get values again.
  //delay(20);
  delay(2000);

}


// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calculate temperature in deg C
float bmp085GetTemperature(unsigned int ut){
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  float temp = ((b5 + 8)>>4);
  temp = temp /10;

  return temp;
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up){
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  long temp = p;
  return temp;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;

  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();

  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();

  // Wait at least 4.5ms
  delay(5);

  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP(){

  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();

  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  msb = bmp085Read(0xF6);
  lsb = bmp085Read(0xF7);
  xlsb = bmp085Read(0xF8);

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}

void writeRegister(int deviceAddress, byte address, byte val) {
  Wire.beginTransmission(deviceAddress); // start transmission to device 
  Wire.write(address);       // send register address
  Wire.write(val);         // send value to write
  Wire.endTransmission();     // end transmission
}

int readRegister(int deviceAddress, byte address){

  int v;
  Wire.beginTransmission(deviceAddress);
  Wire.write(address); // register to read
  Wire.endTransmission();

  Wire.requestFrom(deviceAddress, 1); // read a byte

  while(!Wire.available()) {
    // waiting
  }

  v = Wire.read();
  return v;
}

float calcAltitude(float pressure){

  float A = pressure/101325;
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 1 - C;
  C = C /0.0000225577;

  return C;
}

void getPM()
{
	int ppm;
	int count_sum = 0;
	long sum = 0;

	int flag_end = false;
	int avg_num = 10;
	byte receive_pre = 0x00;


	int flag_start = false;
	// DebugSerial.println("--------------------------");
	flag_end = false;
	while (flag_end == false)
	{
		// DebugSerial.println("+++++++++++++++++++++++++");
		// Serial.flush();
		if (PM2_5Serial.available() > 0) {

			char inChar = (char)PM2_5Serial.read();
			buffer[count] = (byte)inChar;
			if (buffer[count] == 0x4d &&  receive_pre == 0x42)
			{
				buffer[0] = 0x42;
				count = 1;	
				flag_start = true;
				//DebugSerial.println("\r\n##################################################");
				//DebugSerial.println("start");
			}
			receive_pre = (byte)inChar;
			// DebugSerial.print("##count=");
			// DebugSerial.println(count);

			// DebugSerial.print("buffer[count]=");
			// DebugSerial.print(buffer[count],HEX);
			// DebugSerial.print(",");

			count++;

			if (count >= 32)
			{
				count = 0;
				if (flag_start)
				{
					int checksum = 0;
					for(int i = 0 ; i < 30 ; i++)
					{
						checksum += buffer[i];
					}
					if (buffer[0] == 0x42 && buffer[1] == 0x4d && checksum == buffer[30]*256+buffer[31])
					{
						bufferComplete = true;
						// DebugSerial.println("bufferComplete = true;");
						
						flag_end = true;    //结束了，可以下一帧数据

            if (bufferComplete == true) {
              DebugSerial.println("Checksum correct");
            }

						//把接收到的数据转发出来
						// if (bufferComplete == true) {
						// 	for (int i = 0 ; i < 32 ; i++)
						// 	{
						// 		DebugSerial.print(buffer[i], HEX);
						// 		DebugSerial.print(",");
						// 	}
						// 	DebugSerial.println("");							
						// }
					}
					else
					{
						// DebugSerial.println("\r\n****error data*****");
						// for (int i = 0 ; i < 7 ; i++)
						// {
						// 	DebugSerial.print(buffer[i], HEX);
						// 	DebugSerial.print(",");
						// }
						// DebugSerial.println("\r\n****error*****");
            // DebugSerial.println("");
					}
				}
			}
		}

	}
}

