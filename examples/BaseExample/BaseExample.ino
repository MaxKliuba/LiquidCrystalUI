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

#define MENU_PAGE_ID 0 // Unique page IDs
// #define ...PAGE_ID 1
// ...

#include "MenuPageUI.h"
// #include "...PageUI.h"
// ...

LiquidCrystalPage* pages[] = {
  &menuPage,
  // &...Page,
  // ...
};
LiquidCrystalPager pager = LiquidCrystalPager(pages, 1); // Size of `pages`

EncButton<EB_TICK, 2, 3, 4> enc(INPUT);

void setup() {
  lcdI2C.init();
  lcdI2C.setBacklight(true);

  menuTitleView.setText(F("|MENU|"));
  itemView1.setText(F("Item 1"));
  itemView2.setText(F("Item 2"));
  itemView3.setText(F("Item 3"));
  itemView4.setText(F("Item 4"));
  itemView5.setText(F("Item 5"));
  itemView6.setText(F("Item 6"));
  itemView7.setText(F("Item 7"));
  itemView8.setText(F("Item 8"));
  itemView9.setText(F("Item 9"));
  itemView10.setText(F("Item 10"));
}

void loop() {
  enc.tick();

  pager.tick();
  processPages();
}
