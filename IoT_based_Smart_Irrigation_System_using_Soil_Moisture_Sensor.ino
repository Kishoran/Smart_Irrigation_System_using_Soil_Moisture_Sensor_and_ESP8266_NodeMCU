#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

const char*ssid = "Kishoran@02"; // Your network id
const char*password = "12345678";//your network password 
WiFiClient client;

unsigned long myChannelNumber = 2598506; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "J7XOUF19XCA2BZF3"; //Your Write API Key
const char * myReadAPIKey = "NYGHBXX0W0W2FJHS"; //Your Write API Key


#define DHTPIN D3          
DHT dht(DHTPIN, DHT11);
const int moisturePin = A0;
const int motorPin = D1;
const int led = D2;
unsigned long interval = 10000;
unsigned long previousMillis = 0;
unsigned long interval1 = 1000;

unsigned long previousMillis1 = 0;

float moisturePercentage;              //moisture reading

float h;                  // humidity reading

float t;   

void setup() {
pinMode(motorPin, OUTPUT);
pinMode(led , OUTPUT);
pinMode(led,LOW);
digitalWrite(motorPin, LOW); // keep motor off initally
dht.begin();
WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);

}

void loop() {
  unsigned long currentMillis = millis();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  moisturePercentage = ( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) );
  Serial.print("Soil Moisture is  = ");
  Serial.print(moisturePercentage);
  Serial.println("%");
  if (moisturePercentage < 50) {
    digitalWrite(motorPin, HIGH);
    digitalWrite(led,HIGH);


  }
  if (moisturePercentage > 50 && moisturePercentage < 55) {
    digitalWrite(motorPin, HIGH);
    digitalWrite(led,HIGH);
  }
  if (moisturePercentage > 56) {
    digitalWrite(motorPin, LOW);

  }
  // if ((unsigned long)(currentMillis - previousMillis) >= interval) {
  //   sendThingspeak();
  //   previousMillis = millis();
  //   client.stop();
  // }
  ThingSpeak.writeField(myChannelNumber, 1,moisturePercentage, myWriteAPIKey); //Update in ThingSpeak
  delay(2000);
  ThingSpeak.writeField(myChannelNumber, 2,t, myWriteAPIKey); //Update in ThingSpeak
  delay(2000);
  ThingSpeak.writeField(myChannelNumber, 3,h, myWriteAPIKey); //Update in ThingSpeak
  delay(2000);

}
