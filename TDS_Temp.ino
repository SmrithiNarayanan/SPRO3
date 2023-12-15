#include <OneWire.h>
#include <DallasTemperature.h>

const byte tds_sensor = A4;
float aref = 4.3;
float ec = 0;
unsigned int tds = 0;
float ecCalibration = 1;

#define ONE_WIRE_BUS 9

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup(void)
{
  sensors.begin(); // Start up the temperature sensor library
  Serial.begin(9600); // Debugging on hardware Serial 0
}

void loop(void)
{
  // Send the command to get temperatures
  sensors.requestTemperatures();
  // Read temperature from the sensor
  float temp = sensors.getTempCByIndex(0);
  // Call the TDS function with the current temperature
  readTdsQuick(temp);
  delay(1000);
}

void readTdsQuick(float temp)
{
  float rawEc = analogRead(tds_sensor) * aref / 1024.0; // Read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float tempCoefficient = 1.0 + 0.02 * (temp - 25.0);     // Temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  ec = (rawEc / tempCoefficient) * ecCalibration;         // Temperature and calibration compensation
  tds = (133.42 * pow(ec, 3) - 255.86 * ec * ec + 857.39 * ec) * 0.5; // Convert voltage value to TDS value
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" C | TDS: ");
  Serial.println(tds);
}
