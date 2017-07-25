
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

#include <ESP8266WiFi.h>
#include <Servo.h>

Servo smotor;;
int ledPin = 0;

//const char* ssid = "UPC55DF6F7";
//const char* password = "2fsdapYb7ccU";
//const char* host = "192.168.0.73";

//const char* ssid = "Around25 Community";
//const char* password = "fullstack";
//const char* host = "192.168.0.73";

const char* ssid = "Hotspot";
const char* password = "gratisbos";
const char* host = "";

WiFiServer server(301);

void servoTurn(int x)
{
  if (x==0)
  {
    delay(1);
    smotor.write(0);
    delay(1);
  }
  else
  {
    delay(1);
    smotor.write(30);
    delay(1);
  }
}

void initWiFi()
{
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());

}

void reconnectWiFi()
{
  WiFi.begin(ssid, password);
  Serial.print("Reconnecting..");
  Serial.print("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(9600);
  smotor.attach(16); //D0
  delay(1);
  smotor.write(0);
  delay(10);
  //prepare GPIO2
  pinMode(ledPin, OUTPUT);
  digitalWrite(D3, LOW);
  initWiFi();

}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
    reconnectWiFi();


    WiFiClient client = server.available();
    if (!client) {
      return;
    }

    while (!client.available()) {
      delay(1);
    }

    String req = client.readStringUntil('\r');
    client.flush();
    // Match the request
    if (req.indexOf("") != -10) {  //checks if you're on the main page

      if (req.indexOf("/OFF") != -1) { //checks if you clicked OFF
        digitalWrite(ledPin, LOW);
        Serial.println("Requested OFF");
        servoTurn(0);
      }
      if (req.indexOf("/ON") != -1) { //checks if you clicked ON
        digitalWrite(ledPin, HIGH);
        Serial.println("Requested ON");
        servoTurn(1);
      }
    }
    else {
      Serial.println("invalid request");
     client.stop();
      return;
    }

    String s = "HTTP/1.1 200 OK\r\n";
    s += "Content-Type: text/html\r\n\r\n";
    s += "<!DOCTYPE HTML>\r\n<html>\r\n";
    s += "<br><input type=\"button\" name=\"bl\" value=\"ON\" onclick=\"location.href='/ON'\">";
    s += "<br><br><br>";
    s += "<br><input type=\"button\" name=\"bl\" value=\"OFF\" onclick=\"location.href='/OFF'\">";
    s += "</html>\n";
    client.flush();
    client.print(s);
    delay(1);
}
