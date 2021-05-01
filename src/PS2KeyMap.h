/* Version V1.0.5
  PS2KeyMap.h - PS2KeyAdvanced library
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

  Takes output key codes from PS2KeyAdvanced and translates them to ASCII/UTF-8
  if valid as these characters, or returns PS2KeyAdvanced codings for function keys
  as these do NOT have any standard ASCII/UTF-8 encodings.

  Use defines from here and PS2KeyAdvanced.h to make reference to key codes

  This library REQUIRES PS2KeyAdvanced as the codes used to remap to ASCII/UTF-8
  are specific to that library to match ALL keys on a keyboard

  To reduce flash memory footprint comment out any of the following in
  PSKeyMap.h and recompile

    // Uncomment any of following defines to include those mappings
    // or comment to exclude
    //#define GERMAN
    //#define FRENCH
    //#define SPANISH
    //#define ITALIAN
    //#define SPECIAL

  The functions in this library takes the unsigned int values produced from
  PS2KeyAdvanced and translate them as follows into an unsigned int value

        0       invalid/error
        1-1F    ASCII Control Codes (BS, TAB, ESC, ENTER, SPACE)
        20-7F   ASCII Printable characters and DEL
        80-9F   UTF-8 Control codes (none in base library)
        A0-FF   UTF-8 1 byte printable characters (See PS2KeyMap.h)

  All other codes for F1 to F24 (yes there are 24 function key codes) arrow keys page up
  page down etc are returned as the same codes as PS2KeyAdvanced uses.

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

#ifndef PS2KeyMap_h
#define PS2KeyMap_h

#ifndef PS2KeyAdvanced_h
  #error PS2KeyAdvanced library missing
#endif

/* UTF-8 single byte LATIN encodings
   128 to 159 (0x80 to 0x9F) are control characters application generated
   160 to 255 (0xA0 to 0XFF) are used depending on keymap tables
   These defines are used to insert them into correct tables */

/****************************************************************
  IMPORTANT NOTE EURO currency Symbol is NOT supported in UTF-8 single
  byte codings, as EURO symbol came after UTF-8 single byte codings
****************************************************************/
#define PS2_NO_BREAK_SPACE            160 // (0xA0) nbsp
#define PS2_INVERTED_EXCLAMATION      161 // (0xA1) ¡
#define PS2_CENT_SIGN                 162 // (0xA2) ¢
#define PS2_POUND_SIGN                163 // (0xA3) £
#define PS2_CURRENCY_SIGN             164 // (0xA4) ¤
#define PS2_YEN_SIGN                  165 // (0xA5) ¥
#define PS2_BROKEN_BAR                166 // (0xA6) ¦
#define PS2_SECTION_SIGN              167 // (0xA7) §
#define PS2_DIAERESIS                 168 // (0xA8) ¨
#define PS2_COPYRIGHT_SIGN            169 // (0xA9) ©
#define PS2_FEMININE_ORDINAL          170 // (0xAA) ª
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE   171 // (0xAB) «
#define PS2_NOT_SIGN                  172 // (0xAC) ¬
#define PS2_HYPHEN                    173 // (0xAD) -
#define PS2_REGISTERED_SIGN           174 // (0xAE) ®
#define PS2_MACRON                    175 // (0xAF) ¯
#define PS2_DEGREE_SIGN               176 // (0xB0) °
#define PS2_PLUS_MINUS_SIGN           177 // (0xB1) ±
#define PS2_SUPERSCRIPT_TWO           178 // (0xB2) ²
#define PS2_SUPERSCRIPT_THREE         179 // (0xB3) ³
#define PS2_ACUTE_ACCENT              180 // (0xB4) ´
#define PS2_MICRO_SIGN                181 // (0xB5) µ
#define PS2_PILCROW_SIGN              182 // (0xB6) ¶
#define PS2_MIDDLE_DOT                183 // (0xB7) ·
#define PS2_CEDILLA                   184 // (0xB8) ¸
#define PS2_SUPERSCRIPT_ONE           185 // (0xB9) ¹
#define PS2_MASCULINE_ORDINAL         186 // (0xBA) º
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE  187 // (0xBB) »
#define PS2_FRACTION_ONE_QUARTER      188 // (0xBC) ¼
#define PS2_FRACTION_ONE_HALF         189 // (0xBD) ½
#define PS2_FRACTION_THREE_QUARTERS   190 // (0xBE) ¾
#define PS2_INVERTED_QUESTION_MARK    191 // (0xBF) ¿
#define PS2_A_GRAVE                   192 // (0xC0) À
#define PS2_A_ACUTE                   193 // (0xC1) Á
#define PS2_A_CIRCUMFLEX              194 // (0xC2) Â
#define PS2_A_TILDE                   195 // (0xC3) Ã
#define PS2_A_DIAERESIS               196 // (0xC4) Ä
#define PS2_A_RING_ABOVE              197 // (0xC5) Å
#define PS2_AE                        198 // (0xC6) Æ
#define PS2_C_CEDILLA                 199 // (0xC7) Ç
#define PS2_E_GRAVE                   200 // (0xC8) È
#define PS2_E_ACUTE                   201 // (0xC9) É
#define PS2_E_CIRCUMFLEX              202 // (0xCA) Ê
#define PS2_E_DIAERESIS               203 // (0xCB) Ë
#define PS2_I_GRAVE                   204 // (0xCC) Ì
#define PS2_I_ACUTE                   205 // (0xCD) Í
#define PS2_I_CIRCUMFLEX              206 // (0xCE) Î
#define PS2_I_DIAERESIS               207 // (0xCF) Ï
#define PS2_ETH                       208 // (0xD0) Ð
#define PS2_N_TILDE                   209 // (0xD1) Ñ
#define PS2_O_GRAVE                   210 // (0xD2) Ò
#define PS2_O_ACUTE                   211 // (0xD3) Ó
#define PS2_O_CIRCUMFLEX              212 // (0xD4) Ô
#define PS2_O_TILDE                   213 // (0xD5) Õ
#define PS2_O_DIAERESIS               214 // (0xD6) Ö
#define PS2_MULTIPLICATION            215 // (0xD7) ×
#define PS2_O_STROKE                  216 // (0xD8) Ø
#define PS2_U_GRAVE                   217 // (0xD9) Ù
#define PS2_U_ACUTE                   218 // (0xDA) Ú
#define PS2_U_CIRCUMFLEX              219 // (0xDB) Û
#define PS2_U_DIAERESIS               220 // (0xDC) Ü
#define PS2_Y_ACUTE                   221 // (0xDD) Ý
#define PS2_THORN                     222 // (0xDE) Þ
#define PS2_SHARP_S                   223 // (0xDF) ß
#define PS2_a_GRAVE                   224 // (0xE0) à
#define PS2_a_ACUTE                   225 // (0xE1) á
#define PS2_a_CIRCUMFLEX              226 // (0xE2) â
#define PS2_a_TILDE                   227 // (0xE3) ã
#define PS2_a_DIAERESIS               228 // (0xE4) ä
#define PS2_a_RING_ABOVE              229 // (0xE5) å
#define PS2_ae                        230 // (0xE6) æ
#define PS2_c_CEDILLA                 231 // (0xE7) ç
#define PS2_e_GRAVE                   232 // (0xE8) è
#define PS2_e_ACUTE                   233 // (0xE9) é
#define PS2_e_CIRCUMFLEX              234 // (0xEA) ê
#define PS2_e_DIAERESIS               235 // (0xEB) ë
#define PS2_i_GRAVE                   236 // (0xEC) ì
#define PS2_i_ACUTE                   237 // (0xED) í
#define PS2_i_CIRCUMFLEX              238 // (0xEE) î
#define PS2_i_DIAERESIS               239 // (0xEF) ï
#define PS2_eth                       240 // (0xF0) ð
#define PS2_n_TILDE                   241 // (0xF1) ñ
#define PS2_o_GRAVE                   242 // (0xF2) ò
#define PS2_o_ACUTE                   243 // (0xF3) ó
#define PS2_o_CIRCUMFLEX              244 // (0xF4) ô
#define PS2_o_TILDE                   245 // (0xF5) õ
#define PS2_o_DIAERESIS               246 // (0xF6) ö
#define PS2_DIVISION                  247 // (0xF7) ÷
#define PS2_o_STROKE                  248 // (0xF8) ø
#define PS2_u_GRAVE                   249 // (0xF9) ù
#define PS2_u_ACUTE                   250 // (0xFA) ú
#define PS2_u_CIRCUMFLEX              251 // (0xFB) û
#define PS2_u_DIAERESIS               252 // (0xFC) ü
#define PS2_y_ACUTE                   253 // (0xFD) ý
#define PS2_thorn                     254 // (0xFE) þ
#define PS2_y_DIAERESIS               255 // (0xFF) ÿ


// Meta data of a key map.
typedef struct {
  const char countryCode[3];  // ISO country code (2 chars and null).
  uint8_t numRows;  // Number of rows in the map array.
  const uint16_t* map;  // Map array pointer.
} PS2KeyMap_t;


class PS2KeyMap {
 public:
  /**
   * This constructor sets the default key mapping to US.
   */
  PS2KeyMap();

  /**
   * Sets the map pointer to the given key map. If NULL is passed, the US key map
   * is selected.
   *
   * A map can be included by using #include <PS2KeyMaps/Swedish.h>, which defines
   * the variable keyMap_Swedish.
   */
  void setMap(PS2KeyMap_t* keyMap);

  /**
   * Returns the selected map as a char pointer (2 chars and terminator).
   */
  const PS2KeyMap_t* getMap();

  /**
   * Remaps the key code returned from PS2KeyAdvanced to a UTF-8 number (1-255).
   * Leaves the status bits (the top byte) unchanged. Invalid codes returned as 0.
   *
   * Parameter keyCode  The value returned by PS2KeyAdvanced::read().
   */
  uint16_t remapKey(const uint16_t keyCode);

  /**
   * Returns uint8_t version of remapKey ONLY for standard ASCII/UTF-8 codes.
   * Invalid codes returned as 0.
   */
  uint8_t remapKeyByte(const uint16_t keyCode);

 private:
  uint8_t scanMap(const uint16_t keyCode, const PS2KeyMap_t* keyMap);

  PS2KeyMap_t* mSelectedMap;
};

#endif  // PS2KeyMap_h
