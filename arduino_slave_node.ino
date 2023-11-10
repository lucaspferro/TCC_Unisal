#include <Wire.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define MY_ADDRESS 8  // Endereço I2C do ESP8266
#define ADAFRUIT_SERVER "io.adafruit.com"
#define ADAFRUIT_PORT   1883
#define ADAFRUIT_USERNAME "seu_usuario_adafruit"
#define ADAFRUIT_KEY "seu_chave_adafruit"

// Defina os tópicos MQTT e os feeds do Adafruit
Adafruit_MQTT_Client mqtt(&WiFiClient, ADAFRUIT_SERVER, ADAFRUIT_PORT, ADAFRUIT_USERNAME, ADAFRUIT_KEY);

// Defina o feed no qual você deseja publicar os dados
Adafruit_MQTT_Publish feedVoltage = Adafruit_MQTT_Publish(&mqtt, ADAFRUIT_USERNAME "/feeds/mhr");
Adafruit_MQTT_Publish feedFlowRate = Adafruit_MQTT_Publish(&mqtt, ADAFRUIT_USERNAME "/feeds/Calculo_vazao_corrente");


void receiveEvent() {
  while (Wire.available()) {
    char identificador = Wire.read();
    

  }
} 
void receiveEvent(int bytes) {
  if (bytes >= sizeof(float) * 2) {
    float voltage = 0.0;
    float flowRate = 0.0;

    Wire.readBytes((byte*)&voltage, sizeof(float));
    Wire.readBytes((byte*)&flowRate, sizeof(float));

    // Faça algo com os dados recebidos, por exemplo, imprimir no Serial Monitor
    Serial.print("Dados Recebidos do Arduino - Tensão: ");
    Serial.print(voltage, 2);
    Serial.print(" V, Fluxo: ");
    Serial.print(flowRate, 2);
    Serial.println(" L/min");
  }
          // Publica os dados nos feeds do Adafruit
    if (feedVoltage.publish(voltage)) {
      Serial.println("Dados de tensão publicados com sucesso no Adafruit!");
    } else {
      Serial.println("Erro ao publicar dados de tensão no Adafruit.");
    }

    if (feedFlowRate.publish(flowRate)) {
      Serial.println("Dados de fluxo publicados com sucesso no Adafruit!");
    } else {
      Serial.println("Erro ao publicar dados de fluxo no Adafruit.");
    }
  }




void setup() {
  Serial.begin(9600);
  Wire.begin(MY_ADDRESS);  // Inicializa a comunicação I2C e define o endereço do ESP8266
  Wire.onReceive(receiveEvent);  // Configura a função de recebimento de dados
  // Conecta-se à rede Wi-Fi
  // Substitua "SSID" e "PASSWORD" pelos detalhes da sua rede Wi-Fi
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede Wi-Fi...");
  }
  Serial.println("Conectado à rede Wi-Fi.");
}

void loop() {
  // Mantenha a conexão MQTT ativa
  if (!mqtt.connected()) {
    Serial.println("Conectando ao broker MQTT...");
    if (mqtt.connect()) {
      Serial.println("Conectado ao broker MQTT!");
    } else {
      Serial.println("Falha na conexão MQTT. Tente novamente em 10 segundos.");
      delay(2000);
    }
  }

  // Loop MQTT para manter a conexão
  mqtt.loop();
}
