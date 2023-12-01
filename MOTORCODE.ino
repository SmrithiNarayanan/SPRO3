//Motors on (5secs) and off (1sec)

// Motor A

int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor B

int enB = 3;
int in3 = 5;
int in4 = 4;

void setup()

{
  // Set all the motor control pins to outputs

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

}
void loop()

{
	  digitalWrite(in1, LOW);
	  digitalWrite(in2, HIGH);  
	  digitalWrite(in3, LOW);
	  digitalWrite(in4, HIGH);
		analogWrite(enA, 200);
		analogWrite(enB, 200);
		delay(5000;
	  digitalWrite(in1, LOW);
	  digitalWrite(in2, LOW);  
	  digitalWrite(in3, LOW);
	  digitalWrite(in4, LOW);
		delay(1000);
}