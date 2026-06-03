/*
    MAPA DOS PINOS

    SENSOR DE ACELERAÇÃO E GIRO MPU-6050:
        - SCL: GPIO 22
        - SDA: GPIO 21
        - INT: GPIO 4 (opcional para detecção de movimento)
        - XDA: GPIO 16 (opcional para comunicação I2C)
        - XCL: GPIO 17 (opcional para comunicação I2C)
        - ADO: GPIO 0 (Endereço I2C, pode ser conectado a GND ou 3.3V para definir o endereço do sensor)

    SENSOR DE CORRENTE ACS712:
        - VCC: 5V
        - GND: GND
        - OUT: GPIO 34 (entrada analógica para leitura do valor de corrente)

    SENSOR DE PRESSÃO BMP280:
        - VCC: 3.3V
        - GND: GND
        - SCL: GPIO 22 (I2C Clock)
        - SDA: GPIO 21 (I2C Data)
    
    MÓDULO MICRO SD:
        - VCC: 3.3V
        - GND: GND
        - CS: GPIO 5 (Chip Select)
        - SCK: GPIO 18 (Serial Clock)
        - MOSI: GPIO 23 (Master Out Slave In)
        - MISO: GPIO 19 (Master In Slave Out)
    
    MAPA DO PROGRAMA:
        - Configuração dos sensores (MPU-6050, ACS712, BMP280)
*/

/* CONTROLE ENERGÉTICO */

/* FAZER PROCESSAMENTO PARALELO - */

/* GRAVAR DADOS EM MEMÓRIA SECUNDÁRIA - MÓDULO MICRO SD*/

/* CALCULAR CONSUMO DE ENERGIA - SENSOR DE CORRENTE ACS712 */

/* CALCULAR PRESSÃO - SENSOR BMP280*/

/* ACELERAÇÃO E GIRO - SENSOR MPU-6050*/

/*
* 3 eixos para o acelerômetro
* 3 eixos para o giroscópio, sendo ao todo 6 graus de liberdade (6DOF)
* Sensor de temperatura integrado: O MPU-6050 também possui um sensor de temperatura integrado, que pode ser utilizado para monitorar a temperatura ambiente ou a temperatura do próprio sensor. Medições entre -40 e +85 ºC
* Sensor captura os canais X, Y e Z ao mesmo tempo, permitindo uma análise completa do movimento em três dimensões.

* Pinout ESP32:
* 3V3 - Alimentação de 3.3V
* GND - Terra
* SCL - GPIO 22 (I2C Clock)
* SDA - GPIO 21 (I2C Data)
* INT - GPIO 4 (Interrupt, opcional para detecção de movimento)
* XDA - GPIO 16 (I2C Data, opcional para comunicação I2C)
* XCL - GPIO 17 (I2C Clock, opcional para comunicação I2C)
* ADO - GPIO 0 (Endereço I2C, pode ser conectado a GND ou 3.3V para definir o endereço do sensor)
*/
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

/* GRAVAR DADOS NO CARTÃO DE MEMÓRIA */

/* ENVIAR DADOS VIA INTERNET (WiFi/3G/4G/LoRa)*/

/* EXIBIR GRÁFICOS DE DESEMPENHO */

/* ENVIAR DADOS PARA INTERFACE WEB */

/* PAINEL DE CONTROLE FÍSICO */

/* PAINEL DE CONTROLE REMOTO */

/* INTERFACE WEB / WEBSERVER */
#include <WiFi.h>                                                                 //Inclusão das bibliotecas
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define BMP_SDA 21                                                                //Definição dos pinos I2C
#define BMP_SCL 22
 
Adafruit_BMP280 bmp;                                                              //Inicia o objeto do sensor

WebServer sv(80);
  
const char* ssid= "nome da sua rede";                                             //Dados da sua rede Wi-Fi                                                
const char* senha = "senha da sua rede";

void conectado() {                                                                //Sub-rotina para caso o servidor fique online
  sv.send(200, "text/html", html(bmp.readTemperature(), bmp.readPressure()));     //Envia ao servidor, em formato HTML, o nosso script, com os parâmetros de pressão e temperatura
}

void nao_encontrado() {                                                           //Sub-rotina para caso seja retornado um erro
  sv.send(404, "text/plain", "Não encontrado");                                   //Retorna a mensagem de erro em caso de um retorno 404
}

String html(float temperatura, float pressao) {                                   //Variável que armazenará o script HTML
  String cd = "<!DOCTYPE html>\n";
  cd += "<html lang=\"pt-br\">\n";
  cd += "<head>\n";
  cd += "<meta charset=\"UTF-8\">\n";
  cd += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
  cd += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";

  cd += "<title>Web Server ORBISAT</title>\n";

  cd += "<style>\n";
  cd += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  cd += "body{margin-top: 50px;} \n";
  cd += "h1 {color: #444444; margin: 50px auto 30px;}\n";
  cd += "p {font-size: 24px; color: #444444; margin-bottom: 10px;}\n";
  cd += "</style>\n";

  cd += "</head>\n";
  cd += "<body>\n";
  cd += "<div id=\"webpage\">\n";
  cd += "<h1>Dados dos sensores</h1>\n";
  cd += "<p>Temperatura: ";
  cd += (int)temperatura;
  cd += " *C</p>\n";
  cd += "<p>Umidade: 65%</p>\n";
  cd += "<p>Pressão Atmosférica: ";
  cd += (int)pressao / 101325;
  cd += " atm</p>\n";
  cd += "</div>\n";
  cd += "</body>\n";  
  cd += "</html>\n";

  return cd;                                                                      //Retorna o script                                             
} 

void setup() {
  Serial.begin(115200);                                                           //Inicia o monitor serial
  delay(100);

  if (!bmp.begin(0x76)) {                                                         //Tenta iniciar o sensor
    Serial.println("Sensor não encontrado");
    while(1);
  }

  Serial.print("Se conectando a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, senha);                                                        //Se conecta ao Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {                                         //Verifica se a conexão foi bem-sucedida
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());                                                 //Imprime o endereço de IP

  sv.on("/", conectado);
  sv.onNotFound(nao_encontrado);
  sv.begin();                                                                     //Inicia o servidor

  Serial.println("Servidor Online");
}

void loop() { 
  sv.handleClient();                                                           //Executa as ações do servidor
}

void setup(void)
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10); // espera pela conexão serial
    }
    // Inicializa o sensor MPU-6050
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_16_G); // Configura o alcance do acelerômetro para ±16g
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);       // Configura o alcance do giroscópio para ±250°/s
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);    // Configura a largura de banda do filtro para 21 Hz
    Serial.println("");
    delay(100); // Pequena pausa para estabilizar o sensor
}

acelerometroGyroscopio(sensors_event_t a, sensors_event_t g, sensors_event_t temp)
{
    /* LEITURA DE DADOS DO SENSOR MPU-6050 */

    /* Imprimir valores */
    Serial.print("Acelerômetro ");
    Serial.print("X: ");
    Serial.print(a.acceleration.x);
    Serial.print(" m/s^2, ");
    Serial.print("Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(" m/s^2, ");
    Serial.print("Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");
    Serial.print("Giroscópio ");
    Serial.print("X: ");
    Serial.print(g.gyro.x);
    Serial.print(" rad/s, ");
    Serial.print("Y: ");
    Serial.print(g.gyro.y);
    Serial.print(" rad/s, ");
    Serial.print("Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");
}

void loop()
{
    /* LEITURA DE DADOS DO SENSOR MPU-6050 */

    sensors_event_t a, g, temp; // Variáveis para armazenar os eventos do acelerômetro, giroscópio e temperatura
    mpu.getEvent(&a, &g, &temp); // Obtém os eventos do acelerômetro, giroscópio e temperatura
    acelerometroGyroscopio(a, g, temp); // Chama a função para processar e imprimir os dados do acelerômetro e giroscópio
    delay(10);

    /* INTERFACEAMENTO PARA APLICATIVO MOBILE */
    /* Desenvolvimento de Dart + Flutter para aplicativo de apresentação para celulares. Modelo de representação de pitch*/

    /* INTERFACEAMENTO COM SERVIDORES - Firebase, mosquitto.org ou equivalente*/
    /* Desenvolver envio de informações para base de servidores e reaproveitar os dados de forma prática
    Aplicar conceitos de IoT para comunicação e aplicações a respeito dos sensores e aplicações*/
}