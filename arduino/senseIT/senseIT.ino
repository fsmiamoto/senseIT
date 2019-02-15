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
// JSON para os dados dos sensores
StaticJsonBuffer<200> jsonBuffer;
JsonObject& dados = jsonBuffer.createObject();

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
    // Obtém leitura dos sensores 
    const int temp = (int) dht.readTemperature();
    const int hum = (int) dht.readHumidity();
    const int gas = analogRead(gasPin);
    // Atualiza JSON
    dados["temp"] = temp;
    dados["hum"] = hum;
    dados["gas"] = gas;
    // Imprime temperatura
    lcd.setCursor(13, 0);
    lcd.print(temp); 
    lcd.write(0xDF); // Imprime simbolo de grau
    lcd.print("C");
    // Imprime umidade
    lcd.setCursor(13, 1);
    lcd.print(hum);
    lcd.print("%");
    // Imprime gas
    lcd.setCursor(13, 2);
    //TODO: Não atualizar demais
    lcd.print("25");

    // listen for incoming clients
    EthernetClient client = server.available();
    if (client) {
        Serial.println("new client");
        // an http request ends with a blank line
        bool currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: application/json");
                    client.println("Access-Control-Allow-Origin: *");
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                    client.println();
                    dados.prettyPrintTo(client); // Envia JSON para cliente
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
