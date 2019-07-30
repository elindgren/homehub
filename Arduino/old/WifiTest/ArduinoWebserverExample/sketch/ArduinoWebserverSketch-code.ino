/*
   Based on Webserver example by Bportaluri
   http://kjll.cm/arduino-wifiesp
*/

//Include the correct libraries; SoftwareSerial and Wifi Library
#include <SoftwareSerial.h>
#include "WiFiEsp.h"
// 433 MHz sender
#include <RCSwitch.h>

//Create WiFi module object on GPIO pin 6 (RX) and 7 (TX)
SoftwareSerial Serial1(6, 7);

//Create RC module
RCSwitch mySwitch = RCSwitch();

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

  //Initialize RC module - output on pin 10 (PWM)
  mySwitch.enableTransmit(10);

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

String toggleLights(String command) {
  // Parses the sent command and toggles the corresponding lights and generates a response.
  Serial.println("Command: " + command);
  String response = "";
  if (command == "GET /AH") {
    Serial.println("Turning On LED!");
    digitalWrite(LED_BUILTIN, HIGH); 
    response = "LED On";
    mySwitch.send("000101010001010101010101"); //Rec. 1 on
  } else if (command == "GET /AL") {
    Serial.println("Turning Off LED!");
    digitalWrite(LED_BUILTIN, LOW);
    response = "LED Off";
    mySwitch.send("000101010001010101010111"); //Rec. 1 off
  } else if (command == "GET /TH") {
    Serial.println("Turning on TV!");
    response = "TV On";
  } else if (command == "GET /TL") {
    Serial.println("Turning off TV!");
    response = "TV Off";
  } else if (command == "GET /1H") {
    // Turns on shelf 1
    Serial.println("Turning on shelf 1!");
    response = "Shelf 1 on";
  } else if (command == "GET /1L") {
    // Turns off shelf 1
    Serial.println("Turning off shelf 1!");
    response = "Shelf 1 off";
  } else if (command == "GET /2H") {
    // Turns on shelf 2
    Serial.println("Turning on shelf 2!");
    response = "Shelf 2 on";
  } else if (command == "GET /2L") {
    // Turns off shelf 2 
    Serial.println("Turning off shelf 2!");
    response = "Shelf 2 off";
  } else if (command == "GET /VH") {
    // Turns on vitrin
    Serial.println("Turning on vitrin!");
    response = "Vitrin on";
  } else if (command == "GET /VL") {
    // Turns off vitrin
    Serial.println("Turning off vitrin!");
    response = "Vitrin off";
  } else {
    Serial.println("Unknown command!");
    response = "Unknown command";
  }
  return response;
}

void loop() {
  //Based on the Arduino Ethernet Webserver example code
  //URL: https://www.arduino.cc/en/Tutorial/WebServer

  //Listen for incoming clients
  WiFiEspClient client = server.available(); //Using WifiEsp, not Ethernet!
  if (client) {
    Serial.println("New client");
    //an http request ends with a blank line
    bool currentLineIsBlank = true;
    String currentLine = "";
    String command = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a
        // newline character) and the line is blan, the http request has ended
        // So you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print(toggleLights(command));
          client.print("<br />");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          //Save the command from the first line if the current line is the first line.
          if (currentLine.indexOf("GET") > -1) {
            currentLine.remove(8, 9); // Remove the HTTP/1.1 part of the request
            currentLine.trim();
            command = currentLine;
            Serial.println("Command found: " + command);
          }
          // You're starting a new line
          currentLineIsBlank = true;
          currentLine = "";
        } else if (c != '\r') {
          //you've gotten a character on the current line
          currentLineIsBlank = false;
          currentLine += c;

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

