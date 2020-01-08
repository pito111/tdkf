// LED pin (13 for Teensy 4.0 onboard LED)
const byte led = 13;
// base "off" brightness level of LED
const byte ledBase = 20;
// "on" brightness level of LED
const byte ledOn = 100;
// delay after pin level change in microsecconds
const int pinDelay = 10;

// keyboard row list
const byte buttonColumns[] = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 14, 17 };
// keyboard column list
const byte buttonRows[] = { 21, 22, 23, 16, 19 };
// number of columns
const byte buttonColumnsCount = sizeof(buttonColumns) / sizeof(byte);
// number of rows
const byte buttonRowsCount = sizeof(buttonRows) / sizeof(byte);

// the setup routine runs once when you press reset:
void setup() {
  // initialize the LED pin as an output.
  pinMode(led, OUTPUT);
  // start on base level to indicate working order
  analogWrite(led, ledBase);

  // initialize all column pins to INPUT_PULLUP
  for(int i = 0; i < buttonColumnsCount; i++) {
    pinMode(buttonColumns[i], INPUT_PULLUP);
  }
  // initialize all row pins to OUTPUT with state LOW
  for(int i = 0; i < buttonRowsCount; i++) {
    pinMode(buttonRows[i], OUTPUT);
    digitalWrite(buttonRows[i], LOW);
  }
}

// declare array for registering key statuses
boolean keys[buttonColumnsCount][buttonRowsCount][2];
// keyboard map. Be carefull with correct number of rows and columns.
// transposed for better readability
int keymap[buttonRowsCount][buttonColumnsCount] = {
  { KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFT_BRACE, KEY_RIGHT_BRACE, 0, 0},
  { KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_QUOTE, KEY_BACKSLASH, 0, 0},
  { MODIFIERKEY_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_SPACE, 0, 0},
  { MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL, MODIFIERKEY_CTRL },
  { KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER }
};

// variable to register if any key is pressed
boolean anyKeyPressed;

// the loop routine runs over and over again forever:
void loop() {

  // archive previous state of pressed keys
  for( int i=0; i < buttonColumnsCount; i++) {
    for (int j = 0; j < buttonRowsCount; j++) {
      keys[i][j][1] = keys[i][j][0];
    }
  }

  // reset key presses
  for( int i=0; i < buttonColumnsCount; i++) {
    for (int j = 0; j < buttonRowsCount; j++) {
      keys[i][j][0] = false;
    }
  }
  anyKeyPressed = false;
  
  // read pressed keys
  for( int i=0; i < buttonColumnsCount; i++) {
    // if at least one key in column is pressed
    if(digitalRead(buttonColumns[i]) == LOW) {
      // set all rows to HIGH
      for (int j = 0; j < buttonRowsCount; j++) digitalWrite(buttonRows[j], HIGH);
      delayMicroseconds(pinDelay);
      // scan row by row with LOW to get all pressed keys in a column
      for (int j = 0; j < buttonRowsCount; j++) {
        digitalWrite(buttonRows[j], LOW);
        delayMicroseconds(pinDelay);
        keys[i][j][0] = digitalRead(buttonColumns[i]) == LOW ? true : false;
        
        // register if any key is pressed at all
        if( ! anyKeyPressed and keys[i][j][0]) anyKeyPressed = true;

        digitalWrite(buttonRows[j], HIGH);
        delayMicroseconds(pinDelay);
      }
      // reset rows to LOW after scan
      for (int j = 0; j < buttonRowsCount; j++) digitalWrite(buttonRows[j], LOW);
      delayMicroseconds(pinDelay);
    } else {
      // if no key is pressed in a row, reset whole row
      for (int j = 0; j < buttonRowsCount; j++) keys[i][j][0] = false;
    }
  }

  // send key events to PC
  for( int i=0; i < buttonColumnsCount; i++) {
    for (int j = 0; j < buttonRowsCount; j++) {
      // if key was just pressed
      if( keys[i][j][0] and ! keys[i][j][1] ) {
        Keyboard.press(keymap[j][i]);
        // set LED to "on" level if key is pressed
        analogWrite(led, ledOn);
      }
      // if key was just released
      if( ! keys[i][j][0] and keys[i][j][1] ) {
        Keyboard.release(keymap[j][i]);
      }
    }
  }
  // set LED to "off" level if no key is pressed
  if( ! anyKeyPressed ) analogWrite(led, ledBase);
}
