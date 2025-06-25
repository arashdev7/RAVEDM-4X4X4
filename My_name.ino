const int rowpins[] = {2, 3, 4, 5};
const int columnPins[] = {6, 7, 8, 9};
const int htpins[] = {10, 11, 12, 13}; // 13 is top layer, 10 is bottom

// 4x4 font for A, R, A, S, H (each is 4 rows of 4 bits)
byte letters[][4] = {
  // A
  {
    B0110,
    B1001,
    B1111,
    B1001
  },
  // R
  {
    B1110,
    B1001,
    B1110,
    B1001
  },
  // A
  {
    B0110,
    B1001,
    B1111,
    B1001
  },
  // S
  {
    B0111,
    B0100,
    B0010,
    B1110
  },
  // H
  {
    B1001,
    B1001,
    B1111,
    B1001
  }
};

const int numLetters = sizeof(letters) / sizeof(letters[0]);

// Control a single LED
void ustadd(int row, int col, int ht, bool on) {
  // Turn off all rows and heights first
  for (int i = 0; i < 4; i++) {
    digitalWrite(rowpins[i], LOW);
    digitalWrite(htpins[i], HIGH); // HIGH = off
  }

  // Turn on the selected row
  digitalWrite(row, HIGH);

  // Pulse the column
  digitalWrite(col, LOW);
  digitalWrite(col, HIGH);
  digitalWrite(col, LOW);

  // Enable the height (layer)
  digitalWrite(ht, LOW);  // LOW = ON
}

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(rowpins[i], OUTPUT);
    pinMode(columnPins[i], OUTPUT);
    pinMode(htpins[i], OUTPUT);
    digitalWrite(rowpins[i], LOW);
    digitalWrite(columnPins[i], LOW);
    digitalWrite(htpins[i], HIGH); // Turn all heights OFF initially
  }
}

// Show a 4x4 letter pattern on a given height layer
void showLetterAtHeight(byte letter[4], int htPin) {
  for (int row = 0; row < 4; row++) {
    byte pattern = letter[row];
    for (int col = 0; col < 4; col++) {
      bool on = bitRead(pattern, 3 - col); // Left to right
      if (on) {
        ustadd(rowpins[row], columnPins[col], htPin, true);
        delay(0.1); // Small pulse for visibility
        ustadd(rowpins[row], columnPins[col], htPin, false);
      }
    }
  }
}

void loop() {
  for (int l = 0; l < numLetters; l++) {
    // Drop each letter from top (13) to bottom (10)
    for (int h = 3; h >= 0; h--) {
      for (int t = 0; t < 100; t++) { // Display each height frame for a short time
        showLetterAtHeight(letters[l], htpins[h]);
      }
    }
    delay(200); // Pause between letters
  }
}

