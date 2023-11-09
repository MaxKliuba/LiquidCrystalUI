/*
   LiquidCrystalUI - Library
   https://github.com/MaxKliuba/LiquidCrystalUI
   by MaxClub Technology, 2022
*/

#pragma once

#ifndef _POINTER_CHARMAP_LOCATION
#define _POINTER_CHARMAP_LOCATION 0
#endif

#ifndef _BACK_ARROW_CHARMAP_LOCATION
#define _BACK_ARROW_CHARMAP_LOCATION 1
#endif

#ifndef _MENU_CHARMAP_LOCATION
#define _MENU_CHARMAP_LOCATION 1
#endif

#ifndef _DOWN_ARROW_CHARMAP_LOCATION
#define _DOWN_ARROW_CHARMAP_LOCATION 2
#endif

#ifndef _ELLIPSIS_CHARMAP_LOCATION
#define _ELLIPSIS_CHARMAP_LOCATION 3
#endif

#ifndef _APPLY_CHARMAP_LOCATION
#define _APPLY_CHARMAP_LOCATION 4
#endif

#ifndef _LEFT_BORDER_CHARMAP_LOCATION
#define _LEFT_BORDER_CHARMAP_LOCATION 5
#endif

#ifndef _RIGHT_BORDER_CHARMAP_LOCATION
#define _RIGHT_BORDER_CHARMAP_LOCATION 6
#endif

#ifndef _OUTLINE_BORDER_CHARMAP_LOCATION
#define _OUTLINE_BORDER_CHARMAP_LOCATION 7
#endif

#ifndef _PROGRESS_BAR_LEFT_CHARMAP_LOCATION
#define _PROGRESS_BAR_LEFT_CHARMAP_LOCATION 5
#endif

#ifndef _PROGRESS_BAR_MIDDLE_CHARMAP_LOCATION
#define _PROGRESS_BAR_MIDDLE_CHARMAP_LOCATION 7
#endif

#ifndef _PROGRESS_BAR_RIGHT_CHARMAP_LOCATION
#define _PROGRESS_BAR_RIGHT_CHARMAP_LOCATION 6
#endif

#ifndef _GRAPH_1_CHARMAP_LOCATION
#define _GRAPH_1_CHARMAP_LOCATION 0
#endif

#ifndef _GRAPH_2_CHARMAP_LOCATION
#define _GRAPH_2_CHARMAP_LOCATION 2
#endif

#ifndef _GRAPH_3_CHARMAP_LOCATION
#define _GRAPH_3_CHARMAP_LOCATION 3
#endif

#ifndef _GRAPH_4_CHARMAP_LOCATION
#define _GRAPH_4_CHARMAP_LOCATION 4
#endif

#ifndef _GRAPH_5_CHARMAP_LOCATION
#define _GRAPH_5_CHARMAP_LOCATION 5
#endif

#ifndef _GRAPH_6_CHARMAP_LOCATION
#define _GRAPH_6_CHARMAP_LOCATION 6
#endif

#ifndef _GRAPH_7_CHARMAP_LOCATION
#define _GRAPH_7_CHARMAP_LOCATION 7
#endif

const uint8_t _GRAPH_CHARMAP_LOCATIONS[7] = {
  _GRAPH_1_CHARMAP_LOCATION,
  _GRAPH_2_CHARMAP_LOCATION,
  _GRAPH_3_CHARMAP_LOCATION,
  _GRAPH_4_CHARMAP_LOCATION,
  _GRAPH_5_CHARMAP_LOCATION,
  _GRAPH_6_CHARMAP_LOCATION,
  _GRAPH_7_CHARMAP_LOCATION
};

const uint8_t _BLANK_CHAR = 16;
const uint8_t _FILLED_CHAR = 255;

const uint8_t _POINTER_CHARMAP[8] = {
  0b00000,
  0b00100,
  0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b00000,
  0b00000,
};
const uint8_t _SELECTED_POINTER_CHARMAP[8] = {
  0b11111,
  0b11011,
  0b11101,
  0b00000,
  0b11101,
  0b11011,
  0b11111,
  0b11111,
};

const uint8_t _BACK_ARROW_CHARMAP[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100,
  0b00111,
  0b00000,
};
const uint8_t _SELECTED_BACK_ARROW_CHARMAP[8] = {
  0b11011,
  0b10001,
  0b01010,
  0b11011,
  0b11011,
  0b11011,
  0b11000,
  0b11111,
};

const uint8_t _MENU_CHARMAP[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b10100,
  0b10010,
  0b11111,
  0b00010,
  0b00100,
};
const uint8_t _SELECTED_MENU_CHARMAP[8] = {
  0b01111,
  0b01111,
  0b01111,
  0b01011,
  0b01101,
  0b00000,
  0b11101,
  0b11011,
};

const uint8_t _DOWN_ARROW_CHARMAP[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b10001,
  0b01010,
  0b00100,
  0b00000,
};

const uint8_t _ELLIPSIS_CHARMAP[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b10101,
  0b00000,
};

const uint8_t _APPLY_CHARMAP[8] = {
  0b00000,
  0b00001,
  0b00011,
  0b10110,
  0b11100,
  0b01000,
  0b00000,
  0b00000,
};
const uint8_t _SELECTED_APPLY_CHARMAP[8] = {
  0b11111,
  0b11110,
  0b11100,
  0b01001,
  0b00011,
  0b10111,
  0b11111,
  0b11111,
};

const uint8_t _LEFT_BORDER_CHARMAP[8] = {
  0b11111,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b11111,
};

const uint8_t _RIGHT_BORDER_CHARMAP[8] = {
  0b10101,
  0b00000,
  0b00001,
  0b00000,
  0b00000,
  0b00001,
  0b00000,
  0b10101,
};

const uint8_t _OUTLINE_BORDER_CHARMAP[8] = {
  0b10101,
  0b00000,
  0b10001,
  0b00000,
  0b00000,
  0b10001,
  0b00000,
  0b10101,
};

const uint8_t _PROGRESS_BAR_LEFT_CHARMAP[8] = {
  0b11111,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b11111,
};

const uint8_t _PROGRESS_BAR_MIDDLE_CHARMAP[8] = {
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
};

const uint8_t _PROGRESS_BAR_RIGHT_CHARMAP[8] = {
  0b11111,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b11111,
};

const uint8_t _GRAPH_1_CHARMAP[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
};

const uint8_t _GRAPH_2_CHARMAP[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
};

const uint8_t _GRAPH_3_CHARMAP[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
};

const uint8_t _GRAPH_4_CHARMAP[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};

const uint8_t _GRAPH_5_CHARMAP[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};

const uint8_t _GRAPH_6_CHARMAP[8] = {
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};

const uint8_t _GRAPH_7_CHARMAP[8] = {
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};