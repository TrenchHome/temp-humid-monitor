import json
import sqlite3

DB_Name = "MQTT_Data.db"

# ======================
# Database Manager Class

class DatabaseManager():
	def __init__(self):
		self.conn = sqlite3.connect(DB_Name)
		self.conn.execute('pragma foreign_keys = on')
		self.conn.commit()
		self.cur = self.conn.cursor()

	def add_del_update_db_record(self, sql_query, args=() ):
		self.cur.execute(sql_query, args)
		self.conn.commit()
		return

	def __del__(self):
		self.cur.close()
		self.conn.close()

# ======================
# Functions to push received data into database

# Save data to DHT11_Temperate_Data and DHT11_Humidity_Data table

def DataHandler(topic, jsonData):
	json_Dict = json.loads(jsonData)
	sensorID = json_Dict['SensorID']
	datetime = json_Dict['Datetime']
	temp = json_Dict['Temperature']
	humi = json_Dict['Humidity']
	dbObj = DatabaseManager()
	dbObj.add_del_update_db_record("insert into DHT11_Temperature_Data(SensorID, Datetime, Temperature) values(?,?,?)", [sensorID, datetime, temp])
	dbObj.add_del_update_db_record("insert into DHT11_Humidity_Data(SensorID, Datetime, Humidity) values(?,?,?)", [sensorID, datetime, humi])
	
	del dbObj
	print ("Data is inserted into " + DB_Name)
	print ("")
