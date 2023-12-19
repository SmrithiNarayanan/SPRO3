#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 9 //TDS 

String command = "";            	//Variable for storing received data
#define SensorPin 0          // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
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


void setup() 
{
Serial.begin(9600);     	//Sets the data rate in bits per second (baud) for serial data transmission
pinMode(13, OUTPUT);    	//Sets digital pin 13 as output pin
}
void loop()
{
if(Serial.available() > 0)  // Send data only when you receive data:
{
command = Serial.readString();  	//Read the incoming data and store it into variable data
if(command == "1")        	//Checks whether value of data is equal to 1 
mode1();
}                           
}

void obtainPH () {
for(int i=0;i<10;i++)     //Get 10 sample value from the sensor for smooth the value
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
String stringValue = String(phValue);
Serial.print("PH value and TDS are: ");
Serial.println(stringValue);
digitalWrite(13, HIGH);   
delay(800);
digitalWrite(13, LOW);
}

unsigned int readTdsQuick(float temp)
{
  float rawEc = analogRead(tds_sensor) * aref / 1024.0; // Read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float tempCoefficient = 1.0 + 0.02 * (temp - 25.0);     // Temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  ec = (rawEc / tempCoefficient) * ecCalibration;         // Temperature and calibration compensation
  tds = (133.42 * pow(ec, 3) - 255.86 * ec * ec + 857.39 * ec) * 0.5; // Convert voltage value to TDS value
  //return tds;
}

void mode1(){            //mode 1 = ph and TDS
  obtainPH();
  // Send the command to get temperatures
  sensors.requestTemperatures();
  // Read temperature from the sensor
  float temp = sensors.getTempCByIndex(0);
  // Call the TDS function with the current temperature
  TDS = readTdsQuick(temp);
  String stringTDSValue = String(TDS);
  //Serial.print("TDS: ");
  Serial.println(stringTDSValue);
}

