#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

/***********************************/
#define dhtPin 16
/***********************************/

/*WiFi connention*/
const char* WiFi_ssid = "lifeisajoke";                  //WiFi AP ssid
const char* WiFi_pswd = "lazyisthebestpolicy";          //WiFi AP pswd

/*MQTT connection*/
IPAddress MQTT_broker(192,168,43,45);                   //MQTT broker ip address
const int MQTT_port = 1883;

/*****************************************************************************/

WiFiClient espClient;
PubSubClient client(espClient);

const char* MQTT_clientID = "client_id";
const char* MQTT_user = "my_user";
const char* MQTT_pswd = "my_pswd";
const char* MQTT_dht_topic = "Factory/dht";
const char* MQTT_sensor_topic = "Factory/sensor";

/*Global Variables*/
unsigned long prevMillis = 0;
const int interval = 5000;    //read data per 5 seconds
char pub_msg[256];
DHTesp dht;

void setup_wifi() {
  delay(10);
  Serial.println("\nWifi connencting to " + String(WiFi_ssid));
  
  WiFi.begin(WiFi_ssid, WiFi_pswd);
  while(WiFi.status() != WL_CONNECTED) {  //status() return wl_status_t object
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWifi connencted. IP adderss: " + WiFi.localIP().toString() + "\n"); //inherent from WiFiClient.localIP()
}

void reconnect() {
  while(!client.connected()) {
    if (client.connect(MQTT_clientID, MQTT_user, MQTT_pswd)) {
      Serial.println("MQTT-broker connected");
      if (client.subscribe(MQTT_sensor_topic, 0)) {
        Serial.print("Subsrcibe topic \"");
        Serial.print(MQTT_sensor_topic);
        Serial.println("\" succesfully.\n");
      }
    }
    else {
      Serial.println("MQTT-broker connection failed, rc="+ String(client.state())+ " try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(const char topic[], byte* payload, unsigned int length) {  
  Serial.print("\n*** Message from topic \"");
  Serial.print(topic);
  Serial.println("\" ***");
  Serial.print("*** Msg: \"");
  for (int i=0; i<length; i++)
    Serial.print( (char)payload[i] );
  Serial.println("\" ***\n");
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
    sprintf(pub_msg, "{\"SensorID\":\"%d\", \"Datetime\":\"%ld\", \"Temperature\":\"%.2f\", \"Humidity\":\"%.2f\"}", 1, prevMillis, dht.getTemperature(), dht.getHumidity());
    client.publish(MQTT_dht_topic, pub_msg);
    Serial.print("Message is published to topic \"");
    Serial.print(MQTT_dht_topic);
    Serial.println("\"");

    Serial.print("msg published: ");
    Serial.println(pub_msg);
  }
}

