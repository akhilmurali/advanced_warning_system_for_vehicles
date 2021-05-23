import paho.mqtt.client as mqtt
import ssl
import thread

run_flag = True
#Event Handlers
def on_connect_aws_broker(local_client, userdata, flags, rc):    
    print("Connection returned result: " + str(rc) )

def on_message_aws_broker(local_client, userdata, msg):
    print("topic: "+msg.topic+"     payload: "+str(msg.payload))

def on_connect_local_broker(local_client, userdata, flags, rc):
    # rc is the error code returned when connecting to the broker
    print("Connected!", str(rc))
    # Once the local_client has connected to the broker, subscribe to the topic
    local_client.subscribe(local_mqtt_topic)

def on_message_local_broker(local_client, userdata, msg):
    print("Topic: ", msg.topic + "\nMessage: " + str(msg.payload))
    #hostendpoint: a37mwryi7jvbcj-ats.iot.ap-south-1.amazonaws.com
    #Publish to AWS IoT Core broker when message is received.
    aws_client.publish("topic/water_level", str(msg.payload), qos=1)        
    print("Sent message to aws broker")

local_mqtt_username = "mos_user"
local_mqtt_password = "123"
local_mqtt_topic = "topic/water_level_breach"
local_mqtt_broker_ip = "raspberrypi.local"

local_client = mqtt.Client()
local_client.username_pw_set(local_mqtt_username, local_mqtt_password)
#Bind hanlders to local_client
local_client.on_connect = on_connect_local_broker
local_client.on_message = on_message_local_broker
local_client.connect(local_mqtt_broker_ip, 1883)

aws_client = mqtt.Client()
awshost = "a37mwryi7jvbcj-ats.iot.ap-south-1.amazonaws.com"
awsport = 8883
clientId = "vws-gateway-device"
thingName = "vws-gateway-device"
caPath = "/home/pi/Desktop/vws_gateway/Certificates/AmazonRootCA1.pem"
#path to certificate
certPath = "/home/pi/Desktop/vws_gateway/Certificates/vws-gateway-device.cert.pem"
#path to private key
keyPath = "/home/pi/Desktop/vws_gateway/Certificates/vws-gateway-device.private.key"
#set the TLS parameters
aws_client.tls_set(caPath, certfile=certPath, keyfile=keyPath, cert_reqs=ssl.CERT_REQUIRED, tls_version=ssl.PROTOCOL_TLSv1_2, ciphers=None)
aws_client.on_connect = on_connect_aws_broker
aws_client.on_message = on_message_aws_broker
#establish a connection with the AWS endpoint
aws_client.connect(awshost, awsport, keepalive=60)

#Attach Loop related functions:
while (run_flag):
    aws_client.loop(0.01)
    local_client.loop(0.01)

