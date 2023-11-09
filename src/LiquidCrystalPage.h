/*
   LiquidCrystalUI - Library
   https://github.com/MaxKliuba/LiquidCrystalUI
   by MaxClub Technology, 2022
*/

#pragma once

#include "ILiquidCrystal.h"
#include "LiquidCrystalCharmaps.h"
#include "LiquidCrystalView.h"

class LcdPageRow {
  public:
    LcdPageRow(LiquidCrystalView** items, uint8_t size) {
      _size = size;
      _items = items;
    }

    uint8_t getSize() {
      return _size;
    }

    LiquidCrystalView* getItemByIndex(uint8_t index) {
      return _items[index];
    }

  private:
    LiquidCrystalView** _items;
    uint8_t _size;
};


class LcdPageLayout {
  public:
    LcdPageLayout(LcdPageRow** rows, uint8_t rowsCount) {
      _rowsCount = rowsCount;
      _rows = rows;
    }

    uint8_t getRowsCount() {
      return _rowsCount;
    }

    LcdPageRow* getRowByIndex(uint8_t index) {
      return _rows[index];
    }

  private:
    LcdPageRow** _rows;
    uint8_t _rowsCount;
};


class LiquidCrystalPage {
  public:
    static const uint8_t NO_ID = 255;

    LiquidCrystalPage(
      ILiquidCrystal* lcd,
      uint8_t id,
      LcdPageLayout* pageLayout,
      bool isDownArrowVisible = true
    ) {
      _lcd = lcd;
      _id = id;
      _pageLayout = pageLayout;
      _isDownArrowVisible = isDownArrowVisible;
    }

    void tick() {
      if (!_isInitialized) {
        init();
      }

      LiquidCrystalView* selectedView = NULL;

      int8_t windowOffset = getMinWindowOffsetToRowIndex(_selectedRowIndex);

      for (uint8_t r = 0; r < _lcd->getRows(); r++) {
        if (windowOffset != 0 || _isDataChanged) {
          uint8_t prevRowIndex = _firstVisibleRowIndex + r;

          if (prevRowIndex < _pageLayout->getRowsCount()) {
            LcdPageRow* pageRow = _pageLayout->getRowByIndex(prevRowIndex);

            for (uint8_t i = 0; i < pageRow->getSize(); i++) {
              LiquidCrystalView* itemView = pageRow->getItemByIndex(i);
              itemView->setYPosition(r);
              itemView->clear();
            }
          }
        }

        uint8_t rowIndex = _firstVisibleRowIndex + windowOffset + r;

        if (rowIndex < _pageLayout->getRowsCount()) {
          LcdPageRow* pageRow = _pageLayout->getRowByIndex(rowIndex);

          for (uint8_t i = 0; i < pageRow->getSize(); i++) {
            LiquidCrystalView* itemView = pageRow->getItemByIndex(i);
            itemView->setYPosition(r);

            if (itemView->getId() == _selectedItemId && !itemView->isSelected()) {
              selectedView = itemView;
            } else if (itemView->getId() != _selectedItemId) {
              if (itemView->isSelected()) {
                itemView->unselect();
              }
              if (itemView->isEditing()) {
                itemView->endEditing();
              }
            }

            itemView->tick();
          }
        }
      }

      if (_isDownArrowVisible && (_isDataChanged || _isSelectedRowIndexChanged)) {
        if (_firstVisibleRowIndex + windowOffset + _lcd->getRows() < _pageLayout->getRowsCount()) {
          _lcd->createChar(_DOWN_ARROW_CHARMAP_LOCATION, _DOWN_ARROW_CHARMAP);
          _lcd->setCursor(_lcd->getCols() - 1, _lcd->getRows() - 1);
          _lcd->write(_DOWN_ARROW_CHARMAP_LOCATION);
        } else {
          _lcd->setCursor(_lcd->getCols() - 1, _lcd->getRows() - 1);
          _lcd->write(_BLANK_CHAR);
        }
      }

      if (selectedView != NULL && selectedView->getId() == _selectedItemId && !selectedView->isSelected()) {
        selectedView->select();
        selectedView->tick();
      }

      _firstVisibleRowIndex += windowOffset;
      _isDataChanged = false;
      _isSelectedRowIndexChanged = false;
    }

    void init() {
      _isInitialized = true;
      _lcd->clear();

      if (_selectedItemId == NO_ID) {
        findAndSelectEnabledItemById();
      }
      _isSelectedRowIndexChanged = true;
    }

    void close(bool reset = false) {
      _isInitialized = false;
      _isDataChanged = false;
      _isSelectedRowIndexChanged = false;

      if (reset) {
        _firstVisibleRowIndex = 0;
        _selectedRowIndex = 0;;
        _selectedItemIndex = 0;
        _selectedItemId = NO_ID;
      }

      for (uint8_t r = 0; r < _pageLayout->getRowsCount(); r++) {
        LcdPageRow* pageRow = _pageLayout->getRowByIndex(r);

        for (uint8_t i = 0; i < pageRow->getSize(); i++) {
          LiquidCrystalView* itemView = pageRow->getItemByIndex(i);
          itemView->close();
        }
      }

      _lcd->clear();
    }

    void findAndSelectEnabledItemById(uint8_t id = NO_ID) {
      for (int8_t r = 0; r < _pageLayout->getRowsCount(); r++) {
        LcdPageRow* pageRow = _pageLayout->getRowByIndex(r);

        for (int8_t i = 0; i < pageRow->getSize(); i++) {
          LiquidCrystalView* itemView = pageRow->getItemByIndex(i);

          if (itemView->isEnabled() && (itemView->getId() == id || id == NO_ID)) {
            _isSelectedRowIndexChanged = true;
            _firstVisibleRowIndex = 0;
            _selectedRowIndex = r;
            _selectedItemIndex = i;
            _selectedItemId = itemView->getId();

            return;
          }
        }
      }
    }

    void nextItem() {
      for (int8_t r = _selectedRowIndex; r < _pageLayout->getRowsCount(); r++) {
        LcdPageRow* pageRow = _pageLayout->getRowByIndex(r);
        int8_t startItemIndex = (r == _selectedRowIndex ? _selectedItemIndex : 0);

        for (int8_t i = startItemIndex; i < pageRow->getSize(); i++) {
          LiquidCrystalView* itemView = pageRow->getItemByIndex(i);

          if (_selectedRowIndex == r && _selectedItemIndex == i) {
            continue;
          }

          if (itemView->isEnabled()) {
            _isSelectedRowIndexChanged = _selectedRowIndex != r;
            _selectedRowIndex = r;
            _selectedItemIndex = i;
            _selectedItemId = itemView->getId();

            return;
          }
        }
      }
    }

    void prevItem() {
      for (int8_t r = _selectedRowIndex; r >= 0; r--) {
        LcdPageRow* pageRow = _pageLayout->getRowByIndex(r);
        int8_t startItemIndex = (r == _selectedRowIndex ? _selectedItemIndex : pageRow->getSize() - 1);

        for (int8_t i = startItemIndex; i >= 0; i--) {
          LiquidCrystalView* itemView = pageRow->getItemByIndex(i);

          if (_selectedRowIndex == r && _selectedItemIndex == i) {
            continue;
          }

          if (itemView->isEnabled()) {
            _isSelectedRowIndexChanged = _selectedRowIndex != r;
            _selectedRowIndex = r;
            _selectedItemIndex = i;
            _selectedItemId = itemView->getId();

            return;
          }
        }
      }
    }

    void notifyDataChanged() {
      _isDataChanged = true;
    }

    uint8_t getId() {
      return _id;
    }

    uint8_t getSelectedItemId() {
      return _selectedItemId;
    }

    bool getDownArrowVisiblity() {
      return _isDownArrowVisible;
    }

    void setDownArrowVisibility(bool isVisible) {
      _isDownArrowVisible = isVisible;
    }

  private:
    ILiquidCrystal* _lcd;
    uint8_t _id;
    LcdPageLayout* _pageLayout;
    bool _isDownArrowVisible;

    bool _isInitialized = false;
    bool _isDataChanged = false;
    bool _isSelectedRowIndexChanged = false;

    uint8_t _selectedRowIndex = 0;
    uint8_t _selectedItemIndex = 0;
    uint8_t _selectedItemId = NO_ID;
    uint8_t _firstVisibleRowIndex = 0;

    int8_t getMinWindowOffsetToRowIndex(uint8_t rowIndex) {
      if (rowIndex < _firstVisibleRowIndex) return rowIndex - _firstVisibleRowIndex;
      else if (rowIndex > _firstVisibleRowIndex + _lcd->getRows() - 1) return rowIndex - (_firstVisibleRowIndex + _lcd->getRows() - 1);
      else return 0;
    }
};
