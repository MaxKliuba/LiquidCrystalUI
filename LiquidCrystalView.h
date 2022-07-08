/*
 * LiquidCrystalUI - Library
 * https://github.com/MaxKliuba/LiquidCrystalUI
 * by MaxClub Technology, 2022
 */

#pragma once

#include "ILiquidCrystal.h"
#include "LiquidCrystalCharmaps.h"

enum ScrollMode {
  NONE_SCROLL_MODE,
  AUTO_SCROLL_MODE,
  FOCUSED_SCROLL_MODE,
};

const uint8_t X_CENTER = 255;

class LiquidCrystalView {
  public:
    LiquidCrystalView(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      bool _isEditable
    ) {
      lcd = _lcd;
      id = _id;
      xPosition = _xPosition;
      yPosition = _yPosition;
      isSelectableFlag = _isSelectable;
      isEditableFlag = _isEditable;
    }

    virtual void tick() {
      if (!isInitialized) {
        init();
      }
      if (!isDrawn || isDataChanged) {
        draw();
      }
    }

    virtual void init() {
      isInitialized = true;
      draw();
    }

    virtual void draw() {
      isDrawn = true;
      isDataChanged = false;
    }

    virtual void clear() {
      isDrawn = false;
    }

    virtual void stop() {
      isInitialized = false;
      isSelectedFlag = false;
      isEditingFlag = false;
      isDataChanged = false;
      clear();
    }

    virtual void select() {
      if (!isSelectableFlag) {
        return;
      }
      isSelectedFlag = true;
      notifyDataChanged();
    }

    virtual void unselect() {
      if (!isSelectableFlag) {
        return;
      }
      isSelectedFlag = false;
      notifyDataChanged();
    }

    virtual void beginEditing() {
      if (!isEditableFlag) {
        return;
      }
      isEditingFlag = true;
      notifyDataChanged();
    }

    virtual void endEditing() {
      if (!isEditableFlag) {
        return;
      }
      isEditingFlag = false;
      notifyDataChanged();
    }

    virtual uint8_t getWidth() = 0;

    void notifyDataChanged() {
      isDataChanged = true;
    }

    uint8_t getId() {
      return id;
    }

    uint8_t getXPosition() {
      return xPosition == X_CENTER ? (lcd->getCols() - getWidth()) / 2 : xPosition;
    }

    void setXPosition(uint8_t newXPosition) {
      xPosition = newXPosition;
    }

    uint8_t getYPosition() {
      return yPosition;
    }

    void setYPosition(uint8_t newYPosition) {
      yPosition = newYPosition;
    }

    bool isSelected() {
      return isSelectedFlag;
    }

    bool isEditing() {
      return isEditingFlag;
    }

    bool hasFocus() {
      return isSelectedFlag || isEditingFlag;
    }

    bool isSelectable() {
      return isSelectableFlag;
    }

    void setSelectable(bool isSelectable) {
      isSelectableFlag = isSelectable;
    }

    bool isEditable() {
      return isEditableFlag;
    }

    void setEditable(bool isEditable) {
      isEditableFlag = isEditable;
    }

  protected:
    ILiquidCrystal* lcd;

    bool isSelectableFlag = false;
    bool isEditableFlag = false;

    bool isInitialized = false;
    bool isDrawn = false;
    bool isSelectedFlag = false;
    bool isEditingFlag = false;
    bool isDataChanged = false;

  private:
    uint8_t id;
    uint8_t xPosition;
    uint8_t yPosition;
};

class LcdPointerSelectableView : public LiquidCrystalView {
  public:
    LcdPointerSelectableView(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      bool _isEditable
    ) : LiquidCrystalView(_lcd, _id, _xPosition, _yPosition, _isSelectable, _isEditable) {
    }

    virtual void draw() override {
      LiquidCrystalView::draw();
      if (isSelectableFlag || isEditableFlag) {
        if (isEditingFlag) {
          lcd->createChar(_POINTER_CHARMAP_LOCATION, _SELECTED_POINTER_CHARMAP);
          lcd->setCursor(getXPosition() - 1, getYPosition());
          lcd->write(_POINTER_CHARMAP_LOCATION);
        } else if (isSelectedFlag) {
          lcd->createChar(_POINTER_CHARMAP_LOCATION, _POINTER_CHARMAP);
          lcd->setCursor(getXPosition() - 1, getYPosition());
          lcd->write(_POINTER_CHARMAP_LOCATION);
        } else {
          lcd->setCursor(getXPosition() - 1, getYPosition());
          lcd->write(_BLANK_CHAR);
        }
      }
    }

    virtual void clear() override {
      LiquidCrystalView::clear();
      uint8_t pointerCharmapSize = (isSelectableFlag || isEditableFlag) ? 1 : 0;
      lcd->setCursor(getXPosition() - pointerCharmapSize, getYPosition());
      lcd->write(_BLANK_CHAR);
    }
};

class LcdScrollableView : public LcdPointerSelectableView {
  public:
    LcdScrollableView(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      bool _isEditable,
      String _text,
      uint8_t _textMaxLength,
      ScrollMode _scrollMode,
      uint16_t _scrollPeriod
    ) : LcdPointerSelectableView(_lcd, _id, _xPosition, _yPosition, _isSelectable, _isEditable) {
      text = _text;
      textMaxLength = _textMaxLength;
      scrollMode = _scrollMode;
      scrollPeriod = _scrollPeriod;
    }

    void tick() override {
      LiquidCrystalView::tick();
      tickScroll();
    }

    void init() override {
      resetScrolling();
      LiquidCrystalView::init();
    }

    void draw() override {
      LcdPointerSelectableView::draw();

      lcd->setCursor(getScrollXPosition(), getYPosition());
      if (getText().length() > getTextMaxLength()) {
        if (scrollMode == AUTO_SCROLL_MODE || (scrollMode == FOCUSED_SCROLL_MODE && hasFocus())) {
          drawText();
        } else {
          lcd->print(getText().substring(0, getTextMaxLength() - 1));
          lcd->createChar(_ELLIPSIS_CHARMAP_LOCATION, _ELLIPSIS_CHARMAP);
          lcd->setCursor(getScrollXPosition() + getTextMaxLength() - 1, getYPosition());
          lcd->write(_ELLIPSIS_CHARMAP_LOCATION);
        }
      } else {
        resetScrolling();
        lcd->print(getText());
        int8_t spaseLength = getTextMaxLength() - getText().length();
        for (uint8_t i = 0; i < spaseLength; i++) {
          lcd->write(_BLANK_CHAR);
        }
      }
    }

    void clear() override {
      LcdPointerSelectableView::clear();
      lcd->setCursor(getScrollXPosition(), getYPosition());
      for (uint8_t i = 0; i < getTextMaxLength(); i++) {
        lcd->write(_BLANK_CHAR);
      }
    }

    void stop() override {
      resetScrolling();
      LiquidCrystalView::stop();
    }

    void select() override {
      LiquidCrystalView::select();
      resetScrolling();
    }

    void beginEditing() override {
      LiquidCrystalView::beginEditing();
      resetScrolling();
    }

    uint8_t getWidth() override {
      return getTextMaxLength();
    }

    void setText(String text, uint8_t textMaxLength) {
      this->text = text;
      this->textMaxLength = textMaxLength;
    }

    ScrollMode getScrollMode() {
      return scrollMode;
    }

    void setScrollMode(ScrollMode scrollMode) {
      this->scrollMode = scrollMode;
    }

    uint16_t getScrollPeriod() {
      return scrollPeriod;
    }

    void setScrollPeriod (uint16_t scrollPeriod) {
      this->scrollPeriod = scrollPeriod;
    }

    virtual String getText() {
      return text;
    }

    virtual uint8_t getTextMaxLength() {
      return textMaxLength;
    }

    virtual uint8_t getScrollXPosition() {
      return getXPosition();
    }

  protected:
    String text;
    uint8_t textMaxLength;
    ScrollMode scrollMode;
    uint16_t scrollPeriod;

    void tickScroll() {
      String text = getText();
      if (text.length() <= getTextMaxLength()) {
        return;
      }
      if (scrollMode == AUTO_SCROLL_MODE || (scrollMode == FOCUSED_SCROLL_MODE && hasFocus())) {
        if (millis() - scrollTimer >= scrollPeriod) {
          scrollTimer = millis();
          drawText();
          firstCharIndex = text.length() < firstCharIndex ? 0 : firstCharIndex + 1;
        }
      }
    }

    void drawText() {
      lcd->setCursor(getScrollXPosition(), getYPosition());
      String currentText = getText() + "  ";
      String tempText = currentText.substring(firstCharIndex, min(firstCharIndex + getTextMaxLength(), currentText.length()));
      tempText += currentText.substring(0, getTextMaxLength() - tempText.length());
      lcd->print(tempText);
    }

    void resetScrolling() {
      firstCharIndex = 0;
      scrollTimer = millis();
    }

  private:
    uint32_t scrollTimer = 0;
    uint8_t firstCharIndex = 0;
};

class LcdTextView : public LcdScrollableView {
  public:
    LcdTextView(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      bool _isEditable,
      String _text,
      uint8_t _textMaxLength,
      String(*_getValue)(),
      uint8_t _valueMaxLength,
      ScrollMode _scrollMode,
      uint16_t _scrollPeriod
    ) : LcdScrollableView(_lcd, _id, _xPosition, _yPosition, _isSelectable, _isEditable, _text, _textMaxLength, _scrollMode, _scrollPeriod) {
      getValue = _getValue;
      valueMaxLength = _valueMaxLength;
    }

    void setValueFun(String(*getValue)(), uint8_t valueMaxLength) {
      this->getValue = getValue;
      this->valueMaxLength = valueMaxLength;
    }

    String getText() override {
      return LcdScrollableView::getText() + (getValue != NULL ? getValue() : "");
    }

    uint8_t getTextMaxLength() override {
      return LcdScrollableView::getTextMaxLength() + valueMaxLength;
    }

  protected:
    String(*getValue)();
    uint8_t valueMaxLength;
};

class LcdTitleView : public LcdScrollableView {
  public:
    LcdTitleView(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      bool _isEditable,
      String _text,
      uint8_t _textMaxLength,
      bool _hasPrevPage,
      bool _hasNextPage,
      ScrollMode _scrollMode,
      uint16_t _scrollPeriod
    ) : LcdScrollableView(_lcd, _id, _xPosition, _yPosition, _isSelectable, _isEditable, _text, _textMaxLength, _scrollMode, _scrollPeriod) {
      hasPrevPage = _hasPrevPage;
      hasNextPage = _hasNextPage;
    }

    void draw() override {
      LcdScrollableView::draw();
      lcd->setCursor(getXPosition(), getYPosition());
      lcd->print(hasPrevPage ? "<" : "|");
      lcd->setCursor(getScrollXPosition() + LcdScrollableView::getTextMaxLength(), getYPosition());
      lcd->print(hasNextPage ? ">" : "|");
    }

    void clear() override {
      LcdScrollableView::clear();
      lcd->setCursor(getXPosition(), getYPosition());
      lcd->write(_BLANK_CHAR);
      lcd->setCursor(getScrollXPosition() + LcdScrollableView::getTextMaxLength(), getYPosition());
      lcd->write(_BLANK_CHAR);
    }

    uint8_t getWidth() {
      return LcdScrollableView::getWidth() + 2;
    }

    bool getHasPrevPage() {
      return hasPrevPage;
    }

    bool setHasPrevPage(bool hasPrevPage) {
      this->hasPrevPage = hasPrevPage;
    }

    bool getHasNextPage() {
      return hasNextPage;
    }

    bool setHasNextPage(bool hasNextPage) {
      this->hasNextPage = hasNextPage;
    }

    uint8_t getScrollXPosition() override {
      return getXPosition() + 1;
    }
  protected:
    bool hasPrevPage;
    bool hasNextPage;
};

class LcdCheckableView : public LcdScrollableView {
  public:
    LcdCheckableView(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      String _text,
      uint8_t _textMaxLength,
      bool(*_getValue)(),
      ScrollMode _scrollMode,
      uint16_t _scrollPeriod
    ) : LcdScrollableView(_lcd, _id, _xPosition, _yPosition, _isSelectable, false, _text, _textMaxLength, _scrollMode, _scrollPeriod) {
      getValue = _getValue;
    }

    void draw() override {
      LcdScrollableView::draw();
      if (getValue != NULL && getValue()) {
        drawCheck();
      } else {
        drawUncheck();
      }
    }

    void clear() override {
      LcdScrollableView::clear();
      lcd->setCursor(LcdScrollableView::getScrollXPosition() + LcdScrollableView::getTextMaxLength(), getYPosition());
      for (uint8_t i = 0; i < getWidth(); i++) {
        lcd->write(_BLANK_CHAR);
      }
    }

    void beginEditing() override {
      return;
    }

    void endEditing() override {
      return;
    }

    void setEditable(bool isEditable) {
      isEditableFlag = false;
    }

    void setValueFun(bool(*getValue)()) {
      this->getValue = getValue;
    }

  protected:
    bool(*getValue)();

    virtual void drawCheck() = 0;

    virtual void drawUncheck() = 0;
};

class LcdSwitch : public LcdCheckableView {
  public:
    LcdSwitch(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      String _text,
      uint8_t _textMaxLength,
      bool(*_getValue)(),
      ScrollMode _scrollMode,
      uint16_t _scrollPeriod
    ) : LcdCheckableView(_lcd, _id, _xPosition, _yPosition, _isSelectable, _text, _textMaxLength, _getValue, _scrollMode, _scrollPeriod) {
    }

    uint8_t getWidth() override {
      return LcdScrollableView::getTextMaxLength() + 2;
    }

  protected:
    void drawCheck() override {
      uint8_t checkableViewXPosition = LcdScrollableView::getScrollXPosition() + LcdScrollableView::getTextMaxLength();
      lcd->createChar(_LEFT_BORDER_CHARMAP_LOCATION, _LEFT_BORDER_CHARMAP);
      lcd->setCursor(checkableViewXPosition, getYPosition());
      lcd->write(_LEFT_BORDER_CHARMAP_LOCATION);
      lcd->setCursor(checkableViewXPosition + 1, getYPosition());
      lcd->write(_FILL_CHAR);
    }

    void drawUncheck() override {
      uint8_t checkableViewXPosition = LcdScrollableView::getScrollXPosition() + LcdScrollableView::getTextMaxLength();
      lcd->setCursor(checkableViewXPosition, getYPosition());
      lcd->write(_FILL_CHAR);
      lcd->createChar(_RIGHT_BORDER_CHARMAP_LOCATION, _RIGHT_BORDER_CHARMAP);
      lcd->setCursor(checkableViewXPosition + 1, getYPosition());
      lcd->write(_RIGHT_BORDER_CHARMAP_LOCATION);
    }
};

class LcdCheckBox : public LcdCheckableView {
  public:
    LcdCheckBox(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      String _text,
      uint8_t _textMaxLength,
      bool(*_getValue)(),
      ScrollMode _scrollMode,
      uint16_t _scrollPeriod
    ) : LcdCheckableView(_lcd, _id, _xPosition, _yPosition, _isSelectable, _text, _textMaxLength, _getValue, _scrollMode, _scrollPeriod) {
    }

    uint8_t getWidth() override {
      return LcdScrollableView::getTextMaxLength() + 1;
    }

  protected:
    void drawCheck() override {
      uint8_t checkableViewXPosition = LcdScrollableView::getScrollXPosition() + LcdScrollableView::getTextMaxLength();
      lcd->setCursor(checkableViewXPosition, getYPosition());
      lcd->write(_FILL_CHAR);
    }

    void drawUncheck() override {
      uint8_t checkableViewXPosition = LcdScrollableView::getScrollXPosition() + LcdScrollableView::getTextMaxLength();
      lcd->createChar(_OUTLINE_BORDER_CHARMAP_LOCATION, _OUTLINE_BORDER_CHARMAP);
      lcd->setCursor(checkableViewXPosition, getYPosition());
      lcd->write(_OUTLINE_BORDER_CHARMAP_LOCATION);
    }
};

class LcdCharmapOnlyButton : public LiquidCrystalView {
  public:
    LcdCharmapOnlyButton(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      uint8_t _charmapLocation,
      uint8_t _charmap[],
      uint8_t _selectedCharmap[] = NO_CHARMAP
    ) : LiquidCrystalView(_lcd, _id, _xPosition, _yPosition, _isSelectable, false) {
      charmapLocation = _charmapLocation;
      for (int8_t i = 0; i < 8; i++) {
        charmap[i] = _charmap[i];
        selectedCharmap[i] = _selectedCharmap[i];
      }
    }

    static LcdCharmapOnlyButton createBackButton(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable
    ) {
      return LcdCharmapOnlyButton(
               _lcd, _id, _xPosition, _yPosition, _isSelectable,
               _BACK_ARROW_CHARMAP_LOCATION, _BACK_ARROW_CHARMAP, _SELECTED_BACK_ARROW_CHARMAP
             );
    }

    static LcdCharmapOnlyButton createMenuButton(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable
    ) {
      return LcdCharmapOnlyButton(
               _lcd, _id, _xPosition, _yPosition, _isSelectable,
               _MENU_CHARMAP_LOCATION, _MENU_CHARMAP, _SELECTED_MENU_CHARMAP
             );
    }

    static LcdCharmapOnlyButton createApplyButton(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable
    ) {
      return LcdCharmapOnlyButton(
               _lcd, _id, _xPosition, _yPosition, _isSelectable,
               _APPLY_CHARMAP_LOCATION, _APPLY_CHARMAP, _SELECTED_APPLY_CHARMAP
             );
    }

    void draw() override {
      LiquidCrystalView::draw();
      lcd->createChar(charmapLocation, isSelectedFlag ? selectedCharmap : charmap);
      lcd->setCursor(getXPosition(), getYPosition());
      lcd->write(charmapLocation);
    }

    void clear() override {
      LiquidCrystalView::clear();
      lcd->setCursor(getXPosition(), getYPosition());
      lcd->write(_BLANK_CHAR);
    }

    void beginEditing() override {
      return;
    }

    void endEditing() override {
      return;
    }

    uint8_t getWidth() override {
      return 1;
    }

    void setEditable(bool isEditable) {
      isEditableFlag = false;
    }

    void setCharmap(uint8_t charmapLocation, uint8_t charmap[], uint8_t selectedCharmap[] = NO_CHARMAP) {
      this->charmapLocation = charmapLocation;
      for (int8_t i = 0; i < 8; i++) {
        this->charmap[i] = charmap[i];
        this->selectedCharmap[i] = selectedCharmap[i];
      }
    }

  protected:
    uint8_t charmapLocation;
    uint8_t charmap[8];
    uint8_t selectedCharmap[8];
};

class LcdTextButton : public LcdScrollableView {
  public:
    LcdTextButton(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      String _text,
      uint8_t _textMaxLength,
      bool _hasChar,
      bool _hasBorder,
      ScrollMode _scrollMode,
      uint16_t _scrollPeriod
    ) : LcdScrollableView(_lcd, _id, _xPosition, _yPosition, _isSelectable, false, _text, _textMaxLength, _scrollMode, _scrollPeriod) {
      hasChar = _hasChar;
      hasBorder = _hasBorder;
    }

    void draw() override {
      LcdScrollableView::draw();
      if (hasChar) {
        drawChar();
      }
      if (hasBorder) {
        lcd->setCursor(getXPosition(), getYPosition());
        lcd->print("[");
        lcd->setCursor(getScrollXPosition() + LcdScrollableView::getTextMaxLength(), getYPosition());
        lcd->print("]");
      }
    }

    void clear() override {
      LcdScrollableView::clear();
      if (hasChar) {
        lcd->setCursor(getCharXPosition(), getYPosition());
        lcd->write(_BLANK_CHAR);
      }
      if (hasBorder) {
        lcd->setCursor(getXPosition(), getYPosition());
        lcd->write(_BLANK_CHAR);
        lcd->setCursor(getScrollXPosition() + LcdScrollableView::getTextMaxLength(), getYPosition());
        lcd->write(_BLANK_CHAR);
      }
    }

    uint8_t getWidth() override {
      return LcdScrollableView::getTextMaxLength() + (hasChar ? 1 : 0) + (hasBorder ? 2 : 0);
    }

    uint8_t getCharXPosition() {
      return getXPosition() + (hasBorder ? 1 : 0);
    }

    void getHasChar() {
      return hasChar;
    }

    void getHasBorder() {
      return hasBorder;
    }

    void setHasBorder(bool hasBorder) {
      this->hasBorder = hasBorder;
    }

    void beginEditing() override {
      return;
    }

    void endEditing() override {
      return;
    }

    void setEditable(bool isEditable) {
      isEditableFlag = false;
    }

    uint8_t getScrollXPosition() override {
      return getXPosition() + (hasChar ? 1 : 0) + (hasBorder ? 1 : 0);
    }

  protected:
    uint8_t hasChar;
    bool hasBorder;

    virtual void drawChar() = 0;
};

class LcdCharTextButton : public LcdTextButton {
  public:
    LcdCharTextButton(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      String _text,
      uint8_t _textMaxLength,
      char _character,
      bool _hasBorder,
      ScrollMode _scrollMode,
      uint16_t _scrollPeriod
    ) : LcdTextButton(_lcd, _id, _xPosition, _yPosition, _isSelectable, _text, _textMaxLength, _character != NO_CHAR, _hasBorder, _scrollMode, _scrollPeriod) {
      character = _character;
    }

    char getCharacter() {
      return character;
    }

    void setCharacter(char character) {
      hasChar = character != NO_CHAR;
      this->character = character;
    }

  protected:
    char character;

    void drawChar() override {
      lcd->setCursor(getCharXPosition(), getYPosition());
      lcd->write(character);
    }
};

class LcdCharmapTextButton : public LcdTextButton {
  public:
    LcdCharmapTextButton(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      String _text,
      uint8_t _textMaxLength,
      uint8_t _charmapLocation,
      uint8_t _charmap[],
      bool _withBorder,
      ScrollMode _scrollMode,
      uint16_t _scrollPeriod
    ) : LcdTextButton(_lcd, _id, _xPosition, _yPosition, _isSelectable, _text, _textMaxLength, _charmap != NO_CHARMAP, _withBorder, _scrollMode, _scrollPeriod) {
      charmapLocation = _charmapLocation;
      for (int8_t i = 0; i < 8; i++) {
        charmap[i] = _charmap[i];
      }
    }

    void setCharmap(uint8_t charmapLocation, uint8_t charmap[]) {
      hasChar = charmap != NO_CHARMAP;
      this->charmapLocation = charmapLocation;
      for (int8_t i = 0; i < 8; i++) {
        this->charmap[i] = charmap[i];
      }
    }

  protected:
    uint8_t charmapLocation;
    uint8_t charmap[8];

    void drawChar() override {
      lcd->createChar(charmapLocation, charmap);
      lcd->setCursor(getCharXPosition(), getYPosition());
      lcd->write(charmapLocation);
    }
};

class LcdCustomView : public LcdPointerSelectableView {
  public:
    LcdCustomView(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      bool _isEditable,
      uint8_t _maxWidth,
      void(*_onDraw)(),
      void(*_onClear)()
    ) : LcdPointerSelectableView(_lcd, _id, _xPosition, _yPosition, _isSelectable, _isEditable) {
      maxWidth = _maxWidth;
      onDraw = _onDraw;
      onClear = _onClear;
    }

    void draw() override {
      LcdPointerSelectableView::draw();
      if (onDraw != NULL) {
        onDraw();
      }
    }

    void clear() override {
      LcdPointerSelectableView::clear();
      if (onClear != NULL) {
        onClear();
      }
    }

    uint8_t getWidth() override {
      return maxWidth;
    }

    void setOnDraw(void(*onDraw)()) {
      this->onDraw = onDraw;
    }

    void setOnClear(void(*onClear)()) {
      this->onClear = onClear;
    }

  protected:
    uint8_t maxWidth;
    void(*onDraw)();
    void(*onClear)();
};

class LcdProgressBar : public LcdPointerSelectableView {
  public:
    LcdProgressBar(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      bool _isSelectable,
      uint8_t _width,
      uint8_t(*_getValue)()
    ) : LcdPointerSelectableView(_lcd, _id, _xPosition, _yPosition, _isSelectable, false) {
      width = _width;
      getValue = _getValue;
    }

    void draw() override {
      LcdPointerSelectableView::draw();

      uint8_t value = constrain(map(getValue != NULL ? getValue() : 0, 0, 100, 0, width * 5), 0, width * 5);
      uint8_t fillChars = value / 5;
      if (fillChars == 0) {
        lcd->createChar(_PROGRESS_BAR_LEFT_CHARMAP_LOCATION, _PROGRESS_BAR_LEFT_CHARMAP);
        lcd->setCursor(getXPosition(), getYPosition());
        lcd->write(_PROGRESS_BAR_LEFT_CHARMAP_LOCATION);
      }
      lcd->setCursor(getXPosition(), getYPosition());
      for (uint8_t i = 0; i < fillChars; i++) {
        lcd->write(_FILL_CHAR);
      }
      if (fillChars < width) {
        lcd->createChar(_PROGRESS_BAR_MIDDLE_CHARMAP_LOCATION, _PROGRESS_BAR_MIDDLE_CHARMAP);
        lcd->setCursor(getXPosition() + fillChars + (fillChars == 0 ? 1 : 0), getYPosition());
        for (uint8_t i = 0; i < width - fillChars - 1; i++) {
          lcd->write(_PROGRESS_BAR_MIDDLE_CHARMAP_LOCATION);
        }
        lcd->createChar(_PROGRESS_BAR_RIGHT_CHARMAP_LOCATION, _PROGRESS_BAR_RIGHT_CHARMAP);
        lcd->setCursor(getXPosition() + width - 1, getYPosition());
        lcd->write(_PROGRESS_BAR_RIGHT_CHARMAP_LOCATION);
      }
    }

    void clear() override {
      LcdPointerSelectableView::clear();
      lcd->setCursor(getXPosition(), getYPosition());
      for (uint8_t i = 0; i < getWidth(); i++) {
        lcd->write(_BLANK_CHAR);
      }
    }

    void beginEditing() override {
      return;
    }

    void endEditing() override {
      return;
    }

    uint8_t getWidth() override {
      return width;
    }

    void setEditable(bool isEditable) {
      isEditableFlag = false;
    }

    void setWidth(uint8_t width) {
      this->width = width;
    }

    void setValueFun(uint8_t(*getValue)()) {
      this->getValue = getValue;
    }

  protected:
    uint8_t width;
    uint8_t(*getValue)();
};

class LcdGraph : public LiquidCrystalView {
  public:
    LcdGraph(
      ILiquidCrystal* _lcd,
      uint8_t _id,
      uint8_t _xPosition,
      uint8_t _yPosition,
      uint8_t _width,
      uint8_t _height,
      uint8_t _minValue,
      uint8_t _maxValue,
      uint8_t*(*_getValues)()
    ) : LiquidCrystalView(_lcd, _id, _xPosition, _yPosition, false, false) {
      width = _width;
      height = _height;
      minValue = _minValue;
      maxValue = _maxValue;
      getValues = _getValues;
    }

    void draw() override {
      LiquidCrystalView::draw();

      lcd->createChar(_GRAPH_1_CHARMAP_LOCATION, _GRAPH_1_CHARMAP);
      lcd->createChar(_GRAPH_2_CHARMAP_LOCATION, _GRAPH_2_CHARMAP);
      lcd->createChar(_GRAPH_3_CHARMAP_LOCATION, _GRAPH_3_CHARMAP);
      lcd->createChar(_GRAPH_4_CHARMAP_LOCATION, _GRAPH_4_CHARMAP);
      lcd->createChar(_GRAPH_5_CHARMAP_LOCATION, _GRAPH_5_CHARMAP);
      lcd->createChar(_GRAPH_6_CHARMAP_LOCATION, _GRAPH_6_CHARMAP);
      lcd->createChar(_GRAPH_7_CHARMAP_LOCATION, _GRAPH_7_CHARMAP);

      uint8_t* values = getValues();
      for (uint8_t x = 0; x < width; x++) {
        uint8_t value = constrain(map(values[x], minValue, maxValue, 1, height * 8), minValue, maxValue);
        uint8_t fillChars = value / 8;
        uint8_t partialChar = value % 8;
        for (uint8_t y = 0; y < fillChars; y++) {
          lcd->setCursor(getXPosition() + x, getYPosition() + height - 1 - y);
          lcd->write(_FILL_CHAR);
        }
        if (partialChar > 0 && partialChar < 8) {
          lcd->setCursor(getXPosition() + x, getYPosition() + height - 1 - fillChars);
          lcd->write(GRAPH_CHARMAP_LOCATIONS[partialChar - 1]);
        }
        int8_t startYPosition = getYPosition() + height - 1 - fillChars - (partialChar > 0 ? 1 : 0);
        for (int8_t y = startYPosition; y >= getYPosition(); y--) {
          lcd->setCursor(getXPosition() + x, y);
          lcd->write(_BLANK_CHAR);
        }
      }
    }

    void clear() override {
      LiquidCrystalView::clear();
      for (uint8_t y = 0; y < getHeight(); y++) {
        lcd->setCursor(getXPosition(), getYPosition() + y);
        for (uint8_t x = 0; x < getWidth(); x++) {
          lcd->write(_BLANK_CHAR);
        }
      }
    }

    void select() override {
      return;
    }

    void unselect() override {
      return;
    }

    void beginEditing() override {
      return;
    }

    void endEditing() override {
      return;
    }

    uint8_t getWidth() override {
      return width;
    }

    void setSelectable(bool isSelectable) {
      isSelectableFlag = false;
    }

    void setEditable(bool isEditable) {
      isEditableFlag = false;
    }

    void setWidth(uint8_t width) {
      this->width = width;
    }

    uint8_t getHeight() {
      return height;
    }

    void setHeight(uint8_t height) {
      this->height = height;
    }

    uint8_t getMinValue() {
      return minValue;
    }

    void setMinValue(uint8_t minValue) {
      this->minValue = minValue;
    }

    uint8_t getMaxValue() {
      return maxValue;
    }

    void setMaxValue(uint8_t maxValue) {
      this->maxValue = maxValue;
    }

    void setValuesFun(uint8_t*(*getValues)()) {
      this->getValues = getValues;
    }

  protected:
    uint8_t width;
    uint8_t height;
    uint8_t minValue;
    uint8_t maxValue;
    uint8_t*(*getValues)();

    const uint8_t GRAPH_CHARMAP_LOCATIONS[7] = {
      _GRAPH_1_CHARMAP_LOCATION,
      _GRAPH_2_CHARMAP_LOCATION,
      _GRAPH_3_CHARMAP_LOCATION,
      _GRAPH_4_CHARMAP_LOCATION,
      _GRAPH_5_CHARMAP_LOCATION,
      _GRAPH_6_CHARMAP_LOCATION,
      _GRAPH_7_CHARMAP_LOCATION
    };
};
