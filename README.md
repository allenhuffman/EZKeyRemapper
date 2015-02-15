EZKeyRemapper
=============

This program will remap the keys of an Adafruit Bluefruit EZ-Key over a TX/RX
UART serial connection. 
 
Hardware:

http://www.adafruit.com/bluefruit
 
Documentation:
 
http://learn.adafruit.com/introducing-bluefruit-ez-key-diy-bluetooth-hid-keyboard/overview
 
See my original articles:

http://subethasoftware.com/2014/03/19/adafruit-bluefruit-ez-key/
http://subethasoftware.com/2014/03/20/arduino-based-adafruit-ez-key-remapper/


REVISION
========
* 2014-03-22 0.00 allenh - Created, based on EZKeyDemo source.
* 2014-03-26 0.01 allenh - Cleaning up code.
* 2015-02-14 0.02 allenh - Adding some "const" to make it build with 1.6.0.

FILES
=====

* README.md - this file
* EZKeyRemapper.ino - the main function
* EZKey.h - HID keyboard/mouse definitions.
* EZKeyStructures.h - structure placed in its own file to work around IDE bug.
* LineInput.ino - line input routine

CONFIGURATION
=============

 Define the pins on the Arduino that will be used for TX and RX in the
 Software Serial library, and connected to the Bluefruit EZ-KEY.

```
/*---------------------------------------------------------------------------*/
// CONFIGURATION
/*---------------------------------------------------------------------------*/
#define RX_PIN         10
#define TX_PIN         11
```

RUNNING
=======
 
Basically, for each of the 12 inputs you can specify a modifier (SHIFT, CTRL,
ALT, etc.) plus up to 6 keycodes which will be sent out (with a Keys-Down) via
Bluetooth. When the input is released, a matching Keys-Up message is sent.

To customize, you simply need to specify an input (0-11) and then a modifier
(8 available choices) and then up to 6 keycodes (about 112 options available,
such as ARROW UP or “k” or numeric keypad minus).

See the original article for a walk-through.

Hopefully the user interface will be self-explanatory.
 
More to come...
