#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define USONIC_DIV 58.0
#define MEASURE_SAMPLE_DELAY 5
#define MEASURE_SAMPLES 20
#define MEASURE_DELAY 25

// Motor A
int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor B
int enB = 3;
int in3 = 5;
int in4 = 4;

void setup(){
Serial.begin(9600); // Serial monitoring
pinMode(TRIGGER_PIN, OUTPUT); // Initializing Trigger Output and Echo Input
pinMode(ECHO_PIN, INPUT);
digitalWrite(TRIGGER_PIN, LOW); // Reset the trigger pin and wait a half a second
delayMicroseconds(10000);

// Set all the motor control pins to outputs
pinMode(enA, OUTPUT);
pinMode(enB, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
}

void loop(){
  // Turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 200); // Set speed to 200 out of possible range 0~255
  // Turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  analogWrite(enB, 200); // Set speed to 200 out of possible range 0~255
  
  
  delay(MEASURE_DELAY);
  long distance = measure();
  Serial.print("Distance: ");
  Serial.print(distance + 22);
  Serial.println(" mm");

  // Check if the distance value is 200 four times
  static int counter = 0;
  if (distance <=240) {
    counter++;
    if (counter == 4) {
      switchMotorDirection();
      counter = 0;
      Serial.println("Switched direction!");
    }
  } else {
    counter = 0;
  }

  delayMicroseconds(1000);
}

void switchMotorDirection() {
  // Turn on motor A in opposite direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 200); // Set speed to 200 out of possible range 0~255
  // Turn on motor B in opposite direction
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  analogWrite(enB, 200); // Set speed to 200 out of possible range 0~255
  delay(4000);
}

long measure(){
long measureSum = 0;
for (int i = 0; i < MEASURE_SAMPLES; i++){
delay(MEASURE_SAMPLE_DELAY);
measureSum += singleMeasurement();
}
return measureSum / MEASURE_SAMPLES;
}

long singleMeasurement(){
long duration = 0; // Measure: Put up Trigger...
digitalWrite(TRIGGER_PIN, HIGH); // Wait for 11 µs ...
delayMicroseconds(20); // Put the trigger down ...
digitalWrite(TRIGGER_PIN, LOW); // Wait for the echo ...
duration = pulseIn(ECHO_PIN, HIGH);
return (long) (((float) duration / USONIC_DIV) * 10.0);
}