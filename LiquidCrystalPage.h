/*
 * LiquidCrystalUI - Library
 * https://github.com/MaxKliuba/LiquidCrystalUI
 * by MaxClub Technology, 2022
 */

#pragma once

#include "ILiquidCrystal.h"
#include "LiquidCrystalCharmaps.h"
#include "LiquidCrystalView.h"

class LcdPageRow {
  public:
    LcdPageRow(LiquidCrystalView** _items, uint8_t _size) {
      size = _size;
      items = new LiquidCrystalView*[size];
      for (uint8_t i = 0; i < size; i++) {
        items[i] = _items[i];
      }
    }

    uint8_t getSize() {
      return size;
    }

    LiquidCrystalView* getItemByIndex(uint8_t index) {
      return items[index];
    }

  protected:
    LiquidCrystalView** items;
    uint8_t size;
};

class LcdPageLayout {
  public:
    LcdPageLayout(LcdPageRow** _rows, uint8_t _rowsCount) {
      rowsCount = _rowsCount;
      rows = new LcdPageRow*[rowsCount];
      for (uint8_t i = 0; i < rowsCount; i++) {
        rows[i] = _rows[i];
      }
    }

    uint8_t getRowsCount() {
      return rowsCount;
    }

    LcdPageRow* getRowByIndex(uint8_t index) {
      return rows[index];
    }

  protected:
    LcdPageRow** rows;
    uint8_t rowsCount;
};

class LiquidCrystalPage {
  public:
    LiquidCrystalPage(ILiquidCrystal* _lcd, uint8_t _id, LcdPageLayout* _pageLayout) {
      lcd = _lcd;
      id = _id;
      pageLayout = _pageLayout;
    }

    void tick() {
      if (!isInitialized) {
        init();
      }
      if (selectedItemId == -1) {
        findFirstSelectableItemId();
      }

      bool showDownArrow = true;
      for (uint8_t r = 0; r < lcd->getRows(); r++) {
        if (isDataChanged && prevSelectedRowIndex != -1) {
          uint8_t prevRowIndex = prevSelectedRowIndex >= lcd->getRows() ? r + prevSelectedRowIndex - lcd->getRows() + 1 : r;
          if (prevRowIndex < pageLayout->getRowsCount()) {
            LcdPageRow* pageRow = pageLayout->getRowByIndex(prevRowIndex);
            for (uint8_t i = 0; i < pageRow->getSize(); i++) {
              LiquidCrystalView* itemView = pageRow->getItemByIndex(i);
              itemView->setYPosition(r);
              itemView->clear();
            }
          }
        }
        uint8_t rowIndex = selectedRowIndex >= lcd->getRows() ? r + selectedRowIndex - lcd->getRows() + 1 : r;
        if (rowIndex < pageLayout->getRowsCount()) {
          LcdPageRow* pageRow = pageLayout->getRowByIndex(rowIndex);
          for (uint8_t i = 0; i < pageRow->getSize(); i++) {
            LiquidCrystalView* itemView = pageRow->getItemByIndex(i);
            itemView->setYPosition(r);
            if (itemView->getId() == selectedItemId && !itemView->isSelected()) {
              itemView->select();
            } else if ((itemView->getId() != selectedItemId && itemView->isSelected()) ||
                       prevSelectedRowIndex != selectedRowIndex || prevSelectedItemIndex != selectedItemIndex) {
              itemView->unselect();
            }
            if (itemView->getId() != selectedItemId && itemView->isEditing()) {
              itemView->endEditing();
            }
            itemView->tick();
          }
        }
        if (rowIndex == pageLayout->getRowsCount() - 1) {
          showDownArrow = false;
        }
      }

      if (isDownArrowVisible() && prevSelectedRowIndex != selectedRowIndex) {
        if (showDownArrow) {
          lcd->createChar(_DOWN_ARROW_CHARMAP_LOCATION, _DOWN_ARROW_CHARMAP);
          lcd->setCursor(lcd->getCols() - 1, lcd->getRows() - 1);
          lcd->write(_DOWN_ARROW_CHARMAP_LOCATION);
        } else {
          lcd->setCursor(lcd->getCols() - 1, lcd->getRows() - 1);
          lcd->write(' ');
        }
      }

      if (prevSelectedRowIndex != selectedRowIndex || prevSelectedItemIndex != selectedItemIndex || isDataChanged) {
        prevSelectedRowIndex = selectedRowIndex;
        prevSelectedItemIndex = selectedItemIndex;
        isDataChanged = false;
      }
    }

    void init() {
      isInitialized = true;
      lcd->clear();
    }

    void stop(bool reset = false) {
      isInitialized = false;
      isDataChanged = false;
      if (reset) {
        resetSelectedItemId();
      }
      for (int16_t r = 0; r < pageLayout->getRowsCount(); r++) {
        LcdPageRow* pageRow = pageLayout->getRowByIndex(r);
        for (int16_t i = 0; i < pageRow->getSize(); i++) {
          LiquidCrystalView* itemView = pageRow->getItemByIndex(i);
          itemView->stop();
        }
      }
      lcd->clear();
    }

    void notifyDataChanged() {
      isDataChanged = true;
    }

    void nextItem() {
      for (int8_t r = selectedRowIndex; r < pageLayout->getRowsCount(); r++) {
        LcdPageRow* pageRow = pageLayout->getRowByIndex(r);
        int8_t startItemIndex = (r == selectedRowIndex ? selectedItemIndex : 0);
        for (int8_t i = startItemIndex; i < pageRow->getSize(); i++) {
          LiquidCrystalView* itemView = pageRow->getItemByIndex(i);
          if (selectedRowIndex == r && selectedItemIndex == i) {
            continue;
          }
          if (itemView->isSelectable()) {
            prevSelectedRowIndex = selectedRowIndex;
            selectedRowIndex = r;
            prevSelectedItemIndex = selectedItemIndex;
            selectedItemIndex = i;
            selectedItemId = itemView->getId();
            notifyDataChanged();
            return;
          }
        }
      }
    }

    void prevItem() {
      for (int8_t r = selectedRowIndex; r >= 0; r--) {
        LcdPageRow* pageRow = pageLayout->getRowByIndex(r);
        int8_t startItemIndex = (r == selectedRowIndex ? selectedItemIndex : pageRow->getSize() - 1);
        for (int8_t i = startItemIndex; i >= 0; i--) {
          LiquidCrystalView* itemView = pageRow->getItemByIndex(i);
          if (selectedRowIndex == r && selectedItemIndex == i) {
            continue;
          }
          if (itemView->isSelectable()) {
            prevSelectedRowIndex = selectedRowIndex;
            selectedRowIndex = r;
            prevSelectedItemIndex = selectedItemIndex;
            selectedItemIndex = i;
            selectedItemId = itemView->getId();
            notifyDataChanged();
            return;
          }
        }
      }
    }

    void resetSelectedItemId() {
      prevSelectedRowIndex = -1;
      selectedRowIndex = -1;
      prevSelectedItemIndex = -1;
      selectedItemIndex = -1;
      selectedItemId = -1;
    }

    uint8_t getId() {
      return id;
    }

    uint8_t getSelectedItemId() {
      return selectedItemId;
    }

    bool isDownArrowVisible() {
      return isDownArrowVisibleFlag;
    }

    void setDownArrowVisibility(bool isVisible) {
      isDownArrowVisibleFlag = isVisible;
    }

  private:
    ILiquidCrystal* lcd;

    uint8_t id;
    LcdPageLayout *pageLayout;

    bool isInitialized = false;
    bool isDataChanged = false;
    int8_t prevSelectedRowIndex = -1;
    int8_t selectedRowIndex = -1;
    int8_t prevSelectedItemIndex = -1;
    int8_t selectedItemIndex = -1;
    int8_t selectedItemId = -1;
    bool isDownArrowVisibleFlag = true;

    void findFirstSelectableItemId() {
      for (int8_t r = 0; r < pageLayout->getRowsCount(); r++) {
        LcdPageRow* pageRow = pageLayout->getRowByIndex(r);
        for (int8_t i = 0; i < pageRow->getSize(); i++) {
          LiquidCrystalView* itemView = pageRow->getItemByIndex(i);
          if (itemView->isSelectable()) {
            prevSelectedRowIndex = selectedRowIndex;
            selectedRowIndex = r;
            prevSelectedItemIndex = selectedItemIndex;
            selectedItemIndex = i;
            selectedItemId = itemView->getId();
            notifyDataChanged();
            return;
          }
        }
      }
    }
};
