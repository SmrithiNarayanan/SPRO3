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

//flag variables
int flag=0;


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
  if (flag==0) {
    Movemotor(1, 1,0,200);
    Serial.println("flag0");
    
    //delay(MEASURE_DELAY);
    long distance = measure()+22;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");

    // Check if the distance value is 200 four times
    static int counter_0 = 0;
    if (distance <= 300) {
      counter_0++;
      if (counter_0 == 4) {
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
    Serial.println("flag1");
    Movemotor(1, 1,1000,200); //change delay after testing
    Movemotor(1, 0,2000,200); //change delay after testing 
    static int counter_1 = 0;
    // check distance 4 times 
    for(int i=0;i<5;i++){
      //Start sensor detection again
      long distance = measure()+22;
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" mm");
      //increment counter 
      if (distance > 300) {
        counter_1++; 
    }
  }
  if(counter_1>=2){
    Movemotor(1, 1,1000,200); //change delay after testing
    Movemotor(1, 0,2000,200); //change delay after testing
    flag = 0;
  }
  if(counter_1<2){
    flag=2;
    Serial.println("flag2");
  }

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
