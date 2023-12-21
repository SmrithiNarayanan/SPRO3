// Motor A
int enA = 3;
int in1 = 2;
int in2 = 4;

// Motor B
int enB = 5;
int in3 = 6;
int in4 = 7;

String command = "";            	//Variable for storing received data

// Flag variables
int flag = 0;

void setup() {
    Serial.begin(9600); // Serial monitoring
     // Set all the motor control pins to outputs
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

}
void loop(){
    if(Serial.available() > 0){// Send data only when you receive data:
command = Serial.readString();  	//Read the incoming data and store it into variable data
if(command == "1"){        	//Checks whether value of data is equal to 1 
   Movemotor(1,1,0,200);      
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




