/*
    L3G4200D Triple Axis Gyroscope: Output for L3G4200D_processing_pry.pde
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-l3g4200d.html
    GIT: https://github.com/jarzebski/Arduino-L3G4200D
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(5,6);

#include <Wire.h>
#include <L3G4200D.h>

L3G4200D gyroscope;

// Timers
unsigned long timer = 0;
float timeStep = 0.01;
float timeStep_send = 0.5;
float current_milis = millis();

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;

int LED = 13;
boolean Blink = false;

void setup() 
{
  s.begin(115200);
  Serial.begin(115200);

  // Initialize L3G4200D
  // Set scale 2000 dps and 400HZ Output data rate (cut-off 50)
  while (!gyroscope.begin(L3G4200D_SCALE_250DPS, L3G4200D_DATARATE_400HZ_50))
  {
    // Waiting for initialization

    if (Blink)
    {
      digitalWrite(LED, HIGH);
    } else
    {
      digitalWrite(LED, LOW);
    }

    Blink = !Blink;

    delay(500);
  }

  digitalWrite(LED, HIGH);

  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  Serial.println("Starting ");
  gyroscope.calibrate(100);

  digitalWrite(LED, LOW);
}



void loop()
{
  timer = millis();

  // Read normalized values
  Vector norm = gyroscope.readNormalize();

  // Calculate Pitch, Roll and Yaw
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;

  // Output raw
//  Serial.print(norm.XAxis);
//  Serial.print(":");
//  Serial.print(norm.YAxis);
//  Serial.print(":");
//  Serial.print(norm.ZAxis);
//  Serial.print(":");
  Serial.print(pitch);
  Serial.print(":");
  Serial.print(roll);
  Serial.print(":");
  Serial.println(yaw);
  
  // Output indicator
  if (Blink)
  {
    digitalWrite(LED, HIGH);
  } else
  {
    digitalWrite(LED, LOW);
  }

  Blink = !Blink;

  if (isnan(yaw) || isnan(pitch) || isnan(roll)) {
    Serial.println("Nan. Return.");
    return;
  }
 
  
  char message[40];
  sprintf(message, "\"y\":%d, \"p\":%d, \"r\":%d", (int)yaw, (int)pitch, (int)roll);

  Serial.println(message);
  Serial.print("s.available: ");
  Serial.println(s.available());
  if(s.available()>0)
  {
    
    if (millis() - current_milis > timeStep_send * 1000)
    {
      
      current_milis = millis();
      Serial.println("Printed to s(5,6)");
      s.write(message);
//      Serial.print("Printed to s(5,6)");
//      Serial.println(message);
    }
  }

  // Wait to full timeStep period
  if ((timeStep*1000) - (millis() - timer) > 0) 
  {
//    Serial.println((timeStep*1000) - (millis() - timer));
    delay((timeStep*1000) - (millis() - timer)); 
  }
}

//}
