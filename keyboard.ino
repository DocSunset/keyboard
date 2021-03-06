#include "key.h"

// Teensy 2.0 has the LED on pin 11
// Teensy++ 2.0 has the LED on pin 6
// Teensy 3.0 has the LED on pin 13
const int ledPin = 13;

const byte ROWS = 6;  
const byte COLS = 15;

int LAYERS = 2;

bool toggleBind = false;
int currLayer = 0;
int prevLayer = 0;
int desiredLayer = 1;

#define TW_LEFT_SHIFT -1
#define TW_LEFT_CTRL -2
#define TW_LEFT_ALT -3
#define TW_LEFT_GUI -4
#define TW_RIGHT_SHIFT -5
#define TW_RIGHT_CTRL -6
#define TW_RIGHT_ALT -7
#define TW_RIGHT_GUI -8
#define TW_SUPER -9
#define TW_NO_OP -10

int layout[][ROWS][COLS] = 
{  
  {
    
  //layer 0
  {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_INSERT, KEY_F6, TW_SUPER, KEY_F7, KEY_PRINTSCREEN, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12},
  {KEY_ESC, KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, TW_LEFT_GUI, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_LEFT_BRACE, KEY_DELETE },
  {KEY_TAB, KEY_EQUAL, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_DELETE, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_QUOTE, KEY_RIGHT_BRACE},
  {KEY_BACKSPACE, TW_LEFT_CTRL, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_MINUS, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_ENTER, KEY_BACKSLASH},
  {TW_NO_OP, TW_LEFT_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_BACKSPACE, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, TW_RIGHT_SHIFT, TW_NO_OP},
  {TW_NO_OP, TW_LEFT_ALT, KEY_HOME, KEY_PAGE_DOWN, KEY_PAGE_UP, KEY_END, TW_LEFT_GUI, KEY_ENTER, KEY_SPACE, KEY_LEFT, KEY_DOWN, KEY_UP, KEY_RIGHT, TW_RIGHT_ALT, TW_NO_OP}

  },{
    
  {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_INSERT, KEY_F6, TW_SUPER, KEY_F7, KEY_PRINTSCREEN, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12},
  {KEY_ESC, KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, TW_LEFT_GUI, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_LEFT_BRACE, KEY_DELETE },
  {KEY_TAB, KEY_EQUAL, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_DELETE, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_QUOTE, KEY_RIGHT_BRACE},
  {KEY_BACKSPACE, TW_LEFT_CTRL, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_MINUS, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_ENTER, KEY_BACKSLASH},
  {TW_NO_OP, TW_LEFT_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_BACKSPACE, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, TW_RIGHT_SHIFT, TW_NO_OP},
  {TW_NO_OP, TW_LEFT_ALT, KEY_HOME, KEY_PAGE_DOWN, KEY_PAGE_UP, KEY_END, TW_LEFT_GUI, KEY_ENTER, KEY_SPACE, KEY_LEFT, KEY_DOWN, KEY_UP, KEY_RIGHT, TW_RIGHT_ALT, TW_NO_OP}

  }
  
};

byte row[ROWS] = {0, 1, 2, 3, 4, 5};
byte col[COLS] = {21,20,19,18,17,16,15,14,12,11,10,9,8,7,6};

Key key[ROWS][COLS];

int buff[] = {0,0,0,0,0,0};
char mod[] = {0,0,0,0};



void setup() 
{
  Serial.begin(9600);
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);

  for (int c = 0; c < COLS; c++)
    pinMode(col[c], INPUT_PULLUP);

  for (int r = 0; r < ROWS; r++)
    pinMode(row[r], OUTPUT);

  for (int c = 0; c < COLS; ++c)
  {
    for (int r = 0; r < ROWS; ++r)
    {
      key[r][c].setup();
    }
  }
}

// This function will take keypresses passed to it (in the form of an int, for no particular reason)
// and add them to set of six keys that will be passed to the computer when Keyboard.send_now() is called.
// Basically, this collects the currently pressed keys and stores them until they can be passed to the computer.
void setKey(int keypress)
{
  Serial.println(keypress, DEC);
  // Look for unused keys in the buffer  
  int i, j;
  for(i = 0; i < 6 && buff[i] != 0; i++){}
  for(j = 0; j < 4 && mod[j] != 0; j++){}
 
  if( j < 4 )
  {
    switch (keypress)
    {
    case TW_NO_OP:
      break;

    case TW_LEFT_CTRL:
      mod[j] = KEY_LEFT_CTRL;
      break;

    case TW_LEFT_SHIFT:
      mod[j] = KEY_LEFT_SHIFT;
      break;

    case TW_LEFT_ALT:
      mod[j] = KEY_LEFT_ALT;
      break;

    case TW_LEFT_GUI:
      mod[j] = KEY_LEFT_GUI;
      break;

    case TW_RIGHT_CTRL:
      mod[j] = KEY_RIGHT_CTRL;
      break;

    case TW_RIGHT_SHIFT:
      mod[j] = KEY_RIGHT_SHIFT;
      break;

    case TW_RIGHT_ALT:
      mod[j] = KEY_RIGHT_ALT;
      break;

    case TW_RIGHT_GUI:
      mod[j] = KEY_RIGHT_GUI;
      break;

    case TW_SUPER:
      mod[j] = KEY_LEFT_CTRL | KEY_LEFT_SHIFT | KEY_LEFT_ALT | KEY_LEFT_GUI;
      break;

    default:
      if (i < 6) {
        buff[i] = keypress;
      }
    }
  }
  
  if(holdKey('^')) // Prevent setting layer key into set_key or set_modifier
    return;
  
  // Hold keypresses in buffer
  Keyboard.set_modifier(mod[0]|mod[1]|mod[2]|mod[3]);
  Keyboard.set_key1(buff[0]);
  Keyboard.set_key2(buff[1]);
  Keyboard.set_key3(buff[2]);
  Keyboard.set_key4(buff[3]);
  Keyboard.set_key5(buff[4]);
  Keyboard.set_key6(buff[5]);
}

// This method sends the depressed keys and clears the buffer.
void sendKey()
{
  
  Keyboard.send_now();
  clearBuffer();
  
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.set_key2(0);
  Keyboard.set_key3(0);
  Keyboard.set_key4(0);
  Keyboard.set_key5(0);
  Keyboard.set_key6(0);
}

void clearBuffer()
{
  for(int x = 0; x < 6; x++){ buff[x] = 0; }
  for(int x = 0; x < 4; x++){ mod[x] = 0; }
  
}

// Detects when a key is held down, returns true if held down, false if not
bool holdKey(char keypress)
{
  if(buff[0] == keypress ||
     buff[1] == keypress ||
     buff[2] == keypress ||
     buff[3] == keypress ||
     buff[4] == keypress ||
     buff[5] == keypress)
    return true;
  
  return false;
}

// Calling this function will cycle to the next layer
void cycleLayer()
{ 
  if(currLayer == (LAYERS - 1)) // Reached maximum layer, going back to first layer
    currLayer = 0;
    
  else
    currLayer++; // Increments to the next layer
}

// Toggles between two layers, the curret layer and desired layer
void toggleLayer(char keyHeld, int desLayer)
{
  if (holdKey(keyHeld)){
    prevLayer = currLayer; // Saves previous layer
    currLayer = desLayer; // Desired layer
  }
  
  else
    currLayer = prevLayer; // Returns to previous layer
}

// Goes to desired layer when keyHeld is pressed, returns to previous layer when released 
void holdLayer(char keyHeld, int desLayer)
{
  if(holdKey(keyHeld))
  {
    if(!toggleBind) // Saves the previous layer, using boolean to prevent updating prevLayer more than once
    { 
      prevLayer = currLayer;
      toggleBind = 1;
    }
    
    currLayer = desLayer; // Desire layer
  }
  else
  {
    if(toggleBind)
      toggleBind = !toggleBind; // Resets boolean
    
    currLayer = prevLayer; // Returns to previous layer
  }
}

double counter = 0;
double pop_count_accumulator = 0;
uint8_t max_pop_count = 0;
void loop() 
{
  for (int r = 0; r < ROWS; ++r) 
  {
    digitalWrite(row[r], LOW);
    for (int c = 0; c < COLS; ++c)
    {
      int raw_state = !digitalRead(col[c]);
      key[r][c].update(raw_state);
      if (key[r][c].pressed)  
      {
        setKey(layout[currLayer][r][c]);
      }
    }
    digitalWrite(row[r], HIGH);
  }
  
  // If the fn layer key is held, it changes the layer to the desired layer,
  // when released, returns to previous layer
  holdLayer('^', desiredLayer); 
  
  // Now that all of the keys have been polled it is time to send them out!
  sendKey();
  delay(1);
}

// https://forum.arduino.cc/index.php?topic=475435.0
void print_binary(uint32_t var) 
{
  for (uint32_t test = 0x80000000; test; test >>= 1) 
    Serial.write(var & test ? '1' : '0');
}
