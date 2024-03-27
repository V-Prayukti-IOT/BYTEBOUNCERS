#define sensorPin A1  // Replace with the analog pin connected to the soil moisture sensor

int sensorValue;  // Variable to store the analog reading from the sensor
int moistureLevel;  // Variable to store the calculated moisture percentage

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
}

void loop() {
  // Read analog value from the sensor pin
  sensorValue = analogRead(sensorPin);

  // Map the sensor value (0-1023) to a moisture percentage (0-100%)
  // You might need to adjust these values based on your specific sensor characteristics
  moistureLevel = map(sensorValue, 0, 1023, 100, 0);  // Dry soil maps to 100%, wet soil maps to 0%

  // Print sensor value and moisture level for debugging
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  Serial.print("Moisture Level: ");
  Serial.print(moistureLevel);
  Serial.println("%");

  // Add logic here to control something based on moisture level (optional)
  // For example, turn on an LED or activate a pump if moisture falls below a threshold

  delay(500);  // Delay between readings (adjust as needed)
}