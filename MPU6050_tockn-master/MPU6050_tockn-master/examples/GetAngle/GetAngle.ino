#include <SoftwareSerial.h>
SoftwareSerial s(5,6);

#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

float timeStep_send = 0.1;
float current_milis = millis();

float pitch = 0;
float roll = 0;
float yaw = 0;

void setup() {
  Serial.begin(115200);
  s.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ());

  pitch = mpu6050.getAngleX();
  roll = mpu6050.getAngleY();
  yaw = mpu6050.getAngleZ();
  

  char message[15];
//  Serial.println(message);
  sprintf(message, "%d,%d,%d", (int)yaw, (int)pitch, (int)roll);

//  Serial.println(message);
//  Serial.print("s.available: ");
//  Serial.println(s.available());
  if(s.available()>0)
  {
    Serial.println(s.availableForWrite());
    if (millis() - current_milis > timeStep_send * 1000)
    {   
      current_milis = millis();
//      Serial.println("Printed to s(5,6)");
      s.write(message, 15);
      Serial.print("Printed to s(5,6) ");
      Serial.print(sizeof(message));
      Serial.print(" ");
      Serial.println(message);
    }
  }
}
