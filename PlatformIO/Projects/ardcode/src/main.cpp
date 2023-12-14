#include <Arduino.h>
#include <WiFi.h>
//#include <WebServer.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "SMART_LIGHTS";     // Replace with your network SSID
const char* password = "helloworld"; // Replace with your network password
AsyncWebServer server(80); 
// put function declarations here:
void handleCommand(AsyncWebServerRequest *request);
void interpretBits(const String& bits);

void setup() {
  // put your setup code here, to run once:

}


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
    // Your initial setup code here
    // (e.g., initializing serial communication, setting up WiFi, etc.)

    // Server handler setup
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String htmlContent = R"=====(
            <!DOCTYPE html>
            <html>
            <head>
                <style>
                    /* CSS content here */
                </style>
            </head>
            <body>
                <!-- HTML content here -->
                <script>
                    // JavaScript content here
                </script>
            </body>
            </html>
        )=====";
        request->send(200, "text/html", htmlContent);
    });

    // Start the server
      server.on("/command", HTTP_GET, handleCommand);

  // Start the server
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
 
    // Your main program loop code here
    // This is typically where you handle ongoing tasks
}
void handleCommand(AsyncWebServerRequest *request) {
  if (request->hasParam("bits")) {
    String bits = request->getParam("bits")->value();
    interpretBits(bits);
    request->send(200, "text/plain", "Received: " + bits);
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}

void interpretBits(const String& bits) {
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