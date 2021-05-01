/**
 * The key map shall only contain the differences from the US map.
 */
#pragma once

#define COUNTRY_CODE "SE"
#define KEY_MAP_NAME keyMap_Swedish

#if defined(PS2_REQUIRES_PROGMEM)
static const uint16_t PROGMEM keyMap[][2] = {
#else
static const uint16_t keyMap[][2] = {
#endif
  // Top row, without modifier keys
  {PS2_KEY_SINGLE, PS2_SECTION_SIGN},  // §
  {PS2_KEY_MINUS, '+'},
  {PS2_KEY_EQUAL, PS2_ACUTE_ACCENT},  // ´
  // Top row, with Sihft key
  {PS2_SHIFT + PS2_KEY_SINGLE, PS2_FRACTION_ONE_HALF},  // ½
  {PS2_SHIFT + PS2_KEY_2, '"'},
  {PS2_SHIFT + PS2_KEY_4, PS2_CURRENCY_SIGN},  // ¤
  {PS2_SHIFT + PS2_KEY_6, '&'},
  {PS2_SHIFT + PS2_KEY_7, '/'},
  {PS2_SHIFT + PS2_KEY_8, '('},
  {PS2_SHIFT + PS2_KEY_9, ')'},
  {PS2_SHIFT + PS2_KEY_0, '='},
  {PS2_SHIFT + PS2_KEY_MINUS, '?'},
  {PS2_SHIFT + PS2_KEY_EQUAL, '`'},
  // Top row, with Alt Gr key
  {PS2_ALT_GR + PS2_KEY_2, '@'},
  {PS2_ALT_GR + PS2_KEY_3, PS2_POUND_SIGN},  // £
  {PS2_ALT_GR + PS2_KEY_4, '$'},
  {PS2_ALT_GR + PS2_KEY_7, '{'},
  {PS2_ALT_GR + PS2_KEY_8, '['},
  {PS2_ALT_GR + PS2_KEY_9, ']'},
  {PS2_ALT_GR + PS2_KEY_0, '}'},
  {PS2_ALT_GR + PS2_KEY_MINUS, '\\'},
  // Second row, without modifier keys
  {PS2_KEY_OPEN_SQ, PS2_a_RING_ABOVE},  // å
  {PS2_KEY_CLOSE_SQ, PS2_DIAERESIS},  // ¨
  // Second row, with Shift key
  {PS2_SHIFT + PS2_KEY_OPEN_SQ, PS2_A_RING_ABOVE},  // Å
  {PS2_SHIFT + PS2_KEY_CLOSE_SQ, '^'},
  // Second row, with Alt Gr key
  {PS2_ALT_GR + PS2_KEY_CLOSE_SQ, '~'},
  // Third row, without modifier keys
  {PS2_KEY_SEMI, PS2_o_DIAERESIS},  // ö
  {PS2_KEY_APOS, PS2_a_DIAERESIS}, // ä
  {PS2_KEY_BACK, '\''},
  // Third row, with Shift key
  {PS2_SHIFT + PS2_KEY_SEMI, PS2_O_DIAERESIS},  // Ä
  {PS2_SHIFT + PS2_KEY_APOS, PS2_A_DIAERESIS},  // Ö
  {PS2_SHIFT + PS2_KEY_BACK, '*'},
  // Fourth row, without modifier keys
  {PS2_KEY_EUROPE2, '<'},
  {PS2_KEY_DIV, '-'},
  // Fourth row, with Shift key
  {PS2_SHIFT + PS2_KEY_EUROPE2, '>'},
  {PS2_SHIFT + PS2_KEY_COMMA, ';'},
  {PS2_SHIFT + PS2_KEY_DOT, ':'},
  {PS2_SHIFT + PS2_KEY_DIV, '_'},
  // Fourth row, with Alt Gr key
  {PS2_ALT_GR + PS2_KEY_EUROPE2, '|'},
  {PS2_ALT_GR + PS2_KEY_M, PS2_MICRO_SIGN},  // µ
};

PS2KeyMap_t KEY_MAP_NAME = {COUNTRY_CODE, sizeof(keyMap) / (2*sizeof(keyMap[0][0])), keyMap[0]};

#undef COUNTRY_CODE
#undef KEY_MAP_NAME
