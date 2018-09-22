/*
   Based on Webserver example by Bportaluri
   http://kjll.cm/arduino-wifiesp
*/

//Include the correct libraries; SoftwareSerial and Wifi Library
#include <SoftwareSerial.h>
#include "WiFiEsp.h"


//Create WiFi module object on GPIO pin 6 (RX) and 7 (TX)
SoftwareSerial Serial1(6, 7);

//Define network and password for Wifi; global arrays for wifi settings
char ssid[] = ">tfw good internet";
char pass[] = "07344442";

//Declare and initialize variable for radio status
int status = WL_IDLE_STATUS;

//Port for webserver to communicate to Android app
WiFiEspServer server(80);

// the setup function runs once when you press reset or power the board
void setup() {
  // DEBUG initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //Initialize serial for debugging
  Serial.begin(115200);

  //Initialize serial for ESP module
  Serial1.begin(9600);

  //Initialize ESP module
  WiFi.init(&Serial1);

  //Check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Wifi status: ");
    Serial.println(WiFi.status());
    Serial.println("WiFi shield not present!");
    //Don't continue
    while (true);
  }

  //Attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    //Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);

  }
  Serial.println("You're connected to the network");
  printWifiStatus();

  //Start webservver
  server.begin();
}

void printWifiStatus() {
  //Print the SSID of the network
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  //Print the IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  //Based on the Arduino Ethernet Webserver example code
  //URL: https://www.arduino.cc/en/Tutorial/WebServer
  
  //Listen for incoming clients
  WiFiEspClient client = server.available(); //Using WifiEsp, not Ethernet!
  if(client){
    Serial.println("New client");
    //an http request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()){
      if (client.available()){
          char c = client.read();
          Serial.write(c);
          // if you've gotten to the end of the line (received a
          // newline character) and the line is blan, the http request has ended
          // So you can send a reply
          if (c == '\n' && currentLineIsBlank){
            // send a standard http response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println("Refresh: 5");  // refresh the page automatically every 5 sec
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.print("LED On");
            client.print("<br />");
            client.println("</html>");
            break;
          }
          if (c == '\n'){
            // You're starting a new line
            currentLineIsBlank = true;
          } else if (c != '\r'){
            //you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
   }
 }
