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

// JSON para os medidasSensores dos sensores
StaticJsonBuffer<200> jsonBuffer;
JsonObject& medidasSensores = jsonBuffer.createObject();

// Buffer para HTTP
String httpReq;

// Variáveis para medidas
int temp;
int hum;
int gas;

void setup() {
    // Inicialização dos sensores e display
    dht.begin();
    lcd.begin(20, 4);
    // Imprime texto base no LCD
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

    // start the Ethernet connection and the server:
    Ethernet.begin(mac, ip);

    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        while (true) {
            delay(1); // do nothing, no point running without Ethernet hardware
        }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("Ethernet cable is not connected.");
    }

    // start the server
    server.begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());
}


void loop() {

    leSensores();
    atualizaJsonSensores();
    atualizaLCD();

    // Verifica por clientes
    client = server.available(); 

    // Limpa buffer de requisção
    httpReq = "";

    if (client) {
        bool currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                // Lê requisição
                char c = client.read();
                httpReq.concat(c);

                if (c == '\n' && currentLineIsBlank) {
                    if(httpContem("GET /sensors")) { enviaJsonSensores(client); } 
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
        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
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
void enviaJsonSensores(EthernetClient client){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Connection: close");
    client.println("Refresh: 5");
    client.println();
    medidasSensores.prettyPrintTo(client); 
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
 * httpContem - Procura por uma substring dentro de um vetor de caracteres
 * Retorna 1 caso encontre e 0 caso contrário
 */ 
const int httpContem(String search){ 
    if( httpReq.indexOf(search) == -1)
        return 0;
    else
        return 1;
}