//Motor function
//0 off
//1 forward
//2 backward
//delay in milliseconds

// Motor A
int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor B
int enB = 3;
int in3 = 5;
int in4 = 4;

#define SensorPin 0          // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

int recieved_data = 0;            	//Variable for storing received data
int send_data = 0;                     // data to send back to app

void setup() {
  Serial.begin(9600); // Serial monitoring
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(13,OUTPUT);  
  
}

void loop()
{
if(Serial.available() > 0)  // Send data only when you receive data:
{
recievied_data = Serial.read();  	//Read the incoming data and store it into variable data
Serial.print(recievied_data);    	//Print Value inside data in Serial monitor
Serial.print("n");    	          //New line 
if (recieved_data == '0') {       //stop mode
Movemotor(0,0,3000,200); 
}

if (recievied_data == '1')        // mainatance mode 
{
measurePH();                             //turn on PH sensor
}

if(recievied_data == '2') {       	//clean mode 
Movemotor(1,1,3000,200);          //move both motors forward
Movemotor(1,2,3000,200);          //move motor a forward and motor b backward
Movemotor(1,0,3000,200);          //move motor a forward and motor b off
}
if(recieved_data == '3'){         //mode maintace and clean 
  Movemotor(1,1,3000,200);          //move both motors forward
  Movemotor(1,2,3000,200);          //move motor a forward and motor b backward
  Movemotor(1,0,3000,200);          //move motor a forward and motor b off
  measurePH();

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

void measurePH () {
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
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  digitalWrite(13, HIGH);   	
  delay(800);
  digitalWrite(13, LOW);
}
