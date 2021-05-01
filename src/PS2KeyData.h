/* Version V1.0.6
  PS2KeyMap.h - PS2KeyAdvanced library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Paul Carpenter, PC Services <sales@pcserviceselectronics.co.uk>
  Created September 2014
  Updated January 2016 - Paul Carpenter - add tested on Due and tidy ups for V1.5 Library Management
  January 2020 - Paul Carpenter - extend library properties for V2.2 of Arduino Library Management
    March 2020 - Paul Carpenter - add Spanish and Italian Mappings
    April 2020 - Paul Carpenter - Correct _KeyMaps definition error in last version

  PRIVATE to library data and key mapping tables

  This library REQUIRES PS2KeyAdvanced and PS2KeyMap.h as the codes used to
  remap to ASCII/UTF-8 are specific to that library to match ALL keys on a keyboard

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
#ifndef PS2KeyData_h
#define PS2KeyData_h

// Standard ASCII/UTF-8 control Characters
#define PS2_BACKSPACE  0x08
#define PS2_TAB        0x09
#define PS2_ENTER      0x0D
#define PS2_ESC        0x1B
#define PS2_DELETE     0x7F
#define PS2_SPACE      0x20


// Standard ASCII control characters array
// in order of PS2_KEY_* values. Order is important.
#if defined(PS2_REQUIRES_PROGMEM)
const uint8_t PROGMEM _control_codes[] = {
#else
const uint8_t _control_codes[] = {
#endif
  PS2_DELETE,
  PS2_ESC,
  PS2_BACKSPACE,
  PS2_TAB,
  PS2_ENTER,
  PS2_SPACE,
};


#if defined(PS2_REQUIRES_PROGMEM)
const uint16_t PROGMEM _US_ASCII[][2] = {
#else
const uint16_t _US_ASCII[][2] = {
#endif
  {PS2_SHIFT + PS2_KEY_1, '!'},
  {PS2_SHIFT + PS2_KEY_2, '@'},
  {PS2_SHIFT + PS2_KEY_3, '#'},
  {PS2_SHIFT + PS2_KEY_4, '$'},
  {PS2_SHIFT + PS2_KEY_5, '%'},
  {PS2_SHIFT + PS2_KEY_6, '^'},
  {PS2_SHIFT + PS2_KEY_7, '&'},
  {PS2_SHIFT + PS2_KEY_8, '*'},
  {PS2_SHIFT + PS2_KEY_9, '('},
  {PS2_SHIFT + PS2_KEY_0, ')'},
  {PS2_KEY_KP_DOT, '.'},
  {PS2_KEY_KP_ENTER, PS2_ENTER},
  {PS2_KEY_KP_PLUS, '+'},
  {PS2_KEY_KP_MINUS, '-'},
  {PS2_KEY_KP_TIMES, '*'},
  {PS2_KEY_KP_DIV, '/'},
  {PS2_KEY_KP_COMMA, ','},
  {PS2_KEY_KP_EQUAL, '='},
  {PS2_KEY_APOS, '\''},
  {PS2_SHIFT + PS2_KEY_APOS, '"'},
  {PS2_KEY_COMMA, ','},
  {PS2_SHIFT + PS2_KEY_COMMA, '<'},
  {PS2_KEY_MINUS, '-'},
  {PS2_SHIFT + PS2_KEY_MINUS, '_'},
  {PS2_KEY_DOT, '.'},
  {PS2_SHIFT + PS2_KEY_DOT, '>'},
  {PS2_KEY_DIV, '/'},
  {PS2_SHIFT + PS2_KEY_DIV, '?'},
  {PS2_KEY_SINGLE, '`'},
  {PS2_SHIFT + PS2_KEY_SINGLE, '~'},
  {PS2_KEY_SEMI, ';'},
  {PS2_SHIFT + PS2_KEY_SEMI, ':'},
  {PS2_KEY_BACK, '\\'},
  {PS2_SHIFT + PS2_KEY_BACK, '|'},
  {PS2_KEY_OPEN_SQ, '['},
  {PS2_SHIFT + PS2_KEY_OPEN_SQ, '{'},
  {PS2_KEY_CLOSE_SQ, ']'},
  {PS2_SHIFT + PS2_KEY_CLOSE_SQ, '}'},
  {PS2_KEY_EQUAL, '='},
  {PS2_SHIFT + PS2_KEY_EQUAL, '+'},
};

PS2KeyMap_t keyMap_UnitedStates = {"US", sizeof(_US_ASCII) / (2*sizeof(_US_ASCII[0][0])), _US_ASCII[0]};

#endif
