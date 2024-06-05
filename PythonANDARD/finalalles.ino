#include <FastLED.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <AsyncWebServer_ESP32_SC_W5500.h>

// definitions for 3rd communication pattern - FFT music analysis
#define PIN            2  // Define the pin for LED data
#define NUMPIXELS      360 // Number of LEDs in the strip
#define BAUD_RATE 115200
#define LED_PIN            2  // Define the pin for LED data
#define LED_COUNT       360 // Number of LEDs in the strip
#define NUM_LEDS    360   // Number of LEDs in your strip
#define NUM_BANDS   9     // Number of frequency bands
#define BEAT_LEDS   40    // Number of LEDs at each end to flash for a beat


const int numberOfBands = 9;
const int ledsPerBand = LED_COUNT / numberOfBands;
const int flashDuration = 300;
bool flashRed = false;

const char* ssid = "SMARTlights";     // Replace with your network SSID
const char* password = "hello123"; // Replace with your network password
AsyncWebServer server(80);

// put function declarations here:
void handleCommand(AsyncWebServerRequest *request);
void adjustColors(int &red, int &green, int &blue);
void interpretBits(const String& bits);
void triggerBeatEffect();
void resetBeatLEDs();
uint32_t volumeToColor(int band);
void setStripColor(byte red, byte green, byte blue);
void updateLEDs(uint8_t volumes[]);
void readSerialAndProcess();

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



//variable declarations for LEDs
int commnum = -1;
int PERMON = 0;
unsigned long previousMillis = 0;
uint16_t hue = 0;
String truebits;
uint16_t permreset = 0;

bool beatActive = false;
unsigned long beatEffectDuration = 200; // Duration of the beat effect in milliseconds
unsigned long lastBeatTime = 0;






void setup() {
  Serial.begin(115200); // Start Serial communication
  // Serial.begin(115200);
  WiFi.begin(ssid, password); // Connect to Wi-Fi

  // Wait for connection
  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Could not create wi-fi network. Check SSID and password for validity.");
  }
  IPAddress myIP = WiFi.softAPIP();
  //  WiFi.begin(ssid, password); // Connect to home Wi-Fi
  //Serial.print("Connecting to Wi-Fi");
  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
  //  Serial.print(".");
  //}
  //Serial.println("");
  //Serial.print("Connected to Wi-Fi. IP address: ");
  //Serial.println(WiFi.localIP());
  //



  Serial.print("AP IP address: ");
  Serial.println(myIP);

  //  Serial.println("");
  //  Serial.print("Connected to ");
  //  Serial.println(ssid);
  server.on("/command", handleCommand); // Handle "/command" URI
 // server.begin(); // Start the server
  Serial.println("HTTP server started");
  // Your initial setup code here
  // (e.g., initializing serial communication, setting up WiFi, etc.)












  // server setup
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    String htmlContent = R"=====(
      <!DOCTYPE html>
      <html>
      <head>
        <title>Health Monitoring System</title>
        <style>

body {
    --backcolor: rgb(255, 166, 107);
    --buttoncolor: rgb(240, 124, 78);
    user-select: none;
    font-family: 'Arial', sans-serif;
    margin: 0;
    padding: 20px;
    text-align: center;
    background: var(--backcolor); /* Example game-like background color */
    background-size: cover;
    background-repeat: no-repeat;
    background-position: center;
    color: #fff;
}



.button {
    
    
    padding: 0.8em 1.7em;
    margin: 10px;
    background-color: transparent;
    border-radius: .3em;
    position: relative;
    overflow: hidden;
    cursor: pointer;
    transition: .5s;
    font-weight: 400;
    font-size: 17px;
    border: 1px solid black; /*var(--color);*/
    font-family: inherit;
    text-transform: uppercase;
    color: black; /* var(--color);*/
    z-index: 1;
}

.button::before, .button::after {
    content: '';
    display: block;
    width: 50px;
    height: 50px;
    transform: translate(-50%, -50%);
    position: absolute;
    border-radius: 50%;
    z-index: -1;
    background-color: var(--buttoncolor);
    transition: 1s ease;
}

.pressables {
    display: flex;
    justify-content: center;
    flex-wrap: wrap;
}

.slider-container {
    display: flex;
    align-items: center;
    justify-content: flex-start;
    margin-bottom: 10px;
}

.slider-container label {
    width: 100px; /* Adjust as needed */
    margin-right: 10px;
}

.slider-container input[type="range"] {
    flex-grow: 1;
    margin-right: 10px;
}

.slider-container input[type="text"] {
    width: 60px; /* Adjust as needed */
}


.slider-container {
    display: flex;
    align-items: center;
    justify-content: flex-start;
    margin-bottom: 1.4%;
    margin-left: 15%;
    width: 70%; /* Adjust the width as per your requirement */
}

.slider-container label {
    margin-right: 10px;
    flex-shrink: 0;
    width: 100px; /* Adjust label width as needed */
}

.slider-container input[type="range"] {
    width: 200px; /* Set a fixed width for sliders */
    margin-right: 10px;
}

.slider-container input[type="text"] {
    width: 60px;
    flex-shrink: 0;
}







.button::before {
    top: -1em;
    left: -1em;
}

.button::after {
    left: calc(100% + 1em);
    top: calc(100% + 1em);
}

.button:hover::before, .button:hover::after {
    height: 410px;
    width: 410px;
}

.button:hover {
    color: rgb(10, 25, 30);
}


.button.active {
    background-color: var(--buttoncolor); /* Highlight color for active button */
    color: white; /* Change text color for visibility */
}


/* Mobile responsiveness */
@media screen and (max-width: 1000px) {
    .pressables {
        flex-direction: column;
    }

    .button {
        width: 80%;
        margin: 10px auto;
        transition: 0.2s ease;
    }
    .button::before, .button::after {
        transition: 0.4s ease;
        height: 0px; /* Increased height */
        width: 0px; /* Increased width */
    }
    .button:hover::before, .button:hover::after {
        height: 500px; /* Further increase for overlap */
        width: 500px; /* Further increase for overlap */
    }

    .slider-container {
        justify-content: flex-start;
        margin-bottom: 4.4%;
        margin-left: 5%;
        width: 90%; /* Adjust the width as per your requirement */
    }
}



/*for text labels of slider */
.sliders label {
    display: block;
    margin: 10px 0;
}

.sliders input[type=range] {
    width: 60%;
    vertical-align: middle;
}

.sliders input[type=text] {
    width: 60px;
    margin-left: 10px;
    padding: 5px;
    border: 1px solid #ddd; /* Light gray border */
    border-radius: 5px; /* Rounded corners */
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2); /* Subtle shadow for depth */
    font-family: 'Arial', sans-serif; /* Consistent font with the rest of the page */
    text-align: center; /* Center the text inside the input box */
    background-color: #f9f9f9; /* Light background color */
    transition: all 0.3s ease; /* Smooth transition for focus effect */
}

.sliders input[type=text]:focus {
    outline: none; /* Removes the default focus outline */
    border-color: #009688; /* Highlight color when focused */
    box-shadow: 0 0 8px rgba(0, 150, 136, 0.6); /* More prominent shadow when focused */
}


/*for ACTUAL slider */
.sliders input[type=range] {
    -webkit-appearance: none; /* Override default appearance */
    width: 60%;
    height: 10px;
    border-radius: 5px;
    background: #ddd;
    outline: none;
    opacity: 0.7;
    -webkit-transition: .2s;
    transition: opacity .2s;
}

.sliders input[type=range]::-webkit-slider-thumb {
    -webkit-appearance: none; /* Override default appearance */
    appearance: none;
    width: 25px;
    height: 25px;
    border-radius: 50%;
    background: #fdfdfd;
    cursor: pointer;
}

.sliders input[type=range]::-moz-range-thumb {
    width: 25px;
    height: 25px;
    border-radius: 50%;
    background: #009688;
    cursor: pointer;
}

        
        </style>
      </head>

    <body>
      <h1>WIFIlights</h1>

      <!-- Debug section for packet -->
      <div id="debugSection" style="position: absolute; top: 10px; left: 10px; color: black;">
          <p></p>
          <p id="debugValue"></p>
      </div>

      <!-- RGB sliders -->
      <div id="colorPicker" class="sliders" style="display:none;">
          <label>R: <input type="range" id="redSlider" min="0" max="255" value="128"> <input type="text" id="redValue" value="128"></label><br>
          <label>G: <input type="range" id="greenSlider" min="0" max="255" value="128"> <input type="text" id="greenValue" value="128"></label><br>
          <label>B: <input type="range" id="blueSlider" min="0" max="255" value="128"> <input type="text" id="blueValue" value="128"></label><br>
      </div>

      <!-- HUE brightness and warmth -->
      <div id="huePicker" class="sliders" style="display:none;">
          <div class="slider-container">
              <label class="texthuelevel">Brightness: </label>
              <input type="range" id="brightnessSlider" min="0" max="100" value="50">
              <input type="text" id="brightnessValue" value="50">
          </div>
          <div class="slider-container">
              <label class="texthuelevel">Speed: </label>
              <input type="range" id="warmthSlider" min="0" max="100" value="50">
              <input type="text" id="warmthValue" value="50">
          </div>
      </div>

      <!-- PATTERN choice buttons -->
      <div class="pressables"> 
          <button class="button" id="solidcolor">SOLID COLOR</button>
          <button class="button HUE" id="HUE">HUE STROLLER</button>
          <button class="button " id="patt3">PERM-OFF</button>
          <button class="button " id="patt4">PERM-ON</button>
          <button class="button " id="patt5">SUPRISE 3</button>
      </div>
      <script src="/script.js"></script>
    </body>

        <script>

//contents
// default listeners
// rgb slider
// hue slider
// toggle slider func
// button active highlight
// transmit bits func



//default listeners 
document.getElementById('patt3').addEventListener('click', function() {
    toggleSliders('');
    sendColorBits('0002');
});

document.getElementById('patt4').addEventListener('click', function() {
    toggleSliders('');
    sendColorBits('0003');
});

document.getElementById('patt5').addEventListener('click', function() {
    toggleSliders('');
    sendColorBits('0004');
    
});

//turn on rgB sliders and update color
document.getElementById('solidcolor').addEventListener('click', function() {
    toggleSliders('colorPicker');
    updateColor();
});


// RGB slider active 

['red', 'green', 'blue'].forEach(color => {
    document.getElementById(color + 'Slider').addEventListener('input', function() {
        document.getElementById(color + 'Value').value = this.value;
        updateColor();
    });

    document.getElementById(color + 'Value').addEventListener('input', function() {
        document.getElementById(color + 'Slider').value = this.value;
        updateColor();
    });
});

function updateColor() {
    let red = document.getElementById('redValue').value;
    let green = document.getElementById('greenValue').value;
    let blue = document.getElementById('blueValue').value;
    sendColorBits('0000' + red.toString(16).padStart(3, '0') + green.toString(16).padStart(3, '0') + blue.toString(16).padStart(3, '0'));
}




//hue updater
document.getElementById('HUE').addEventListener('click', function() {
    toggleSliders('huePicker');
    updateHue();
    
   
});

['brightness', 'warmth'].forEach(property => {
    document.getElementById(property + 'Slider').addEventListener('input', function() {
        document.getElementById(property + 'Value').value = this.value;
        updateHue();
    });

    document.getElementById(property + 'Value').addEventListener('input', function() {
        document.getElementById(property + 'Slider').value = this.value;
        updateHue();
    });
});

function updateHue() {
    let brightness = document.getElementById('brightnessValue').value;
    let warmth = document.getElementById('warmthValue').value;
    sendColorBits('0001' + brightness.padStart(3, '0') + warmth.padStart(3, '0'));
}


function toggleSliders(activeSliderId) {
    // Hide all sliders with the class 'sliders'
    document.querySelectorAll('.sliders').forEach(slider => {
        slider.style.display = 'none';
    });

    // Show only the active slider, if specified
    if (activeSliderId && document.getElementById(activeSliderId)) {
        document.getElementById(activeSliderId).style.display = 'block';
    }
}



// make button highlight active
let buttons = document.querySelectorAll('.button');
buttons.forEach(btn => {
    btn.addEventListener('click', function() {
        buttons.forEach(b => b.classList.remove('active')); // Remove active from all buttons
        this.classList.add('active'); // Add active to clicked button
    });
});


//update and send packet



function sendColorBits(bits) {
   // document.getElementById('debugValue').innerText = 'Hex: ' + bits;
    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log('Sent bits: ' + bits);
            console.log(this.responseText);
         
        }
    };

    xhttp.open("GET", "http://" + window.location.hostname + "/command?bits=" + bits, true);
    xhttp.send();
  
}



        </script>
      </body>
      </html>
    )=====";
    request->send(200, "text/html", htmlContent);
  });

  server.begin();
  Serial.println("Server started successfully.");




}






















































































void setStripColor(byte red, byte green, byte blue) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

void updateLEDs(uint8_t volumes[]) {
    int ledsPerBand = (NUM_LEDS - 2 * BEAT_LEDS) / NUM_BANDS;
    for (int band = 0; band < NUM_BANDS; band++) {
        int numLitLEDs = map(volumes[band], 0, 10, 0, ledsPerBand);
        for (int i = band * ledsPerBand + BEAT_LEDS; i < band * ledsPerBand + numLitLEDs + BEAT_LEDS; i++) {
            strip.setPixelColor(i, volumeToColor(band));
        }
        for (int i = band * ledsPerBand + numLitLEDs + BEAT_LEDS; i < (band + 1) * ledsPerBand + BEAT_LEDS; i++) {
            strip.setPixelColor(i, strip.Color(0, 0, 0));
        }
    }
    strip.show();
}

uint32_t volumeToColor(int band) {
    uint32_t colors[NUM_BANDS] = {
        strip.Color(255, 183, 38), // oran
        strip.Color(237, 222, 38), // yel
        strip.Color(110, 255, 38), // gre
        strip.Color(38, 255, 201), // turq
        strip.Color(38, 147, 200), // Light blue
        strip.Color(0, 0, 255), // Full blue
        strip.Color(165, 38, 255), // Purple
        strip.Color(179, 27, 128), // Magenta
        strip.Color(115, 17, 60)  // Purple
        /*
     (30, 200), 
    (60, 240),
    (250, 480),
    (500, 770),
    (800, 1060),
    (1100, 1550),
    (1600, 2420),
    (2500, 3900),
    (4000, 8000),
         */
      
    };
    return colors[band % NUM_BANDS];
}

void triggerBeatEffect() {
    for (int i = 0; i < BEAT_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0)); // First 30 LEDs
        strip.setPixelColor(NUM_LEDS - 1 - i, strip.Color(255, 0, 0)); // Last 30 LEDs
    }
    strip.show();
    beatActive = true;
    lastBeatTime = millis();
}

void resetBeatLEDs() {
    for (int i = 0; i < BEAT_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0)); // Reset first 30 LEDs to black
        strip.setPixelColor(NUM_LEDS - 1 - i, strip.Color(0, 0, 0)); // Reset last 30 LEDs to black
    }
    strip.show();
}



void readSerialAndProcess() {
  if (Serial.available() >= NUM_BANDS + 1) { // Check if enough data is available
    uint8_t volumes[NUM_BANDS];
    
    for (int i = 0; i < NUM_BANDS; i++) {
      
      volumes[i] = Serial.read(); // Read volume level for each band
    }
    
    char signal = Serial.peek(); // Check for beat signal without removing it from buffer
    if (signal == 'B') {
      Serial.read(); // Consume the 'B'
      triggerBeatEffect();
    }
    updateLEDs(volumes);
  }

  if (beatActive && (millis() - lastBeatTime > beatEffectDuration)) {
    beatActive = false;
    resetBeatLEDs();
  }
}












void loop() {

  //  if (commnum == -1) {
  //    //run default case
  //
  //
  ////    for (int i = 0; i < strip.numPixels(); i++) {
  ////      strip.setPixelColor(i, strip.Color(0, 100, 100)); // Default blue - can implement default hue wave if desired
  ////
  ////
  ////    } strip.show();
  //
  //  }

  if (commnum == 0) {
    int redc = truebits.substring(4, 7).toInt();
    int greenc = truebits.substring(7, 10).toInt();
    int bluec = truebits.substring(10, 13).toInt();

    adjustColors(redc, greenc, bluec);
    redc *= 1.3;



    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(redc, greenc, bluec)); // Red color


    } strip.show();
  }

  else if (permreset == 1) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0)); // Set the color to the first LED
      permreset = 0;

    }
    strip.show();

  }
  else if (commnum == 1 || PERMON == 1) { //runs permon if any wifi issue arrises

    int brightness = truebits.substring(4, 7).toInt() * 1.7;
    int speedv = truebits.substring(7, 10).toInt();
    if (PERMON == 1) {
      brightness = 200;
      speedv = 60;
    }
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > 6) {
      // Serial.println("huenew: " + hue);
      hue += 45; //65k max
      hue += (int)((float)speedv / 10);
      if (hue > 65535) {
        hue = 0;
      }
      previousMillis = currentMillis;
    }
    uint32_t color = strip.ColorHSV(hue, 255, brightness);

    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color); // Set the color to the first LED


    }
    strip.show();

  }













  else if (commnum == 3 || commnum == -1) {
    readSerialAndProcess();

  }
}
























void adjustColors(int &red, int &green, int &blue) {
  // If all colors are above 70, scale them down so the highest is at 70
  red *= 6;
  if (red > 70 && green > 70 && blue > 70) {
    int maxColor = max(red, max(green, blue));
    float scale = 70.0 / maxColor;
    red *= scale;
    green *= scale;
    blue *= scale;
  }

  // If 2 colors are above 150, scale them so the highest is 150
  int countAbove150 = (red > 150) + (green > 150) + (blue > 150);
  if (countAbove150 >= 2) {
    int maxColor = max(red, max(green, blue));
    float scale = 150.0 / maxColor;
    red *= scale;
    green *= scale;
    blue *= scale;
  }

  // Ensure the sum of all colors never exceeds 300
  int total = red + green + blue;
  if (total > 300) {
    float scale = 300.0 / total;
    red *= scale;
    green *= scale;
    blue *= scale;
  }
}







void handleCommand(AsyncWebServerRequest * request) {
  if (request->hasParam("bits")) {
    String bits = request->getParam("bits")->value();
    interpretBits(bits);
    request->send(200, "text/plain", "Received: " + bits);
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}



void interpretBits(const String & bits) {


  truebits = bits;
  Serial.println("Interpreting bits: " + bits);


  // Extract the first 4 characters
  String command = bits.substring(0, 4);


  if (command == "0000") {
    commnum = 0;


    //delay(180);
  } else if (command == "0001") {
    commnum = 1;
  } else if (command == "0002") {
    PERMON = 0;
    permreset = 1;
    commnum = 2;
    // Logic for command 0002
  } else if (command == "0003") {
    // PERMON=1;
    commnum = 3;
    // Logic for command 0003
  } else if (command == "0004") {
    // Logic for command 0004
  }
  // ... and so on for other commands
}
