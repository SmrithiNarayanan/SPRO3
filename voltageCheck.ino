
int analogPin= A2; 
float threshold = 3.75; // might change based on testing 

void setup() {
  Serial.begin(9600);

}

void loop() {
// read voltage 
int sensorVal = analogRead(analogPin);
//convert the sensor value to voltage 
float voltage = sensorVal *(5.0/1023.0);

//Print voltage 
Serial.print("Voltage: ");
Serial.print(voltage);
Serial.println(" V");

//Check if below threshold 
if (voltage < threshold) {
    Serial.println("Battery Low!");
  }

}
