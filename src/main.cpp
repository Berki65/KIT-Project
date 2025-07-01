#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// =============================
// 🖥 TFT Display Pin Definitions
// =============================
#define TFT_CS   8
#define TFT_DC   10
#define TFT_RST  9

// =============================
// 💡 LED Pin Definitions
// =============================
const int LED_RGB_A_R = 1; // Red LED pin for Team A
const int LED_RGB_A_G = 2; // Green LED pin for Team A
const int LED_RGB_A_B = 3; // Blue LED pin for Team A

const int LED_RGB_B_R = 4; // Red LED pin for Team B
const int LED_RGB_B_G = 5; // Green LED pin for Team B
const int LED_RGB_B_B = 6; // Blue LED pin for Team B

// =============================
// 🔘 Button Pin Definition
// =============================
const int gameStartButton = 12; // Button to start the game

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// =============================
// 🎮 Game Constants
// =============================
const int totalRounds = 5; // Change to 10 for full game


// =============================
// 📝 Questions Array
// =============================
String allQuestions[10] = {
  "Question 1",
  "Question 2",
  "Question 3",
  "Question 4",
  "Question 5",
  "Question 6",
  "Question 7",
  "Question 8",
  "Question 9",
  "Question 10"
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

// =============================
// 🚀 Setup Function
// =============================
void setup() {
  // Start Serial for debugging
  Serial.begin(9600);
  Serial.println("Starting TFT");

  // Initialize TFT
  tft.begin();
  tft.setRotation(1);
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

void showQuestion(int roundCounter){
  // Show current question
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.print(allQuestions[roundCounter]);
  tft.setCursor(0, 30);
  tft.print("Input your answer!");
}

void evalAnswerA(int score){
  scoreTeamA += score * questionValue[roundCounter];
}

void evalAnswerB(int score){
  scoreTeamB += score * questionValue[roundCounter];
}

void evalAnswer(int score, bool isTeamA, bool isTeamB){
  if(isTeamA){
    evalAnswerA(score);
  } else {
    evalAnswerB(score);
  }
}

void readAnswer(){
  int analogValueA = analogRead(A0); 
  if (analogValueA >950){
    evalAnswerA(2);
    answerTeamA = true;
  }else if (analogValueA < 750 && analogValueA > 500){
    evalAnswerA(1);
    answerTeamA = true;
  } else if (analogValueA < 500 && analogValueA > 350){
    evalAnswerA(0);
    answerTeamA = true;
  } else if (analogValueA < 350 && analogValueA > 200){
    evalAnswerA(-1);
    answerTeamA = true;
  } else if (analogValueA < 200 && analogValueA > 50){
    evalAnswerA(-2);
    answerTeamA = true;
  }

  int analogValueB = analogRead(A1);
  if (analogValueB >950){
    evalAnswerB(2);
    answerTeamB = true;
  } else if (analogValueB < 750 && analogValueB > 500){
    evalAnswerB(1);
    answerTeamB = true;
  } else if (analogValueB < 500 && analogValueB > 350){
    evalAnswerB(0);
    answerTeamB = true;
  } else if (analogValueB < 350 && analogValueB > 200){
    evalAnswerB(-1);
    answerTeamB = true;
  } else if (analogValueB < 200 && analogValueB > 50){
    evalAnswerB(-2);
    answerTeamB = true;
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
// 🔄 Main Loop
// =============================
void loop() {
  if(roundCounter < totalRounds){
    // Display current question
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.print(allQuestions[roundCounter]);

    // Process inputs (example: give points to both)
    if(answerTeamA) scoreTeamA++;
    if(answerTeamB) scoreTeamB++;

    // Debug print
    Serial.print("Round "); Serial.print(roundCounter + 1);
    Serial.print(" | TeamA: "); Serial.print(scoreTeamA);
    Serial.print(" | TeamB: "); Serial.println(scoreTeamB);

    // Next round
    roundCounter++;
    delay(1000); // Delay before next question
  } 
  else {
    // Game finished
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.print("Game Over");
    tft.setCursor(0, 30);
    tft.print("Team A: "); tft.print(scoreTeamA);
    tft.setCursor(0, 60);
    tft.print("Team B: "); tft.print(scoreTeamB);

    while(1); // Stop loop
  }
}