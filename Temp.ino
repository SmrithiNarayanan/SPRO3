#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 9

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  // Start the DS18B20 sensor
 sensors.begin();
}
void loop() {
  // Request temperature data from all sensors
  sensors.requestTemperatures();
  // Get the temperature in Celsius
  float temperatureC = sensors.getTempCByIndex(0);
  // Check if the temperature reading is valid
  if (temperatureC != DEVICE_DISCONNECTED_C) {
    // Print the temperature to the Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
  } else {
    Serial.println("Error reading temperature");
  }

  // Wait for a moment before taking the next reading
  delay(1000);
}