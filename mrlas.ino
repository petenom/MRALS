// Copyright (c) Arduino. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


// for ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>


int NodeMCUpirPin = 5;
char* ssid = "abcxyz"; //  your network SSID (name)
char* pass = "abcxyz";    // your network password (use for WPA, or use as key for WEP)
String apiKey = "abcxyz";
const char* server = "api.thingspeak.com";

// change the next line to use on non-WiFi101 based boards/shields

WiFiClientSecure sslClient; // for ESP8266
WiFiClient client;


void setup() {
  pinMode(NodeMCUpirPin, INPUT);
  digitalWrite(NodeMCUpirPin, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  blinkLedStatus(1);
  initSerial();
  initWifi();
  //  initTime();

  // iotHubClient.begin();
}

void PublishRoomBooked()
{

  if (client.connect(server, 80)) { //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String("1");
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("Room not Available");
  }
  client.stop();


}

void PublishRoomAvailable()
{

  if (client.connect(server, 80)) { //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String("0");
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("Room  Available");
  }
  client.stop();



}



void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    delay(100);
    initWifi();
    return;
  }

  if (digitalRead(NodeMCUpirPin) == HIGH) {
    Serial.println("High Pin");
    PublishRoomBooked();
    digitalWrite(LED_BUILTIN, LOW);
  } else {
        Serial.println("Low Pin");
        PublishRoomAvailable();
        digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(16000);

}


void initSerial() {
  //Initialize serial and wait for port to open:


  // Uncomment the next two lines For ESP8266 boards (and comment out the line above)
  Serial.begin(115200);
  Serial.setDebugOutput(true);

}

void initWifi() {
  
  boolean blnSSIDfound = false;
  int intSSIDStringfound = -1;
  //set IP if not correct
  IPAddress ip = WiFi.localIP();

  // Connect to WiFi network
  Serial.println();
  delay(10);
  Serial.println();
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  Serial.println("Setup done");
  Serial.println();
  delay(10);

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);

      intSSIDStringfound = WiFi.SSID(i).indexOf('STCS');
      if (intSSIDStringfound >0) {
        Serial.println("STCS fOUND");
         ssid = "abcxyz"; //  your network SSID (name)
         pass = "abcxyz";    // your network password (use for WPA, or use as key for WEP)
         goto foundSSID;
      } //endif STCS

      intSSIDStringfound=0;
      intSSIDStringfound = WiFi.SSID(i).indexOf('Gurgaon');
      if (intSSIDStringfound >0) {
        Serial.println("Gurgaon Lab fOUND");
         ssid = "abcxyz"; //  your network SSID (name)
         pass = "abcxyz";    // your network password (use for WPA, or use as key for WEP)
         goto foundSSID;
      } //endif LAB

    
    }
  }
  Serial.println("");

foundSSID:
  // Wait a bit before scanning again
  delay(5000);

//forceSSID
         ssid = "abcxyz"; //  your network SSID (name)
         pass = "abcxyz";    // your network password (use for WPA, or use as key for WEP)

         
  
  Serial.print("Connecting to ");
  delay(10);
  Serial.println(ssid);
  delay(10);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.print("ESP8266 IP: ");
  Serial.println(WiFi.localIP());

  blinkLedStatus(2);

}

void blinkLedStatus(int x)
{
int ledState = LOW;     


switch (x) {

  case 1 :  // Booting Up)
            for (int i=0 ; i <= 20 ; i++) {
              if (ledState == LOW)
                  ledState = HIGH;  // Note that this switches the LED *off*
              else
                  ledState = LOW;   // Note that this switches the LED *on*
                  digitalWrite(LED_BUILTIN, ledState);
              delay(50);              
            }
          break;

    case 2 :  // Connected to WIFI)
            for (int i=0 ; i <= 10 ; i++) {
              if (ledState == LOW)
                  ledState = HIGH;  // Note that this switches the LED *off*
              else
                  ledState = LOW;   // Note that this switches the LED *on*
                  digitalWrite(LED_BUILTIN, ledState);
              delay(100);              
            }
          break;

}
  


  
}
