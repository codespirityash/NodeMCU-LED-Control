#include <ESP8266WiFi.h>

const char* ssid = "Lakhanis jio";       // Change to your Wi-Fi name
const char* password = "idontknow"; // Change to your Wi-Fi password

WiFiServer server(80);
int ledPin = 2;  // LED on GPIO2 (D4)

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); // LED OFF initially

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWi-Fi Connected!");
    Serial.print("NodeMCU IP Address: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {
    WiFiClient client = server.available();
    if (!client) {
        return;
    }

    String request = client.readStringUntil('\r');
    Serial.println("Request: " + request);
    client.flush();

    if (request.indexOf("/LED=ON") != -1) {
        digitalWrite(ledPin, LOW);  // Turn LED ON
    }
    if (request.indexOf("/LED=OFF") != -1) {
        digitalWrite(ledPin, HIGH); // Turn LED OFF
    }

    bool ledState = digitalRead(ledPin) == LOW;  // true if ON, false if OFF

    // Send JSON response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println();
    client.print("{\"led\":");
    client.print(ledState ? "true" : "false");
    client.println("}");

    client.stop();
}
