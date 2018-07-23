import paho.mqtt.client as mqtt
from DatabaseManager import DataHandler

# MQTT settings
MQTT_Broker = "140.113.67.247" # test ip address
MQTT_Port = 1883
Keep_Alive_Interval = 45
MQTT_Topic = "Factory/#" # all topic below Factory

# sub topics
def on_connect(mosq, obj, flags, rc):
	mqttc.subscribe(MQTT_Topic, 0)

# save data into DB table
def on_message(mosq, obj, msg):
	print ("MQTT Data Received...")
	print ("MQTT Topic: " + msg.topic )
	print ("Data: " + str(msg.payload))
	DataHandler(msg.topic, msg.payload)

def on_subscribe(mosq, obj, mid, granted_qos):
	print ("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_log(mqttc, obj, level, string):
	print (string)

mqttc = mqtt.Client()

# Assign event callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe

# Connect
mqttc.connect(MQTT_Broker, int(MQTT_Port), int(Keep_Alive_Interval) )

# Continue the network loop
mqttc.loop_forever()
