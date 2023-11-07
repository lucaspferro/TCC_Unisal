#include <Wire.h>

const int relayPin = 13; // Pino de controle do relé
const int sensorPin = A0; // Pino de leitura do sensor

void setup() {
  Wire.begin(8); // Inicializa o Arduino como escravo com endereço 8
  Wire.onReceive(receiveEvent); // Configura a função de recebimento
  Wire.onRequest(sendData); // Configura a função de solicitação
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Garante que o relé esteja inicialmente desligado
}

void loop() {
  // Leia a tensão do sensor
  int sensorValue = analogRead(sensorPin);
  float voltage = (sensorValue / 1023.0) * 5.0; // Converta o valor para tensão (5V como referência)

  Serial.print("Tensão lida: ");
  Serial.print(voltage, 2); // Exibe a tensão com duas casas decimais
  Serial.println(" V");

  delay(1000);
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
