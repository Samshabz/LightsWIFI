#include <Arduino.h>
#include <WiFi.h>
//#include <WebServer.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "SMARTLIGHTS";     // Replace with your network SSID
const char* password = "helloworld"; // Replace with your network password
AsyncWebServer server(80); 
// put function declarations here:
void handleCommand(AsyncWebServerRequest *request);
void interpretBits(const String& bits);




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
  String htmlContent = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>WIFIlights</title>
      <link rel="stylesheet" type="text/css" href="/style.css">
    </head>
    <body>
      <h1>WIFIlights</h1>

      <!-- Debug section for packet -->
      <div id="debugSection" style="position: absolute; top: 10px; left: 10px; color: black;">
          <p>Debug Info:</p>
          <p id="debugValue">Hex: None</p>
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
              <label class="texthuelevel">Warmth: </label>
              <input type="range" id="warmthSlider" min="0" max="100" value="50">
              <input type="text" id="warmthValue" value="50">
          </div>
      </div>

      <!-- PATTERN choice buttons -->
      <div class="pressables"> 
          <button class="button" id="solidcolor">SOLID COLOR</button>
          <button class="button HUE" id="HUE">HUE STROLLER</button>
          <button class="button " id="patt3">SUPRISE 1</button>
          <button class="button " id="patt4">SUPRISE 2</button>
          <button class="button " id="patt5">SUPRISE 3</button>
      </div>
      <script src="/script.js"></script>
    </body>
    </html>
  )rawliteral";
  request->send(200, "text/html", htmlContent);
});

  // CSS handler
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    String cssContent = R"rawliteral(
      


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
@media screen and (max-width: 600px) {
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

    )rawliteral";
    request->send(200, "text/css", cssContent);
  });

  // JavaScript handler
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    String jsContent = R"rawliteral(
      
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
    sendColorBits('0000' + red.toString(16).padStart(2, '0') + green.toString(16).padStart(2, '0') + blue.toString(16).padStart(2, '0'));
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
    sendColorBits('0001' + brightness.padStart(2, '0') + warmth.padStart(2, '0'));
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
    document.getElementById('debugValue').innerText = 'Hex: ' + bits;
    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log('Sent bits: ' + bits);
            console.log(this.responseText);
         
        }
    };
/*    xhttp.open("GET", "http://[ESP8266_IP_ADDRESS]/command?bits=" + bits, true);
    xhttp.send();*/
    xhttp.open("GET", "http://" + window.location.hostname + "/command?bits=" + bits, true);
    xhttp.send();
  
}


    )rawliteral";
    request->send(200, "application/javascript", jsContent);
  });

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
     Serial.println("herewego");
    // Add your logic here for command 0000
  } else if (command == "0001") {
     Serial.println("herewego");
    // Logic for command 0001
  } else if (command == "0002") {
     Serial.println("herewego");
    // Logic for command 0002
  } else if (command == "0003") {
    // Logic for command 0003
  } else if (command == "0004") {
    // Logic for command 0004
  }
  // ... and so on for other commands
}