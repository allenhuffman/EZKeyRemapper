// Due to bugs in the Arduino IDE, sometimes structures have to be placed
// in their own .h file.

#define INPUTS         12
#define INPUT_KEYCODES 6

typedef struct
{
  const uint8_t keycode;
  const char    *name;
} 
KeyCodeStruct;

typedef struct {
  uint8_t modifier;
  uint8_t keycode[INPUT_KEYCODES]; // Up to 6 per.
} 
RawHIDKeyboardReportStruct;

