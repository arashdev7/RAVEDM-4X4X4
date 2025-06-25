const int rowpins[] = {2, 3, 4, 5};
const int columnPins[] = {6, 7, 8, 9};
const int htpins[] = {10, 11, 12, 13};

struct Drop {
  int row;
  int col;
  int heightIndex;  // 0 to 3 (from top to bottom)
  bool active;
};

const int MAX_DROPS = 4;
Drop drops[MAX_DROPS];

void ustadd(int row, int col, int ht, bool bull) {
  bool rowpinvalue[4] = {false, false, false, false};
  bool htpinvalue[4] = {true, true, true, true};

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

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(rowpins[i], OUTPUT);
    pinMode(columnPins[i], OUTPUT);
    pinMode(htpins[i], OUTPUT);
    digitalWrite(rowpins[i], LOW);
    digitalWrite(columnPins[i], LOW);
    digitalWrite(htpins[i], HIGH);
  }

  randomSeed(analogRead(A0));

  for (int i = 0; i < MAX_DROPS; i++) {
    drops[i].active = false;
  }
}

void loop() {
  // Try to activate a new drop
  for (int i = 0; i < MAX_DROPS; i++) {
    if (!drops[i].active && random(0, 10) < 2) {  // 20% chance per loop
      drops[i].row = rowpins[random(0, 4)];
      drops[i].col = columnPins[random(0, 4)];
      drops[i].heightIndex = 0;  // Start at top
      drops[i].active = true;
      break;
    }
  }

  // Clear cube
  for (int r = 2; r <= 5; r++) {
    for (int c = 0; c < 4; c++) {
      for (int h = 4; h > 0; h--) {
        ustadd(r, columnPins[c], htpins[h], false);
      }
    }
  }

  // Draw active drops
  for (int i = 0; i < MAX_DROPS; i++) {
    if (drops[i].active) {
      ustadd(drops[i].row, drops[i].col, htpins[drops[i].heightIndex], true);
      drops[i].heightIndex++;
      if (drops[i].heightIndex >= 4) {
        drops[i].active = false;  // Remove drop at bottom
      }
    }
  }

  delay(150);  // Adjust fall speed
}
