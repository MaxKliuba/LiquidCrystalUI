/*
   LiquidCrystalUI - Library
   https://github.com/MaxKliuba/LiquidCrystalUI
   by MaxClub Technology, 2022
*/

#pragma once

class ILiquidCrystal {
  public:
    virtual void init() = 0;

    virtual void setBacklight(bool hasBacklight) = 0;

    virtual uint8_t getCols() = 0;

    virtual uint8_t getRows() = 0;

    virtual void clear() = 0;

    virtual void createChar(uint8_t location, uint8_t charmap[]) = 0;

    virtual void setCursor(uint8_t col, uint8_t row) = 0;

    virtual void setCursorVisibility(bool isVisible) = 0;

    virtual void setCursorBlinking(bool isBlinking) = 0;

    virtual void write(uint8_t value) = 0;

    virtual void print(const String &str) = 0;

    virtual void print(const __FlashStringHelper *str) = 0;
};
