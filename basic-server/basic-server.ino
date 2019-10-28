/*
 * Basic Web Server on NodeMCU
 * 
 * 
 */

// ---- DEPENDENCIES ----
#include <ESP8266WiFi.h>

// ---- MODIFIABLE CONSTANTS ----
const char * ssid = "LU-IoT2"; // put your ssid (network name) here
const char * password = "CSC49008"; // put your network password here

// ---- UNMODIFIABLE CONSTANTS ----
WiFiServer myServer(80); // parameter is the server port


/*
 * Setup
 */
void setup() {

  Serial.begin(9600);
  connectToWifi(ssid, password);
  myServer.begin();

}

/*
 * Loop
 */
void loop() {
  WiFiClient nextClient; // clients will connect to the server
  nextClient = myServer.available(); // checks for a new client
  if (nextClient != NULL) {
    String clientData = ""; // no client data yet
    while (nextClient.connected()) {
      if (nextClient.available()) { // has the client sent data?
        processClientData(nextClient, clientData);
      }
    }
  }
}

/*
 * Process Client Data
 */
void processClientData(WiFiClient nextClient, String clientData) {
  char ch = nextClient.read(); // read next character from client
  if (ch == '\n') { // we have reached the end of a line
    if (clientData.length() == 0 ) { // tell http client we are done
      nextClient.println("HTTP/1.1 200 OK");
      nextClient.println("Content-type:text/html");
      nextClient.println("Connection: close");
      nextClient.println();
      nextClient.stop(); // disconnect from the client
      Serial.println("Client Disconnected");
    } else {
      
    }
    clientData = ""; // clear out current command (we're done with it)
  } else if (ch != '\r') {
    clientData += ch; // append character to data client sent
  }
}

/*
 * Connect to WiFi
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
