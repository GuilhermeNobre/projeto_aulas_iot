#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "time.h"
#include "web_data.h"
#include <PubSubClient.h>

#define pinRele 13
#define LED_BUILTIN 4

WiFiClient espClient;
PubSubClient client(espClient);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "WIFI";
const char* password = "XXXXXX";
AsyncWebServer server(80);

const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "ESP32";
const char* mqttPassword = "123";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightoffset_sec = 3600;

bool state_on = true;


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void onCSSRequest(AsyncWebServerRequest *request)
{
    request ->send(200, "text/css", style_css);
}

void onTurnHigh(AsyncWebServerRequest *request)
{
    digitalWrite(pinRele, HIGH);
    request -> send(202, "text/plain", "ON");
}

void onTurnLow(AsyncWebServerRequest *request)
{
    digitalWrite(pinRele, LOW);
    request -> send(202, "text/plain", "LOW");
}

unsigned long epochTime; 

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  Serial.println(messageTemp);

  if (messageTemp == "botao") 
  {
    if (!state_on) 
    {
      digitalWrite(pinRele, LOW);  
    }
    else
    {
      digitalWrite(pinRele, HIGH);  
    }
  }

  
  if (messageTemp == "on") {
    Serial.println("LED is on");
    digitalWrite(pinRele, LOW);
  } else if (messageTemp == "off") {
    Serial.println("LED is off");
    digitalWrite(pinRele, HIGH);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT server...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("Connected to MQTT server");
      client.subscribe("abc/dce/fgh");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(pinRele, OUTPUT);

  Serial.println("VAMOS COMECAR");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("WiFi Failed!");
    ESP.restart();
    return;
  }
  else
  {
    pinMode(LED_BUILTIN, OUTPUT);
    for(byte i = 0; i < 2; i++)
    {
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    digitalWrite(LED_BUILTIN, HIGH);
  }


  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  configTime(0, 0, ntpServer);

  epochTime = getTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  delay(1000);

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/style.css", HTTP_GET, onCSSRequest);
  server.on("/high", HTTP_GET, onTurnHigh);
  server.on("/low", HTTP_GET, onTurnLow);
  server.onNotFound(notFound);
  server.begin();

  // client.setServer(mqttServer, mqttPort);
  // client.setCallback(callback);

  // while (!client.connected()) {
  //   Serial.println("Connecting to MQTT server...");
  //   if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
  //     Serial.println("Connected to MQTT server");
  //     client.subscribe("abc/dce/fgh");
  //   } else {
  //     Serial.print("Failed with state ");
  //     Serial.print(client.state());
  //     delay(2000);
  //   }
  // }
  
}

void loop() {
  // if (!client.connected()) {
  //   reconnect();
  // }
  // client.loop();
}