#include "HX7
11.h"


// Define HX711 Pins
const int DT_PIN = 5;
const int SCK_PIN = 6;


// Define LED Pins
const int redPin = 2;
const int yellowPin = 3;
const int greenPin = 4;


// Initialize HX711 object
HX711 scale;


// Set your weight thresholds (in grams or raw units)
const float LOW_THRESHOLD = 0.1;  // Below this = Red
const float HIGH_THRESHOLD = 0.4; // Above this = Green


// Calibration factor (adjust after running test code)
float calibration_factor = 8400.0;


void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }


  // Setup LED pins
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);


  // Setup HX711
  scale.begin(DT_PIN, SCK_PIN);
 
  // Reset load cell to 0 on startup (Tare)
  Serial.println("Taring scale... Do not touch load cell.");
  scale.set_scale(calibration_factor);
  scale.tare();
  Serial.println("Scale Ready!");
}


void loop() {
  // Read weight (average of 5 readings for stability)
  float weight = scale.get_units(5);


  Serial.print("Weight: ");
  Serial.println(weight);


  // Evaluate weight threshold and change LEDs
  if (weight > HIGH_THRESHOLD) {
    // Green ON (Light weight)
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, HIGH);
  }
  else if (weight >= LOW_THRESHOLD && weight < HIGH_THRESHOLD) {
    // Yellow ON (Medium weight)
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
  else {
    // Red ON (Heavy weight / Limit reached)
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
  }


  delay(200); // Small delay between readings
}

