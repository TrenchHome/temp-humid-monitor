#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#define dhtPin 16

/*WiFi connention*/
const char* ssid = "zepheroo";
const char* password = "99999999";

/*MQTT connection*/
IPAddress mqtt_server(192,168,43,232);
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_id = "my_id";
const char* mqtt_user = "my_user";
const char* mqtt_pass = "my_pass";
const char* topic = "test";

/*Global Variables*/
unsigned long prevMillis = 0;
const int interval = 5000;    //read data per 5 seconds
char payload[256];
DHTesp dht;

void setup_wifi() {
  delay(10);
  Serial.println("\nWifi connencting to " + String(ssid));
  
  WiFi.begin(ssid, password);  //In ESP8266WiFiSTAClass
  while(WiFi.status() != WL_CONNECTED) { //status() return wl_status_t object
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWifi connencted. IP adderss: " + WiFi.localIP().toString() + "\n"); //inherent from WiFiClient.localIP()
}

void reconnect() {
  while(!client.connected()) {
    if (client.connect(mqtt_id, mqtt_user, mqtt_pass))
      Serial.println("MQTT connedted");
    else {
      Serial.println("failed, rc="+ String(client.state())+ " try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.setup(dhtPin, DHTesp::DHT11);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  if (millis()-prevMillis > interval) {
    prevMillis = millis();
    sprintf(payload, "{\"temp\": %.2f, \"humid\": %.2f}", dht.getTemperature(), dht.getHumidity());
    Serial.println("Payload: " + String(payload));
    client.publish(topic, payload, true);
  }
}

