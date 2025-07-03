#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// =============================
// 🖥 TFT Display Pin Definitions
// =============================
#define TFT_CS   8 // Chip Select pin for TFT
#define TFT_DC   10 // Data/Command pin for TFT
#define TFT_RST  9 // Reset pin for TFT

// =============================
// 💡 LED Pin Definitions
// =============================
#define LED_RGB_A_R 1 // Red LED pin for Team A
#define LED_RGB_A_G 2 // Green LED pin for Team A
#define LED_RGB_A_B 3 // Blue LED pin for Team A
#define LED_RGB_B_R 4 // Red LED pin for Team B
#define LED_RGB_B_G 5 // Green LED pin for Team B
#define LED_RGB_B_B 6 // Blue LED pin for Team B

uint8_t r,g,b,r2,g2,b2; // RGB values for LEDs

// =============================
// Buzzer Pin Definition
// =============================
#define BUZZER_PIN 7 // Buzzer pin for sound effects

// =============================
// 🔘 Button Pin Definition
// =============================
const int gameStartButton = 12; // Button to start the game

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST); // Create TFT display object

// =============================
// 🎮 Game Constants
// =============================
const int totalRounds = 5; // Change to 10 for full game


// =============================
// GameState Enum
// =============================
enum GameState { WAIT_START, SHOW_QUESTION, WAIT_ANSWER, SHOW_RESULT, GAME_OVER }; // Define game states
GameState gameState = WAIT_START; // Initialize game state
unsigned long stateStartTime = 0; // Variable to track time in current state
const unsigned long questionTime = 10000; // 10 seconds per question
const unsigned long resultTime = 2000;    // 2 seconds to show result


// =============================
// 📝 Questions Array
// =============================
String allQuestions[10] = {
  "How do you prefer to recharge your energy alone or with others?",
  "How do you make important decisions  based on logic or feelings?",
  "Do you prefer planning things in advance or staying flexible?",
  "What do you focus on more in life  facts and reality, or ideas and possibilities?",
  "When you're in conflict, do you want to talk immediately or take space first?",
  "After a Long day, do you feel best when you can spend time alone?",
  "⁠Do you usually think things through in your head before speaking?",
  "Do you prefer to observe and analyze before jumping into a discussion?",
  "Are you more interested in what could be than in what is?",
  "Do you tend to leave tasks until the last minute and then solve them creatively  "
};

int questionValue[10] ={2,1,4,2,4,5,1,5,5,2};

// =============================
// 🔧 Game Variables
// =============================
int roundCounter = 0;
int scoreTeamA = 0;
int scoreTeamB = 0;

int scoreTeamAButton = 0;
int scoreTeamBButton = 0;

int score = 0;

bool answerTeamA = false;
bool answerTeamB = false;


// =============================
// 🚀 Setup Function
// =============================
void setup() {
  // Start Serial for debugging
  Serial.begin(9600);
  Serial.println("Starting TFT");

  // Button setup
  pinMode(gameStartButton, INPUT_PULLUP); // Use internal pull-up resistor

  // Initialize TFT
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  Serial.println("TFT initialized");

  // Show initial screen
  tft.setCursor(0, 0);
  tft.print("Ready to start!");
  tft.setCursor(0, 30);
  tft.print("Press button to start");
  
  }
  

// =============================
// 🎮 Game Start Function
// =============================
void gameStart(){
  // Reset variables for new game if needed
  // roundCounter = 0;
  scoreTeamA = 0;
  scoreTeamB = 0;
}
void visualStart(){
  // Show start screen
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.print("Game Started!");
  delay(1000); // Show for a second
}
// =============================
// 💡 LED Functions
// =============================
void clearLED() {
  // Turn off all LEDs
  analogWrite(LED_RGB_A_R, 0);
  analogWrite(LED_RGB_A_G, 0);
  analogWrite(LED_RGB_A_B, 0);
  analogWrite(LED_RGB_B_R, 0);
  analogWrite(LED_RGB_B_G, 0);
  analogWrite(LED_RGB_B_B, 0);
}

void LedWaitStart() {
  // Simple RGB animation: cycle through colors for both teams
  static uint8_t phase = 0;
  static unsigned long lastUpdate = 0;
  const unsigned long interval = 30; // ms between color changes

  if (millis() - lastUpdate > interval) {
    lastUpdate = millis();
    phase++;
    // Animate Team A (RGB)
    r = (sin(phase * 0.07) * 127) + 128;
    g = (sin(phase * 0.07 + 2.1) * 127) + 128;
    b = (sin(phase * 0.07 + 4.2) * 127) + 128;
    // Animate Team B (RGB, phase shifted)
    r2 = (sin(phase * 0.07 + 1.05) * 127) + 128;
    g2 = (sin(phase * 0.07 + 3.15) * 127) + 128;
    b2 = (sin(phase * 0.07 + 5.25) * 127) + 128;

    analogWrite(LED_RGB_A_R, r);
    analogWrite(LED_RGB_A_G, g);
    analogWrite(LED_RGB_A_B, b);

    analogWrite(LED_RGB_B_R, r2);
    analogWrite(LED_RGB_B_G, g2);
    analogWrite(LED_RGB_B_B, b2); 
  }
}

void playTone(int freq, int duration) {
  tone(BUZZER_PIN, freq);  // 周波数で鳴らす
  delay(duration);         // 指定時間だけ待つ
  noTone(BUZZER_PIN);      // 音を止める
  delay(50);               // 区切りの無音時間
}

void ledTeamA() {
  // Team A LED: Green
  analogWrite(LED_RGB_A_R, 0);
  analogWrite(LED_RGB_A_G, 255);
  analogWrite(LED_RGB_A_B, 0);
}
void ledTeamB() {
  // Team B LED: Green
  analogWrite(LED_RGB_B_R, 0);
  analogWrite(LED_RGB_B_G, 255);
  analogWrite(LED_RGB_B_B, 0);
}

void DuringQuestion() {
  static uint8_t phase = 0;
  static unsigned long lastUpdate = 0;
  const unsigned long interval = 40;

  if (millis() - lastUpdate > interval) {
    lastUpdate = millis();
    phase++;
    
    r = (sin(phase * 0.05) * 127) + 128;
    g = (sin(phase * 0.05 + 2.0) * 127) + 128;
    b = (sin(phase * 0.05 + 4.0) * 127) + 128;

    r2 = (sin(phase * 0.05) * 127) + 128;
    g2 = (sin(phase * 0.05 + 2.0) * 127) + 128;
    b2 = (sin(phase * 0.05 + 4.0) * 127) + 128;

    analogWrite(LED_RGB_A_R, r);
    analogWrite(LED_RGB_A_G, g);
    analogWrite(LED_RGB_A_B, b);

    analogWrite(LED_RGB_B_R, r2);
    analogWrite(LED_RGB_B_G, g2);
    analogWrite(LED_RGB_B_B, b2);
  }
}

void afterAnswer() {
  
  analogWrite(LED_RGB_A_R, 0);
  analogWrite(LED_RGB_A_G, 0);
  analogWrite(LED_RGB_A_B, 255);

  analogWrite(LED_RGB_B_R, 0);
  analogWrite(LED_RGB_B_G, 0);
  analogWrite(LED_RGB_B_B, 255);

  playTone(880, 100); // 明るい確認音
  delay(150);
  clearLED();
}

void showGameOver() {

  analogWrite(LED_RGB_A_R, 255);
  analogWrite(LED_RGB_A_G, 0);
  analogWrite(LED_RGB_A_B, 0);

  analogWrite(LED_RGB_B_R, 255);
  analogWrite(LED_RGB_B_G, 0);
  analogWrite(LED_RGB_B_B, 0);

  // 悲しいメロデ
  playTone(440, 250);
  playTone(392, 250);
  playTone(349, 350);
  delay(100);
  playTone(330, 500);
  clearLED();

}

void setLEDGreen() {
  analogWrite(LED_RGB_A_R, 0);
  analogWrite(LED_RGB_A_G, 255);
  analogWrite(LED_RGB_A_B, 0);

  analogWrite(LED_RGB_B_R, 0);
  analogWrite(LED_RGB_B_G, 255);
  analogWrite(LED_RGB_B_B, 0);
}

void showRsults() {
  // 緑点灯
  setLEDGreen();

  // 明るくポジティブなメロディ
  playTone(659, 150);  // E5
  playTone(784, 150);  // G5
  playTone(880, 150);  // A5
  playTone(1047, 250); // C6
  delay(100);
  playTone(880, 200);
  playTone(1047, 300);

  // 
  clearLED();
}

// =============================

// =============================
// 🔊 Buzzer Functions
// =============================
void buzzerStart(){
  // Play a fast pitch-up sound on the buzzer
  for (int freq = 400; freq <= 1200; freq += 40) {
    tone(BUZZER_PIN, freq, 30);
    delay(20);
  }
  noTone(BUZZER_PIN);
}

void buzzerAnswer(){
  // Play a short sound for answer evaluation
  tone(BUZZER_PIN, 1000, 200); // 1kHz for 200ms
  delay(200);
  noTone(BUZZER_PIN);
}

// =============================
// 📝 Question Display and Evaluation Functions
// =============================
void showQuestion(int roundCounter){
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.println(allQuestions[roundCounter]);
  tft.setCursor(0, 200);
  tft.print("Input your answer!");
}

void checkAnswerButton(){
   if(answerTeamA && answerTeamB) {
    // Both teams answered, evaluate results
    if (scoreTeamAButton == scoreTeamBButton) {
      score += 10; // Both teams matched
      Serial.println("Both teams matched!");
    } else if (abs(scoreTeamAButton - scoreTeamBButton) == 1) {
      score += 8; // Close match
      Serial.println("Close match!");
    } else if (abs(scoreTeamAButton - scoreTeamBButton) == 2) {
      score += 5; // Moderate match
      Serial.println("Moderate match!");
    } else if (abs(scoreTeamAButton - scoreTeamBButton) == 3) {
      score += 3; // Distant match
      Serial.println("Distant match!");
    } else {
      score += 0; // No match
      Serial.println("No match!");
    }
    // Reset for next round
    scoreTeamAButton = 0;
    scoreTeamBButton = 0;
  }
}

void setAnswerButtons(bool isA, int score) {

  if(isA){
    scoreTeamAButton += score;
  }
  else {
    scoreTeamBButton += score;
  }
}

void readAnswer(){
  int analogValueA = analogRead(A1); 
  if (analogValueA >950 && answerTeamA == false && analogValueA < 1023){
    buzzerAnswer(); // Play sound for answer evaluation
    answerTeamA = true;
    setAnswerButtons(true, 1);
    checkAnswerButton(); 
    ledTeamA(); // Turn on Team A LED
  }else if (analogValueA < 750 && analogValueA > 500 && answerTeamA == false){
    answerTeamA = true;
    buzzerAnswer(); // Play sound for answer evaluation
    setAnswerButtons(true, 2);
    checkAnswerButton(); 
    ledTeamA(); // Turn on Team A LED
  } else if (analogValueA < 500 && analogValueA > 350 && answerTeamA == false){
    answerTeamA = true;
    buzzerAnswer(); // Play sound for answer evaluation
    setAnswerButtons(true, 3);
    checkAnswerButton(); 
    ledTeamA(); // Turn on Team A LED
  } else if (analogValueA < 350 && analogValueA > 200 && answerTeamA == false){
    answerTeamA = true;
    buzzerAnswer(); // Play sound for answer evaluation
    setAnswerButtons(true, 4);
    checkAnswerButton(); 
    ledTeamA(); // Turn on Team A LED
  } else if (analogValueA < 200 && analogValueA > 50 && answerTeamA == false){
    answerTeamA = true;
    buzzerAnswer(); // Play sound for answer evaluation
    setAnswerButtons(true, 5);
    checkAnswerButton(); 
    ledTeamA(); // Turn on Team A LED
  }

  int analogValueB = analogRead(A2);
  if (analogValueB >950 && analogValueB < 1023 && answerTeamB == false){
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
    setAnswerButtons(false, 5);
    checkAnswerButton(); 
    ledTeamB(); // Turn on Team B LED
  } else if (analogValueB < 750 && analogValueB > 500 && answerTeamB == false){
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
    setAnswerButtons(false, 4);
    checkAnswerButton(); 
    ledTeamB(); // Turn on Team B LED
  } else if (analogValueB < 500 && analogValueB > 350 && answerTeamB == false){
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
    setAnswerButtons(false, 3); 
    checkAnswerButton(); 
    ledTeamB(); // Turn on Team B LED
  } else if (analogValueB < 350 && analogValueB > 200 && answerTeamB == false){
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
    setAnswerButtons(false, 2); 
    checkAnswerButton(); 
    ledTeamB(); // Turn on Team B LED
  } else if (analogValueB < 200 && analogValueB > 50 && answerTeamB == false){
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
    setAnswerButtons(false, 1); 
    checkAnswerButton(); 
    ledTeamB(); // Turn on Team B LED
  }
}

// void scoreInput(int buttonA, int buttonB) {
//   if (buttonA == buttonB){
//      score += 10;
//   }
//   else if(buttonA-buttonB == 1 || buttonA-buttonB == -1){
//      score += 8;
//   }
//   else if(buttonA-buttonB == 2 || buttonA-buttonB == -2){
//      score += 5;
//   }
//   else if(buttonA-buttonB == 3 || buttonA-buttonB == -3){
//      score += 3;
//   }
//   else if(buttonA-buttonB == 4 || buttonA-buttonB == -4){
//      score += 0;
//   }
// }

void evaluateResult(){
  // Display scores
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.print("Team A: ");
  tft.print(scoreTeamA);
  tft.setCursor(0, 30);
  tft.print("Team B: ");
  tft.print(scoreTeamB);

  // Wait for a while before next round
  delay(2000);
  
  // Reset answers for next round
  answerTeamA = false;
  answerTeamB = false;

  // Increment round counter
  roundCounter++;
}
// =============================

// =============================
// 🔄 Main Loop
// =============================
void loop() {

  switch (gameState) {
  case WAIT_START: {
  static bool drawn = false;
  static unsigned long buttonPressStart = 0;
  static bool buttonHeld = false;
  // setEDGreen();
  
  LedWaitStart();
  if (!drawn) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.print("Ready to start!");
    tft.setCursor(0, 30);
    tft.print("Hold button 1s to start");
    drawn = true;
  }

  int reading = digitalRead(gameStartButton);
  if (reading == HIGH) {
    if (!buttonHeld) {
      buttonPressStart = millis();
      buttonHeld = true;
    } else if (millis() - buttonPressStart >= 1000) {
      Serial.println("✅ Button held for 1 second, starting game!");
      gameStart();
      clearLED(); // Clear LEDs.     
      buzzerStart(); // Play start sound
      // roundCounter = 0; 
      gameState = SHOW_QUESTION;
      drawn = false;
      buttonHeld = false;
      // Wait for button release to avoid accidental retrigger
      while (digitalRead(gameStartButton) == LOW) { delay(10); }
    }
  } else {
    buttonHeld = false;
  }
  break;
}

case SHOW_QUESTION:
  showQuestion(roundCounter);
  clearLED();
  answerTeamA = false;
  answerTeamB = false;
  scoreTeamAButton = 0;
  scoreTeamBButton = 0;
  gameState = WAIT_ANSWER;
  break;

case WAIT_ANSWER:
  readAnswer();
  if (answerTeamA && answerTeamB) {
    roundCounter++;
    if (roundCounter >= totalRounds) {
      gameState = GAME_OVER;
    } else {
      gameState = SHOW_QUESTION;
    }
    answerTeamA = false;
    answerTeamB = false;
  }
  break;

    case SHOW_RESULT:
      // Store if answers matched for this round
      // answersMatched[roundCounter] = (answerTeamA == answerTeamB);
      tft.fillScreen(ILI9341_BLACK);
      delay(500); // Short pause for display update
        answerTeamA = false; // Reset answers for next round
        answerTeamB = false; // Reset answers for next round
      if (roundCounter >= totalRounds) {
        gameState = GAME_OVER;
      } else {
        gameState = SHOW_QUESTION;
        answerTeamA = false; // Reset answers for next round
        answerTeamB = false; // Reset answers for next round
      }
      break;

    case GAME_OVER:
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextSize(2); // Ensure readable font size
      tft.setCursor(0, 0);
      Serial.println(score);
      if(score >= 35){
        tft.print("You two are a matching!");
        tft.setCursor(0, 30);
        tft.print("Your score: ");
        tft.print(score);
        while(1);
      }
      else{
        tft.print("You two are not a matching!");
        while(1);
      }

  }
}

