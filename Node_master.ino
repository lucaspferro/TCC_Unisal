#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

// Informações da sua conexão Wi-Fi
#define WIFI_SSID "Sala-2"
#define WIFI_PASS "gixpg300m"

// Informações de conexão MQTT do Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "mhrunisal"
#define AIO_KEY         "aio_OpYU48IF9T5BNzZibh6fhyQV4esL"

// Pino do sensor de fluxo conectado ao ESP8266 (exemplo: D2)
const int sensorPin = D2;

volatile int pulseCount = 0;
float flowRate = 0.0;
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime = 0;

// Crie uma instância WiFiClient
WiFiClient client;

// Crie uma instância do cliente MQTT
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Crie uma instância de publicação MQTT para enviar dados
Adafruit_MQTT_Publish flowFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mhr");
const char* AIO_FEED = AIO_USERNAME "/feeds/";
Adafruit_MQTT_Subscribe button = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Botao_valvula");

 

ICACHE_RAM_ATTR void pulseCounter() {
  pulseCount++;
}

void setup() {
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH); // Ativa o pull-up interno

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, FALLING);

  // Conecta-se à rede Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
   mqtt.subscribe(&button);


  // Inicializa a conexão MQTT
  mqtt.connect();
    Wire.begin();

}

void loop() {
  MQTT_connect();
// ---- Acionar o relé no Arduino ----
//codigo para entrada de dados do adafruirt
// Monitore o feed no Adafruit IO para controlar o relé
   Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(1000))) {
       Serial.println(" funciona pfr ");
    if (subscription == &button) {
      Serial.print("Estado do botão: ");
      Serial.println((char *)button.lastread);

      // Leitura do estado do botão (ON ou OFF)
      if (strcmp((char *)button.lastread, "ON") == 0) {
        // Envie o estado do botão via I2C para outro dispositivo
        Serial.println("1");
      } else if (strcmp((char *)button.lastread, "OFF") == 0) {
        // Envie o estado do botão via I2C para outro dispositivo
           Serial.println("0");;
      }
    }
  }
//   Wire.beginTransmission(8); // Endereço I2C do Arduino Escravo
//   Wire.write(1); // 1 para ligar o relé, 0 para desligar
//   Wire.endTransmission();
//
//   delay(2000); // Aguarde 2 segundos antes de desligar o relé
//
//  // Envie um comando para desligar o relé
//
//   Wire.beginTransmission(8); // Endereço I2C do Arduino Escravo
//   Wire.write(0); // 1 para ligar o relé, 0 para desligar
//   Wire.endTransmission();
//
//   delay(2000); // Aguarde 2 segundos antes de ligar o relé novamente

  // Certifique-se de definir corretamente o pino do relé e configurá-lo.

  // ---- Sensor de Tensão ----
  Wire.requestFrom(8, 4); // Endereço I2C do Arduino Escravo e quantidade de bytes esperada

  if (Wire.available() >= 4) {
    byte data[4];
    for (int i = 0; i < 4; i++) {
      data[i] = Wire.read();
    }
    float voltage;
    memcpy(&voltage, data, sizeof(voltage));

    Serial.print("Tensão recebida do Arduino Escravo: ");
    Serial.print(voltage, 2); // Exibe a tensão com duas casas decimais
    Serial.println(" V");
  }

  // ---- Sensor de Fluxo ----
  unsigned long currentTime = millis();
  if (currentTime - oldTime > 1000) {
    flowRate = pulseCount / 7.5; // 7.5 é o fator de escala para o sensor YF-S201
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    oldTime = currentTime;
    Serial.print("Taxa de Vazão: ");
    Serial.print(flowRate);
    Serial.print(" L/min\tFluxo Acumulado: ");
    Serial.print(totalMilliLitres);
    Serial.println(" mL");
    pulseCount = 0;
  }
  
  // Envie o valor do fluxo para o feed no Adafruit IO
  if (flowFeed.publish(flowRate)) {
    Serial.println("Dado de fluxo enviado com sucesso!");
  } else {
    Serial.println("Falha ao enviar dado de fluxo.");
  }
  
  mqtt.processPackets(10000); // Exemplo de timeout de 10 segundos (ajuste conforme necessário)
  delay(100);
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
