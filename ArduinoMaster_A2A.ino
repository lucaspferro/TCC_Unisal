#include <Wire.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>


const int relayPin = 13; // Pino de controle do relé

/ Pino do sensor de fluxo conectado ao ESP8266 (exemplo: D2)
const int sensorPin = D2;

volatile int pulseCount = 0;
float flowRate = 0.0; // Taxa de fluxo em L/min
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime = 0;

 void pulseCounter() {
  pulseCount++;
}

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Garante que o relé esteja inicialmente desligado
  Wire.begin();

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, FALLING);
}

void loop() {
// Leia a tensão do sensor
  int sensorValue = analogRead(sensorPin);
  float voltage = (sensorValue / 1023.0) * 5.0; // Converta o valor para tensão (5V como referência)
  Serial.print("Tensão lida: ");
  Serial.print(voltage, 2); // Exibe a tensão com duas casas decimais
  Serial.println(" V");

  delay(1000);
//----Sensor de Fluxo----
  unsigned long currentTime = millis();
  if (currentTime - oldTime > 1000) {
    flowRate = pulseCount / 7.5; // 7.5 é o fator de escala para o sensor YF-S201
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    oldTime = currentTime;
    Serial.print("Taxa de Vazão: ");
    Serial.print(flowRate);
    Serial.print("L/min\tFluxo Acumulado: ");
    Serial.print(totalMilliLitres);
    Serial.println(" mL");
    pulseCount = 0;
  }

  Wire.beginTransmission(8);  // Endereço I2C do ESP8266
  Wire.write("SENSORES:");    // Identificador de dados dos sensores
  Wire.write((byte*)&tensao, sizeof(voltage));  // Envia a tensão como um array de bytes
  Wire.write((byte*)&fluxo, sizeof(flowRate));    // Envia o fluxo como um array de bytes
  Wire.endTransmission();

return 0;



}

void receiveEvent(int bytes) {
  int command = Wire.read();
  if (command == 1) {
    digitalWrite(relayPin, HIGH); // Liga o relé
  } else if (command == 0) {
    digitalWrite(relayPin, LOW); // Desliga o relé
  }
}

