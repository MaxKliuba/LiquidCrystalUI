/*
   LiquidCrystalUI - Library
   https://github.com/MaxKliuba/LiquidCrystalUI
   by MaxClub Technology, 2022
*/

#include <EncButton.h>

#include <LiquidCrystalCharmaps.h>
#include <LiquidCrystalI2CWrapper.h>
#include <LiquidCrystalView.h>
#include <LiquidCrystalPage.h>
#include <LiquidCrystalPager.h>

#define LCD_ADDRESS 0x3f // 0x3f or 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystalI2CWrapper lcdI2C(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

#define MAIN_PAGE_ID 0
#define DATETIME_PAGE_ID 1
#define DETAILS_PAGE_ID 2
#define MENU_PAGE_ID 3
#define SET_VALUES_PAGE_ID 4
#define PROGRESSBAR_PAGE_ID 5
#define GRAPH_PAGE_ID 6
#define CUSTOM_VIEW_PAGE_ID 7
#define RESET_VALUES_PAGE_ID 8

#define MENU_CHARMAP_VIEW_ID 0
#define BACK_CHARMAP_VIEW_ID 0

LcdMenuCharmapView menuCharmapView(&lcdI2C, MENU_CHARMAP_VIEW_ID, LCD_START_HORIZONTALLY, LCD_AUTO_VERTICALLY);
LcdBackCharmapView backCharmapView(&lcdI2C, BACK_CHARMAP_VIEW_ID, LCD_START_HORIZONTALLY, LCD_AUTO_VERTICALLY);

struct Values {
  byte textValue = 0;
  byte charValue = 48;
  bool switchValue = false;
  bool checkBoxValue = false;
  byte progressBarValue = 0;
  uint8_t graphValues[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
};

struct {
  bool editTitleOnPageInit = false;
  Values values;
} state;

#include "MainPageUI.h"
#include "DatetimePageUI.h"
#include "DetailsPageUI.h"
#include "MenuPageUI.h"
#include "SetValuesPageUI.h"
#include "ProgressBarPageUI.h"
#include "GraphPageUI.h"
#include "CustomViewPageUI.h"
#include "ResetValuesPageUI.h"

LiquidCrystalPage* pages[] = {
  &mainPage,
  &datetimePage,
  &detailsPage,
  &menuPage,
  &setValuesPage,
  &progressBarPage,
  &graphPage,
  &customViewPage,
  &resetValuesPage,
};
LiquidCrystalPager pager = LiquidCrystalPager(pages, 9);

EncButton<EB_TICK, 2, 3, 4> enc(INPUT);

void setup() {
  lcdI2C.init();
  lcdI2C.setBacklight(true);

  mainTitleView.setText(F("|MAIN>"));

  datetimeTitleView.setText(F("<DATE&TIME>"));

  detailsTitleView.setText(F("<DETAILS|"));

  menuTitleView.setText(F("|MENU|"));
  setValuesMenuItemView.setText(F("Set values"));
  progressBarMenuItemView.setText(F("Progress bar"));
  graphMenuItemView.setText(F("Graph"));
  customViewMenuItemView.setText(F("Custom view"));
  resetValuesMenuItemView.setText(F("Reset values"));

  setValuesTitleView.setText(F("|SET VALUES|"));
  textValueLabelView.setText(F("Text:"));
  charValueLabelView.setText(F("Char:"));
  switchLabelView.setText(F("switch"));
  checkBoxLabelView.setText(F("checkbox"));

  progressBarTitleView.setText(F("|PROGRESS BAR|"));

  graphTitleView.setText(F("|GRAPH|"));

  resetValuesTitleView.setText(F("RESET?"));
  resetValuesOkView.setText(F("[OK]"));
}

void loop() {
  enc.tick();

  pager.tick();
  processPages();
}

void shiftArrayLeft(byte arr[], byte length) {
  byte firstByte = arr[0];
  for (int i = 0; i < length - 1; i++) {
    arr[i] = arr[i + 1];
  }
  arr[length - 1] = firstByte;
}

void shiftArrayRight(byte arr[], byte length) {
  byte lastByte = arr[length - 1];
  for (int i = length - 1; i > 0; i--) {
    arr[i] = arr[i - 1];
  }
  arr[0] = lastByte;
}
