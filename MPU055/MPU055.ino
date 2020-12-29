#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "FIT2"; // Enter your WiFi name
const char* password = "kkkc2016"; // Enter WiFi password

const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* TOPIC = "linhnv/gyro/sensor/01";

const long utcOffsetInSeconds = 0;

#define LED 2 //Define blinking LED pin

WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);

char* char_jsonMessage = (char*) malloc(100);

void setup(void) 
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT); // Initialize the LED pin as an output
  WiFi.begin(ssid, password);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  int i = 0;
  // Wait for the Wi-Fi to connect
  
  while (WiFi.status() != WL_CONNECTED)
  { 
    delay(900);
    digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
    delay(100);
    digitalWrite(LED, HIGH); // Turn the LED off by making the voltage HIGH
    Serial.print(++i);
    Serial.print(' ');
  }

  Serial.println();
  Serial.println("Connected to the WiFi network");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");

    if (client.connect("129skdfhnsdfh4n"))
    {
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(500);
      digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
      delay(1500);
      digitalWrite(LED, HIGH); // Turn the LED off by making the voltage HIGH
    }
  }

  delay(1000);
  timeClient.begin();
    
  bno.setExtCrystalUse(true);
}

void loop(void) 
{
  timeClient.update();
  if (!client.connected()) {
    reconnect();
  }
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Display the floating point data */
//  Serial.print("X: ");
//  Serial.print(event.orientation.x, 4);
//  Serial.print("\tY: ");
//  Serial.print(event.orientation.y, 4);
//  Serial.print("\tZ: ");
//  Serial.print(event.orientation.z, 4);
//  Serial.println("");

  String jsonMessage = timeClient.getEpochTime() + String(",") + (event.orientation.x) + String(",") + (event.orientation.y) + String(",") + (event.orientation.z);
  Serial.print("JSON message: ");
  Serial.println(timeClient.getEpochTime());
  Serial.println(jsonMessage);

  jsonMessage.toCharArray(char_jsonMessage, 100);
  client.publish(TOPIC, char_jsonMessage);
  digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
  
  delay(100);
}


void reconnect()
{
  while (!client.connected()) 
    {
      Serial.println("Connecting to MQTT...");

      if (client.connect("17020845"))
      {
        Serial.println("connected");  
      } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(500);
        digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
        delay(1500);
        digitalWrite(LED, HIGH); // Turn the LED off by making the voltage HIGH
      }
    }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
  delay(10);
  digitalWrite(LED, HIGH); // Turn the LED off by making the voltage HIGH
  delay(10);
  digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
  delay(10);
  digitalWrite(LED, HIGH); // Turn the LED off by making the voltage HIGH
  
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");
}
