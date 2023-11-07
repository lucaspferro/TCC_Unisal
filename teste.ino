#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
const int sensorPin2 = A0;

// Wi-Fi and Adafruit IO credentials
#define WIFI_SSID       "Rancho"
#define WIFI_PASS       "gixpg300m"
#define AIO_USERNAME    "mhrunisal"
#define AIO_KEY         "aio_ggcC56DrYeVxYposrKzVEKk6GtNU"

// Water flow sensor pin
int sensorPin = D2;

// Create a client instance to connect to Adafruit IO
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, "io.adafruit.com", 1883, AIO_USERNAME, AIO_KEY);

// Create a feed for the water flow data
Adafruit_MQTT_Publish waterFlow = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mhr");

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(sensorPin), increase, RISING);
  delay(10);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Initialize the MQTT connection
  MQTT_connect();
}
float volume;
volatile long pulse;
unsigned long lastTime;

void loop() {
  MQTT_connect();

  // Read data from the water flow sensor (you should have your specific code here)
  delay(2500);
  volume = 2.663 * pulse / 1000 * 30;
  if (millis() - lastTime > 5000) {
    pulse = 0;
    lastTime = millis();
    
  }
  Serial.print(volume);
  Serial.println(" L/m");

  // Publish the water flow data to Adafruit IO
  waterFlow.publish(volume);

  delay(2000);  // Adjust the interval as needed
  
//sensor de corrente 
//  float currentSensorValue = analogRead(sensorPin);
  // Você pode calibrar o valor do sensor para obter leituras de corrente reais
  // Consulte a folha de dados do ACS712 para obter informações de calibração.
  // Por exemplo, para o ACS712 de 5A, 185mV/A é um valor comum.

//  float current_mA = (currentSensorValue - 512) * 0.185; // Calibrar o valor
//  Serial.print("Corrente: ");
  //Serial.print(current_mA, 2); // Duas casas decimais
  //Serial.println(" mA");

  //delay(1000); // Espera 1 segundo entre as leituras
}
ICACHE_RAM_ATTR void increase() {
  pulse++;
}

void MQTT_connect() {
  int8_t ret;

  // Ensure MQTT connection is established
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println("MQTT Connected!");
}
