#define _DISABLE_TLS_
#include <ThingerWiFiNINA.h>

#define USERNAME "adityapatil688"
#define DEVICE_ID "mkr1010"
#define DEVICE_CREDENTIAL "mkr1010"

#define SSID "PC"
#define SSID_PASSWORD "7558425414"

#include <Servo.h>
Servo myservo1;
Servo myservo2;

#include "DHT.h"
#define DHTPIN 5 // Temperature and Humidity
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

ThingerWiFiNINA thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

#include <MQUnifiedsensor.h>
#define placa "Arduino MKR"
#define Voltage_Resolution 5
#define pin A2 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  
//#define calibration_button 13 //Pin to calibrate your sensor

MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

float soil_moisture; 
int Servo_;

void setup()
{ //27
  Serial.begin(9600);
  thing.add_wifi(SSID, SSID_PASSWORD);
  
  pinMode(A0, INPUT); // Light
  pinMode(A1, INPUT); // Soil Moisture
  pinMode(A2, INPUT); // CO2
  pinMode(10, OUTPUT);// Water Pump
  pinMode(2, OUTPUT); // Servo Motor
  
  myservo1.attach(1);
  myservo2.attach(2);
  
  dht.begin();
  
  MQ135.setRegressionMethod(1);
  MQ135.init();
 
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++) //40
  {
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
  }
  MQ135.setR0(calcR0/10);
}

void loop()
{ //44
  thing.handle();
  
  thing["Water pump"] << digitalPin(10);
  thing["Temperature"] >> outputValue(dht.readTemperature());
  thing["Humidity"] >> outputValue(dht.readHumidity());
  
  soil_moisture = 100 - ( (analogRead(A1)/1023.00) * 100 );
  thing["Soil Moisture"] >> outputValue(soil_moisture);
  
  thing["Light"] >> outputValue(analogRead(A0));
  
  thing ["Servo_"] << [](pson &in){
     if(in.is_empty())
      in = (int)Servo_;
    else
    {
      Servo_ = (int)in;
      myservo1.write((int)in);
      myservo2.write((int)in);
      delay(500);
    }
    Servo_ = in;
 }; //60
 
  MQ135.update();
  MQ135.setA(110.47); MQ135.setB(-2.862); 
  float CO2 = MQ135.readSensor();
  Serial.print("|   ");
  Serial.print(CO2); 
  Serial.println("   |");
   
  thing["CO2"] >> outputValue(MQ135.readSensor() + 400); //67
}
