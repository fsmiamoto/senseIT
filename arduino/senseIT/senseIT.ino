/*
senseIT - Projeto de monitoramento de temperatura, umidade e presença de gás
com comunicação via JSON.

Baseado no exemplo WebServer da biblioteca Ethernet

Autor: Francisco Miamoto
GitHub: /fsmiamoto
 */

#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>

// Definições de rede:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 100, 200);

// DHT 11
const int dhtPin = 30; // Pino do sensor DHT
DHT dht(dhtPin, DHT11); // Objeto do sensor de temperatura

// MQ-2
const int gasPin = 10; // Pino analógico

// LCD
LiquidCrystal lcd(34,36,38,46,42,44); 

// Inicia servidor na porta 80 (Padrão HTTP)
EthernetServer server(80);
EthernetClient client;

// JSON para os medições dos sensores
StaticJsonBuffer<200> jsonBufferRes;
StaticJsonBuffer<200> jsonBufferReq;
JsonObject& medidasSensores = jsonBufferRes.createObject();


// Buffer para HTTP
String httpReq;

// Variáveis para medidas
int temp;
int hum;
int gas;

void setup() {
    // Inicialização dos sensores
    dht.begin();

    // Imprime texto base no LCD
    lcd.begin(20, 4);
    lcd.setCursor(0, 0);
    lcd.print("Temperatura: ");
    lcd.setCursor(0, 1);
    lcd.print("    Umidade: ");
    lcd.setCursor(0, 2);
    lcd.print("        Gas: ");
    lcd.setCursor(0, 3);
    lcd.print("IP:  192.168.100.200");
    
    // Inicia porta serial
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Ethernet WebServer Example");

    // Inicia conexão ethernet
    Ethernet.begin(mac, ip);

    if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("Cabo ethernet desconectado!");
    }

    // Inicia servidor
    server.begin();
    Serial.print("Servidor disponível em: ");
    Serial.println(Ethernet.localIP());
}


void loop() {
    leSensores();
    atualizaJsonSensores();
    atualizaLCD();
    atendeRequisicao();
}

/*
 * atendeRequisicao - Função que gera respostas à requisições HTTP
*/
void atendeRequisicao(){
    // Verifica por clientes
    client = server.available(); 
    // Limpa buffer de requisção
    httpReq = "";

    if (client) {
        bool currentLineIsBlank = true;
        httpReq = "";
        while (client.connected()) {
            if (client.available()) {
                // Lê requisição
                char c = client.read();
                Serial.print(c);
                httpReq.concat(c);

                // TODO: Revisar detecção do fim da requisição
                if (c == '\n' && currentLineIsBlank) {
                    enviaResposta();
                    break;
                }

                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
    }
    delay(1);
    client.stop();
}

/** 
 * enviaResposta - Envia resposta apropriada à página Web
*/
void enviaResposta(){
    client.println("HTTP/1.1 200 OK");
    client.println("Access-Control-Allow-Origin: *");
    if(httpContem("GET /sensors")){
        enviaJsonSensores();
    }
    else if(httpContem("POST /ip")){
        recebeIp();
    }
    else if(httpContem("OPTIONS /ip")){
        enviaOpcoes();
    }
}

/*
 * leSensores - Obtém medições dos sensores
*/
void leSensores(){
    temp = (int) dht.readTemperature();
    hum = (int) dht.readHumidity();
    gas = analogRead(gasPin);
}

/*
 * atualizaJsonSensores - Insere dados dos sensores no json
*/
void atualizaJsonSensores(){
    medidasSensores["temp"] = temp;
    medidasSensores["hum"] = hum;
    medidasSensores["gas"] = gas;
}

/* 
 * enviaJsonSensores - Envia resposta HTTP com JSON dos sensores
*/
void enviaJsonSensores(){
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println("Refresh: 5");
    client.println();
    medidasSensores.prettyPrintTo(client); 
}

void recebeIp(){
    char c;
    do{
        c = client.read();
        if(c != -1)
            httpReq.concat(c);
    }while(c != -1);

    Serial.println(pegaJsonHttpReq());
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("IP Atualizado");
    client.println();
}

void enviaOpcoes(){
    client.println("Content-Type: text/plain");
    client.println("Access-Control-Allow-Methods: POST, GET, OPTIONS");
    client.println("Access-Control-Allow-Headers: Content-Type");
    client.println("Connection: Keep-Alive");
    client.println();
}

/*
 * pegaJsonHttpReq - Obtém texto Json da requisição HTTP
*/
String pegaJsonHttpReq(){
    const int comeco = httpReq.indexOf('{');
    const int fim = httpReq.indexOf('}');
    return httpReq.substring(comeco,fim);
}

/* 
 * atualizaLCD - Imprime medidasSensores dos sensores no LCD
*/
void atualizaLCD(){
    // Imprime temperatura
    lcd.setCursor(13, 0);
    lcd.print(temp);  
    // Imprime simbolo de grau
    lcd.write(0xDF);
    lcd.print("C");

    // Imprime umidade
    lcd.setCursor(13, 1);
    lcd.print(hum);
    lcd.print("%");

    // Imprime gas
    lcd.setCursor(13, 2);
    lcd.print("25");
}

/*
 * httpContem - Procura por uma substring dentro da requisição
 * Caso encontre, retorna 1 e caso contrário, retorna 0.
 */ 
const int httpContem(String search){ 
    if( httpReq.indexOf(search) == -1)
        return 0;
    else
        return 1;
}
