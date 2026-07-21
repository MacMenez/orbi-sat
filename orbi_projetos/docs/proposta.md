Engenheiro de Computação. Projeto completo. Desenvolvimento de uma módulo para aplicações aeronáuticas que se assemelham a drones, sonobóias, cubesat, satélites experimentais, foguetes experimentais, etc. Preciso de uma organização de tudo que já planejei e um início/base de protótipo para construir este projeto e aplicá-lo em testes de laboratório/campo. O que for necessário adicionar, adicione. Me dê o máximo de orientações e auxílio para a construção do mesmo. Envolverá hardware, software, metodologias, infraestrutura web/redes e afins.

Por não ter muita experiencia, além de atividades acadêmicas com orientações de um professor/tutor, precisarei deste apoio e, quaisquer que sejam, interveções/correções. Inclusive as decisões de ambiente de desenvolvimento, componentes, estratégias e afins, devem ser revidas por você e me orientar sobre qual usar, o por quê de usar e o como usar. Algumas coisas básicas não são necessárias, porém conforme for, precisarei de ajuda extra.

Descritivo:
* Ambiente de Desenvolvimento
    * Windows 11/Ubuntu(versão atualizada): máquina com dual boot.
    * Android(versão 12): no momento consegue ir até esta versão, celular de teste da marca Motorola
* Metodologia Ágil
	* SCRUM: Código por etapas, teste de cada componente, adição ao código-fonte, teste geral de funcionamento, próximo. (Creio ser assim que funciona);
* Modelagem:
	* Ultimaker Cura: Modelagem + Impressão3D;
	* Kicad: Modelagem + Planejamento eletrônico + impressão da PCB;
	* MYSQL ou equivalente: Modelagem do Banco de Dados;
	* ArduinoIDE: Desenvolvimento do Firmware;
	* VisualStudio Code: Desenvolvimento da programação;
	* Mosquito MQTT ou equivalente: Publisher/Subscriber para funcionar como "servidor" online;
    * IotMQTT (Aplicativo Android): Visualização por aplicativo de celular;
* Hardware:
	* ESP32-S2-WROOM;
	* MPU5060;
	* BMP280;
	* Módulo Micro SD Card;
	* Display OLED 128x64 0.96" I2C;
	* Display IPS OLED de 1.3 Polegadas 240*240 RGB TFT;
	* LED;
	* LED RGB;
	* Push bottons de 4 terminais (minimo 6 push-button), cima, baixo, esquerda, direita, botão A e botão B;
	* LDR;
	* Buzzer;
	* DHT111;
	* Módulo GPS NEO-6M com Antena;
	* Câmera Módulo VGA OV7670 640x480;
	* MicroServer SG90 (ou equivalente superior);
	* Pinos de I/O, GND, VCC (3V/5V), SDA, SCL, etc;
* Software
	* Linguaem C para Arduino;
	* SQL;
    * Flutter;
* Tarefas:
	* Leitura de todos os sensores;
    * Registro em memória (cartão SD ou equivalente);
    * Envio dos dados da memória para servior (MQTT ou equivalente);
    * Plotagem de gráfico;
    * Análise de desempenho;
    * Geração/criação de relatório;
    * Acesso remoto (quando possivel), principalmente à câmera;
    * Geolocalização (GPS, LORA ou equivalente);
    * Módulo de recuperação do projeto (para casos de perda em locais de mata ou equivalentes locais que possa cair e não ser visível a localização)
    * Envios de dados e plotagem em página web (página do projeto), servirá como painel web de verificação em tempo real
    * Página web (landing page ou semelhante) para divulgação do projeto com a integração da página de dados em tempo real/ao vivo
    * Interface de configuração e calibragem dos sensores, tanto local (por executáveis ou equivalente para computadores) quanto web (plataforma online), tendo a versão web integração com login/senha para controle de acesso
    * Exibição de painel de forma física (OLED) atraés dos botões
    * Configurações de forma física com o auxílio do painel (OLED) através dos botões

Fim do  que fiz/pensei até o momento.

Faltou algo? Algo está errado? Conforme for, enviarei coisas que já fiz para complementação ou correção. Dadas as informações acima, faças as devidas contribuições e, por partes, vamos implementado e dando continuidade no projeto.


---

Seguirei 
1 - Definir Hardware final + Git
2 - Firmware base (sensores)
3 - Back-end/MQTT (Mosquitto)
4 - Estrutua Kicad/PCB