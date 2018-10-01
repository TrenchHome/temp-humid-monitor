#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

/***********************************/
#define dhtPin 16
/***********************************/

/*WiFi connention*/
const char* WiFi_ssid = "zepheroo";
const char* WiFi_pswd = "99999999";

/*MQTT connection*/
IPAddress MQTT_broker(192,168,43,232);
const int MQTT_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const char* MQTT_clientID = "client_id";
const char* MQTT_user = "my_user";
const char* MQTT_pswd = "my_pswd";
const char* MQTT_topic = "Factory/test";

/*Global Variables*/
unsigned long prevMillis = 0;
const int interval = 5000;    //read data per 5 seconds
char pub_msg[256];
DHTesp dht;

void setup_wifi() {
  delay(10);
  Serial.println("\nWifi connencting to " + String(WiFi_ssid));
  
  WiFi.begin(WiFi_ssid, WiFi_pswd);  //In ESP8266WiFiSTAClass
  while(WiFi.status() != WL_CONNECTED) { //status() return wl_status_t object
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWifi connencted. IP adderss: " + WiFi.localIP().toString() + "\n"); //inherent from WiFiClient.localIP()
}

void reconnect() {
  while(!client.connected()) {
    if (client.connect(MQTT_clientID, MQTT_user, MQTT_pswd))
      Serial.println("MQTT-broker connected");
    else {
      Serial.println("MQTT-broker connection failed, rc="+ String(client.state())+ " try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(const char topic[], byte* payload, unsigned int length) {  
  Serial.println("*********************\n******\n***");
  Serial.println(topic);
  for (int i=0; i<length; i++)
    Serial.println( (char)payload[i] );
}

void setup() {
  Serial.begin(115200);
  dht.setup(dhtPin, DHTesp::DHT11);
  setup_wifi();
  client.setCallback(callback);
  client.setServer(MQTT_broker, MQTT_port);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  if (millis()-prevMillis >= interval) {
    prevMillis = millis();
    sprintf(pub_msg, "{\"SensorID\":\"%d\", \"Datetime\":\"%s\", \"Temperature\":\"%.2f\", \"Humidity\":\"%.2f\"}", 1, "now", dht.getTemperature(), dht.getHumidity());
    Serial.print("message is published: ");
    Serial.println(pub_msg);
    client.publish(MQTT_topic, pub_msg);
  }
}

