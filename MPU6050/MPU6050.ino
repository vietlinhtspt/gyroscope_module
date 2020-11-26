/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Pitch & Roll & Yaw Gyroscope Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/
#include <SoftwareSerial.h>
SoftwareSerial s(5,6);

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Timers
unsigned long timer = 0;
float timeStep = 0.01;
float timeStep_send = 0.5;
float current_milis = millis();

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;

void setup() 
{
  s.begin(115200);
  Serial.begin(115200);

  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3);
}

void loop()
{
  timer = millis();

  // Read normalized values
  Vector norm = mpu.readNormalizeGyro();

  // Calculate Pitch, Roll and Yaw
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;

  // Output raw
//  Serial.print(" Pitch = ");
  Serial.print(pitch);
//  Serial.print(" Roll = ");
  Serial.print(":");
  Serial.print(roll);  
//  Serial.print(" Yaw = ");
  Serial.print(":");
  Serial.println(yaw);

  char message[30];
  sprintf(message, "\"y\":%d, \"p\":%d, \"r\":%d", (int)yaw, (int)pitch, (int)roll);

//  Serial.println(message);
//  Serial.print("s.available: ");
//  Serial.println(s.available());
  if(s.available()>0)
  {
    
    if (millis() - current_milis > timeStep_send * 1000)
    {
      
      current_milis = millis();
//      Serial.println("Printed to s(5,6)");
      s.write(message);
//      Serial.print("Printed to s(5,6)");
//      Serial.println(message);
    }
  }

  // Wait to full timeStep period
  if (((timeStep*1000) - (millis() - timer)) > 50)
  {
    delay((timeStep*1000) - (millis() - timer));
  }
  
}
