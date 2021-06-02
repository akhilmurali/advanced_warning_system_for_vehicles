#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#define sensorPin A0
#define READ_FREQUENCY 60000
// Value for storing water level
int val = 0;
const char* clientID = "Client ID";
const char* ssid = "XXXXXXXXXXXXXXX";
const char* wifi_password = "XXXXXXXXXXXXXX";
const char* mqtt_server = "XXXXXXXXXXXXX";
const char* mqtt_topic = "topic/water_level_breach";
const char* mqtt_username = "mos_user";
const char* mqtt_password = "XXX";
String coordinates[5][2] = {{"10.530345", "76.214729"}, {"10.073232", "76.302765"}, {"10.051969", "76.315773"}, {"10.107796", "10.107796"}, {"10.184909", "76.375305"}};

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker

void setup() {
  // Set D7 as an OUTPUT and use it as power pin to sensor
  Serial.begin(9600);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}

int readSensor() {
  val = analogRead(sensorPin);
  return val;            

void loop() {
  int level = readSensor();
  Serial.print("Water level: ");
  Serial.println(level);
  int random_index = random(0, 4);
  String data = "{\"Lat\":" + coordinates[random_index][0] + ", \"Lon\":" + coordinates[random_index][1] + ", \"WaterLevel\":" + level + "}";
  Serial.println(data);
  int str_len = data.length() + 1; 
  char char_array[str_len];
  data.toCharArray(char_array, str_len);
  if (client.publish(mqtt_topic, char_array)) {
     Serial.println("Published data to local broker.");
  }
  else {
     Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
     client.connect(clientID, mqtt_username, mqtt_password);
     delay(10); 
     client.publish(mqtt_topic, char_array);
  }
  delay(READ_FREQUENCY);
}
