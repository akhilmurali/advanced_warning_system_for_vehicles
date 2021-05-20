import paho.mqtt.client as mqtt

mqtt_username = "mos_user"
mqtt_password = "***"
mqtt_topic = "topic/water_level_breach"
mqtt_broker_ip = "raspberrypi.local"

client = mqtt.Client()
client.username_pw_set(mqtt_username, mqtt_password)

def on_connect(client, userdata, flags, rc):
    # rc is the error code returned when connecting to the broker
    print "Connected!", str(rc)

    # Once the client has connected to the broker, subscribe to the topic
    client.subscribe(mqtt_topic)

def on_message(client, userdata, msg):
    print "Topic: ", msg.topic + "\nMessage: " + str(msg.payload)
    #hostendpoint: a37mwryi7jvbcj-ats.iot.ap-south-1.amazonaws.com
    #Publish to AWS IoT Core broker when message is received.

#Bind hanlders to client
client.on_connect = on_connect
client.on_message = on_message

client.connect(mqtt_broker_ip, 1883)
client.loop_forever()
client.disconnect()
