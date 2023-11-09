/*
   LiquidCrystalUI - Library
   https://github.com/MaxKliuba/LiquidCrystalUI
   by MaxClub Technology, 2022
*/

#pragma once

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "ILiquidCrystal.h"

class LiquidCrystalI2CWrapper : public ILiquidCrystal {
  public:
    LiquidCrystalI2CWrapper(LiquidCrystal_I2C* lcd, uint8_t cols, uint8_t rows) {
      _lcd = lcd;
      _cols = cols;
      _rows = rows;
    }

    LiquidCrystalI2CWrapper(uint8_t address, uint8_t cols, uint8_t rows) :
      LiquidCrystalI2CWrapper(new LiquidCrystal_I2C(address, cols, rows), cols, rows) {
    }

    void init() {
      _lcd->init();
    }

    void setBacklight(bool hasBacklight) {
      hasBacklight ? _lcd->backlight() : _lcd->noBacklight();
    }

    uint8_t getCols() {
      return _cols;
    }

    uint8_t getRows() {
      return _rows;
    }

    void clear() {
      _lcd->clear();
    }

    void createChar(uint8_t location, uint8_t charmap[]) {
      _lcd->createChar(location, charmap);
    }

    void setCursor(uint8_t col, uint8_t row) {
      _lcd->setCursor(col, row);
    }

    void setCursorVisibility(bool isVisible) {
      isVisible ? _lcd->cursor() : _lcd->noCursor();
    }

    void setCursorBlinking(bool isBlinking) {
      isBlinking ? _lcd->blink() : _lcd->noBlink();
    }

    void write(uint8_t value) {
      _lcd->write(value);
    }

    void print(const String &str) {
      _lcd->print(str);
    }

    void print(const __FlashStringHelper *str) {
      _lcd->print(str);
    }

  private:
    LiquidCrystal_I2C* _lcd;
    uint8_t _cols;
    uint8_t _rows;
};
