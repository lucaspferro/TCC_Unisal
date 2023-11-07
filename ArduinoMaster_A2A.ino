#include <Wire.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>
#include <A2a.h>
#define endereco 0x08

A2a arduinoMaster;

const int relayPin = 13; // Pino de controle do relé

/ Pino do sensor de fluxo conectado ao ESP8266 (exemplo: D2)
const int sensorPin = D2;

volatile int pulseCount = 0;
float flowRate = 0.0; // Taxa de fluxo em L/min
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime = 0;

ICACHE_RAM_ATTR void pulseCounter() {
  pulseCount++;
}

void setup() {
  Wire.begin(8); // Inicializa o Arduino como escravo com endereço 8
  Wire.onReceive(receiveEvent); // Configura a função de recebimento
  Wire.onRequest(sendData); // Configura a função de solicitação
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Garante que o relé esteja inicialmente desligado
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH); // Ativa o pull-up interno

  Serial.begin(115200);

  Wire.begin();

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
  arduinoMaster.varWireWrite(0,(totalMilliLitres));
  delay(100);
//--------------------

  arduinoMaster.varWireWrite(2, (voltage));
  delay(100);
 //-------------------

 //arduinoMaster.varWireWrite(6, (Irms));
 //arduinoMaster.begin(endereco);
 delay(100);
 //-------------------

 //arduinoMaster.varWireWrite(4, (FP));
  delay(100);


 arduinoMaster.begin(endereco);
 arduinoMaster.onReceive(receberDados);
 arduinoMaster.onRequest(enviarDados);
// arduinoMaster.endTransmission();
 
  delay (2000);
  Serial.print("***************");
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

void sendData() {
  // Quando o Arduino Mestre solicitar dados, envie a tensão lida
  int sensorValue = analogRead(sensorPin);
  float voltage = (sensorValue / 1023.0) * 5.0; // Converta o valor para tensão (5V como referência)

  // Envie os dados para o Arduino Mestre
  byte data[4];
  memcpy(data, &voltage, sizeof(voltage));
  Wire.write(data, sizeof(data));
}

void receberDados(){
  arduinoMaster.receiveData();
 }

void enviarDados(){
  arduinoMaster.sendData();
  }
