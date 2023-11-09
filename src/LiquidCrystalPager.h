/*
 * LiquidCrystalUI - Library
 * https://github.com/MaxKliuba/LiquidCrystalUI
 * by MaxClub Technology, 2022
 */

#pragma once

#include "LiquidCrystalPage.h"

class LiquidCrystalPager {
  public:
    LiquidCrystalPager(LiquidCrystalPage** pages, uint8_t size) {
      _size = size;
      _pages = pages;
    }

    void tick() {
      if (!_isInitialized) {
        init();
      }

      if (_prevPageId != _currentPageId && _prevPageId != -1) {
        for (uint8_t i = 0; i < _size; i++) {
          LiquidCrystalPage* page = _pages[i];
          
          if (page->getId() == _prevPageId) {
            _prevPageId = _currentPageId;;
            page->close();
            
            break;
          }
        }
      }

      for (uint8_t i = 0; i < _size; i++) {
        LiquidCrystalPage* page = _pages[i];
        
        if (page->getId() == _currentPageId) {
          if (_isDataChanged) {
            _isDataChanged = false;
            page->close();
          }
          
          page->tick();
        }
      }
    }

    void init() {
      _isInitialized = true;
      _isDataChanged = false;
      
      _prevPageId = -1;
      _currentPageId = _size > 0 ? _pages[0]->getId() : -1;
    }

    void close() {
      _isInitialized = false;
      _isDataChanged = false;
      
      for (uint8_t i = 0; i < _size; i++) {
        LiquidCrystalPage* page = _pages[i];
        
        if (page->getId() == _currentPageId) {
          page->close();
          
          break;
        }
      }
      
      _prevPageId = -1;
      _currentPageId = -1;
    }

    void notifyDataChanged() {
      _isDataChanged = true;
    }

    void navigateTo(uint8_t pageId) {
      for (uint8_t i = 0; i < _size; i++) {
        LiquidCrystalPage* page = _pages[i];
        
        if (page->getId() == pageId) {
          _prevPageId = _currentPageId;
          _currentPageId = pageId;
          
          return;
        }
      }
    }

    int8_t getCurrentPageId() {
      return _currentPageId;
    }

  private:
    LiquidCrystalPage** _pages;
    uint8_t _size;

    bool _isInitialized = false;
    bool _isDataChanged = false;
    int8_t _prevPageId = -1;
    int8_t _currentPageId = -1;
};
