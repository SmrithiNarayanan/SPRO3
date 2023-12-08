#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define USONIC_DIV 58.0
#define MEASURE_SAMPLE_DELAY 5
#define MEASURE_SAMPLES 20
#define MEASURE_DELAY 25

// Motor A
int enA = 3;
int in1 = 2;
int in2 = 4;

// Motor B
int enB = 5;
int in3 = 6;
int in4 = 7;

//flag variables
int flag=0;
const int buttonPin = 10;  

void setup(){
Serial.begin(9600); // Serial monitoring
pinMode(TRIGGER_PIN, OUTPUT); // Initializing Trigger Output and Echo Input
pinMode(ECHO_PIN, INPUT);
digitalWrite(TRIGGER_PIN, LOW); // Reset the trigger pin and wait a half a second
delayMicroseconds(10000);

//bump sensor setup
pinMode(buttonPin, INPUT_PULLUP);


// Set all the motor control pins to outputs
pinMode(enA, OUTPUT);
pinMode(enB, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
}

void loop(){
int buttonState;

if (flag==0) {
  Movemotor(1,1,0,200);
  
  
  //delay(MEASURE_DELAY);
  long distance = measure()+22;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" mm");
  Serial.println("flag0");

  // Check if the distance value is 200 four times
  static int counter_0 = 0;
  if (distance <= 400) {
    counter_0++;
    if (counter_0 == 3) {
      flag=1;
      counter_0 = 0;
      Serial.println("flag=1");
    }
  } else {
    counter_0 = 0;
  }
  delay(1000);
}

if (flag==1){
  buttonState = LOW;
  while (buttonState == LOW) {
    Movemotor(2, 2,1000,100);
    Serial.println("NO");
    buttonState = digitalRead(buttonPin);
  }
  Serial.println("button pressed");
  Movemotor(2, 2,1000,200); //goes back
  Serial.println("Back");
  Movemotor(1, 2,2000,200); //turns 90 degrees
  Serial.println("turn");
  Movemotor(1, 2,4000,100); //forward a bit
  Serial.println("forward");
  if(digitalRead(buttonPin) == HIGH){
    Movemotor(2, 2,1000,200); //goes back
    Movemotor(1, 2,2000,200); //turns 90 degrees
    flag = 2;
    Serial.println("flag=2");
  }
  else{
    Movemotor(1, 2,2000,200); //turns 90 degrees
    flag = 0;
  }
}

if(flag==2){
  Movemotor(1, 1,0,200); 
  // check distance
  long distance = measure()+22;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" mm");
  Serial.println("flag2");

  // Check if the distance value
  static int counter_0 = 0;
  if (distance <= 400) {
    counter_0++;
    if (counter_0 == 3) { // near wall 
      buttonState = LOW;
      while (buttonState == LOW) {
        Movemotor(2, 2,1000,100);// slow down motor
        Serial.println("NO");
        buttonState = digitalRead(buttonPin);
      }
      Serial.println("button pressed");
      flag = 3;
    }
  } else {
    counter_0 = 0;
    Movemotor(1, 1,0,200); // keep moving forward as normal 
  }
  delay(1000);
  }

  if (flag == 3){
    Movemotor(0, 0,0,200);
  }

}

void Movemotor(int motoradir, int motorbdir,int time,int speed) {
  if (motoradir == 1){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }

  if (motoradir == 2){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }
  if (motoradir == 0){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }
  if (motorbdir == 1){
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

  if (motorbdir == 2){
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }
  if (motorbdir == 0){
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }
  
  analogWrite(enA, speed); 
  analogWrite(enB, speed); 
  delay(time);
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
