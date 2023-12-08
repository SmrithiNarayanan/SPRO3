#include <ezButton.h>

ezButton button(6); 

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(50); // set debounce time to 100 milliseconds


}

void loop() {
  button.loop(); // MUST call the loop() function first
  if(button.isPressed()){
    Serial.println("The button is pressed");
  }
   
}
