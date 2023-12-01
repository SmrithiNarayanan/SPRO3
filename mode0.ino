//mode 0

// define sensor connections
#define TRIGPIN 10
#define ECHOPIN 11 

// floats to calculte dist
float duration, dist;

void setup() {
 //serial monitor set up 
Serial.begin(115200);

//set pinmode for sensor connections
pinMode(ECHOPIN, INPUT);
pinMode(TRIGPIN, OUTPUT); 
}

void loop() {
 //set trigger pin low for 2us
 digitalWrite (TRIGPIN, LOW);
 delayMicroseconds(2);

 //set trigger pin high for 2us (send pulse)
digitalWrite(TRIGPIN, HIGH);
delayMicroseconds(20);

//return the trigger pin to low 
 digitalWrite(TRIGPIN, LOW);

 //Measure the width of the  incoming pulse
 duration= pulseIn(ECHOPIN,HIGH);

 //determine distance from duration
 //Use 343 meters per second as speed of sound
 //Divide by 1000 as we want millimeters

dist = (duration/2)*0.343;

//print result 
Serial.print("distance: ");
Serial.print(dist);
Serial.print(" mm ");

//Delay before repeating measurement
delay(1000);

}
