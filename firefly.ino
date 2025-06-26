#include <Arduino.h>

// Pin setup
const int columnPins[] = {6, 7, 8, 9};
const int rowpins[] = {2, 3, 4, 5};
const int htpins[] = {10, 11, 12, 13};

bool rowpinvalue[4] = {false, false, false, false};
bool htpinvalue[4] = {true, true, true, true};

const int pwmResolution = 10;
const int pwmCycleDelay = 2;

const int NUM_FIREFLIES = 7;
const int MAX_BRIGHTNESS = 100;

// ----------------------------
// Firefly Struct
struct Firefly {
  int row, colPin, ht;
  int brightness;
  int fadeDir;     // 1 = fade in, -1 = fade out
  bool active;
};

Firefly fireflies[NUM_FIREFLIES];

// ----------------------------
// ustadd (same as before)
void ustadd(int row, int col, int ht, bool bull) {
  for (int i = 0; i < 4; i++) {
    rowpinvalue[i] = false;
    htpinvalue[i] = true;
  }

  rowpinvalue[row - 2] = bull;
  for (int i = 0; i < 4; i++) {
    digitalWrite(rowpins[i], rowpinvalue[i]);
  }

  digitalWrite(col, LOW);
  digitalWrite(col, HIGH);
  digitalWrite(col, LOW);

  htpinvalue[ht - 10] = !bull;
  for (int i = 0; i < 4; i++) {
    digitalWrite(htpins[i], htpinvalue[i]);
  }
}

// ----------------------------
// softwarePWM for one LED
void softwarePWM(int row, int col, int ht, int brightnessLevel) {
  for (int i = 0; i < pwmResolution; i++) {
    if (i < brightnessLevel) {
      ustadd(row, col, ht, true);
    } else {
      ustadd(row, col, ht, false);
    }
    delayMicroseconds(pwmCycleDelay);
  }
  ustadd(row, col, ht, false);
}

// ----------------------------
// Setup
void setup() {
  int allPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
  for (int i = 0; i < 8; i++) {
    pinMode(allPins[i], OUTPUT);
    digitalWrite(allPins[i], LOW);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(htpins[i], OUTPUT);
    digitalWrite(htpins[i], HIGH);
  }

  randomSeed(analogRead(A0));

  // Initialize fireflies
  for (int i = 0; i < NUM_FIREFLIES; i++) {
    fireflies[i].active = false;
  }
}

// ----------------------------
// Loop
void loop() {
  // Maybe activate a new firefly
  for (int i = 0; i < NUM_FIREFLIES; i++) {
    if (!fireflies[i].active && random(0, 100) < 10) { // 10% chance to spawn
      fireflies[i].row = random(2, 6);
      fireflies[i].colPin = columnPins[random(0, 4)];
      fireflies[i].ht = random(10, 14);
      fireflies[i].brightness = 0;
      fireflies[i].fadeDir = 1;
      fireflies[i].active = true;
    }
  }

  // Show all active fireflies
  for (int cycle = 0; cycle < 5; cycle++) {  // Do short PWM cycles for smooth blending
    for (int i = 0; i < NUM_FIREFLIES; i++) {
      if (fireflies[i].active) {
        softwarePWM(
          fireflies[i].row,
          fireflies[i].colPin,
          fireflies[i].ht,
          fireflies[i].brightness
        );
      }
    }
  }

  // Update brightness and deactivate if needed
  for (int i = 0; i < NUM_FIREFLIES; i++) {
    if (fireflies[i].active) {
      fireflies[i].brightness += fireflies[i].fadeDir * 2;  // Fading speed

      if (fireflies[i].brightness >= MAX_BRIGHTNESS) {
        fireflies[i].brightness = MAX_BRIGHTNESS;
        fireflies[i].fadeDir = -1;
      }

      if (fireflies[i].brightness <= 0) {
        fireflies[i].brightness = 0;
        fireflies[i].active = false;
      }
    }
  }

  delay(10);  // Small delay to control animation speed
}

