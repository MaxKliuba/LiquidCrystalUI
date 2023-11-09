#define CUSTOM_VIEW_ID 1

void onDraw(ILiquidCrystal* lcd);
void onClear(ILiquidCrystal* lcd);

LcdCustomView customView(&lcdI2C, CUSTOM_VIEW_ID, 3, LCD_AUTO_VERTICALLY, onDraw, onClear);

LcdPageRow* customViewPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{ &backCharmapView, &customView }, 2),
};
LcdPageLayout customViewPageLayout = LcdPageLayout(customViewPageRows, 1);
LiquidCrystalPage customViewPage = LiquidCrystalPage(&lcdI2C, CUSTOM_VIEW_PAGE_ID, &customViewPageLayout, false);

void onDraw(ILiquidCrystal* lcd) {
  for (uint8_t i = 0; i < 2; i++) {
    lcd->setCursor(customView.getXPosition(), i);
    lcd->print(F("+-----------+"));
  }
}

void onClear(ILiquidCrystal* lcd) {
  for (uint8_t i = 0; i < 2; i++) {
    lcd->setCursor(customView.getXPosition(), i);
    for (uint8_t j = 0; j < 13; j++) {
      lcd->write(_BLANK_CHAR);
    }
  }
}
