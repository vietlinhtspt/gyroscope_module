#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "FIT2"; // Enter your WiFi name
const char* password = "kkkc2016"; // Enter WiFi password
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;

const char* TOPIC = "linhnv/gyro/sensor/01";

#define LED 2 //Define blinking LED pin

WiFiClient espClient;
PubSubClient client(espClient);

// Define NTP Client to get time
const long utcOffsetInSeconds = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int data;

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

void setup() 
{
    pinMode(LED, OUTPUT); // Initialize the LED pin as an output
    s.begin(115200);
    Serial.begin(115200);
    Serial.print("Connecting to WiFi.. ");
    WiFi.begin(ssid, password);
    
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
 
//    client.publish(TOPIC, "Hello from ESP8266"); //Topic name
//    client.subscribe(TOPIC);
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

void loop() 
{
//  s.write("s");

  if (!client.connected()) {
      reconnect();
  }
  
  if (s.available()>0)
  {
      String content = "";
      char character;
      while(s.available()) 
      {
  //        Serial.println("Get character");
          character = s.read();
          content.concat(character);
      }
      if (content != "") {
          Serial.println(content);
          timeClient.update();
          String jsonMessage = timeClient.getEpochTime() + String(",") + String(content);
          Serial.print("JSON message: ");
          char* char_jsonMessage = (char*) malloc(100);
          Serial.println(jsonMessage);
          jsonMessage.toCharArray(char_jsonMessage, 100);
          client.publish(TOPIC, char_jsonMessage);
          digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
    }
  }
}
