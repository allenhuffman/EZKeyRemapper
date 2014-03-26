/*-----------------------------------------------------------------------------
 
 Bluefruit EZ-Key Remapper
 By Allen C. Huffman (alsplace@pobox.com)
 www.subethasoftware.com
 
 This program will remap the keys of an Adafruit Bluefruit EZ-Key
 over a TX/RX UART serial connection. 
 
 Hardware:
 
 http://www.adafruit.com/bluefruit
 
 Documentation:
 
 http://learn.adafruit.com/introducing-bluefruit-ez-key-diy-bluetooth-hid-keyboard/overview
 
 CONFIGURATION:
 1. Define the pins on the Arduino that will be used for TX and RX in
 .  the Software Serial library, and connected to the Bluefruit EZ-KEY.
 
 VERSION HISTORY:
 2014-03-22 0.00 allenh - Created, based on EZKeyDemo source.
 2014-03-26 0.01 allenh - Cleaning up code.
 
 TODO:
 * Clean up the user interface, and the code. It's quite messy right now.
 
 TOFIX:
 * TODO...
 -----------------------------------------------------------------------------*/
#define VERSION "0.01"

#include <avr/pgmspace.h>
#include <SoftwareSerial.h>

#include "EZKey.h"
#include "EZKeyStructures.h"

/*---------------------------------------------------------------------------*/
// CONFIGURATION
/*---------------------------------------------------------------------------*/
#define RX_PIN         10
#define TX_PIN         11

/*---------------------------------------------------------------------------*/
// You should not need to change any of these items.
#define NUM_INPUTS     12

#define WIDTH          80   // Screen width.
#define COLUMNS        4

#define NUM_MODIFIERS  9
#define NUM_KEYCODES   112

#define BUFSIZE        4   // Maximum input buffer size.
/*---------------------------------------------------------------------------*/
// USER INTERFACE STUFF
/*---------------------------------------------------------------------------*/
// Keyboard modifiers.
// Store these strings in Flash to save RAM.
const char modNone[]        PROGMEM = "NONE";
const char modCtrlLeft[]    PROGMEM = "CTRL_LEFT";
const char modShiftLeft[]   PROGMEM = "SHIFT_LEFT";
const char modAltLeft[]     PROGMEM = "ALT_LEFT";
const char modGUILeft[]     PROGMEM = "GUI_LEFT";
const char modCtrlRight[]   PROGMEM = "CTRL_RIGHT";
const char modShiftRight[]  PROGMEM = "SHIFT_RIGHT";
const char modAltRight[]    PROGMEM = "ALT_RIGHT";
const char modGUIRight[]    PROGMEM = "GUI_RIGHT";

// Create an array of option codes and pointers to Flash strings, in Flash.
KeyCodeStruct modifiers[NUM_MODIFIERS] PROGMEM =
{
  { 
    MODIFIER_NONE,          modNone      }
  ,
  { 
    MODIFIER_CONTROL_LEFT,  modCtrlLeft         }
  ,
  { 
    MODIFIER_SHIFT_LEFT,    modShiftLeft         }
  ,
  { 
    MODIFIER_ALT_LEFT,      modAltLeft         }
  ,
  { 
    MODIFIER_GUI_LEFT,      modGUILeft         }
  ,
  { 
    MODIFIER_CONTROL_RIGHT, modCtrlRight         }
  ,
  { 
    MODIFIER_SHIFT_RIGHT,   modShiftRight         }
  ,
  { 
    MODIFIER_ALT_RIGHT,     modAltRight         }
  ,
  { 
    MODIFIER_GUI_RIGHT,     modGUIRight         }
};

const char keyNoneStr[]   PROGMEM = "NONE";
const char keyAStr[]      PROGMEM = "A";
const char keyBStr[]      PROGMEM = "B";
const char keyCStr[]      PROGMEM = "C";
const char keyDStr[]      PROGMEM = "D";
const char keyEStr[]      PROGMEM = "E";
const char keyFStr[]      PROGMEM = "F";
const char keyGStr[]      PROGMEM = "G";
const char keyHStr[]      PROGMEM = "H";
const char keyIStr[]      PROGMEM = "I";
const char keyJStr[]      PROGMEM = "J";
const char keyKStr[]      PROGMEM = "K";
const char keyLStr[]      PROGMEM = "L";
const char keyMStr[]      PROGMEM = "M";
const char keyNStr[]      PROGMEM = "N";
const char keyOStr[]      PROGMEM = "O";
const char keyPStr[]      PROGMEM = "P";
const char keyQStr[]      PROGMEM = "Q";
const char keyRStr[]      PROGMEM = "R";
const char keySStr[]      PROGMEM = "S";
const char keyTStr[]      PROGMEM = "T";
const char keyUStr[]      PROGMEM = "U";
const char keyVStr[]      PROGMEM = "V";
const char keyWStr[]      PROGMEM = "W";
const char keyXStr[]      PROGMEM = "X";
const char keyYStr[]      PROGMEM = "Y";
const char keyZStr[]      PROGMEM = "Z";

const char key1Str[]      PROGMEM = "1";
const char key2Str[]      PROGMEM = "2";
const char key3Str[]      PROGMEM = "3";
const char key4Str[]      PROGMEM = "4";
const char key5Str[]      PROGMEM = "5";
const char key6Str[]      PROGMEM = "6";
const char key7Str[]      PROGMEM = "7";
const char key8Str[]      PROGMEM = "8";
const char key9Str[]      PROGMEM = "9";
const char key0Str[]      PROGMEM = "0";

const char keyReturnStr[]       PROGMEM = "RETURN";
const char keyEscapeStr[]       PROGMEM = "ESCAPE";
const char keyBackspaceStr[]    PROGMEM = "BACKSPACE";
const char keyTabStr[]          PROGMEM = "TAB";
const char keySpaceStr[]        PROGMEM = "SPACE";
const char keyMinusStr[]        PROGMEM = "MINUS";
const char keyEqualStr[]        PROGMEM = "EQUAL";
const char keyBracketLeftStr[]  PROGMEM = "BRACKET_LEFT";
const char keyBracketRightStr[] PROGMEM = "BRACKET_RIGHT";
const char keyBackslashStr[]    PROGMEM = "BACKSLASH";
const char keyEurope1Str[]      PROGMEM = "EUROPE_1";
const char keySemicolonStr[]    PROGMEM = "SEMICOLON";
const char keyApostropheStr[]   PROGMEM = "APOSTROPHE";
const char keyGraveStr[]        PROGMEM = "GRAVE";
const char keyCommaStr[]        PROGMEM = "COMMA";
const char keyPeriodStr[]       PROGMEM = "PERIOD";
const char keySlashStr[]        PROGMEM = "SLASH";
const char keyCapsLockStr[]     PROGMEM = "CAPS_LOCK";

const char keyF1Str[]  PROGMEM = "F1";
const char keyF2Str[]  PROGMEM = "F2";
const char keyF3Str[]  PROGMEM = "F3";
const char keyF4Str[]  PROGMEM = "F4";
const char keyF5Str[]  PROGMEM = "F5";
const char keyF6Str[]  PROGMEM = "F6";
const char keyF7Str[]  PROGMEM = "F7";
const char keyF8Str[]  PROGMEM = "F8";
const char keyF9Str[]  PROGMEM = "F9";
const char keyF10Str[] PROGMEM = "F10";
const char keyF11Str[] PROGMEM = "F11";
const char keyF12Str[] PROGMEM = "F12";

const char keyPrintScreenStr[] PROGMEM = "PRINT_SCREEN";
const char keyScrollLockStr[]  PROGMEM = "SCROLL_LOCK";
const char keyPauseStr[]       PROGMEM = "PAUSE";
const char keyInsertStr[]      PROGMEM = "INSERT";
const char keyHomeStr[]        PROGMEM = "HOME";
const char keyPageUpStr[]      PROGMEM = "PAGE_UP";
const char keyDeleteStr[]      PROGMEM = "DELETE";
const char keyEndStr[]         PROGMEM = "END";
const char keyPageDownStr[]    PROGMEM = "PAGE_DOWN";
const char keyArrowRightStr[]  PROGMEM = "ARROW_RIGHT";
const char keyArrowLeftStr[]   PROGMEM = "ARROW_LEFT";
const char keyArrowDownStr[]   PROGMEM = "ARROW_DOWN";
const char keyArrowUpStr[]     PROGMEM = "ARROW_UP";
const char keyNumLockStr[]     PROGMEM = "NUM_LOCK";

const char keyKeypadDivideStr[]   PROGMEM = "KEYPAD_SLASH";
const char keyKeypadMultiplyStr[] PROGMEM = "KEYPAD_*";
const char keyKeypadSubtractStr[] PROGMEM = "KEYPAD_MINUS";
const char keyKeypadAddStr[]      PROGMEM = "KEYPAD_PLUS";
const char keyKeypadEnterStr[]    PROGMEM = "KEYPAD_ENTER";
const char keyKeypad1Str[]        PROGMEM = "KEYPAD_1";
const char keyKeypad2Str[]        PROGMEM = "KEYPAD_2";
const char keyKeypad3Str[]        PROGMEM = "KEYPAD_3";
const char keyKeypad4Str[]        PROGMEM = "KEYPAD_4";
const char keyKeypad5Str[]        PROGMEM = "KEYPAD_5";
const char keyKeypad6Str[]        PROGMEM = "KEYPAD_6";
const char keyKeypad7Str[]        PROGMEM = "KEYPAD_7";
const char keyKeypad8Str[]        PROGMEM = "KEYPAD_8";
const char keyKeypad9Str[]        PROGMEM = "KEYPAD_9";
const char keyKeypad0Str[]        PROGMEM = "KEYPAD_0";
const char keyKeypadDecimalStr[]  PROGMEM = "KEYPAD_PERIOD";

const char keyEurope2Str[]      PROGMEM = "EUROPE_2";
const char keyApplicationStr[]  PROGMEM = "APPLICATION";
const char keyPowerStr[]        PROGMEM = "POWER";
const char keyKeypadEqualStr[]  PROGMEM = "KEYPAD_EQUAL";
const char keyF13Str[]          PROGMEM = "F13";
const char keyF14Str[]          PROGMEM = "F14";
const char keyF15Str[]          PROGMEM = "F15";
const char keyControlLeftStr[]  PROGMEM = "CONTROL_LEFT";
const char keyShiftLeftStr[]    PROGMEM = "SHIFT_LEFT";
const char keyAltLeftStr[]      PROGMEM = "ALT_LEFT";
const char keyGUILeftStr[]      PROGMEM = "GUI_LEFT";
const char keyControlRightStr[] PROGMEM = "CONTROL_RIGHT";
const char keyShiftRightStr[]   PROGMEM = "SHIFT_RIGHT";
const char keyAltRightStr[]     PROGMEM = "ALT_RIGHT";
const char keyGUIRightStr[]     PROGMEM = "GUI_RIGHT";

KeyCodeStruct keycodes[NUM_KEYCODES] PROGMEM =
{
  { 
    0, keyNoneStr         }
  ,
  { 
    KEY_A, keyAStr         }
  ,
  { 
    KEY_B, keyBStr         }
  ,
  { 
    KEY_C, keyCStr         }
  ,
  { 
    KEY_D, keyDStr         }
  ,
  { 
    KEY_E, keyEStr         }
  ,
  { 
    KEY_F, keyFStr         }
  ,
  { 
    KEY_G, keyGStr         }
  ,
  { 
    KEY_H, keyHStr         }
  ,
  { 
    KEY_I, keyIStr         }
  ,
  { 
    KEY_J, keyJStr         }
  ,
  { 
    KEY_K, keyKStr         }
  ,
  { 
    KEY_L, keyLStr         }
  ,
  { 
    KEY_M, keyMStr         }
  ,
  { 
    KEY_N, keyNStr         }
  ,
  { 
    KEY_O, keyOStr         }
  ,
  { 
    KEY_P, keyPStr         }
  ,
  { 
    KEY_Q, keyQStr         }
  ,
  { 
    KEY_R, keyRStr         }
  ,
  { 
    KEY_S, keySStr         }
  ,
  { 
    KEY_T, keyTStr         }
  ,
  { 
    KEY_U, keyUStr         }
  ,
  { 
    KEY_V, keyVStr         }
  ,
  { 
    KEY_W, keyWStr         }
  ,
  { 
    KEY_X, keyXStr         }
  ,
  { 
    KEY_Y, keyYStr         }
  ,
  { 
    KEY_Z, keyZStr         }
  ,

  { 
    KEY_1, key1Str         }
  ,
  { 
    KEY_2, key2Str         }
  ,
  { 
    KEY_3, key3Str         }
  ,
  { 
    KEY_4, key4Str         }
  ,
  { 
    KEY_5, key5Str         }
  ,
  { 
    KEY_6, key6Str         }
  ,
  { 
    KEY_7, key7Str         }
  ,
  { 
    KEY_8, key8Str         }
  ,
  { 
    KEY_9, key9Str         }
  ,
  { 
    KEY_0, key0Str         }
  ,

  { 
    KEY_RETURN,        keyReturnStr         }
  ,
  { 
    KEY_ESCAPE,        keyEscapeStr         }
  ,
  { 
    KEY_BACKSPACE,     keyBackspaceStr         }
  ,
  { 
    KEY_TAB,           keyTabStr         }
  ,
  { 
    KEY_SPACE,         keySpaceStr         }
  ,
  { 
    KEY_MINUS,         keyMinusStr         }
  ,
  { 
    KEY_EQUAL,         keyEqualStr         }
  ,
  { 
    KEY_BRACKET_LEFT,  keyBracketLeftStr         }
  ,
  { 
    KEY_BRACKET_RIGHT, keyBracketRightStr         }
  ,
  { 
    KEY_BACKSLASH,     keyBackslashStr         }
  ,
  { 
    KEY_EUROPE_1,      keyEurope1Str         }
  ,
  { 
    KEY_SEMICOLON,     keySemicolonStr         }
  ,
  { 
    KEY_APOSTROPHE,    keyApostropheStr         }
  ,
  { 
    KEY_GRAVE,         keyGraveStr         }
  ,
  { 
    KEY_COMMA,         keyCommaStr         }
  ,
  { 
    KEY_PERIOD,        keyPeriodStr         }
  ,
  { 
    KEY_SLASH,         keySlashStr         }
  ,
  { 
    KEY_CAPS_LOCK,     keyCapsLockStr         }
  ,

  { 
    KEY_F1,            keyF1Str         }
  ,
  { 
    KEY_F2,            keyF2Str         }
  ,
  { 
    KEY_F3,            keyF3Str         }
  ,
  { 
    KEY_F4,            keyF4Str         }
  ,
  { 
    KEY_F5,            keyF5Str         }
  ,
  { 
    KEY_F6,            keyF6Str         }
  ,
  { 
    KEY_F7,            keyF7Str         }
  ,
  { 
    KEY_F8,            keyF8Str         }
  ,
  { 
    KEY_F9,            keyF9Str         }
  ,
  { 
    KEY_F10,           keyF10Str         }
  ,
  { 
    KEY_F11,           keyF11Str         }
  ,
  { 
    KEY_F12,           keyF12Str         }
  ,

  { 
    KEY_PRINT_SCREEN,  keyPrintScreenStr         }
  ,
  { 
    KEY_SCROLL_LOCK,   keyScrollLockStr         }
  ,
  { 
    KEY_PAUSE,         keyPauseStr         }
  ,
  { 
    KEY_INSERT,        keyInsertStr         }
  ,
  { 
    KEY_HOME,          keyHomeStr         }
  ,
  { 
    KEY_PAGE_UP,       keyPageUpStr         }
  ,
  { 
    KEY_DELETE,        keyDeleteStr         }
  ,
  { 
    KEY_END,           keyEndStr         }
  ,
  { 
    KEY_PAGE_DOWN,     keyPageDownStr         }
  ,
  { 
    KEY_ARROW_RIGHT,   keyArrowRightStr         }
  ,
  { 
    KEY_ARROW_LEFT,    keyArrowLeftStr         }
  ,
  { 
    KEY_ARROW_DOWN,    keyArrowDownStr         }
  ,
  { 
    KEY_ARROW_UP,      keyArrowUpStr         }
  ,
  { 
    KEY_NUM_LOCK,      keyNumLockStr         }
  ,

  { 
    KEY_KEYPAD_DIVIDE, keyKeypadDivideStr         }
  ,
  { 
    KEY_KEYPAD_MULTIPLY, keyKeypadMultiplyStr         }
  ,
  { 
    KEY_KEYPAD_SUBTRACT, keyKeypadSubtractStr         }
  ,
  { 
    KEY_KEYPAD_ADD,    keyKeypadAddStr         }
  ,
  { 
    KEY_KEYPAD_ENTER,  keyKeypadEnterStr         }
  ,
  { 
    KEY_KEYPAD_1,      keyKeypad1Str         }
  ,
  { 
    KEY_KEYPAD_2,      keyKeypad2Str         }
  ,
  { 
    KEY_KEYPAD_3,      keyKeypad3Str         }
  ,
  { 
    KEY_KEYPAD_4,      keyKeypad4Str         }
  ,
  { 
    KEY_KEYPAD_5,      keyKeypad5Str         }
  ,
  { 
    KEY_KEYPAD_6,      keyKeypad6Str         }
  ,
  { 
    KEY_KEYPAD_7,      keyKeypad7Str         }
  ,
  { 
    KEY_KEYPAD_8,      keyKeypad8Str         }
  ,
  { 
    KEY_KEYPAD_9,      keyKeypad9Str         }
  ,
  { 
    KEY_KEYPAD_0,      keyKeypad0Str         }
  ,
  { 
    KEY_KEYPAD_DECIMAL,keyKeypadDecimalStr         }
  ,

  { 
    KEY_EUROPE_2,      keyEurope2Str         }
  ,
  { 
    KEY_APPLICATION,   keyApplicationStr         }
  ,
  { 
    KEY_POWER,         keyPowerStr         }
  ,
  { 
    KEY_KEYPAD_EQUAL,  keyKeypadEqualStr         }
  ,
  { 
    KEY_F13,           keyF13Str         }
  ,
  { 
    KEY_F14,           keyF14Str         }
  ,
  { 
    KEY_F15,           keyF15Str         }
  ,
  { 
    KEY_CONTROL_LEFT,  keyControlLeftStr         }
  ,
  { 
    KEY_SHIFT_LEFT,    keyShiftLeftStr         }
  ,
  { 
    KEY_ALT_LEFT,      keyAltLeftStr         }
  ,
  { 
    KEY_GUI_LEFT,      keyGUILeftStr         }
  ,
  { 
    KEY_CONTROL_RIGHT, keyControlRightStr         }
  ,
  { 
    KEY_SHIFT_RIGHT,   keyShiftRightStr         }
  ,
  { 
    KEY_ALT_RIGHT,     keyAltRightStr         }
  ,
  { 
    KEY_GUI_RIGHT,     keyGUIRightStr         }
};

/*---------------------------------------------------------------------------*/
// Some macros we will use to make things simpler.
#define FLASHSTR(x) (const __FlashStringHelper*)(x)
#define FLASHPTR(x) (const __FlashStringHelper*)pgm_read_word(&x)

// Initialize the Software Serial port.
SoftwareSerial EZKey(RX_PIN, TX_PIN); // RX, TX

// FIXTHIS: Lazy programmer using globals.
RawHIDKeyboardReportStruct input[NUM_INPUTS];

void setup()
{
  Serial.begin(9600);

  Serial.println();
  Serial.print(F("Adafruit Bluefruit EZ-Key Remapper "));
  Serial.print(VERSION);
  Serial.println(F(" by Allen C. Huffman (alsplace@pobox.com)"));

  Serial.print(F("Configured for an EZ-Key connected using: TX PIN "));
  Serial.print(TX_PIN);
  Serial.print(F(" and RX PIN"));
  Serial.println(RX_PIN);

  // We talk to the EZ-Key at 9600 baud.
  EZKey.begin(9600);

} // end of setup()

/*---------------------------------------------------------------------------*/

void loop()
{
  int  i, j;
  char ch;
  char buffer[BUFSIZE];
  int  choice;
  bool done;

  // Initialize to nothin'.
  for (i=0; i<NUM_INPUTS; i++)
  {
    input[i].modifier = MODIFIER_NONE;
    for (j=0; j<INPUT_KEYCODES; j++)
    {
      input[i].keycode[j] = KEY_NONE;
    }
  }
  // Set up the input structure to mimic what the EZ-Key defaults are.
  // I do not know if there is a way to query the EZ-Key to get this
  // information, so if the EZ-Key changes the default values, this code
  // would have to be updated to match.
  input[0].keycode[0] = KEY_ARROW_UP;
  input[1].keycode[0] = KEY_ARROW_DOWN;
  input[2].keycode[0] = KEY_ARROW_LEFT;
  input[3].keycode[0] = KEY_ARROW_RIGHT;
  input[4].keycode[0] = KEY_RETURN;
  input[5].keycode[0] = KEY_SPACE;
  input[6].keycode[0] = KEY_1;
  input[7].keycode[0] = KEY_2;
  input[8].keycode[0] = KEY_W;
  input[9].keycode[0] = KEY_A;
  input[10].keycode[0] = KEY_S;
  input[11].keycode[0] = KEY_D;

  Serial.println();
  Serial.println(F("Waiting for EZ-Key to be in remapping mode. Apply power to the"));
  Serial.println(F("EZ-Key while holding down the PAIR button."));
  
  expectFromEZKey("Remap ready!\n", 0);

  Serial.println();
  Serial.println(F("EZ-Key is in remap mode."));

  done = false;
  while(done==false)
  {
    Serial.println();
    Serial.print(F("Enter input to configure (0-"));
    Serial.print(NUM_INPUTS-1);
    Serial.print(F("), [L)ist, [U)pdate or [Q)uit: "));

    if (lineInput(buffer, 2)>0)
    {
      switch(toupper(buffer[0]))
      {
      case 'L':
        showInputConfig(input);
        break;

      case 'U':
        updateEZKey();
        break;

      case 'Q':
        done = true;
        Serial.println();
        Serial.println(F("(Restarting utility.)"));
        break;

      // If anything else, see if it is a digit and handle it, if so.
      default:
        if (isdigit(buffer[0]))
        {
          choice = atoi(buffer);
          editInputConfig(choice);
        }
        break;
      }
    } // end of if (lineInput(buffer, 2)>0)
  } // end of while(done==false)

  Serial.println();
  Serial.println(F("Exit... Program restarting..."));
} // end of loop()

/*---------------------------------------------------------------------------*/

void showInputConfig(RawHIDKeyboardReportStruct input[])
{
  uint8_t i, j;
  uint8_t modifier, keycode;
  bool    showPlus;

  Serial.println();
  Serial.println(F("Input Key Codes:"));
  Serial.println(F("----- ----------"));

  for (i=0; i<NUM_INPUTS; i++)
  {
    showPlus = false;

    if (i<10) Serial.print(" ");
    Serial.print(i);
    Serial.print(".   ");

    modifier = input[i].modifier;

    // Skip "NONE"
    if (modifier!=MODIFIER_NONE)
    {
      Serial.print(FLASHPTR(modifiers[lookupModifier(modifier)].name));
      showPlus = true;
    }

    for (j=0; j<INPUT_KEYCODES; j++)
    {
      keycode = input[i].keycode[j];

      // Skip "NONE"
      if (keycode==KEY_NONE) continue;

      if (showPlus==true) Serial.print(" + ");
      Serial.print(FLASHPTR(keycodes[lookupKeycode(keycode)].name));
    }
    Serial.println();
  } // end of for (i=0; i<NUM_INPUTS; i++)
} // end of showInputConfig()

/*---------------------------------------------------------------------------*/

void showKeyCodes(KeyCodeStruct *items, uint8_t numItems)
{
  uint8_t i, j, rows, first;
  uint8_t k, len;

  Serial.println();

  rows = (numItems/COLUMNS) + ((numItems % COLUMNS)>0);

  for (i=0, first=0; i<rows; i++, first++ )
  {
    for (j = first; j<numItems; j += rows )
    {
      if (j<100) Serial.print(" ");
      if (j<10) Serial.print(" ");
      Serial.print(j);
      Serial.print(". ");
      len = Serial.print(FLASHPTR(items[j].name));

      for (k=len+5; k<(WIDTH/COLUMNS); k++)
      {
        Serial.print(" ");
      }
    }
    Serial.println();
  }
} // end of showKeyCodes()

/*---------------------------------------------------------------------------*/

uint8_t lookupKeycode(uint8_t keycode)
{
  uint8_t i;
  uint8_t temp;

  //Serial.print("looking up keycode "); Serial.print(keycode);

  for (i=0; i<sizeof(keycodes)/sizeof(*keycodes); i++)
  {
    temp = pgm_read_byte(&keycodes[i].keycode);
    if (keycode == temp)
    {
      //Serial.print(", found "); Serial.println(i);
      return i;
    }
  }

  return 0; // NONE!
} // end of lookupKeycode()

/*---------------------------------------------------------------------------*/

uint8_t lookupModifier(uint8_t modifier)
{
  uint8_t i;
  uint8_t temp;

  //Serial.print("looking up modifier "); Serial.print(modifier);

  for (i=0; i<sizeof(modifiers)/sizeof(*modifiers); i++)
  {
    temp = pgm_read_byte(&modifiers[i].keycode);
    if (modifier == temp)
    {
      //Serial.print(", found "); Serial.println(i);
      return i;
    }
  }

  // If not found, we return nothing.
  return MODIFIER_NONE; // NONE!
} // end of lookupModifier()

/*---------------------------------------------------------------------------*/

void showHeader()
{
  int i;

  for (i=0; i<WIDTH; i++) Serial.print("-");
  Serial.println();
} // end of showHeader()

/*---------------------------------------------------------------------------*/

void editInputConfig(uint8_t num)
{
  char    buffer[BUFSIZE];
  uint8_t choice;
  bool    done;
  uint8_t i;

  if (num>=NUM_INPUTS)
  {
    Serial.print(F("Valid Inputs are 0-"));
    Serial.print(NUM_INPUTS-1);
    Serial.println();
    return;
  }

  Serial.println();
  Serial.print(F("Editing Input "));
  Serial.print(num);
  Serial.println(F(" Configuration:"));

  done = false;

  while(done==false)
  {
    //1 : Modifier:CONTROL_LEFT - Enter modifier # (0-8), ?=List, ENTER=Skip: 1
    Serial.println();
    Serial.print(F("Modifier: "));
    Serial.print(FLASHPTR(modifiers[lookupModifier(input[num].modifier)].name));
    Serial.print(F(" - Enter new modifier # (0-8), [L)ist, [ENTER)Skip or [Q)uit: "));

    if (lineInput(buffer, 1)>0)
    {
      switch(toupper(buffer[0]))
      {
      case 'L':
        showKeyCodes(modifiers, NUM_MODIFIERS);
        break;

      case 'Q':
        done = true;
        break;

      default:
        if (isdigit(buffer[0]))
        {
          choice = atoi(buffer);
          if (choice>=NUM_MODIFIERS)
          {
            Serial.print(F("Valid Modifiers are 0-"));
            Serial.print(NUM_MODIFIERS-1);
            Serial.println();
          }
          else
          {
            input[num].modifier = pgm_read_byte(&modifiers[choice].keycode);
            Serial.print(F("Setting new modifier value to: "));
            Serial.println(FLASHPTR(modifiers[choice].name));
          }
        } 
      } // end of switch(toupper(buffer[0]))
    }
    else // nothing, so must have pressed ENTER or something.
    {
      done = true;
    }
    // end of (lineInput(buffer, 1)>0)
  } // end of while(done==false);

  // Handle key codes.
  for (i=0; i<INPUT_KEYCODES; i++)
  {
    done = false;
    while(done==false)
    {
      Serial.println();
      Serial.print(F("Keycode"));
      Serial.print(i);
      Serial.print(F(": "));
      Serial.print(FLASHPTR(keycodes[lookupKeycode(input[num].keycode[i])].name));
      Serial.print(F(" - Enter new key code # (0-111), [L)ist, [ENTER)Skip or [Q)uit: "));

      if (lineInput(buffer, 3)>0)
      {
        switch(toupper(buffer[0]))
        {
        case 'L':
          showKeyCodes(keycodes, NUM_KEYCODES);
          break;

        case 'Q':
          done = true;
          break;

        default:
          if (isdigit(buffer[0]))
          {
            choice = atoi(buffer);
            if (choice>=NUM_KEYCODES)
            {
              Serial.print(F("Valid Key Codes are 0-"));
              Serial.print(NUM_KEYCODES-1);
              Serial.println();
            }
            else
            {
              input[num].keycode[i] = pgm_read_byte(&keycodes[choice].keycode);
              Serial.print(F("Setting new keycode value to: "));
              Serial.println(FLASHPTR(keycodes[choice].name));
            }
          } 
        } // end of switch(toupper(buffer[0]))
      }
      else // nothing, so must have pressed ENTER or something.
      {
        done = true;
      } // end of (lineInput(buffer, 1)>0)
    } // end of while(done==false)
  } // end of for (i=0; i<INPUT_KEYCODES; i++) 
} // end of editInputConfig()

/*---------------------------------------------------------------------------*/

// To update the EZ-Key (when it is in remapping mode), we write out a string
// that starts with a lowercase "s" followed by 129 two digit HEX numbers
// (eight for each input, in order, then 00s to pad). The 129th hex digit is
// a checksum value. We are sending enouhg data for 16 inputs, but the EZ-Key
// only has 12. I suppose internally maybe the chipset is able to handle 16?
void updateEZKey()
{
  int  i, j;
  byte checksum;

  Serial.println();
  Serial.println(F("Sending HEX remap string to EZ-Key:"));
  Serial.println();

  // Calculate checksum byte, while outputing HEX string.
  Serial.print("s");
  EZKey.print("s");

  checksum = 0;
  
  for (i=0; i<INPUTS; i++)
  {
    printByteAsHex(input[i].modifier);
    checksum = checksum + input[i].modifier;

    // then a 0 byte, which we don't need to add to the checksum.
    Serial.print("00");
    EZKey.print("00");
    
    for (j=0; j<INPUT_KEYCODES; j++)
    {
      printByteAsHex(input[i].keycode[j]);
      checksum = checksum + input[i].keycode[j];
    }
    // Make console output look prettier.
    if (i % 4==3)
    {
      Serial.println();
      Serial.print(" ");
    }
  }
  // Then pad the rest (up to 16*8 bytes, 128 bytes total)
  for ( ; i<16; i++)
  {
    Serial.print(F("0000000000000000"));
    EZKey.print(F("0000000000000000"));

    // Make console output look prettier.
    if (i % 4==3)
    {
      Serial.println();
      Serial.print(" ");
    }
  }

  // Add the checksum byte (129).
  printByteAsHex(checksum);
  EZKey.print("\n");
  Serial.println();

  Serial.println();
  Serial.println(F("Waiting for confirmation from EZ-Key:"));
  Serial.println();

  // "OK" first... Then...
  if (expectFromEZKey("Set Mapping:", 2)==false)
  {
    Serial.println();
    Serial.println(F("ERROR: Timed out waiting for EZ-Key."));
  }
  else
  {
    // Flush out the rest of the data...
    while (EZKey.available()>0)
    {
      Serial.write(EZKey.read());
    }
    Serial.println();
    Serial.println(F("EZ-Key should now be remapped."));
  }
} // end of printRemapString()

/*---------------------------------------------------------------------------*/
// Print a HEX byte as two digits. The Serial.print(x, HEX) function does not
// 0-pad values less than 0x0F. i.e., it prints "F" instead of "0F".
void printByteAsHex(uint8_t value)
{
  if (value<=0x0F)
  {
    Serial.print("0");
    EZKey.print("0");
  }
  Serial.print(value, HEX);
  EZKey.print(value, HEX);
} // end of printByteAsHex()

/*---------------------------------------------------------------------------*/
// Currently uses RAM strings.
// TODO: Make it also use FLASH strings!
bool expectFromEZKey(char *string, uint8_t timeout)
{
  bool    status;
  uint8_t len;
  uint8_t matches = 0;
  char    ch;
  unsigned long timeoutTime;
  
  len = strlen(string);

  // Initial time to wait.
  timeoutTime = millis()+(timeout*1000); // 1000 ms per second.

  while(1)
  {
    // Check for timeout.
    if ( timeout>0 && (long)(millis()-timeoutTime) >= 0 )
    {
      // We have timed out.
      status = false;
      break;
    }
    
    if (EZKey.available()>0)
    {
      ch = EZKey.read();
      
      // Did we match the next character we expect?
      if (ch == string[matches])
      {
        matches++;
      }
      else // reset out match character...
      {
        matches = 0;
      }
      // Echo what we read.
      Serial.write(ch);
      
      // If we matched all the characters in the string, we are done.
      if (matches>=len)
      {
        // Match found!
        status = true;
        break;
      }
    } // end of if (EZKey.available()>0)
  } // end of while(1)
  
  return status;
} // end of expectFromEZKey()

/*---------------------------------------------------------------------------*/
// End of EZKeyRemapper

