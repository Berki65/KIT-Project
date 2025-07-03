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

bool answerTeamA = false;
bool answerTeamB = false;

// Store if answers matched for each round
bool answersMatched[10] = {false};

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
  roundCounter = 0;
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


  // 悲しいメロディ
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

void evalAnswerA(int score){
  scoreTeamA += score * questionValue[roundCounter];
  Serial.print("Team A score: ");
  Serial.println(scoreTeamA);
}

void evalAnswerB(int score){
  scoreTeamB += score * questionValue[roundCounter];
  Serial.print("Team B score: ");
  Serial.println(scoreTeamB);
}

void readAnswer(){
  int analogValueA = analogRead(A1); 
  if (analogValueA >950 && answerTeamA == false && analogValueA < 1023){
    evalAnswerA(2);
    buzzerAnswer(); // Play sound for answer evaluation
    answerTeamA = true;
  }else if (analogValueA < 750 && analogValueA > 500 && answerTeamA == false){
    evalAnswerA(1);
    answerTeamA = true;
    buzzerAnswer(); // Play sound for answer evaluation
  } else if (analogValueA < 500 && analogValueA > 350 && answerTeamA == false){
    evalAnswerA(0);
    answerTeamA = true;
    buzzerAnswer(); // Play sound for answer evaluation
  } else if (analogValueA < 350 && analogValueA > 200 && answerTeamA == false){
    evalAnswerA(-1);
    answerTeamA = true;
    buzzerAnswer(); // Play sound for answer evaluation
  } else if (analogValueA < 200 && analogValueA > 50 && answerTeamA == false){
    evalAnswerA(-2);
    answerTeamA = true;
    buzzerAnswer(); // Play sound for answer evaluation
  }

  int analogValueB = analogRead(A2);
  if (analogValueB >950 && analogValueB < 1023 && answerTeamB == false){
    evalAnswerB(2);
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
  } else if (analogValueB < 750 && analogValueB > 500 && answerTeamB == false){
    evalAnswerB(1);
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
  } else if (analogValueB < 500 && analogValueB > 350 && answerTeamB == false){
    evalAnswerB(0);
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
  } else if (analogValueB < 350 && analogValueB > 200 && answerTeamB == false){
    evalAnswerB(-1);
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
  } else if (analogValueB < 200 && analogValueB > 50 && answerTeamB == false){
    evalAnswerB(-2);
    answerTeamB = true;
    buzzerAnswer(); // Play sound for answer evaluation
  }
}


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
  // LedWaitStart();
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
      roundCounter = 0;
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
      answerTeamA = false;
      answerTeamB = false;
      // stateStartTime = millis(); // add this line to record start time
      gameState = WAIT_ANSWER;
      break;

case WAIT_ANSWER:
  readAnswer();
  // if ((answerTeamA && answerTeamB) || (millis() - stateStartTime > questionTime)) {
  //   gameState = SHOW_RESULT;
  // }
    if (answerTeamA && answerTeamB) {
      gameState = SHOW_QUESTION;
      answerTeamA = false; // Reset answers for next round
      answerTeamB = false; // Reset answers for next round
    }
  break;

    case SHOW_RESULT:
      // Store if answers matched for this round
      answersMatched[roundCounter] = (answerTeamA == answerTeamB);
      delay(500); // Short pause for display update
      roundCounter++;
      if (roundCounter >= totalRounds) {
        gameState = GAME_OVER;
      } else {
        gameState = SHOW_QUESTION;
      }
      break;

    case GAME_OVER:
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextSize(2); // Ensure readable font size
      tft.setCursor(0, 0);
      tft.print("Game Over");
      for (int i = 0; i < totalRounds; i++) {
        int y = 30 + i * 24; // 24 pixels per line for size 2
        tft.setCursor(0, y);
        tft.print("R"); tft.print(i+1); tft.print(": ");
        if (answersMatched[i]) {
          tft.print("Match");
        } else {
          tft.print("No Match");
        }
      }
      while (1); // Stop loop
      break;
  }
}


