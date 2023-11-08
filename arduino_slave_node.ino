// INCLUSÃO DE BIBLIOTECAS
#include <A2a.h>
#include "config.h"

// DEFINIÇÕES
#define endereco 0x08
#define tempoAtualizacao 5000

// INSTANCIANDO OBJETOS
AdafruitIO_Feed *counter = io.feed("counter");
AdafruitIO_Feed *frequencia = io.feed("frequencia");
AdafruitIO_Feed *corrente = io.feed("corrente");
AdafruitIO_Feed *fatorpotencia = io.feed("fatorpotencia");
AdafruitIO_Feed *displayLED = io.feed("displayLED");

A2a arduinoSlave; // esse é o slave 

// DECLARAÇÃO DE FUNÇÕES
void configuraMQTT();
void retornoDisplayLED(AdafruitIO_Data *data);
bool monitoraTensao ();
bool monitoraFrequencia();
bool monitorafatorPotencia();

// DECLARAÇÃO DE VARIÁVEIS
unsigned long controleTempo = 0;
bool comandoRecebido = false;
unsigned int valorTensao, valorFrequencia, valorFP;

void setup() {
   Serial.begin(9600);
  while (! Serial);

  configuraMQTT();

  arduinoSlave.begin(0, 2);

  Serial.println("Atualizando valor do Display de LED");
  
  displayLED->get();
  io.run();
  
  Serial.println("Fim Setup");
 
}

void loop() {
    io.run(); 
// --------------------TENSAO-----------------------// 
  if (millis() > controleTempo + tempoAtualizacao) {
   if (monitoraTensao()) {
      controleTempo = millis();
        counter->save(valorTensao);
        Serial.println(valorTensao);    }
  }
//---------frequencia------------------------------
  if (millis() > controleTempo + tempoAtualizacao) {
    if (monitoraFrequencia()) {
      controleTempo = millis();
        frequencia->save(valorFrequencia);
        Serial.println(valorFrequencia);    }
  }
//------------fator potencia-------------------
  
if (millis() > controleTempo + tempoAtualizacao) {
    if (monitorafatorPotencia()) {
      controleTempo = millis();
        fatorpotencia->save(valorFP);
        Serial.println(valorFP);    }
  }
}
 


void receiveEvent(int bytes) {
 if (Serial.available() > 0) {
    String receivedData = Serial.readString();
    
    if (receivedData == "1") {
      digitalWrite(relayPin, HIGH); // Liga o relé
      Serial.println("Relé ligado");
    } else if (receivedData == "0") {
      digitalWrite(relayPin, LOW); // Desliga o relé
      Serial.println("Relé desligado");
    }
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
