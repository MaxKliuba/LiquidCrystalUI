/*
 * LiquidCrystalUI - Library
 * https://github.com/MaxKliuba/LiquidCrystalUI
 * by MaxClub Technology, 2022
 */

#pragma once

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "ILiquidCrystal.h"

class LiquidCrystal_I2C_Wrapper : public ILiquidCrystal {
  public:
    LiquidCrystal_I2C_Wrapper(LiquidCrystal_I2C* _lcd, uint8_t _cols, uint8_t _rows) {
      lcd = _lcd;
      cols = _cols;
      rows = _rows;
    }

    uint8_t getCols() {
      return cols;
    }

    uint8_t getRows() {
      return rows;
    }

    void clear() {
      lcd->clear();
    }

    void createChar(uint8_t location, uint8_t charmap[]) {
      lcd->createChar(location, charmap);
    }

    void setCursor(uint8_t col, uint8_t row) {
      lcd->setCursor(col, row);
    }

    void write(uint8_t value) {
      lcd->write(value);
    }

    void print(const String &str) {
      lcd->print(str);
    }

  private:
    LiquidCrystal_I2C* lcd;
    uint8_t cols;
    uint8_t rows;
};
