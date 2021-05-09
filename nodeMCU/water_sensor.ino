#define sensorPower 7
#define sensorPin A0
#define CALIBRATED_THRESHOLD 500
#define READ_FREQUENCY 60000
// Value for storing water level
int val = 0;
int lat = 0.00, lon= 0.00;

void setup() {
  // Set D7 as an OUTPUT and use it as power pin to sensor
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
  Serial.begin(9600);
}

bool crossed_threshold(int sensorValue){
  return sensorValue > CALIBRATED_THRESHOLD;
}

int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}

void loop() {
  //get the reading from the function below and print it
  int level = readSensor();
  Serial.print("Water level: ");
  Serial.println(level);
  if (crossed_threshold(level)){
    //Publish a message with lat lon to gateway device.
    Serial.print("Water Level breached threshold. Publishing Data to local gateway device");
  }
  // Read the sensor data once every minute
  delay(READ_FREQUENCY);
}