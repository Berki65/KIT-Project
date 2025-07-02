// #include <Arduino.h>

// // Define analog input pins
// const int analogPin1 = A1;
// const int analogPin2 = A2;

// // Reference voltage for Arduino (typically 5.0V or 3.3V)
// const float referenceVoltage = 5.0;

// void setup() {
//     Serial.begin(9600);
// }

// void loop() {
//     int rawValue1 = analogRead(analogPin1);
//     int rawValue2 = analogRead(analogPin2);

//     // Convert the analog reading (0-1023) to voltage
//     float voltage1 = (rawValue1 / 1023.0) * referenceVoltage;
//     float voltage2 = (rawValue2 / 1023.0) * referenceVoltage;

//     Serial.print("Channel 1 Voltage: ");
//     Serial.print(voltage1, 3);
//     Serial.print(" V\t");

//     Serial.print("Channel 2 Voltage: ");
//     Serial.print(voltage2, 3);
//     Serial.println(" V");

//     delay(1000); // Wait 1 second before next reading
// }