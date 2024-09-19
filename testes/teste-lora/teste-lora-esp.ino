// Definições de Pinos
#define pinRX 16  // Altere para o pino desejado
#define pinTX 17  // Altere para o pino desejado

#define pinLed 11

// Definições
#define msg1 "OK"
#define msg2 "ajuda"

#define DISPONIVEL 0
#define ESPERANDO 1
#define CHAMANDO 2

// Declaração de Variáveis
int funcao = 0;

void setup() {
  Serial.begin(9600);
  Serial.setPins(pinRX, pinTX); // Define os pinos RX e TX se necessário

  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);

  delay(2000);
  Serial.println("Fim Setup");
}

void loop() {
  // Enviar mensagem
  if (funcao == DISPONIVEL) {
    Serial.print(msg2);
    Serial.print("enviando ");
    Serial.println(msg2);
    funcao = ESPERANDO;

    if (Serial.available()) {
      String recebido = Serial.readString();
      Serial.print(recebido);

      if (recebido == msg2) { funcao = CHAMANDO; }
    }
    // Se mandou mensagem
    else if (funcao == ESPERANDO) {
      Serial.print(msg1);
      funcao = DISPONIVEL;

      if (Serial.available()) {
        String recebido = Serial.readString();
        Serial.print(recebido);

        if (recebido == msg1) {
          digitalWrite(pinLed, HIGH);
          delay(5000);
          digitalWrite(pinLed, LOW);
          funcao = DISPONIVEL;
        }
      }
    }
    //
    else if (funcao == CHAMANDO) {
      if (Serial.available()) {
        String recebido = Serial.readString();
        Serial.print(recebido);

        if (recebido == msg1) { funcao = DISPONIVEL; }
      }
    }
  }
  piscaLed(funcao);
}

// Função de Piscar LED
void piscaLed(int modo) {
  switch (modo) {
    case ESPERANDO:
      digitalWrite(pinLed, bitRead(millis(), 10));
      break;
    case CHAMANDO:
      digitalWrite(pinLed, bitRead(millis(), 18));
      break;
    default:
      digitalWrite(pinLed, LOW);
  }
}
