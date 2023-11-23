// Código apenas para demonstração da lógica, necessário fazer as alterações para receber os dados vindos do ADAFRUIT IO, que ira ativar o modo viagem.

const int pinoBotao = 2; // Pino digital onde o botão está conectado
const int pinoRelay = 9; // Pino digital onde o relé está conectado

void setup() {
  pinMode(pinoBotao, INPUT); // Define o pino do botão como entrada
}

void loop() {
  int estadoBotao = digitalRead(pinoBotao); // Lê o estado do botão

  // Se o botão for pressionado (estado LOW)
  if (estadoBotao == LOW) {
    while (estadoBotao == LOW) { // Enquanto o botão estiver pressionado, mantenha o modo viagem ativo
      int valorVazao = digitalRead(SensorVazao); // Valor da variavel da vazão
      
      if (valorVazao >= "Especificar valor em litros") { // Enquanto o botão estiver pressionado, mantenha o relé ligado

        // Colocar o código que envia o email para o usuário avisando que o modo viagem foi ativado

        while (estadoBotao == LOW) {
          digitalWrite(pinoRelay, HIGH); // Liga o relé
          estadoBotao = digitalRead(pinoBotao); // Atualiza o estado do botão;
        }

      estadoBotao = digitalRead(pinoBotao); // Atualiza o estado do botão
      }
    }  
  }else {
    // Quando o botão for solto, desliga o relé
    digitalWrite(pinoRelay, LOW);
  }
}