#include <OneWire.h>
#include <DallasTemperature.h>

#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define USONIC_DIV 58.0
#define MEASURE_SAMPLE_DELAY 5
#define MEASURE_SAMPLES 20
#define MEASURE_DELAY 25
#define SensorPin 1          // the pH meter Analog output is connected with the Arduino’s Analog
#define ONE_WIRE_BUS 9 //TDS 

// Motor A
int enA = 3;
int in1 = 2;
int in2 = 4;

// Motor B
int enB = 5;
int in3 = 6;
int in4 = 7;

// Flag variables
int flag = 0;
const int buttonPin = 10;

//pH variables
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b, pH;
int buf[10],temp;

//TDS variables and config
const byte tds_sensor = A4;
float aref = 4.3;
float ec = 0;
unsigned int tds = 0;
unsigned int TDS = 0;
float ecCalibration = 1;
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup() {
    Serial.begin(9600); // Serial monitoring
    pinMode(TRIGGER_PIN, OUTPUT); // Initializing Trigger Output and Echo Input
    pinMode(ECHO_PIN, INPUT);
    digitalWrite(TRIGGER_PIN, LOW); // Reset the trigger pin and wait a half a second
    delayMicroseconds(10000);
    sensors.begin(); // Start up the temperature sensor library

    //pH setup
    pinMode(13,OUTPUT);

    // Bump sensor setup
    pinMode(buttonPin, INPUT);

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
  //if user selection 1
  mode1(); 
  // if user selection 2
  mode2(); 
  // if user selection 3
  mode3(); 
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

float obtainpH(){
  for(int i=0;i<10;i++)   	//Get 10 sample value from the sensor for smooth the value
  {
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
  avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  digitalWrite(13, HIGH);   	
  delay(800);
  digitalWrite(13, LOW);
  return phValue;
}

unsigned int readTdsQuick(float temp)
{
  float rawEc = analogRead(tds_sensor) * aref / 1024.0; // Read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float tempCoefficient = 1.0 + 0.02 * (temp - 25.0);     // Temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  ec = (rawEc / tempCoefficient) * ecCalibration;         // Temperature and calibration compensation
  tds = (133.42 * pow(ec, 3) - 255.86 * ec * ec + 857.39 * ec) * 0.5; // Convert voltage value to TDS value
  return tds;
}
void mode1(){
  pH = obtainpH();
  Serial.print("pH:");  
  Serial.println(pH,2);

  // Send the command to get temperatures
  sensors.requestTemperatures();
  // Read temperature from the sensor
  float temp = sensors.getTempCByIndex(0);
  // Call the TDS function with the current temperature
  TDS = readTdsQuick(temp);
  Serial.print("TDS: ");
  Serial.println(TDS);
}
void mode2(){
  if (flag==0) {
  buttonState = HIGH;
  if (buttonState == HIGH) {
    Movemotor(1,1,0,200);
    buttonState = digitalRead(buttonPin);
    //delay(MEASURE_DELAY);
    long distance = measure()+22;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
    Serial.println("flag0");

    // Check if the distance value is 400 three times
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
    if(buttonState == HIGH){
      Serial.println("button pressed");
      Movemotor(2,2,0,200); //go back
      flag = 1;
    }
  }
  
  delay(1000);
}

if (flag==1){
  buttonState = HIGH ;
  while (buttonState == HIGH) {
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
  buttonState = HIGH;
  if (buttonState == HIGH) {
  Movemotor(1, 1,0,200); 
  buttonState = digitalRead(buttonPin);
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
      buttonState = HIGH;
      while (buttonState == HIGH) {
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
  }
  if(buttonState == HIGH){
      Serial.println("button pressed");
      flag = 3;
    }
  delay(1000);
  }

  if (flag == 3){
    Movemotor(0, 0,0,200);
  }
}
void mode3(){
  mode1();
  mode2(); 
}
