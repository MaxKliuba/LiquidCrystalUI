/*
 * LiquidCrystalUI - Library
 * https://github.com/MaxKliuba/LiquidCrystalUI
 * by MaxClub Technology, 2022
 */

#pragma once

#include "ILiquidCrystal.h"
#include "LiquidCrystalCharmaps.h"
#include "LiquidCrystalView.h"
#include "LiquidCrystalPage.h"

class LiquidCrystalPager {
  public:
    LiquidCrystalPager(LiquidCrystalPage** _pages, uint8_t _size) {
      size = _size;
      pages = new LiquidCrystalPage*[size];
      for (uint8_t i = 0; i < size; i++) {
        pages[i] = _pages[i];
      }
    }

    void tick() {
      if (!isInitialized) {
        init();
      }

      if (prevPageId != currentPageId && prevPageId != -1) {
        for (uint8_t i = 0; i < size; i++) {
          LiquidCrystalPage* page = pages[i];
          if (page->getId() == prevPageId) {
            prevPageId = currentPageId;;
            page->stop();
            break;
          }
        }
      }

      for (uint8_t i = 0; i < size; i++) {
        LiquidCrystalPage* page = pages[i];
        if (page->getId() == currentPageId) {
          if (isDataChanged) {
            isDataChanged = false;
            page->stop();
          }
          page->tick();
        }
      }
    }

    void init() {
      isInitialized = true;
      isDataChanged = false;
      if (size > 0) {
        prevPageId = -1;
        currentPageId = pages[0]->getId();
      }
    }

    void stop() {
      isInitialized = false;
      isDataChanged = false;
      for (uint8_t i = 0; i < size; i++) {
        LiquidCrystalPage* page = pages[i];
        if (page->getId() == currentPageId) {
          page->stop();
          break;
        }
      }
      prevPageId = -1;
      currentPageId = -1;
    }

    void notifyDataChanged() {
      isDataChanged = true;
    }

    void navigateTo(uint8_t pageId) {
      for (uint8_t i = 0; i < size; i++) {
        LiquidCrystalPage* page = pages[i];
        if (page->getId() == pageId) {
          prevPageId = currentPageId;
          currentPageId = pageId;
          return;
        }
      }
    }

    int8_t getCurrentPageId() {
      return currentPageId;
    }

  protected:
    LiquidCrystalPage** pages;
    uint8_t size;

    bool isInitialized = false;
    bool isDataChanged = false;
    int8_t prevPageId = -1;
    int8_t currentPageId = -1;
};
