// #include <Arduino.h>

// // #define LED_RED 1
// // #define LED_GREEN 2
// // #define LED_BLUE 3
// // uint8_t r, g, b;
// // uint8_t i;


// // #define BUZZER 9

//   const int LED = 9;
//   int val = 0;  

//   const int SENSOR = 0; 


// void setup() {
//   // pinMode(LED_RED, OUTPUT);
//   // pinMode(LED_GREEN, OUTPUT);
//   // pinMode(LED_BLUE, OUTPUT);


//   // pinMode(BUZZER, OUTPUT);

//   pinMode(LED, OUTPUT);
//   Serial.begin(9600);

// }

// // // Play sound
// // void play(int freq, int dur) {
// //   tone(BUZZER, freq);
// //   delay(dur);
// //   noTone(BUZZER);
// //   delay(80); // kurze Pause
// // }

// void loop() {
//   // // // // First code snippet
//   // for(i=0; ; i++){
//   //   r = (sin(2 * 3.14 / 256 * i) + 1) * 128;
//   //   g = (sin(2 * 3.14/256 * (i + 85) + 1) * 128);
//   //   b = (sin(2 * 3.14/256*(i+170) + 1) * 128);
 
//   //   analogWrite(LED_RED, r);
//   //   analogWrite(LED_GREEN, g);
//   //   analogWrite(LED_BLUE, b);
//   //   delay(10);

//   // }

//   // // // BUZZER

//   // // PART 1
//   // play(330, 400);  // E4
//   // play(392, 400);  // G4
//   // play(440, 400);  // A4
//   // play(330, 400);  // E4
//   // play(392, 400);  // G4
//   // play(494, 400);  // B4
//   // play(523, 400);  // C5
//   // play(494, 400);  // B4
//   // play(440, 600);  // A4
//   // delay(400);

//   // // PART 2
//   // play(330, 400);  // E4
//   // play(392, 400);  // G4
//   // play(440, 400);  // A4
//   // play(330, 400);  // E4
//   // play(392, 400);  // G4
//   // play(494, 400);  // B4
//   // play(523, 400);  // C5
//   // play(494, 400);  // B4
//   // play(440, 600);  // A4
//   // delay(400);

//   // // PART 3 (kleine Variation)
//   // play(392, 400);  // G4
//   // play(440, 400);  // A4
//   // play(494, 400);  // B4
//   // play(392, 400);  // G4
//   // play(523, 400);  // C5
//   // play(587, 400);  // D5
//   // play(659, 400);  // E5
//   // play(587, 400);  // D5
//   // play(523, 600);  // C5


//   // val = analogRead(A0);
//   // analogWrite(LED, val / 4); 
//   // delay(100); 


//   val = analogRead(SENSOR);
//   Serial.println(val);
//   delay(100);


// }
