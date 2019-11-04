 /*
 * Basic Web Server on NodeMCU
 * 
 * 
 * 4 November 2019
 * By Brenden Roland
 * 
 */

// ---- DEPENDENCIES ----
#include <ESP8266WiFi.h>

// ---- MODIFIABLE CONSTANTS ----
const char * ssid = "LU-IoT2"; // put your ssid (network name) here
const char * password = "CSC49008"; // put your network password here
#define LED_PIN 16 // pin your led is on
#define LED_SENSE 5 // pin for led sensing

// ---- UNMODIFIABLE CONSTANTS ----
WiFiServer myServer(80); // parameter is the server port

/*
 * Setup
 */
void setup() {

  Serial.begin(9600);
  connectToWifi(ssid, password);
  myServer.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_SENSE, INPUT);

}

/*
 * Loop
 * 
 * where all the work happens
 */
void loop() {
  
  WiFiClient nextClient; // clients will connect to the server
  nextClient = myServer.available(); // checks for a new client
  
  if (nextClient != NULL) { // another client is available
    
    String clientData = ""; // no client data yet
    while (nextClient.connected()) {
      
      if (nextClient.available()) { // has the client sent data?
        char ch = nextClient.read(); // read next character from client
        
        if (ch == '\n') { // we have reached the end of a line
          String clientRequest = ""; // client request after the GET command
          if (clientData.length() == 0 ) { // tell http client we are done
            nextClient.println("HTTP/1.1 200 OK");
            nextClient.println("Content-type:text/html");
            nextClient.println("Connection: close");
            nextClient.println();
            nextClient.stop(); // disconnect from the client
          } else {

            // put one line of the request into clientRequest
            clientRequest += clientData;

            // check the line to see if it starts with the string "GET",
            // if it does, then this will be the line with our special
            // values that we added to the url, such as "value=0" put
            // after the url in "192.168.x.x/value=0"
            if (clientRequest.startsWith("GET")) {
              String requestValue = clientRequest.substring(3);
              requestValue.trim();
              int HTTPIndex = requestValue.indexOf("HTTP/1.1");
              requestValue = requestValue.substring(1, HTTPIndex);
              handleRequest(requestValue);
            }
            
          }
          clientData = ""; // clear out current command (we're done with it)
        } else if (ch != '\r') {
          clientData += ch; // append character to data client sent
        } // end if (ch != '\r')
        
      } // end if (nextClient.available())
      
    } // end while
    
  } // end if (nextClient != NULL)
  
} // end loop()

/*
 * Connect to WiFi
 * 
 * connect to a wireless network given the ssid and key of the network
 */
void connectToWifi(String ssid, String key) {
  WiFi.begin(ssid, key);
  Serial.print("Connecting to: "); Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) { // while not connected
    delay(500);
    Serial.print('.');
  }
  Serial.print("\nYour IP Address is: "); Serial.println(WiFi.localIP());
}

/*
 * Handle Request
 * 
 * process the http get request
 */
void handleRequest(String request) {
  request.trim();
  request.toLowerCase();
  //Serial.print("Request: "); Serial.println(request);
  if (request.equals("on")) {
    changeLEDState(1);
  } else if (request.equals("off")) {
    changeLEDState(0);
  } else if (request.equals("toggle")) {
    toggleLED();
  }
  Serial.print("Command: "); Serial.println(request);
}

/*
 * Change LED State
 * 
 * Changes the led state to a given state
 */
void changeLEDState(int state) {
  switch (state) {
    case 0:
      digitalWrite(LED_PIN, LOW);
      break;
    case 1:
      digitalWrite(LED_PIN, HIGH);
      break;
  }
}

/*
 * Toggle LED
 * 
 * Reads the current state of the led and changes it to the opposite
 */
void toggleLED() {
  float  ledState = digitalRead(LED_SENSE);
  if (ledState >= 0.5) {
    changeLEDState(0);
  } else if (ledState < 0.5) {
    changeLEDState(1);
  }
}
