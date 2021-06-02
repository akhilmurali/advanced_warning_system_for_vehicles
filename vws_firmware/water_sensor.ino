#include <ESP8266WiFi.h> // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
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

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}

int readSensor() {
  val = analogRead(sensorPin);    // Read the analog value form sensor
  return val;             // send current reading
}

void loop() {
  //get the reading from the function below and print it
  int level = readSensor();
  Serial.print("Water level: ");
  Serial.println(level);
  int random_index = random(0, 4);
  String data = "{\"Lat\":" + coordinates[random_index][0] + ", \"Lon\":" + coordinates[random_index][1] + ", \"WaterLevel\":" + level + "}";
  Serial.println(data);
  int str_len = data.length() + 1; 
  // Prepare the character array (the buffer) 
  char char_array[str_len];
  // Copy it over 
  data.toCharArray(char_array, str_len);
  if (client.publish(mqtt_topic, char_array)) {
     Serial.println("Published data to local broker.");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
     Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
     client.connect(clientID, mqtt_username, mqtt_password);
     delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
     client.publish(mqtt_topic, char_array);
  }
  // Read the sensor data once every minute
  delay(READ_FREQUENCY);
}
