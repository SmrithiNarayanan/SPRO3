
#include <Vcc.h>

//MORE DECIMAL PLACES LEAD TO MUCH ACCURATE BATTERY READING(thus for VccMax and VccCorrection)
const float VccMin   = 0.0;           // Minimum expected Vcc level, in Volts.
const float VccMax   = 4.888;         // Maximum expected Vcc level, in Volts(Use a multimeter to measure the maximum output voltage of your regulator. Thus when you connect a fully charged battery to it).
const float VccCorrection = 0.9852;  // Tweak this value until the output of "v" on line 38 is the same as what you measured with your multimeter above for VccMax

const float lowBat = 10.5/2.65; //Indicate the value your battery manufacturer states as absolutely dead battery
const float fullBat = 12.7/2.65; //Indicate the value your fully charged battery will be at

float threshold = 3.98; // might change based on testing 


Vcc vcc(VccCorrection);

//BATTERY IS CONNECTED TO A2
int batt_pin = A2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //READ CURRENT REGULATOR OUTPUT VOLTAGE(VCC)
  //COLLECT 100 VCC VOLTAGE SAMPLES
  int j;
  float v = 0;
  for (j = 0; j < 100; j++) {
    v += vcc.Read_Volts();
    delay(5);
  }

  //FIND AVERAGE OF 100 VCC VOLTAGE SAMPLES
  v = v / j;

  //PRINTING AVERAGE OF 100 VCC VOLTAGE SAMPLES TO 3 DECIMAL PLACES
  Serial.println();
  Serial.print("VCC: ");
  Serial.println(v, 3);

  
  //COLLECT 100 BATTERY VOLTAGE SAMPLES
  int i;
  float batt_volt = 0;
  for (i = 0; i < 100; i++) {
    //USING THE CURRENT VCC VOLTAGE AS REFFERENCE TO CALCULATE BATTERY VOLTAGE
    batt_volt += ((analogRead(batt_pin) * (v / 1023.0)));
    delay(5);
  }

  //FIND AVERAGE OF 100 BATTERY VOLTAGE SAMPLES
  batt_volt = batt_volt / i;

  Serial.print("Battery: ");
  Serial.print(batt_volt, 2);
  Serial.print("V ");
  //CONVERTING BATTERY VOLTAGE TO PERCENTAGE ASSUMING 4.19V AS THE FULL CHARGE VALUE
  Serial.print(((batt_volt - lowBat )/ (fullBat - lowBat)) * 100);
  Serial.println("%");

  if (batt_volt < threshold) {
    Serial.println("Battery Low!");
  }

  delay(100);
  
}