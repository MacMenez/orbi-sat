//#include <LoRa.h>
#include <SPI.h>
#include <LoRa_E32.h>

// Pinos para o módulo A
#define SS_A 10 // NSS do módulo A
#define RST_A 9 // RST do módulo A
#define DI0_A 2 // DIO0 do módulo A

// Pinos para o módulo B
#define SS_B 12 // NSS do módulo B
#define RST_B 9 // RST do módulo B (compartilhado com o módulo A)
#define DI0_B 4 // DIO0 do módulo B

// Cria objeto LoRa para o módulo A
LoRa_E32 LoRa_A = LoRa_E32();

// Cria objeto LoRa para o módulo B
LoRa_E32 LoRa_B = LoRa_E32();

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  // Inicializa a comunicação LoRa para o módulo A
  if (!LoRa_A.begin(915E6, SS_A, RST_A, DI0_A)) {
    Serial.println("Erro ao inicializar o LoRa A");
    while (1); // Bloqueia o código se a inicialização falhar
  }

  // Inicializa a comunicação LoRa para o módulo B
  if (!LoRa_B.begin(915E6, SS_B, RST_B, DI0_B)) {
    Serial.println("Erro ao inicializar o LoRa B");
    while (1); // Bloqueia o código se a inicialização falhar
  }
}

void loop() {
  // Transmitir mensagem do módulo A
  String mensagem = "Olá do módulo A!";
  LoRa_A.beginPacket(); // Inicia um pacote LoRa
  LoRa_A.print(mensagem); // Envia a mensagem
  LoRa_A.endPacket(); // Termina o pacote

  Serial.println("Mensagem enviada: " + mensagem);
  delay(5000); // Espera 5 segundos

  // Receber mensagem do módulo B
  if (LoRa_B.available()) {
    String mensagem = LoRa_B.readString();
    Serial.println("Mensagem recebida: " + mensagem);

    // Aciona o LED do Arduino Nano
    digitalWrite(13, HIGH);
    delay(500); // Mantém o LED ligado por 0,5 segundo
    digitalWrite(13, LOW); // Desliga o LED
  }
}