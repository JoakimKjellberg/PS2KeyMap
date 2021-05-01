/*  international keyboard mapping example to serial port

    Example keyboard on Arduino to Serial port using baud of 115,200

    PS2KeyMap extension library for PS2KeyAdvanced library,
    Key mapping to ASCII/UTF-8 mapping and International Keyboard
    Layout Example.

  IMPORTANT WARNING

    If using a DUE or similar board with 3V3 I/O you MUST put a level translator
    like a Texas Instruments TXS0102 or FET circuit as the signals are
    Bi-directional (signals transmitted from both ends on same wire).

    Failure to do so may damage your Arduino Due or similar board.

  Test History
    September 2014 Uno and Mega 2560 September 2014 using Arduino V1.6.0
    January 2016   Uno, Mega 2560 and Due using Arduino 1.6.7 and Due Board
                    Manager V1.6.6
    March 2020  Extend for Italian and Spanish keyboards

  PS2KeyMap uses a default US-ASCII Map but different country
  mappings can be selected on the fly

  Map to the keyboard you want when running by typing

    U for US keyboard
    G for UK keyboard
    D for German keyboard
    F for French keyboard
    I for Italian keyboard
    E for Spanish keyboard

  Defaults to US on start up

  The circuit:
   * KBD Clock (PS2 pin 1) to an interrupt pin on Arduino (this example pin 3)
   * KBD Data (PS2 pin 5) to a data pin (this example pin 4)
   * +5V from Arduino to PS2 pin 1
   * GND from Arduino to PS2 pin 3

   The connector to mate with PS2 keyboard is a 6 pin Female Mini-Din connector
   PS2 Pins to signal
    1       KBD Data
    3       GND
    4       +5V
    5       KBD Clock

   Keyboard has 5V and GND connected see plenty of examples and
   photos around on Arduino site and other sites about the PS2 Connector.

 Interrupts

   Clock pin from PS2 keyboard MUST be connected to an interrupt
   pin, these vary with the different types of Arduino

   For PS2KeyAdvanced you pass this info into begin()

     keyboard.begin(DATAPIN, IRQPIN);

  Valid irq pins:
     Arduino Uno:  2, 3
     Arduino Due:  All pins, except 13 (LED)
     Arduino Mega: 2, 3, 18, 19, 20, 21
     Teensy 2.0:   All pins, except 13 (LED)
     Teensy 2.0:   5, 6, 7, 8
     Teensy 1.0:   0, 1, 2, 3, 4, 6, 7, 16
     Teensy++ 2.0: 0, 1, 2, 3, 18, 19, 36, 37
     Teensy++ 1.0: 0, 1, 2, 3, 18, 19, 36, 37
     Sanguino:     2, 10, 11

  Like the Original library and example this is under LGPL license.

  Written by Paul Carpenter, PC Services <sales@pcserviceselectronics.co.uk>
*/

#include <PS2KeyAdvanced.h>
#include <PS2KeyMap.h>

// Include all key maps
#include <PS2KeyMaps/Swedish.h>

/* Keyboard constants  Change to suit your Arduino
   define pins used for data and clock from keyboard */
#define DATAPIN 4
#define IRQPIN  3

PS2KeyAdvanced keyboard;
PS2KeyMap keymap;

uint16_t code;
bool mapChanged;


void setup() {
  Serial.begin(115200);
  Serial.println("PS2KeyMap plus PS2KeyAdvanced Libraries");
  Serial.println("International Keyboard Test:");
  Serial.println("Default is US layout, type a key to change layout");
  Serial.println(" U for US     G for GB/UK");
  Serial.println(" D for DE     F for FR");
  Serial.println(" I for IT     E for ES");
  Serial.println(" S for SE     X for SPECIAL");
  Serial.println(" All keys on keyboard echoed here");
  // Start keyboard setup while outputting
  keyboard.begin(DATAPIN, IRQPIN);
  // Disable Break codes (key release) from PS2KeyAdvanced
  keyboard.setNoBreak(1);
  // and set no repeat on CTRL, ALT, SHIFT, GUI while outputting
  keyboard.setNoRepeat(1);
}


void loop() {
  code = keyboard.available();
  if (code > 0) {
    code = keyboard.read();
    Serial.print("Value 0x");
    Serial.print(code, HEX);

    code = keymap.remapKey(code);
    if (code > 0) {
      if (code & 0xFF) {
        Serial.print(" mapped 0x");
        Serial.print(code, HEX);
        Serial.print(" - Status Bits 0x");
        Serial.print(code >> 8, HEX);
        Serial.print("  Code 0x");
        Serial.print(code & 0xFF, HEX);
        Serial.print("  (");
        Serial.write(code & 0xFF);
        Serial.print(")\n");
      }

      // process special commands
      mapChanged = false;
      switch (code & 0xFF) {
        case 'D':
        case 'd':
                //keymap.selectMap(&keyMap_German);
                //mapChanged = true;
                break;
        case 'F':
        case 'f':
                //keymap.selectMap(&keyMap_French);
                //mapChanged = true;
                break;
        case 'E':
        case 'e':
                //keymap.selectMap(&keyMap_Spanish);
                //mapChanged = true;
                break;
        case 'I':
        case 'i':
                //keymap.selectMap(&keyMap_Italian);
                //mapChanged = true;
                break;
        case 'G':
        case 'g':
                //keymap.selectMap(&keyMap_GreatBritain);
                //mapChanged = true;
                break;
        case 'U':
        case 'u':
                keymap.selectMap(NULL);  // NULL -> United States
                mapChanged = true;
                break;
        case 'S':
        case 's':
                keymap.selectMap(&keyMap_Swedish);
                mapChanged = true;
                break;
        case 'X':
        case 'x':
                //keymap.selectMap(&keyMap_MyCustomMap);
                //mapChanged = true;
                break;
      }

      if (mapChanged) {
        Serial.print("Keyboard set to ");
        Serial.println(keymap.getMap()->countryCode);
      }
    }
    else {
      Serial.println(" Keyboard protocol or function");
    }
  }

  delay(100);
}