#include <Arduino.h>

const int columnPins[] = {6, 7, 8, 9};
const int rowpins[] = {2, 3, 4, 5};
const int htpins[] = {10, 11, 12, 13};

bool rowpinvalue[4] = {false, false, false, false};
bool htpinvalue[4] = {true, true, true, true};

const int pwmResolution = 5;
const int pwmCycleDelay = 0;
const int maxBrightness = 5;

// LED position struct
struct LED {
  int row;      // 2-5
  int colPin;   // actual pin: 6–9
  int ht;       // 10-13
  float dist;   // distance from center
};

// ----------------------------------------
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

// ----------------------------------------
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

// ----------------------------------------
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
}

// ----------------------------------------
// LED list and distance bands
LED leds[64];
int bandCount = 6; // number of distance rings
int bandLEDCounts[6] = {0};        // How many LEDs in each band
LED* bands[6][64];                 // LED pointers per band

// ----------------------------------------
// Calculate distance bands
void buildWaveMap() {
  int idx = 0;
  for (int z = 0; z < 4; z++) {
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        float dx = x - 1.5;
        float dy = y - 1.5;
        float dz = z - 1.5;
        float dist = sqrt(dx * dx + dy * dy + dz * dz);

        LED& led = leds[idx++];
        led.row = rowpins[y];
        led.colPin = columnPins[x];
        led.ht = htpins[z];
        led.dist = dist;
      }
    }
  }

  // Assign to bands based on rounded distance
  for (int i = 0; i < 64; i++) {
    int band = round(leds[i].dist);  // 0 to 3
    if (band >= bandCount) band = bandCount - 1;
    bands[band][bandLEDCounts[band]++] = &leds[i];
  }
}

// ----------------------------------------
// Wave loop
void loop() {
  buildWaveMap();

  // Expand from center out
  for (int cycle = 0; cycle < 3; cycle++) {  // Repeat the explosion
    for (int dir = 0; dir < 2; dir++) {  // 0 = out, 1 = in
      for (int b = 0; b < bandCount; b++) {
        int band = dir == 0 ? b : bandCount - 1 - b;

        // Fade in this band
        for (int br = 0; br <= maxBrightness; br += 10) {
          for (int repeat = 0; repeat < 2; repeat++) {
            for (int i = 0; i < bandLEDCounts[band]; i++) {
              LED* led = bands[band][i];
              softwarePWM(led->row, led->colPin, led->ht, br);
            }
          }
        }

        // Hold
        for (int hold = 0; hold < 3; hold++) {
          for (int i = 0; i < bandLEDCounts[band]; i++) {
            LED* led = bands[band][i];
            softwarePWM(led->row, led->colPin, led->ht, maxBrightness);
          }
        }

        // Fade out
        for (int br = maxBrightness; br >= 0; br -= 10) {
          for (int repeat = 0; repeat < 2; repeat++) {
            for (int i = 0; i < bandLEDCounts[band]; i++) {
              LED* led = bands[band][i];
              softwarePWM(led->row, led->colPin, led->ht, br);
            }
          }
        }
      }
    }

    delay(5);  // Wait before next pulse
  }
}
