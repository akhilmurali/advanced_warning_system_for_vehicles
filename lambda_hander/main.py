import os
import psycopg2
import json

conn = None
cursor = None


# Handles connection cleanup
def cleanup():
    global conn
    global cursor
    cursor.close()
    conn.close()


def get_connection():
    global conn
    try:
        # connect to the PostgreSQL server
        print('Connecting to the PostgreSQL database...')
        conn = psycopg2.connect(
            host=os.getenv("DB_HOST"),
            database=os.getenv("DB_NAME"),
            user=os.getenv("DB_USER"),
            password=os.getenv("DB_PASSWORD"))
        return conn
    except (Exception, psycopg2.DatabaseError) as error:
        print(error)
        raise psycopg2.DatabaseError


def lambda_handler(event, context):
    global cursor
    connection = get_connection()
    # create a cursor
    cursor = connection.cursor()
    if "Records" in event:
        for record in event["Records"]:
            print(record["body"])
            message = json.loads(record["body"])
            # execute a statement
            lat = message["Lat"]
            lon = message["Lon"]
            water_level = 120
            query = f'INSERT INTO vws_geo_db (LAT, LON, WATER_LEVEL) VALUES({lat}, {lon}, {water_level})'
            print(query)
            cursor.execute(f'INSERT INTO vws_geo_db (LAT, LON, WATER_LEVEL) VALUES({lat}, {lon}, {water_level})')
            conn.commit()
            cleanup()
    else:
        print("Record Attribute not present in event. Quitting")
