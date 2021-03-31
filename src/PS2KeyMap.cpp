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


/* Searches a key map for the given key combination and returns the
   corresponding character, or 0 if not found.

   Checks using default mask of keycode plus ALT_GR and SHIFT.
   To match other modifiers, you need to modify this function and set the
   mask to a combination of:
   - PS2_CTRL  match either Control key as well
   - PS2_ALT   match left ALT as well
   - PS2_GUI   match either GUI key as well

   Assumes map table has 2 dimensions of type prog_uint16_t (in Flash memory)
   i.e. an array like test[][2], where:
   - First entry  (test[x][0]) is item to match
   - Second entry (test[x][1]) is item to return

   Parameters are
        keyCode   unsigned int 16 from PS2KeyAdvanced::read().
        mapIndex  index of mapping table to use.
*/
uint16_t PS2KeyMap::scanMap(uint16_t keyCode, uint8_t mapIndex) {
  const uint16_t *mapArray = _KeyMaps[mapIndex].map;
  const uint16_t numWords = _KeyMaps[mapIndex].numEntries*2;  // Number of 16-bit ints in the map
  const uint16_t lookupCode = keyCode & (PS2_ALT_GR + PS2_SHIFT + 0xFF);  // Leave only required bits for checking against

  // Scan Lookup Table (array) jumping 2 integers (i.e. one entry) at a time
  for (uint16_t idx = 0; idx < numWords; idx += 2) {
#if defined(PS2_REQUIRES_PROGMEM)
    if (lookupCode == pgm_read_word(mapArray + idx)) {
      keyCode &= ~0xFF;  // clear old bits retaining CTRL etc
      keyCode |= pgm_read_word(mapArray + idx + 1);  // Make new code
      break;
    }
#else
    if (lookupCode == *(mapArray + idx)) {
      keyCode &= ~0xFF;  // clear old bits retaining CTRL etc
      keyCode |= *(mapArray + idx + 1);  // Make new code
      break;
    }
#endif
  }

  return keyCode;
}


uint8_t PS2KeyMap::selectMap(char* countryCode) {
  const uint8_t numMaps = sizeof(_KeyMaps) / sizeof(_KeyMaps[0]);

  for (uint8_t i = 0; i < numMaps; i++) {
    // Compare both the letters and the null terminator
    if (memcmp(countryCode, &_KeyMaps[i].name, 3) == 0) {
      mSelectedMap = i;
      return 1;
    }
  }

  return 0;
}


const char* PS2KeyMap::getMap(void) {
  return _KeyMaps[mSelectedMap].name;
}


uint16_t PS2KeyMap::remapKey(uint16_t keyCode) {
  uint16_t returnCode = 0;
  const uint8_t bottomByte = keyCode & 0xFF;

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
  else if (keyCode & PS2_FUNCTION) {
    // Treat other function keys as non-printable
    returnCode = 0;
  }
  else {
    // Some codes are straight return values. We deal with ones that need work.
    if (bottomByte >= PS2_KEY_A && bottomByte <= PS2_KEY_Z) {
      returnCode = keyCode;

      // Deal with Caps Lock by inverting PS2_SHIFT and remove caps lock
      if (keyCode & PS2_CAPS) {
        returnCode ^= (PS2_SHIFT + PS2_CAPS);
      }

      if (!(returnCode & PS2_SHIFT)) {
        // When A-Z not Shifted, create lower case a-z.
        returnCode |= 0x20;
      }
      else {
        // When Shifted, pass through code as received as the codes are A-Z
        returnCode &= ~PS2_SHIFT;
      }
    }
    else if (bottomByte >= PS2_KEY_KP0 && bottomByte <= PS2_KEY_KP9) {
      // Convert KeyPad 0-9 to number codes
      returnCode = keyCode | 0x10;
      returnCode &= ~(PS2_SHIFT + PS2_ALT + PS2_ALT_GR + PS2_CTRL + PS2_GUI + PS2_CAPS);
    }
    else {
      returnCode = keyCode;

      // Some keys ignore Shift, like the extra keys on numeric keypad
      if ((bottomByte >= PS2_KEY_KP_DOT && bottomByte <= PS2_KEY_KP_DIV) ||
          bottomByte == PS2_KEY_KP_EQUAL || bottomByte == PS2_KEY_KP_COMMA) {
        returnCode &= ~PS2_SHIFT;
      }

      // Do default US-ASCII mapping of remainders excluding (CTRL, ALT, GUI)
      const uint8_t mapIndexUS = 0;
      returnCode = (returnCode & PS2_ALT_GR) + scanMap(returnCode & ~PS2_ALT_GR, mapIndexUS);
    }

    // Now do remap if necessary for all modifiers
    if (returnCode > 0 && mSelectedMap > 0) {
      returnCode = scanMap(returnCode, mSelectedMap);
    }
    else if (bottomByte == PS2_KEY_EUROPE2) {
      // For US-ASCII ignore extra key in case
      returnCode = 0;
    }

    returnCode &= ~(PS2_SHIFT + PS2_ALT_GR);  // Now clear Shift and AltGr status
  }

  return returnCode;
}


/*
   Returns uint8_t version of remapKey ONLY for standard ASCII/UTF-8 codes
   Invalid codes returned as 0
   Also any key with BREAK condition are returned as 0
*/
uint8_t PS2KeyMap::remapKeyByte(uint16_t code) {
  uint16_t data;

  // Invalid codes returned as 0
  if(code & (PS2_FUNCTION + PS2_BREAK)) {
    return 0;
  }

  data = remapKey(code);
  return (uint8_t)(data & 0xFF);
}
