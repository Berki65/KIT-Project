#include <Arduino.h>

// put function declarations here:
const int LED_RED = 5;
// const int BUTTON = 7; 
int val = 0;
int i = 0;



void setup() {
  pinMode(LED_RED, OUTPUT);
  // pinMode(BUTTON, INPUT);
}



void loop() {


  // // // LED SWITCH
  
  
  // // OUR SOLUTION
  // digitalWrite(LED_RED, LOW);
  
  // if (digitalRead(BUTTON)){
  //   digitalWrite(LED_RED, HIGH);
  // }

  // // Professor

  // val = digitalRead(BUTTON);

  // if(val == HIGH){
  //   digitalWrite(LED_RED, HIGH);
  // }
  // else{
  //   digitalWrite(LED_RED, LOW);
  // }

  // // // PWM

  // // OUR SOLUTION

  // for(i = 0; i < 255; i++){
  //   analogWrite(LED_RED, i);
  //   delay(10);
  // }
  // for(i = 255; i > 0; i--){

  //   analogWrite(LED_RED, i);
  //   delay(10);  
  // }



}
