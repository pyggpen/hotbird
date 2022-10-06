// wish I knew where this came from.
// see the readme for why I didn't continue this.

// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Replace with your network details
const char* ssid = "name";
const char* password = "pass";

// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 5
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature DS18B20(&oneWire);

  byte i;
  byte j;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;

// Web Server on port 80
WiFiServer server(80);

// only runs once on boot
void setup() {
  
  pinMode (0, OUTPUT); digitalWrite (0, LOW);
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  DS18B20.begin();
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.softAPdisconnect (true);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(5000);
  Serial.println(WiFi.localIP());
  digitalWrite(0, HIGH);
}

// runs over and over again
void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();
  
  if (client) {
    digitalWrite(0, LOW);
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            // getTemperature();
            //getMoreTemp();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><tt><h1>HOTBIRD1FW1BSW2019SN001</h1><h3>");

 for ( j=0; j< 10; j++) {
 if ( !oneWire.search(addr) ) { Serial.println("No more devices."); oneWire.reset_search(); break; } else { 
  client.print ("ROM = "); for( i = 0; i < 8; i++) { if ( addr[i]<16 ) { client.print("0"); } client.print(addr[i], HEX); client.println(' '); }

  
  if ( OneWire::crc8(addr, 7) != addr[7] ) { client.println(F("BAD CRC")); }

// set the chip type
   switch (addr[0]) {
   case 0x10:
      type_s = 1;
      break;
   case 0x28:
      type_s = 0;
      break;
   case 0x22:
      type_s = 0;
      break;
   default:
      Serial.println(F("BAD TYPE"));
      // alm=11;
      return;
  }

  oneWire.reset(); oneWire.select(addr); oneWire.write(0x44); delay(1000);        // start conversion
  present = oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {data[i] = oneWire.read();}

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  
celsius = (float)raw / 16.0; client.print("  Temp = "); client.println(celsius); client.print("<br>");
}}          client.println("</h3></tt></body></html>");  
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
    digitalWrite(0, HIGH);
  }
}   

