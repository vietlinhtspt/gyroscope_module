#include <Wire.h>
#include <ESP8266WiFi.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;

//boolean fall = false; //stores if a fall has occurred
//boolean trigger1=false; //stores if first trigger (lower threshold) has occurred
//boolean trigger2=false; //stores if second trigger (upper threshold) has occurred
//boolean trigger3=false; //stores if third trigger (orientation change) has occurred
//byte trigger1count=0; //stores the counts past since trigger 1 was set true
//byte trigger2count=0; //stores the counts past since trigger 2 was set true
//byte trigger3count=0; //stores the counts past since trigger 3 was set true
int angleChange=0;

// WiFi network info.
const char *ssid =  "FIT2";     // Enter your WiFi Name
const char *pass =  "kkkc2016"; // Enter your WiFi Password
void send_event(const char *event);
//const char *host = "maker.ifttt.com";
//const char *privateKey = "hUAAAz0AVvc6-NW1UmqWXXv6VQWmpiGFxx3sV5rnaM9";
void setup(){
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.println("Wrote to IMU");
//  Serial.println("Connecting to ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, pass);
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    delay(500);
//    Serial.print(".");              // print ... till not connected
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");
}

void loop(){
  mpu_read();
  ax = (AcX-2050)/16384.00;
  ay = (AcY-77)/16384.00;
  az = (AcZ-1947)/16384.00;
  gx = (GyX+270)/131.07;
  gy = (GyY-351)/131.07;
  gz = (GyZ+136)/131.07;
  // calculating Amplitute vactor for 3 axis
  Serial.print(AcX);
  Serial.print(":");
  Serial.print(AcY);
  Serial.print(":");
  Serial.print(AcZ);
  Serial.print(":");
  Serial.print(GyZ);
  Serial.print(":");
  Serial.print(GyZ);
  Serial.print(":");
  Serial.print(GyZ);
  Serial.println();
  
  
  delay(100);
}

void mpu_read(){
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
 Wire.endTransmission(false);
 Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
 AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
 AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
 AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
 Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
 GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
 GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
 GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
 }
 
