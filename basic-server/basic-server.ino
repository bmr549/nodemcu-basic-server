#include <ESP8266WiFi.h>

WiFiServer myServer(80); // parameter is the server port

void setup() {

  Serial.begin(9600);
  myServer.begin();

}

void loop() {
  
  WiFiClient nextClient; // clients will connect to server
  nextClient = myServer.available(); // checks for a new client
  if (nextClient != NULL) {
    String clientData = ""; // no client data yet
    while (nextClient.connected()) {
      if (nextClient.available()) {
        processClientData(nextClient);
      }
    }
  }

}

void processClientData(WiFiClient nextClient) {
  char ch = nextClient.read(); // read next character from client
  if (ch == '\n') { // if this is end of line means client is done sending commands
    if (clientData.length() == 0 { // tell http cleint we are done
      nextClient.println("HTTP/1.1 200 OK");
      nextClient.println("Content-type:text/html");
      nextClient.println("Connection: close");
      nextClient.println();
      nextClient.stop(); // disconnect from the client
    } else {
      // read clientData and process what command says
    }
    clientData = ""; // clear out current command (we're done with it)
  } else if (ch != '\r') {
    clientData += ch; // append character to data client sent
  }
}
