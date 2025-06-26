// #include <Arduino.h>

// const int LED_MIDDLE = 9;
// const int LED_P_ONE = 10;
// const int LED_P_TWO = 11;

// const int INPUT_P_ONE = 3;
// const int INPUT_P_TWO = 4;
// const int INPUT_GAME_START = 2;

// unsigned int time = 0;
// unsigned int timeStart;

// bool gameStarted = false;
// bool reactionWindowOpen = false;



// void setup() {
//   pinMode(LED_MIDDLE, OUTPUT);
//   pinMode(LED_P_ONE, OUTPUT);
//   pinMode(LED_P_TWO, OUTPUT);

//   pinMode(INPUT_P_ONE, INPUT_PULLUP);
//   pinMode(INPUT_P_TWO, INPUT_PULLUP);
//   pinMode(INPUT_GAME_START, INPUT_PULLUP);

//   Serial.begin(9600);
//   randomSeed(analogRead(A0));
// }

// void randomTime() {
//   time = random(3000, 10000); 
//   Serial.print("Random time set to: ");
//   Serial.println(time);
// }

// void resetGame() {
//   digitalWrite(LED_MIDDLE, LOW);
//   digitalWrite(LED_P_ONE, LOW);
//   digitalWrite(LED_P_TWO, LOW);
//   time = 0;
//   timeStart = 0;

// }

// void startGame() {
//   resetGame();
//   randomTime();
//   Serial.println("Game started!");
//   delay(time);

//   digitalWrite(LED_MIDDLE, HIGH);
//   reactionWindowOpen = true;

// }


// void loop(){
//     if (!gameStarted && digitalRead(INPUT_GAME_START) == LOW) {
//     delay(50);
//     if (digitalRead(INPUT_GAME_START) == LOW) {
//       gameStarted = true;
//       startGame();
//     }
//   }

//   if (reactionWindowOpen) {
//     if (digitalRead(INPUT_P_ONE) == LOW) {
//       Serial.println("Player 1 wins!");
//       digitalWrite(LED_P_ONE, HIGH);
//       reactionWindowOpen = false;
//     } else if (digitalRead(INPUT_P_TWO) == LOW) {
//       Serial.println("Player 2 wins!");
//       digitalWrite(LED_P_TWO, HIGH);
//       reactionWindowOpen = false;
//     }
//   }
// }