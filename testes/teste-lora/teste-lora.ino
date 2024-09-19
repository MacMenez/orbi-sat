// //Inclusão de Bibliotecas
// #include <SoftwareSerial.h>

// // Definições de Pinos
// #define pinRX 7
// #define pinTx 8

// #define pinLed 11

// // Definições
// #define msg1 "OK"
// #define msg2 "ajuda"

// #define DISPONIVEL 0
// #define ESPERANDO 1
// #define CHAMANDO 2

// // Instanciando Objetos
// SoftwareSerial lora(pinRX, pinTx);

// // Declaração de Funções
// void piscaLed(int modo);

// // Declaração de Variáveis
// int funcao = 0;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   lora.begin(9600);

//   pinMode(pinLed, OUTPUT);
//   digitalWrite(pinLed, LOW);

//   delay(2000);

//   Serial.println("Fim Setup");
// }

// void loop() {
//   // put your main code here, to run repeatedly:

//   // Enviar mensagem
//   if (funcao == DISPONIVEL) {
//     // Usar botão para enviar mensagem se neessário, neste caso, colocar as linhas abaixo dentro da verificação
//     lora.print(msg2);
//     Serial.print("enviando");
//     Serial.println(msg2);
//     funcao = ESPERANDO;

//     if (lora.available()) {
//       String recebido = lora.readString();
//       Serial.print(recebido);

//       if (recebido == msg2) { funcao = CHAMANDO; }
//     }
//     // Se mandou mensagem
//     else if (funcao == ESPERANDO) {
//       // Usar botão para enviar mensagem se neessário, neste caso, colocar as 2 linhas abaixo dentro da verificação
//       lora.print(msg1);
//       funcao == DISPONIVEL;

//       if (lora.available()) {
//         String recebido = lora.readString();
//         Serial.print(recebido);

//         if (recebido == msg1) {
//           digitalWrite(pinLed, HIGH);
//           delay(5000);
//           digitalWrite(pinLed, LOW);

//           funcao == DISPONIVEL;
//         }
//       }
//     }
//     //
//     else if (funcao == CHAMANDO) {
//       // Usar botão para enviar mensagem se neessário, neste caso, colocar as 2 linhas abaixo dentro da verificação
//       String recebido = lora.readString();
//       lora.print(recebido);

//       if (lora.available()) {
//         Serial.print(recebido);
//         if (recebido == msg1) { funcao = DISPONIVEL; }
//       }
//     }
//   }
//   piscaLed(funcao);
// }
// // Função de Piscar LED
// void piscaLed(int modo) {
//   switch (modo) {
//     case ESPERANDO:
//       digitalWrite(pinLed, bitRead(millis(), 10));
//       break;
//     case CHAMANDO:
//       digitalWrite(pinLed, bitRead(millis(), 18));
//     default:
//       digitalWrite(pinLed, LOW);
//   }
// }