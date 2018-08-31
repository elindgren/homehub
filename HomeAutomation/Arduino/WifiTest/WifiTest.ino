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
char pass[] = "";

//Declare and initialize variable for radio status
int status = WL_IDLE_STATUS;

//Port for webserver to communicate to Android app
WiFiEspServer server(80);

// the setup function runs once when you press reset or power the board
void setup() {
  //Initialize serial for debugging
  Serial.begin(115200);

  //Initialize serial for ESP module
  Serial1.begin(9600);

  //Initialize ESP module
  WiFi.init(&Serial1);

  //Check fo the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
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
  //Using code from: https://www.youtube.com/watch?v=ZH7ufemP8e0
  WiFiEspClient client = server.available(); //Listen for incoming clients

  if (client) { //If a client is found
    Serial.println("New Client");
    String currentLine = ""; //Make a string to hold incoming data from the client
    while (client.connected()) { //Loop while the client is connected
      if (client.available()) { //If there are bytes to read from the client
        char c = client.read(); //read the byte, then
        Serial.write(c); //print it out on the serial monitor
        if (c == '\n') { //If it's a newline char

          //If the current line is blank, you got two newline characters in a row.
          //that's the end of the client HTTP request, so send a response

          if (currentLine.length() == 0) {
            //HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            //and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("Value at A0 is ");
            client.print(analogRead(A0));
            client.print("<br>");
            //THe HTTP response ends with a other blank line:
            client.println();
            //break out of the while loop:
            break;
          }
          else { //If you got a newline, then clear currentline:
            currentLine = "";
          }
        }
        else if (c != '\r') { //If you got anything but a carriage return character
          currentLine += c; //add it to the end of the currentLine
        }

        //Check to see if the client request was "Get /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(1, HIGH); //Get /H turns the LED on
          }
          if (currentLine.endsWith("GET /L")) {
          digitalWrite(1, LOW); //Get /L turns the LED off //Change lControl to something else - in the example it is the pin that the led is connected to
        }
      }
    }
  }
}


