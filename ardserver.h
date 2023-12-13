#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "SMART_LIGHTS";     // Replace with your network SSID
const char* password = "helloworld"; // Replace with your network password

ESP8266WebServer server(80); // Server on port 80

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password); // Connect to Wi-Fi
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/command", handleCommand); // Handle "/command" URI
  server.begin(); // Start the server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // Handle client requests
}

void handleCommand() {
  String bitArray = server.arg("bits"); // Get the 'bits' argument from the URL query string
  interpretBits(bitArray); // Function to interpret the bit array
  server.send(200, "text/plain", "Received: " + bitArray); // Respond to the client
}

void interpretBits(String bits) {
    Serial.println("Interpreting bits: " + bits);

    // Extract the first 4 characters
    String command = bits.substring(0, 4);

    if (command == "0000") {
        // Add your logic here for command 0000
    } else if (command == "0001") {
        // Logic for command 0001
    } else if (command == "0002") {
        // Logic for command 0002
    } else if (command == "0003") {
        // Logic for command 0003
    } else if (command == "0004") {
        // Logic for command 0004
    }
    // ... and so on for other commands
}