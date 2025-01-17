/* Version V1.0.5
  PS2KeyMap.cpp - PS2KeyMap library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Paul Carpenter, PC Services <sales@pcserviceselectronics.co.uk>
  Created September 2014
  Updated January 2016 - Paul Carpenter - add tested on Due and tidy ups for V1.5 Library Management
  January 2020 - Paul Carpenter - extend library properties for V2.2 of Arduino Library Management
    March 2020 - Paul Carpenter - add Spanish and Italian Mappings

  IMPORTANT WARNING
    If using a DUE or similar board with 3V3 I/O you MUST put a level translator
    like a Texas Instruments TXS0102 or FET circuit as the signals are
    Bi-directional (signals transmitted from both ends on same wire).

    Failure to do so may damage your Arduino Due or similar board.

  Test History
    September 2014 Uno and Mega 2560 September 2014 using Arduino V1.6.0
    January 2016   Uno, Mega 2560 and Due using Arduino 1.6.7 and Due Board
                    Manager V1.6.6

  This is for a LATIN style keyboard. Currently Supports
    US - Default
    UK - By selecting with string "UK" or "GB"
    DE - German if define GERMAN exists
    FR - French if define FRENCH exists
    ES - Spanish if define SPANISH exists
    IT - Italian if define ITALIAN exists
    --   Special if you have created your own mapping see PS2KeyMap.h

  US and UK mappings are base layouts always compiled. All mappings are done
  based on US mapping with CHANGE information to reduce storage space.

  To add other mappings

  Works with PS2KeyAdvanced library ONLY as this is an extension of that
  library.

  REQUIRES PS2KeyAdvanced library BEFORE THIS ONE

  To reduce flash memory footprint comment out any of the following in
  PSKeyMap.h and recompile

    // Uncomment any of following defines to include those mappings
    // or comment to exclude
    //#define GERMAN
    //#define FRENCH
    //#define SPANISH
    //#define ITALIAN
    //#define SPECIAL

  Library converts key codes from PS2KeyAdvanced to enable full ASCCII/UTF-8
  codes depending on language of keyboard and all the modifier keys or any
  combination of them e.g.
        SHIFT
        CTRL
        ALT
        ALT GR (Right ALT)
        GUI  (Windows key)

  Remap functions convert to USASCII and US keyboard first, then if an
  additional map is selected, scans that table for the few keys that are
  different. Making additional keyboard mappings easier, as well as
  enabling special key combinations to produce special codes. Also you
  can ignore key combinations.

  For example you could specify that CTRL + ENTER key produces the linefeed
  code, by adding one entry in a mapping table.

  Allows tables for any keyboard layout and special cases to be added
  Returns either uint8_t or uint16_t depending on which function is used

  When returning a uint8_t (byte) the code ranges are
        0       invalid/error
        1-1F    ASCII Control Codes (BS, TAB, ESC, ENTER, SPACE)
        20-7F   ASCII Printable characters and DEL
        80-9F   UTF-8 Control codes (none in base library)
        A0-FF   UTF-8 1 byte printable characters (See PS2KeyMap.h)

  When return an uint16_t containing
        Make/Break status
        Caps status
        Shift, CTRL, ALT, ALT GR, GUI keys
        Flag for function key not a displayable/printable character
        8 bit key code

    Top Byte is 8 bits denoting as follows with defines for bit code
        (see PS2KeyAdvanced.h for definitions)

        Define name bit     description
        PS2_BREAK   15      1 = Break key code
                   (MSB)    0 = Make Key code
        PS2_SHIFT   14      1 = Shift key pressed as well (either side)
                            0 = NO shift key
        PS2_CTRL    13      1 = Ctrl key pressed as well (either side)
                            0 = NO Ctrl key
        PS2_CAPS    12      1 = Caps Lock ON
                            0 = Caps lock OFF
        PS2_ALT     11      1 = Left Alt key pressed as well
                            0 = NO Left Alt key
        PS2_ALT_GR  10      1 = Right Alt (Alt GR) key pressed as well
                            0 = NO Right Alt key
        PS2_GUI      9      1 = GUI key pressed as well (either)
                            0 = NO GUI key
        PS2_FUNCTION 8      1 = FUNCTION key non-printable character (plus space, tab, enter)
                            0 = standard character key

  Code Ranges (bottom byte of uint16_t)
     When bit 8 = 0
        0       invalid/error
        1-1F    ASCII Control Codes (BS, TAB, ESC, ENTER, SPACE)
        20-7F   ASCII Printable characters and DEL
        80-9F   UTF-8 Control codes (none in base library)
        A0-FF   UTF-8 1 byte printable characters (See PS2KeyMap.h)

      When bit 8 = 1 Function keys see PS2KeyAdvanced for coding details)
        0       invalid/error
        1-19    Functions PGUP PGDN etc..
        1A-60   NOT RETURNED
        61-9F   Function keys and other special keys (plus F2 and F1)
                    61-78 F1 to F24
                    79-8A Multimedia
                    8B-8D ACPI power
                    90-9F Special multilingual
        A0-FF   Keyboard communications commands (note F2 and F1 are special
                codes for special multi-lingual keyboards)

  Error Codes
     Most functions return 0 or 0xFFFF as error, other codes to note and
     handle appropriately for bottom byte -
        0xAA   keyboard has reset and passed power up tests
               will happen if keyboard plugged in after code start
        0xFC   Keyboard General error or power up fail

  Functions contained are
    selectMap()  Pass in 2 digit ISO country code to select map if loaded
                 returns 0 for done or 1 for not found.

    remapKey()  Pass in unsigned int returned from PS2KeyAdvanced
                Returns 0 for error
                        unsigned int as described above
    remapKeyByte() Returns uint8_t version of remapKey ONLY for standard ASCII/UTF-8 codes
                   Invalid codes returned as 0

  To create your own map to ADD to this library see the readme.txt file in
  the library directory

  See PS2KeyAvanced.h for returned definitions of Keys and accessible
  definitions

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/* To Add a key mapping for one of the below add the line to YOUR CODE
   BEFORE including PS2KeyMap.h see examples for this library

    #define FRENCH
    #define GERMAN

   Add following line when you add you own map see #ifdef SPECIAL sections
   Selected by country code of "--" when created and enabled

     #define SPECIAL
*/

/*------------------ Code starts here -------------------------*/
#include <Arduino.h>
// Internal headers for library defines/codes/etc
#include <PS2KeyAdvanced.h>
#include "PS2KeyMap.h"
#include "PS2KeyData.h"


PS2KeyMap::PS2KeyMap() {
  setMap(NULL);
};


/**
 * Searches a key map for the given key combination and returns the
 * corresponding character, or 0 if not found.
 *
 * Assumes map table has 2 dimensions of type prog_uint16_t (in Flash memory)
 * i.e. an array like test[][2], where:
 * - First entry  (test[x][0]) is item to match
 * - Second entry (test[x][1]) is item to return
 *
 * Parameters are
 *      keyCode   unsigned int 16 from PS2KeyAdvanced::read().
 *      mapIndex  index of mapping table to use.
 */
uint8_t PS2KeyMap::scanMap(const uint16_t keyCode, const PS2KeyMap_t* keyMap) {
  const uint16_t* mapArray = keyMap->map;
  const uint16_t numWords = keyMap->numRows*2;  // Number of 16-bit ints in the map

  // Scan Lookup Table (array) jumping 2 integers (i.e. one entry) at a time
  for (uint16_t idx = 0; idx < numWords; idx += 2) {
#if defined(PS2_REQUIRES_PROGMEM)
    if (keyCode == pgm_read_word(mapArray + idx)) {
      return (pgm_read_word(mapArray + idx + 1) & 0xFF);
    }
#else
    if (keyCode == *(mapArray + idx)) {
      return (*(mapArray + idx + 1) & 0xFF);
    }
#endif
  }

  return 0;
}


void PS2KeyMap::setMap(PS2KeyMap_t* keyMap) {
  if (keyMap == NULL) {
    mSelectedMap = &keyMap_UnitedStates;
  }
  else {
    mSelectedMap = keyMap;
  }
}


const PS2KeyMap_t* PS2KeyMap::getMap() {
  return mSelectedMap;
}


uint16_t PS2KeyMap::remapKey(const uint16_t keyCode) {
  const uint8_t mapIndexUS = 0;
  const uint8_t bottomByte = keyCode & 0xFF;
  uint16_t returnCode = 0;

  if (bottomByte >= PS2_KEY_DELETE && bottomByte <= PS2_KEY_SPACE) {
    // Conversion of standard ASCII control codes
#if defined(PS2_REQUIRES_PROGMEM)
    uint8_t bottomByteAscii = pgm_read_byte(&_control_codes[bottomByte - PS2_KEY_DELETE]);
#else
    uint8_t bottomByteAscii = _control_codes[bottomByte - PS2_KEY_DELETE];
#endif
    returnCode = keyCode & ~PS2_FUNCTION;  // Remove the FUNCTION-bit from the top byte
    returnCode &= 0xFF00;  // Remove the bottom byte
    returnCode |= bottomByteAscii;  // Replace the bottom byte with the corresponding ASCII code
  }
  else if ((keyCode & PS2_FUNCTION) || (keyCode & PS2_BREAK) || bottomByte == 0xFA) {
    // Treat other function keys (and break/release) as non-printable.
    // The lock keys (num lock, scroll lock, caps lock) are received as 0xFA, don't print them.
    returnCode = 0;
  }
  else {
    uint8_t remappedChar = 0;

    if (mSelectedMap != &keyMap_UnitedStates) {
      remappedChar = scanMap(keyCode & (PS2_SHIFT + PS2_ALT_GR + 0x00FF), mSelectedMap);
    }

    if (remappedChar == 0) {
      // No value found in the country-specific map, check the US map instead
      remappedChar = scanMap(keyCode & (PS2_SHIFT + PS2_ALT_GR + 0x00FF), &keyMap_UnitedStates);
    }

    if (remappedChar == 0 && (keyCode & (PS2_CTRL + PS2_ALT + PS2_ALT_GR)) == 0) {
      // No value found in any map, try some standard replacements instead.
      // But only if no modifier keys (other than Shift) are pressed.
      if ((keyCode & PS2_SHIFT) == 0 && bottomByte >= PS2_KEY_A && bottomByte <= PS2_KEY_Z) {
        // Lower case a-z
        remappedChar = bottomByte + 0x20;
      }
      else if (bottomByte >= PS2_KEY_KP0 && bottomByte <= PS2_KEY_KP9) {
        // Convert KeyPad 0-9 to number codes
        remappedChar = bottomByte + 0x10;
      }
      else if ((keyCode & (PS2_CTRL + PS2_ALT + PS2_ALT_GR)) == 0) {
        // Use the default values from PS2KeyAdvanced.h (like 0-9 and A-Z), but only if
        // no modifier keys are pressed.
        remappedChar = bottomByte;
      }
    }

    if ((keyCode & PS2_CAPS) &&
        ((remappedChar >= 0x41 && remappedChar <= 0x5A) ||  // A-Z
        (remappedChar >= 0x61 && remappedChar <= 0x7A) ||  // a-z
        (remappedChar >= 0xC0 && remappedChar <= 0xFE &&  // À-þ...
        remappedChar != 0xF7 && remappedChar != 0xD7))) {  // ...but not × and ÷
      // When Caps Lock is active, change the case for letters like a-z, à, ö, ñ to
      // A-Z, À, Ö, Ñ - and vice versa.
      remappedChar ^= 0x20;
    }

    if (remappedChar > 0) {
      returnCode = (keyCode & 0xFF00) | ((uint16_t)remappedChar & 0x00FF);
    }
  }

  return returnCode;
}


uint8_t PS2KeyMap::remapKeyByte(const uint16_t code) {
  return (remapKey(code) & 0xFF);
}
