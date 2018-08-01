import sqlite3

DB_Name = "MQTT_Data.db"

# SQLite DB Table Schema
TableSchema = """
drop table if exists DHT11_Temperature_Data;
create table DHT11_Temperature_Data (
	id integer primary key autoincrement,
	SensorID text,
	Datetime text,
	Temperature text 
);

drop table if exists DHT11_Humidity_Data;
create table DHT11_Humidity_Data (
	id integer primary key autoincrement,
	SensorID text,
	Datetime text,
	Humidity text
);
"""

# Connect or create DB file
conn = sqlite3.connect(DB_Name)
curs = conn.cursor()

# Create Tables
sqlite3.complete_statement(TableSchema)
curs.executescript(TableSchema)

# Close DB
curs.close()
conn.close()

