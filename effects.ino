// Function declaration
void activateRowAndPulseColumn(int rowPin, int colPin, unsigned long durationMs = 20);

// Column pin list
const int columnPins[] = {6, 7, 8, 9};
int rowpins[]={2,3,4,5};
int htpins[]={10,11,12,13};
bool rowpinvalue[4] = {false, false, false, false};
bool htpinvalue[4] = {true, true, true, true};

const int rowPin = 2;
const int colPin = 8;
const int htPin = 13;
const int pwmResolution = 100;    // 0–100 for brightness levels
const int pwmCycleDelay = 1;      // 1 ms per cycle step → 100 steps = 100ms per brightness change


void setup() {
  // Initialize all row and column pins as OUTPUT and set LOW
  int allPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

  for (int i = 0; i < 8; i++) {
    pinMode(allPins[i], OUTPUT);
    digitalWrite(allPins[i], LOW);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(htpins[i], OUTPUT);
    digitalWrite(htpins[i], HIGH);
  }
}




void ustadd(int row, int col, int ht, bool bull) {
   //Step 1: Clear all data first
  for (int i = 0; i < 4; i++) {
    rowpinvalue[i] = false;
    htpinvalue[i] = true;
  }

  // Step 2: Set intended values
  
  rowpinvalue[row - 2] = bull;        // Set the selected row
 

  // Step 3: Write data to row pins BEFORE pulsing column clock
  for (int i = 0; i < 4; i++) {
    digitalWrite(rowpins[i], rowpinvalue[i]);
  }

  // Step 4: Pulse the target column's shift register clock
  digitalWrite(col, LOW);

  digitalWrite(col, HIGH);

  digitalWrite(col, LOW);


  htpinvalue[ht - 10] = !bull;        // Ground the selected height (only if bull==true)
    // Step 5: Set the height control pins
  for (int i = 0; i < 4; i++) {
    digitalWrite(htpins[i], htpinvalue[i]);
  }
}

// Software PWM using ustadd
void softwarePWM(int row, int col, int ht, int brightnessLevel) {
  // One full PWM cycle (~100 steps)
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

void spiralEffect(int delayTime = 50) {
  int order[16] = {0, 1, 5, 9, 13, 14, 15, 11, 7, 3, 2, 6, 10, 8, 4, 12};
  for (int z = 0; z < 4; z++) {
    for (int i = 0; i < 16; i++) {
      int y = order[i] / 4;
      int x = order[i] % 4;

      ustadd(rowpins[y], columnPins[x], htpins[z], true);
      delay(delayTime);
      ustadd(rowpins[y], columnPins[x], htpins[z], false);
    }
  }

  for (int z = 3; z >= 0; z--) {
    for (int i = 0; i < 16; i++) {
      int y = order[i] / 4;
      int x = order[i] % 4;

      ustadd(rowpins[y], columnPins[x], htpins[z], true);
      delay(delayTime);
      ustadd(rowpins[y], columnPins[x], htpins[z], false);
    }
  }
}

void bouncingDot(int bounces = 20, int delayTime = 100) {
  int x = 0, y = 0, z = 0;
  int dx = 1, dy = 1, dz = 1;

  for (int i = 0; i < bounces; i++) {
    ustadd(rowpins[y], columnPins[x], htpins[z], true);
    delay(delayTime);
    ustadd(rowpins[y], columnPins[x], htpins[z], false);

    x += dx; y += dy; z += dz;
    if (x == 3 || x == 0) dx = -dx;
    if (y == 3 || y == 0) dy = -dy;
    if (z == 3 || z == 0) dz = -dz;
  }
}

void firePulse(int delayTime = 70) {
  for (int z = 0; z < 4; z++) {
    for (int i = 0; i < 16; i++) {
      int x = i % 4;
      int y = i / 4;
      ustadd(rowpins[y], columnPins[x], htpins[z], true);
    }
    delay(delayTime);
  }

  for (int z = 0; z < 4; z++) {
    for (int i = 0; i < 16; i++) {
      int x = i % 4;
      int y = i / 4;
      ustadd(rowpins[y], columnPins[x], htpins[z], false);
    }
    delay(delayTime);
  }
}

void twinkle(int flashes = 50, int onTime = 30) {
  for (int i = 0; i < flashes; i++) {
    int x = random(0, 4);
    int y = random(0, 4);
    int z = random(0, 4);
    ustadd(rowpins[y], columnPins[x], htpins[z], true);
    delay(onTime);
    ustadd(rowpins[y], columnPins[x], htpins[z], false);
  }
}

void loop() {
  spiralEffect();
  bouncingDot();
  firePulse();
  twinkle();
}
