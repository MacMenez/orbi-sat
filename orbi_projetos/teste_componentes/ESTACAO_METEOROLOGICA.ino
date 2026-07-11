/*
  MAPA DOS PINOS

  LDR 1
  BOTAO 2
  BOTAO 3
  BOTAO 4
  BOTAO 5
  BOTAO 6
  BOTAO 7
  PAINEL 8
  PAINEL 9
  MOTOR 10
  CHUVA 11
  SOLO 12
  REGADOR 13
  REGADOR 14
  DHT 15
  SIRENE 17
  LED TESTE 33
  
  MAPA DO PROGRAMA

  CONFIGURAÇÕES DE REDES: Tudo relacionado a conexões e configurações com o broker, internet e etc.
  ÁREA DE TESTE: Gerar temperatura aleatória + publicar no broker e testar LED. VEM PRIMEIRO PARA CONSEGUIR TESTAR O RESTO
  CONFIGURAÇÕES DO PAINEL OLED: Local com tudo relacionado a configuração do painel OLED
  CONFIGURAÇÕES DE LDR: Todas as configurações para o Piranômetro e Heliógrafo
  CONFIGURAÇÕES DA SIRENE ALERTA: Todas as configurações para alertas e sinais sonoros
  CONFIGURAÇÕES DE TEMPERATURA E HUMIDADE: Todas as configurações para o temometro e relatividade do ar
  CONFIGURAÇÕES DA COBERTURA PLANTAÇÃO: Todas as configurações para ativar protetores das platações
  CONFIGURAÇÕES DO REGADOR: Todas as configurações para controlar a rega das plantações
  CONFIGURAÇÕES DE CHUVA: Todas as configurações relacionadas ao monitoramento das chuvas
  CONFIGURAÇÕES DE UMIDADE DE SOLO: Todas as configurações do monitoramento e análise de rega do solo
  Prototypes: Local para as funções que mantém a conexão com a rede
  Funções de Conexão de Rede: Local para realizar as ativações e estabelecer as conxões com a rede
*/

/* INÍCIO CONFIGURAÇÕES DE REDES */
  #include <arduino.h>
  #include <WiFi.h>
  #include <PubSubClient.h>

  #define ID_MQTT "IoT_PUC_SG_mqtt"  //id mqtt (para identificação de sessão)

  // Informar dados de acesso à rede
  // const char* SSID     = TOPICO_SUBSCRIBE_LOGIN_REDE_USUARIO;
  // const char* PASSWORD = TOPICO_SUBSCRIBE_LOGIN_REDE_SENHA;
  
  // const char* SSID = "moto g(6) play 1143";  // SSID / nome da rede WI-FI que deseja se conectar
  // const char* PASSWORD = "123456789";        // Senha da rede WI-FI que deseja se conectar

  const char* SSID     =  "Menez"; // SSID / nome da rede WI-FI que deseja se conectar
  const char* PASSWORD =  "best#menez"; // Senha da rede WI-FI que deseja se conectar
  
  // const char* SSID     =  "iPhone"; // SSID / nome da rede WI-FI que deseja se conectar
  // const char* PASSWORD =  "12345678"; // Senha da rede WI-FI que deseja se conectar

  // Informar daods de conexão do servidor
  const char* BROKER_MQTT = "test.mosquitto.org";
  int BROKER_PORT = 1883;  // Porta do Broker MQTT

  //Variáveis e objetos globais
  WiFiClient espClient;          // Cria o objeto espClient
  PubSubClient MQTT(espClient);  // Instancia o Cliente MQTT passando o objeto espClient
/* FIM CONFIGURAÇÕES DE REDES */



/* INÍCIO CONFIGURAÇÕES PARA TESTES */
  //#define LED_BUILTIN 2 - BOTÃO DE TESTE
  #define PIN_LED 33

  /* Definicoes para o MQTT */
  #define TOPICO_SUBSCRIBE_LED "topico_liga_desliga_led"
  #define TOPICO_PUBLISH_TEMPERATURA "topico_sensor_temperatura"

  long numAleatorio;

  void gerarTempAleat() {
    char temperatura_str[10] = { 0 };               // cria string para temperatura
    numAleatorio = random(10, 101);                 // gera um valor aleatório de temperatura entre 10 e 100
    sprintf(temperatura_str, "%dC", numAleatorio);  // formata a temperatura aleatoria  como string

    Serial.print("Gerando temperatura aleatoria: ");
    Serial.println(temperatura_str);

    /*  Publica a temperatura */
    MQTT.publish(TOPICO_PUBLISH_TEMPERATURA, temperatura_str);
  }

  void testeLED() {
    digitalWrite(PIN_LED, HIGH);
    delay(1000);
    digitalWrite(PIN_LED, LOW);  // apaga o LED
  }
/* FIM CONFIGURAÇÕES PARA TESTES */

/* INÍCIO CONFIGURAÇÕES DE LDR */
  // MQTT DO LDR - ANÁLISE DO SOL
  #define TOPICO_PUBLISH_PIRANOMETRO "iot_puc_minas_topico_sensor_ldr_piranometro"
  #define TOPICO_PUBLISH_ANOMALIA "iot_puc_minas_topico_sensor_ldr_topico_anomalia"

  #define TOPICO_PUBLISH_HELIOGRAFO_SOL "iot_puc_minas_topico_sensor_ldr_heliografo_sol"
  #define TOPICO_PUBLISH_HELIOGRAFO_LUMINOSIDADE "iot_puc_minas_topico_sensor_ldr_heliografo_luminosidade"
  #define TOPICO_PUBLISH_ECOSSISTEMA "iot_puc_minas_topico_sensor_ldr_topico_ecossistema"

  #define LDR 1 /* CONFIGURAÇÃO DO SENSOR LDR - PINAGEM */

  // Variáveis para controle do tempo das leituras
  unsigned long t_anterior_piranometro = 0; // Função millis para o LDR simular um Piranômetro
  unsigned long t_anterior_heliografo = 0; // Função millis para o LDR simular um Heliógrafo
  const long intervaloPiranometro = 30000; // Intervalo de leitura do piranômetro (30 segundos)
  const long intervaloHeliografo = 60000; // Intervalo de leitura do heliógrafo (1 minuto)

  // Variáveis para medições do heliógrafo
  unsigned long duracaoBrilhoSolar = 0; // Duração do brilho solar em milissegundos
  unsigned long inicioBrilhoSolar = 0;
  bool brilhoSolarAtivo = false;

  /* Funções de Leitura e Formatação do sensor */
  void heliografo(float nivelLuminosidadePct, unsigned long millisAtualLDR) {
    if (nivelLuminosidadePct > 50) {
      if (!brilhoSolarAtivo) {
        inicioBrilhoSolar = millisAtualLDR;
        brilhoSolarAtivo = true;
      }
    } else {
      if (brilhoSolarAtivo) {
        duracaoBrilhoSolar += millisAtualLDR - inicioBrilhoSolar;
        brilhoSolarAtivo = false;
      }
    }

    // Conversão da duração de milissegundos para horas
    // float duracaoBrilhoSolarHoras = duracaoBrilhoSolar / 3600000.0;
    float duracaoBrilhoSolarHoras = duracaoBrilhoSolar / 60000.0; // 1 minuto para apresentar na palestra

    Serial.print("Duração do Brilho Solar: ");
    Serial.print(duracaoBrilhoSolarHoras);
    Serial.println(" horas");

    // Impacto na Agricultura e Ecossistemas
    char statusEcossistema[100] = { 0 };  // Enviar Alertas do Ecossistema Para o MQTT

    if (duracaoBrilhoSolarHoras < 2) {
      strcpy(statusEcossistema, "Alerta: Baixa duração de brilho solar, pode impactar na fotossíntese e crescimento das plantas.");
      Serial.println("Alerta: Baixa duração de brilho solar, pode impactar na fotossíntese e crescimento das plantas.");
    } else if (duracaoBrilhoSolarHoras > 10) {
      strcpy(statusEcossistema, "Nota: Alta duração de brilho solar, favorável para a fotossíntese.");
      Serial.println("Nota: Alta duração de brilho solar, favorável para a fotossíntese.");
    }

    // Envio dos dados do Heliógrafo para o broker MQTT
    MQTT.publish(TOPICO_PUBLISH_ECOSSISTEMA, statusEcossistema);

    strcpy(statusEcossistemaPainel, statusEcossistema);  // Painel Heliógrafo

    char duracaoBrilhoSolarHoras_str[10] = { 0 };
    sprintf(duracaoBrilhoSolarHoras_str, "%.2f", duracaoBrilhoSolarHoras);
    MQTT.publish(TOPICO_PUBLISH_HELIOGRAFO_SOL, duracaoBrilhoSolarHoras_str);

    char nivelLuminosidadePct_str[10] = { 0 };
    sprintf(nivelLuminosidadePct_str, "%.2f", nivelLuminosidadePct);
    MQTT.publish(TOPICO_PUBLISH_HELIOGRAFO_LUMINOSIDADE, nivelLuminosidadePct_str);
  }

  void piranometro(float irradianciaPct) {

    // Análise de Anomalias Climáticas
    char statusAnomalia[100] = { 0 };  // Enviar Alertas de Anomalias Pelo MQTT

    if (irradianciaPct < 20) {
      strcpy(statusAnomalia, "Anomalia Detectada: Baixa Irradiância Solar (Possível tempestade de poeira ou vulcão)");
      Serial.println("Anomalia Detectada: Baixa Irradiância Solar (Possível tempestade de poeira ou vulcão)");
    } else {
      strcpy(statusAnomalia, "Nenhuma Anomalia Detectada!");
      Serial.println("Nenhuma Anomalia Detectada!");
    }

    // Envio dos dados do Piranômetro para o broker MQTT
    MQTT.publish(TOPICO_PUBLISH_ANOMALIA, statusAnomalia);  // Análise de Anomalias

    strcpy(statusAnomaliaPainel, statusAnomalia);  // Painel Piranômetro

    char irradianciaPct_str[10] = { 0 };
    sprintf(irradianciaPct_str, "%.2f", irradianciaPct);
    MQTT.publish(TOPICO_PUBLISH_PIRANOMETRO, irradianciaPct_str);  // Porcentagem de irradianciaPct
  }
/* FIM CONFIGURAÇÕES DE LDR */

/* INÍCIO CONFIGURAÇÕES DE TEMPERATURA E HUMIDADE */
  #include <DHT.h>         // Sensor DHT11 - UMIDADE E TEMPERATURA

  // MQTT DO SENSOR DHT - TEMPERATURA E UMIDADE
  #define TOPICO_PUBLISH_TERMOMETRO "iot_puc_minas_topico_sensor_DHT_temp"
  #define TOPICO_PUBLISH_TEMP_MED "iot_puc_minas_topico_sensor_DHT_TMED"
  #define TOPICO_PUBLISH_TEMP_MAX "iot_puc_minas_topico_sensor_DHT_TMAX"
  #define TOPICO_PUBLISH_TEMP_MIN "iot_puc_minas_topico_sensor_DHT_TMIN"
  #define TOPICO_PUBLISH_UMIDADE "iot_puc_minas_topico_sensor_DHT_hum"
  #define TOPICO_PUBLISH_HUM_MED "iot_puc_minas_topico_sensor_DHT_HMED"
  #define TOPICO_PUBLISH_HUM_MAX "iot_puc_minas_topico_sensor_DHT_HMAX"
  #define TOPICO_PUBLISH_HUM_MIN "iot_puc_minas_topico_sensor_DHT_HMIN"
  #define TOPICO_PUBLISH_IND_CALOR "iot_puc_minas_topico_sensor_DHT_calor"
  #define TOPICO_PUBLISH_PTS_ORVALHO_CALOR "iot_puc_minas_topico_sensor_DHT_orvalho"

  #define DHTPIN 15      // Pino onde o sensor DHT está conectado
  #define DHTTYPE DHT11  // Tipo de sensor DHT

  DHT dht(DHTPIN, DHTTYPE); // Criar objeto para leitura do DHT

  /* Variáveis de Leitura dos Valores Fornecidos Pelo Sensor*/
  float temperatura;
  float umidade;

  /* Variáveis de tempo para leituras */
  unsigned long millisAnteriorTemp = 0;
  unsigned long millisAnteriorHum = 0;
  const long intervaloTemp = 30000;  // Intervalo de 1 minuto para temperatura = 60000
  const long intervaloHum = 30000;   // Intervalo de 1 minuto para umidade = 60000

  /* Variáveis para cálculos de médias e extremos */
  float somaTemp = 0;
  float somaHum = 0;
  int contadorTemp = 0;
  int contadorHum = 0;
  float tempMax = -100.0;
  float tempMin = 100.0;
  float humMax = 0;
  float humMin = 100;

  void termometro(float temperatura) {
    somaTemp += temperatura;
    contadorTemp++;
    if (temperatura > tempMax) tempMax = temperatura;
    if (temperatura < tempMin) tempMin = temperatura;

    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");

    /* Envio dos Dados da Temperatura Para a Nuvem */
    char temperatura_str[10] = { 0 };
    sprintf(temperatura_str, "%.2f", temperatura);
    MQTT.publish(TOPICO_PUBLISH_TERMOMETRO, temperatura_str);
  }

  void umidadeRelativaAr(float umidade) {
    somaHum += umidade;
    contadorHum++;
    if (umidade > humMax) humMax = umidade;
    if (umidade < humMin) humMin = umidade;
    
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");

    /* Envio dos Dados da Umidade Para a Nuvem */
    char umidade_str[10] = { 0 };
    sprintf(umidade_str, "%.2f", umidade);
    MQTT.publish(TOPICO_PUBLISH_UMIDADE, umidade_str);
  }

  void analiseCalculoMed(float tempAvg, float humAvg) {
    Serial.print("Temperatura Média: ");
    Serial.print(tempAvg);
    Serial.println(" °C");

    /* Envio dos Dados da Temperatura Para a Nuvem */
    char tempMed_str[10] = { 0 };
    sprintf(tempMed_str, "%.2f", tempAvg);
    MQTT.publish(TOPICO_PUBLISH_TEMP_MED, tempMed_str);

    Serial.print("Umidade Média: ");
    Serial.print(humAvg);
    Serial.println(" %");

    /* Envio dos Dados da Umidade Para a Nuvem */
    char humMed_str[10] = { 0 };
    sprintf(humMed_str, "%.2f", humAvg);
    MQTT.publish(TOPICO_PUBLISH_HUM_MED, humMed_str);
  }

  void analiseCalculoMax() {
    Serial.print("Temperatura Máxima: ");
    Serial.print(tempMax);
    Serial.println(" °C");

    /* Envio dos Dados da Temperatura Para a Nuvem */
    char tempMax_str[10] = { 0 };
    sprintf(tempMax_str, "%.2f", tempMax);
    MQTT.publish(TOPICO_PUBLISH_TEMP_MAX, tempMax_str);

    Serial.print("Umidade Máxima: ");
    Serial.print(humMax);
    Serial.println(" %");

    /* Envio dos Dados da Umidade Para a Nuvem */
    char humMax_str[10] = { 0 };
    sprintf(humMax_str, "%.2f", humMax);
    MQTT.publish(TOPICO_PUBLISH_HUM_MAX, humMax_str);
  }

  void analiseCalculoMin() {
    Serial.print("Temperatura Mínima: ");
    Serial.print(tempMin);
    Serial.println(" °C");

    /* Envio dos Dados da Temperatura Para a Nuvem */
    char tempMin_str[10] = { 0 };
    sprintf(tempMin_str, "%.2f", tempMin);
    MQTT.publish(TOPICO_PUBLISH_TEMP_MIN, tempMin_str);

    Serial.print("Umidade Mínima: ");
    Serial.print(humMin);
    Serial.println(" %");

    /* Envio dos Dados da Umidade Para a Nuvem */
    char humMin_str[10] = { 0 };
    sprintf(humMin_str, "%.2f", humMin);
    MQTT.publish(TOPICO_PUBLISH_HUM_MIN, humMin_str);
  }

  void indiceDeCalor(float indiceCalor) {
    Serial.print("Índice de Calor: ");
    Serial.print(indiceCalor);
    Serial.println(" °C");

    /* Envio dos Dados da Temperatura Para a Nuvem */
    char ind_calor_str[10] = { 0 };
    sprintf(ind_calor_str, "%.2f", indiceCalor);
    MQTT.publish(TOPICO_PUBLISH_IND_CALOR, ind_calor_str);
  }

  void pontoOrvalho(float pontoDeOrvalho) {
    Serial.print("Ponto de Orvalho: ");
    Serial.print(pontoDeOrvalho);
    Serial.println(" °C");

    /* Envio dos Dados da Temperatura Para a Nuvem */
    char pts_orvalho_str[10] = { 0 };
    sprintf(pts_orvalho_str, "%.2f", pontoDeOrvalho);
    MQTT.publish(TOPICO_PUBLISH_PTS_ORVALHO_CALOR, pts_orvalho_str);
  }


/* FIM CONFIGURAÇÕES DE TEMPERATURA E HUMIDADE */

/* INICIO CONFIGURAÇÕES DA SIRENE ALERTA */
  #define BUZZER 17  // Pino do Buzzer
  int alertaSirene = 0;

  // Variáveis globais para o controle da sirene
  unsigned long tempoAnterior = 0;
  int estadoSirene = 0;

  void alertaChuva() {
    unsigned long tempoAtual = millis();
    const unsigned long intervaloFrequenciaBaixa = 1000;
    const unsigned long intervaloFrequenciaMedia = 1000;
    const unsigned long intervaloDesligada = 100;

    switch (estadoSirene) {
      case 0:  // Estado inicial
        tone(BUZZER, 50);  // Frequência baixa
        tempoAnterior = tempoAtual;
        estadoSirene = 1;
        break;

      case 1:  // Frequência baixa
        if (tempoAtual - tempoAnterior >= intervaloFrequenciaBaixa) {
          tone(BUZZER, 100);  // Frequência média
          tempoAnterior = tempoAtual;
          estadoSirene = 2;
        }
        break;

      case 2:  // Frequência média
        if (tempoAtual - tempoAnterior >= intervaloFrequenciaMedia) {
          noTone(BUZZER);  // Desliga o som temporariamente
          tempoAnterior = tempoAtual;
          estadoSirene = 3;
        }
        break;

      case 3:  // Desligada temporariamente
        if (tempoAtual - tempoAnterior >= intervaloDesligada) {
          estadoSirene = 0;  // Reinicia o ciclo
        }
        break;
    }
  }
/* FIM CONFIGURAÇÕES DA SIRENE ALERTA */

/* INÍCIO CONFIGURAÇÕES DA COBERTURA PLANTAÇÃO */
  #include <ESP32Servo.h>  // Servomotor

  #define MOTOR_COBERTURA 10  // Definindo o pino ao qual o servo está conectado
  // #define MOTOR_COBERTURA 7 // Definindo o pino ao qual o servo está conectado
  Servo cobertura;  // Criando um objeto Servo
  int alnguloDeCobertura = 0;
  bool ativacaoCobertura = true;  // Inicio - Plantação sem a cobertura ativada. Pode mudar(true).

  void ativarCobertura() { cobertura.write(90); }
  // void ativarCobertura() { cobertura.write(180); }
  void desativarCobertura() { cobertura.write(0); }
/* FIM CONFIGURAÇÕES DA COBERTURA PLANTAÇÃO */

/* INÍCIO CONFIGURAÇÕES DO REGADOR */
  #define LED_REGADOR 13        // Rega está acontecendo(Verde)
  #define LED_REGADOR_CHUVA 14  // Não pode regar - chuva (Vermelho)

  bool regadorLigado = true;

  void ativarRegador() { digitalWrite(LED_REGADOR, HIGH); }  // Pino 13 LED VERDE
  void desativarRegador() { digitalWrite(LED_REGADOR, LOW); }  // Pino 13 LED VERDE
/* FIM CONFIGURAÇÕES DO REGADOR */

/* INÍCIO CONFIGURAÇÕES DE CHUVA */
  // MQTT DO SENSOR DE CHUVA
  #define TOPICO_PUBLISH_CHUVA "iot_puc_minas_topico_sensor_chuva"
  #define TOPICO_PUBLISH_CHUVA_PREVISAO "iot_puc_minas_topico_sensor_chuva_previsao"
  #define TOPICO_PUBLISH_CHUVA_ALERTA "iot_puc_minas_topico_sensor_chuva_alerta"

  // MQTT DE CASOS ESPECIAIS
  #define TOPICO_PUBLISH_CHUVA_COBERTURA "iot_puc_minas_topico_chuva_cobertura"           // Ativar a cobertura da plantação no ícone do painel
  #define TOPICO_SUBSCRIBE_CHUVA_COBERTURA "iot_puc_minas_topico_chuva_cobertura_ativar"  // Ativar a cobertura da plantação por botão do painel
  #define TOPICO_PUBLISH_CHUVA_REGADOR "iot_puc_minas_topico_chuva_regador"               // Ativar o regador no ícone do painel
  #define TOPICO_SUBSCRIBE_CHUVA_REGADOR "iot_puc_minas_topico_chuva_regador_ativar"      // Ativar a cobertura da plantação por botão do painel

  #define RAIN_SENSOR 11               // Pino de leitura analógica no ESP32 (GPIO34)
  int leituraAnalogica_Chuva = 0;      // Valor inicial para leitura do sensor de chuva
  float porcentagemChuva = 0.0;        // Valor utilizado para calcular a previsão da quantidade de chuva
  unsigned long t_anterior_chuva = 0;  // Variável para armazenar o último tempo em que a leitura foi feita
  const long intervalo_chuva = 30000;  // Intervalo de 1 minuto (60000 milissegundos)

  void pluviometro(float porcentagemChuva) {
    /* Envio dos Dados do Sensor de Chuva Para a Nuvem */
    char chuva_str[10] = { 0 };                     // String de envio da porcentagem de chuva
    sprintf(chuva_str, "%.2f", porcentagemChuva);   // Formatar porcentagem de chuva como String
    MQTT.publish(TOPICO_PUBLISH_CHUVA, chuva_str);  // Publicar Porcentagem da Chuva

    Serial.print("Porcentagem de Chuva: ");
    Serial.print(porcentagemChuva);
    Serial.println("%");

    // A variação é decrescente, ou seja, seco dá o valor total (4095) até molhado que será próximo de 0.
    char previsaoChuva[100] = { 0 };  // Enviar Alerta de Chuva Para o MQTT

    if (porcentagemChuva <= 10.0) {
      strcpy(previsaoChuva, "Sem previsão de chuva");  // Sem previsão de Chuva
      Serial.println("Sem previsão de chuva");         // Sem previsão de Chuva

      MQTT.publish(TOPICO_PUBLISH_CHUVA_ALERTA, "0");

      ativacaoCobertura = true;  // Permitir que a cobertura seja ativada ou desativado

      regadorLigado = true;  // Permitir que o regador seja ligado ou desligado

      alertaSirene = 0;
    } 
    else if (porcentagemChuva <= 33.33) {
      strcpy(previsaoChuva, "Chuva Leve/Chuvisco");  // Chuva Leve/Chuviscando
      Serial.println("Chuva Leve/Chuvisco");         // Chuva Leve/Chuviscando

      MQTT.publish(TOPICO_PUBLISH_CHUVA_ALERTA, "1");

      ativacaoCobertura = true;  // Permitir que a cobertura seja ativada ou desativada

      MQTT.publish(TOPICO_PUBLISH_CHUVA_REGADOR, "desligarRegadorChuva");
      desativarRegador();
      regadorLigado = false;  // Não tem necessidade de regar com chuva. Proibir ligar o regador.

      alertaSirene = 0;
    } 
    else if (porcentagemChuva <= 66.66) {
      strcpy(previsaoChuva, "Chuva Moderada");  // Chuva Moderada
      Serial.println("Chuva Moderada");         // Chuva Moderada
      MQTT.publish(TOPICO_PUBLISH_CHUVA_ALERTA, "2");

      ativacaoCobertura = true;  // Permitir que a cobertura seja ativada ou desativada

      MQTT.publish(TOPICO_PUBLISH_CHUVA_REGADOR, "desligarRegadorChuva");
      desativarRegador();
      regadorLigado = false;  // Não tem necessidade de regar com chuva. Proibir ligar o regador.

      alertaSirene = 1;
    } 
    else {
      strcpy(previsaoChuva, "Chuva Intensa");  // Chuva intensa
      Serial.println("Chuva Intensa");         // Chuva intensa
      MQTT.publish(TOPICO_PUBLISH_CHUVA_ALERTA, "3");

      /*###################################*/
      alertaSirene = 1;  // Verificar depois com Julio o motivo de estar interferindo no funcionamento dos sensores e do servomotor mesmo mudando o pino
      /*###################################*/

      MQTT.publish(TOPICO_PUBLISH_CHUVA_COBERTURA, "ativarCoberturaChuva");
      ativarCobertura();
      ativacaoCobertura = false;  // NÃO Permitir que a cobertura seja ativada ou desativada

      MQTT.publish(TOPICO_PUBLISH_CHUVA_REGADOR, "desligarRegadorChuva");
      desativarRegador();
      regadorLigado = false;  // Não tem necessidade de regar com chuva. Proibir ligar o regador.
    }
    MQTT.publish(TOPICO_PUBLISH_CHUVA_PREVISAO, previsaoChuva);  // Publicar Porcentagem da Chuva

    strcpy(previsaoChuvaPainel, previsaoChuva);  // Painel PluviômetroF
  }
/* FIM CONFIGURAÇÕES DE CHUVA */

/* INÍCIO CONFIGURAÇÕES DE UMIDADE DE SOLO */
  // MQTT DO SENSOR DE UMIDADE DE SOLO
  #define TOPICO_PUBLISH_MONITORAMENTO_SOLO "iot_puc_minas_topico_sensor_umidade_solo_monitoramento"
  #define TOPICO_PUBLISH_IRRIGACAO "iot_puc_minas_topico_sensor_umidade_solo_irrigacao"
  #define TOPICO_PUBLISH_MONITOR_SECA "iot_puc_minas_topico_sensor_umidade_solo_seca"

  #define UMIDADE_SOLO 12  // Pino analógico do sensor de umidade de solo

  // Variáveis para controle do tempo das leituras
  unsigned long t_anterior_umidade_solo = 0;  // Função Millis para o sensor de umidade do solo
  const long intervaloUmidadeSolo = 30000;    // Intervalo de leitura do sensor de umidade de solo (30 segundos)

  // Variáveis para medições e análise
  float umidadeSolo = 0.0;        // Valor da leitura atual do sensor
  float percentualUmidade = 0;  // Valor da umidade percentual

  void analiseSolo(float percentualUmidade){
    // Análise de Monitoramento de Secas
    char monitoraSeca[100] = { 0 };  // Envar Alerta de Seca Para o MQTT

    if (percentualUmidade < 20) {
      strcpy(monitoraSeca, "Alerta: Nível de umidade do solo crítico - Possível seca!");
      Serial.println("Alerta: Nível de umidade do solo crítico - Possível seca!");
    } else {
      strcpy(monitoraSeca, "Nível de umidade do solo - Normal!");
      Serial.println("Nível de umidade do solo - Normal!");
    }

    // Envio dos dados do Sensor de Umidade de Solo para o broker MQTT
    MQTT.publish(TOPICO_PUBLISH_MONITOR_SECA, monitoraSeca);

    strcpy(monitorSecaPainel, monitoraSeca);  // Painel Pluviômetro

    // Análise de Gestão da Irrigação
    char irrigacao[100] = { 0 };  // Enviar Alerta de Irrigação Para o MQTT

    if (percentualUmidade < 40) {
      strcpy(irrigacao, "Sugestão: Necessário iniciar irrigação.");
      Serial.println("Sugestão: Necessário iniciar irrigação.");
    }
    else if (percentualUmidade > 80) {
      strcpy(irrigacao, "Sugestão: Solo bem irrigado. Evitar excesso de água.");
      Serial.println("Sugestão: Solo bem irrigado. Evitar excesso de água.");
    }

    // Envio dos dados do Sensor de Umidade de Solo para o broker MQTT
    MQTT.publish(TOPICO_PUBLISH_IRRIGACAO, irrigacao);

    strcpy(irrigacaoPainel, irrigacao);  // Painel Puviômetro

    char percentualUmidade_str[10] = { 0 };
    sprintf(percentualUmidade_str, "%.2f", percentualUmidade);
    MQTT.publish(TOPICO_PUBLISH_MONITORAMENTO_SOLO, percentualUmidade_str);
  }

/* FIM CONFIGURAÇÕES DE UMIDADE DE SOLO */

/* Prototypes */
void initWiFi(void);
void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);

/* Implementações de Funções de Conexão de Rede */

/* Função: inicializa e conecta-se na rede WI-FI desejada | Parâmetros: nenhum | Retorno: nenhum */
void initWiFi(void) {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconnectWiFi();
}

/* Função: inicializa parâmetros de conexão MQTT(endereço do broker, porta e inicializa a função de callback) | Parâmetros: nenhum | Retorno: nenhum */
void initMQTT(void) {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);  //informa qual broker e porta deve ser conectado
  MQTT.setCallback(mqtt_callback);           //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

/* Função: função de callback esta função é chamada toda vez que uma informação de um dos tópicos subescritos chega) | Parâmetros: nenhum | Retorno: nenhum */
void mqtt_callback(char* topic, byte* payload, unsigned int length){
  String msg;

  /* obtem a string do payload recebido */
  for (int i = 0; i < length; i++){
    char c = (char)payload[i];
    msg += c;
  }

  Serial.print("Chegou a seguinte string via MQTT: ");
  Serial.println(msg);
  /* toma ação dependendo da string recebida */
  if (msg.equals("ativaLEDTeste")){
    digitalWrite(PIN_LED, HIGH);
    Serial.println("LED aceso mediante comando MQTT");
  }
  else if(msg.equals("desativaLEDTeste")){
    digitalWrite(PIN_LED, LOW);
    Serial.println("LED apagado mediante comando MQTT");
  }
  else if(msg.equals("ativarCobertura")){
    if(ativacaoCobertura){ 
      ativarCobertura();
      MQTT.publish(TOPICO_PUBLISH_CHUVA_COBERTURA, "ativarCobertura");
    }
    else{ Serial.println("Chuva Intensa. Cobertura já está está ativada!");}
  }
  else if(msg.equals("desativarCobertura")){
    if(ativacaoCobertura){ 
      desativarCobertura();
      MQTT.publish(TOPICO_PUBLISH_CHUVA_COBERTURA, "desativarCobertura");
    }
    else{ Serial.println("Chuva Intensa. Por segurança cobertura não será desativada!");}
  }
  else if(msg.equals("ligarRegador")){
    if(regadorLigado){
      ativarRegador();
      MQTT.publish(TOPICO_PUBLISH_CHUVA_REGADOR, "ligarRegador");
    }
    else{ Serial.println("Está Chuvendo! Regador não será ligado.");}
  }
  else if(msg.equals("desligarRegador")){
    if(regadorLigado){
      desativarRegador();
      MQTT.publish(TOPICO_PUBLISH_CHUVA_REGADOR, "desligarRegador");
    }
    else{ Serial.println("Está Chuvendo! Regador não será utilizado.");}
  }
  else{Serial.println("Não identificou comando MQTT");}
}

/* Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair) em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito. | Parâmetros: nenhum | Retorno: nenhum */
void reconnectMQTT(void) {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE_LED);
      MQTT.subscribe(TOPICO_SUBSCRIBE_CHUVA_COBERTURA);
      MQTT.subscribe(TOPICO_SUBSCRIBE_CHUVA_REGADOR);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Havera nova tentativa de conexao em 2s");
      delay(2000);
    }
  }
}

/* Função: verifica o estado das conexões WiFI e ao broker MQTT. Em caso de desconexão (qualquer uma das duas), a conexão é refeita. | Parâmetros: nenhum | Retorno: nenhum */
void VerificaConexoesWiFIEMQTT(void) {
  if (!MQTT.connected())
    reconnectMQTT();  //se não há conexão com o Broker, a conexão é refeita

  reconnectWiFi();  //se não há conexão com o WiFI, a conexão é refeita
}

/* Função: reconecta-se ao WiFi | Parâmetros: nenhum | Retorno: nenhum */
void reconnectWiFi(void) {
  //se já está conectado à rede WI-FI, nada é feito.
  //Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }
  WiFi.begin(SSID, PASSWORD);  // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("\nIP obtido: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  //Enviar e receber dados em 9600 baud
  delay(1000);
  Serial.println("\nDisciplina IoT: acesso a nuvem via ESP32");
  delay(1000);

  /* INÍCIO CONFIGURAÇÕES PARA TESTES */
  pinMode(PIN_LED, OUTPUT);  // programa LED interno como saida
  testeLED();

  /* GERANDO TEMPERATURA COMO UM NÚMERO ALEATÓRIO. Inicializa o gerador de números aleatórios. 
    Um pino analógico desconectado irá retornar um valor aleatório de tensão em analogRead() */
  randomSeed(analogRead(0));
  /* FIM CONFIGURAÇÕES PARA TESTES */

  /* INÍCIO CONFIGURAÇÕES DE REDES */
    initWiFi(); /* Inicializa a conexao wi-fi */
    initMQTT(); /* Inicializa a conexao ao broker MQTT */
  /* FIM CONFIGURAÇÕES DE REDES */

  /* INÍCIO CONFIGURAÇÕES DO SENSOR LDR */
    pinMode(LDR, INPUT);
  /* FIM CONFIGURAÇÕES DO SENSOR LDR */

  /* INICIO CONFIGURAÇÕES DO SENSOR DHT */
    dht.begin();
  /* FIM CONFIGURAÇÕES DO SENSOR DHT */
  
  /* INICIO CONFIGURAÇÕES DO PAINEL OLED */
    // CONFIGURAÇÃO DISPLAY OLED - ATIVAÇÃO PINAGEM
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inicializa o display OLED
    display.display();                          // Inicializa o display com o buffer atual

    // Mostra o logo da Franzininho como splash screen
    display.clearDisplay();                                                            // Limpa a tela
    display.drawBitmap(0, 0, franzininho_logo_128x64, imageWidth, imageWidth, WHITE);  // Desenha o logo da Franzininho
    display.display();                                                                 // Atualiza o display
    delay(2000);                                                                       // Espera 2 segundos

    display.clearDisplay();  // Limpa a tela
    display.display();       // Atualiza o display
    delay(100);              // Pequeno atraso antes de começar a exibir os valores

    // CONFIGURAÇÃO BOTÕES (TECLADO DE ESCOLHA) - ATIVAÇÃO PINAGEM
    pinMode(BT_A, INPUT_PULLUP);      // Configura o pino do botão A como entrada com resistor de pull-up interno
    pinMode(BT_B, INPUT_PULLUP);      // Configura o pino do botão B como entrada com resistor de pull-up interno
    pinMode(BT_UP, INPUT_PULLUP);     // Configura o pino do botão CIMA como entrada com resistor de pull-up interno
    pinMode(BT_DOWN, INPUT_PULLUP);   // Configura o pino do botão BAIXO como entrada com resistor de pull-up interno
    pinMode(BT_RIGHT, INPUT_PULLUP);  // Configura o pino do botão DIREITA como entrada com resistor de pull-up interno
    pinMode(BT_LEFT, INPUT_PULLUP);   // Configura o pino do botão ESQUERDAcomo entrada com resistor de pull-up interno
  /* FIM CONFIGURAÇÕES DO PAINEL OLED */
  
  /* INICIO CONFIGURAÇÕES DA COBERTURA */
    cobertura.attach(MOTOR_COBERTURA);  // Inicializando o objeto Servo
    cobertura.write(0);
  /* FIM CONFIGURAÇÕES DA COBERTURA */

  /* INICIO CONFIGURAÇÕES DA SIRENE ALERTA */
    noTone(BUZZER);  // Desliga o som temporariamente, se necessário
  /* FIM CONFIGURAÇÕES DA SIRENE ALERTA */
  
  /* INICIO CONFIGURAÇÕES DO REGADOR (LED RGB) */
    pinMode(LED_REGADOR, OUTPUT);
    pinMode(LED_REGADOR_CHUVA, OUTPUT);
  /* FIM CONFIGURAÇÕES DO REGADOR (LED RGB) */

  /* INICIO CONFIGURAÇÕES DA CHUVA */
    pinMode(RAIN_SENSOR, INPUT);
  /* FIM CONFIGURAÇÕES DA CHUVA */

  /* INICIO CONFIGURAÇÕES DA UMIDADE SOLO */
    pinMode(UMIDADE_SOLO, INPUT);
  /* FIM CONFIGURAÇÕES DA UMIDADE SOLO */
}

void loop() {
  // put your main code here, to run repeatedly:

  VerificaConexoesWiFIEMQTT(); /* garante funcionamento das conexões WiFi e ao broker MQTT */

  gerarTempAleat();

  /* INÍCIO SIRENE DE ALERTA */
    noTone(BUZZER);
    switch (alertaSirene) {
      case 0:
        noTone(BUZZER);
      break;
      case 1:
        alertaChuva();
      break;
      default:
        noTone(BUZZER);
      break;
    }
  /* FIM SIRENE DE ALERTA */

  /* INÍCIO SENSOR LDR - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */
    unsigned long millisAtualLDR = millis();

    // Leitura do Piranômetro
    if (millisAtualLDR - t_anterior_piranometro >= intervaloPiranometro) {
      t_anterior_piranometro = millisAtualLDR;

      int leituraLDR = analogRead(LDR); // Lê o valor do LDR
      float irradianciaPct = map(analogRead(LDR), 0, 8191, 0, 100); // Convertendo para porcentagem

      Serial.print("Irradiância Solar (Piranômetro): ");
      Serial.print(irradianciaPct);
      Serial.println(" %");

      piranometro(irradianciaPct);

      piranometroPainel = irradianciaPct; // Painel Piranômetro
    }

    // Leitura do Heliógrafo
    if (millisAtualLDR - t_anterior_heliografo >= intervaloHeliografo) {
      t_anterior_heliografo = millisAtualLDR;

      int leituraLDR = analogRead(LDR); // Lê o valor do LDR
      float nivelLuminosidadePct = map(analogRead(LDR), 0, 8191, 0, 100); // Convertendo para porcentagem
      heliografo(nivelLuminosidadePct, millisAtualLDR);
    }
  /* FIM SENSOR LDR - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */
  Serial.println("------------------");

  /* INICÍO SENSOR LDR - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */
  /* FIM SENSOR LDR - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */
  Serial.println("------------------");

  /* INÍCIO SENSOR DE UMIDADE E TEMPERATURA - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */
    unsigned long millisAtual = millis();

    // Leitura de Temperatura
    if (millisAtual - millisAnteriorTemp >= intervaloTemp) {
      millisAnteriorTemp = millisAtual;

      temperatura = dht.readTemperature();
      if (!isnan(temperatura)) {
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.println(" °C");
      } else { Serial.println("Falha na leitura do sensor DHT de temperatura!"); }

      termometro(temperatura);

      temperaturaPainel = temperatura;  // Painel Termômetro
    }

    // Leitura de Umidade
    if (millisAtual - millisAnteriorHum >= intervaloHum) {
      millisAnteriorHum = millisAtual;

      umidade = dht.readHumidity();
      if (!isnan(umidade)) {
        Serial.print("Umidade: ");
        Serial.print(umidade);
        Serial.println(" %");
      } else { Serial.println("Falha na leitura do sensor DHT de umidade!"); }
      umidadeRelativaAr(umidade);

      umidadePainel = umidade;  // Painel Umidade
    }

    // Análises e Cálculos (Chamados a cada minuto ou após certas leituras)
    if (contadorTemp > 0 && contadorHum > 0) {
      float tempAvg = somaTemp / contadorTemp;
      float humAvg = somaHum / contadorHum;
      
      analiseCalculoMed(tempAvg, humAvg);
      tempAvgPainel = tempAvg;  // Painel Temperatura
      humAvgPainel = humAvg;  // Painel Umidade

      analiseCalculoMax();
      tempMaxPainel = tempMax;  // Painel Temperatura
      humMaxPainel = humMax;  // Painel Umidade

      analiseCalculoMin();
      tempMinPainel = tempMin;  // Painel Temperatura
      humMinPainel = humMin;  // Painel Umidade

      // Índice de Calor
      float indiceCalor = dht.computeHeatIndex(dht.readTemperature(), dht.readHumidity(), false);
      indiceDeCalor(indiceCalor);

      // Ponto de Orvalho
      float pontoDeOrvalho = temperatura - ((100 - umidade) / 5);
      pontoOrvalho(pontoDeOrvalho);
      
    }
  /* FIM SENSOR DE UMIDADE E TEMPERATURA - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */
  Serial.println("------------------");

  /* INÍCIO SENSOR DE CHUVA - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */

    unsigned long millisAtualChuva = millis();  // Obtém o tempo atual

    if (millisAtualChuva - t_anterior_chuva >= intervalo_chuva) {
      // Salva o tempo atual como o último tempo em que a leitura foi feita
      t_anterior_chuva = millisAtualChuva;

      // Faz a leitura do sensor
      leituraAnalogica_Chuva = analogRead(RAIN_SENSOR);  // Valor máximo enviado pelo sensor é 4095 em leitura analógica no ESP32

      // Calcular a porcentagem da chuva (0-100%)
      // porcentagemChuva = ((leituraAnalogica_Chuva / 4095.0)) * 100;
      // porcentagemChuva = (leituraAnalogica_Chuva / 4095.0)+70;
      porcentagemChuva = map(analogRead(RAIN_SENSOR), 8191, 0, 0, 100);

      Serial.print("Leitura Analógica: ");
      Serial.println(leituraAnalogica_Chuva);

      pluviometro(porcentagemChuva);

      porcentagemChuvaPainel = porcentagemChuva;  // Painel Pluviômetro
    }
  /* FIM SENSOR DE CHUVA - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */
  Serial.println("------------------");

  /* INÍCIO SENSOR UMIDADE DO SOLO - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */

    unsigned long milliasAtualSolo = millis();

    // Leitura do sensor de umidade do solo
    if (milliasAtualSolo - t_anterior_umidade_solo >= intervaloUmidadeSolo) {
      t_anterior_umidade_solo = milliasAtualSolo;

      umidadeSolo = analogRead(UMIDADE_SOLO);  // Lê o valor analógico do sensor

      // Converte o valor analógico para a umidade percentual
      percentualUmidade = map(analogRead(UMIDADE_SOLO), 1450, 0, 0, 100);  // Valores são decrescentes. Aproximadamente 1450 (total marcado) é seco e 0 é 100% úmido

      // Exibe o valor da umidade
      Serial.print("Umidade do Solo (valor bruto): ");
      Serial.println(umidadeSolo);
      Serial.print("Umidade do Solo: ");
      Serial.print(percentualUmidade);
      Serial.println("%");
      
      analiseSolo(percentualUmidade);

      percentualUmidadePainel = percentualUmidade;  // Painel Pluviômetro
    }

  /* FIM SENSOR UMIDADE DO SOLO - LEITURA, ANÁLISE E TRATAMENTO DE DADOS DO SENSOR */
  Serial.println("------------------");

  MQTT.loop(); /* keep-alive da comunicação com broker MQTT */

  /* INÍCIO MENU - PAINEL DE EXIBIÇÃO */
    unsigned long milliasAtualPainel = millis();

    Serial.print("Valor da variável opcaoMenu: ");
    Serial.println(opcaoMenu);

    menu(opcaoMenu);  // Exibe o painel correspondente a opcaoMenu
    if (milliasAtualPainel - t_anterior_painel >= intervaloBotao) {
      t_anterior_painel = milliasAtualPainel; // Atualiza o tempo da última leitura do botão
      // Verifica se o botão foi pressionado (estado LOW) e efetua a mudança dos paineis de informação
      if (digitalRead(BT_A) == LOW || digitalRead(BT_UP) == LOW || digitalRead(BT_LEFT) == LOW) { opcaoMenu--; }

      if (digitalRead(BT_B) == LOW || digitalRead(BT_RIGHT) == LOW || digitalRead(BT_DOWN) == LOW) { opcaoMenu++; }

      // Garante que os valores dos paineis permaneçam dentro dos limites
      if (opcaoMenu > 7) { opcaoMenu = 1; } 
      else if (opcaoMenu < 1) { opcaoMenu = 7; }
    }
  /* FIM MENU - PAINEL DE EXIBIÇÃO */
  Serial.println("####################################################################");
  
  delay(2000); /* Refaz o ciclo após 2 segundos */
}
