/*
   LiquidCrystalUI - Library
   https://github.com/MaxKliuba/LiquidCrystalUI
   by MaxClub Technology, 2022
*/

#pragma once

#include "ILiquidCrystal.h"
#include "LiquidCrystalCharmaps.h"

const uint8_t LCD_START_HORIZONTALLY = 0;
const uint8_t LCD_CENTER_HORIZONTALLY = 254;
const uint8_t LCD_END_HORIZONTALLY = 255;
const uint8_t LCD_AUTO_VERTICALLY = 0;

const uint16_t LCD_DEFAULT_SCROLLING_PERIOD = 400;

class LiquidCrystalView {
  public:
    LiquidCrystalView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool isEnabled,
      bool isVisible
    ) {
      _lcd = lcd;
      _id = id;
      _xPosition = xPosition;
      _yPosition = yPosition;
      _isEnabled = isEnabled;
      _isVisible = isVisible;
    }

    void tick() {
      if (!_isVisible && _isDrawn) {
        close();
      } else if (_isVisible && (checkAndUpdateDataChanges() || !_isDrawn || _isFocusChanged || _isDataChanged)) {
        if (_isDrawn && !_isFocusChanged) {
          clear();
        }
        draw();
      }
    }

    virtual void clear() {
      _isDrawn = false;

      _lcd->setCursorVisibility(false);
      _lcd->setCursorBlinking(false);

      _lcd->setCursor(getXPosition(), getYPosition());
      for (uint8_t i = 0; i < getWidth(); i++) {
        _lcd->write(_BLANK_CHAR);
      }
    }

    void close() {
      clear();

      _isSelected = false;
      _isEditing = false;
      _isDataChanged = false;
      _isFocusChanged = true;
    }

    void select() {
      _isSelected = true;
      _isFocusChanged = true;
    }

    void unselect() {
      _isSelected = false;
      _isFocusChanged = true;
    }

    void beginEditing() {
      _isEditing = true;
      _isFocusChanged = true;
    }

    void endEditing() {
      _isEditing = false;
      _isFocusChanged = true;
    }

    void notifyDataChanged() {
      _isDataChanged = true;
    }

    virtual uint8_t getWidth() = 0;

    uint8_t getId() {
      return _id;
    }

    uint8_t getXPosition() {
      switch (_xPosition) {
        case LCD_START_HORIZONTALLY: return 0;
        case LCD_CENTER_HORIZONTALLY: return (_lcd->getCols() - getWidth()) / 2;
        case LCD_END_HORIZONTALLY: return _lcd->getCols() - getWidth();
        default: return _xPosition;
      }
    }

    void setXPosition(uint8_t newXPosition) {
      _xPosition = newXPosition;
    }

    uint8_t getYPosition() {
      return _yPosition;
    }

    void setYPosition(uint8_t newYPosition) {
      _yPosition = newYPosition;
    }

    bool isSelected() {
      return _isSelected;
    }

    bool isEditing() {
      return _isEditing;
    }

    bool hasFocus() {
      return _isSelected || _isEditing;
    }

    bool isEnabled() {
      return _isEnabled;
    }

    void setEnabled(bool isEnabled) {
      _isEnabled = isEnabled;
      notifyDataChanged();
    }

    bool getVisibility() {
      return _isVisible;
    }

    void setVisibility(bool isVisible) {
      _isVisible = isVisible;
      notifyDataChanged();
    }

  protected:
    ILiquidCrystal* _lcd;

    virtual void draw() {
      _isDrawn = true;
      _isDataChanged = false;
      _isFocusChanged = false;
    }

    virtual bool checkAndUpdateDataChanges() {
      return false;
    }

    bool isChanged() {
      return _isDataChanged || _isFocusChanged;
    }

  private:
    uint8_t _id;
    uint8_t _xPosition;
    uint8_t _yPosition;
    bool _isEnabled;
    bool _isVisible;

    bool _isDrawn = false;
    bool _isDataChanged = false;
    bool _isFocusChanged = true;
    bool _isSelected = false;
    bool _isEditing = false;
};


class LcdPointerSelectableView : public LiquidCrystalView {
  public:
    LcdPointerSelectableView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool isEnabled,
      bool isVisible
    ) : LiquidCrystalView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
    }

    void clear() override {
      LiquidCrystalView::clear();

      if (isEnabled()) {
        _lcd->setCursor(getXPosition() - 1, getYPosition());
        _lcd->write(_BLANK_CHAR);
      }
    }

  protected:
    virtual void draw() override {
      LiquidCrystalView::draw();

      _lcd->setCursorVisibility(false);
      _lcd->setCursorBlinking(false);

      if (isEnabled()) {
        if (isEditing()) {
          _lcd->createChar(_POINTER_CHARMAP_LOCATION, _SELECTED_POINTER_CHARMAP);
          _lcd->setCursor(getXPosition() - 1, getYPosition());
          _lcd->write(_POINTER_CHARMAP_LOCATION);
        } else if (isSelected()) {
          _lcd->createChar(_POINTER_CHARMAP_LOCATION, _POINTER_CHARMAP);
          _lcd->setCursor(getXPosition() - 1, getYPosition());
          _lcd->write(_POINTER_CHARMAP_LOCATION);
        } else {
          _lcd->setCursor(getXPosition() - 1, getYPosition());
          _lcd->write(_BLANK_CHAR);
        }
      }
    }
};


class LcdTextView : public LcdPointerSelectableView {
  public:
    LcdTextView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      String text,
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdPointerSelectableView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      _text = text;
    }

    uint8_t getWidth() override {
      return _text.length();
    }

    String getText() {
      return _text;
    }

    void setText(String text) {
      _text = text;
      notifyDataChanged();
    }

  private:
    String _text;

    void draw() override {
      LcdPointerSelectableView::draw();

      _lcd->setCursor(getXPosition(), getYPosition());
      _lcd->print(_text);
    }
};


class LcdFlashTextView : public LcdPointerSelectableView {
  public:
    LcdFlashTextView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdPointerSelectableView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
    }

    uint8_t getWidth() override {
      return _text != NULL ? strlen_P((const char *)_text) * sizeof(uint8_t) : 0;
    }

    __FlashStringHelper* getText() {
      return _text;
    }

    void setText(const __FlashStringHelper *text) {
      _text = text;
      notifyDataChanged();
    }

  private:
    __FlashStringHelper *_text = NULL;

    void draw() override {
      LcdPointerSelectableView::draw();

      if (_text != NULL) {
        _lcd->setCursor(getXPosition(), getYPosition());
        _lcd->print(_text);
      }
    }
};


class LcdTextValueView : public LcdPointerSelectableView {
  public:
    LcdTextValueView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      uint8_t maxLength,
      String (*getValue)(),
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdPointerSelectableView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      _maxLength = maxLength;
      _getValue = getValue;
    }

    uint8_t getWidth() override {
      return _maxLength;
    }

    uint8_t getMaxLength() {
      return _maxLength;
    }

    void setMaxLength(uint8_t maxLength) {
      _maxLength = maxLength;
      notifyDataChanged();
    }

    void setValueFun(String (*getValue)()) {
      _getValue = getValue;
      notifyDataChanged();
    }

  private:
    uint8_t _maxLength;
    String (*_getValue)();

    String _prevValue;

    void draw() override {
      LcdPointerSelectableView::draw();

      if (_getValue != NULL) {
        _lcd->setCursor(getXPosition(), getYPosition());
        _lcd->print(_getValue());
      }
    }

    bool checkAndUpdateDataChanges() override {
      if (_getValue != NULL && _prevValue != _getValue()) {
        _prevValue = _getValue();

        return true;
      }

      return false;
    }
};


class LcdCursorSelectaleView : public LiquidCrystalView {
  public:
    LcdCursorSelectaleView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool isEnabled = true,
      bool isVisible = true
    ) : LiquidCrystalView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
    }

  protected:
    void draw() override {
      LiquidCrystalView::draw();

      _lcd->setCursor(getXPosition(), getYPosition());
      if (isEditing()) {
        _lcd->setCursorVisibility(true);
        _lcd->setCursorBlinking(true);
      } else if (isSelected()) {
        _lcd->setCursorVisibility(true);
        _lcd->setCursorBlinking(false);
      } else {
        _lcd->setCursorVisibility(false);
        _lcd->setCursorBlinking(false);
      }
    }
};


class LcdCharView : public LcdCursorSelectaleView {
  public:
    LcdCharView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      char charValue,
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdCursorSelectaleView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      _charValue = charValue;
    }

    uint8_t getWidth() override {
      return 1;
    }

    void setChar(char charValue) {
      _charValue = charValue;
      notifyDataChanged();
    }

    char getChar() {
      return _charValue;
    }

  private:
    char _charValue;

    char _prevValue;

    void draw() override {
      _lcd->setCursor(getXPosition(), getYPosition());
      _lcd->write(_charValue);

      LcdCursorSelectaleView::draw();
    }

    bool checkAndUpdateDataChanges() override {
      if (_prevValue != _charValue) {
        _prevValue = _charValue;

        return true;
      }

      return false;
    }
};


class LcdCharValueView : public LcdCursorSelectaleView {
  public:
    LcdCharValueView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      char (*getValue)(),
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdCursorSelectaleView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      _getValue = getValue;
    }

    uint8_t getWidth() override {
      return _getValue != NULL ? 1 : 0;
    }

    void setValueFun(char (*getValue)()) {
      _getValue = getValue;
      notifyDataChanged();
    }

  private:
    char (*_getValue)();

    char _prevValue;

    void draw() override {
      if (_getValue != NULL) {
        _lcd->setCursor(getXPosition(), getYPosition());
        _lcd->write(_getValue());
      }

      LcdCursorSelectaleView::draw();
    }

    bool checkAndUpdateDataChanges() override {
      if (_getValue != NULL && _prevValue != _getValue()) {
        _prevValue = _getValue();

        return true;
      }

      return false;
    }
};


class LcdCharmapView : public LcdPointerSelectableView {
  public:
    LcdCharmapView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      uint8_t charmapLocation,
      const uint8_t charmap[8],
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdPointerSelectableView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      setCharmap(charmapLocation, charmap);
    }

    uint8_t getWidth() override {
      return 1;
    }

    void setCharmap(uint8_t charmapLocation, uint8_t charmap[8]) {
      _charmapLocation = charmapLocation;

      for (int8_t i = 0; i < 8; i++) {
        _charmap[i] = charmap[i];
      }

      notifyDataChanged();
    }

  private:
    uint8_t _charmapLocation;
    uint8_t _charmap[8];

    void draw() override {
      LcdPointerSelectableView::draw();

      _lcd->createChar(_charmapLocation, _charmap);
      _lcd->setCursor(getXPosition(), getYPosition());
      _lcd->write(_charmapLocation);
    }
};


class LcdCharmapSelectableView : public LiquidCrystalView {
  public:
    LcdCharmapSelectableView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      uint8_t charmapLocation,
      const uint8_t charmap[8],
      const uint8_t selectedCharmap[8],
      bool isEnabled = true,
      bool isVisible = true
    ) : LiquidCrystalView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      setCharmap(charmapLocation, charmap, selectedCharmap);
    }

    uint8_t getWidth() override {
      return 1;
    }

    void setCharmap(uint8_t charmapLocation, uint8_t charmap[8], uint8_t selectedCharmap[8]) {
      _charmapLocation = charmapLocation;

      for (int8_t i = 0; i < 8; i++) {
        _charmap[i] = charmap[i];
        _selectedCharmap[i] = selectedCharmap[i];
      }

      notifyDataChanged();
    }

  protected:
    void draw() override {
      LiquidCrystalView::draw();

      _lcd->createChar(_charmapLocation, isSelected() ? _selectedCharmap : _charmap);
      _lcd->setCursor(getXPosition(), getYPosition());
      _lcd->write(_charmapLocation);
    }

  private:
    uint8_t _charmapLocation;
    uint8_t _charmap[8];
    uint8_t _selectedCharmap[8];
};


class LcdBackCharmapView : public LcdCharmapSelectableView {
  public:
    LcdBackCharmapView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdCharmapSelectableView(
        lcd, id, xPosition, yPosition,
        _BACK_ARROW_CHARMAP_LOCATION, _BACK_ARROW_CHARMAP, _SELECTED_BACK_ARROW_CHARMAP,
        isEnabled, isVisible) {
    }
};


class LcdMenuCharmapView : public LcdCharmapSelectableView {
  public:
    LcdMenuCharmapView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdCharmapSelectableView(
        lcd, id, xPosition, yPosition,
        _MENU_CHARMAP_LOCATION, _MENU_CHARMAP, _SELECTED_MENU_CHARMAP,
        isEnabled, isVisible) {
    }
};


class LcdApplyCharmapView : public LcdCharmapSelectableView {
  public:
    LcdApplyCharmapView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdCharmapSelectableView(
        lcd, id, xPosition, yPosition,
        _APPLY_CHARMAP_LOCATION, _APPLY_CHARMAP, _SELECTED_APPLY_CHARMAP,
        isEnabled, isVisible) {
    }
};


class LcdCheckableView : public LcdPointerSelectableView {
  public:
    LcdCheckableView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool (*getValue)(),
      bool isEnabled,
      bool isVisible
    ) : LcdPointerSelectableView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      _getValue = getValue;
    }

    void setValueFun(bool (*getValue)()) {
      _getValue = getValue;
      notifyDataChanged();
    }

  protected:
    void draw() override {
      LcdPointerSelectableView::draw();

      if (_getValue != NULL && _getValue()) {
        drawChecked();
      } else {
        drawUnchecked();
      }
    }

    bool checkAndUpdateDataChanges() override {
      if (_getValue != NULL && _prevValue != _getValue()) {
        _prevValue = _getValue();

        return true;
      }

      return false;
    }

    virtual void drawChecked() = 0;

    virtual void drawUnchecked() = 0;

  private:
    bool (*_getValue)();

    bool _prevValue;
};


class LcdSwitchView : public LcdCheckableView {
  public:
    LcdSwitchView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool (*getValue)(),
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdCheckableView(lcd, id, xPosition, yPosition, getValue, isEnabled, isVisible) {
    }

    uint8_t getWidth() override {
      return 2;
    }

  private:
    void drawChecked() override {
      _lcd->createChar(_LEFT_BORDER_CHARMAP_LOCATION, _LEFT_BORDER_CHARMAP);
      _lcd->setCursor(getXPosition(), getYPosition());
      _lcd->write(_LEFT_BORDER_CHARMAP_LOCATION);
      _lcd->write(_FILLED_CHAR);
    }

    void drawUnchecked() override {
      _lcd->createChar(_RIGHT_BORDER_CHARMAP_LOCATION, _RIGHT_BORDER_CHARMAP);
      _lcd->setCursor(getXPosition(), getYPosition());
      _lcd->write(_FILLED_CHAR);
      _lcd->write(_RIGHT_BORDER_CHARMAP_LOCATION);
    }
};


class LcdCheckBoxView : public LcdCheckableView {
  public:
    LcdCheckBoxView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      bool (*getValue)(),
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdCheckableView(lcd, id, xPosition, yPosition, getValue, isEnabled, isVisible) {
    }

    uint8_t getWidth() override {
      return 1;
    }

  private:
    void drawChecked() override {
      _lcd->setCursor(getXPosition(), getYPosition());
      _lcd->write(_FILLED_CHAR);
    }

    void drawUnchecked() override {
      _lcd->createChar(_OUTLINE_BORDER_CHARMAP_LOCATION, _OUTLINE_BORDER_CHARMAP);
      _lcd->setCursor(getXPosition(), getYPosition());
      _lcd->write(_OUTLINE_BORDER_CHARMAP_LOCATION);
    }
};


class LcdProgressBarView : public LcdPointerSelectableView {
  public:
    LcdProgressBarView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      uint8_t width,
      uint8_t minValue,
      uint8_t maxValue,
      uint8_t (*getValue)(),
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdPointerSelectableView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      _width = width;
      _minValue = minValue;
      _maxValue = maxValue;
      _getValue = getValue;
    }

    uint8_t getWidth() override {
      return _width;
    }

    void setWidth(uint8_t width) {
      _width = width;
      notifyDataChanged();
    }

    uint8_t getMinValue() {
      return _minValue;
    }

    void setMinValue(uint8_t minValue) {
      _minValue = minValue;
      notifyDataChanged();
    }

    uint8_t getMaxValue() {
      return _maxValue;
    }

    void setMaxValue(uint8_t maxValue) {
      _maxValue = maxValue;
      notifyDataChanged();
    }

    void setValueFun(uint8_t (*getValue)()) {
      _getValue = getValue;
      notifyDataChanged();
    }

  private:
    uint8_t _width;
    uint8_t _minValue;
    uint8_t _maxValue;
    uint8_t (*_getValue)();

    uint8_t _prevValue;

    void draw() override {
      LcdPointerSelectableView::draw();

      uint8_t value = constrain(map(_getValue != NULL ? _getValue() : 0, _minValue, _maxValue, 0, _width * 5), 0, _width * 5);
      uint8_t filledChars = value / 5;
      if (filledChars == 0) {
        _lcd->createChar(_PROGRESS_BAR_LEFT_CHARMAP_LOCATION, _PROGRESS_BAR_LEFT_CHARMAP);
        _lcd->setCursor(getXPosition(), getYPosition());
        _lcd->write(_PROGRESS_BAR_LEFT_CHARMAP_LOCATION);
      }

      _lcd->setCursor(getXPosition(), getYPosition());
      for (uint8_t i = 0; i < filledChars; i++) {
        _lcd->write(_FILLED_CHAR);
      }

      if (filledChars < _width) {
        _lcd->createChar(_PROGRESS_BAR_MIDDLE_CHARMAP_LOCATION, _PROGRESS_BAR_MIDDLE_CHARMAP);
        _lcd->setCursor(getXPosition() + filledChars + (filledChars == 0 ? 1 : 0), getYPosition());
        for (uint8_t i = 0; i < _width - filledChars - 1; i++) {
          _lcd->write(_PROGRESS_BAR_MIDDLE_CHARMAP_LOCATION);
        }

        _lcd->createChar(_PROGRESS_BAR_RIGHT_CHARMAP_LOCATION, _PROGRESS_BAR_RIGHT_CHARMAP);
        _lcd->setCursor(getXPosition() + _width - 1, getYPosition());
        _lcd->write(_PROGRESS_BAR_RIGHT_CHARMAP_LOCATION);
      }
    }

    bool checkAndUpdateDataChanges() override {
      if (_getValue != NULL && _prevValue != _getValue()) {
        _prevValue = _getValue();

        return true;
      }

      return false;
    }
};


class LcdGraphView : public LiquidCrystalView {
  public:
    LcdGraphView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      uint8_t width,
      uint8_t height,
      uint8_t minValue,
      uint8_t maxValue,
      uint8_t* (*getValues)(),
      bool isEnabled = false,
      bool isVisible = true
    ) : LiquidCrystalView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      _width = width;
      _height = height;
      _minValue = minValue;
      _maxValue = maxValue;
      _getValues = getValues;
    }

    void clear() override {
      LiquidCrystalView::clear();

      for (uint8_t y = 0; y < _height; y++) {
        _lcd->setCursor(getXPosition(), getYPosition() + y);
        for (uint8_t x = 0; x < _width; x++) {
          _lcd->write(_BLANK_CHAR);
        }
      }
    }

    uint8_t getWidth() override {
      return _width;
    }

    void setWidth(uint8_t width) {
      _width = width;
      notifyDataChanged();
    }

    uint8_t getHeight() {
      return _height;
    }

    void setHeight(uint8_t height) {
      _height = height;
      notifyDataChanged();
    }

    uint8_t getMinValue() {
      return _minValue;
    }

    void setMinValue(uint8_t minValue) {
      _minValue = minValue;
      notifyDataChanged();
    }

    uint8_t getMaxValue() {
      return _maxValue;
    }

    void setMaxValue(uint8_t maxValue) {
      _maxValue = maxValue;
      notifyDataChanged();
    }

    void setValuesFun(uint8_t* (*getValues)()) {
      _getValues = getValues;
      notifyDataChanged();
    }

  private:
    uint8_t _width;
    uint8_t _height;
    uint8_t _minValue;
    uint8_t _maxValue;
    uint8_t* (*_getValues)();

    void draw() override {
      LiquidCrystalView::draw();

      if (_getValues == NULL) {
        return;
      }

      _lcd->createChar(_GRAPH_1_CHARMAP_LOCATION, _GRAPH_1_CHARMAP);
      _lcd->createChar(_GRAPH_2_CHARMAP_LOCATION, _GRAPH_2_CHARMAP);
      _lcd->createChar(_GRAPH_3_CHARMAP_LOCATION, _GRAPH_3_CHARMAP);
      _lcd->createChar(_GRAPH_4_CHARMAP_LOCATION, _GRAPH_4_CHARMAP);
      _lcd->createChar(_GRAPH_5_CHARMAP_LOCATION, _GRAPH_5_CHARMAP);
      _lcd->createChar(_GRAPH_6_CHARMAP_LOCATION, _GRAPH_6_CHARMAP);
      _lcd->createChar(_GRAPH_7_CHARMAP_LOCATION, _GRAPH_7_CHARMAP);

      const uint8_t* values = _getValues();

      for (uint8_t x = 0; x < _width; x++) {
        uint8_t value = constrain(map(values[x], _minValue, _maxValue, 1, _height * 8), 1, _height * 8);
        uint8_t filledChars = value / 8;
        uint8_t partialChar = value % 8;

        for (uint8_t y = 0; y < filledChars; y++) {
          _lcd->setCursor(getXPosition() + x, getYPosition() + _height - 1 - y);
          _lcd->write(_FILLED_CHAR);
        }

        if (partialChar > 0 && partialChar < 8) {
          _lcd->setCursor(getXPosition() + x, getYPosition() + _height - 1 - filledChars);
          _lcd->write(_GRAPH_CHARMAP_LOCATIONS[partialChar - 1]);
        }

        int8_t startYPosition = getYPosition() + _height - 1 - filledChars - (partialChar > 0 ? 1 : 0);
        for (int8_t y = startYPosition; y >= getYPosition(); y--) {
          _lcd->setCursor(getXPosition() + x, y);
          _lcd->write(_BLANK_CHAR);
        }
      }
    }
};


class LcdCustomView : public LcdPointerSelectableView {
  public:
    LcdCustomView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      void (*onDraw)(ILiquidCrystal*),
      void (*onClear)(ILiquidCrystal*),
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdPointerSelectableView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      _onDraw = onDraw;
      _onClear = onClear;
    }

    void clear() {
      LcdPointerSelectableView::clear();

      if (_onClear != NULL) {
        _onClear(_lcd);
      }
    }

    uint8_t getWidth() override {
      return 0;
    }

    void setOnDraw(void (*onDraw)(ILiquidCrystal*)) {
      _onDraw = onDraw;
      notifyDataChanged();
    }

    void setOnClear(void (*onClear)(ILiquidCrystal*)) {
      _onClear = onClear;
      notifyDataChanged();
    }

  private:
    void (*_onDraw)(ILiquidCrystal*);
    void (*_onClear)(ILiquidCrystal*);

    void draw() override {
      LcdPointerSelectableView::draw();

      if (_onDraw != NULL) {
        _onDraw(_lcd);
      }
    }
};


class LcdScrollableTextView : public LcdPointerSelectableView {
  public:
    LcdScrollableTextView(
      ILiquidCrystal* lcd,
      uint8_t id,
      uint8_t xPosition,
      uint8_t yPosition,
      uint8_t maxLength,
      String text,
      bool focusedScrollingOnly = true,
      uint16_t scrollingPeriod = LCD_DEFAULT_SCROLLING_PERIOD,
      bool isEnabled = true,
      bool isVisible = true
    ) : LcdPointerSelectableView(lcd, id, xPosition, yPosition, isEnabled, isVisible) {
      _maxLength = maxLength;
      _text = text;
      _focusedScrollingOnly = focusedScrollingOnly;
      _scrollingPeriod = scrollingPeriod;
    }

    uint8_t getWidth() override {
      return _maxLength;
    }

    uint8_t getMaxLength() {
      return _maxLength;
    }

    void setMaxLength(uint8_t maxLength) {
      _maxLength = maxLength;
      notifyDataChanged();
    }

    String getText() {
      return _text;
    }

    void setText(String text) {
      _text = text;
      notifyDataChanged();
    }

    bool isFocusedScrollingOnly() {
      return _focusedScrollingOnly;
    }

    void setScrollingMode(bool focusedScrollingOnly) {
      _focusedScrollingOnly = focusedScrollingOnly;
    }

    uint16_t getScrollingPeriod() {
      return _scrollingPeriod;
    }

    void setScrollingPeriod(uint16_t scrollingPeriod) {
      _scrollingPeriod = scrollingPeriod;
    }

  private:
    uint8_t _maxLength;
    String _text;
    bool _focusedScrollingOnly;
    uint16_t _scrollingPeriod;

    uint32_t _scrollingTimer = 0;
    uint8_t _firstCharIndex = 0;

    void draw() override {
      LcdPointerSelectableView::draw();

      if (_text.length() > _maxLength) {
        if (!_focusedScrollingOnly || hasFocus()) {
          String text = _text + "  ";
          String scrolledText = text.substring(_firstCharIndex, min(_firstCharIndex + _maxLength, text.length()));
          scrolledText += text.substring(0, _maxLength - scrolledText.length());

          _lcd->setCursor(getXPosition(), getYPosition());
          _lcd->print(scrolledText);
        } else {
          _lcd->setCursor(getXPosition(), getYPosition());
          _lcd->print(_text.substring(0, _maxLength - 1));

          _lcd->createChar(_ELLIPSIS_CHARMAP_LOCATION, _ELLIPSIS_CHARMAP);
          _lcd->setCursor(getXPosition() + _maxLength - 1, getYPosition());
          _lcd->write(_ELLIPSIS_CHARMAP_LOCATION);
        }
      } else {
        _lcd->setCursor(getXPosition(), getYPosition());
        _lcd->print(_text);
      }
    }

    bool checkAndUpdateDataChanges() override {
      if (isChanged()) {
        _firstCharIndex = 0;
        _scrollingTimer = millis();
      }

      if (_text.length() <= _maxLength) {
        return false;
      }

      if ((!_focusedScrollingOnly || hasFocus()) && (millis() - _scrollingTimer >= _scrollingPeriod)) {
        _scrollingTimer = millis();
        _firstCharIndex = _text.length() < _firstCharIndex ? 0 : _firstCharIndex + 1;

        return true;
      }

      return false;
    }
};
