// #include <Arduino.h>

// //LED 
// const int LED_MIDDLE = 9;
// const int LED_P_ONE = 10;
// const int LED_P_TWO = 11;

// // BUTTONS
// const int INPUT_P_ONE = 6;
// const int INPUT_P_TWO = 5;
// const int INPUT_GAME_START = 7;

// // VARIABLES
// unsigned int time = 0;

// // GAME STATE
// bool reactionWindowOpen = false;


// // Setup function to initialize pins and serial communication
// void setup() {
//   pinMode(LED_MIDDLE, OUTPUT);
//   pinMode(LED_P_ONE, OUTPUT);
//   pinMode(LED_P_TWO, OUTPUT);

//   pinMode(INPUT_P_ONE, INPUT_PULLUP);
//   pinMode(INPUT_P_TWO, INPUT_PULLUP);
//   pinMode(INPUT_GAME_START, INPUT_PULLUP);

//   Serial.begin(9600);
// }

// // Calculate a random time between 3 and 10 seconds
// void randomTime() {
//   time = random(3000, 10000); 
//   Serial.print("Random time set to: ");
//   Serial.println(time);
// }

// // Reset the game state
// void resetGame() {
//   digitalWrite(LED_MIDDLE, LOW);
//   digitalWrite(LED_P_ONE, LOW);
//   digitalWrite(LED_P_TWO, LOW);
//   time = 0;
//   reactionWindowOpen = false;

// }

// // Start the game by resetting the state and setting a random time
// void startGame() {
//   resetGame();
//   randomTime();
//   Serial.println("Game started!");
//   delay(time);

//   digitalWrite(LED_MIDDLE, HIGH);
//   reactionWindowOpen = true;

// }

// // Main loop function to handle game logic
// void loop(){

//   // Check if the game start button is pressed
//   if(digitalRead(INPUT_GAME_START)){
//     resetGame();
//     startGame();
//   }
  
//   // If the reaction window is open, check for player reactions
//   if(reactionWindowOpen){
//     if(digitalRead(INPUT_P_ONE) == HIGH){
//       digitalWrite(LED_P_ONE, HIGH);
//       reactionWindowOpen = false;
//       Serial.println("Player 1 reacted!");
//       delay(3000);
//       resetGame();
//       startGame();
//     }
//     else if(digitalRead(INPUT_P_TWO) == HIGH){
//       digitalWrite(LED_P_TWO, HIGH);
//       reactionWindowOpen = false;
//       Serial.println("Player 2 reacted!");
//       delay(3000);
//       resetGame();
//       startGame();
//     }
//   }

// }